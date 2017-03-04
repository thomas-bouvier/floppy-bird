/*!
* \file game.h
* \brief File containing structures and propotypes for the game
*/
#ifndef GAME_H
#define GAME_H

/*!
* \struct Bird game.h
*/
typedef struct{
    int x;              /*!< the abscissa coordonate */
    int y;              /*!< the ordonate coordonate */
    char sprite;        /*!< the path for the sprite */
    int gravity;        /*!< the speed of climb/fall of the bird */
} Bird;


#endif // GAME_H
