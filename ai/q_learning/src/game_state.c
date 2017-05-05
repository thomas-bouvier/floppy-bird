#include "game_state.h"

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
* \param[in] delta_x the distance between the left side of the camera and the left side of the next pipe
* \param[in] delta_y the distance between the bird's upper side and the top of the next pipe
* \param[in] bird_state 0 if the bird is dead, 1 otherwise
* \return Return a state, distances=-1 if the bird is dead, NULL if error
*/
State * getCurrentState(int delta_x, int delta_y, int pipe_height, int velocity, int bird_state)
{
	State * new_state = NULL;
	switch(bird_state)
	{
		case 0:
				new_state = (State *) malloc(sizeof(State));
				new_state->delta_x = -1;
				new_state->delta_y = -1;
				new_state->pipe_height = -1;
				new_state->velocity = -1;
				return new_state;
		case 1:
				new_state = (State *) malloc(sizeof(State));
				new_state->delta_x = delta_x;
				new_state->delta_y = delta_y;
				new_state->pipe_height = pipe_height;
				new_state->velocity = velocity;
				return new_state;
		default:
			freeState(new_state);
			break;
	}
	return NULL;
}

/*!
* \brief Get the current velocity thanks to the last actions known
* \param[in] last_actions array of last actions performed
* \return Return a velocity, a high negative value is linked to a fast falling bird
*/
int getCurrentVelocity(int * last_states)
{
    int i=0;

    while(i<4 && last_states[i] != 1) i++;
    switch(i)
    {
        case 0:
            return +5;
        case 1:
            return +3;
        case 2:
            return +0;
        case 3:
            return -3;
        default:
            return -5;
    }
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
* \brief Adapt the value of the bird_state variable coming from the game to the q_learning algorithm
* \param[in] initial_bird_state variable coming from the game
* \return Return a new value to fit with the q_learning algorithm, -1 if incorrect input
*/
int processing_birdstate(int initial_bird_state)
{
	if(initial_bird_state!=1 && initial_bird_state!=0) return -1;
	return (initial_bird_state)? 0:1;
}

