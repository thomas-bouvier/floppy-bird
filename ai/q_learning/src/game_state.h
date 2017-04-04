/*!
* \file game_state.h
* \brief File containing structures and prototypes linked to the extraction of the game state
*/
#ifndef GAME_STATE_H
#define GAME_STATE_H

#include "conf.h"

/*!
* \struct State qlearning.h
* \brief A State contains every informations needed to know exactly the state of a bird and pipes.
*/

typedef struct State State;

struct State {
  short int delta_x;    /*!< X is the horizontal distance from the bird to the next lower pipe */
  short int delta_y;    /*!< Y is the vertical distance from the bird to the next lower pipe */
};

State * getCurrentState(int delta_x, int delta_y, int bird_state);
void freeState(State * state);
int getCurrentReward(int bird_state);

int processing_dxdy(int initial_d);

#endif
