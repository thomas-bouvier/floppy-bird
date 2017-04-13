#include "file_manager.h"
#include <string.h>

/*!
* \brief Get the current state if the bird is alive else the state is not saved
* \param[in] delta_x the distance between the bird's bottom-right hand corner and the left side of the next pipe
* \param[in] delta_y the distance between the bird's bottom-right hand corner and the top of the next pipe
* \param[in] bird_state 0 if the bird is dead, 1 otherwise
* \return Return a state, distances=-1 if the bird is dead, NULL if error
*/
MatrixQ * loadQMatrix()
{
	MatrixQ * matrixQ = NULL;
	matrixQ = (MatrixQ *) malloc(sizeof(MatrixQ));
	matrixQ->nb_states = 0;
	return matrixQ;
}

/*!
* \brief Save the matrixQ in a text file and free the matrixQ
* \param[in] matrixQ the matrix to be saved
* \param[in] filename the filename where data have to be stored
* \return Return -1 if error
*/
int saveQMatrix(MatrixQ * matrixQ, char * filename)
{
	int i=0;
	FILE * fp = NULL;

	printf("filename");
	if((fp = fopen(filename, "w+")) == NULL)
	{
		fprintf(stderr, "Error occurred when opening file %s.\n", filename);
		return -1;
	}
	
	fprintf(fp, "%d\n", matrixQ->nb_states);

	for(i=0; i<NB_ACTIONS*matrixQ->nb_states; ++i)
	{
		fprintf(fp, "%f\n", matrixQ->reward[i]);
	}

	if(fclose(fp) == EOF)
	{
		fprintf(stderr, "Error occurred when closing file %s.\n", filename);
		return -1;
	}
	/*freeMatrixQ(matrixQ);*/
	return 1;
}
