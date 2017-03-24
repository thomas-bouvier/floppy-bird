/*!
* \file qlearning.h
* \brief File containing structures and prototypes linked to the q-learning algorithm
*/
#ifndef QLEARNING_H
#define QLEARNING_H

#include <stdlib.h>
#include <stdio.h>

#include "conf.h"

typedef struct State State;
typedef struct Actions Actions;
typedef struct MatrixQ MatrixQ;

/*!
* \struct State qlearning.h
* \brief A State contains every informations needed to know exactly the state of a bird and pipes.
*/
struct State {
  /*int id_state;			/*!< ID of the current state */*/
  short int delta_x;    /*!< X is the horizontal distance from the bird to the next lower pipe */
  short int delta_y;    /*!< Y is the vertical distance from the bird to the next lower pipe */
  /*char bird_state;	/*!< bird_state is the state of the bird with those parameters, 0=death 1=live */*/
};

/*!
* \struct Actions qlearning.h
* \brief Actions contains every actions that the bird can perform.
*/
/*struct Actions {
  char jump;	/*!< jump is equal to zero if no jump otherwise one */
};*/

/*!
* \struct MatrixQ qlearning.h
* \brief MatrixQ linked a state with an action to a reward
*/
struct MatrixQ {
	int nb_states;
	/*int nb_actions;*/
	State *state;
	/*Actions action;*/
	int *reward; /*!< Q(state, action) array */
};

State * getCurrentState(int id_state, int delta_x, int delta_y, int dead);
void freeState(State * state);
int getCurrentReward(int bird_state);

int findStateIndex(State * cur_state,MatrixQ * matrixQ); /* otherwise creation */
int AddState(State * cur_state,MatrixQ * matrixQ);
void updateLastQ(struct MatrixQ *matrixQ, int laststate_index, int lastaction_index, int Reward, int level);

int findBestAction(int state_index, struct MatrixQ matrixQ);


/*Load/Save of the QMatrix functions*/

#endif
