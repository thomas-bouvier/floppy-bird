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

/*!
* \struct MatrixQ qlearning.h
* \brief MatrixQ linked a state with an action to a reward
*/
typedef struct{
	int nb_states; /*!< Quantity of states */
	State *state; /*!< Known states array */
	float *reward; /*!< Q(state, action) (Q-values) array */
} MatrixQ;

int findStateIndex(State * cur_state, MatrixQ * matrixQ);
int AddState(State * cur_state, MatrixQ * matrixQ);
void updateQReward(MatrixQ *matrixQ, int * last_states_index, int * last_action);
float computeQReward(MatrixQ *matrixQ, int current_index, int state_index, int action, int increment, int reward_value);

int findBestAction(int state_index, MatrixQ * matrixQ);

void freeMatrixQ(MatrixQ * matrixQ);

void show_matrixQ(MatrixQ * matrixQ);

#endif
