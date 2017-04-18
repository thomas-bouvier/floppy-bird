#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H

#include <cv.h>
#include <highgui.h>
#include <unistd.h>
#include "configuration.h"
#include "tracking.h"
#include "imageBroadcast.h"
#include "RaspiCamCV.h"

typedef struct TrackedObject TrackedObject;

void binarisation(TrackedObject* obj);
void addObjectToVideo(TrackedObject* obj);
CvRect initWorkSpace(RaspiCamCvCapture * capture, char* window, FILE* loadFile);
void saveWorkingSpace(CvRect* workingSpace, FILE* saveFile);


#endif /*IMAGE_PROCESSING_H*/
