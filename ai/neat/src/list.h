/*!
* \file list.h
* \brief File containing structures and prototypes for a generic list
*/
#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>

typedef void (* FreeFunction) (void *);

typedef struct List List;

typedef struct Node {
  void * data;
  struct Node * next;
} Node;

/*!
* \struct List network.h
* \brief A List is the implementation of a linked list structure.
*/
struct List {
  Node * first;                   /*<! the address of the first element of the List */
  Node * current;                 /*<! the address of the current element of the List */
  Node * last;                    /*<! the address of the last element of the List */
  FreeFunction free_function;
};

Node * newNode();

List * newList(FreeFunction free_function);
void initList(List * list);
void freeList(List * list);

int emptyList(List * list);
int outOfList(List * list);

void setOnFirst(List * list);
void setOn(List * list, int index);
void next(List * list);

void * getCurrent(List * list);

int add(List * list, void * element);
int delete(List * list, void * element);

void sort(List * list, int (*f) (const void *, const void *));

int find(List * list, void * element);
int count(List * list);

#endif // LIST_H
