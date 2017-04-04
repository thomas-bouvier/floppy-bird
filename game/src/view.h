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
#include <SDL2/SDL.h>

void drawRectangle(SDL_Renderer * renderer, Camera * camera, int x, int y, int w, int h, int r, int g, int b);
void drawBird(SDL_Renderer * renderer, Bird * bird, Camera * camera);
void drawObstacle(SDL_Renderer * renderer, Obstacle * obstacle, Camera * camera);
void drawLowForTI(SDL_Renderer * renderer, Camera * camera);
void drawUpForTI(SDL_Renderer * renderer, Camera * camera);
void displayGame(SDL_Renderer * renderer, Bird * bird, List * l, Camera * camera);
void quitGame(SDL_Window * window, SDL_Renderer * renderer);

#endif // VIEW_H
