/*!
* \file network.h
* \brief File containing structures and prototypes for the genome
*/
#ifndef NETWORK_H
#define NETWORK_H

#include <stdlib.h>
#include <stdio.h>

#include "neat_conf.h"
#include "generic_list.h"

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
    int neuron_in_id;                 /*!< the id of the predecessor Neuron */
    int neuron_out_id;                /*!< the id of the successor Neuron */
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
    BASIC
};

/*!
* \struct Neuron network.h
* \brief A Neuron defines a node in the Network. It can be an input, or an output.
*/
struct Neuron {
    GenericList * connection_genes_input;       /*!< the successors ConnectionGene linked to this Neuron */
    int id;                                     /*!< the id of this Neuron */
    NeuronType type;                            /*!< the type of this Neuron */
    double value;                               /*!< the value attached to this Neuron */
};

Neuron * newNeuron(NeuronType type);
void freeNeuron(void * neuron);
int addNeuron(GenericList * neurons, Neuron * neuron);

ConnectionGene * newConnectionGene(double weight, unsigned char enabled, int innovation);
void * cloneConnectionGene(void * connection_gene);
void freeConnectionGene(void * connection_gene);

#endif // NETWORK_H
