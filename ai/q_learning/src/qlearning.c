#include "qlearning.h"

/*!
* \brief Deallocated a state
* \param[in] state the state to free
*/
void freeState(State * state)
{
	free(state);
}

/*!
* \brief Get the current state if the bird is alive else the state is not saved
* \param[in] delta_x the distance between the bird's bottom-right hand corner and the left side of the next pipe
* \param[in] delta_y the distance between the bird's bottom-right hand corner and the top of the next pipe
* \param[in] bird_state -1 if the bird is dead, 1 otherwise
* \return Return a state, distances=-1 if the bird is dead, NULL if error
*/
State * getCurrentState(int delta_x, int delta_y, int bird_state)
{
	switch(bird_state)
	{
		case -1:
				State * new_state = (State *) malloc(sizeof(State));
				new_state->delta_x = -1;
				new_state->delta_y = -1;
				break;
		case 1:
				State * new_state = (State *) malloc(sizeof(State));
				new_state->delta_x = delta_x;
				new_state->delta_y = delta_y;
				return new_state;
				break;
		default:
			return NULL;
			break;
	}
	return NULL;
}

/*!
* \brief Get the reward according to the current state of the bird
* \param[in] bird_state -1 if the bird is dead, another positive integer value otherwise
* \return Return a reward value
*/
int getCurrentReward(int bird_state)
{
	switch(bird_state)
	{
		case -1:
			return DEATH_REWARD;
			break;		
		default:
			return LIFE_REWARD;
			break;
	}
	return NULL;
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

int AddState(State * cur_state, MatrixQ * matrixQ)
{

}

int findBestAction(int state_index, struct MatrixQ matrixQ)
{

}
