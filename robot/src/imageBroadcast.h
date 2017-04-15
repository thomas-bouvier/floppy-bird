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
	IplImage* img;		// The image to show
	char* windowTitle;	// The title of the window showing the image
	CvFont * font;		// The font if text on image
};

void updateImage(ImageBroadcast* flux,IplImage* img);
void updateImageFromCapture(ImageBroadcast* flux, RaspiCamCvCapture * capture);
void showImage(ImageBroadcast* flux);
void initImageBroadcast(ImageBroadcast* flux, IplImage* img, char* windowTitle, CvFont * font);
void addObjectToVideo(ImageBroadcast* flux, TrackedObject* obj, int nbPixels);
void initFont(ImageBroadcast* flux, CvFont * font);


#endif /*IMAGE_BROADCAST_H*/
