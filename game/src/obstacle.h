/*!
* \file obstacle.h
* \brief File containing structures and prototypes for the obstacles
*/
#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "constants.h"
#include "pipe.h"
#include <stdlib.h>
#include <stdio.h>

/*!
* \struct Obstacle obstacle.h
* \brief An obstacle contains two pipes, and the bird has to pass between them
*/
typedef struct{
    Pipe * upper;           /*!< the upper pipe */
    Pipe * lower;           /*!< the lower pipe */
    int gap;                /*!< the gap between two pipes */
} Obstacle;

Obstacle * newObstacle(int number, int height_lower, int obstacle_gap);
void freeObstacle(Obstacle * obstacle);

#endif // OBSTACLE_H
