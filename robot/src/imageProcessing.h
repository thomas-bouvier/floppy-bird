/*!
* \file tracking.h
* \brief File containing structures and prototypes of functions related to image processing
*/

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

/*!
* \brief Define the differents type of object we can ADD to an image
*/
enum {ELLIPSE,RECTANGLE,NONE};

void binarisation(TrackedObject* obj, boolean displayTrackZone);
void addObjectToVideo(TrackedObject* obj);
CvRect initWorkSpace(RaspiCamCvCapture * capture, char* window, FILE* loadFile);
void saveWorkingSpace(CvRect* workingSpace, FILE* saveFile);
CvScalar reverse(CvScalar colour);


#endif /*IMAGE_PROCESSING_H*/
