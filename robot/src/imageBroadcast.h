/*!
* \file imageBroadcast.h
* \brief File containing structures and prototypes of functions related to image broadcasting
*/

#ifndef IMAGE_BROADCAST_H
#define IMAGE_BROADCAST_H

#include <cv.h>
#include <highgui.h>
#include "configuration.h"
#include "tracking.h"

struct ImageBroadcast{
	IplImage* img;		/* The image to show */
	char* windowTitle;	/* The title of the window showing the image */
	CvFont * font;		/* The font if text on image */
};

/* Struct to define a volatile rectangle => tells if the rectangle is defined / completely defined/ not defined */
struct VolatileRect{
	CvRect rect;
	int originDefined;
	int rectDefined;
};

void updateImage(ImageBroadcast* flux,IplImage* img);
void updateImageFromCapture(ImageBroadcast* flux, RaspiCamCvCapture * capture);
void showImage(ImageBroadcast* flux);
void initImageBroadcast(ImageBroadcast* flux, IplImage* img, char* windowTitle, CvFont * font);
void initFont(ImageBroadcast* flux, CvFont * font);
void getObjectColor(int event, int x, int y, int flags, void *param);
void getCurrentPointCoordinates(int event, int x, int y, int flags, void *param);


#endif /*IMAGE_BROADCAST_H*/
