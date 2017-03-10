#include "population.h"

/*!
* \brief Create a MatingPool
* \return Return a MatingPool, NULL if error
*/
MatingPool * newMatingPool() {
  MatingPool * new_mating_pool = (MatingPool *) malloc(sizeof(MatingPool));

  if (new_mating_pool == (MatingPool *) NULL) {
    fprintf(stderr, "Error while allocating memory for new MatingPool\n");
    return NULL;
  }

  new_mating_pool->nb_species = 0;
  new_mating_pool->generation = 0;
  new_mating_pool->max_fitness = 0.0;

  return new_mating_pool;
}

/*!
* \brief Populate a MatinPool with new Genome elements.
* \param[out] pool the MatingPool to populate with new Genome elements
*/
void populateMatingPool(MatingPool * pool) {
  int i;
  Genome * genome = NULL;

  if (pool->nb_species == 0)
    addSpeciesToMatingPool(pool, newSpecies(pool));

  for (i = 0; i < POPULATION; ++i) {
    genome = newGenome();
    addGenomeToSpecies(&pool->species[0], genome);
  }
}

/*!
* \brief Add the given Species to the given MatinPool
* \param[out] pool the MatingPool where to insert the Species
* \param[in] species the Species to insert
* \return int 1 if the Species was successfully added to the MatingPool, 0 otherwise
*/
int addSpeciesToMatingPool(MatingPool * pool, Species * species) {
  if (pool->nb_species == N_MAX_SPECIES) {
    fprintf(stderr, "Can't add new Species to MatingPool : reached limit (%d, max=%d)\n", pool->nb_species, N_MAX_SPECIES);
    return 0;
  }

  pool->species[pool->nb_species] = *species;
  pool->nb_species++;

  return 1;
}

/*!
* \brief Create a Species
* \return Return a MatingPool, or NULL if error
*/
Species * newSpecies(MatingPool * pool) {
  Species * new_species = (Species *) malloc(sizeof(Species));

  if (new_species == (Species *) NULL) {
    fprintf(stderr, "Error while allocating memory new Species\n");
    return NULL;
  }

  new_species->id = pool->nb_species;

  return new_species;
}

/*!
* \brief Add a genome to the appropriate species of the mating pool
* \param[out] pool the MatingPool where Genome elements are added
* \param[out] genome the Genome to add to Species
* \return int 1 if the Genome was successfully added to the Species, 0 otherwise
*/
int addGenomeToSpecies(Species * species, Genome * genome) {
  if (species->nb_genomes == N_MAX_GENOMES) {
    fprintf(stderr, "Can't add Genome to Species : reached limit (%d, max=%d)\n", species->nb_genomes, N_MAX_GENOMES);
    return 0;
  }

  species->genomes[species->nb_genomes] = *genome;
  species->nb_genomes++;

  return 1;
}
