/*!
* \file game.c
* \brief File containing the functions to manage the game
*/

#include "game.h"








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


