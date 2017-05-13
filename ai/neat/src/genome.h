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

#include "neat_conf.h"
#include "network.h"
#include "neat_utils.h"

#define fast_sigmoid(x) ((x) / (1 + fabs(x)))

/*!
* \struct Genome genome.h
* \brief A Genome contains a Network element.
*/
typedef struct {
    GenericList * neurons;                  /*!< the GenericList of Neuron elements attached to this Genome */
    int nb_neurons;                         /*!< the number of Neuron elements contained in this Genome */
    GenericList * connection_genes;         /*!< the GenericList of ConnectionGene elements attached to this Genome */
    int nb_connection_genes;                /*!< the number of ConnectionGene elements contained in this Genome */
    double fitness;                         /*!< the fitness of the Genome */
    float mutation_rates[4];                /*!< the mutation rates attached to this Genome */
    int global_rank;                        /*!< the global rank of this Genome, including other Species */
    int * innovation;                       /*!< the address of the innovation number of the MatingPool containing this Genome */
    int nb_mutations;                       /*!< the number of mutations of this Genome */
    int mutations_history[N_MAX_MUTATIONS]; /*!< the mutations history of this Genome */
    int max_neurons;
} Genome;

Genome * newGenome(int * innovation);
void * cloneGenome(void * genome);
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

double sameSpecies(Genome * genome_1, Genome * genome_2);
int linked(Neuron * neuron_in, Neuron * neuron_out);

double * evaluateGenome(Genome * genome, double * input);

Neuron * getRandomNeuron(Genome * genome);
ConnectionGene * getRandomConnectionGene(Genome * genome);

int writeGraphVizGenome(Genome * genome, char * filename);
void printGenome(Genome * genome);

#endif // GENOME_H
