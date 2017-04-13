#include "qmatrix.h"

/*!
* \brief Deallocated a Q Matrix
* \param[in] matrixQ matrix of every known state
*/
void freeMatrixQ(MatrixQ * matrixQ)
{
	free(matrixQ->state);
	free(matrixQ->reward);
	free(matrixQ);
}

/*!
* \brief Print the Q matrix
* \param[in] matrixQ matrix of every known state
*/
void show_matrixQ(MatrixQ * matrixQ)
{
	clearScreen();
	int i;
	printf("	| Action 0		| Action 1\n");
	for(i=0; i<matrixQ->nb_states; ++i) printf("idx %d	| %f		| %f	\n", i, matrixQ->reward[i*2+0], matrixQ->reward[i*2+1]);
}

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
		fprintf(stderr, "Error occurred when allocated a new state: index %d", matrixQ->nb_states);
		return -1;
	}
	if((matrixQ->reward = (float *) realloc(matrixQ->reward, NB_ACTIONS*(matrixQ->nb_states+1)*sizeof(float))) == NULL) 
	{
		fprintf(stderr, "Error occurred when allocated the reward array: index %d", matrixQ->nb_states);
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
	if(matrixQ->reward[state_index*2] == matrixQ->reward[state_index*2+1]) return randomAtMost(1);
	return (matrixQ->reward[state_index*2] < matrixQ->reward[state_index*2+1])? 1:0;
}

/*!
* \brief update each Q reward for every state_index given (currently only one state)
* \param[in] matrixQ matrix of every known state
* \param[in] state_index index from the last state to the older one to find them in the Q matrix
* \param[in] last_action last_action performed
*/
void updateQReward(MatrixQ * matrixQ, int * last_states_index, int last_action)
{
		if(last_states_index[1] != -1)
		{
			matrixQ->reward[last_states_index[1]*2+last_action] = computeQReward(matrixQ, last_states_index[0], last_states_index[1], last_action);
		}
}


/*!
* \brief compute a new Q reward
* \param[in] matrixQ matrix of every known state
* \param[in] current_index index of the current state
* \param[in] state_index index of the state where change the Q reward
* \param[in] action action performed when the bird was in the state corresponding to index_state
*/
float computeQReward(MatrixQ *matrixQ, int current_index, int state_index, int action)
{
	float optimal_nextvalue, old_value, new_value;

	optimal_nextvalue = (float) (matrixQ->reward[current_index*2+findBestAction(current_index, matrixQ)]);

	old_value = (float) matrixQ->reward[state_index*2+action];

	new_value = old_value + LEARNING_RATE * (getCurrentReward(current_index) + DISCOUNT * optimal_nextvalue - old_value);
			if(new_value < HIGHER_QREWARD_LIMIT && new_value > LOWER_QREWARD_LIMIT)  return new_value;
			else return old_value;
}
