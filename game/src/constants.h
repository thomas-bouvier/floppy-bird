/*!
* \file constants.h
* \brief File containing all constants for the game
*/
#ifndef CONSTANTS_H
#define CONSTANTS_H


/*! \def SCREEN_WIDTH
* \brief The width of the screen in pixels
*/
#define SCREEN_WIDTH 1024

/*! \def SCREEN_HEIGHT
* \brief The height of the screen in pixels
*/
#define SCREEN_HEIGHT 768

/*! \def CAMERA_SPEED
* \brief The speed of the camera
*/
#define CAMERA_SPEED 8


/*! \def GRAVITY
* \brief The gravity of the bird
*/
#define GRAVITY 3

/*! \def BIRD_X_OFFSET
* \brief The distance in pixels between the bird and the left side of the screen
*/
#define BIRD_X_OFFSET (SCREEN_WIDTH / 3)

/*! \def BIRD_SIZE
* \brief The size of a bird in pixels
*/
#define BIRD_SIZE 30

/*! \def BIRD_MAX_FALL_SPEED
* \brief The maximum speed of fall of the bird
*/
#define BIRD_MAX_FALL_SPEED 35

/*! \def BIRD_JUMP
* \brief The speed of a jump of the bird
*/
#define BIRD_JUMP -25


/*! \def PIPE_WIDTH
* \brief The width of a pipe in pixels
*/
#define PIPE_WIDTH 80

/*! \def PIPE_X_OFFSET
* \brief The distance in pixels between two obstacles
*/
#define PIPE_X_OFFSET 400

/*! \def PIPES_ON_SCREEN
* \brief The number of obstacles displayed on screen
*/
#define PIPES_ON_SCREEN (SCREEN_WIDTH/PIPE_X_OFFSET)


/*! \enum Action
* \brief The possible actions for the player
* \var NOTHING nothing to be done
* \var JUMP make the bird jump
* \var QUIT quit the game
*/
enum{NOTHING, JUMP, QUIT} Action;


#endif // CONSTANTS_H
