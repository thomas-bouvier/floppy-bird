/*!
* \file qmatrix.h
* \brief File containing structures and prototypes linked to the Q matrix management
*/
#ifndef QMATRIX_H
#define QMATRIX_H

#include <stdlib.h>
#include <stdio.h>

#include "game_state.h"
#include "utils.h"
#include "file_manager.h"


/*typedef struct Actions Actions;*/
typedef struct MatrixQ MatrixQ;


/*!
* \struct Actions qlearning.h
* \brief Actions contains every actions that the bird can perform.
*/
/*struct Actions {
  char jump;	!< jump is equal to zero if no jump otherwise one 
};*/

/*!
* \struct MatrixQ qlearning.h
* \brief MatrixQ linked a state with an action to a reward
*/
struct MatrixQ {
	int nb_states; 
	State *state;
	int *reward; /*!< Q(state, action) array */
};

int findStateIndex(State * cur_state, MatrixQ * matrixQ);
int AddState(State * cur_state, MatrixQ * matrixQ);
void updateQReward(MatrixQ *matrixQ, int * state_index, int * last_action);

int findBestAction(int state_index, MatrixQ * matrixQ);

void freeMatrixQ(MatrixQ * matrixQ);

#endif
