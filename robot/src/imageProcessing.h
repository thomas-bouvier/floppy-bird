#ifndef IMAGE_PROCESSING_H
#define IMAGE_PROCESSING_H

#include <cv.h>
#include <highgui.h>
#include "configuration.h"

CvPoint binarisation(IplImage* image, int *nbPixels, char* window);


#endif /*IMAGE_PROCESSING_H*/
