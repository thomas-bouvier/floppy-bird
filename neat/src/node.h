/*!
* \file node.h
* \brief File containing structures and prototypes linked to a neuron
*/
#ifndef NODE_H
#define NODE_H

#include <stdlib.h>
#include <stdio.h>

#include "conf.h"

/*!
* \struct Neuron node.h
* \brief A neuron defines a node in the network. It can be an input, or an output.
*/
typedef struct {
  ConnectionGene connections[N_MAX_INPUTS];    /*!< connection genes attached to this neuron */
  short int n_connections;                     /*!< the number of connection genes linked to this neuron */
  double value;                                /*<! the value attached to this neuron */
} Neuron;

Neuron * newNeuron();

#endif // NODE_H
