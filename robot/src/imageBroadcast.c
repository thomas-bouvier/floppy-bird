#include "imageBroadcast.h"

/*!
* \brief Init image broadcast function : init an ImageBroadcast and create the associated window
* \param[in] flux : address of the ImageBroadcast 
* \param[in] img : The image to show 
* \param[in] workingSpace : The working space if defined 
* \param[in] windowTitle : The title of the window showing the image 
* \param[in] font : The font if text on image
*/
void initImageBroadcast(ImageBroadcast* flux, IplImage* img, CvRect* workingSpace, char* windowTitle, CvFont * font)
{
	flux->img = img;
	flux->windowTitle = windowTitle;
	flux->font = font;
	flux->workingSpace = workingSpace;
	cvNamedWindow(windowTitle, CV_WINDOW_AUTOSIZE);
}

/*!
* \brief Init font function : init the font of an ImageBroadcast
* \param[in] flux : address of the ImageBroadcast 
*/
void initFont(ImageBroadcast* flux){
	double hScale=0.4;
	double vScale=0.4;
	int    lineWidth=1;
	if(flux->font != NULL)
		cvInitFont(flux->font, CV_FONT_HERSHEY_SIMPLEX|CV_FONT_ITALIC, hScale, vScale, 0, lineWidth, 8);
	else 
		fprintf(stderr,"error in initFont : flux->font NULL\n");
}

/*!
* \brief Update image function : update the image associated with an ImageBroadcast
* \param[in] flux : address of the ImageBroadcast 
* \param[in] img : The image to update 
*/
void updateImage(ImageBroadcast* flux,IplImage* img)
{
	flux->img = img;
}

/*!
* \brief Show image function : show the ImageBroadcast image in the associated window
* \param[in] flux : address of the ImageBroadcast 
*/
void showImage(ImageBroadcast* flux)
{
	if(flux->windowTitle != NULL) 
		cvShowImage(flux->windowTitle,flux->img);
}

/*!
* \brief Get the color of the pixel where the mouse has clicked
* \ We put this color as model color (the color we want to tracked)
* \param[in] event : type of event
* \param[in] x : The abscise of the point
* \param[in] y : The ordonate of the point
* \param[in] flags : not used here
* \param[in] param : The TrackedObject associated with the event
*/
void getObjectColor(int event, int x, int y, int flags, void *param) {
 
    struct TrackedObject* obj = (struct TrackedObject*)param;	/* tracked object is passed through param */
    CvScalar pixel;
    IplImage *hsv;
 
    if(event == CV_EVENT_LBUTTONUP) {
		
        /* Get the hsv image */
        hsv = cvCloneImage(obj->rawFlux->img);
        /* Save the new color for the tracker */
		obj->trackerColor = cvGet2D(hsv, y, x);
        cvCvtColor(obj->rawFlux->img, hsv, CV_BGR2HSV);
 
        /* Get the selected pixel */
        pixel = cvGet2D(hsv, y, x);
 
        /* Change the value of the tracked color with the color of the selected pixel */
        obj->h = (int)pixel.val[0];
        obj->s = (int)pixel.val[1];
        obj->v = (int)pixel.val[2];
        printf("color selected : h=%d s=%d v=%d\n",obj->h,obj->s,obj->v);
 
        /* Release the memory of the hsv image */
        cvReleaseImage(&hsv);
 
    }
}

/*!
* \brief Get the color of the pixel where the mouse has clicked and set it in a PipeDynamicTracker
* \ We put this color as model color (the color we want to tracked)
* \param[in] event : type of event
* \param[in] x : The abscise of the point
* \param[in] y : The ordonate of the point
* \param[in] flags : not used here
* \param[in] param : The PipeDynamicTracker associated with the event
*/
void getPipeColor(int event, int x, int y, int flags, void *param) {
	int i;
    struct PipeDynamicTracker* pipeDynTracker = (struct PipeDynamicTracker*)param;	/* PipeDynamicTracker is passed through param */
    CvScalar pixel;
    IplImage *hsv;
 
    if(event == CV_EVENT_LBUTTONUP) {
		
        /* Get the hsv image */
        hsv = cvCloneImage(pipeDynTracker->pipeTracker[0].rawFlux->img);
        /* Save the new color for the tracker */
        for(i = 0; i < NB_PIPE_TRACKER; i++){
			TrackedObject* obj = &(pipeDynTracker->pipeTracker[i]);
			obj->trackerColor = cvGet2D(hsv, y, x);
		}
        cvCvtColor(pipeDynTracker->pipeTracker[0].rawFlux->img, hsv, CV_BGR2HSV);
 
        /* Get the selected pixel */
        pixel = cvGet2D(hsv, y, x);
 
        /* Change the value of the tracked color with the color of the selected pixel */
        for(i = 0; i < NB_PIPE_TRACKER; i++){
			TrackedObject* obj = &(pipeDynTracker->pipeTracker[i]);
			obj->h = (int)pixel.val[0];
			obj->s = (int)pixel.val[1];
			obj->v = (int)pixel.val[2];
		}
 
        /* Release the memory of the hsv image */
        cvReleaseImage(&hsv);
    }
}

/*!
* \brief Get the current point coordinates to define a VolatileRect
* \param[in] event : type of event
* \param[in] x : The abscise of the point
* \param[in] y : The ordonate of the point
* \param[in] flags : not used here
* \param[in] param : The VolatileRect associated with the event
*/
void getCurrentPointCoordinates(int event, int x, int y, int flags, void *param){
	struct VolatileRect * workingSpace = (struct VolatileRect *)param;
	if(workingSpace->originDefined == 1 && event == CV_EVENT_MOUSEMOVE){
		workingSpace->rect = cvRect(min(x,workingSpace->origin.x),min(y,workingSpace->origin.y),abs(x-workingSpace->origin.x),abs(y-workingSpace->origin.y));
	}
	
	if(event == CV_EVENT_LBUTTONUP){
		if(workingSpace->originDefined){
			workingSpace->rect = cvRect(min(x,workingSpace->origin.x),min(y,workingSpace->origin.y),abs(x-workingSpace->origin.x),abs(y-workingSpace->origin.y));
			workingSpace->rectDefined = 1;
		} else {
			workingSpace->origin = cvPoint(x,y);
			workingSpace->originDefined = 1;
		}
	}
}


/*!
* \brief Load image function : load the ImageBroadcast associated image 
* \param[in] flux : address of the ImageBroadcast 
* \param[in] capture : address of the capture
*/
void loadImage(ImageBroadcast* flux,RaspiCamCvCapture * capture)
{
	if(capture != NULL){	
		flux->img = raspiCamCvQueryFrame(capture);
		if(flux->workingSpace != NULL)
			cvSetImageROI(flux->img,*(flux->workingSpace));
	} else {	/* capture NULL */
		fprintf(stderr,"Error : capture NULL");
	}
}
