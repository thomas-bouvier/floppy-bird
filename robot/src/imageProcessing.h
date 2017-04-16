#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H

#include <cv.h>
#include <highgui.h>
#include "configuration.h"
#include "tracking.h"
#include "imageBroadcast.h"

CvPoint binarisation(IplImage* image, int *nbPixels, char* window);
void addObjectToVideo(TrackedObject* obj, int nbPixels);
CvRect initWorkSpace(RaspiCamCvCapture * capture, char* window);


#endif /*IMAGE_PROCESSING_H*/
