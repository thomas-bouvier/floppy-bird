#include "imageProcessing.h"

/*
 * Transform the image into a two colored image, one color for the color we want to track, another color for the others colors
 * From this image, we get two datas : the number of pixel detected, and the center of gravity of these pixel
 */
CvPoint binarisation(IplImage* image, int *nbPixels, char* window) {
 
    int x, y;
    IplImage *hsv, *mask;
    IplConvKernel *kernel;
    int sommeX = 0, sommeY = 0;
    *nbPixels = 0;
	
	int zoneWidth = 50;		// The zone width in wich the colour will be tracked
	CvRect roi = cvRect(((image->roi->width/3) - (zoneWidth/2)),0,zoneWidth,image->roi->height);
	
    mask = cvCreateImage(cvGetSize(image), image->depth, 1);	/* Create the mask &initialize it to white (no color detected) */
	
    // Create the hsv image
    hsv = cvCloneImage(image);
    cvCvtColor(image, hsv, CV_BGR2HSV);
 
	
	
    // We create the mask
    cvInRangeS(hsv, cvScalar(h - Htolerance -1, s - Stolerance, 0,0), cvScalar(h + Htolerance -1, s + Stolerance, 255,0), mask);
	
    // Create kernels for the morphological operation
    kernel = cvCreateStructuringElementEx(5, 5, 2, 2, CV_SHAPE_RECT,NULL);
 
    // Morphological opening (inverse because we have white pixels on black background)
    cvDilate(mask, mask, kernel, 2);
    cvErode(mask, mask, kernel, 2);  
    cvSetImageROI(mask,roi);
	
    // We go through the mask to look for the tracked object and get its gravity center
    for(x = mask->roi->xOffset; x < mask->roi->width + mask->roi->xOffset; x++) {
        for(y = mask->roi->yOffset; y < mask->roi->height + mask->roi->yOffset ; y++) { 
 
            // If its a tracked pixel, count it to the center of gravity's calcul
            if((mask->imageData[x+ y*mask->widthStep]) == 255) {
                sommeX += x;
                sommeY += y;
                (*nbPixels)++;
            }
        }
    }
    
    
    
      for(x = mask->roi->xOffset; x < mask->roi->width - mask->roi->xOffset; x++) {
        for(y = mask->roi->yOffset; y < mask->roi->height - mask->roi->yOffset ; y++) { 
 
            // If its a tracked pixel, count it to the center of gravity's calcul
            if(((uchar *)(mask->imageData + y*mask->widthStep))[x] == 255) {
                sommeX += x;
                sommeY += y;
                (*nbPixels)++;
            }
        }
    }
	
	cvResetImageROI(mask);
    // Show the result of the mask image
    if(window != NULL) 
		cvShowImage(window, mask);
   
	cvRectangleR(image,roi,cvScalar(0,0,255,0),1,8,0);
    // We release the memory of kernels
    cvReleaseStructuringElement(&kernel);
 
    // We release the memory of the mask
    cvReleaseImage(&mask);
    // We release the memory of the hsv image
    cvReleaseImage(&hsv);
 
    // If there is no pixel, we return a center outside the image, else we return the center of gravity
    if(*nbPixels > 0)
        return cvPoint((int)(sommeX / (*nbPixels)), (int)(sommeY / (*nbPixels)));
    else
        return cvPoint(-1, -1);
}

/*
 * Add a circle on the video that follow your colored object
 */
void addObjectToVideo(TrackedObject* obj, int nbPixels) {
	
    /* Draw an object centered on its origin */
    if (nbPixels > NB_PIXEL_THRESHOLD){
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
	initImageBroadcast(&flux, NULL,window,NULL);
	
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

