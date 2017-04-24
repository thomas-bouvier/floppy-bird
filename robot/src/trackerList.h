/*!
* \file trackerList.h
* \brief File containing structures and prototypes of functions to manage a list of TrackedObject
*/

#ifndef TRACKER_LIST_H
#define TRACKER_LIST_H

#include "tracking.h"

/*!
* \struct NodeList 
* \brief A NodeList is an element of a List
*/
typedef struct NodeList {
	TrackedObject* trackedOject;	/*!<  the TrackedObject associated to the node */
	struct NodeList* next;			/*!<  the next node */
}NodeList;

/*!
* \struct List 
* \brief Rely all tracker specific informations 
*/
typedef struct{
	NodeList* first;		/*!<  the first node of the List */
	NodeList* current;		/*!<  the current node of the List */
	NodeList* last;			/*!<  the last node of the List */
}List; 



#endif /* TRACKER_LIST_H */
