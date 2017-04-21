/*!
* \file file_manager.h
* \brief File containing structures and prototypes linked to the loading/saving of data
*/
#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#include "qmatrix.h"

MatrixQ * loadQMatrix(char * filename);
int saveQMatrix(MatrixQ * matrixQ, char * filename);

#endif
