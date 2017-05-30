/*!
* \file control.h
* \brief File containing the prototypes for the control
*/
#ifndef CONTROL_H
#define CONTROL_H

#include "constants.h"
#include <SDL2/SDL.h>

Action detectTouch();
void emptyEvent();
Action waiting();
int waitClick();
int actionOnMainMenu(SDL_Event event, int * levelFromFile, int * simplifiedMode, int * speedAcceleration, int * gapModification);
int actionOnPauseMenu();
int actionAtEnd(SDL_Event event);


#endif // CONTROL_H
