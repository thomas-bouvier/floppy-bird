/*!
* \file genome.h
* \brief File containing structures and prototypes for the Genome.
*/
#ifndef GENOME_H
#define GENOME_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>

#include "network.h"
#include "utils.h"

/*!
* \struct Genome genome.h
* \brief A Genome contains a Network element.
*/
typedef struct {
  Network * network;                        /*!< the Network attached to this Genome */
  int nb_neurons;                           /*!< te number of Neuron elements contained in this Genome */
  int nb_connection_genes;                  /*!< the number of ConnectionGene elements contained in this Genome */
  float fitness;                            /*!< the fitness of the Genome */
  float mutation_rates[4];                  /*!< the mutation rates attached to this Genome */
  int global_rank;                          /*!< the global rank of this Genome, including other Species */
  int * innovation;                         /*!< the address of the innovation number of the MatingPool containing this Genome */
  int nb_mutations;                         /*!< the number of mutations of this Genome */
  int mutations_history[N_MAX_MUTATIONS];   /*!< the mutations history of this Genome */
} Genome;

Genome * newGenome(int * innovation);
void freeGenome(void * genome);

int generateGenome(Genome * genome);
int addNeuronToGenome(Genome * genome, Neuron * neuron);
int addConnectionGeneToGenome(Genome * genome, Neuron * neuron_1, Neuron * neuron_2, ConnectionGene * connection_gene);

int mutate(Genome * genome);
int mutatePoint(Genome * genome);
int mutateLink(Genome * genome);
int mutateNode(Genome * genome);
int mutateEnableFlag(Genome * genome, unsigned char enable);

Genome * crossover(Genome * genome_1, Genome * genome_2);

int sameSpecies(Genome * genome_1, Genome * genome_2);
int linked(Neuron * neurion_in, Neuron * neuron_out);

Neuron * getRandomNeuron(Genome * genome);

int writeGraphVizGenome(Genome * genome, char * filename);

#endif // GENOME_H
