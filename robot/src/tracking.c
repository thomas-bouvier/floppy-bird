#include "tracking.h"


void initTrackedObject(TrackedObject* obj, int hue, int sat, int val, ImageBroadcast* rawFlux, ImageBroadcast* binFlux, CvRect trackZone, int shape)
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
	obj->width = 0;
	obj->height = 0;
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
		binarisation(obj);
		addObjectToVideo(obj);
		showImage(obj->rawFlux);
		showImage(obj->binFlux);
	}
}

void loadTrackedObject(TrackedObject* obj,ImageBroadcast* rawFlux, ImageBroadcast* binFlux,FILE * loadFile)
{
	obj->computeTracking = true;
	fread(&(obj->h),sizeof(int),1,loadFile);
	fread(&(obj->s),sizeof(int),1,loadFile);
	fread(&(obj->v),sizeof(int),1,loadFile);
	obj->rawFlux = rawFlux;
	obj->binFlux = binFlux;
	obj->nbPixels = 0;
	fread(&(obj->trackingZone),sizeof(CvRect),1,loadFile);
	fread(&(obj->shape),sizeof(int),1,loadFile);
	obj->origin = cvPoint(-1,-1);
	fread(&(obj->width),sizeof(int),1,loadFile);
	fread(&(obj->height),sizeof(int),1,loadFile);
}

void saveTrackedObject(TrackedObject* obj,FILE * saveFile)
{
	/* File : workingspace,hue,sat,val,trackingZone,shape,width,height */
	fwrite(obj->rawFlux->workingSpace,sizeof(CvRect),1,saveFile);
	fwrite(&(obj->h),sizeof(int),1,saveFile);
	fwrite(&(obj->s),sizeof(int),1,saveFile);
	fwrite(&(obj->v),sizeof(int),1,saveFile);
	fwrite(&(obj->trackingZone),sizeof(CvRect),1,saveFile);
	fwrite(&(obj->shape),sizeof(int),1,saveFile);
	fwrite(&(obj->width),sizeof(int),1,saveFile);
	fwrite(&(obj->width),sizeof(int),1,saveFile);
}
