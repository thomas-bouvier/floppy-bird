#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>
#include <math.h>
#include <cv.h>
#include <highgui.h>
#include <sys/time.h>
#include <time.h>

#include "RaspiCamCV.h"
#include "configuration.h"
#include "stylus.h"

// Maths methods
#define max(a, b) ((a) > (b) ? (a) : (b))
#define min(a, b) ((a) < (b) ? (a) : (b))
#define abs(x) ((x) > 0 ? (x) : -(x))
#define sign(x) ((x) > 0 ? 1 : -1)

// Color tracked and our tolerance towards it
int h = 0, s = 0, v = 0, Htolerance = 5, Stolerance = 30;
IplImage* image;


enum {CIRCLE,RECTANGLE};

struct VolatileRect{
	CvRect rect;
	int origineDefined;
	int rectDefined;
};

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

    // Create the mask &initialize it to white (no color detected)
    mask = cvCreateImage(cvGetSize(image), image->depth, 1);

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
 * Add a circle on the video that fellow your colored object
 */
void addObjectToVideo(char* window, IplImage* image, int shape, CvPoint position, int nbPixels) {

    // Draw an object (circle) centered on the calculated center of gravity
    if (nbPixels > NB_PIXEL_THRESHOLD){
		switch (shape){
			case CIRCLE:
				cvDrawCircle(image, position, BIRD_CIRCLE_DIAMETER, CV_RGB(255, 0, 0), 1,8,0);
				break;
			case RECTANGLE:
				break;
		}
		cvShowImage(window, image);
	}


    // We show the image on the window


}

/*
 * Get the color of the pixel where the mouse has clicked
 * We put this color as model color (the color we want to tracked)
 */
void getObjectColor(int event, int x, int y, int flags, void *param) {

    // Vars
    CvScalar pixel;
    IplImage *hsv;
    param = NULL;

    if(event == CV_EVENT_LBUTTONUP) {

        // Get the hsv image
        hsv = cvCloneImage(image);
        cvCvtColor(image, hsv, CV_BGR2HSV);

        // Get the selected pixel
        pixel = cvGet2D(hsv, y, x);

        // Change the value of the tracked color with the color of the selected pixel
        h = (int)pixel.val[0];
        s = (int)pixel.val[1];
        v = (int)pixel.val[2];

        // Release the memory of the hsv image
            cvReleaseImage(&hsv);

    }

}

void getCurrentPointCoordinates(int event, int x, int y, int flags, void *param){
	struct VolatileRect * workingSpace = (struct VolatileRect *)param;
	if(workingSpace->origineDefined == 1 && event == CV_EVENT_MOUSEMOVE){
		CvPoint origin = cvPoint(workingSpace->rect.x,workingSpace->rect.y);
		workingSpace->rect = cvRect(min(x,origin.x),min(y,origin.y),abs(x-origin.x),abs(y-origin.y));
	}

	if(event == CV_EVENT_LBUTTONUP){
		printf("click at x=%d \ty=%d\n",x,y);
		if(workingSpace->origineDefined){
			CvPoint origin = cvPoint(workingSpace->rect.x,workingSpace->rect.y);
//			printf("Working area : \nx :\t%d\t%d\ny :\t%d\t%d\n",point1.x,point2.x,point1.y,point2.y);
			workingSpace->rect = cvRect(min(x,origin.x),min(y,origin.y),abs(x-origin.x),abs(y-origin.y));
			workingSpace->rectDefined = 1;
		} else {
			workingSpace->rect.x = x;
			workingSpace->rect.y = y;
			workingSpace->origineDefined = 1;
		}
	}
}

=======
#include "imageBroadcast.h"
#include "tracking.h"
#include "imageProcessing.h"
#include "pipeTracking.h"

/*!
* \brief init capture function : init and configure the capture
*/
RaspiCamCvCapture * initCapture(){
	RASPIVID_CONFIG * config = (RASPIVID_CONFIG*)malloc(sizeof(RASPIVID_CONFIG));

	config->width = CAPTURE_WIDTH;
	config->height = CAPTURE_HEIGHT;
	config->bitrate = 0;	/* zero: leave as default */
	config->framerate = 0;
	config->monochrome = 0;

	RaspiCamCvCapture * capture = (RaspiCamCvCapture *) raspiCamCvCreateCameraCapture2(0, config);
	free(config);
	return capture;
}

void initFont(CvFont * font){
	double hScale=0.4;
	double vScale=0.4;
	int    lineWidth=1;

	cvInitFont(font, CV_FONT_HERSHEY_SIMPLEX|CV_FONT_ITALIC, hScale, vScale, 0, lineWidth, 8);
}

CvRect initWorkSpace(RaspiCamCvCapture * capture, char* window){
	struct VolatileRect workingSpace;
	workingSpace.origineDefined = 0;
	workingSpace.rectDefined = 0;

	cvNamedWindow(window, CV_WINDOW_AUTOSIZE);
	cvMoveWindow(window, 0, 100);
	cvSetMouseCallback(window, getCurrentPointCoordinates, &workingSpace);
	printf("Definition of the working space \n");
	while(workingSpace.rectDefined == 0) {			// wait for the definition of the workspace
		image = raspiCamCvQueryFrame(capture);
		if(workingSpace.origineDefined) {
			cvRectangleR(image,workingSpace.rect,cvScalar(0,0,255,0),1,8,0);
		}
			cvShowImage(window, image);
		char keyPressed = cvWaitKey(30);
		switch (keyPressed){
			case 27:				//ESC to reset the rectangle origin
				workingSpace.origineDefined = 0;
				break;
		}
	}
	printf("Working space defined\n");
	cvDestroyWindow(window);
	return workingSpace.rect;
}


int main(int argc, char *argv[]){
	/* Variables*/
	RaspiCamCvCapture * capture = initCapture();
	struct TrackedObject birdTracker;
	struct TrackedObject* pipeTracker = (struct TrackedObject *)malloc(NB_PIPE_TRACKER*sizeof(struct TrackedObject));
	struct PipeDynamicTracker pipeDynTracker;
	struct ImageBroadcast cameraFlux;
	struct ImageBroadcast birdBinFlux;
	struct ImageBroadcast pipeBinFlux;
	//CvFont * font = (CvFont *)malloc(sizeof(CvFont));
	CvRect workingSpace;
	Stylus stylus;
	char colourTrackingWindow[] = "Color Tracking";
	char birdWindow[] = "Bird tracking";
	char pipeWindow[] = "Pipe tracking";
	char workSpaceDefWindow[] = "WorkingSpaceDefinition";

	int c;
	FILE* loadFile = NULL;
	FILE* saveFile = NULL;
	FILE* logFile = NULL;

	while((c = getopt(argc,argv,"l:s:d:")) != -1)
		switch(c){
			case 'l':
				loadFile = fopen(optarg,"rb");
				if(saveFile!=NULL){
					fprintf(stderr,"cannot load and save a file at the same time");
					return 1;
				}
				break;
			case 's':
				saveFile = fopen(optarg,"wb");
				if(loadFile!=NULL){
					fprintf(stderr,"cannot load and save a file at the same time");
					return 1;
				}
				break;
			case 'd':		/* Save data into a log file */
				logFile = fopen(optarg,"w");
				if(logFile==NULL){
					fprintf(stderr,"cannot open logfile %s\n",optarg);
					return 1;
				}
				break;
			default:
				printf("Unknown option :\nUsage : -l loadFileName OR -s saveFileName\n");
				return 1;
		}


	/* Setup */
	wiringPiSetup();	/* Setup the GPIO */
	attach(&stylus,PWM_PIN,STYLUS_CLICK_POSITION,STYLUS_REST_POSITION,PRESS_DELAY,REST_DELAY);
	enable(&stylus);
	//initFont(font);
	workingSpace = initWorkSpace(capture, workSpaceDefWindow,loadFile);

	initImageBroadcast(&cameraFlux, NULL, &workingSpace, colourTrackingWindow, NULL);
	loadImage(&cameraFlux,capture);
	initImageBroadcast(&birdBinFlux, NULL, &workingSpace, birdWindow, NULL);
	initImageBroadcast(&pipeBinFlux, NULL, &workingSpace, pipeWindow, NULL);
	int i;
	if(loadFile == NULL){
		int width = cameraFlux.img->roi->width;
		int height = cameraFlux.img->roi->height;
		initTrackedObject(&birdTracker,0,0,0,&cameraFlux,&birdBinFlux,cvRect(((width/3) - (width*RELATIVE_WIDTH_BIRD_TRACKING_ZONE/2)),0,width*RELATIVE_WIDTH_BIRD_TRACKING_ZONE,height),RECTANGLE);
		for(i = 0; i < NB_PIPE_TRACKER; i++){
			initTrackedObject(&pipeTracker[0]+i*sizeof(TrackedObject*),0,0,0,&cameraFlux,&pipeBinFlux,cvRect(width - (width*RELATIVE_WIDTH_BIRD_TRACKING_ZONE) -1,0,width*RELATIVE_WIDTH_PIPE_TRACKING_ZONE,height),RECTANGLE);
		}
		initPipeDynamicTracker(&pipeDynTracker, &pipeTracker);
    } else {		/* We load data form the file */
		loadTrackedObject(&birdTracker,&cameraFlux,&birdBinFlux,loadFile);
		for(i = 0; i < NB_PIPE_TRACKER; i++){
			loadTrackedObject(&pipeTracker[0]+i*sizeof(TrackedObject*),&cameraFlux,&pipeBinFlux,loadFile);
		}
		initPipeDynamicTracker(&pipeDynTracker, &pipeTracker);
	}

    if(logFile != NULL){
		fprintf(logFile,"Time (us);Bird height;Pipe height;Bird - Pipe relative distance\n");
	}
	int exit =0;
	struct timeval startTime, lastTime, currentTime;
	gettimeofday(&startTime,NULL);
	gettimeofday(&lastTime,NULL);
	printf("initOK\n");
	do {
		loadImage(&cameraFlux,capture);
		updateTracking(&birdTracker);
		updatePipeDynamicTracker(&pipeDynTracker);
		/* Time computing */
		gettimeofday(&currentTime,NULL);		/* update the time */
		long int frameTime = (currentTime.tv_sec - lastTime.tv_sec)*1000000+currentTime.tv_usec- lastTime.tv_usec;
		gettimeofday(&lastTime,NULL);
		float frameRate = 1000000.0/frameTime;
		printf("FPS : %f \t",frameRate);
		showImage(&cameraFlux);
		CvPoint pipe = nextPipe(&pipeDynTracker,birdTracker.origin.x - birdTracker.width/2);
		float birdHeight = getRelativeDistance(&birdTracker,UP);
		float pipeHeight = 1-((float)pipe.y/(pipeBinFlux.img->height));
		float pipeBirdDist = (float)pipe.x/birdTracker.origin.x;
		printf("pipe : h%f w%f ; bird : h%f\n",pipeHeight,pipeBirdDist,birdHeight);
		if(logFile != NULL){
			fprintf(logFile,"%ld;%f;%f;%f\n",(long int)((currentTime.tv_sec - startTime.tv_sec)*1000000+currentTime.tv_usec- startTime.tv_usec),birdHeight,pipeHeight,pipeBirdDist);
		}

		char key = cvWaitKey(1);

		switch(key)
		{
			case 'b':		/* b to select the color of the bird */
				cvSetMouseCallback(colourTrackingWindow, getObjectColor,&birdTracker);
				break;
			case 'p':		/* p to select the color of the pipe */
				cvSetMouseCallback(colourTrackingWindow, getPipeColor,&pipeDynTracker);
				break;
			case 32:		/* space to click */
				click(&stylus);
				printf("click\n");
				break;
			case 27:		/* Esc to exit */
				exit = 1;
				break;
		}
		update(&stylus);

	} while (!exit);

	if(loadFile != NULL)
		fclose(loadFile);
	if(saveFile != NULL){
		saveWorkingSpace(&workingSpace,saveFile);
		saveTrackedObject(&birdTracker,saveFile);
		for(i = 0; i < NB_PIPE_TRACKER; i++){
			saveTrackedObject(&pipeTracker[0]+i*sizeof(TrackedObject*),saveFile);
		}
		fclose(saveFile);
	}
	if(logFile != NULL)
		fclose(logFile);
    cvDestroyAllWindows();
    /* Release memory */
    releaseTrackingImageMemory(&birdTracker);
    for(i = 0; i < NB_PIPE_TRACKER; i++){
		releaseTrackingImageMemory(&pipeTracker[0]+i*sizeof(TrackedObject*));
	}
	free(pipeTracker);
	raspiCamCvReleaseCapture(&capture);

	/* disabling servomotor */
	disable(&stylus);

	return 0;
}
