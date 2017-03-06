/*!
* \file game.h
* \brief File containing structures and prototypes for the game
*/
#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>

/*!
* \struct Bird game.h
* \brief The bird that the player controls by taping on the screen or clicking on the mouse
*/
typedef struct{
    int x;                  /*!< the bird abscissa coordonate */
    int y;                  /*!< the bird ordonate coordonate */
    SDL_Surface* surface;   /*!< the bird surface */
    int dirY;               /*!< the speed of climb/fall of the bird */
} Bird;

/*!
* \struct Pipe game.h
* \brief The pipes that the bird has to avoid
*/
typedef struct{
    int x;                  /*!< the pipe abscissa coordonate */
    int y;                  /*!< the pipe ordonate coordonate */
    SDL_Surface* surface;   /*!< the pipe surface */
} Pipe;

Bird * newBird(int bird_x, int bird_y, char * bird_path);
Pipe * newPipe(int pipe_x, int pipe_y, char * pipe_path);



#endif // GAME_H
