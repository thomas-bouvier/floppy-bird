#include "generic_list.h"

/*!
* \brief Create a Node.
* \return Return a new Node, NULL if error
*/
Node * newNode() {
  Node * new_node = NULL;

  if ((new_node = malloc(sizeof(Node))) == (Node *) NULL) {
    fprintf(stderr, "Error while allocating memory for new Node\n");
    return NULL;
  }

  new_node->data = NULL;
  new_node->next = NULL;

  return new_node;
}

/*!
* \brief Create a GenericList.
* \return Return a new GenericList, NULL if error
*/
GenericList * newGenericList(CloneFunction clone_function, FreeFunction free_function) {
  GenericList * new_list = NULL;

  if (free_function == NULL) {
    fprintf(stderr, "Free function can't be NULL for new GenericList\n");
    return NULL;
  }

  if ((new_list = (GenericList *) malloc(sizeof(GenericList))) == (GenericList *) NULL) {
    fprintf(stderr, "Error while allocating memory for new GenericList\n");
    return NULL;
  }

  new_list->clone_function = clone_function;
  new_list->free_function = free_function;

  return new_list;
}

/*!
* \brief Initialize the GenericList structure members to be consistent with an empty list.
* \param[out] list the GenericList to be initialized
*/
void initGenericList(GenericList * list) {
    list->first = NULL;
    list->current = NULL;
    list->last = NULL;
}

/*!
* \brief Clone the given GenericList.
* \param[out] list the GenericList to clone
* \return Return a new GenericList, NULL if error
*/
GenericList * cloneGenericList(GenericList * list) {
    GenericList * new_list = newGenericList(list->clone_function, list->free_function);

    initGenericList(new_list);

    setOnFirstElement(list);
    while(!outOfGenericList(list)) {
        add(new_list, new_list->clone_function(getCurrent(list)));

        nextElement(list);
    }

    new_list->current = new_list->first;

    return new_list;
}

/*!
* \brief Suppress all elements from the GenericList.
* \param[out] list the GenericList to be emptied
*/
void freeGenericList(GenericList * list) {
  Node * element_to_delete = NULL;

  setOnFirstElement(list);
  while(!outOfGenericList(list)) {
    element_to_delete = list->current;
    nextElement(list);

    list->free_function(element_to_delete->data);

    free(element_to_delete);
  }

  free(list);
}

/*!
* \brief Check if the GenericList is empty.
* \param[in] list the GenericList to check
* \return int 1 if the GenericList is empty, 0 otherwise
*/
int emptyGenericList(GenericList * list) {
  return list->first == NULL;
}

/*!
* \brief Check if the current element is not valid (ie. NULL).
* \param[in] list the GenericList to check
* \return int 1 if the current element is not valid, 0 otherwise
*/
int outOfGenericList(GenericList * list) {
  return list->current == NULL;
}

/*!
* \brief Set the current element on the first one of the GenericList.
* \param[out] list the GenericList to be modified
*/
void setOnFirstElement(GenericList * list) {
  list->current = list->first;
}

/*!
* \brief Set the current element on the given index of the GenericList.
* \param[out] list the GenericList to be modified
* \param[in] index the index to set the current element on
*/
void setOn(GenericList * list, int index) {
  int i = 0;

  setOnFirstElement(list);
  while (!outOfGenericList(list) && i != index) {
    nextElement(list);
    ++i;
  }
}

/*!
* \brief Set the current element on the next one, if the current element is not NULL.
* \param[out] list the GenericList to be modified
*/
void nextElement(GenericList * list) {
  if (!outOfGenericList(list))
    list->current = list->current->next;
}

/*!
* \brief Retrieve the element pointed by current pointer in the current element.
* \param[in] list the GenericList
* \return the element of the current element if valid, NULL otherwise
*/
void * getCurrent(GenericList * list) {
  if (!outOfGenericList(list))
    return list->current->data;

  return NULL;
}

/*!
* \brief Insert an element at the tail of the GenericList. The current pointer is updated to point to it.
* \param[out] list the GenericList
* \param[out] element the element to be inserted
* \return int 1 if the element was successfully inserted, 0 otherwise
*/
int add(GenericList * list, void * element) {
    Node * new_node = NULL;

    if (!element)
        return 0;

    if ((new_node = newNode(list->clone_function)) == (Node *) NULL) {
        fprintf(stderr, "Error while allocating memory for new Node\n");
        return 0;
    }

    new_node->data = element;
    new_node->next = NULL;

    if (emptyGenericList(list))
        list->first = new_node;
    else
        list->last->next = new_node;

    list->current = list->last = new_node;

    return 1;
}

/*!
* \brief Insert an element before the current position of the GenericList. The current pointer is updated to point to it.
* \param[out] list the GenericList
* \param[out] element the element to be inserted before the current element
* \return int 1 if the element was successfully inserted, 0 otherwise
*/
static int insertBeforeCurrent(GenericList * list, void * element) {
    Node * new_node = NULL;
    Node * previous_node = NULL;
    Node * stop = list->current;

    if (!outOfGenericList(list)) {
        setOnFirstElement(list);
        while (list->current != stop) {
            previous_node = list->current;
            nextElement(list);
        }

        if ((new_node = newNode()) == (Node *) NULL) {
            fprintf(stderr, "Error while allocating memory for new Node\n");
            return 0;
        }

        new_node->data = element;

        if (list->current == list->first) {
            new_node->next = list->first;
            list->first = new_node;
        } else {
            new_node->next = list->current;
            previous_node->next = new_node;
        }
    }
    else {
        add(list, element);
    }

    return 1;
}

static int deleteFirst(GenericList * list, int retrieve_data, void ** data) {
    Node * element_to_delete = list->first;

    list->first = list->first->next;
    setOnFirstElement(list);

    if (list->first == NULL)
      list->last = NULL;

    if (retrieve_data)
      *data = element_to_delete->data;
    else if (list->free_function)
      list->free_function(element_to_delete->data);

    free(element_to_delete);

    return 1;
}

static int deleteLast(GenericList * list, int retrieve_data, void ** data) {
    Node * element_to_delete = list->last;
    Node * previous_element = NULL;

    setOnFirstElement(list);
    while (list->current != list->last) {
        previous_element = list->current;
        nextElement(list);
    }

    previous_element->next = NULL;
    list->current = list->last = previous_element;

    if (retrieve_data)
      *data = element_to_delete->data;
    else if (list->free_function)
      list->free_function(element_to_delete->data);

    free(element_to_delete);

    return 1;
}

static int deleteCurrent(GenericList * list, int retrieve_data, void ** data) {
    Node * previous_element = NULL;
    Node * stop = NULL;

    if (list->current == list->first)
        return deleteFirst(list, retrieve_data, data);

    else if (list->current == list->last)
        return deleteLast(list, retrieve_data, data);

    else {
        stop = list->current;

        setOnFirstElement(list);
        while (list->current != stop) {
            previous_element = list->current;
            nextElement(list);
        }

        previous_element->next = list->current->next;

        if (retrieve_data)
            *data = list->current->data;
        else if (list->free_function)
            list->free_function(list->current->data);

        free(list->current);

        return 1;
    }
}

/*!
* \brief Delete the given element from the GenericList.
* \param[out] list the GenericList
* \param[out] element the element to delete
* \return int 1 if the element was successfully deleted, 0 otherwise
*/
int delete(GenericList * list, void * element) {
    setOnFirstElement(list);
    while (!outOfGenericList(list)) {
        if (list->current->data == element) {
            deleteCurrent(list, 0, NULL);
            return 1;
        }

        nextElement(list);
    }

    return 0;
}

/*!
* \brief Clear the entire given GenericList.
* \param[out] list the GenericList to clear
* \return int 1 if the GenericList was successfully cleared, 0 otherwise
*/
int clearGenericList(GenericList * list) {
    setOnFirstElement(list);
    while (!outOfGenericList(list)) {
        if (!delete(list, getCurrent(list))) {
            fprintf(stderr, "Error while clearing GenericList\n");
            return 0;
        }

        setOnFirstElement(list);
    }

    return 1;
}

/*!
* \brief Sort the given GenericList using the provided function.
* \param[out] list the GenericList to sort
* \param[in] f the function to use to sort the elements
*/
void sort(GenericList * list, int (*f) (const void *, const void *)) {
    Node * pos = NULL;
    void ** save;

    int c = count(list);

    if (count(list) > 1) {
        save = malloc(sizeof(void *));

        setOnFirstElement(list);
        nextElement(list);
        while (!outOfGenericList(list)) {
            pos = list->current->next;
            deleteCurrent(list, 1, save);

            setOnFirstElement(list);
            while (list->current != pos && (*f)(*save, list->current->data) < 0)
                nextElement(list);

            if (!insertBeforeCurrent(list, *save))
                fprintf(stderr, "Element coudn't be inserted in GenericList\n");

            list->current = pos;
        }

        free(save);
    }

    if (c != count(list))
        printf("====================================================\n");
}

/*!
* \brief Check if the given element is stored in the GenericList.
* \param[in] list the GenericList
* \param[in] element the element to be found
* \return int 1 if the element was found, 0 otherwise
*/
int find(GenericList * list, void * element) {
  setOnFirstElement(list);
  while (!outOfGenericList(list)) {
    if (list->current->data == element)
      return 1;

    nextElement(list);
  }

  return 0;
}

/*!
* \brief Count the number of elements in the given GenericList.
* \param[in] list the GenericList to scan
*/
int count(GenericList * list) {
    Node * pos = list->current;
    int count = 0;

    setOnFirstElement(list);
    while (!outOfGenericList(list)) {
        ++count;
        nextElement(list);
    }

    list->current = pos;

    return count;
}

/*!
* \brief Print the given GenericList.
* \param[out] list the GenericList to print
*/
void printGenericList(GenericList * list) {
    int index = 0;

    printf("list address: %p\n", list);
    printf("%d elements\n\n", count(list));

    setOnFirstElement(list);
    while (!outOfGenericList(list)) {
        printf("%d.\n", index);
        ++index;

        printf("node address: %p\n", list->current);
        printf("next node address: %p\n", list->current->next);
        printf("data address: %p\n", list->current->data);

        if (list->current->next)
            printf("next data address: %p\n", list->current->next->data);

        printf("\n");

        nextElement(list);
    }
}
