/*!
* \file list.h
* \brief File with the structure and prototypes of a list of obstacles
*/

#ifndef LIST_H
#define LIST_H

#include "constants.h"
#include "obstacle.h"

/*!
* \struct List list.h
* \brief The list of obstacle displayed on screen
*/
typedef struct{
    Obstacle * first;           /*!< the address of the first obstacle in the list*/
    Obstacle * current;         /*!< the address of the first obstacle in the list*/
    Obstacle * last;            /*!< the address of the first obstacle in the list*/
} List;

void initList(List * l);
int isEmpty(List * l);
int isFirst(List * l);
int isLast(List * l);
int outOfList(List * l);
void setOnFirst(List * l);
void setOnLast(List * l);
void next (List * l);
int deleteFirst(List * l);
int insertLast(List * l, Obstacle * obstacle);

#endif // LIST_H
