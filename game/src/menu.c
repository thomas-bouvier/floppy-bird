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
	SDL_Rect dest = {SCREEN_HEIGHT/2 - text_surface->w/2, position, text_surface->w, text_surface->h};
	SDL_RenderCopy(renderer, text_texture, NULL, &dest);
	SDL_FreeSurface(text_surface);
	SDL_DestroyTexture(text_texture);
}


