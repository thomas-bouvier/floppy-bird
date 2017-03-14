/*!
* \file list.c
* \brief File containing the functions to manage a list of obstacles
*/
#include "list.h"

List * newList(){
	List * new_list = (List*) malloc(sizeof(List));
	if(new_list == NULL){
		fprintf(stderr, "List allocation failure");
		return NULL;
	}
	new_list->first = new_list->current = new_list->last = NULL;
	return new_list;
}

int isEmpty(List * l){
	return (l->first == NULL);
}

int isFirst(List * l){
	return (l->current == l->first);
}

int isLast(List * l){
	return (l->current == l->last);
}

int outOfList(List * l){
	return (l->current == NULL);
}

void setOnFirst(List * l){
	l->current = l->first;
}

void setOnLast(List * l){
	l->current = l->last;
}

void next (List * l){
	if(!outOfList(l))
		l->current = l->current->next;
}

int deleteFirst(List * l){
	Obstacle * to_del = l->first;
	if(isEmpty(l))
		return 0;
	l->first = l->first->next;
	setOnFirst(l);
	if(l->first == NULL)
		l->last = NULL;
	freeObstacle(to_del);
	return 1;
}

int insertLast(List * l, Obstacle * obstacle){
	if(isEmpty(l))
		l->first = obstacle;
	else
		l->last->next = obstacle;
	l->last = obstacle;
	return 1;

}

