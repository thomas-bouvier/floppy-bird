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
	Stylus stylus;
  attach(&stylus,PWM_PIN,STYLUS_CLICK_POSITION,STYLUS_REST_POSITION,PRESS_DELAY,REST_DELAY);
  enable(&stylus);

	RASPIVID_CONFIG * config = (RASPIVID_CONFIG*)malloc(sizeof(RASPIVID_CONFIG));
	
	config->width=960;
	config->height=720;
	config->bitrate=0;	// zero: leave as default
	config->framerate=0;
	config->monochrome=0;
	
	RaspiCamCvCapture * capture = (RaspiCamCvCapture *) raspiCamCvCreateCameraCapture2(0, config); 
	free(config);
	
	
CvFont font;
	double hScale=0.4;
	double vScale=0.4;
	int    lineWidth=1;

	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX|CV_FONT_ITALIC, hScale, vScale, 0, lineWidth, 8);

	cvNamedWindow("RaspiCamTest", 1);
	int exit =0;
	do {
		IplImage* image = raspiCamCvQueryFrame(capture);
		
		char text[200];
		sprintf(
			text
			, "w=%.0f h=%.0f fps=%.0f bitrate=%.0f monochrome=%.0f"
			, raspiCamCvGetCaptureProperty(capture, RPI_CAP_PROP_FRAME_WIDTH)
			, raspiCamCvGetCaptureProperty(capture, RPI_CAP_PROP_FRAME_HEIGHT)
			, raspiCamCvGetCaptureProperty(capture, RPI_CAP_PROP_FPS)
			, raspiCamCvGetCaptureProperty(capture, RPI_CAP_PROP_BITRATE)
			, raspiCamCvGetCaptureProperty(capture, RPI_CAP_PROP_MONOCHROME)
		);
		cvPutText (image, text, cvPoint(05, 40), &font, cvScalar(255, 255, 0, 0));
		
		sprintf(text, "Press ESC to exit");
		cvPutText (image, text, cvPoint(05, 80), &font, cvScalar(255, 255, 0, 0));
		
		cvShowImage("RaspiCamTest", image);
		
		char key = cvWaitKey(1);
		
		switch(key)	
		{
			case 32:		// space to click
				click(&stylus);
				printf("click\n");
				break;
			case 27:		// Esc to exit
				exit = 1;
				break;
			case 60:		// < (less than)
				raspiCamCvSetCaptureProperty(capture, RPI_CAP_PROP_FPS, 25);	// Currently NOOP
				break;
			case 62:		// > (greater than)
				raspiCamCvSetCaptureProperty(capture, RPI_CAP_PROP_FPS, 30);	// Currently NOOP
				break;
		}
		update(&stylus);
		
	} while (!exit);

	cvDestroyWindow("RaspiCamTest");
	raspiCamCvReleaseCapture(&capture);
	disable(&stylus);
	
	return 0; 
}


