/*!
* \file genome.h
* \brief File containing structures and prototypes for the genome
*/
#ifndef GENOME_H
#define GENOME_H

#include <stdlib.h>
#include <stdio.h>

#include "conf.h"
#include "utils.h"

/*!
* \struct ConnectionGene genome.h
* \brief A connection gene joins nodes in the genome
*/
typedef struct {
  short int input;          /*!< the input node attached to the connection gene */
  short int output;         /*!< the output node attached to the connection gene */
  double weight;            /*!< the weight of the connection gene */
  short int innovation;     /*!< the innovation number of the connection gene, used to track its history */
  unsigned char enabled;    /*!< the enabled/disabled flag of the connection gene */
} ConnectionGene;

/*!
* \struct Neuron node.h
* \brief A neuron defines a node in the network. It can be an input, or an output.
*/
typedef struct {
  ConnectionGene connections[N_MAX_INPUTS];    /*!< connection genes attached to this neuron */
  short int nb_connections;                    /*!< the number of connection genes linked to this neuron */
  double value;                                /*<! the value attached to this neuron */
} Neuron;

/*!
* \struct NetworkElement genome.h
* \brief A network element links an unique id to a Neuron.
*/
typedef struct {
  short int id;         /*!< the unique id for this network element */
  Neuron * neuron;      /*!< the neuron for this network element */;
} NetworkElement;

/*!
* \struct Genome genome.h
* \brief A genome contains connection genes
*/
typedef struct {
  ConnectionGene genes[N_MAX_CONNECTION_GENES];   /*!< the connection genes contained in this genome */
  NetworkElement network[N_MAX_NETWORK];          /*!< the network elements (id + neuron) contained in this genome */
  short int nb_genes;                             /*!< the number of genes in this genome */
  short int nb_network_elements;                  /*!< the number of network elements in this genome */
} Genome;

ConnectionGene * newConnectionGene(short int input, short int output, double weight, short int innovation, unsigned char enabled);
Neuron * newNeuron();
NetworkElement * newNetworkElement(short int id, Neuron * neuron);
Genome * newGenome();

int generateGenome(Genome * genome);
int addToNetwork(Genome * genome, NetworkElement * network_element);

int genomeHasNetworkElement(Genome * genome, short int id);

Neuron * getRandomNeuron(Genome * genome);

#endif // GENOME_H
