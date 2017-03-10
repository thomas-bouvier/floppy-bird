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
  Genome genomes[N_MAX_GENOMES];    /*!< genomes contained in this Species */
  short int id;                     /*!< the id of this species */
  short int nb_genomes;             /*!< the number of genomes contained in this Species */
  double max_fitness;               /*!< the maximum score of all genomes in this Species */
};

/*!
* \struct MatingPool population.h
* \brief A MatingPool gathers all the Species.
*/
struct MatingPool {
  Species species[N_MAX_SPECIES];     /*!< the genomes in the mating pool are split into distinct groups known as species */
  short int nb_species;               /*!< the number of species contained in this mating pool */
  short int generation;               /*!< the current number of generations */
  double max_fitness;                 /*!< the maximum score of all genomes in this mating pool */
};

MatingPool * newMatingPool();
void populateMatingPool(MatingPool * mating_pool);

int addSpeciesToMatingPool(MatingPool * pool, Species * species);

Species * newSpecies(MatingPool * pool);

int addGenomeToSpecies(Species * species, Genome * genome);

#endif // POPULATION_H
