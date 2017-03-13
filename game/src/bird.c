/*!
* \file bird.c
* \brief File containing the functions to control the bird
*/
#include "bird.h"

/*!
* \brief Create a bird in the middle of the window
* \return Return the created bird, NULL if error
*/
Bird * newBird()
{
    Bird * new_bird = (Bird*) malloc(sizeof(Bird));
    if(new_bird == NULL)
    {
        fprintf(stderr, "Bird allocation problem");
        return NULL;
    }
    new_bird->x = BIRD_X_OFFSET;
    new_bird->y = SCREEN_HEIGHT/2;
    new_bird->w = BIRD_SIZE;
    new_bird->h = BIRD_SIZE;
    new_bird->dir_y = 0;
    return new_bird;
}

/*!
*\brief Update the ordinate of the bird and the direction of its next move
*\param[out] bird the bird to be updated
*\param[in] t the detection of the touch that imply a jump
*/
void updateBird(Bird * bird, int t)
{
    if( t == JUMP)
    {
        bird->dir_y=BIRD_JUMP;
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

/*!
* \brief Deallocate memory of the bird
* \param[out] bird the bird to deallocate
*/
void freeBird(Bird * bird)
{
    free(bird);
}
