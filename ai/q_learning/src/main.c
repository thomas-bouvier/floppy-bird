#include "qmatrix.h"

int main()
{
	int tab_dx[18] = {20, 10, 30, 20, 10, 30, 20, 10, 30, 20, 10, 30, 20, 10, 30, 20, 10, 30};
	int tab_dy[18] = {20, 10, 30, 20, 10, 30, 20, 10, 30, 20, 10, 30, 20, 10, 30, 20, 10, 30};
	int tab_bird_state[18] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0};
	
	int i, j;
	
	/*Load Q matrix data */
	MatrixQ * matrixQ = NULL;
	matrixQ = (MatrixQ *) malloc(sizeof(MatrixQ));
	matrixQ->nb_states = 0;

	int save_state[NB_SAVED_STATES];
	for(i=0; i<NB_SAVED_STATES; ++i) save_state[i]=-1;
	int last_action=0;

	while(1)
	{
		for(i=0; i<18; ++i)
		{

			for(j=NB_SAVED_STATES-1; j>0; --j) save_state[j]=save_state[j-1];
		
			save_state[0]=findStateIndex(getCurrentState(tab_dx[i], tab_dy[i], tab_bird_state[i]), matrixQ);
			updateQReward(matrixQ, save_state, last_action);
			last_action = findBestAction(save_state[0], matrixQ);
			if(save_state[0] == -1) for(i=0; i<NB_SAVED_STATES; ++i) save_state[i]=-1;
		}
	}
	
	/*Save Q matrix data */

	return 0;
}
