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
	struct ImageBroadcast cameraFlux;
	struct ImageBroadcast processedFlux;
	//CvFont * font = (CvFont *)malloc(sizeof(CvFont));
	CvRect workingSpace;
	Stylus stylus;
	char colourTrackingWindow[] = "Color Tracking";
	char maskWindow[] = "Mask";
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
	initImageBroadcast(&processedFlux, NULL, &workingSpace, maskWindow, NULL);
	if(loadFile == NULL){
		initTrackedObject(&birdTracker,0,0,0,&cameraFlux,&processedFlux,cvRect(((cameraFlux.img->roi->width/3) - (WIDTH_BIRD_TRACKING_ZONE/2)),0,WIDTH_BIRD_TRACKING_ZONE,cameraFlux.img->roi->height),RECTANGLE);
    } else {		/* We load data form the file */
		loadTrackedObject(&birdTracker,&cameraFlux,&processedFlux,loadFile);
	}
    
    
    cvSetMouseCallback(colourTrackingWindow, getObjectColor,&birdTracker);
	int exit =0;
	do {
		loadImage(&cameraFlux,capture);
		updateTracking(&birdTracker);
		
		char key = cvWaitKey(1);
		
		switch(key)	
		{
			case 32:		/* space to click */
				click(&stylus);
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
		saveTrackedObject(&birdTracker,saveFile);
		fclose(saveFile);
	}
    cvDestroyAllWindows();
	raspiCamCvReleaseCapture(&capture);
	disable(&stylus);
	
	return 0; 
}


