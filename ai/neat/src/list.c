#include "list.h"

/*!
* \brief Create a List.
* \return Return a new List, NULL if error
*/
List * newList(FreeFunction free_function) {
  List * new_list = (List *) malloc(sizeof(List));

  if (new_list == (List *) NULL) {
    fprintf(stderr, "Error while allocating memory for new List\n");
    return NULL;
  }

  new_list->free_function = free_function;

  return new_list;
}

/*!
* \brief Initialize the List structure members to be consistent with an empty list.
* \param[out] list the List to be initialized
*/
void initList(List * list) {
  list->first = NULL;
  list->current = NULL;
  list->last = NULL;
}

/*!
* \brief Suppress all elements from the List.
* \param[out] list the List to be emptied
*/
void freeList(List * list) {
  Node * element_to_delete = NULL;

  setOnFirst(list);
  while(!outOfList(list)) {
    element_to_delete = list->current;
    next(list);

    if (list->free_function)
      list->free_function(element_to_delete->data);

    free(element_to_delete);
  }

  free(list);
}

/*!
* \brief Check if the List is empty.
* \param[in] list the List to check
* \return int 1 if the List is empty, 0 otherwise
*/
int emptyList(List * list) {
  return list->first == NULL;
}

/*!
* \brief Check if the current element is not valid (ie. NULL).
* \param[in] list the List to check
* \return int 1 if the current element is not valid, 0 otherwise
*/
int outOfList(List * list) {
  return list->current == NULL;
}

/*!
* \brief Set the current element on the first one of the List.
* \param[out] list the List to be modified
*/
void setOnFirst(List * list) {
  list->current = list->first;
}

/*!
* \brief Set the current element on the given index of the List.
* \param[out] list the List to be modified
* \param[in] index the index to set the current element on
*/
void setOn(List * list, int index) {
  int i = 0;

  setOnFirst(list);
  while (!outOfList(list) && i != index) {
    next(list);
    ++i;
  }
}

/*!
* \brief Set the current element on the next one, if the current element is not NULL.
* \param[out] list the List to be modified
*/
void next(List * list) {
  if (!outOfList(list))
    list->current = list->current->next;
}

/*!
* \brief Retrieve the element pointed by current pointer in the current element.
* \param[in] list the List
* \return the element of the current element if valid, NULL otherwise
*/
void * getCurrent(List * list) {
  if (!outOfList(list))
    return list->current->data;

  return NULL;
}

/*!
* \brief Insert an element at the tail of the List. The current pointer is updated to point to it.
* \param[out] list the List
* \param[out] element the element to be inserted
* \return int 1 if the element was successfully inserted, 0 otherwise
*/
int add(List * list, void * element) {
  Node * new_node = NULL;

  if (!element)
    return 0;

  if ((new_node = malloc(sizeof(Node))) == (Node *) NULL) {
    fprintf(stderr, "Error while allocating memory for new Node\n");
    return 0;
  }

  new_node->data = element;

  if (emptyList(list))
    list->first = new_node;
  else
    list->last->next = new_node;

  list->current = list->last = new_node;
  new_node->next = NULL;

  return 1;
}

static int deleteFirst(List * list) {
  Node * element_to_delete = list->first;

  if (!emptyList(list)) {
    list->first = list->first->next;
    setOnFirst(list);

    if (list->first == NULL)
      list->last = NULL;

    if (list->free_function)
      list->free_function(element_to_delete->data);

    free(element_to_delete);

    return 1;
  }

  return 0;
}

static int deleteLast(List * list) {
  Node * element_to_delete = list->last;
  Node * previous_element = NULL;

  if (!emptyList(list)) {
    setOnFirst(list);
    if (list->current == element_to_delete)
      list->first = NULL;

    else {
      while (list->current != list->last) {
        previous_element = list->current;
        next(list);
      }

      previous_element->next = NULL;
    }

    list->current = list->last = previous_element;

    if (list->last == NULL)
      list->first = NULL;

    if (list->free_function)
      list->free_function(element_to_delete->data);

    free(element_to_delete);

    return 1;
  }

  return 0;
}

static int deleteCurrent(List * list) {
  Node * previous_element = NULL;
  Node * stop = NULL;

  if (!outOfList(list)) {
    if (list->current == list->first)
      return deleteFirst(list);

    else if (list->current == list->last)
      return deleteLast(list);

    else {
      stop = list->current;

      setOnFirst(list);
      while (list->current != stop) {
        previous_element = list->current;
        next(list);
      }

      previous_element->next = list->current->next;

      if (list->free_function)
        list->free_function(list->current->data);

      free(list->current);

      return 1;
    }
  }

  return 0;
}

/*!
* \brief Delete the given element from the List.
* \param[out] list the List
* \param[out] element the element to delete
* \return int 1 if the element was successfully deleted, 0 otherwise
*/
int delete(List * list, void * element) {
  setOnFirst(list);
  while (!outOfList(list)) {
    if (list->current->data == element) {
      deleteCurrent(list);
      return 1;
    }

    next(list);
  }

  return 0;
}

/*!
* \brief Check if the given element is stored in the List.
* \param[in] list the List
* \param[in] element the element to be found
* \return int 1 if the element was found, 0 otherwise
*/
int find(List * list, void * element) {
  setOnFirst(list);
  while (!outOfList(list)) {
    if (list->current->data == element)
      return 1;

    next(list);
  }

  return 0;
}

/*!
* \brief Count the number of elements in the given List.
* \param[in] list the List to scan
*/
int count(List * list) {
  int count = 0;

  setOnFirst(list);
  while (!outOfList(list)) {
    ++count;
    next(list);
  }

  return count;
}
