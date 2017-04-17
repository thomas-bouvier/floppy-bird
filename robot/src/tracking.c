#incude "tracking.h"


void initTrackedObject(TrackedObject* obj, int hue, int sat, int val, ImageBroadcast* rawFlux, ImageBroadcast* binFlux, CvRect* trackZone, int shape, int width, int height)
{
	obj->computeTracking = true;
	obj->h = hue;
	obj->s = sat;
	obj->v = val;
	obj->rawFlux = rawFlux;
	obj->binFlux = binFlux;
	obj->nbPixels = 0;
	obj->trackingZone = trackZone;
	obj->shape = shape;
	obj->origin = cvPoint(-1,-1);
	obj->width = width;
	obj->height = height;
}

void enableTracking(TrackedObject* obj)
{
	obj->computeTracking = true;
}

void disableTracking(TrackedObject* obj)
{
	obj->computeTracking = false;
}

void updateTracking(TrackedObject* obj)
{
	if(obj->computeTracking){
		//TODO
	}
}


