/*!
* \file imageBroadcast.h
* \brief File containing structures and prototypes of functions related to image broadcasting
*/

#ifndef IMAGE_BROADCAST_H
#define IMAGE_BROADCAST_H

#include <stdio.h>
#include <cv.h>
#include <highgui.h>
#include "configuration.h"
#include "tracking.h"
#include "RaspiCamCV.h"

typedef struct ImageBroadcast ImageBroadcast;

/*!
* \struct ImageBroadcast 
* \brief ImageBroadcast is used to cast an image on the computer
*/
struct ImageBroadcast{
	IplImage* img;		/*!< The image to show */
	char* windowTitle;	/*!< The title of the window showing the image */
	CvFont * font;		/*!< The font if text on image */
	CvRect* workingSpace;	/*!< The working space if defined */
};


/*!
* \struct VolatileRect 
* \brief define a volatile rectangle => tells if the rectangle is defined / completely defined/ not defined
*/
struct VolatileRect{
	CvRect rect;			/*!< The rect being defined */
	CvPoint origin;			/*!< The origin of the rectangle (left up corner) */
	boolean originDefined;		/*!< Tells if origin is defined or not */
	boolean rectDefined;		/*!< Tells if the rect is completely defined or not */
};

void updateImage(ImageBroadcast* flux,IplImage* img);
void showImage(ImageBroadcast* flux);
void initImageBroadcast(ImageBroadcast* flux, IplImage* img, CvRect* workingSpace, char* windowTitle, CvFont * font);
void initFont(ImageBroadcast* flux);
void getObjectColor(int event, int x, int y, int flags, void *param);
void getCurrentPointCoordinates(int event, int x, int y, int flags, void *param);
void loadImage(ImageBroadcast* flux,RaspiCamCvCapture * capture);

#endif /*IMAGE_BROADCAST_H*/
