#include "view.h"

/*!
* \brief Color a rectangle
* \param[out] surface the drawing target
* \param[in] x the abscissa of the rectangle
* \param[in] y the ordinate of the rectangle
* \param[in] w the width of the rectangle
* \param[in] h the height of the rectangle
* \param[in] r the red component (integer between 0 and 255)
* \param[in] g the green component (integer between 0 and 255)
* \param[in] b the blue component (integer between 0 and 255)
*/
void drawRectangle(SDL_Surface * surface, int x, int y, int w, int h, int r, int g, int b)
{
    SDL_Rect rect = {x, y, w, h};
    SDL_FillRect(surface, &rect, SDL_MapRGB(surface->format, r, g, b));
}

/*!
* \brief Color the bird
* \param[out] surface the drawing target
* \param[in] bird the bird to color
*/
void drawBird(SDL_Surface * surface, Bird * bird)
{
    drawRectangle(surface, bird->x - BIRD_SIZE/2, bird->y - BIRD_SIZE/2, bird->w, bird->h, 255, 0, 0);
}

/*!
* \brief Color the two pipes of an obstacle
* \param[out] surface the drawing target
* \param[in] obstacle the two pipes to color
*/
void drawObstacle(SDL_Surface * surface, Obstacle * obstacle)
{
    Pipe * obslow = obstacle->lower;
    Pipe * obsup = obstacle->lower;
    drawRectangle(surface, obslow->x, obslow->y, obslow->w, obslow->h, 0, 0, 255);
    drawRectangle(surface, obsup->x, obsup->y, obsup->w, obsup->h, 0, 255, 0);
}
