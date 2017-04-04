/*!
* \file sound.c
* \brief File containing the functions to manage the sound
*/
#include "sound.h"

int playSound(Sound sound)
{
	Mix_AllocateChannels(3);
	Mix_Chunk * son1;
	Mix_Chunk * son2;
	Mix_Chunk * son3;
	son1 = Mix_LoadWAV("jump.wav");
	son2 = Mix_LoadWAV("obstacle.wav");
	son3 = Mix_LoadWAV("death.wav");
	if(sound == JUMPSOUND)
	{
		Mix_PlayChannel(0, son1, 0);
		return 1;
	}
	if(sound == OBSTACLE)
	{
		Mix_PlayChannel(1, son2, 0);
		return 1;
	}
	if(sound == DEATH)
	{
		Mix_PlayChannel(2, son3, 0);
		return 1;
	}
	Mix_FreeChunk(son1);
    Mix_FreeChunk(son2);
    Mix_FreeChunk(son3);
	return 0;
}
