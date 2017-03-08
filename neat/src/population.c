#include "population.h"

/*!
* \brief Create a mating pool gene
* \return Return a mating pool, NULL if error
*/
MatingPool * newMatingPool() {
  MatingPool * mating_pool = (MatingPool *) malloc(sizeof(MatingPool));

  if (mating_pool == (MatingPool *) NULL) {
    fprintf(stderr, "Error while allocating memory for MatingPool\n");
    return NULL;
  }

  mating_pool->nb_species = 0;
  mating_pool->generation = 0;
  mating_pool->max_fitness = 0.0;

  return mating_pool;
}

/*!
* \brief Populate a mating pool with new genomes
* \param[out] pool the mating pool to populate with new genomes
*/
void populateMatingPool(MatingPool * pool) {
  int i;
  Genome * genome = NULL;

  for (i = 0; i < POPULATION; ++i) {
    genome = newGenome();
    addGenomeToSpecies(pool, genome);
  }
}

/*!
* \brief Add a genome to the appropriate species of the mating pool
* \param[out] pool the mating pool where genomes are added
* \param[out] genome the genome to add to species
*/
void addGenomeToSpecies(MatingPool * pool, Genome * genome) {
  int i;

  for (i = 0; i < pool->nb_species; ++i) {

  }
}
