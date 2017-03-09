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
/*!
* \brief Detect the collisions
* \param[in] bird the bird to test the collision with
* \param[in] obstacle the only obstacle that the bird can hit
* \return 1 if the bird hit something 0 in other cases
*/
int detectHit(Bird * bird, Obstacle * obstacle)
{
    int h = 0;
    if(bird->y + bird->h/2 >= SCREEN_HEIGHT) h = 1; //detect the collision with the ground
    if(bird->x + bird->w/2 >= obstacle->lower->x)
    {
        if(bird->y - bird->h/2 <= obstacle->upper->y + obstacle->upper->h) h = 1; //collision with the upper pipe
        if(bird->y + bird->h/2 >= obstacle->lower->y) h = 1;                      //collision with the lower pipe
    }
    return h;
}
