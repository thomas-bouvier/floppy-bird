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
* \param[in] bird_path the path of the bird sprite
* \param[in] bird_gravity the speed of climb/fall of the bird
* \return Return a bird, NULL if error
*/
Bird * newBird(int bird_x, int bird_y, char * bird_path)
{
    Bird * new_bird = (Bird*) malloc(sizeof(Bird));
    if(new_bird == NULL)
    {
        fprintf(stderr, "Bird allocation problem");
        return NULL;
    }
    new_bird->x = bird_x;
    new_bird->y = bird_y;
    new_bird->surface = SDL_LoadBMP(bird_path);
    if(new_bird->surface==NULL)
    {
        fprintf(stderr, "Sprite loading failure(%s)\n",SDL_GetError());
        return NULL;
    }
    new_bird->dirY = 0;
    return new_bird;
}

/*!
*\brief Create a pipe
* \param[in] pipe_x the pipe abscissa coordonate
* \param[in] pipe_y the pipe ordonate coordonate
* \param[in] pipe_path the path of the pipe sprite
* \return Return a pipe, NULL if error
*/
Pipe * newPipe(int pipe_x, int pipe_y, char * pipe_path)
{
    Pipe * new_pipe = (Pipe*) malloc(sizeof(Pipe));
    if(new_pipe == NULL)
    {
        fprintf(stderr, "Pipe allocation problem");
        return NULL;
    }
    new_pipe->x = pipe_x;
    new_pipe->y = pipe_y;
    new_pipe->surface = SDL_LoadBMP(pipe_path);
    if(new_pipe->surface==NULL)
    {
        fprintf(stderr, "Sprite loading failure(%s)\n",SDL_GetError());
        return NULL;
    }
    return new_pipe;
}


