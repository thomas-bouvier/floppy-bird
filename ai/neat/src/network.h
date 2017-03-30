/*!
* \file network.h
* \brief File containing structures and prototypes for the genome
*/
#ifndef NETWORK_H
#define NETWORK_H

#include <stdlib.h>
#include <stdio.h>

#include "conf.h"
#include "neuronList.h"
#include "connectionGeneList.h"

typedef struct ConnectionGene ConnectionGene;

typedef struct ConnectionGeneList ConnectionGeneList;

typedef enum NeuronType NeuronType;

typedef struct Neuron Neuron;

typedef struct NeuronList NeuronList;

/*!
* \struct ConnectionGene network.h
* \brief A ConnectionGene joins Neuron elements in the Network.
*/
struct ConnectionGene {
  struct ConnectionGene * next;     /*!< the address of the next ConnectionGene in the list */
  double weight;                    /*!< the weight of the connection gene */
  unsigned char enabled;            /*!< the enabled/disabled flag of the connection gene */
  Neuron * neuron_in;               /*!< the predecessor Neuron */
  Neuron * neuron_out;              /*!< the successor Neuron */
  int innovation;                   /*!< the innovation number of the Genome containing this ConnectionGene */
};

/*!
* \struct ConnectionGeneList network.h
* \brief A ConnectionGeneList is a conatiner for ConnectionGene elements.
*/
struct ConnectionGeneList {
  ConnectionGene * first;     /*<! the address of the first ConnectionGene of the list */
  ConnectionGene * current;   /*<! the address of the current ConnectionGene in the list */
  ConnectionGene * last;      /*<! the address of the last ConnectionGene of the list */
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
  ConnectionGeneList * connections;   /*!< the successors ConnectionGene linked to this Neuron */
  struct Neuron * next;               /*!< the address of the next Neuron in the list */
  short int id;                       /*!< the id of this Neuron */
  NeuronType type;                    /*!< the type of this Neuron */
  double value;                       /*!< the value attached to this Neuron */
};

/*!
* \struct NeuronList network.h
* \brief A NeuronList is a container for Neuron elements.
*/
struct NeuronList {
  Neuron * first;       /*<! the address of the first Neuron of the list */
  Neuron * current;     /*<! the address of the current Neuron in the list */
  Neuron * last;        /*<! the address of the last Neuron of the list */
};

/*!
* \brief The Network type is actually a NeuronList.
*/
typedef NeuronList Network;

Neuron * newNeuron(NeuronType type);
void freeNeuron(Neuron * neuron);

int addNeuronToNetwork(Network * network, Neuron * neuron);

ConnectionGene * newConnectionGene(double weight, unsigned char enabled, int innovation);
ConnectionGene * cloneConnectionGene(ConnectionGene * connection_gene);
void freeConnectionGene(ConnectionGene * connection_gene);

int addConnectionGeneToNeurons(Neuron * neuron_in, Neuron * neuron_out, ConnectionGene * connection_gene);

#endif // NETWORK_H
