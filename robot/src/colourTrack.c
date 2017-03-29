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
 
// Step mooving for object min & max
#define STEP_MIN 1
#define STEP_MAX 100 

// Position of the object we overlay
CvPoint objectPos = {-1, -1};
// Color tracked and our tolerance towards it
int h = 0, s = 0, v = 0, Htolerance = 5, Stolerance = 30;
IplImage* image;
 
/*
 * Transform the image into a two colored image, one color for the color we want to track, another color for the others colors
 * From this image, we get two datas : the number of pixel detected, and the center of gravity of these pixel
 */
CvPoint binarisation(IplImage* image, int *nbPixels) {
 
    int x, y;
    IplImage *hsv, *mask;
    IplConvKernel *kernel;
    int sommeX = 0, sommeY = 0;
    *nbPixels = 0;
	
	CvRect roi = cvRect(100,10,100,image->height-20);
	
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
	//printf("width : %d\nheight : %d\nxoffset : %d\nyoffset : %d\n",mask->roi->width,mask->roi->height,mask->roi->xOffset,mask->roi->yOffset);
	//cvResetImageROI(mask);
	
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
    
    cvResetImageROI(mask);
    /*
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
	*/
	
    // Show the result of the mask image
    cvShowImage("Mask", mask);
   
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
void addObjectToVideo(IplImage* image, CvPoint objectNextPos, int nbPixels) {
 
    int objectNextStepX, objectNextStepY;
 
    // Calculate circle next position (if there is enough pixels)
    /*if (nbPixels > 10) {
 
        // Reset position if no pixel were found
        if (objectPos.x == -1 || objectPos.y == -1) {
            objectPos.x = objectNextPos.x;
            objectPos.y = objectNextPos.y;
        }
 
        // Move step by step the object position to the desired position
        if (abs(objectPos.x - objectNextPos.x) > STEP_MIN) {
            objectNextStepX = max(STEP_MIN, min(STEP_MAX, abs(objectPos.x - objectNextPos.x) / 2));
            objectPos.x += (-1) * sign(objectPos.x - objectNextPos.x) * objectNextStepX;
        }
        if (abs(objectPos.y - objectNextPos.y) > STEP_MIN) {
            objectNextStepY = max(STEP_MIN, min(STEP_MAX, abs(objectPos.y - objectNextPos.y) / 2));
            objectPos.y += (-1) * sign(objectPos.y - objectNextPos.y) * objectNextStepY;
        }
 
    // -1 = object isn't within the camera range
    } else {
 
        objectPos.x = -1;
        objectPos.y = -1;
 
    }
	*/
    // Draw an object (circle) centered on the calculated center of gravity
    if (nbPixels > 10)
        cvDrawCircle(image, objectNextPos, 15, CV_RGB(255, 0, 0), 1,8,0);
 
    // We show the image on the window
    cvShowImage("Color Tracking", image);
 
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

int main(int argc, char *argv[]){
	
	wiringPiSetup();	// Setup the GPIO
	Stylus stylus;
  attach(&stylus,PWM_PIN,STYLUS_CLICK_POSITION,STYLUS_REST_POSITION,PRESS_DELAY,REST_DELAY);
  enable(&stylus);

	RASPIVID_CONFIG * config = (RASPIVID_CONFIG*)malloc(sizeof(RASPIVID_CONFIG));
	
	config->width=960/2;
	config->height=720/2;
	config->bitrate=0;	// zero: leave as default
	config->framerate=0;
	config->monochrome=0;
	
	RaspiCamCvCapture * capture = (RaspiCamCvCapture *) raspiCamCvCreateCameraCapture2(0, config); 
	free(config);
	
	
CvFont font;
	double hScale=0.4;
	double vScale=0.4;
	int    lineWidth=1;
	int nbPixels;
	// Next position of the object we overlay
    CvPoint objectNextPos;

	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX|CV_FONT_ITALIC, hScale, vScale, 0, lineWidth, 8);

	cvNamedWindow("Color Tracking", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("Mask", CV_WINDOW_AUTOSIZE);
    cvMoveWindow("Mask", 650, 100);
    cvMoveWindow("Color Tracking", 0, 100);
    
    cvSetMouseCallback("Color Tracking", getObjectColor,NULL);
	int exit =0;
	do {
		image = raspiCamCvQueryFrame(capture);
		
		objectNextPos = binarisation(image, &nbPixels);
        addObjectToVideo(image, objectNextPos, nbPixels);
		
		char key = cvWaitKey(1);
		
		switch(key)	
		{
			case 32:		// space to click
				click(&stylus);
				break;
			case 27:		// Esc to exit
				exit = 1;
				break;
			case 60:		// < (less than)
				raspiCamCvSetCaptureProperty(capture, RPI_CAP_PROP_FPS, 25);	// Currently NOOP
				break;
			case 62:		// > (greater than)
				raspiCamCvSetCaptureProperty(capture, RPI_CAP_PROP_FPS, 30);	// Currently NOOP
				break;
		}
		update(&stylus);
		
	} while (!exit);

	cvDestroyWindow("Color Tracking");
    cvDestroyWindow("Mask");
	raspiCamCvReleaseCapture(&capture);
	disable(&stylus);
	
	return 0; 
}


