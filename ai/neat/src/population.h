/*!
* \file population.h
* \brief File containing structures and prototypes linked to the population
*/
#ifndef POPULATION_H
#define POPULATION_H

#include <stdlib.h>
#include <stdio.h>

#include "conf.h"
#include "genome.h"

typedef struct Species Species;

typedef struct MatingPool MatingPool;

/*!
* \struct Species population.h
* \brief A Species contains Genome elements which have similar characteristics.
*/
struct Species {
  Genome genomes[N_MAX_GENOMES];    /*!< Genomes elements contained in this Species */
  short int id;                     /*!< the id of this Species */
  short int nb_genomes;             /*!< the number of Genome elements contained in this Species */
  double max_fitness;               /*!< the maximum score of all Genome elements in this Species */
  int * innovation;                 /*!< the address of the innovation number of the MatingPool containing this Species */
};

/*!
* \struct MatingPool population.h
* \brief A MatingPool gathers all the Species.
*/
struct MatingPool {
  Species species[N_MAX_SPECIES];     /*!< the Genome elements in the MatingPool are split into distinct groups known as Species */
  short int nb_species;               /*!< the number of Species contained in this MatingPool */
  short int generation;               /*!< the current number of generations */
  double max_fitness;                 /*!< the maximum score of all Genome elements in this MatingPool */
  int innovation;                     /*!< the innovation number is used to track the history of a ConnectionGene evolution */
};

MatingPool * newMatingPool();
void freeMatingPool(MatingPool * pool);

void populateMatingPool(MatingPool * pool);
int addSpeciesToMatingPool(MatingPool * pool);

int addGenomeToSpecies(Species * species);

void computeGlobalRanks(MatingPool * pool);

Genome * getRandomGenome(Species * species);

#endif // POPULATION_H
