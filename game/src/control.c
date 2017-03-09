/*!
* \file control.c
* \brief File containing the functions that allow the user to control the game
*/
#include "control.h"

/*!
* \brief Detect the interesting events for the project
*\return Return return 1 if the event to jump is detected, 2 for the one to quit and 0 in other case
*/
int detectTouch()
{
    SDL_Event event;
    while( SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_KEYDOWN:
                    if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                    {
                        return QUIT;
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if(event.button.button == SDL_BUTTON_LEFT)
                    {
                        return JUMP;
                    }
                    break;
            }
        }
        return NOTHING;
}
