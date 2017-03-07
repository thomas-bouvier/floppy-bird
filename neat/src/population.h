/*!
* \file population.h
* \brief File containing structures and prototypes linked to the population
*/
#ifndef POPULATION_H
#define POPULATION_H

#include <stdlib.h>
#include <stdio.h>

#include "conf.h"
#include "species.h"

/*!
* \struct MatingPool genome.h
* \brief A mating pool gathers all the genomes
*/
typedef struct {
  Species species[N_MAX_SPECIES];   /*!< genomes in the mating pool are split into distinct groups known as species */
  short int generation;             /*!< the current number of generations */
  double max_fitness;               /*!< the maximum score of all genomes in this mating pool */
} MatingPool;

#endif // POPULATION_H
