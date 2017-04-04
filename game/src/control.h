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
int waitForTI();


#endif // CONTROL_H
