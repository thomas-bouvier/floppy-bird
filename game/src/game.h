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

void startGame(Bird * bird, Camera * camera, List * l, FILE * level, int levelFromFile);
void cameraScrolling(Camera * camera, Bird * bird);
int createObstacle(Camera * camera, List * l, FILE * level, int number, int levelFromFile);
int deleteObstacle(Camera * camera, List * l);
int detectHit(Bird * bird, Obstacle * obstacle);
int updateScore(int score, Bird * bird, Obstacle * savedObstacle);
int game(Bird * bird, Camera * camera, List * l, FILE * level, int event, int * number, Obstacle * savedObstacle, int * score, int levelFromFile);

#endif // GAME_H
