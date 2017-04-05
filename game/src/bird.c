/*!
* \file bird.c
* \brief File containing the functions to control the bird
*/
#include "bird.h"

/*!
* \brief Initialize the parameters of a given bird
* \param[out] new_bird the given bird
*/
void initBird(Bird * new_bird)
{
    new_bird->x = BIRD_X_OFFSET;
    new_bird->y = SCREEN_HEIGHT/2;
    new_bird->w = BIRD_SIZE;
    new_bird->h = BIRD_SIZE;
    new_bird->dir_y = 0;
}

/*!
*\brief Update the ordinate of the bird and the direction of its next move
*\param[out] bird the bird to be updated
*\param[in] t the detection of the touch that imply a jump
*
* if the parameter t is equal to the constant JUMP then bird_y, the speed on the y axis of the bird take the value BIRD_JUMP
* in others cases bird_y take is increase by GRAVITY (increase because the value max on the y axis is the bottom of the screen)
* then the y coordinate is update in function of the value of its speed on the y axis. After that if the limit
* of the bird (bird->y - BIRD_SIZE/2) is higher than the limit of the screen (0), the bird is replace at the limit
* of the screen, and same for the bottom.
*/
void updateBird(Bird * bird, int t, Sound * sound)
{
    if(t == JUMP)
	{
        bird->dir_y = BIRD_JUMP;
        (*sound) = JUMPSOUND;
    }
    else
    {
        bird->dir_y += GRAVITY;
        if(bird->dir_y > BIRD_MAX_FALL_SPEED)
            bird->dir_y = BIRD_MAX_FALL_SPEED;
    }
    bird->y += bird->dir_y;
    if(bird->y - BIRD_SIZE/2 < 0)
        bird->y = BIRD_SIZE/2;
    if(bird->y + BIRD_SIZE/2 > SCREEN_HEIGHT)
        bird->y = SCREEN_HEIGHT - BIRD_SIZE/2;
}
