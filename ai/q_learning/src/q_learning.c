#include "q_learning.h"

/*!
* \brief Q-learning main loop
* \param[in] matrixQ matrix of every known state
* \param[in] last_states state index from the last state to the older one to find them in the Q matrix
* \param[in] last_action action from the last one to the older one, linked to the last_states array
* \param[in] dx the distance between the left side of the camera and the left side of the next pipe
* \param[in] dy the distance between the bird's upper side and the top of the next pipe
* \param[in] bird_state 0 if the bird is dead, 1 otherwise
*/
int q_learning_loop(MatrixQ * matrixQ, int * last_states, int * last_action, int dx, int dy, int pipe_height, int bird_state)
{
	/* Shift for the new state */
	shift_array(last_states, NB_SAVED_STATES);

	/* Update the new state */
	last_states[0] = findStateIndex(getCurrentState(processing_dxdy(dx), processing_dxdy(dy), pipe_height, getCurrentVelocity(last_action), processing_birdstate(bird_state)), matrixQ);

	/* Update the Q matrix */
	updateQReward(matrixQ, last_states, last_action);

	shift_array(last_action, NB_SAVED_ACTIONS);

	/* Compute the new action */ /* Reset when the bird dies */
	if(last_states[0] != -1) 
	{	
		last_action[0] = findBestAction(last_states[0], matrixQ); /* ACTION GAME */
	}
	else
	{
		init_array(last_states, NB_SAVED_STATES, -1); /* RESET GAME */
		init_array(last_action, NB_SAVED_ACTIONS, -1); /* RESET GAME */
	}
	return 1;
}
