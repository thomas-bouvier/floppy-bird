/*!
* \file list.c
* \brief File containing the functions to manage a list of obstacles
*/
#include "list.h"

/*!
* \brief Initialize a list, and fill it with a unique obstacle
* \param[out] l the list to initialize
*/
void initList(List * l, FILE * level){
    int  i;
    l->nbObstacles = 0;
	l->first = l->current = l->last = NULL;
	for (i=0 ; i<OBSTACLE_NUMBER ; ++i)
        createObstacleFromFile(level, i, l);
}

/*!
* \brief Test if a list is empty or not
* \param[in] l the list to test
* \return Return 1 if the list is empty, 0 if not
*/
int isEmpty(List * l){
	return (l->first == NULL);
}

/*!
* \brief Test if the current obstacle is the first
* \param[in] l the list to test
* \return Return 1 if the current obstacle is the first of the list, 0 if not
*/
int isFirst(List * l){
	return (l->current == l->first);
}

/*!
* \brief Test if the current obstacle is the last
* \param[in] l the list to test
* \return Return 1 if the current obstacle is the last of the list, 0 if not
*/
int isLast(List * l){
	return (l->current == l->last);
}

/*!
* \brief Test if the current node is not valid
* \param[in] l the list to test
* \return Return 1 if the current obstacle is not valid, 0 otherwise
*/
int outOfList(List * l){
	return (l->current == NULL);
}

/*!
* \brief Set the current obstacle on the first one
* \param[in] l the list to modify
*/
void setOnFirst(List * l){
	l->current = l->first;
}

/*!
* \brief Set the current obstacle on the last one
* \param[in] l the list to modify
*/
void setOnLast(List * l){
	l->current = l->last;
}

/*!
* \brief Set the current obstacle on the next one, if it exists
* \param[in] l the list to modify
*/
void next (List * l){
	if(!outOfList(l))
		l->current = l->current->next;
}

/*!
* \brief Delete the first obstacle of the list
* \param[out] l the list to modify
* \return Return 1 if the first obstacle has been deleted, 0 if not
*/
int deleteFirst(List * l){
	Obstacle * to_del = l->first;
	if(isEmpty(l))
		return 0;
	l->first = l->first->next;
	setOnFirst(l);
	if(l->first == NULL)
		l->last = NULL;
	freeObstacle(to_del);
	l->nbObstacles--;
	return 1;
}

/*!
* \brief Insert an obstacle at the end of the list
* \param[out] l the list to modify
* \param[out] obstacle the obstacle to insert
* \return Return 1 if the obstacle has been added, 0 if not
*/
int insertLast(List * l, struct Obstacle * obstacle){
	if(isEmpty(l))
		l->first = obstacle;
	else
		l->last->next = obstacle;
	l->last = obstacle;
	l->nbObstacles++;
	return 1;
}

