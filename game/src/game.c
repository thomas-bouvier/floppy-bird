/*!
* \file game.c
* \brief File containing the functions to manage the game
*/

#include "game.h"
#include <stdlib.h>
#include <stdio.h>





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
* \brief Allow to scroll the camera in the right direction
* \param[out] pipe the pipe to scroll
* \param[out] bird the bird that moves with the camera
*/
void cameraScrolling(Camera * camera, Bird * bird)
{
    camera->view->x += camera->speed;
    bird->x += camera->speed;
}

/*!
* \brief Color a rectangle
* \param[out] surface the drawing target
* \param[in] rect the rectangle to color
* \param[in] r the red component (integer between 0 and 255)
* \param[in] g the green component (integer between 0 and 255)
* \param[in] b the blue component (integer between 0 and 255)
*/
void drawRectangle(SDL_Surface * surface, SDL_Rect * rect, int r, int g, int b)
{
    SDL_FillRect(surface, rect, SDL_MapRGB(surface->format, r, g, b));
}

/*!
* \brief Color the two pipes of an obstacle
* \param[out] surface the drawing target
* \param[in] obstacle the two pipes to color
*/
void drawObstacle(SDL_Surface * surface, Obstacle * obstacle)
{
    drawRectangle(surface, obstacle->lower->coordinates, 255, 0, 0);
    drawRectangle(surface, obstacle->upper->coordinates, 0, 255, 0);
}

/*!
* \brief Deallocate memory of the obstacle and his pipes
* \param[out] obstacle the obstacle to deallocate
*/
void freeObstacle(Obstacle * obstacle)
{
    free(obstacle->lower);
    free(obstacle->upper);
    free(obstacle);
}
