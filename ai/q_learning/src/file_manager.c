#include "file_manager.h"

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
* \brief Get the current state if the bird is alive else the state is not saved
* \param[in] delta_x the distance between the bird's bottom-right hand corner and the left side of the next pipe
* \param[in] delta_y the distance between the bird's bottom-right hand corner and the top of the next pipe
* \param[in] bird_state 0 if the bird is dead, 1 otherwise
* \return Return a state, distances=-1 if the bird is dead, NULL if error
*/
void saveQMatrix(MatrixQ * matrixQ)
{
	freeMatrixQ(matrixQ);
}
