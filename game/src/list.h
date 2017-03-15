/*!
* \file list.h
* \brief File with the structure and prototypes of a list of obstacles
*/
#ifndef LIST_H
#define LIST_H

#include "constants.h"
#include "obstacle.h"

typedef struct List List;

/*!
* \struct List list.h
* \brief The list of obstacle displayed on screen
*/
struct List{
    struct Obstacle * first;           /*!< the address of the first obstacle in the list*/
    struct Obstacle * current;         /*!< the address of the current obstacle in the list*/
    struct Obstacle * last;            /*!< the address of the last obstacle in the list*/
};

void initList(List * l);
int isEmpty(List * l);
int isFirst(List * l);
int isLast(List * l);
int outOfList(List * l);
void setOnFirst(List * l);
void setOnLast(List * l);
void next (List * l);
int deleteFirst(List * l);
int insertLast(List * l, struct Obstacle * obstacle);

#endif // LIST_H
