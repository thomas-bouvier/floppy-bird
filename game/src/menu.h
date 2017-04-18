/*!
* \file menu.h
* \brief File containing the prototypes for the main menu
*/
#ifndef MENU_H
#define MENU_H

#include "constants.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>

void printText(SDL_Renderer * renderer, char * text, int position, TTF_Font * font);
int mainMenu(SDL_Renderer * renderer, TTF_Font * font);
int actionOnMenu(SDL_Event event);

#endif // MENU_H
