#include "imageBroadcast.h"

/*
 * Add a circle on the video that follow your colored object
 */
void addObjectToVideo(char* window, IplImage* image, int shape, CvPoint origin, int width, int height, int nbPixels) {
	
    // Draw an object centered on the calculated center of gravity
    if (nbPixels > NB_PIXEL_THRESHOLD){
		switch (shape){
			case CIRCLE:
				cvDrawCircle(image, origin, width, CV_RGB(255, 0, 0), 1,8,0);	// Draw a circle around the origin
				break;
			case RECTANGLE:
				cvRectangle(image,origin,cvPoint(origin.x+width,origin.y+height), CV_RGB(255, 0, 0), 1,8,0);	// Draw a rectangle frow the origin
				break;
		}
		cvShowImage(window, image);	// We show the image on the window
	}
}

void initFont(CvFont * font){
	double hScale=0.4;
	double vScale=0.4;
	int    lineWidth=1;
	
	cvInitFont(font, CV_FONT_HERSHEY_SIMPLEX|CV_FONT_ITALIC, hScale, vScale, 0, lineWidth, 8);
}

CvRect initWorkSpace(RaspiCamCvCapture * capture, char* window){
	struct VolatileRect workingSpace;
	workingSpace.originDefined = 0;
	workingSpace.rectDefined = 0;
	
	cvNamedWindow(window, CV_WINDOW_AUTOSIZE);
	cvMoveWindow(window, 0, 100);
	cvSetMouseCallback(window, getCurrentPointCoordinates, &workingSpace);
	printf("Definition of the working space \n");
	while(workingSpace.rectDefined == 0) {			// wait for the definition of the workspace
		image = raspiCamCvQueryFrame(capture);
		if(workingSpace.originDefined) {
			cvRectangleR(image,workingSpace.rect,cvScalar(0,0,255,0),1,8,0);
		}
			cvShowImage(window, image);		
		char keyPressed = cvWaitKey(30);
		switch (keyPressed){
			case 27:				//ESC to reset the rectangle origin
				workingSpace.originDefined = 0;
				break;
		}
	}
	printf("Working space defined\n");
	cvDestroyWindow(window);
	return workingSpace.rect;
}
