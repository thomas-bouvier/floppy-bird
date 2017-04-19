#include "imageBroadcast.h"


void initImageBroadcast(ImageBroadcast* flux, IplImage* img, CvRect* workingSpace, char* windowTitle, CvFont * font)
{
	flux->img = img;
	flux->windowTitle = windowTitle;
	flux->font = font;
	flux->workingSpace = workingSpace;
	cvNamedWindow(windowTitle, CV_WINDOW_AUTOSIZE);
}

void initFont(ImageBroadcast* flux){
	double hScale=0.4;
	double vScale=0.4;
	int    lineWidth=1;
	if(flux->font != NULL)
		cvInitFont(flux->font, CV_FONT_HERSHEY_SIMPLEX|CV_FONT_ITALIC, hScale, vScale, 0, lineWidth, 8);
	else 
		fprintf(stderr,"error in initFont : flux->font NULL\n");
}

void updateImage(ImageBroadcast* flux,IplImage* img)
{
	flux->img = img;
}

void showImage(ImageBroadcast* flux)
{
	cvShowImage(flux->windowTitle,flux->img);
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
        hsv = cvCloneImage(obj->rawFlux->img);
        /* Save the new color for the tracker */
		obj->trackerColor = cvGet2D(hsv, y, x);
        cvCvtColor(obj->rawFlux->img, hsv, CV_BGR2HSV);
 
        /* Get the selected pixel */
        pixel = cvGet2D(hsv, y, x);
 
        /* Change the value of the tracked color with the color of the selected pixel */
        obj->h = (int)pixel.val[0];
        obj->s = (int)pixel.val[1];
        obj->v = (int)pixel.val[2];
        printf("color selected : h=%d s=%d v=%d\n",obj->h,obj->s,obj->v);
 
        /* Release the memory of the hsv image */
        cvReleaseImage(&hsv);
 
    }
}

void getCurrentPointCoordinates(int event, int x, int y, int flags, void *param){
	struct VolatileRect * workingSpace = (struct VolatileRect *)param;
	if(workingSpace->originDefined == 1 && event == CV_EVENT_MOUSEMOVE){
		workingSpace->rect = cvRect(min(x,workingSpace->origin.x),min(y,workingSpace->origin.y),abs(x-workingSpace->origin.x),abs(y-workingSpace->origin.y));
	}
	
	if(event == CV_EVENT_LBUTTONUP){
		if(workingSpace->originDefined){
			workingSpace->rect = cvRect(min(x,workingSpace->origin.x),min(y,workingSpace->origin.y),abs(x-workingSpace->origin.x),abs(y-workingSpace->origin.y));
			workingSpace->rectDefined = 1;
		} else {
			workingSpace->origin = cvPoint(x,y);
			workingSpace->originDefined = 1;
		}
	}
}

void loadImage(ImageBroadcast* flux,RaspiCamCvCapture * capture)
{
	flux->img = raspiCamCvQueryFrame(capture);
	if(flux->workingSpace != NULL)
		cvSetImageROI(flux->img,*(flux->workingSpace));
}
