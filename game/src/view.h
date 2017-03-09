#ifndef VIEW_H
#define VIEW_H

#include "constants.h"
#include "bird.h"
#include "obstacle.h"
#include "camera.h"
#include <SDL2/SDL.h>

void initDisplay(SDL_Window * window, SDL_Renderer * renderer);
void drawRectangle(SDL_Surface * surface, int x, int y, int w, int h, int r, int g, int b);
void drawBird(SDL_Surface * surface, Bird * bird);
void drawObstacle(SDL_Surface * surface, Obstacle * obstacle);
void displayGame(Bird * bird, Obstacle * obstacle[], Camera * camera);
void quitGame(SDL_Window * window, SDL_Renderer * renderer);

#endif // VIEW_H
