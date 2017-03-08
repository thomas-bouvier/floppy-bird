/*!
* \file game.c
* \brief File containing the functions to manage the game
*/
#include "control.h"

/*!
* \brief detect the interesting events for the project
* \param[out] event the list of event waiting to be treated
*\return Return return 1 if the event to jump is detected, 2 for the one to quit and 0 in other case
*/
int detectTouch(SDL_Event * event)
{
    while( SDL_PollEvent(event))
        {
            switch (event->type)
            {
                case SDL_KEYDOWN:
                    if(event->key.keysym.scancode==SDL_SCANCODE_ESCAPE)
                    {
                        return 2;
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if(event->button.button==SDL_BUTTON_LEFT)
                    {
                        return 1;
                    }
                    break;
                default:
                    ;
            }
        }
        return 0;
}
