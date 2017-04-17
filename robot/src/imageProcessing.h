#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H

#include <cv.h>
#include <highgui.h>
#include "configuration.h"
#include "tracking.h"
#include "imageBroadcast.h"

void binarisation(TrackedObject* obj)
void addObjectToVideo(TrackedObject* obj, int nbPixels);
CvRect initWorkSpace(RaspiCamCvCapture * capture, char* window);


#endif /*IMAGE_PROCESSING_H*/
