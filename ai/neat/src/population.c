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

void freeMatingPool(MatingPool * pool) {
  int i;
  int j;

  for (i = 0; i < pool->nb_species; ++i) {
    for (j = 0; j < pool->species[i].nb_genomes; ++j) {
      freeNeuronList(pool->species[i].genomes[j].network);
    }
  }

  free(pool);
}

/*!
* \brief Populate a MatinPool with new Genome elements.
* \param[out] pool the MatingPool to populate with new Genome elements
*/
void populateMatingPool(MatingPool * pool) {
  int i;

  if (pool->nb_species == 0)
    addSpeciesToMatingPool(pool);

  for (i = 0; i < POPULATION; ++i) {
    addGenomeToSpecies(&pool->species[0]);
  }
}

/*!
* \brief Add the given Species to the given MatinPool
* \param[out] pool the MatingPool where to insert the Species
* \return int 1 if the Species was successfully added to the MatingPool, 0 otherwise
*/
int addSpeciesToMatingPool(MatingPool * pool) {
  if (pool->nb_species == N_MAX_SPECIES) {
    fprintf(stderr, "Can't add new Species to MatingPool : reached limit (%d, max=%d)\n", pool->nb_species, N_MAX_SPECIES);
    return 0;
  }

  pool->species[pool->nb_species].id = pool->nb_species;
  pool->species[pool->nb_species].nb_genomes = 0;
  pool->species[pool->nb_species].max_fitness = 0.0;

  pool->nb_species++;

  return 1;
}

/*!
* \brief Add a genome to the appropriate species of the mating pool
* \param[out] pool the MatingPool where Genome elements are added
* \return int 1 if the Genome was successfully added to the Species, 0 otherwise
*/
int addGenomeToSpecies(Species * species) {
  Network * network = NULL;

  if (species->nb_genomes == N_MAX_GENOMES) {
    fprintf(stderr, "Can't add Genome to Species : reached limit (%d, max=%d)\n", species->nb_genomes, N_MAX_GENOMES);
    return 0;
  }

  network = newNeuronList();
  initNeuronList(network);

  species->genomes[species->nb_genomes].network = network;

  species->nb_genomes++;

  return 1;
}
