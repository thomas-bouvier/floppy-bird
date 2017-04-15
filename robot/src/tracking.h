/*!
* \file tracking.h
* \brief File containing structures and prototypes of functions related to the tracking of objects
*/

#ifndef TRACKING_H
#define TRACKING_H

#include "configuration.h"


struct TrackedObject{
	boolean computeTracking;	// if false, the tracking is disabled
	int h;		// Hue of the color to track
	int s;		// Saturation of the color to track
	int v;		// Value of the color to track
	IplImage* img;				// The image in which the tracker is shown
	CvRect* trackingZone;		// The zone in which the object is tracked
	int shape;					// The shape to draw around the tracked object
	CvPoint origin;				// The origin of the object (the centre for a circle, the upper left corner for a rectangle)
};

void initTrackedObject(TrackedObject* obj, int hue, int sat, int val, IplImage* img, CvRect* trackZone, int shape);
CvPoint binarisation(IplImage* image, int *nbPixels, char* window);
void getObjectColor(int event, int x, int y, int flags, void *param);
void getCurrentPointCoordinates(int event, int x, int y, int flags, void *param);



#endif /*TRACKING_H*/
