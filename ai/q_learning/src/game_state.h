/*!
* \file game_state.h
* \brief File containing structures and prototypes linked to the game state
*/
#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <stdlib.h>
#include <stdio.h>
#include "conf.h"

/*!
* \struct State qlearning.h
* \brief A State contains every informations needed to know exactly the state of the game
*/
typedef struct {
  short int delta_x;    /*!< delta_x is the horizontal distance from the left side of the camera to the next left side of the lower pipe */
  short int delta_y;    /*!< delta_y is the vertical distance from the bird to the next upper side of the lower pipe */
  short int pipe_height;    /*!< pipe_height is the height of the next pipe */
} State;

State * getCurrentState(int delta_x, int delta_y, int pipe_height, int bird_state);
void freeState(State * state);
int getCurrentReward(int bird_state);

int processing_dxdy(int initial_d);
int processing_birdstate(int initial_bird_state);

#endif
