#include "game_state.h"
#include <stdlib.h>
#include <stdio.h>

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
* \param[in] bird_state 0 if the bird is dead, 1 otherwise
* \return Return a state, distances=-1 if the bird is dead, NULL if error
*/
State * getCurrentState(int delta_x, int delta_y, int bird_state)
{
	State * new_state = NULL;
	switch(bird_state)
	{
		case 0:
				new_state = (State *) malloc(sizeof(State));
				new_state->delta_x = -1;
				new_state->delta_y = -1;
				return new_state;
		case 1:
				new_state = (State *) malloc(sizeof(State));
				new_state->delta_x = delta_x;
				new_state->delta_y = delta_y;
				return new_state;
		default:
			freeState(new_state);
			return NULL;
			break;
	}
	return NULL;
}

/*!
* \brief Get the reward according to the current state of the bird
* \param[in] bird_state -1 if the bird is dead, another positive integer value otherwise
* \return Return a reward value, 0 if error
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
	return 0;
}

/*!
* \brief Approximate a distance dx or dy to get less different states
* \param[in] initial_d initial distance dx or dy
* \return Return a new distance 
*/
int processing_dxdy(int initial_d)
{
	return initial_d/DISTANCE_ACCURACY;
}

/*!
* \brief Adapt the value of the bird_state variable coming from the game to ai1 algorithm
* \param[in] initial_bird_state variable coming from the game
* \return Return a a new value to fit with the q_learning algorithm 
*/
int processing_birdstate(int initial_bird_state)
{
	return (initial_bird_state)? 0:1;
}

