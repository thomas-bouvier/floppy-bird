/*!
* \file game.c
* \brief File containing the functions to manage the game
*/

#include "game.h"
#include <stdlib.h>
#include <stdio.h>

static Bird * newBird(int bird_x, int bird_y, char * bird_sprite, int bird_gravity)
{
    Bird * new_bird = (Bird*) malloc(sizeof(Bird));
    if(new_bird == NULL)
    {
        fprintf(stderr, "Bird allocation problem");
        return EXIT_FAILURE;
    }
    new_bird->x = bird_x;
    new_bird->y = bird_y;
    new_bird->sprite = bird_sprite;
    new_bird->gravity = bird_gravity;
    return new_bird;
}*/

