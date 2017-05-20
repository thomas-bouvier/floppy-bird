/*!
* \file neat_conf.h
* \brief File containing neural network parameters.
*/
#ifndef NEAT_CONF_H
#define NEAT_CONF_H

#define POPULATION                      50

#define N_MAX_SPECIES                   50
#define N_MAX_GENOMES                   50
#define N_MAX_CONNECTION_GENES          50
#define N_MAX_NEURONS                   50
#define N_MAX_NETWORK                   50
#define N_MAX_MUTATIONS                 100

#define N_INPUTS                        4
#define N_OUTPUTS                       1

#define DECREASE_PROBABILITY            0.50
#define POINT_MUTATION_PERTURBATION     0.90

#define POINT_MUTATION_RATE             0.25
#define LINK_MUTATION_RATE              2.00
#define BIAS_MUTATION_RATE              0.40
#define NODE_MUTATION_RATE              0.50
#define ENABLE_MUTATION_RATE            0.20
#define DISABLE_MUTATION_RATE           0.40

#define CROSSOVER_RATE                  0.75

#define SPECIATION_THRESHOLD            1.00
#define WEIGHT_COEFFICIENT              0.40
#define DISJOINT_COEFFICIENT            2.00

#define WEAK_SPECIES_THRESHOLD          1.00
#define STALE_SPECIES_THRESHOLD         10

#endif // NEAT_CONF_H
