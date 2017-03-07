/*!
* \file camera.h
* \brief File containing structures and prototypes for the camera
*/
#ifndef CAMERA_H
#define CAMERA_H

#include "constants.h"
#include <stdlib.h>
#include <stdio.h>

/*!
*\struct Camera camera.h
*\brief The rectangle that moves with the bird
*/
typedef struct{
    int x;                  /*!< the abscissa of the camera */
    int y;                  /*!< the ordinate of the camera */
    int w;                  /*!< the width of the camera */
    int h;                  /*!< the height of the camera */
    int speed;              /*!< the speed of scrolling of the camera */
} Camera;

Camera * newCamera(int x, int camera_speed);

#endif // CAMERA_H

