/*!
* \file pipe.c
* \brief File containing the functions to manage a pipe
*/
#include "pipe.h"

/*!
*\brief Create a pipe
* \param[in] number the pipe number
* \param[in] pipe_y the pipe ordinate
* \param[in] pipe_h the pipe height
* \return Return the created pipe, NULL if error
*/
Pipe * newPipe(int number, int pipe_y, int pipe_h)
{
    Pipe * new_pipe = (Pipe*) malloc(sizeof(Pipe));
    if(new_pipe == NULL)
    {
        fprintf(stderr, "Pipe allocation problem");
        return NULL;
    }
    new_pipe->x = number * PIPE_X_OFFSET;
    new_pipe->y = pipe_y;
    new_pipe->w = PIPE_WIDTH;
    new_pipe->h = pipe_h;
    return new_pipe;
}
