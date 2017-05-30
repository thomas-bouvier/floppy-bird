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

/*!
* \brief Modify the speed of game if it is run in normal mode
* \param[in] score the score of the player
* \param[out] camera the camera whose speed is changed
*
* If the score is < 20, game is run in LOW speed ; score < 40 : NORMAL speed ; score < 60 : HIGH speed ; score > 60 : EXTREME speed
*
*/
void modifySpeed(int score, Camera * camera)
{
    if(score > 60)
        camera->speed = EXTREME;
    else if(score > 40)
        camera->speed = HIGH;
    else if(score > 20)
        camera->speed = NORMAL;
    else
        camera->speed = LOW;
}
