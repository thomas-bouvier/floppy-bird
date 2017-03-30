#include "qmatrix.h"
#include <unistd.h>

void show_matrixQ(MatrixQ * matrixQ);
void clearScreen();
void delay(unsigned int mseconds);

void show_matrixQ(MatrixQ * matrixQ)
{
	clearScreen();
	int i;
	printf("	| Action 0	| Action 1\n");
	for(i=0; i<matrixQ->nb_states; ++i) printf("idx %d	| %d		| %d	\n", i, matrixQ->reward[i*2+0], matrixQ->reward[i*2+1]);
}

void clearScreen()
{
  const char *CLEAR_SCREEN_ANSI = "\e[1;1H\e[2J";
  write(STDOUT_FILENO, CLEAR_SCREEN_ANSI, 12);
}

void delay(unsigned int mseconds)
{
    clock_t goal = mseconds + clock();
    while (goal > clock());
}

int main()
{
	/* Basic initialization */
	int i, j;
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
	for(i=0; i<NB_SAVED_STATES; ++i) save_state[i]=-1;
	int save_last_action[NB_SAVED_STATES];
	for(i=0; i<NB_SAVED_STATES; ++i) save_last_action[i]=-1;

	/* main loop */
	while(1)
	{
		for(i=0; i<18; ++i)
		{
			/* Shift for the new state */
			for(j=NB_SAVED_STATES-1; j>0; --j) save_state[j]=save_state[j-1];
			
			/* Shift for the new action */
			for(j=NB_SAVED_STATES-1; j>0; --j) save_last_action[j]=save_last_action[j-1];

			/* Update the new state */
			save_state[0]=findStateIndex(getCurrentState(tab_dx[i], tab_dy[i], tab_bird_state[i]), matrixQ);
			/* Update the Q matrix if total old state/action is filled */
			if(save_last_action[NB_SAVED_STATES-1] != -1) updateQReward(matrixQ, save_state, save_last_action);

			/* Compute the new action */
			save_last_action[0] = findBestAction(save_state[0], matrixQ);

			/* Reset when the bird dies */
			if(save_state[0] == -1) 
			{
				for(i=0; i<NB_SAVED_STATES; ++i) save_state[i]=-1;
				for(i=0; i<NB_SAVED_STATES; ++i) save_last_action[i]=-1;
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
