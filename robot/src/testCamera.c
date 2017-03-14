#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>
#include <math.h>
#include <cv.h>
#include <highgui.h>
#include "RaspiCamCV.h"

#include "configuration.h"

int main(int argc, char *argv[]){
	
	RASPIVID_CONFIG * config = (RASPIVID_CONFIG*)malloc(sizeof(RASPIVID_CONFIG));
	
	config->width=960;
	config->height=720;
	config->bitrate=0;	// zero: leave as default
	config->framerate=0;
	config->monochrome=0;
	
	RaspiCamCvCapture * capture = (RaspiCamCvCapture *) raspiCamCvCreateCameraCapture2(0, config); 
	free(config);
	
	IplImage* img = raspiCamCvQueryFrame(capture);
	
  int height,width,step,channels;
  uchar *data;
  long int i,j,k;

  // get the image data
  height    = img->height;
  width     = img->width;
  step      = img->widthStep;
  channels  = img->nChannels;
  data      = (uchar *)img->imageData;
  printf("Processing a %dx%d image with %d channels\n",height,width,channels); 

  // create a window
  cvNamedWindow("mainWin", CV_WINDOW_AUTOSIZE); 
  cvMoveWindow("mainWin", 100, 100);

  // invert the image
 // for(i=0;i<height;i++) for(j=0;j<width;j++) for(k=0;k<channels;k++)
   // data[i*step+j*channels+k]=255-data[i*step+j*channels+k];

  // show the image
  cvShowImage("mainWin", img );

  // wait for a key
  cvWaitKey(0);

  // release the image
  cvReleaseImage(&img );
	
	
	
	
	
	return 0; 
}


