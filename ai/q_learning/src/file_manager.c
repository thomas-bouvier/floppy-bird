#include "file_manager.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/*!
* \brief Load the Q matrix from a text file
* \param[in] filename the filename where data are stored
* \return Return the Q matrix filled, NULL if error
*/
MatrixQ * loadQMatrix(char * filename)
{
	MatrixQ * matrixQ = NULL;
	matrixQ = (MatrixQ *) malloc(sizeof(MatrixQ));
	FILE * fp = NULL;
	int i;
	matrixQ->nb_states = 0;

	if((fp = fopen(filename, "r")) == NULL)
	{
		if((fp = fopen(filename, "w+")) == NULL)
		{
			fprintf(stderr, "Error occurred when opening file %s.\n", filename);
			return NULL;
		}
		fprintf(fp, "%d ", matrixQ->nb_states);
	}

	fscanf(fp, "%d", &(matrixQ->nb_states));

	if(matrixQ->nb_states != 0)
	{
		if((matrixQ->state = (State *) malloc((matrixQ->nb_states)*sizeof(State))) == NULL) 
		{
			fprintf(stderr, "Error occurred when allocated a new state: index %d", matrixQ->nb_states);
			return NULL;
		}
		if((matrixQ->reward = (float *) malloc(NB_ACTIONS*(matrixQ->nb_states)*sizeof(float))) == NULL) 
		{
			fprintf(stderr, "Error occurred when allocated the reward array: index %d", matrixQ->nb_states);
			return NULL;
		}
	}

	for(i=0; i<NB_ACTIONS*matrixQ->nb_states; ++i)
	{
		fscanf(fp, " %f", &(matrixQ->reward[i]));
	}

	for(i=0; i<matrixQ->nb_states; ++i)
	{
		fscanf(fp, " %hd", &(matrixQ->state[i].delta_x));
		fscanf(fp, " %hd", &(matrixQ->state[i].delta_y));
	}

	if(fclose(fp) == EOF)
	{
		fprintf(stderr, "Error occurred when closing file %s.\n", filename);
		return NULL;
	}

	return matrixQ;
}

/*!
* \brief Save the Q matrix in a text file
* \param[in] matrixQ the matrix to be saved
* \param[in] filename the filename where data have to be stored
* \return Return -1 if error, otherwise 1
*/
int saveQMatrix(MatrixQ * matrixQ, char * filename)
{
	int i=0;
	FILE * fp = NULL;

	if((fp = fopen(filename, "w+")) == NULL)
	{
		fprintf(stderr, "Error occurred when opening file %s.\n", filename);
		return -1;
	}
	
	fprintf(fp, "%d ", matrixQ->nb_states);

	for(i=0; i<NB_ACTIONS*matrixQ->nb_states; ++i)
	{
		fprintf(fp, "%f ", matrixQ->reward[i]);
	}

	for(i=0; i<matrixQ->nb_states; ++i)
	{
		fprintf(fp, "%d ", matrixQ->state[i].delta_x);
		fprintf(fp, "%d ", matrixQ->state[i].delta_y);
	}

	if(fclose(fp) == EOF)
	{
		fprintf(stderr, "Error occurred when closing file %s.\n", filename);
		return -1;
	}
	return 1;
}

