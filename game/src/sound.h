/*!
* \file sound.h
* \brief File containing the prototypes for the sound
*/
#ifndef SOUND_H
#define SOUND_H

#include "constants.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <stdlib.h>
#include <stdio.h>

int playSound(Sound sound, Mix_Chunk * jump_sound, Mix_Chunk * obstacle_sound, Mix_Chunk * death_sound);

#endif // SOUND_H
