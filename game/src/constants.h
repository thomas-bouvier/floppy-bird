/*!
* \file constants.h
* \brief File containing all constants for the game
*/
#ifndef CONSTANTS_H
#define CONSTANTS_H


#define SCREEN_WIDTH 1024
#define SCREEN_HEIGHT 768

#define GRAVITY 10
#define BIRD_X_OFFSET (SCREEN_WIDTH / 3)
#define BIRD_SIZE 70
#define BIRD_MAX_FALL_SPEED 100
#define BIRD_JUMP 15

#define PIPE_WIDTH 50
#define PIPE_X_OFFSET 300
#define PIPES_ON_SCREEN (SCREEN_WIDTH/PIPE_X_OFFSET)

enum{NOTHING, JUMP, QUIT};

#endif // CONSTANTS_H
