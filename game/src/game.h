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

void initGame(Bird * bird, Camera * camera, Obstacle * obstacle[PIPES_ON_SCREEN]);
void cameraScrolling(Camera * camera, Bird * bird);
int pipeCreation(Camera * camera, Obstacle * ostacle);
int detectHit(Bird * bird, Obstacle * obstacle);
int game(Bird * bird, Camera * camera, Obstacle * obstacle[PIPES_ON_SCREEN]);

#endif // GAME_H
