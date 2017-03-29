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
	int tab_dx[18] = {10, 5, 20, 10, 5, 20, 10, 5, 30, 20, 10, 30, 20, 10, 30, 20, 10, 30};
	int tab_dy[18] = {20, 10, 30, 20, 5, 20, 10, 5, 30, 20, 10, 30, 20, 10, 30, 20, 10, 30};
	int tab_bird_state[18] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
	int tab_last_action[18] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
	
	int i, j;
	srand(time(NULL));

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
			if(save_state[NB_SAVED_STATES-1] != -1)updateQReward(matrixQ, save_state, last_action);
			last_action = findBestAction(save_state[0], matrixQ);
			if(save_state[0] == -1) for(i=0; i<NB_SAVED_STATES; ++i) save_state[i]=-1;
			show_matrixQ(matrixQ);
			delay(50000);
		}
	}
	
	/*Save Q matrix data */

	return 0;
}
