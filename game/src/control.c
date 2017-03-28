/*!
* \file control.c
* \brief File containing the functions that allow the user to control the game
*/
#include "control.h"

/*!
* \brief Detect the interesting events for the game
* \return Return 1 if the event to jump is detected, 2 for the one to quit and 0 in other case
*
* While they are events in the event's list it test its type, if the key escape or window's cross is pressed
* the function return QUIT if it's a left click it return JUMP i other cases it return NOTHING
*/
Action detectTouch()
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
                case SDL_WINDOWEVENT:
                    if(event.window.event == SDL_WINDOWEVENT_CLOSE)
                        return QUIT;
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

void emptyEvent()
{
    SDL_Event event;
    while( SDL_PollEvent(&event));
}
