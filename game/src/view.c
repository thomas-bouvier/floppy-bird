/*!
* \file view.c
* \brief File containing the functions relative to the SDL, that display elements on the screen
*/
#include "view.h"
#include "camera.h"

/*!
* \brief Color a rectangle
* \param[out] renderer the drawing target
* \param[in] camera the view of the scene
* \param[in] x the abscissa of the rectangle
* \param[in] y the ordinate of the rectangle
* \param[in] w the width of the rectangle
* \param[in] h the height of the rectangle
* \param[in] r the red component (integer between 0 and 255)
* \param[in] g the green component (integer between 0 and 255)
* \param[in] b the blue component (integer between 0 and 255)
*/
void drawRectangle(SDL_Renderer * renderer, Camera * camera, int x, int y, int w, int h, int r, int g, int b)
{
    SDL_Rect rect = {x - camera->x, y, w, h};
    SDL_SetRenderDrawColor( renderer, r, g, b, 255 );
    SDL_RenderFillRect(renderer, &rect);
}

/*!
* \brief Color the bird
* \param[out] renderer the drawing target
* \param[in] bird the bird to color
* \param[in] camera the view of the scene
*/
void drawBird(SDL_Renderer * renderer, Bird * bird, Camera * camera)
{
    drawRectangle(renderer, camera, bird->x - BIRD_SIZE/2, bird->y - BIRD_SIZE/2, bird->w, bird->h, 255, 0, 0);
}

/*!
* \brief Color the two pipes of an obstacle
* \param[out] renderer the drawing target
* \param[in] obstacle the two pipes to color
* \param[in] camera the view of the scene
*/
void drawObstacle(SDL_Renderer * renderer, Obstacle * obstacle, Camera * camera)
{
    Pipe obslow = obstacle->lower;
    Pipe obsup = obstacle->upper;
    drawRectangle(renderer, camera, obslow.x, obslow.y, obslow.w, obslow.h, 0, 0, 255);
    drawRectangle(renderer, camera, obsup.x, obsup.y, obsup.w, obsup.h, 0, 255, 0);
}

/*!
* \brief Display the items of the game
* \param[out] renderer the drawing target
* \param[in] bird the bird to display
* \param[in] l the list of obstacle
* \param[in] camera the view of the scene
*/
void displayGame(SDL_Renderer * renderer, Bird * bird, List * l, Camera * camera)
{
    int i = 0;
    setOnFirst(l);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    drawBird(renderer, bird, camera);
    while (i < OBSTACLE_NUMBER)
    {
        if (l->current->lower.x != 0){
            drawObstacle(renderer, l->current, camera);
            next(l);
            ++i;
        }
    }
    SDL_RenderPresent(renderer);
}

/*!
* \brief Quit the SDL and destroy renderer and window
* \param[out] window the window to destroy
* \param[out] renderer the renderer to destroy
*/
void quitGame(SDL_Window * window, SDL_Renderer * renderer)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
