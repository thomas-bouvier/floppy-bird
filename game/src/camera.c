/*!
* \file camera.c
* \brief File containing the functions to manage the camera
*/
#include "camera.h"

/*!
* \brief Initialize the parameters of a given camera
* \param[out] new_camera the given camera
* \param[in] x the camera abscissa
* \param[in] camera_speed the camera speed of scrolling
*/
void initCamera(Camera * new_camera, int x, int camera_speed)
{
    new_camera->x = x;
    new_camera->y = 0;
    new_camera->w = SCREEN_WIDTH;
    new_camera->h = SCREEN_HEIGHT;
    new_camera->speed = camera_speed;
}
