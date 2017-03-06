/*!
* \file game.c
* \brief File containing the functions to manage the game
*/

#include "game.h"
#include <stdlib.h>
#include <stdio.h>

/*!
*\brief Create a bird
* \param[in] bird_x the bird abscissa
* \param[in] bird_y the bird ordinate
* \param[in] bird_path the path of the bird sprite
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
    new_bird->dir_y = 0;
    return new_bird;
}

/*!
*\brief Create a pipe
* \param[in] pipe_x the pipe abscissa
* \param[in] pipe_y the pipe ordinate
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

/*!
* \brief Create a Camera
* \param[in] x the camera abscissa
* \param[in] y the camera ordinate
* \param[in] camera_speed the camera speed of scrolling
* \return Return the created Camera, NULL if error
*/
Camera * newCamera(int x, int y, int camera_speed)
{
    Camera * new_camera = (Camera*) malloc(sizeof(Camera));
    if(new_camera == NULL)
    {
        fprintf(stderr, "Camera allocation problem");
        return NULL;
    }
    SDL_Rect rect = {x, y, SCREEN_HEIGHT, SCREEN_WIDTH};
    new_camera->view = &rect;
    new_camera->speed = camera_speed;
    return new_camera;
}
/*!
*\brief Update the ordinate of the bird and the dir_y
*\param[in] bird the bird to be updated
*/
void updateBirdY(Bird * bird)
{
    bird->y += bird->dir_y;
    bird->dir_y += GRAVITY;
}

/*!
* \brief Allow to scroll the camera in the right direction
* \param[out] pipe the pipe to scroll
*/
void cameraScrolling(Camera * camera)
{
    camera->view->x += camera->speed;
}
