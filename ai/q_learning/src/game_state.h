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
  /*int id_state;			/*!< ID of the current state */*/
  short int delta_x;    /*!< X is the horizontal distance from the bird to the next lower pipe */
  short int delta_y;    /*!< Y is the vertical distance from the bird to the next lower pipe */
  /*char bird_state;	/*!< bird_state is the state of the bird with those parameters, 0=death 1=live */*/
};

State * getCurrentState(int id_state, int delta_x, int delta_y, int dead);
void freeState(State * state);
int getCurrentReward(int bird_state);

#endif
