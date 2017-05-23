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

#include "../../ai/neat/src/generic_list.h"
#include "../../ai/neat/src/population.h"

#include <stdlib.h>
#include <stdio.h>

void startGame(GenericList * bird, Camera * camera, GenericList * obstacle_list, FILE * level, int levelFromFile);
void cameraScrolling(Camera * camera, GenericList * bird);
int createObstacle(Camera * camera, GenericList * obstacle_list, FILE * level, int number, int levelFromFile);
int deleteObstacle(Camera * camera, GenericList * obstacle_list);
int detectHit(Bird * bird, Obstacle * obstacle, Sound * sound);
int birdFall(Bird * bird, int simplifiedMode);
int updateScore(int score, Bird * bird, Obstacle * savedObstacle, Sound * sound);
int ratioBirdHeight(Bird * bird);
int ratioPipeHeight (Bird * bird, GenericList * obstacle_list);
int ratioPipeWidth (Bird * bird, Camera * camera, GenericList * obstacle_list);
void addBird(GenericList * bird);
void freeLists(GenericList * bird_list, GenericList * obstacle_list);

#endif // GAME_H
