/*!
* \file pipe.c
* \brief File containing the functions to manage a pipe
*/
#include "pipe.h"

/*!
* \brief Create a pipe
* \param[out] pipe the pipe to fill
* \param[in] number the pipe number
* \param[in] pipe_y the pipe ordinate
* \param[in] pipe_h the pipe height
* \return Return the created pipe, NULL if error
*/
void initPipe(Pipe * pipe, int number, int pipe_y, int pipe_h)
{
    pipe->x = number * PIPE_X_OFFSET + SCREEN_WIDTH;
    pipe->y = pipe_y;
    pipe->w = PIPE_WIDTH;
    pipe->h = pipe_h;
}
