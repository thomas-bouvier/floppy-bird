/*!
* \file genome.h
* \brief File containing structures and prototypes for the genome
*/
#ifndef GENOME_H
#define GENOME_H

#include <stdlib.h>
#include <stdio.h>

#include "conf.h"

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
* \struct Genome genome.h
* \brief A genome contains connection genes
*/
typedef struct {
  ConnectionGene genes[N_MAX_CONNECTION_GENES];   /*!< genes contained in this genome */
  short int nb_genes;                             /*!< the number of genes in this genome */
} Genome;

ConnectionGene * newConnectionGene(short int input, short int output, double weight, short int innovation, unsigned char enabled);

Genome * newGenome();

#endif // GENOME_H
