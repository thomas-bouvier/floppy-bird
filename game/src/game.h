/*!
* \file game.h
* \brief File containing the prototypes for the game
*/
#ifndef GAME_H
#define GAME_H

#include "constants.h"
#include "bird.h"
#include "obstacle.h"
#include "camera.h"
#include <stdlib.h>
#include <stdio.h>

void initGame(Bird * bird, Camera * camera, Obstacle * obstacle[]);
void cameraScrolling(Camera * camera, Bird * bird);
int obstacleCreation(Camera * camera, Obstacle * ostacle[], int number, int height_lower, int obstacle_gap);
int detectHit(Bird * bird, Obstacle * obstacle);
int game(Bird * bird, Camera * camera, Obstacle * obstacle[],int event,int heightPipe, int number);
void freeAll(Bird * bird, Obstacle * obstacle[], Camera * camera);

#endif // GAME_H
