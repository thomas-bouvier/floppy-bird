/*!
* \file sdl_files.h
* \brief File containing the prototypes to open SDL files
*/
#ifndef SDL_FILES_H
#define SDL_FILES_H

#include "constants.h"
#include "file.h"
#include <stdlib.h>
#include <stdio.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

int openSoundFiles(FILE * config, Mix_Chunk ** jump_sound, Mix_Chunk ** obstacle_sound, Mix_Chunk ** death_sound);
int openSpriteFiles(FILE * config, Sprites * sprites, SDL_Renderer * renderer);
int addBirdSprite(FILE * config, Sprites * sprites, SDL_Renderer * renderer);
int openFontFiles(FILE * config, TTF_Font ** font, TTF_Font ** medium);
void closeFiles(FILE * config, FILE * level, FILE * scoreFile, Mix_Chunk * jump_sound, Mix_Chunk * obstacle_sound,
               Mix_Chunk * death_sound, Sprites * sprites, TTF_Font * font);


#endif // SDL_FILES_H
