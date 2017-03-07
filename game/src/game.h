/*!
* \file game.h
* \brief File containing structures and prototypes for the game
*/
#ifndef GAME_H
#define GAME_H

#include "constants.h"
#include "bird.h"






/*!
*\struct Camera game.h
*\brief The rectangle that moves with the bird
*/
typedef struct{
    SDL_Rect * view;        /*!< the field of view of the camera */
    int speed;              /*!< the speed of scrolling of the camera */
} Camera;

//Constructors

Obstacle * newObstacle(int number, int height_lower, int obstacle_gap);
Camera * newCamera(int x, int camera_speed);

//Model
void cameraScrolling(Camera * camera, Bird * bird);

//View
void drawRectangle(SDL_Surface * surface, SDL_Rect * rect, int r, int g, int b);
void drawObstacle(SDL_Surface * surface, Obstacle * obstacle);

void freeObstacle(Obstacle * obstacle);

#endif // GAME_H
