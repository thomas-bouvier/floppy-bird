/*!
* \file game.h
* \brief File containing structures and prototypes for the game
*/
#ifndef GAME_H
#define GAME_H

#include "constants.h"
#include "bird.h"
#include "obstacle.h"
#include "camera.h"
#include <stdlib.h>
#include <stdio.h>








//Model
void cameraScrolling(Camera * camera, Bird * bird);

/*
//View
void drawRectangle(SDL_Surface * surface, SDL_Rect * rect, int r, int g, int b);
void drawObstacle(SDL_Surface * surface, Obstacle * obstacle);
*/


#endif // GAME_H
