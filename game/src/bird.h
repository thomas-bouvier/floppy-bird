/*!
* \file bird.h
* \brief File containing structures and prototypes for the bird
*/
#ifndef BIRD_H
#define BIRD_H

#include "constants.h"
#include <stdlib.h>
#include <stdio.h>

/*!
* \struct Bird bird.h
* \brief The bird that the player controls by taping on the screen or clicking on the mouse
*/
typedef struct{
    int x;                      /*!< the abscissa of the middle of the bird */
    int y;                      /*!< the ordinate of the middle of bird */
    int h;                      /*!< the height of the bird */
    int w;                      /*!< the width of the bird */
    int dir_y;                  /*!< the speed of climb/fall of the bird */
} Bird;

void initBird(Bird * new_bird);
void updateBird(Bird * bird, int t, Sound * sound);

#endif // BIRD_H
