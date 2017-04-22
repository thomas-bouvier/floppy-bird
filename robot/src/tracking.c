#include "tracking.h"

/*!
* \brief Init TrackedObject function : init and configure a TrackedObject
* \param[in] obj : address of the TrackedObject 
* \param[in] hue : The hue to track
* \param[in] sat : The sat to track
* \param[in] val : The val to track
* \param[in] rawFlux : The address of the flux in which we want to track the object
* \param[in] binFlux : The address of the flux in which we want save results of the tracking
* \param[in] trackZone : a CvRect of the zone in which we want to track the object
* \param[in] shape : The shape of the tracker being displayed
*/
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
	obj->trackerColor = DEFAULT_TRACKER_COLOR;
}

/*!
* \brief enable the tracking of a TrackedObject
* \param[in] obj : address of the TrackedObject 
*/
void enableTracking(TrackedObject* obj)
{
	obj->computeTracking = true;
}

/*!
* \brief disable the tracking of a TrackedObject
* \param[in] obj : address of the TrackedObject 
*/
void disableTracking(TrackedObject* obj)
{
	obj->computeTracking = false;
}

/*!
* \brief Update the tracking of a TrackedObject : computes the tracking and dispays it in the tracking window
* \param[in] obj : address of the TrackedObject 
*/
void updateTracking(TrackedObject* obj)
{
	if(obj->computeTracking){
		binarisation(obj);
		addObjectToVideo(obj);
		showImage(obj->binFlux);
	}
}

/*!
* \brief Release the memory of the last tracking image
* \param[in] obj : address of the TrackedObject 
*/
void releaseTrackingImageMemory(TrackedObject* obj)
{
	cvReleaseImage(&(obj->binFlux->img));
}

/*!
* \brief Load TrackedObject function : init and configure a TrackedObject from a config file
* \ Config file must be organized like this : workingspace,hue,sat,val,trackingZone,shape,width,height,trackerColor,hue,sat,val,... 
* \param[in] obj : address of the TrackedObject 
* \param[in] rawFlux : The address of the flux in which we want to track the object
* \param[in] binFlux : The address of the flux in which we want save results of the tracking
* \param[in] loadFile : The config file to load from
*/
void loadTrackedObject(TrackedObject* obj,ImageBroadcast* rawFlux, ImageBroadcast* binFlux, FILE * loadFile)
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
	fread(&(obj->trackerColor),sizeof(CvScalar),1,loadFile);
}

/*!
* \brief Save TrackedObject function : save a TrackedObject in a config file
* \ Config file must be organized like this : workingspace,hue,sat,val,trackingZone,shape,width,height,trackerColor,hue,sat,val,... 
* \param[in] obj : address of the TrackedObject 
* \param[in] saveFile : The config file to save in
*/
void saveTrackedObject(TrackedObject* obj,FILE * saveFile)
{
	if(saveFile != NULL){
		fwrite(&(obj->h),sizeof(int),1,saveFile);
		fwrite(&(obj->s),sizeof(int),1,saveFile);
		fwrite(&(obj->v),sizeof(int),1,saveFile);
		fwrite(&(obj->trackingZone),sizeof(CvRect),1,saveFile);
		fwrite(&(obj->shape),sizeof(int),1,saveFile);
		fwrite(&(obj->width),sizeof(int),1,saveFile);
		fwrite(&(obj->width),sizeof(int),1,saveFile);
		fwrite(&(obj->trackerColor),sizeof(CvScalar),1,saveFile);
	}
}

/*!
* \brief get a relative distance between the working space and an element of the tracker
* \param[in] obj : address of the TrackedObject 
* \param[in] nature : The nature of the distance (UP,LEFT,RIGHT)
*/
float getRelativeDistance(TrackedObject* obj, int nature)
{
	cvSetImageROI(obj->binFlux->img,obj->trackingZone);
	switch(nature){
		case UP :
			return 1-((float)(obj->origin.y-(obj->height/2))/(obj->binFlux->img->roi->height)); 
			break;
		case LEFT :
			return (float)(obj->origin.x-(obj->height/2))/(obj->binFlux->img->roi->width);
			break;
		case RIGHT :
			return (float)(obj->origin.x+(obj->height/2))/(obj->binFlux->img->roi->width);
			break;
	}
	cvResetImageROI(obj->binFlux->img);
	return 0;
}
