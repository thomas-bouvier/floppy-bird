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
#include "file.h"

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>

void drawRectangle(SDL_Renderer * renderer, Camera * camera, int x, int y, int w, int h, int r, int g, int b);
void drawBird(SDL_Renderer * renderer, Bird * bird, Camera * camera);
void drawObstacle(SDL_Renderer * renderer, Obstacle * obstacle, Camera * camera);
void drawSprite(SDL_Renderer * renderer, Camera * camera, SDL_Texture * texture, int x, int y, int w, int h);
void drawBackground(SDL_Renderer * renderer, Camera * camera, Sprites * sprites);
void drawRealObstacle(SDL_Renderer * renderer, Obstacle * obstacle, Camera * camera, SDL_Texture * pipe1, SDL_Texture * pipe2);
void drawRealBird(SDL_Renderer * renderer, Bird * bird, Camera * camera, SDL_Texture * bird1, SDL_Texture * bird2, SDL_Texture * bird3);
void displayGame(SDL_Renderer * renderer, GenericList * bird, GenericList * obstacle_list, Camera * camera, int score, TTF_Font * font, Sprites * sprites);
void displayRealGame(SDL_Renderer * renderer, GenericList * bird, GenericList * obstacle_list, Camera * camera, int score, TTF_Font * font, Sprites * sprites);
void drawPause(SDL_Renderer * renderer, Camera * camera, Sprites * sprites);
void drawForTI(SDL_Renderer * renderer, Camera * camera);
void quitGame(SDL_Window * window, SDL_Renderer * renderer);
void displayScore(SDL_Renderer * renderer, int score, TTF_Font * font);
int displayBestScore(SDL_Renderer * renderer, TTF_Font * font, FILE * scoreFile);
void tapToPlay(SDL_Renderer * renderer, Camera * camera, Sprites * sprites);

#endif // VIEW_H
