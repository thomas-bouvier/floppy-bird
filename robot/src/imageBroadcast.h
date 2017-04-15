/*!
* \file imageBroadcast.h
* \brief File containing structures and prototypes of functions related to image broadcasting
*/

#ifndef IMAGE_BROADCAST_H
#define IMAGE_BROADCAST_H

struct ImageBroadcast{
	IplImage* img;		// The image to show
	char* windowTitle;	// The title of the window showing the image
};

// Struct to define a volatile rectangle => tells if the rectangle is defined / completely defined/ not defined
struct VolatileRect{
	CvRect rect;
	int originDefined;
	int rectDefined;
};

void addObjectToVideo(char* window, IplImage* image, int shape, CvPoint origin, int width, int height, int nbPixels);
void getObjectColor(int event, int x, int y, int flags, void *param);
void getCurrentPointCoordinates(int event, int x, int y, int flags, void *param);
void initFont(CvFont * font);
CvRect initWorkSpace(RaspiCamCvCapture * capture, char* window);


#endif /*IMAGE_BROADCAST_H*/
