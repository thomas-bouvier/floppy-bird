#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>
#include <math.h>
#include <cv.h>
#include <highgui.h>
#include <sys/time.h>
#include <time.h>
#include "RaspiCamCV.h"

#include "configuration.h"
#include "stylus.h"
#include "imageBroadcast.h"
#include "tracking.h"
#include "imageProcessing.h"
#include "pipeTracking.h"

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
	/* Variables*/
	RaspiCamCvCapture * capture = initCapture();
	struct TrackedObject birdTracker;
	struct TrackedObject* pipeTracker = (struct TrackedObject *)malloc(NB_PIPE_TRACKER*sizeof(struct TrackedObject));
	struct PipeDynamicTracker pipeDynTracker;
	struct ImageBroadcast cameraFlux;
	struct ImageBroadcast birdBinFlux;
	struct ImageBroadcast pipeBinFlux;
	//CvFont * font = (CvFont *)malloc(sizeof(CvFont));
	CvRect workingSpace;
	Stylus stylus;
	char colourTrackingWindow[] = "Color Tracking";
	char birdWindow[] = "Bird tracking";
	char pipeWindow[] = "Pipe tracking";
	char workSpaceDefWindow[] = "WorkingSpaceDefinition";
	
	int c;
	FILE* loadFile = NULL;
	FILE* saveFile = NULL;
	FILE* logFile = NULL;
	
	while((c = getopt(argc,argv,"l:s:d:")) != -1)
		switch(c){
			case 'l':
				loadFile = fopen(optarg,"rb");
				if(saveFile!=NULL){
					fprintf(stderr,"cannot load and save a file at the same time");
					return 1;
				}
				break;
			case 's':
				saveFile = fopen(optarg,"wb");
				if(loadFile!=NULL){
					fprintf(stderr,"cannot load and save a file at the same time");
					return 1;
				}
				break;
			case 'd':		/* Save data into a log file */
				logFile = fopen(optarg,"w");
				if(logFile==NULL){
					fprintf(stderr,"cannot open logfile %s\n",optarg);
					return 1;
				}
				break;
			default:
				printf("Unknown option :\nUsage : -l loadFileName OR -s saveFileName\n");
				return 1;
		}
	
	
	/* Setup */
	wiringPiSetup();	/* Setup the GPIO */
	attach(&stylus,PWM_PIN,STYLUS_CLICK_POSITION,STYLUS_REST_POSITION,PRESS_DELAY,REST_DELAY);
	enable(&stylus);
	//initFont(font);
	workingSpace = initWorkSpace(capture, workSpaceDefWindow,loadFile);

	initImageBroadcast(&cameraFlux, NULL, &workingSpace, colourTrackingWindow, NULL);
	loadImage(&cameraFlux,capture);
	initImageBroadcast(&birdBinFlux, NULL, &workingSpace, birdWindow, NULL);
	initImageBroadcast(&pipeBinFlux, NULL, &workingSpace, pipeWindow, NULL);
	int i;
	if(loadFile == NULL){
		int width = cameraFlux.img->roi->width;
		int height = cameraFlux.img->roi->height;
		initTrackedObject(&birdTracker,0,0,0,&cameraFlux,&birdBinFlux,cvRect(((width/3) - (width*RELATIVE_WIDTH_BIRD_TRACKING_ZONE/2)),0,width*RELATIVE_WIDTH_BIRD_TRACKING_ZONE,height),RECTANGLE);
		for(i = 0; i < NB_PIPE_TRACKER; i++){
			initTrackedObject(&pipeTracker[0]+i*sizeof(TrackedObject*),0,0,0,&cameraFlux,&pipeBinFlux,cvRect(width - (width*RELATIVE_WIDTH_BIRD_TRACKING_ZONE) -1,0,width*RELATIVE_WIDTH_PIPE_TRACKING_ZONE,height),RECTANGLE);
		}
		initPipeDynamicTracker(&pipeDynTracker, &pipeTracker);
    } else {		/* We load data form the file */
		loadTrackedObject(&birdTracker,&cameraFlux,&birdBinFlux,loadFile);
		for(i = 0; i < NB_PIPE_TRACKER; i++){
			loadTrackedObject(&pipeTracker[0]+i*sizeof(TrackedObject*),&cameraFlux,&pipeBinFlux,loadFile);
		}
		initPipeDynamicTracker(&pipeDynTracker, &pipeTracker);
	}
    
    if(logFile != NULL){
		fprintf(logFile,"Time (us);Bird height;Pipe height;Bird - Pipe relative distance\n");
	}
	int exit =0;
	struct timeval startTime, lastTime, currentTime;
	gettimeofday(&startTime,NULL);
	gettimeofday(&lastTime,NULL);
	printf("initOK\n");
	do {
		loadImage(&cameraFlux,capture);
		updateTracking(&birdTracker);
		updatePipeDynamicTracker(&pipeDynTracker);
		/* Time computing */
		gettimeofday(&currentTime,NULL);		/* update the time */
		long int frameTime = (currentTime.tv_sec - lastTime.tv_sec)*1000000+currentTime.tv_usec- lastTime.tv_usec;
		gettimeofday(&lastTime,NULL);
		float frameRate = 1000000.0/frameTime;
		printf("FPS : %f \t",frameRate);
		showImage(&cameraFlux);
		CvPoint pipe = nextPipe(&pipeDynTracker,birdTracker.origin.x - birdTracker.width/2);
		float birdHeight = getRelativeDistance(&birdTracker,UP);
		float pipeHeight = 1-((float)pipe.y/(pipeBinFlux.img->height)); 
		float pipeBirdDist = (float)pipe.x/birdTracker.origin.x;
		printf("pipe : h%f w%f ; bird : h%f\n",pipeHeight,pipeBirdDist,birdHeight);
		if(logFile != NULL){
			fprintf(logFile,"%ld;%f;%f;%f\n",(long int)((currentTime.tv_sec - startTime.tv_sec)*1000000+currentTime.tv_usec- startTime.tv_usec),birdHeight,pipeHeight,pipeBirdDist);
		}
		
		char key = cvWaitKey(1);
		
		switch(key)	
		{
			case 'b':		/* b to select the color of the bird */
				cvSetMouseCallback(colourTrackingWindow, getObjectColor,&birdTracker);
				break;
			case 'p':		/* p to select the color of the pipe */
				cvSetMouseCallback(colourTrackingWindow, getPipeColor,&pipeDynTracker);
				break;
			case 32:		/* space to click */
				click(&stylus);
				printf("click\n");
				break;
			case 27:		/* Esc to exit */
				exit = 1;
				break;
		}
		update(&stylus);
		
	} while (!exit);

	if(loadFile != NULL)
		fclose(loadFile);
	if(saveFile != NULL){
		saveWorkingSpace(&workingSpace,saveFile);
		saveTrackedObject(&birdTracker,saveFile);
		for(i = 0; i < NB_PIPE_TRACKER; i++){
			saveTrackedObject(&pipeTracker[0]+i*sizeof(TrackedObject*),saveFile);
		}
		fclose(saveFile);
	}
	if(logFile != NULL)
		fclose(logFile);
    cvDestroyAllWindows();
    /* Release memory */
    releaseTrackingImageMemory(&birdTracker);
    for(i = 0; i < NB_PIPE_TRACKER; i++){
		releaseTrackingImageMemory(&pipeTracker[0]+i*sizeof(TrackedObject*));
	}
	free(pipeTracker);
	raspiCamCvReleaseCapture(&capture);
	
	/* disabling servomotor */
	disable(&stylus);
	
	return 0; 
}


