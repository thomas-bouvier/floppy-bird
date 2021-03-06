/*!
* \file bird.h
* \brief File containing structures and prototypes for the bird
*/
#ifndef BIRD_H
#define BIRD_H

#include "../../ai/neat/src/population.h"
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
    int dead;                   /*!< the state of the bird (1 for dead, 0 for alive) */
    int flaps;                  /*!< the number of flaps done by this Bird */
    int score;                  /*!< the number of obstacles this Bird has passed */
    Genome * genome;            /*!< the Genome element attached to this Bird */
    Species * species;          /*!< the Species of the Genome attached to this Bird */
} Bird;

Bird * initBird(Genome * genome, Species * species);
void updateBird(Bird * bird, int t, Sound * sound);
void freeBird(Bird * bird);

#endif // BIRD_H
