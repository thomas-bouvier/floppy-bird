/*!
* \file obstacle.h
* \brief File containing structures and prototypes for the obstacles
*/
#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "constants.h"
#include "pipe.h"
#include "bird.h"
#include <stdlib.h>
#include <stdio.h>

/*!
* \struct Obstacle obstacle.h
* \brief An obstacle contains two pipes, and the bird has to pass between them
*/
typedef struct Obstacle{
    Pipe upper;             /*!< the upper pipe */
    Pipe lower;             /*!< the lower pipe */
    int gap;                /*!< the gap between two pipes */
    struct Obstacle * next;        /*!< the next obstacle in the list */
} Obstacle;

void initObstacle(Obstacle * obstacle, int number, int height_lower, int obstacle_gap);
Obstacle * nextObstacle(Obstacle * obstacle, Bird * Bird);
void freeObstacle(Obstacle * obstacle);

#endif // OBSTACLE_H
