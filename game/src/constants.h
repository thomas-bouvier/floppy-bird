/*!
* \file constants.h
* \brief File containing all constants for the game
*/
#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <SDL2/SDL.h>


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
#define BIRD_SIZE 40

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

/*! \def MIN_HEIGHT_LOWER
* \brief The minimum height of the lower pipe of an obstacle
*/
#define MIN_HEIGHT_LOWER 100

/*! \def MAX_HEIGHT_LOWER
* \brief The maximum height of the lower pipe of an obstacle
*/
#define MAX_HEIGHT_LOWER 500

/*! \def NUMBER_OF_OBSTACLE_SIZES
* \brief The number of heights possible for an obstacle
*/
#define NUMBER_OF_OBSTACLE_SIZES 8

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
    PAUSE,              /*! pause the game */
    RESUME              /*! resume game when it was paused */
};

/*!
* The possible sounds for the bird
*/
typedef enum Sound Sound;

/*!
* \enum Sound
* \brief The possible sounds for the bird
*/
enum Sound
{
    NOSOUND,            /*! nothing to be played */
    JUMPSOUND,        	/*! sound when the bird jump */
    OBSTACLE,           /*! sound when the bird pass an obstacle */
    DEATH               /*! sound when the bird die */
};

/*!
* The possible modes of game
*/
typedef enum Mode Mode;

/*!
* \enum Mode
* \brief The possible modes of game
*/
enum Mode
{
    WAIT,               /*! waiting for a choice */
    PLAY,               /*! game played normally, with the user */
    IA1,                /*! game played with the first IA (Q learning) */
    IA2,                /*! game played with the second IA (NEAT) */
    QUITGAME            /*! quit the game */
};

/*!
* The speed of scrolling of the game
*/
typedef enum  Speed Speed;

/*!
* \enum Speed
* \brief The speed of scrolling of the game
*/
enum Speed
{
    LOW = 5,            /*! low speed */
    NORMAL = 6,         /*! normal speed */
    HIGH = 7,           /*! high speed */
    EXTREME = 8         /*! extreme speed */
};

/*!
* \struct Sprites constants.h
* \brief The structure containing all the image useful to the game
*/
typedef struct{
    SDL_Surface * bird1;        /*!< The first image of the bird */
    SDL_Surface * bird2;        /*!< The second image of the bird */
    SDL_Surface * bird3;        /*!< The third image of the bird */
    SDL_Surface * pipe1;        /*!< The image of the upper pipe */
    SDL_Surface * pipe2;        /*!< The image of the lower pipe */
    SDL_Surface * background;   /*!< The image of the background */
    SDL_Surface * ground;       /*!< The image of the ground */
    SDL_Surface * tap_to_play;  /*!< The image to explain how to play */
    SDL_Surface * play;         /*!< The image in menu to resume game */
    SDL_Surface * quit;         /*!< The image in menu to quit game */
    SDL_Surface * pause;        /*!< The image to display pause menu in the game */
}Sprites;

#endif // CONSTANTS_H
