/*!
* \file view.c
* \brief File containing the functions relative to the SDL, that display elements on the screen
*/
#include "view.h"
#include "camera.h"

/*!
* \brief Initialization of the SDL
* \param[out] window the window to create
* \param[out] renderer the renderer to setup
* \return Return 1 if the window and renderer are well created, 0 if failure
*/
int initDisplay(SDL_Window * window, SDL_Renderer * renderer)
{
    /* SDL initialization */
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "SDL initialization failure");
        return 0;
    }

    /* Setup window */
    window = SDL_CreateWindow("Floppy Bird",
                              SDL_WINDOWPOS_UNDEFINED,
                              SDL_WINDOWPOS_UNDEFINED,
                              SCREEN_WIDTH,
                              SCREEN_HEIGHT,
                              SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        fprintf(stderr, "Opening window failure %s\n,", SDL_GetError());
        return 0;
    }

    /* Setup renderer */
    renderer =  SDL_CreateRenderer(window,
                                   -1,
                                   SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor( renderer, 255, 255, 255, 255 );
    /*if (renderer == NULL);
    {
        fprintf(stderr, "Renderer creation failure : %s\n", SDL_GetError());
        return 0;
    }*/

    return 1;
}

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
*/
void drawBird(SDL_Renderer * renderer, Bird * bird, Camera * camera)
{
    drawRectangle(renderer, camera, bird->x - BIRD_SIZE/2, bird->y - BIRD_SIZE/2, bird->w, bird->h, 255, 0, 0);
}

/*!
* \brief Color the two pipes of an obstacle
* \param[out] renderer the drawing target
* \param[in] obstacle the two pipes to color
*/
void drawObstacle(SDL_Renderer * renderer, Obstacle * obstacle, Camera * camera)
{
    Pipe * obslow = obstacle->lower;
    Pipe * obsup = obstacle->lower;
    drawRectangle(renderer, camera, obslow->x, obslow->y, obslow->w, obslow->h, 0, 0, 255);
    drawRectangle(renderer, camera, obsup->x, obsup->y, obsup->w, obsup->h, 0, 255, 0);
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
    drawBird(renderer, bird, camera);
    for (i=0 ; i<PIPES_ON_SCREEN ; ++i)
        drawObstacle(renderer, obstacle[i], camera);
    SDL_RenderPresent(renderer);
    SDL_Delay(30);
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
