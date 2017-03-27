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
				return -1;
				break;
		default:
			while(i<matrixQ->nb_states)
			{
				if(cur_state->delta_x == matrixQ->state[i].delta_x)
					if(cur_state->delta_y == matrixQ->state[i].delta_y)
					{
						freeState(cur_state);
						return i;
					}
				i++;
			}
			break;
	}
	return AddState(cur_state, matrixQ);
}

/*!
* \brief Add a new state in the MatrixQ
* \param[out] cur_state current state to add in the matrix
* \param[out] matrixQ matrix of every known state
* \return Return an index of the matrix corresponding to the current state, -1 if error
*/
int AddState(State * cur_state, MatrixQ * matrixQ)
{
	if((matrixQ->state = (State *) realloc(matrixQ->state ,(matrixQ->nb_states+1)*sizeof(State))) == NULL) 
	{
		fprintf(stderr, "Erreur allocation d'un nouvel état: index %d", matrixQ->nb_states);
		return -1; /*A voir realloc nbstate et nbreward*/
	}
	if((matrixQ->reward = (int *) realloc(matrixQ->reward, (matrixQ->nb_states+1)*sizeof(int))) == NULL) 
	{
		fprintf(stderr, "Erreur allocation d'un nouveau reward: index %d", matrixQ->nb_states);
		return -1; /*A voir realloc nbstate et nbreward*/
	}

	matrixQ->state[matrixQ->nb_states].delta_x = cur_state->delta_x;
	matrixQ->state[matrixQ->nb_states].delta_y = cur_state->delta_y;
	matrixQ->nb_states++;
	free(cur_state);
	return matrixQ->nb_states-1;
}

/*!
* \brief return the action to be performed
* \param[in] state_index
* \param[in] matrixQ matrix of every known state
* \return Return 1 to jump, 0 otherwise
*/
int findBestAction(int state_index, MatrixQ * matrixQ)
{
	return (matrixQ->reward[state_index*2] <= matrixQ->reward[state_index*2+1])? 1:0;
}
/*
void updateLastQ(MatrixQ *matrixQ, int laststate_index, int lastaction_index, int Reward, int level)
{

}*/
