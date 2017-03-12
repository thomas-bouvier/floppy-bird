/*!
* \file view.c
* \brief File containing the functions relative to the SDL, that display elements on the screen
*/
#include "view.h"

/*!
* \brief Color a rectangle
* \param[out] renderer the drawing target
* \param[in] x the abscissa of the rectangle
* \param[in] y the ordinate of the rectangle
* \param[in] w the width of the rectangle
* \param[in] h the height of the rectangle
* \param[in] r the red component (integer between 0 and 255)
* \param[in] g the green component (integer between 0 and 255)
* \param[in] b the blue component (integer between 0 and 255)
*/
void drawRectangle(SDL_Renderer * renderer, int x, int y, int w, int h, int r, int g, int b)
{
    SDL_Rect rect = {x, y, w, h};
    SDL_SetRenderDrawColor( renderer, r, g, b, 255 );
    SDL_RenderFillRect(renderer, &rect);
}

/*!
* \brief Color the bird
* \param[out] renderer the drawing target
* \param[in] bird the bird to color
*/
void drawBird(SDL_Renderer * renderer, Bird * bird)
{
    drawRectangle(renderer, bird->x - BIRD_SIZE/2, bird->y - BIRD_SIZE/2, bird->w, bird->h, 255, 0, 0);
}

/*!
* \brief Color the two pipes of an obstacle
* \param[out] renderer the drawing target
* \param[in] obstacle the two pipes to color
*/
void drawObstacle(SDL_Renderer * renderer, Obstacle * obstacle)
{
    Pipe * obslow = obstacle->lower;
    Pipe * obsup = obstacle->lower;
    drawRectangle(renderer, obslow->x, obslow->y, obslow->w, obslow->h, 0, 0, 255);
    drawRectangle(renderer, obsup->x, obsup->y, obsup->w, obsup->h, 0, 255, 0);
}

/*!
* \brief Display the items of  the game
* \param[out] renderer the drawing target
* \param[in] bird the bird to display
* \param[in] obstacle the table of obstacle to display
* \param[in] obstacle the camera
*/
void displayGame(SDL_Renderer * renderer, Bird * bird, Obstacle * obstacle[], Camera * camera)
{
    int i;
    SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 );
    SDL_RenderClear(renderer);
    drawBird(renderer, bird);
    for (i=0 ; i<PIPES_ON_SCREEN ; ++i)
        drawObstacle(renderer, obstacle[i]);
    SDL_RenderPresent(renderer);
    SDL_Delay(30);
}
