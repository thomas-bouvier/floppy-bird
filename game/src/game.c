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
* \return Return the created bird, NULL if error
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
    SDL_Rect rect;
    rect.x = number * PIPE_WIDTH;
    rect.y = pipe_y;
    rect.w = PIPE_WIDTH;
    rect.h = pipe_h;
    new_pipe->coordinates = &rect;
    return new_pipe;
}

/*!
*\brief Create an obstacle
* \param[in] number the obstacle number
* \param[in] height_lower the ordinate of the lower pipe
* \param[in] gap the gap between two pipes
* \return Return the created obstacle, NULL if error
*/
Obstacle * newObstacle(int number, int height_lower, int obstacle_gap)
{
    Obstacle * new_obstacle = (Obstacle*) malloc(sizeof(Obstacle));
    if(new_obstacle == NULL)
    {
        fprintf(stderr, "Obstacle allocation problem");
        return NULL;
    }
    Pipe * low = newPipe(number, height_lower, height_lower);                           //Lower pipe
    Pipe * up = newPipe(number, 0, SCREEN_HEIGHT - (height_lower + obstacle_gap));       //Upper pipe
    new_obstacle->lower = low;
    new_obstacle->upper = up;
    new_obstacle->gap = obstacle_gap;
    return new_obstacle;
}

/*!
* \brief Create a camera
* \param[in] x the camera abscissa
* \param[in] camera_speed the camera speed of scrolling
* \return Return the created camera, NULL if error
*/
Camera * newCamera(int x, int camera_speed)
{
    Camera * new_camera = (Camera*) malloc(sizeof(Camera));
    if(new_camera == NULL)
    {
        fprintf(stderr, "Camera allocation problem");
        return NULL;
    }
    SDL_Rect rect = {x, SCREEN_HEIGHT, SCREEN_WIDTH, SCREEN_HEIGHT};
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
