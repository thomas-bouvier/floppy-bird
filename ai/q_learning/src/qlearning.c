#include "qlearning.h"

/*!
* \brief Get the current state if the bird is alive else the state is not saved
* \param[in] id_state the number of state currently saved
* \param[in] delta_x the distance between the bird's bottom-right hand corner and the left side of the next pipe
* \param[in] delta_y the distance between the bird's bottom-right hand corner and the top of the next pipe
* \param[in] bird_state 0 if the bird is dead, 1 otherwise
* \return Return a state, id=0 if the bird is dead, NULL if error
*/
State * getCurrentState(int id_state, int delta_x, int delta_y, int bird_state)
{
	switch(dead)
	{
		case 0:
			{
				State * new_state = (State *) malloc(sizeof(State));
				new_state->id_state = 0;
				break;
			}	
		case 1:
			{
				State * new_state = (State *) malloc(sizeof(State));
				new_state->id_state = id_state;
				new_state->delta_x = delta_x;
				new_state->delta_y = delta_y;
				return new_state;
				break;
			}	
		default:
			return NULL;
			break;
	}
	return NULL;
}

/*!
* \brief Get the reward according to the current state of the bird
* \param[in] bird_state 0 if the bird is dead, another positive integer value otherwise
* \return Return a reward value
*/
int getCurrentReward(int bird_state)
{
	switch(bird_state)
	{
		case 0:
			return DEATH_REWARD;
			break;		
		default:
			return LIFE_REWARD;
			break;
	}
	return NULL;
}

int findStateIndex(struct State cur_state, struct MatrixQ matrixQ)
{

}

int findBestAction(int state_index, struct MatrixQ matrixQ)
{

}
