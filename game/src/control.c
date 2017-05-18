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
    while(SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_KEYDOWN:
                    if(event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                        return MENU;
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
                        if(event.button.x >= SCREEN_WIDTH - 80 && event.button.x <= SCREEN_WIDTH - 30 && event.button.y >= 40 && event.button.y <= 90)
                            return PAUSE;
                        else
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
int waitClick()
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
            run = 0;
        }
    }
    return 1;
}

/*!
* \brief Select the mode of game of the player by a click on the options displayed by the menu
* \param[in] event the current event for the SDL
* \param[out] levelFromFile allow to choose if the obstacles are predefined are random
* \param[out] simplifiedMode allow to choose if the game is run in normal or simplified mode
* \param[out] speedAcceleration allow to choose if the player want an acceleration during the game
* \return the choice of mode wanted by the player (WAIT, PLAY, IA1, IA2 or QUITGAME)
*/
int actionOnMainMenu(SDL_Event event, int * levelFromFile, int * simplifiedMode, int * speedAcceleration)
{
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
    {
        if(event.button.x > 100 && event.button.x < 240 && event.button.y > 100 && event.button.y < 171)
            return PLAY;
        if(event.button.x > 100 && event.button.x < 395 && event.button.y > 200 && event.button.y < 271)
            return IA1;
        if(event.button.x > 100 && event.button.x < 410 && event.button.y > 300 && event.button.y < 371)
            return IA2;
        if(event.button.x > 300 && event.button.x < 493 && event.button.y > 450 && event.button.y < 491)
            *simplifiedMode = (*simplifiedMode != 1);
        if(event.button.x > 300 && event.button.x < 518 && event.button.y > 520 && event.button.y < 561)
            *levelFromFile = (*levelFromFile != 1);
        if(event.button.x > 450 && event.button.x < 519 && event.button.y > 590 && event.button.y < 631)
            *speedAcceleration = (*speedAcceleration != 1);
        if(event.button.x > 700 && event.button.x < 900 && event.button.y > 650 && event.button.y < 691)
            return QUITGAME;
        SDL_Delay(200);
    }
    return WAIT;
}

/*!
* \brief Select the action on the pause menu
* \return RESUME, MENU, QUIT or PAUSE according to the user action
*/
int actionOnPauseMenu()
{
    SDL_Event event;
    SDL_PollEvent(&event);
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
    {
        if(event.button.x > 300 && event.button.x < 450 && event.button.y > SCREEN_HEIGHT/2 - 75 && event.button.y < SCREEN_HEIGHT/2 + 75)
            return RESUME;
        if(event.button.x > 600 && event.button.x < 750 && event.button.y > SCREEN_HEIGHT/2 - 75 && event.button.y < SCREEN_HEIGHT/2 + 75)
            return MENU;
    }
    if(event.window.event == SDL_WINDOWEVENT_CLOSE)
        return QUIT;
    return PAUSE;
}

/*!
* \brief Select the action when the bird dies
* \return MENU, RESUME, QUIT or NOTHING according to the user action
*/
int actionAtEnd(SDL_Event event)
{
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
    {
        if(event.button.x > 600 && event.button.x < 800 && event.button.y > 650 && event.button.y < 691)
            return MENU;
        else
            return RESUME;
    }
    if(event.window.event == SDL_WINDOWEVENT_CLOSE)
        return QUIT;
    return NOTHING;
}
