#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>
#include <math.h>
#include <cv.h>
#include <highgui.h>
#include <sys/time.h>
#include "RaspiCamCV.h"

#include "configuration.h"
#include "stylus.h"

int main(int argc, char *argv[]){
	
	wiringPiSetup();	// Setup the GPIO
	/*pinMode(PWM_PIN,PWM_OUTPUT);
	pwmSetMode(PWM_MODE_MS);
	pwmSetRange(PWM_RANGE);
	pwmSetClock(PWM_CLOCK);*/

	/*RASPIVID_CONFIG * config = (RASPIVID_CONFIG*)malloc(sizeof(RASPIVID_CONFIG));
	
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
  */
  
  Stylus stylus;
  attach(&stylus,PWM_PIN,STYLUS_CLICK_POSITION,STYLUS_REST_POSITION,PRESS_DELAY,REST_DELAY);
  enable(&stylus);
  
  char c = '\0';
  //struct timeval tvalBefore, tvalAfter;
  //gettimeofday(&tvalBefore,NULL);
  while(1){
	  //gettimeofday(&tvalAfter,NULL);
	  //printf("%d\n",(int)*fgets(c,1,stdin));
	  //printf("test");
	//if(scanf("%c",&c)){
	//	pwmWrite(PWM_PIN,80);
	//	delay(PRESS_DELAY);
	//	pwmWrite(PWM_PIN,95);
	//}
		click(&stylus);
		int i;
		for (i = 0; i<1000;i++){
			delay(1);
			update(&stylus);
		}
		//printf("time : %f\n",(float)clock()/CLOCKS_PER_SEC);
	//	printf("Time in microseconds: %ld microseconds\n",
      //      ((tvalAfter.tv_sec - tvalBefore.tv_sec)*1000000+tvalAfter.tv_usec) - tvalBefore.tv_usec); 
	//
  }
	
	
	
	
	
	return 0; 
}


