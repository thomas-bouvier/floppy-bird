/*!
* \file game.h
* \brief File containing structures and prototypes for the game
*/
#ifndef GAME_H
#define GAME_H
#include "constants.h"
#include <SDL2/SDL.h>

/*!
* \struct Bird game.h
* \brief The bird that the player controls by taping on the screen or clicking on the mouse
*/
typedef struct{
    SDL_Rect * coordinates;     /*!< the bird ordinate */
    int dir_y;                  /*!< the speed of climb/fall of the bird */
} Bird;

/*!
* \struct Pipe game.h
* \brief The pipes that the bird has to avoid
*/
typedef struct{
    SDL_Rect * coordinates;     /*!< the coordinates of the pipe */
} Pipe;

/*!
* \struct Obstacle game.h
* \brief An obstacle contains two pipes, and the bird has to pass between them
*/
typedef struct{
    Pipe * upper;           /*!< the upper pipe */
    Pipe * lower;           /*!< the lower pipe */
    int gap;                /*!< the gap between two pipes */
} Obstacle;

/*!
*\struct Camera game.h
*\brief The rectangle that moves with the bird
*/
typedef struct{
    SDL_Rect * view;        /*!< the field of view of the camera */
    int speed;              /*!< the speed of scrolling of the camera */
} Camera;

Bird * newBird(int bird_x, int bird_y, char * bird_path);
Pipe * newPipe(int pipe_x, int pipe_y, int pipe_h);
Obstacle * newObstacle(int number, int height_lower, int obstacle_gap);
Camera * newCamera(int x, int camera_speed);
void updateBirdY(Bird * bird);
void cameraScrolling(Camera * camera);

#endif // GAME_H
