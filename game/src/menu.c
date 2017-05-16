/*!
* \file menu.c
* \brief File containing the functions to display the main menu of the game
*/
#include "menu.h"

/*!
* \brief Print a text passed in parameters, at the y position given in parameter (center of the screen in abscissa)
* \param[out] renderer the drawing target
* \param[in] text the text to be written
* \param[in] abscissa the x position of the text on screen (from the left side)
* \param[in] ordinate the y position of the text on screen (from the upper side)
* \param[in] font the font used
*/
void printText(SDL_Renderer * renderer, char * text, int abscissa, int ordinate, TTF_Font * font)
{
    SDL_Color color = {0, 0, 0};
	SDL_Surface * text_surface = TTF_RenderText_Blended(font, text, color);
	SDL_Texture * text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);
	SDL_Rect dest = {abscissa, ordinate, text_surface->w, text_surface->h};
	/*printf("%d %d %d %d\n", abscissa, abscissa + text_surface->w, ordinate, ordinate + text_surface->h);*/
    TTF_SetFontStyle(font, TTF_STYLE_NORMAL);
	SDL_RenderCopy(renderer, text_texture, NULL, &dest);
	SDL_FreeSurface(text_surface);
	SDL_DestroyTexture(text_texture);
}

/*!
* \brief Write the content of the main menu, and start the game in function of the click of the player
* \param[out] renderer the drawing target
* \param[in] big_font the biggest font used in the game
* \param[in] medium_font the same font, but smaller
* \param[in] camera the view of the scene
* \param[out] levelFromFile allow to choose if the obstacles are predefined are random
* \param[out] simplifiedMode allow to choose if the game is run in normal or simplified mode
* \param[in] sprites the structure containing all the image of the game
* \return the choice of mode wanted by the player
*/
int  mainMenu(SDL_Renderer * renderer, Camera * camera, TTF_Font * big_font, TTF_Font * medium_font, int * levelFromFile, int * simplifiedMode, int * speedAcceleration, Sprites * sprites)
{
    SDL_RenderClear(renderer);
    drawBackground(renderer, camera, sprites);
    SDL_Event event;
    SDL_PollEvent(&event);
    /* Play */
    if(event.motion.x > 100 && event.motion.x < 240 && event.motion.y > 100 && event.motion.y < 171)
        TTF_SetFontStyle(big_font, TTF_STYLE_ITALIC);
    printText(renderer, "Play", 100, 100, big_font);
    /* IA1 Demo */
    if(event.motion.x > 100 && event.motion.x < 395 && event.motion.y > 200 && event.motion.y < 271)
        TTF_SetFontStyle(big_font, TTF_STYLE_ITALIC);
    printText(renderer, "IA1 Demo", 100, 200, big_font);
    /* IA2 Demo */
    if(event.motion.x > 100 && event.motion.x < 410 && event.motion.y > 300 && event.motion.y < 371)
        TTF_SetFontStyle(big_font, TTF_STYLE_ITALIC);
    printText(renderer, "IA2 Demo", 100, 300, big_font);
    /* Simplified/Normal Mode */
    printText(renderer, "Mode :", 100, 450, medium_font);
    if(event.motion.x > 300 && event.motion.x < 493 && event.motion.y > 450 && event.motion.y < 491)
        TTF_SetFontStyle(medium_font, TTF_STYLE_ITALIC);
    if((*simplifiedMode) == 1)
        printText(renderer, "Simplified", 300, 450, medium_font);
    else
        printText(renderer, "Normal", 300, 450, medium_font);
    /* Predefined/Random Level */
    printText(renderer, "Level :", 100, 520, medium_font);
    if(event.motion.x > 300 && event.motion.x < 518 && event.motion.y > 520 && event.motion.y < 561)
        TTF_SetFontStyle(medium_font, TTF_STYLE_ITALIC);
    if((*levelFromFile) == 1)
        printText(renderer, "Predefined", 300, 520, medium_font);
    else
        printText(renderer, "Random", 300, 520, medium_font);
    /* Speed acceleration */
    printText(renderer, "Acceleration :", 100, 590, medium_font);
    if(event.motion.x > 450 && event.motion.x < 519 && event.motion.y > 590 && event.motion.y < 631)
        TTF_SetFontStyle(medium_font, TTF_STYLE_ITALIC);
    if((*speedAcceleration) == 1)
        printText(renderer, "Yes", 450, 590, medium_font);
    else
        printText(renderer, "No", 450, 590, medium_font);
    /* Quit game */
    if(event.motion.x > 700 && event.motion.x < 900 && event.motion.y > 650 && event.motion.y < 691)
        TTF_SetFontStyle(medium_font, TTF_STYLE_ITALIC);
    printText(renderer, "Quit game", 700, 650, medium_font);
    /* Displaying */
    SDL_RenderPresent(renderer);
    SDL_Delay(10);
    return actionOnMainMenu(event, levelFromFile, simplifiedMode, speedAcceleration);
}

/*!
* \brief Display the pause menu
* \param[out] renderer the drawing target
* \param[in] camera the view of the game
* \param[in] sprites the structure that contains the Surfaces for the game
*/
void pauseMenu(SDL_Renderer * renderer, Camera * camera, Sprites * sprites)
{
    drawSprite(renderer, camera, sprites->play, camera->x + 300, SCREEN_HEIGHT/2 - 75, 150, 150);
    drawSprite(renderer, camera, sprites->quit, camera->x + 600, SCREEN_HEIGHT/2 - 75, 150, 150);
    SDL_RenderPresent(renderer);
    SDL_Delay(10);
}

/*!
* \brief Manage the view and control when the player loses
* \param[out] renderer the drawing target
* \param[in] camera the view of the game
* \param[in] medium_font the font used to write text
* \return the action to do
*/
int endOfGame(SDL_Renderer * renderer, Camera * camera, TTF_Font * medium_font)
{
    SDL_Event event;
    SDL_PollEvent(&event);
    /*if(event.motion.x > 600 && event.motion.x < 800 && event.motion.y > 650 && event.motion.y < 691)
        TTF_SetFontStyle(medium_font, TTF_STYLE_ITALIC);*/
    printText(renderer, "Return to menu", 600, 650, medium_font);
    SDL_Delay(10);
    return actionAtEnd(event);
}
