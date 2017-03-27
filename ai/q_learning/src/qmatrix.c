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
		return -1;
	}
	if((matrixQ->reward = (int *) realloc(matrixQ->reward, 2*(matrixQ->nb_states+1)*sizeof(int))) == NULL) 
	{
		fprintf(stderr, "Erreur allocation d'un nouveau reward: index %d", matrixQ->nb_states);
		return -1;
	}
	
	matrixQ->reward[matrixQ->nb_states*2] = randomAtMost(RANDOM_INIT_REWARD);
	matrixQ->reward[matrixQ->nb_states*2+1] = randomAtMost(RANDOM_INIT_REWARD);
	matrixQ->state[matrixQ->nb_states].delta_x = cur_state->delta_x;
	matrixQ->state[matrixQ->nb_states].delta_y = cur_state->delta_y;
	matrixQ->nb_states++;
	freeState(cur_state);
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

/*!
* \brief update each Q reward for every state_index given
* \param[in] matrixQ matrix of every known state
* \param[in] state_index index from the last state to the older one to find them in the Q matrix
* \param[in] last_action last_action performed
* \param[in] reward reward function of the current state due to the last action
*/
void updateQReward(MatrixQ *matrixQ, int * state_index, int last_action)
{
	int i, optimal_nextvalue;
	for(i=1; i<NB_SAVED_STATES; ++i)
	{
		optimal_nextvalue = matrixQ->reward[state_index[0]*2+last_action];
		/*matrixQ->reward[state_index[i]*2+last_action] += LEARNING_RATE * (getCurrentReward(state_index[0]) + DISCOUNT * optimal_nextvalue - matrixQ->reward[state_index[i]*2+last_action]);*/
	}
}
