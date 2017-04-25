/*!
* \file dynamicTracking.h
* \brief File containing structures and prototypes of functions related to the tracking of objects dynamicaly
*/
#ifndef DYNAMIC_TRACKING_H
#define DYNAMIC_TRACKING_H

/*!
* \struct DynamicTrackedObject 
* \brief Rely all dynamic tracker specific informations 
*/
struct DynamicTrackedObject{
	TrackedObject* obj;				/*!<  the object to track */
	boolean trackingRunning;		/*!<  if true, tracking is running */
};



#endif /* DYNAMIC_TRACKING_H */
