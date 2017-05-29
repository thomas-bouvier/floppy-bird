/*!
* \file generic_list.h
* \brief File containing structures and prototypes for a generic list
*/
#ifndef GENERIC_LIST_H
#define GENERIC_LIST_H

#include <stdio.h>
#include <stdlib.h>

typedef void * (* CloneFunction) (void *);
typedef void (* FreeFunction) (void *);

typedef struct GenericList GenericList;

typedef struct Node {
  void * data;
  struct Node * next;
} Node;

/*!
* \struct GenericList network.h
* \brief A GenericList is the implementation of a linked GenericList structure.
*/
struct GenericList {
    Node * first;                   /*<! the address of the first element of the GenericList */
    Node * current;                 /*<! the address of the current element of the GenericList */
    Node * last;                    /*<! the address of the last element of the GenericList */
    CloneFunction clone_function;   /*<! the clone function pointer of the stored element */
    FreeFunction free_function;	    /*<! the free function pointer of the stored element */
};

Node * newNode();

GenericList * newGenericList(CloneFunction clone_function, FreeFunction free_function);
void initGenericList(GenericList * list);
void freeGenericList(GenericList * list, int apply_free_function);

int emptyGenericList(GenericList * list);
int outOfGenericList(GenericList * list);

void setOnFirstElement(GenericList * list);
void setOn(GenericList * list, int index);
void nextElement(GenericList * list);

void * getCurrent(GenericList * list);

int add(GenericList * list, void * element);
int delete(GenericList * list, void * element);
int clearGenericList(GenericList * list);

void sort(GenericList * list, int (*f) (const void *, const void *));

int find(GenericList * list, void * element);
int count(GenericList * list);

void printGenericList(GenericList * list);

#endif // GENERIC_LIST_H
