/*!
* \file network.h
* \brief File containing structures and prototypes for the genome
*/
#ifndef NETWORK_H
#define NETWORK_H

#include <stdlib.h>
#include <stdio.h>

#include "conf.h"

/*!
* \struct ConnectionGene network.h
* \brief A ConnectionGene joins Neuron elements in the Network.
*/
typedef struct ConnectionGene {
  struct ConnectionGene * next;     /*!< the address of the next ConnectionGene in the list */
  short int input;                  /*!< the input node attached to the connection gene */
  short int output;                 /*!< the output node attached to the connection gene */
  double weight;                    /*!< the weight of the connection gene */
  short int innovation;             /*!< the innovation number of the connection gene, used to track its history */
  unsigned char enabled;            /*!< the enabled/disabled flag of the connection gene */
  Neuron * neuron;                  /*!< the successor Neuron */
} ConnectionGene;

/*!
* \struct ConnectionGeneList network.h
*
*/
typedef struct {
  ConnectionGene * first;     /*<! the address of the first ConnectionGene of the list */
  ConnectionGene * current;   /*<! the address of the current ConnectionGene in the list */
  ConnectionGene * last;      /*<! the address of the last ConnectionGene of the list */
} ConnectionGeneList;

/*!
* \struct Neuron network.h
* \brief A Neuron defines a node in the Network. It can be an input, or an output.
*/
typedef struct Neuron {
  ConnectionGeneList connections;     /*!< the successors ConnectionGene linked to this Neuron */
  struct Neuron * next;               /*!< the address of the next Neuron in the list */
  short int id;                       /*!< the id of this Neuron */
  double value;                       /*!< the value attached to this Neuron */
} Neuron;

/*!
* \struct NeuronList network.h
*
*/
typedef struct {
  Neuron * first;       /*!< the address of the first Neuron of the list */
  Neuron * current;     /*<! the address of the current Neuron in the list */
  Neuron * last;        /*<! the address of the last Neuron of the list */
} NeuronList;

/*!
* \brief The Network type is actually a NeuronList.
*/
typedef NeuronList Network;

void initNetwork(Network * network);
void deleteNetwork(Network * network);

Neuron * newNeuron();
ConnectionGene * newConnectionGene(short int input, short int output, double weight, short int innovation, unsigned char enabled);

int writeGraphVizNetwork(Network * network, char * filename);

#endif // NETWORK_H
