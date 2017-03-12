/*!
* \file file.c
* \brief File containing the functions to manage the extern files
*/
#include "file.h"

/*!
* \brief Read the predefined level from a file
* \param[in] file the file to read
* \param[in] number the number of the obstacle read
* \return Return the height of the low pipe of the obstacle
*/
int readLevel(FILE * file, int number)
{
    int heightPipe = 0;
    fseek(file, number * sizeof(int), SEEK_SET);
    fscanf(file, "%d", &heightPipe);
    return heightPipe;
}


