/*!
* \file file.h
* \brief File containing the prototypes to manage the extern files
*/
#ifndef FILE_H
#define FILE_H

#include "constants.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int readLevel(FILE * f, int number);
int readConfig(FILE * f, char * config, char * type);
int saveScore(FILE * f, int score);
int readBestScore(FILE * f);
int openGameFiles(FILE * config, FILE ** level, FILE ** scoreFile);
int openSoundFiles(FILE * config, Mix_Chunk ** jump_sound, Mix_Chunk ** obstacle_sound, Mix_Chunk ** death_sound);
int openSpriteFiles(FILE * config, Sprites * sprites);
int openFontFiles(FILE * config, TTF_Font ** font);
void closeFiles(FILE * config, FILE * level, FILE * scoreFile, Mix_Chunk * jump_sound, Mix_Chunk * obstacle_sound,
               Mix_Chunk * death_sound, Sprites * sprites, TTF_Font * font);

#endif // FILE_H
