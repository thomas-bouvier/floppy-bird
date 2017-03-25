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
  Network * network;    /*!< the Network attached to this Genome */
} Genome;

int generateGenome(Genome * genome);

int mutatePoint(Genome * genome);
int mutateEnableFlag(Genome * genome, unsigned char enable);
int mutateNode(Genome * genome);

Neuron * getRandomNeuron(Genome * genome);

int writeGraphVizGenome(Genome * genome, char * filename);

#endif // GENOME_H
