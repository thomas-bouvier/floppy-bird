/*!
* \file genome.h
* \brief File containing structures and prototypes for the Genome.
*/
#ifndef GENOME_H
#define GENOME_H

#include <stdlib.h>
#include <stdio.h>

#include "network.h"
#include "neuronList.h"
#include "connectionGeneList.h"
#include "utils.h"

/*!
* \struct Genome genome.h
* \brief A Genome contains a Network element.
*/
typedef struct {
  Network * network;          /*!< the Network attached to this Genome */
  float mutation_rates[4];    /*!< the mutation rates attached to this Genome */
  int * innovation;           /*!< the address of the innovation number of the MatingPool containing this Genome */
} Genome;

int generateGenome(Genome * genome);

int mutate(Genome * genome);
int mutatePoint(Genome * genome);
int mutateLink(Genome * genome);
int mutateNode(Genome * genome);
int mutateEnableFlag(Genome * genome, unsigned char enable);

int linked(Neuron * neurion_in, Neuron * neuron_out);

Neuron * getRandomNeuron(Genome * genome);

int writeGraphVizGenome(Genome * genome, char * filename);

#endif // GENOME_H
