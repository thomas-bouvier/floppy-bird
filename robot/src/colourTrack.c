#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>
#include <math.h>
#include <cv.h>
#include <highgui.h>
#include <sys/time.h>
#include <time.h>
#include <pthread.h>
#include "RaspiCamCV.h"

#include "configuration.h"
#include "stylus.h"
#include "imageBroadcast.h"
#include "tracking.h"
#include "imageProcessing.h"
#include "pipeTracking.h"
#include "ia.h"

/*!
* \brief init capture function : init and configure the capture
*/
RaspiCamCvCapture * initCapture(){
	RASPIVID_CONFIG * config = (RASPIVID_CONFIG*)malloc(sizeof(RASPIVID_CONFIG));
	
	config->width = CAPTURE_WIDTH;
	config->height = CAPTURE_HEIGHT;
	config->bitrate = 0;	/* zero: leave as default */
	config->framerate = 0;
	config->monochrome = 0;
	
	RaspiCamCvCapture * capture = (RaspiCamCvCapture *) raspiCamCvCreateCameraCapture2(0, config); 
	free(config);
	return capture;
}

int main(int argc, char *argv[]){
		
	/* Image */
	RaspiCamCvCapture * capture = initCapture();			/* The capture = video from the camera */
	struct ImageBroadcast cameraFlux;				/* The raw image from the camera */
	struct ImageBroadcast birdBinFlux;				/* The binairy images resulting from the bird tracking */
	struct ImageBroadcast pipeBinFlux;				/* The binary images resulting from the pipe dynamic tracking */
	CvRect workingSpace;							/* The area defining the limits of the screen */
	
	/* trackers*/
	struct TrackedObject birdTracker;						/* The tracker for te bird */
	struct TrackedObject pipeTracker[NB_PIPE_TRACKER];		/* An array of pipe trackers used to compute the dynamic tracking */
	struct PipeDynamicTracker pipeDynTracker;				/* The pipe dynamic tracker */
	
	/* Mecanic */
	Stylus stylus;			/* The stylus actuated by the servo */
	Robot robot;			/* The robot gathers everything needed to interface the IA with the tracking */
	
	/* Thread */
	pthread_t iaThread;				/* the thread for the ia */
	int retIaThread = -1;			/* the return value when creating the ia thread */					
	
	/* Files */
	int c;		/* Used to test which options are passed through argv */
	FILE* loadFile = NULL;		/* The load File pointer */
	FILE* saveFile = NULL;		/* The save File pointer */
	FILE* logFile = NULL;		/* The log File pointer */
	boolean verbose = false;	/* Tells if we want to display informations on the console */
	
	while((c = getopt(argc,argv,"l:s:d:iv")) != -1)
		switch(c){
			case 'l':		/* -l loadFileName : load working space and structures (trackers...) from the load File*/
				loadFile = fopen(optarg,"rb");
				if(saveFile!=NULL){
					fprintf(stderr,"cannot load and save a file at the same time");
					return 1;
				}
				break;
			case 's':		/* -s saveFileName : save working space and structures (trackers...) into the save File */
				saveFile = fopen(optarg,"wb");
				if(loadFile!=NULL){
					fprintf(stderr,"cannot load and save a file at the same time");
					return 1;
				}
				break;
			case 'd':		/* -d logFileName : Save data into a log file */
				logFile = fopen(optarg,"w");
				if(logFile==NULL){
					fprintf(stderr,"cannot open logfile %s\n",optarg);
					return 1;
				}
				break;
			case 'i':		/* -i : run the IA */
				printf("Creating a thread for the IA\n");
				retIaThread = pthread_create (&iaThread, NULL, mainIa,&robot); 
				if(retIaThread == 0)
					printf("Thread created\n");
				else 
					fprintf (stderr, "%s", strerror (retIaThread));
				break;
			case 'v':		/* -v : verbose mode */
				verbose = true;
				break;
			default:
				printf("Unknown option :\nUsage : \n\t-l loadFileName\n\t-s saveFileName\n\t-d logFileName\n");
				return 1;
		}
	
	
	/* Setup */
	
	/* init the stylus */
	wiringPiSetup();	/* Setup the GPIO */
	attach(&stylus,PWM_PIN,STYLUS_CLICK_POSITION,STYLUS_REST_POSITION,PRESS_DELAY,REST_DELAY);
	enable(&stylus);
	
	/* init working space and flux */
	workingSpace = initWorkSpace(capture, "Working Space Definition",loadFile);
	initImageBroadcast(&cameraFlux, NULL, &workingSpace, "Color Tracking", NULL);
	loadImage(&cameraFlux,capture);
	initImageBroadcast(&birdBinFlux, NULL, &workingSpace, "Bird tracking", NULL);
	initImageBroadcast(&pipeBinFlux, NULL, &workingSpace, "Pipe tracking", NULL);
	
	/* init trackers */
	int i;
	if(loadFile == NULL){	/* init trackers with default parameters */
		int width = cameraFlux.img->roi->width;
		int height = cameraFlux.img->roi->height;
		initTrackedObject(&birdTracker,0,0,0,&cameraFlux,&birdBinFlux,cvRect(((width/3) - (width*RELATIVE_WIDTH_BIRD_TRACKING_ZONE/2)),0,width*RELATIVE_WIDTH_BIRD_TRACKING_ZONE,height),RECTANGLE);
		for(i = 0; i < NB_PIPE_TRACKER; i++){
			initTrackedObject(&pipeTracker[i],0,0,0,&cameraFlux,&pipeBinFlux,cvRect(width - (width*RELATIVE_WIDTH_BIRD_TRACKING_ZONE) -1,0,width*RELATIVE_WIDTH_PIPE_TRACKING_ZONE,height),RECTANGLE);
		}
		initPipeDynamicTracker(&pipeDynTracker, pipeTracker);
    } else {		/* init trackers from the load File */
		loadTrackedObject(&birdTracker,&cameraFlux,&birdBinFlux,loadFile);
		for(i = 0; i < NB_PIPE_TRACKER; i++){
			loadTrackedObject(&pipeTracker[i],&cameraFlux,&pipeBinFlux,loadFile);
		}
		initPipeDynamicTracker(&pipeDynTracker, pipeTracker);
	}
	
	/* init the robot */
    initRobot(&robot, &stylus);
    
    /* init var used in the main loop */
    if(logFile != NULL){
		fprintf(logFile,"Time (us);Bird height;Pipe height;Bird - Pipe relative distance\n");
	}
	
	int exit =0;
	struct timeval startTime, lastTime, currentTime;
	gettimeofday(&startTime,NULL);
	gettimeofday(&lastTime,NULL);
	
	/* main loop */
	do {
		/* Update tracking */
		loadImage(&cameraFlux,capture);
		updateTracking(&birdTracker);
		updatePipeDynamicTracker(&pipeDynTracker);
		showImage(&cameraFlux);
		
		/* Time computing */
		if(verbose){
			gettimeofday(&currentTime,NULL);		/* update the time */
			long int frameTime = (currentTime.tv_sec - lastTime.tv_sec)*1000000+currentTime.tv_usec- lastTime.tv_usec;
			gettimeofday(&lastTime,NULL);
			float frameRate = 1000000.0/frameTime;
			printf("FPS : %f \t",frameRate);
		}
		
		/* Data processing */
		CvPoint pipe = nextPipe(&pipeDynTracker,birdTracker.origin.x - birdTracker.width/2);
		setBirdHeight(&robot, getRelativeDistance(&birdTracker,UP));
		setNextPipeHeight(&robot, 1-((float)pipe.y/(pipeBinFlux.img->height)));
		setNextPipePosition(&robot, (float)pipe.x/birdTracker.origin.x);
		if(verbose)
			printf("pipe : h%f w%f ; bird : h%f\n",getNextPipeHeight(&robot),getNextPipePosition(&robot),getBirdHeight(&robot));
		if(logFile != NULL){
			fprintf(logFile,"%ld;%f;%f;%f\n",(long int)((currentTime.tv_sec - startTime.tv_sec)*1000000+currentTime.tv_usec- startTime.tv_usec),getBirdHeight(&robot),getNextPipeHeight(&robot),getNextPipePosition(&robot));
		}
		
		/* keyboard functions */
		char key = cvWaitKey(1);
		
		switch(key)	
		{
			case 'b':		/* b to select the color of the bird */
				cvSetMouseCallback("Color Tracking", getObjectColor,&birdTracker);
				break;
			case 'p':		/* p to select the color of the pipe */
				cvSetMouseCallback("Color Tracking", getPipeColor,&pipeDynTracker);
				break;
			case 32:		/* space to click */
				click(&stylus);
				printf("click\n");
				break;
			case 27:		/* Esc to exit */
				exit = 1;
				break;
		}
		
		/* update servo */
		update(&stylus);
		
	} while (!exit);

	/* Thread cancel */
	pthread_cancel (iaThread);

	/* close and /  or save files */ 
	if(loadFile != NULL)
		fclose(loadFile);
		
	if(saveFile != NULL){
		saveWorkingSpace(&workingSpace,saveFile);
		saveTrackedObject(&birdTracker,saveFile);
		for(i = 0; i < NB_PIPE_TRACKER; i++){
			saveTrackedObject(&pipeTracker[i],saveFile);
		}
		fclose(saveFile);
	}
	
	if(logFile != NULL)
		fclose(logFile);
		
    /* Release memory */
    releaseTrackingImageMemory(&birdTracker);
    for(i = 0; i < NB_PIPE_TRACKER; i++){
		releaseTrackingImageMemory(&pipeTracker[i]);
	}
	raspiCamCvReleaseCapture(&capture);
	cvDestroyAllWindows();
	
	/* disable servo */
	disable(&stylus);
	
	return 0; 
}


