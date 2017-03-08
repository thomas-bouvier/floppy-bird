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
*/
void updateBird(Bird * bird)
{
    bird->y += bird->dir_y;
    if(bird->y < 0)
        bird->y = 0;
    if(bird->y > SCREEN_HEIGHT)
        bird->y = SCREEN_HEIGHT;
    bird->dir_y += GRAVITY;
    if(bird->dir_y > BIRD_MAX_FALL_SPEED)
        bird->dir_y = BIRD_MAX_FALL_SPEED;
}

/*!
* \brief Deallocate memory of the bird
* \param[out] bird the bird to deallocate
*/
void freeBird(Bird * bird)
{
    free(bird);
}
