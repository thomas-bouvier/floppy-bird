#include "imageBroadcast.h"


void initImageBroadcast(ImageBroadcast* flux, IplImage* img, char* windowTitle, CvFont * font)
{
	flux->img = img;
	flux->windowTitle = windowTitle;
	flux->font = font;
	cvNamedWindow(windowTitle, CV_WINDOW_AUTOSIZE);
}

/*
 * Add a circle on the video that follow your colored object
 */
void addObjectToVideo(ImageBroadcast* flux, TrackedObject* obj, int nbPixels) {
	
    /* Draw an object centered on its origin */
    if (nbPixels > NB_PIXEL_THRESHOLD){
		switch (obj->shape){
			case CIRCLE:
				cvDrawCircle(flux->img, obj->origin, obj->width, TRACKED_OBJECT_DEFAULT_COLOR, 1,8,0);	/* Draw a circle around the origin */
				break;
			case RECTANGLE:
				cvRectangle(flux->img,obj->origin,cvPoint(obj->origin.x+obj->width,obj->origin.y+obj->height), TRACKED_OBJECT_DEFAULT_COLOR, 1,8,0);	/* Draw a rectangle frow the origin */
				break;
		}
	}
}

void initFont(ImageBroadcast* flux){
	double hScale=0.4;
	double vScale=0.4;
	int    lineWidth=1;
	
	cvInitFont(flux->font, CV_FONT_HERSHEY_SIMPLEX|CV_FONT_ITALIC, hScale, vScale, 0, lineWidth, 8);
}

void updateImage(ImageBroadcast* flux,IplImage* img)
{
	flux->img = img;
}

void updateImageFromCapture(ImageBroadcast* flux, RaspiCamCvCapture * capture)
{
	flux->img = raspiCamCvQueryFrame(capture);
}

void showImage(ImageBroadcast* flux)
{
	cvShowImage(flux->windowTitle,flux->img);
}

