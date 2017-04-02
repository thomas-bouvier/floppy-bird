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
#define CAMERA_SPEED 5


/*! \def GRAVITY
* \brief The gravity of the bird
*/
#define GRAVITY 1

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
#define BIRD_JUMP -15


/*! \def PIPE_WIDTH
* \brief The width of a pipe in pixels
*/
#define PIPE_WIDTH 80

/*! \def PIPE_X_OFFSET
* \brief The distance in pixels between two obstacles
*/
#define PIPE_X_OFFSET 400

/*! \def OBSTACLE_GAP
* \brief The distance in pixels between the two pipes of an obstacle
*/
#define OBSTACLE_GAP 200

/*! \def PIPES_ON_SCREEN
* \brief The number of obstacles displayed on screen
*/
#define PIPES_ON_SCREEN ((SCREEN_WIDTH / PIPE_X_OFFSET) + 1)

/*! \def OBSTACLE_NUMBER
* \brief The number of obstacles maximum in the list
*/
#define OBSTACLE_NUMBER (PIPES_ON_SCREEN + 1)

/*! \def FRAME_PER_SECOND
* \brief the number of frame max per second
*/
#define FRAME_PER_SECOND 60

/*!
* The possible actions for the player
*/
typedef enum  Action Action;

/*!
* \enum Action
* \brief The possible actions for the player
*/
enum Action
{
    NOTHING,            /*! nothing to be done */
    JUMP,               /*! make the bird jump */
    QUIT,               /*! quit the game */
    PAUSE               /*! pause the game */
};


#endif // CONSTANTS_H
