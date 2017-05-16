/*!
* \file menu.h
* \brief File containing the prototypes for the main menu
*/
#ifndef MENU_H
#define MENU_H

#include "constants.h"
#include "view.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

void printText(SDL_Renderer * renderer, char * text, int abscissa, int ordinate, TTF_Font * font);
int mainMenu(SDL_Renderer * renderer, Camera * camera, TTF_Font * big_font, TTF_Font * medium_font, int * levelFromFile, int * simplifiedMode, int * speedAcceleration, Sprites * Sprites);
int actionOnMainMenu(SDL_Event event, int * levelFromFile, int * simplifiedMode, int * speedAcceleration);
void pauseMenu(SDL_Renderer * renderer, Camera * camera, Sprites * Sprites);
int actionOnPauseMenu();
int endOfGame(SDL_Renderer * renderer, Camera * camera, TTF_Font * medium_font);

#endif // MENU_H
