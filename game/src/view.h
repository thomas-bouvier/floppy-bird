/*!
* \file view.h
* \brief File containing the prototypes relative to the SDL, that display elements on the screen
*/
#ifndef VIEW_H
#define VIEW_H

#include "constants.h"
#include "bird.h"
#include "obstacle.h"
#include "camera.h"
#include "list.h"
#include "file.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>

/*!
* \struct Sprites view.h
* \brief The structure containing all the image useful to the game
*/
typedef struct{
    SDL_Surface * bird1;        /*!< The first image of the bird */
    SDL_Surface * bird2;        /*!< The second image of the bird */
    SDL_Surface * bird3;        /*!< The third image of the bird */
    SDL_Surface * pipe1;        /*!< The image of the upper pipe */
    SDL_Surface * pipe2;        /*!< The image of the lower pipe */
    SDL_Surface * background;   /*!< The image of the background */
    SDL_Surface * ground;       /*!< The image of the ground */
}Sprites;

void drawRectangle(SDL_Renderer * renderer, Camera * camera, int x, int y, int w, int h, int r, int g, int b);
void drawBird(SDL_Renderer * renderer, Bird * bird, Camera * camera);
void drawObstacle(SDL_Renderer * renderer, Obstacle * obstacle, Camera * camera);
void drawSprite(SDL_Renderer * renderer, Camera * camera, SDL_Surface * surface, int x, int y, int w, int h);
void drawBackground(SDL_Renderer * renderer, Camera * camera, Sprites * sprites);
void drawRealObstacle(SDL_Renderer * renderer, Obstacle * obstacle, Camera * camera, Sprites * sprites);
void drawRealBird(SDL_Renderer * renderer, Bird * bird, Camera * camera, Sprites * sprites);
void displayGame(SDL_Renderer * renderer, Bird * bird, List * l, Camera * camera, int score, TTF_Font * font);
void displayRealGame(SDL_Renderer * renderer, Bird * bird, List * l, Camera * camera, int score, TTF_Font * font, Sprites * sprites);
void drawForTI(SDL_Renderer * renderer, Camera * camera);
void quitGame(SDL_Window * window, SDL_Renderer * renderer);
void displayScore(SDL_Renderer * renderer, int score, TTF_Font * font);
int displayBestScore(SDL_Renderer * renderer, TTF_Font * font, FILE * scoreFile);

#endif // VIEW_H
