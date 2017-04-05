/*!
* \file sound.c
* \brief File containing the functions to manage the sound
*/
#include "sound.h"


/*!
* \brief Play sound in the game
* \param[in] sound the sound to be played
* \param[in] jump_sound sound played when the bird jumps
* \param[in] obstacle_sound the sound played when the bird passes an obstacle
* \param[in] death_sound the sound played when the bird dies
*/
int playSound(Sound sound, Mix_Chunk * jump_sound, Mix_Chunk * obstacle_sound, Mix_Chunk * death_sound)
{
	if(sound == JUMPSOUND)
	{
		Mix_PlayChannel(0, jump_sound, 0);
		return 1;
	}
	if(sound == OBSTACLE)
	{
		Mix_PlayChannel(1, obstacle_sound, 0);
		return 1;
	}
	if(sound == DEATH)
	{
		Mix_PlayChannel(2, death_sound, 0);
		return 1;
	}
	return 0;
}
