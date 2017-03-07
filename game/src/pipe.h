/*!
* \file pipe.h
* \brief File containing structures and prototypes for the pipes
*/
#ifndef PIPE_H
#define PIPE_H

#include "constants.h"
#include <stdlib.h>
#include <stdio.h>

/*!
* \struct Pipe pipe.h
* \brief The pipes that the bird has to avoid
*/
typedef struct{
    int x;                  /*!< the abscissa of the pipe */
    int y;                  /*!< the ordinate of the pipe */
    int w;                  /*!< the width of the pipe */
    int h;                  /*!< the height of the pipe */
} Pipe;

Pipe * newPipe(int number, int pipe_y, int pipe_h);

#endif // PIPE_H
