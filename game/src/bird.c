/*!
* \file bird.c
* \brief File containing the functions to control the bird
*/
#include "bird.h"

/*!
* \brief Initialize the parameters of a given bird
* \return the bird created
*/
Bird * initBird(Genome * genome)
{
    Bird * new_bird = (Bird*) malloc(sizeof(Bird));

    if (new_bird == (Bird *) NULL) {
        fprintf(stderr, "Error while allocating memory for new Bird\n");
        return NULL;
    }

    new_bird->x = BIRD_X_OFFSET;
    new_bird->y = SCREEN_HEIGHT / 2;
    new_bird->w = BIRD_SIZE;
    new_bird->h = BIRD_SIZE;
    new_bird->dir_y = 0;
    new_bird->dead = 0;
    new_bird->must_jump = 0;
    new_bird->flaps = 0;
    new_bird->genome = genome;

    return new_bird;
}

/*!
*\brief Update the ordinate of the bird and the direction of its next move
*\param[out] bird the bird to be updated
*\param[in] t the detection of the touch that imply a jump
*\param[in] sound the sound played when the bird jump
*
* if the parameter t is equal to the constant JUMP then bird_y, the speed on the y axis of the bird take the value BIRD_JUMP
* in others cases bird_y take is increase by GRAVITY (increase because the value max on the y axis is the bottom of the screen)
* then the y coordinate is update in function of the value of its speed on the y axis. After that if the limit
* of the bird (bird->y - BIRD_SIZE/2) is higher than the limit of the screen (0), the bird is replace at the limit
* of the screen, and same for the bottom.
*/
void updateBird(Bird * bird, int t, Sound * sound)
{
    if (bird->dead == 0)
    {
        if (t == JUMP || bird->must_jump == 1)
        {
            bird->dir_y = BIRD_JUMP;
            (*sound) = JUMPSOUND;

            bird->must_jump = 0;
            ++bird->flaps;
        }
        else
        {
            bird->dir_y += GRAVITY;

            if (bird->dir_y > BIRD_MAX_FALL_SPEED)
                bird->dir_y = BIRD_MAX_FALL_SPEED;
        }

        bird->y += bird->dir_y;

        if(bird->y - BIRD_SIZE / 2 < 0)
            bird->y = BIRD_SIZE / 2;

        if(bird->y + BIRD_SIZE / 2 > SCREEN_HEIGHT)
            bird->y = SCREEN_HEIGHT - BIRD_SIZE / 2;
    }
}
/*!
* \brief Deallocate memory of the bird
* \param[out] bird the bird to deallocate
*/
void freeBird(Bird * bird)
{
    free(bird);
}
