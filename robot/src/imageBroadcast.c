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
	if(workingSpace->originDefined == 1 && event == CV_EVENT_MOUSEMOVE){
		CvPoint origin = cvPoint(workingSpace->rect.x,workingSpace->rect.y);
		workingSpace->rect = cvRect(min(x,origin.x),min(y,origin.y),abs(x-origin.x),abs(y-origin.y));
	}
	
	if(event == CV_EVENT_LBUTTONUP){
		printf("click at x=%d \ty=%d\n",x,y);
		if(workingSpace->originDefined){
			CvPoint origin = cvPoint(workingSpace->rect.x,workingSpace->rect.y);
//			printf("Working area : \nx :\t%d\t%d\ny :\t%d\t%d\n",point1.x,point2.x,point1.y,point2.y);
			workingSpace->rect = cvRect(min(x,origin.x),min(y,origin.y),abs(x-origin.x),abs(y-origin.y));
			workingSpace->rectDefined = 1;
		} else {
			workingSpace->rect.x = x;
			workingSpace->rect.y = y;
			workingSpace->originDefined = 1;
		}
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
