#include "connectionGeneList.h"

/*!
* \brief Free the specified ConnectionGene.
* \param[out] connection_gene the ConnectionGene to be freed
*/
static void freeConnectionGene(ConnectionGene * connection_gene) {
  free(connection_gene);
}

/*!
* \brief Initialize the ConnectionGene structure members to be consistent with an empty list.
* \param[out] connection_gene_list the ConnectionGeneList to be initialized
*/
void initConnectionGeneList(ConnectionGeneList * connection_gene_list) {
  connection_gene_list->first = NULL;
  connection_gene_list->current = NULL;
  connection_gene_list->last = NULL;
}

/*!
* \brief Suppress all ConnectionGene elements from the ConnectionGeneList.
* \param[out] connection_gene_list the ConnectionGeneList to be emptied
*/
void deleteConnectionGeneList(ConnectionGeneList * connection_gene_list) {
  ConnectionGene * connection_gene_to_delete = NULL;

  setOnFirstConnectionGene(connection_gene_list);
  while(!outOfConnectionGeneList(connection_gene_list)) {
    connection_gene_to_delete = connection_gene_list->current;
    nextConnectionGene(connection_gene_list);
    freeConnectionGene(connection_gene_to_delete);
  }

  connection_gene_list->first = connection_gene_list->current = connection_gene_list->last = NULL;
}

/*!
* \brief Check if it is an empty ConnectionGeneList.
* \param[in] connection_gene_list the ConnectionGeneList to check
* \return int 1 if empty, 0 otherwise
*/
int emptyConnectionGeneList(ConnectionGeneList * connection_gene_list) {
  return connection_gene_list->first == NULL;
}

/*!
* \brief Check if the current ConnectionGene is not valid (ie. NULL).
* \param[in] connection_gene_list the ConnectionGeneList to check
* \return int 1 if the current ConnectionGene is not valid, 0 otherwise
*/
int outOfConnectionGeneList(ConnectionGeneList * connection_gene_list) {
  return connection_gene_list->current == NULL;
}

/*!
* \brief Set the current ConnectionGene on the first one.
* \param[out] connection_gene_list the ConnectionGeneList to be modified
*/
void setOnFirstConnectionGene(ConnectionGeneList * connection_gene_list) {
  connection_gene_list->current = connection_gene_list->first;
}

/*!
* \brief Set the current ConnectionGene on the next one.
* \param[out] connection_gene_list the ConnectionGeneList to be modified
*/
void nextConnectionGene(ConnectionGeneList * connection_gene_list) {
  if (!outOfConnectionGeneList(connection_gene_list))
    connection_gene_list->current = connection_gene_list->current->next;
}

/*!
* \brief Retrieve the ConnectionGene stored in the current element.
* \param[in] connection_gene_list the ConnectionGeneList
* \return ConnectionGene the ConnectionGene of the current element if valid, NULL otherwise
*/
ConnectionGene * getCurrentConnectionGene(ConnectionGeneList * connection_gene_list) {
  if (!outOfConnectionGeneList(connection_gene_list))
    return connection_gene_list->current;

  return NULL;
}

/*!
* \brief Insert a ConnectionGene at the tail of the ConnectionGeneList.
* \param[out] connection_gene_list the ConnectionGeneList
* \param[in] connection_gene the ConnectionGene to be inserted
* \return int 1 if the ConnectionGene was successfully inserted, 0 otherwise
*/
int addConnectionGene(ConnectionGeneList * connection_gene_list, ConnectionGene * connection_gene) {
  if (emptyConnectionGeneList(connection_gene_list))
    connection_gene_list->first = connection_gene;
  else
    connection_gene_list->last->next = connection_gene;

  connection_gene_list->last = connection_gene;

  return 1;
}

static int deleteFirstConnectionGene(ConnectionGeneList * connection_gene_list) {
  ConnectionGene * connection_gene_to_delete = connection_gene_list->first;

  if (!emptyConnectionGeneList(connection_gene_list)) {
    connection_gene_list->first = connection_gene_list->first->next;
    setOnFirstConnectionGene(connection_gene_list);

    if (connection_gene_list->first == NULL)
      connection_gene_list->last = NULL;

    freeConnectionGene(connection_gene_to_delete);
    return 1;
  }

  return 0;
}

static int deleteLastConnectionGene(ConnectionGeneList * connection_gene_list) {
  ConnectionGene * connection_gene_to_delete = connection_gene_list->last;
  ConnectionGene * previous_connection_gene = NULL;

  if (!emptyConnectionGeneList(connection_gene_list)) {
    setOnFirstConnectionGene(connection_gene_list);
    if (connection_gene_list->current == connection_gene_to_delete)
      connection_gene_list->first = NULL;

    else {
      while (connection_gene_list->current != connection_gene_list->last) {
        previous_connection_gene = connection_gene_list->current;
        nextConnectionGene(connection_gene_list);
      }

      previous_connection_gene->next = NULL;
    }

    connection_gene_list->current = connection_gene_list->last = previous_connection_gene;

    if (connection_gene_list->last == NULL)
      connection_gene_list->first = NULL;

    freeConnectionGene(connection_gene_to_delete);
    return 1;
  }

  return 0;
}

static int deleteCurrentConnectionGene(ConnectionGeneList * connection_gene_list) {
  ConnectionGene * previous_connection_gene = NULL;
  ConnectionGene * stop = NULL;

  if (!outOfConnectionGeneList(connection_gene_list)) {
    if (connection_gene_list->current == connection_gene_list->first)
      deleteFirstConnectionGene(connection_gene_list);

    else if (connection_gene_list->current == connection_gene_list->last)
      deleteLastConnectionGene(connection_gene_list);

    else {
      stop = connection_gene_list->current;

      setOnFirstConnectionGene(connection_gene_list);
      while (connection_gene_list->current != stop) {
        previous_connection_gene = connection_gene_list->current;
        nextConnectionGene(connection_gene_list);
      }

      previous_connection_gene->next = connection_gene_list->current->next;
      freeConnectionGene(connection_gene_list->current);
    }
  }
}

/*!
* \brief Delete the given ConnectionGene from the ConnectionGeneList.
* \param[out] connection_gene_list the ConnectionGeneList
* \param[out] connection_gene the ConnectionGene to delete
* \return int 1 if the ConnectionGene was successfully deleted, 0 otherwise
*/
int deleteConnectionGene(ConnectionGeneList * connection_gene_list, ConnectionGene * connection_gene) {
  setOnFirstConnectionGene(connection_gene_list);
  while (!outOfConnectionGeneList(connection_gene_list)) {
    if (connection_gene_list->current == connection_gene) {
      deleteCurrentConnectionGene(connection_gene_list);
      return 1;
    }

    nextConnectionGene(connection_gene_list);
  }

  return 0;
}

/*!
* \brief Check if the given ConnectionGene is stored in the ConnectionGeneList.
* \param[in] connection_gene_list the ConnectionGeneList
* \param[in] connection_gene the ConnectionGene to be found
* \return int 1 if the ConnectionGene was found, 0 otherwise
*/
int findConnectionGene(ConnectionGeneList * connection_gene_list, ConnectionGene * connection_gene) {
  setOnFirstConnectionGene(connection_gene_list);
  while (!outOfConnectionGeneList(connection_gene_list)) {
    if (connection_gene_list->current == connection_gene)
      return 1;

    nextConnectionGene(connection_gene_list);
  }

  return 0;
}
