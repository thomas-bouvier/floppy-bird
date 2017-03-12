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
#include <SDL2/SDL.h>

int initDisplay(SDL_Window * window, SDL_Renderer * renderer); //return 1 si tout est bon
void drawRectangle(SDL_Renderer * renderer, int x, int y, int w, int h, int r, int g, int b);
void drawBird(SDL_Renderer * renderer, Bird * bird);
void drawObstacle(SDL_Renderer * renderer, Obstacle * obstacle);
void displayGame(SDL_Renderer * renderer, Bird * bird, Obstacle * obstacle[], Camera * camera);
void quitGame(SDL_Window * window, SDL_Renderer * renderer);

#endif // VIEW_H
