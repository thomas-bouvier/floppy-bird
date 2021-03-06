#include "pipeTracking.h"

/*!
* \brief Tell if the two rectangles are currently intersecting each other
* \param[in] rect1 : the first CvRect
* \param[in] rect2 : the second CvRect
* \return true if rectangles are currently intersecting each other, false otherwise
*/
boolean intersectRect(CvRect rect1, CvRect rect2)
{
	if(rect1.x<= rect2.x){
		if(rect2.x <= rect1.x + rect1.width){
			if(rect1.y<= rect2.y){
				if(rect2.y <= rect1.y + rect1.width){
					return true;
				} else {
					return false;
				}
			} else {
				if(rect1.y <= rect2.y + rect2.height){
					return true;
				} else {
					return false;
				}
			}
		} else {
			return false;
		}
	} else {
		return intersectRect(rect2,rect1);
	}
}

/*!
* \brief Init a pipeDynamicTracker struct
* \param[in] pipeDynamicTracker : the addrss of the struct to initialise
* \param[in] pipeTracker : an array of TrackedObject (size = NB_PIPE_TRACKER)
*/
void initPipeDynamicTracker(PipeDynamicTracker* pipeDynamicTracker, struct TrackedObject* pipeTracker)
{
	int i;
	pipeDynamicTracker->pipeTracker = pipeTracker;
	pipeDynamicTracker->trackingRunning[0] = true;
	for(i = 1; i < NB_PIPE_TRACKER;i++){
		pipeDynamicTracker->trackingRunning[i] = false;
	}
}

/*!
* \brief update a pipeDynamicTracker
* \param[in] pipeDynamicTracker : the address of the struct to initialise
*/
void updatePipeDynamicTracker(PipeDynamicTracker* pipeDynamicTracker){
	int i, j;	
	binarisation(&(pipeDynamicTracker->pipeTracker[0]));
	for(i = 0; i < NB_PIPE_TRACKER;i++){
		TrackedObject* obj = &(pipeDynamicTracker->pipeTracker[i]);
		if(pipeDynamicTracker->trackingRunning[i]){
			if(obj->computeTracking){
				findObject(obj,true);
				addObjectToVideo(obj);
				pipeDynamicTracker->trackingRunning[i] = centerTrackingZoneOnTracker(obj);
			}
		} else {
			boolean beginTrack = true;
			for(j = 0; j < NB_PIPE_TRACKER;j++){	/* Search if a tracker is running in the the same zone */
				if(j != i){
					TrackedObject* obj2 = &(pipeDynamicTracker->pipeTracker[j]);
					beginTrack &= !(pipeDynamicTracker->trackingRunning[j] && intersectRect(obj->trackingZone,obj2->trackingZone));
				}
			}
			pipeDynamicTracker->trackingRunning[i] = beginTrack;
		}
	}
	showImage(pipeDynamicTracker->pipeTracker[0].binFlux);
}

/*!
* \brief shift the tracking zone to center it on the tracker
* \param[in] obj : the addrss of the TrackedObject 
* \return trackingRunning
*/
boolean centerTrackingZoneOnTracker(TrackedObject* obj)
{
	if(obj->nbPixels > THRESHOLD_NB_PIXELS_PIPE){
		int newOriginX = obj->origin.x - obj->trackingZone.width/2-1;
		if(newOriginX < 0){
			obj->trackingZone.x = obj->binFlux->img->width - obj->trackingZone.width-1;
			return false;
		}
		obj->trackingZone.x = newOriginX;
		return true;
	} else {
		obj->trackingZone.x = obj->binFlux->img->width - obj->trackingZone.width-1;
		return false;
	}
}

/*!
* \brief search the next pipe
* \param[in] pipeDynamicTracker : the address of the struct to compute
* \param[in] minPosition : the low limit before which a pipe is not considered anymore
* \return the origin (upper left corner) of the pipe
*/
CvPoint nextPipe(PipeDynamicTracker* pipeDynamicTracker, int minPosition)
{
	int i;
	int minX = pipeDynamicTracker->pipeTracker[0].binFlux->img->width;
	int y = 0;
	for(i = 0; i < NB_PIPE_TRACKER;i++){
		TrackedObject* obj = &(pipeDynamicTracker->pipeTracker[i]);
		int x = obj->origin.x-(obj->width/2);
		if(x > minPosition && x < minX){
			minX = x;
			y = obj->origin.y-(obj->height/2);
		}
	}
	return cvPoint(minX,y);
}



