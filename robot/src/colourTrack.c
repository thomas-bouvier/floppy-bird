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

// Maths methods
#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))  
#define abs(x) ((x) > 0 ? (x) : -(x))
#define sign(x) ((x) > 0 ? 1 : -1)

// Color tracked and our tolerance towards it
int h = 0, s = 0, v = 0;
IplImage* image;

RaspiCamCvCapture * initCapture(){
	RASPIVID_CONFIG * config = (RASPIVID_CONFIG*)malloc(sizeof(RASPIVID_CONFIG));
	
	config->width = CAPTURE_WIDTH;
	config->height = CAPTURE_HEIGHT;
	config->bitrate = 0;	// zero: leave as default
	config->framerate = 0;
	config->monochrome = 0;
	
	RaspiCamCvCapture * capture = (RaspiCamCvCapture *) raspiCamCvCreateCameraCapture2(0, config); 
	free(config);
	return capture;
}

int main(int argc, char *argv[]){
	
	// Variables
	int nbPixels;
	RaspiCamCvCapture * capture = initCapture();
	CvFont * font = (CvFont *)malloc(sizeof(CvFont));
	CvRect workingSpace;
	Stylus stylus;
	char colourTrackingWindow[] = "Color Tracking";
	char maskWindow[] = "Mask";
	char workSpaceDefWindow[] = "WorkingSpaceDefinition";
	
	// Setup
	wiringPiSetup();	// Setup the GPIO
	attach(&stylus,PWM_PIN,STYLUS_CLICK_POSITION,STYLUS_REST_POSITION,PRESS_DELAY,REST_DELAY);
	enable(&stylus);
	initFont(font);
	workingSpace = initWorkSpace(capture, workSpaceDefWindow);

	
	cvNamedWindow(colourTrackingWindow, CV_WINDOW_AUTOSIZE);
    cvNamedWindow(maskWindow, CV_WINDOW_AUTOSIZE);
    cvMoveWindow(maskWindow, 650, 100);
    cvMoveWindow(colourTrackingWindow, 0, 100);
    
    cvSetMouseCallback(colourTrackingWindow, getObjectColor,NULL);
	int exit =0;
	do {
		image = raspiCamCvQueryFrame(capture);
		cvSetImageROI(image,workingSpace);
		
		CvPoint position = binarisation(image, &nbPixels, maskWindow);
        addObjectToVideo(colourTrackingWindow, image, RECTANGLE, position,BIRD_CIRCLE_DIAMETER,BIRD_CIRCLE_DIAMETER, nbPixels);
		
		char key = cvWaitKey(1);
		
		switch(key)	
		{
			case 32:		// space to click
				click(&stylus);
				break;
			case 27:		// Esc to exit
				exit = 1;
				break;
		}
		update(&stylus);
		
	} while (!exit);

	
    cvDestroyAllWindows();
	raspiCamCvReleaseCapture(&capture);
	disable(&stylus);
	
	return 0; 
}


