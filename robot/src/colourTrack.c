#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>
#include <math.h>
#include <cv.h>
#include <highgui.h>
#include <sys/time.h>
#include "RaspiCamCV.h"

#include "configuration.h"
#include "stylus.h"
#include "imageBroadcast.h"
#include "tracking.h"
#include "imageProcessing.h"

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
	struct TrackedObject pipeTracker1;
	struct TrackedObject pipeTracker2;
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
	
	while((c = getopt(argc,argv,"l:s:")) != -1)
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
	if(loadFile == NULL){
		int width = cameraFlux.img->roi->width;
		int height = cameraFlux.img->roi->height;
		initTrackedObject(&birdTracker,0,0,0,&cameraFlux,&birdBinFlux,cvRect(((width/3) - (width*RELATIVE_WIDTH_BIRD_TRACKING_ZONE/2)),0,width*RELATIVE_WIDTH_BIRD_TRACKING_ZONE,height),RECTANGLE);
		initTrackedObject(&pipeTracker1,0,0,0,&cameraFlux,&pipeBinFlux,cvRect((width/3) - (width*RELATIVE_WIDTH_BIRD_TRACKING_ZONE/2),0,width*RELATIVE_WIDTH_PIPE_TRACKING_ZONE,height),RECTANGLE);
		initTrackedObject(&pipeTracker2,0,0,0,&cameraFlux,&pipeBinFlux,cvRect(pipeTracker1.trackingZone.x + pipeTracker1.trackingZone.width - (int)(width*RELATIVE_WIDTH_BIRD_TRACKING_ZONE/2),0,width*RELATIVE_WIDTH_PIPE_TRACKING_ZONE,height),RECTANGLE);
    } else {		/* We load data form the file */
		loadTrackedObject(&birdTracker,&cameraFlux,&birdBinFlux,loadFile);
		loadTrackedObject(&pipeTracker1,&cameraFlux,&pipeBinFlux,loadFile);
		loadTrackedObject(&pipeTracker2,&cameraFlux,&pipeBinFlux,loadFile);
	}
    
    
	int exit =0;
	do {
		loadImage(&cameraFlux,capture);
		updateTracking(&birdTracker);
		updateTracking(&pipeTracker1);
		updateTracking(&pipeTracker2);
		showImage(&cameraFlux);
		printf("pipe : h%f w%f ; bird : h%f,w%f\n",getRelativeDistance(&pipeTracker1,UP),getRelativeDistance(&pipeTracker1,LEFT)/getRelativeDistance(&birdTracker,RIGHT),getRelativeDistance(&birdTracker,UP),getRelativeDistance(&birdTracker,LEFT));
		
		char key = cvWaitKey(1);
		
		switch(key)	
		{
			case 'b':		/* b to select the color of the bird */
				cvSetMouseCallback(colourTrackingWindow, getObjectColor,&birdTracker);
				break;
			case 'p':		/* p to select the color of the pipe */
				cvSetMouseCallback(colourTrackingWindow, getObjectColor,&pipeTracker1);
				break;
			case 'q':		/* q to select the color of the pipe */
				cvSetMouseCallback(colourTrackingWindow, getObjectColor,&pipeTracker2);
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
		saveTrackedObject(&pipeTracker1,saveFile);
		saveTrackedObject(&pipeTracker2,saveFile);
		fclose(saveFile);
	}
    cvDestroyAllWindows();
    /* Release memory */
    releaseTrackingImageMemory(&birdTracker);
	releaseTrackingImageMemory(&pipeTracker1);
	releaseTrackingImageMemory(&pipeTracker2);
	raspiCamCvReleaseCapture(&capture);
	
	/* disabling servomotor */
	disable(&stylus);
	
	return 0; 
}


