/*!
* \file obstacle.c
* \brief File containing the functions to manage an obstacle
*/
#include "obstacle.h"

/*!
*\brief Create an obstacle
* \param[in] number the obstacle number
* \param[in] height_lower the ordinate of the lower pipe
* \param[in] obstacle_gap the gap between two pipes
* \return Return the created obstacle, NULL if error
*/
Obstacle * newObstacle(int number, int height_lower, int obstacle_gap)
{
    Obstacle * new_obstacle = (Obstacle*) malloc(sizeof(Obstacle));
    if(new_obstacle == NULL)
    {
        fprintf(stderr, "Obstacle allocation problem");
        return NULL;
    }
    Pipe * low = newPipe(number, SCREEN_HEIGHT - height_lower, height_lower);            //Lower pipe
    Pipe * up = newPipe(number, 0, SCREEN_HEIGHT - (height_lower + obstacle_gap));       //Upper pipe
    new_obstacle->lower = low;
    new_obstacle->upper = up;
    new_obstacle->gap = obstacle_gap;
    return new_obstacle;
}

/*!
* \brief Indicate the next obstacle for the bird
* \param[in] obstacle[] the table of obstacle that appears on screen
* \return Return the next obstacle for the bird. If the bird is between between an obstacle, it returns the
*/
Obstacle * nextObstacle(Obstacle * obstacle, Bird * bird)
{
    int i;
    for (i=0 ; i<PIPES_ON_SCREEN ; ++i)
    {
        if ((*(obstacle+i)).lower->x + PIPE_WIDTH > bird->x + bird->w/2)
            return (Obstacle *) obstacle+i;
    }
    return NULL;
}

/*!
* \brief Deallocate memory of the obstacle and his pipes
* \param[out] obstacle the obstacle to deallocate
*/
void freeObstacle(Obstacle * obstacle)
{
    free(obstacle->lower);
    free(obstacle->upper);
    free(obstacle);
}
