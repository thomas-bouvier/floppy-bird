#include "imageProcessing.h"

/*
 * Transform the image into a two colored image, one color for the color we want to track, another color for the others colors
 * From this image, we get two datas : the number of pixel detected, and the center of gravity of these pixel
 */
void binarisation(TrackedObject* obj) 
{ 
    int x, y;
    IplImage *hsv;
    IplConvKernel *kernel;
    int sumX = 0, sumY = 0;
    obj->nbPixels = 0;
	
	//int zoneWidth = 50;		// The zone width in wich the colour will be tracked
	//CvRect roi = cvRect(((image->roi->width/3) - (zoneWidth/2)),0,zoneWidth,image->roi->height);
	
    obj->binFlux->img = cvCreateImage(cvGetSize(obj->rawFlux->img), obj->rawFlux->img->depth, 1);	/* Create the mask &initialize it to white (no color detected) */
	
    /* Create the hsv image */
    hsv = cvCloneImage(obj->rawFlux->img);
    cvCvtColor(obj->rawFlux->img, hsv, CV_BGR2HSV);
	
    /* We create the mask */
    cvInRangeS(hsv, cvScalar(obj->h - Htolerance -1, obj->s - Stolerance, 0,0), cvScalar(obj->h + Htolerance -1, obj->s + Stolerance, 255,0), obj->binFlux->img);
	
    /* Create kernels for the morphological operation */
    kernel = cvCreateStructuringElementEx(5, 5, 2, 2, CV_SHAPE_RECT,NULL);
 
    /* Morphological opening (inverse because we have white pixels on black background) */
    cvDilate(obj->binFlux->img, obj->binFlux->img, kernel, 2);
    cvErode(obj->binFlux->img, obj->binFlux->img, kernel, 2);  
    cvSetImageROI(obj->binFlux->img,obj->trackingZone);
	
    /* We go through the mask to look for the tracked object and get its gravity center */
    for(x = obj->binFlux->img->roi->xOffset; x < obj->binFlux->img->roi->width + obj->binFlux->img->roi->xOffset; x++) {
        for(y = obj->binFlux->img->roi->yOffset; y < obj->binFlux->img->roi->height + obj->binFlux->img->roi->yOffset ; y++) { 
 
            /* If its a tracked pixel, count it to the center of gravity's calcul */
            if((obj->binFlux->img->imageData[x+ y*obj->binFlux->img->widthStep]) == 255) {
                sumX += x;
                sumY += y;
                (obj->nbPixels)++;
            }
        }
    }
    
    
    
      for(x = obj->binFlux->img->roi->xOffset; x < obj->binFlux->img->roi->width - obj->binFlux->img->roi->xOffset; x++) {
        for(y = obj->binFlux->img->roi->yOffset; y < obj->binFlux->img->roi->height - obj->binFlux->img->roi->yOffset ; y++) { 
 
            /* If its a tracked pixel, count it to the center of gravity's calcul */
            if(((uchar *)(obj->binFlux->img->imageData + y*obj->binFlux->img->widthStep))[x] == 255) {
                sumX += x;
                sumY += y;
                (obj->nbPixels)++;
            }
        }
    }
	
	cvResetImageROI(obj->binFlux->img);
    
	/* Show the tracking zone in the full colored image*/
	cvRectangleR(obj->rawFlux->img,obj->trackingZone,cvScalar(0,0,255,0),1,8,0);

	/* Release memory */
    cvReleaseStructuringElement(&kernel);
    cvReleaseImage(&hsv);
 
    // If there is no pixel, we return a center outside the image, else we return the center of gravity
    if(obj->nbPixels > 0)
        obj->origin = cvPoint((int)(sumX / (obj->nbPixels)), (int)(sumY / (obj->nbPixels)));
    else
        obj->origin = cvPoint(-1, -1);
}

/*
 * Add a circle on the video that follow your colored object
 */
void addObjectToVideo(TrackedObject* obj) {
	
    /* Draw an object centered on its origin */
    if (obj->nbPixels > NB_PIXEL_THRESHOLD){
		switch (obj->shape){
			case CIRCLE:
				cvDrawCircle(obj->rawFlux->img, obj->origin, obj->width, TRACKED_OBJECT_DEFAULT_COLOR, 1,8,0);	/* Draw a circle around the origin */
				break;
			case RECTANGLE:
				cvRectangle(obj->rawFlux->img,obj->origin,cvPoint(obj->origin.x+obj->width,obj->origin.y+obj->height), TRACKED_OBJECT_DEFAULT_COLOR, 1,8,0);	/* Draw a rectangle frow the origin */
				break;
		}
	}
}

CvRect initWorkSpace(RaspiCamCvCapture * capture, char* window){
	struct VolatileRect workingSpace;
	struct ImageBroadcast flux;
	
	workingSpace.originDefined = 0;
	workingSpace.rectDefined = 0;
	initImageBroadcast(&flux, NULL, NULL, window, NULL);
	
	cvSetMouseCallback(window, getCurrentPointCoordinates, &workingSpace);
	printf("Definition of the working space \n");
	while(workingSpace.rectDefined == 0) {			/* wait for the definition of the workspace */
		updateImageFromCapture(&flux,capture);
		if(workingSpace.originDefined) {
			cvRectangleR(flux.img,workingSpace.rect,cvScalar(0,0,255,0),1,8,0);
		}
			showImage(&flux);		
		char keyPressed = cvWaitKey(30);
		switch (keyPressed){
			case 27:				/* ESC to reset the rectangle origin */
				workingSpace.originDefined = 0;
				break;
		}
	}
	printf("Working space defined\n");
	cvDestroyWindow(window);
	return workingSpace.rect;
}

