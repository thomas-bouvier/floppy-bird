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
* \struct Bird game.h
* \brief The bird that the player controls by taping on the screen or clicking on the mouse
*/
typedef struct{
    int x;                      /*!< the abscissa of the bird */
    int y;                      /*!< the ordinate of the bird */
    int h;                      /*!< the height of the bird */
    int w;                      /*!< the width of the bird */
    int dir_y;                  /*!< the speed of climb/fall of the bird */
} Bird;

Bird * newBird();

void updateBird(Bird * bird);
void freeBird(Bird * bird);

#endif // BIRD_H
