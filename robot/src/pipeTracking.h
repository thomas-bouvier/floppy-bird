/*!
* \file pipeTracking.h
* \brief File containing structures and prototypes of functions related to the tracking of pipes
*/
#ifndef PIPE_TRACKING_H
#define PIPE_TRACKING_H

#include <cv.h>
#include "configuration.h"

typedef struct TrackedObject TrackedObject;

/*!
* \struct PipeDynamicTracker 
* \brief struct specific to the tracking of pipes (dynamic tracking) 
*/
struct PipeDynamicTracker{
	TrackedObject* pipeTracker[NB_PIPE_TRACKER];		/*!<  pipe trackers */
	boolean trackingRunning[NB_PIPE_TRACKER];			/*!<  for each tracker, if true, tracking is running */
};

boolean intersectRect(CvRect rect1, CvRect rect2);
void initPipeDynamicTracker(PipeDynamicTracker* pipeDynamicTracker, TrackedObject* pipeTracker[NB_PIPE_TRACKER]);
void updatePipeDynamicTracker(PipeDynamicTracker* pipeDynamicTracker);
boolean centerTrackingZoneOnTracker(TrackedObject* obj);

#endif /* DYNAMIC_TRACKING_H */
