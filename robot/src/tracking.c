#incude "tracking.h"


void initTrackedObject(TrackedObject* obj, int hue, int sat, int val, IplImage* img, CvRect* trackZone, int shape, int width, int height)
{
	obj->computeTracking = true;
	obj->h = hue;
	obj->s = sat;
	obj->v = val;
	obj->img = img;
	obj->trackingZone = trackZone;
	obj->shape = shape;
	obj->origin = cvPoint(-1,-1);
	obj->width = width;
	obj->height = height;
}

void enableTracking(TrackedObject* obj)
{
	obj->computeTracking = true;
}

void disableTracking(TrackedObject* obj)
{
	obj->computeTracking = false;
}

void updateTracking(TrackedObject* obj)
{
	if(obj->computeTracking){
		
	}
}


/*
 * Get the color of the pixel where the mouse has clicked
 * We put this color as model color (the color we want to tracked)
 */
void getObjectColor(int event, int x, int y, int flags, void *param) {
 
    struct TrackedObject* obj = (struct TrackedObject*)param;	/* tracked object is passed through param */
    CvScalar pixel;
    IplImage *hsv;
 
    if(event == CV_EVENT_LBUTTONUP) {
 
        /* Get the hsv image */
        hsv = cvCloneImage(obj->img);
        cvCvtColor(image, hsv, CV_BGR2HSV);
 
        /* Get the selected pixel */
        pixel = cvGet2D(hsv, y, x);
 
        /* Change the value of the tracked color with the color of the selected pixel */
        obj->h = (int)pixel.val[0];
        obj->s = (int)pixel.val[1];
        obj->v = (int)pixel.val[2];
 
        /* Release the memory of the hsv image */
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
			workingSpace->rect = cvRect(min(x,origin.x),min(y,origin.y),abs(x-origin.x),abs(y-origin.y));
			workingSpace->rectDefined = 1;
		} else {
			workingSpace->rect.x = x;
			workingSpace->rect.y = y;
			workingSpace->originDefined = 1;
		}
	}
}

CvRect initWorkSpace(RaspiCamCvCapture * capture, char* window){
	struct VolatileRect workingSpace;
	struct ImageBroadcast flux;
	
	workingSpace.originDefined = 0;
	workingSpace.rectDefined = 0;
	initImageBroadcast(&flux, NULL,window,NULL);
	
	cvSetMouseCallback(window, getCurrentPointCoordinates, &workingSpace);
	printf("Definition of the working space \n");
	while(workingSpace.rectDefined == 0) {			/* wait for the definition of the workspace */
		updateImageFromCapture(&flux,capture);
		if(workingSpace.originDefined) {
			cvRectangleR(flux.img,workingSpace.rect,cvScalar(0,0,255,0),1,8,0);
		}
			showImage(&flux);		
		char keyPressed = cvWaitKey(30);
		switch (keyPressed){
			case 27:				/* ESC to reset the rectangle origin */
				workingSpace.originDefined = 0;
				break;
		}
	}
	printf("Working space defined\n");
	cvDestroyWindow(window);
	return workingSpace.rect;
}
