/*!
* \file tracking.h
* \brief File containing structures and prototypes of functions related to the tracking of objects
*/

#ifndef TRACKING_H
#define TRACKING_H

#include <cv.h>
#include <highgui.h>
#include "configuration.h"
#include "imageBroadcast.h"
#include "imageProcessing.h"

typedef struct ImageBroadcast ImageBroadcast;
typedef struct TrackedObject TrackedObject;

struct TrackedObject{
	boolean computeTracking;	/* if false, the tracking is disabled */
	int h;		/* Hue of the color to track */
	int s;		/* Saturation of the color to track */
	int v;		/* Value of the color to track */
	ImageBroadcast* rawFlux;				/* The Full colored image in which the tracker is being displayed */
	ImageBroadcast* binFlux;				/* The binarised image in which the tracking is being processed */
	int nbPixels;			/* The number of pixel with the matching colour */
	CvRect trackingZone;		/* The zone in which the object is tracked */
	int shape;					/* The shape to draw around the tracked object */
	int width;					/* The width of the shape */
	int height;					/* The height of the shape */
	CvPoint origin;				/* The origin of the object (the centre for a circle, the upper left corner for a rectangle) */
};

void initTrackedObject(TrackedObject* obj, int hue, int sat, int val, ImageBroadcast* rawFlux, ImageBroadcast* binFlux, CvRect trackZone, int shape);
void enableTracking(TrackedObject* obj);
void disableTracking(TrackedObject* obj);
void updateTracking(TrackedObject* obj);
void loadTrackedObject(TrackedObject* obj,ImageBroadcast* rawFlux, ImageBroadcast* binFlux,FILE * loadFile);
void saveTrackedObject(TrackedObject* obj,FILE * saveFile);





#endif /*TRACKING_H*/
