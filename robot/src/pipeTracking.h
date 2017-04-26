/*!
* \file pipeTracking.h
* \brief File containing structures and prototypes of functions related to the tracking of pipes
*/
#ifndef PIPE_TRACKING_H
#define PIPE_TRACKING_H

#include <cv.h>
#include "configuration.h"
#include "tracking.h"

typedef struct PipeDynamicTracker PipeDynamicTracker;
typedef struct TrackedObject TrackedObject;

/*!
* \struct PipeDynamicTracker 
* \brief struct specific to the tracking of pipes (dynamic tracking) 
*/
struct PipeDynamicTracker{
	TrackedObject** pipeTracker;		/*!<  array of pipe trackers */
	boolean trackingRunning[NB_PIPE_TRACKER];			/*!<  for each tracker, if true, tracking is running */
};

boolean intersectRect(CvRect rect1, CvRect rect2);
void initPipeDynamicTracker(PipeDynamicTracker* pipeDynamicTracker, TrackedObject* pipeTracker[NB_PIPE_TRACKER]);
void updatePipeDynamicTracker(PipeDynamicTracker* pipeDynamicTracker);
boolean centerTrackingZoneOnTracker(TrackedObject* obj);
CvPoint nextPipe(PipeDynamicTracker* pipeDynamicTracker, int minPosition);

#endif /* DYNAMIC_TRACKING_H */
