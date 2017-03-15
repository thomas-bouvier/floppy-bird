/*!
* \file constants.h
* \brief File containing all constants for the game
*/
#ifndef CONSTANTS_H
#define CONSTANTS_H


#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

#define GRAVITY 2
#define BIRD_X_OFFSET (SCREEN_WIDTH / 3)
#define BIRD_SIZE 50
#define BIRD_MAX_FALL_SPEED 20
#define BIRD_JUMP -20

#define PIPE_WIDTH 80
#define PIPE_X_OFFSET 400
#define PIPES_ON_SCREEN (SCREEN_WIDTH/PIPE_X_OFFSET)

enum{NOTHING, JUMP, QUIT};

#endif // CONSTANTS_H
