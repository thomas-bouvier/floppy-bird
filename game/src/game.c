/*!
* \file game.c
* \brief File containing the functions to manage the game
*/
#include "game.h"

/*!
* \brief Allow to scroll the camera in the right direction
* \param[out] pipe the pipe to scroll
* \param[out] bird the bird that moves with the camera
*/
void cameraScrolling(Camera * camera, Bird * bird)
{
    camera->x += camera->speed;
    bird->x += camera->speed;
}




