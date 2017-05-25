/*!
* \file neat_conf.h
* \brief File containing neural network parameters.
*/
#ifndef NEAT_CONF_H
#define NEAT_CONF_H

/*! \def POPULATION
* \brief The number of Genome elements in a new generation.
*/
#define POPULATION                      50

/*! \def N_INPUTS
* \brief The number of input Neuron elements in the Genome.
*/
#define N_INPUTS                        4

/*! \def N_OUTPUTS
* \brief The number of output Neuron elements in the Genome.
*/
#define N_OUTPUTS                       1

/*! \def N_MAX_SPECIES
* \brief The max number of Species allowed in a new generation.
*/
#define N_MAX_SPECIES                   50

/*! \def N_MAX_GENOMES
* \brief The max number of Genome elements allowed in a Species.
*/
#define N_MAX_GENOMES                   50

/*! \def N_MAX_CONNECTION_GENES
* \brief The max number of ConnectionGene elements allowed in a Genome.
*/
#define N_MAX_CONNECTION_GENES          50

/*! \def N_MAX_NEURONS
* \brief The max number of Neuron elements allowed in a Genome.
*/
#define N_MAX_NEURONS                   50

/*! \def N_MAX_MUTATIONS
* \brief The max number of mutations allowed in a Genome.
*/
#define N_MAX_MUTATIONS                 100

/*! \def DECREASE_PROBABILITY
* \brief Probability of decreasing a mutation rate, instead of increasing it.
*/
#define DECREASE_PROBABILITY            0.50

/*! \def POINT_MUTATION_PERTURBATION
* \brief Probability of adding a fraction of POINT_MUTATION_RATE to the weight of a ConnectionGene, instead of reseting it around 0.
*/
#define POINT_MUTATION_PERTURBATION     0.75

/*! \def POINT_MUTATION_RATE
* \brief The mutation rate linked to point mutations.
*/
#define POINT_MUTATION_RATE             0.25

/*! \def LINK_MUTATION_RATE
* \brief The mutation rate linked to link mutations.
*/
#define LINK_MUTATION_RATE              2.00

/*! \def BIAS_MUTATION_RATE
* \brief The mutation rate linked to bias mutations.
*/
#define BIAS_MUTATION_RATE              0.40

/*! \def NODE_MUTATION_RATE
* \brief The mutation rate linked to node mutations.
*/
#define NODE_MUTATION_RATE              0.50

/*! \def ENABLE_MUTATION_RATE
* \brief The mutation rate linked to enable mutations.
*/
#define ENABLE_MUTATION_RATE            0.20

/*! \def DISABLE_MUTATION_RATE
* \brief The mutation rate linked to disable mutations.
*/
#define DISABLE_MUTATION_RATE           0.40

/*! \def CROSSOVER_RATE
* \brief The probability of performing a crossover instead of a mutation when breeding a child Genome.
*/
#define CROSSOVER_RATE                  0.50

/*! \def SPECIATION_THRESHOLD
* \brief The threshold used to determine if two Genome elements are from the same Species.
*/
#define SPECIATION_THRESHOLD            6.00

/*! \def WEIGHT_COEFFICIENT
* \brief The weight given to the weight of ConnectionGene elements when performing speciation.
*/
#define WEIGHT_COEFFICIENT              0.40

/*! \def DISJOINT_COEFFICIENT
* \brief The weight given to disjoint ConnectionGene elements when performing speciation.
*/
#define DISJOINT_COEFFICIENT            2.00

/*! \def WEAK_SPECIES_THRESHOLD
* \brief The threshold used for a Species to be considered as weak.
*/
#define WEAK_SPECIES_THRESHOLD          1.00

/*! \def STALE_SPECIES_THRESHOLD
* \brief The number of generations a Species without any mutation will survive.
*/
#define STALE_SPECIES_THRESHOLD         15

#endif // NEAT_CONF_H
