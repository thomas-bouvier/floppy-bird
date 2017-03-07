/*!
* \file species.h
* \brief File containing structures and prototypes linked to species
*/
#ifndef SPECIES_H
#define SPECIES_H

#include <stdlib.h>
#include <stdio.h>

#include "conf.h"
#include "genome.h"

/*!
* \struct Species species.h
* \brief A species contains genomes which have similar characteristics
*/
typedef struct {
  Genome genomes[N_MAX_GENOMES];    /*!< genomes contained in this species */
  double max_fitness;               /*!< the maximum score of all genomes in species */
} Species;

#endif // SPECIES_H
