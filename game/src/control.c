/*!
* \file control.c
* \brief File containing the functions that allow the user to control the game
*/
#include "control.h"

/*!
* \brief Detect the interesting events for the game
* \return Return 1 if the event to jump is detected, 2 for the one to quit, 3 for the pause and 0 in other case
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
                        return QUIT;
                    if(event.key.keysym.scancode == SDL_SCANCODE_SPACE)
                    {
                        return PAUSE;
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
/*!
* \brief empty the stack containing all the event
*/
void emptyEvent()
{
    SDL_Event event;
    while( SDL_PollEvent(&event));
}
/*!
* \brief wait the right event to stop the pause of the game
* \return return PAUSE if the pause have to be stopped or QUIT if the game have to be stopped
*/
Action waiting()
{
    Action a = NOTHING;
    while((a != PAUSE) && (a != QUIT))
    {
        a = detectTouch();
    }
    return a;
}
/*!
* \brief wait a left click on a square at the center of the screen
* \return return 0 if the game have to be stop and 1 if all went good
*/
int waitForTI()
{
    int run = 1;
    SDL_Event event;
    while(run)
    {
        SDL_PollEvent(&event);
        if(event.window.event == SDL_WINDOWEVENT_CLOSE)
            return 0;
        if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
            return 0;
        if(event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
        {
            int x = event.button.x;
            int y = event.button.y;
            int x_max = SCREEN_WIDTH/2 + BIRD_SIZE/2;
            int x_min = SCREEN_WIDTH/2 - BIRD_SIZE/2;
            int y_max = SCREEN_HEIGHT/2 + BIRD_SIZE/2;
            int y_min = SCREEN_HEIGHT/2 - BIRD_SIZE/2;
            if(x <= x_max && x >= x_min && y <= y_max && y >= y_min)
                run = 0;
        }
    }
    return 1;
}
