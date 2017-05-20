/*!
* \file obstacle.h
* \brief File containing structures and prototypes for the obstacles
*/
#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "constants.h"
#include "pipe.h"
#include "bird.h"
#include "list.h"
#include "file.h"
#include <stdlib.h>
#include <stdio.h>

/*!
*An obstacle contains two pipes, and the bird has to pass between them
*/
typedef struct Obstacle Obstacle;

/*!
The list of obstacle displayed on screen
*/
typedef struct List List;

/*!
* \struct Obstacle obstacle.h
* \brief An obstacle contains two pipes, and the bird has to pass between them
*/
struct Obstacle{
    Pipe upper;                     /*!< the upper pipe */
    Pipe lower;                     /*!< the lower pipe */
    int gap;                        /*!< the gap between two pipes */
    struct Obstacle * next;         /*!< the next obstacle in the list */
};

Obstacle * newObstacle(int number, int height_lower, int obstacle_gap, Obstacle * next_obstacle);
Obstacle * nextBirdObstacle(List * l, Bird * bird);
void freeObstacle(Obstacle * obstacle);
void createObstacleFromFile(FILE * level, int number, List * l);
void createObstacleRandomly(int number, List * l);
int obstaclePassed(Bird * bird, Obstacle * savedObstacle, Sound * sound);
void modifyGap(int score);

#endif // OBSTACLE_H
