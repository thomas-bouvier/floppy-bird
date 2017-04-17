/*!
* \file menu.c
* \brief File containing the functions to display the main menu of the game
*/
#include "menu.h"

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

void mainMenu(SDL_Renderer * renderer, TTF_Font * font)
{
    SDL_RenderClear(renderer);
    SDL_Event event;
    SDL_PollEvent(&event);
    if(event.motion.x > 437 && event.motion.x < 587 && event.motion.y > 200 && event.motion.y < 271)
        TTF_SetFontStyle(font, TTF_STYLE_ITALIC);
    printText(renderer, "Play", 200, font);
    if(event.motion.x > 365 && event.motion.x < 659 && event.motion.y > 400 && event.motion.y < 471)
        TTF_SetFontStyle(font, TTF_STYLE_ITALIC);
    printText(renderer, "IA1 Demo", 400, font);
    if(event.motion.x > 357 && event.motion.x < 667 && event.motion.y > 600 && event.motion.y < 671)
        TTF_SetFontStyle(font, TTF_STYLE_ITALIC);
    printText(renderer, "IA2 Demo", 600, font);
    SDL_RenderPresent(renderer);
}
