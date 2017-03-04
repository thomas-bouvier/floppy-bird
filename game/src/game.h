/*!
* \file game.h
* \brief File containing structures and prototypes for the game
*/
#ifndef GAME_H
#define GAME_H

/*!
* \struct Bird game.h
* \brief The bird that the player controls by taping on the screen or clicking on the mouse
*/
typedef struct{
    int x;              /*!< the bird abscissa coordonate */
    int y;              /*!< the bird ordonate coordonate */
    char * sprite;      /*!< the path of the bird sprite */
    int gravity;        /*!< the speed of climb/fall of the bird */
} Bird;

/*!
* \struct Pipe game.h
* \brief The pipes that the bird has to avoid
*/
typedef struct{
    int x;              /*!< the pipe abscissa coordonate */
    int y;              /*!< the pipe ordonate coordonate */
    char * sprite;      /*!< the path of the pipe sprite */
    int speed;          /*!< the speed of scrolling of the pipes */
} Pipe;

Bird * newBird(int bird_x, int bird_y, char * bird_sprite, int bird_gravity);
Pipe * newPipe(int pipe_x, int pipe_y, char * pipe_sprite, int pipe_speed);
void pipeScrolling(Pipe * pipe);



#endif // GAME_H
