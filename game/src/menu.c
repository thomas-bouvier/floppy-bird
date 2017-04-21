/*!
* \file menu.c
* \brief File containing the functions to display the main menu of the game
*/
#include "menu.h"

/*!
* \brief Print a text passed in parameters, at the y position given in parameter (center of the screen in abscissa)
* \param[out] renderer the drawing target
* \param[in] text the text to be written
* \param[in] position the y position of the text on screen (from the upper side)
* \param[in] font the font used
*/
void printText(SDL_Renderer * renderer, char * text, int position, TTF_Font * font)
{
    SDL_Color color = {0, 0, 0};
	SDL_Surface * text_surface = TTF_RenderText_Blended(font, text, color);
	SDL_Texture * text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	SDL_Rect dest = {SCREEN_WIDTH/2 - text_surface->w/2, position, text_surface->w, text_surface->h};
	//printf("%d %d %d %d\n", SCREEN_WIDTH/2 - text_surface->w/2, SCREEN_WIDTH/2 + text_surface->w/2, position, position + text_surface->h);
    TTF_SetFontStyle(font, TTF_STYLE_NORMAL);
	SDL_RenderCopy(renderer, text_texture, NULL, &dest);
	SDL_FreeSurface(text_surface);
	SDL_DestroyTexture(text_texture);
}

/*!
* \brief Write the content of the main menu, and start the game in function of the click of the player
* \param[out] renderer the drawing target
* \param[in] font the font used
* \param[out] levelFromFile allow to choose if the obstacles are predefined are random
* \param[out] simplifiedMode allow to choose if the game is run in normal or simplified mode
* \return the choice of mode wanted by the player
*/
int  mainMenu(SDL_Renderer * renderer, TTF_Font * font, int * levelFromFile, int * simplifiedMode)
{
    SDL_RenderClear(renderer);
    SDL_Event event;
    SDL_PollEvent(&event);
    /* Play */
    if(event.motion.x > 437 && event.motion.x < 587 && event.motion.y > 150 && event.motion.y < 221)
        TTF_SetFontStyle(font, TTF_STYLE_ITALIC);
    printText(renderer, "Play", 150, font);
    /* IA1 Demo */
    if(event.motion.x > 365 && event.motion.x < 659 && event.motion.y > 300 && event.motion.y < 371)
        TTF_SetFontStyle(font, TTF_STYLE_ITALIC);
    printText(renderer, "IA1 Demo", 300, font);
    /* IA2 Demo */
    if(event.motion.x > 357 && event.motion.x < 667 && event.motion.y > 450 && event.motion.y < 521)
        TTF_SetFontStyle(font, TTF_STYLE_ITALIC);
    printText(renderer, "IA2 Demo", 450, font);
    /* Simplified/Normal Mode */
    if(event.motion.x > 237 && event.motion.x < 787 && event.motion.y > 600 && event.motion.y < 671)
        TTF_SetFontStyle(font, TTF_STYLE_ITALIC);
    if((*simplifiedMode) == 1)
        printText(renderer, "Simplified Mode", 600, font);
    else
        printText(renderer, "Normal Mode", 600, font);
    /* Predefined/Random Level */
    if(event.motion.x > 222 && event.motion.x < 802 && event.motion.y > 750 && event.motion.y < 821)
        TTF_SetFontStyle(font, TTF_STYLE_ITALIC);
    if((*levelFromFile) == 1)
        printText(renderer, "Predefined Level", 750, font);
    else
        printText(renderer, "Random Level", 750, font);
    /* Displaying */
    SDL_RenderPresent(renderer);
    return actionOnMenu(event, levelFromFile, simplifiedMode);
}

/*!
* \brief Select the mode of game of the player by a click on the options displayed by the menu
* \param[in] event the current event for the SDL
* \param[out] levelFromFile allow to choose if the obstacles are predefined are random
* \param[out] simplifiedMode allow to choose if the game is run in normal or simplified mode
* \return the choice of mode wanted by the player (WAIT, PLAY, IA1 or IA2)
*/
int actionOnMenu(SDL_Event event, int * levelFromFile, int * simplifiedMode)
{
    if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT)
    {
        if(event.button.x > 437 && event.button.x < 587 && event.button.y > 150 && event.button.y < 221)
            return PLAY;
        if(event.button.x > 365 && event.button.x < 659 && event.button.y > 300 && event.button.y < 371)
            return IA1;
        if(event.button.x > 357 && event.button.x < 667 && event.button.y > 450 && event.button.y < 521)
            return IA2;
        if(event.button.x > 237 && event.button.x < 787 && event.button.y > 600 && event.button.y < 671)
            *simplifiedMode = (*simplifiedMode != 1);
        if(event.button.x > 222 && event.button.x < 802 && event.button.y > 750 && event.button.y < 821)
            *levelFromFile = (*levelFromFile != 1);
        SDL_Delay(200);
    }
    return WAIT;
}
