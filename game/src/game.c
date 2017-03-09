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
* \brief Deallocate an obstacle when it leaves the screen by the left, and create a new obstacle on the right side of the screen. Update the table of obstacle
* \param[out] camera the view of the game
* \param[out] obstacle the table of obstacle that appears on the screen
* \param[in] number the obstacle number of the new obstacle
* \param[in] height_lower the ordinate of the lower pipe of the new obstacle
* \param[in] obstacle_gap the gap between two pipes of the new obstacle
*/
int obstacleCreation(Camera * camera, Obstacle * obstacle[], int number, int height_lower, int obstacle_gap)
{
    if ((obstacle[0]->lower->x + PIPE_WIDTH) < camera->x)
    {
        freeObstacle(obstacle[0]);
        int i;
        for (i=0 ; i<PIPES_ON_SCREEN - 1 ; ++i)
        {
            obstacle[i] = obstacle[i + 1];
        }
        obstacle[PIPES_ON_SCREEN - 1] = newObstacle(number, height_lower, obstacle_gap);
    }
    return EXIT_SUCCESS;
}



