#include "qmatrix.h"

int main()
{
	/* Basic initialization */
	int i;
	srand(time(NULL));

	/* Load Q matrix data */
	MatrixQ * matrixQ = NULL;
	matrixQ = (MatrixQ *) malloc(sizeof(MatrixQ));
	matrixQ->nb_states = 0;

	/* Array to simulate dx, dy, the bird state*/
	int tab_dx[18];
	int tab_dy[18];
	int tab_bird_state[18] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
	for(i=0; i<18; ++i)
	{
		tab_dx[i] = randomAtMost(5);
		tab_dy[i] = randomAtMost(5);
	}

	/* Number of last states saved / last actions */
	int save_state[NB_SAVED_STATES];
	int save_action[NB_SAVED_STATES];
	init_array(save_state, NB_SAVED_STATES, -1);
	init_array(save_action, NB_SAVED_STATES, -1);

	/* main loop */
	while(1)
	{
		for(i=0; i<18; ++i)
		{
			/* Shift for the new state */
			shift_array(save_state, NB_SAVED_STATES);
			shift_array(save_action, NB_SAVED_STATES);

			/* Update the new state */
			save_state[0]=findStateIndex(getCurrentState(tab_dx[i], tab_dy[i], tab_bird_state[i]), matrixQ);
			/* Update the Q matrix */
			updateQReward(matrixQ, save_state, save_action);

			/* Compute the new action */
			save_action[0] = findBestAction(save_state[0], matrixQ);

			/* Reset when the bird dies */
			if(save_state[0] == -1) 
			{
				init_array(save_state, NB_SAVED_STATES, -1);
				init_array(save_action, NB_SAVED_STATES, -1);
			}

			/* utils */
			show_matrixQ(matrixQ);
			delay(50000);
		}
	}
	
	/*Save Q matrix data */

	freeMatrixQ(matrixQ);

	return 0;
}
