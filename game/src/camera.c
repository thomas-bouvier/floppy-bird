/*!
* \file camera.c
* \brief File containing the functions to manage the camera
*/
#include "camera.h"

/*!
* \brief Create a camera
* \param[in] x the camera abscissa
* \param[in] camera_speed the camera speed of scrolling
* \return Return the created camera, NULL if error
*/
Camera * newCamera(int x, int camera_speed)
{
    Camera * new_camera = (Camera*) malloc(sizeof(Camera));
    if(new_camera == NULL)
    {
        fprintf(stderr, "Camera allocation problem");
        return NULL;
    }
    new_camera->x = x;
    new_camera->y = 0;
    new_camera->w = SCREEN_WIDTH;
    new_camera->h = SCREEN_HEIGHT;
    new_camera->speed = camera_speed;
    return new_camera;
}
