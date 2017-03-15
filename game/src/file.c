/*!
* \file file.c
* \brief File containing the functions to manage the extern files
*/
#include "file.h"


int readLevel(FILE * f, int number)
{
    int heightPipe = 0;
    fseek(f, number * sizeof(int), SEEK_SET);
    fscanf(f, "%d", &heightPipe);
    return heightPipe;
}


