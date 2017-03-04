/*!
* \file game.c
* \brief File containing the functions to manage the game
*/

#include "game.h"
#include <stdlib.h>
#include <stdio.h>

/*!
*\brief Create a bird
* \param[in] bird_x the bird abscissa coordonate
* \param[in] bird_y the bird ordonate coordonate
* \param[in] bird_sprite the path of the bird sprite
* \param[in] bird_gravity the speed of climb/fall of the bird
* \return Return a bird, NULL if error
*/
static Bird * newBird(int bird_x, int bird_y, char * bird_sprite, int bird_gravity)
{
    Bird * new_bird = (Bird*) malloc(sizeof(Bird));
    if(new_bird == NULL)
    {
        fprintf(stderr, "Bird allocation problem");
        return NULL;
    }
    new_bird->x = bird_x;
    new_bird->y = bird_y;
    new_bird->sprite = bird_sprite;
    new_bird->gravity = bird_gravity;
    return new_bird;
}

/*!
*\brief Create a pipe
* \param[in] pipe_x the pipe abscissa coordonate
* \param[in] pipe_y the pipe ordonate coordonate
* \param[in] pipe_sprite the path of the pipe sprite
* \param[in] pipe_speed the speed of scrolling of the pipes
* \return Return a pipe, NULL if error
*/
static Pipe * newPipe(int pipe_x, int pipe_y, char * pipe_sprite, int pipe_speed)
{
    Pipe * new_pipe = (Pipe*) malloc(sizeof(Pipe));
    if(new_pipe == NULL)
    {
        fprintf(stderr, "Pipe allocation problem");
        return NULL;
    }
    new_pipe->x = pipe_x;
    new_pipe->y = pipe_y;
    new_pipe->sprite = pipe_sprite;
    new_pipe->speed = pipe_speed;
    return new_pipe;
}

/*!
* \brief Allow to scroll the pipe in the left direction
* \param[out] pipe the pipe to scroll
*/
void pipeScrolling(Pipe * pipe)
{
    pipe->x -= pipe->speed;
}
