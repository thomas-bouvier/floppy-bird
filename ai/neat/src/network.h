/*!
* \file network.h
* \brief File containing structures and prototypes for the genome
*/
#ifndef NETWORK_H
#define NETWORK_H

#include <stdlib.h>
#include <stdio.h>

#include "conf.h"
#include "list.h"

typedef struct ConnectionGene ConnectionGene;

typedef enum NeuronType NeuronType;

typedef struct Neuron Neuron;

/*!
* \struct ConnectionGene network.h
* \brief A ConnectionGene joins Neuron elements in the Network.
*/
struct ConnectionGene {
  double weight;                    /*!< the weight of the connection gene */
  unsigned char enabled;            /*!< the enabled/disabled flag of the connection gene */
  Neuron * neuron_in;               /*!< the predecessor Neuron */
  Neuron * neuron_out;              /*!< the successor Neuron */
  int innovation;                   /*!< the innovation number of the Genome containing this ConnectionGene */
};

/*!
* \enum NeuronType network.h
* \brief A NeuronType refers to the type of the Neuron.
*/
enum NeuronType {
  UNKNOW,
  INPUT,
  OUTPUT,
  BIAS,
  BASIC
};

/*!
* \struct Neuron network.h
* \brief A Neuron defines a node in the Network. It can be an input, or an output.
*/
struct Neuron {
  List * connections;                 /*!< the successors ConnectionGene linked to this Neuron */
  short int id;                       /*!< the id of this Neuron */
  NeuronType type;                    /*!< the type of this Neuron */
  double value;                       /*!< the value attached to this Neuron */
};

/*!
* \brief The Network type is actually a neuron List.
*/
typedef List Network;

Neuron * newNeuron(NeuronType type);
void * cloneNeuron(void * neuron);
Neuron * cloneNeuronWithoutConnections(Neuron * neuron);
void freeNeuron(void * neuron);

int addNeuronToNetwork(Network * network, Neuron * neuron);

ConnectionGene * newConnectionGene(double weight, unsigned char enabled, int innovation);
void * cloneConnectionGene(void * connection_gene);
void freeConnectionGene(void * connection_gene);

int addConnectionGeneToNeurons(Neuron * neuron_in, Neuron * neuron_out, ConnectionGene * connection_gene);

#endif // NETWORK_H
