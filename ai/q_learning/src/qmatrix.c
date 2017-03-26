#include "qmatrix.h"

/*!
* \brief Search and return the index of the current state in the MatrixQ
* \param[in] cur_state current state to search or add in the matrix
* \param[in] matrixQ matrix of every known state
* \return Return an index of the matrix corresponding to the current state
*/
int findStateIndex(State * cur_state, MatrixQ * matrixQ)
{
	int i=0;

	switch(cur_state->delta_x)
	{
		case -1:
				return 0;
				break;
		default:
			while(i++<matrixQ->nb_states)
			{
				if(cur_state->delta_x == matrixQ->state->delta_x)
					if(cur_state->delta_y == matrixQ->state->delta_y)
					{
						freeState(cur_state);
						return i;
					}
			}
			break;
	}
	return AddState(State * cur_state, MatrixQ * matrixQ);
}

/*!
* \brief Add a new state in the MatrixQ
* \param[out] cur_state current state to add in the matrix
* \param[out] matrixQ matrix of every known state
* \return Return an index of the matrix corresponding to the current state, -1 if error
*/
int AddState(State * cur_state, MatrixQ * matrixQ)
{
	if(matrixQ = realloc(matrixQ, sizeof(matrixQ)+sizeof(State)+1) == NULL) return -1; /*A voir realloc nbstate et nbreward*/
	matrixQ->state[matrixQ->nb_state]->delta_x = cur_state->delta_x;
	matrixQ->state[matrixQ->nb_state]->delta_y = cur_state->delta_y;
	matrixQ->nb_state++;
	free(cur_state);
	return matrixQ->nb_state-1;
}

/*!
* \brief return the action to be performed
* \param[in] state_index
* \param[in] matrixQ matrix of every known state
* \return Return 1 to jump, 0 otherwise
*/
int findBestAction(int state_index, struct MatrixQ matrixQ)
{
	return (matrixQ->reward[state_index*2] <= matrixQ->reward[state_index*2+1])? 1:0;
}

void updateLastQ(struct MatrixQ *matrixQ, int laststate_index, int lastaction_index, int Reward, int level)
{

}
