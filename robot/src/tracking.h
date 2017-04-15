/*!
* \file tracking.h
* \brief File containing structures and prototypes of functions related to the tracking of objects
*/

#ifndef TRACKING_H
#define TRACKING_H


struct TrackedObject{
	boolean computeTracking;	// if false, the tracking is disabled
	IplImage* img;				// The image in which the tracker is shown
	CvRect trackingZone;		// The zone in which the object is tracked
	int shape;					// The shape to draw around the tracked object
	CvPoint origin;				// The origin of the object (the centre for a circle, the upper left corner for a rectangle)
};

CvPoint binarisation(IplImage* image, int *nbPixels, char* window) ;



#endif /*TRACKING_H*/
