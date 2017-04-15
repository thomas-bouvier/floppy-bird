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
int h = 0, s = 0, v = 0, Htolerance = 5, Stolerance = 30;
IplImage* image;


enum {CIRCLE,RECTANGLE};

struct VolatileRect{
	CvRect rect;
	int origineDefined;
	int rectDefined;
};
 
/*
 * Transform the image into a two colored image, one color for the color we want to track, another color for the others colors
 * From this image, we get two datas : the number of pixel detected, and the center of gravity of these pixel
 */
CvPoint binarisation(IplImage* image, int *nbPixels, char* window) {
 
    int x, y;
    IplImage *hsv, *mask;
    IplConvKernel *kernel;
    int sommeX = 0, sommeY = 0;
    *nbPixels = 0;
	
	int zoneWidth = 50;		// The zone width in wich the colour will be tracked
	CvRect roi = cvRect(((image->roi->width/3) - (zoneWidth/2)),0,zoneWidth,image->roi->height);
	
    // Create the mask &initialize it to white (no color detected)
    mask = cvCreateImage(cvGetSize(image), image->depth, 1);
	
    // Create the hsv image
    hsv = cvCloneImage(image);
    cvCvtColor(image, hsv, CV_BGR2HSV);
 
	
	
    // We create the mask
    cvInRangeS(hsv, cvScalar(h - Htolerance -1, s - Stolerance, 0,0), cvScalar(h + Htolerance -1, s + Stolerance, 255,0), mask);
	
    // Create kernels for the morphological operation
    kernel = cvCreateStructuringElementEx(5, 5, 2, 2, CV_SHAPE_RECT,NULL);
 
    // Morphological opening (inverse because we have white pixels on black background)
    cvDilate(mask, mask, kernel, 2);
    cvErode(mask, mask, kernel, 2);  
    cvSetImageROI(mask,roi);
	
    // We go through the mask to look for the tracked object and get its gravity center
    for(x = mask->roi->xOffset; x < mask->roi->width + mask->roi->xOffset; x++) {
        for(y = mask->roi->yOffset; y < mask->roi->height + mask->roi->yOffset ; y++) { 
 
            // If its a tracked pixel, count it to the center of gravity's calcul
            if((mask->imageData[x+ y*mask->widthStep]) == 255) {
                sommeX += x;
                sommeY += y;
                (*nbPixels)++;
            }
        }
    }
    
    
    
      for(x = mask->roi->xOffset; x < mask->roi->width - mask->roi->xOffset; x++) {
        for(y = mask->roi->yOffset; y < mask->roi->height - mask->roi->yOffset ; y++) { 
 
            // If its a tracked pixel, count it to the center of gravity's calcul
            if(((uchar *)(mask->imageData + y*mask->widthStep))[x] == 255) {
                sommeX += x;
                sommeY += y;
                (*nbPixels)++;
            }
        }
    }
	
	cvResetImageROI(mask);
    // Show the result of the mask image
    if(window != NULL) 
		cvShowImage(window, mask);
   
	cvRectangleR(image,roi,cvScalar(0,0,255,0),1,8,0);
    // We release the memory of kernels
    cvReleaseStructuringElement(&kernel);
 
    // We release the memory of the mask
    cvReleaseImage(&mask);
    // We release the memory of the hsv image
    cvReleaseImage(&hsv);
 
    // If there is no pixel, we return a center outside the image, else we return the center of gravity
    if(*nbPixels > 0)
        return cvPoint((int)(sommeX / (*nbPixels)), (int)(sommeY / (*nbPixels)));
    else
        return cvPoint(-1, -1);
}

/*
 * Add a circle on the video that fellow your colored object
 */
void addObjectToVideo(char* window, IplImage* image, int shape, CvPoint origin, int width, int height, int nbPixels) {
	
    // Draw an object (circle) centered on the calculated center of gravity
    if (nbPixels > NB_PIXEL_THRESHOLD){
		switch (shape){
			case CIRCLE:
				cvDrawCircle(image, origin, width, CV_RGB(255, 0, 0), 1,8,0);	// Draw a circle around the origin
				break;
			case RECTANGLE:
				cvRectangle(image,origin,cvPoint(origin.x+width,origin.y+height), CV_RGB(255, 0, 0), 1,8,0);	// Draw a rectangle frow the origin
				break;
		}
		cvShowImage(window, image);
	}
        
 
    // We show the image on the window
    
 
}
 
/*
 * Get the color of the pixel where the mouse has clicked
 * We put this color as model color (the color we want to tracked)
 */
void getObjectColor(int event, int x, int y, int flags, void *param) {
 
    // Vars
    CvScalar pixel;
    IplImage *hsv;
    param = NULL;
 
    if(event == CV_EVENT_LBUTTONUP) {
 
        // Get the hsv image
        hsv = cvCloneImage(image);
        cvCvtColor(image, hsv, CV_BGR2HSV);
 
        // Get the selected pixel
        pixel = cvGet2D(hsv, y, x);
 
        // Change the value of the tracked color with the color of the selected pixel
        h = (int)pixel.val[0];
        s = (int)pixel.val[1];
        v = (int)pixel.val[2];
 
        // Release the memory of the hsv image
            cvReleaseImage(&hsv);
 
    }
 
}

void getCurrentPointCoordinates(int event, int x, int y, int flags, void *param){
	struct VolatileRect * workingSpace = (struct VolatileRect *)param;
	if(workingSpace->origineDefined == 1 && event == CV_EVENT_MOUSEMOVE){
		CvPoint origin = cvPoint(workingSpace->rect.x,workingSpace->rect.y);
		workingSpace->rect = cvRect(min(x,origin.x),min(y,origin.y),abs(x-origin.x),abs(y-origin.y));
	}
	
	if(event == CV_EVENT_LBUTTONUP){
		printf("click at x=%d \ty=%d\n",x,y);
		if(workingSpace->origineDefined){
			CvPoint origin = cvPoint(workingSpace->rect.x,workingSpace->rect.y);
//			printf("Working area : \nx :\t%d\t%d\ny :\t%d\t%d\n",point1.x,point2.x,point1.y,point2.y);
			workingSpace->rect = cvRect(min(x,origin.x),min(y,origin.y),abs(x-origin.x),abs(y-origin.y));
			workingSpace->rectDefined = 1;
		} else {
			workingSpace->rect.x = x;
			workingSpace->rect.y = y;
			workingSpace->origineDefined = 1;
		}
	}
}

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

void initFont(CvFont * font){
	double hScale=0.4;
	double vScale=0.4;
	int    lineWidth=1;
	
	cvInitFont(font, CV_FONT_HERSHEY_SIMPLEX|CV_FONT_ITALIC, hScale, vScale, 0, lineWidth, 8);
}

CvRect initWorkSpace(RaspiCamCvCapture * capture, char* window){
	struct VolatileRect workingSpace;
	workingSpace.origineDefined = 0;
	workingSpace.rectDefined = 0;
	
	cvNamedWindow(window, CV_WINDOW_AUTOSIZE);
	cvMoveWindow(window, 0, 100);
	cvSetMouseCallback(window, getCurrentPointCoordinates, &workingSpace);
	printf("Definition of the working space \n");
	while(workingSpace.rectDefined == 0) {			// wait for the definition of the workspace
		image = raspiCamCvQueryFrame(capture);
		if(workingSpace.origineDefined) {
			cvRectangleR(image,workingSpace.rect,cvScalar(0,0,255,0),1,8,0);
		}
			cvShowImage(window, image);		
		char keyPressed = cvWaitKey(30);
		switch (keyPressed){
			case 27:				//ESC to reset the rectangle origin
				workingSpace.origineDefined = 0;
				break;
		}
	}
	printf("Working space defined\n");
	cvDestroyWindow(window);
	return workingSpace.rect;
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


