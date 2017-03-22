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
#include "list.h"
#include <stdlib.h>
#include <stdio.h>

void startGame(Bird * bird, Camera * camera, List * l);
void cameraScrolling(Camera * camera, Bird * bird);
int createObstacle(Camera * camera, List * l, int number, int height_lower, int obstacle_gap);
int deleteObstacle(Camera * camera, List * l);
int detectHit(Bird * bird, Obstacle * obstacle);
int game(Bird * bird, Camera * camera, List * l, int event,int heightPipe, int number);

#endif // GAME_H
