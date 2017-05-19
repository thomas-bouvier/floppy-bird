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

#include "../../ai/neat/src/generic_list.h"
#include "../../ai/neat/src/population.h"

#include <stdlib.h>
#include <stdio.h>

void startGame(GenericList * bird, Camera * camera, List * l, FILE * level, int levelFromFile);
void startGameNeat(GenericList * bird_list, Camera * camera, List * l, FILE * level, int levelFromFile, MatingPool * pool);
void cameraScrolling(Camera * camera, GenericList * bird);
void modifySpeed(int score, Camera * camera);
int createObstacle(Camera * camera, List * l, FILE * level, int number, int levelFromFile);
int deleteObstacle(Camera * camera, List * l);
int detectHit(Bird * bird, Obstacle * obstacle, Sound * sound);
int birdFall(Bird * bird, int simplifiedMode);
int updateScore(int score, Bird * bird, Obstacle * savedObstacle, Sound * sound);
int ratioBirdHeight(Bird * bird);
int ratioPipeHeight (Bird * bird, List * l);
int ratioPipeWidth (Bird * bird, Camera * camera, List * l);
void addBird(GenericList * bird);

#endif // GAME_H
