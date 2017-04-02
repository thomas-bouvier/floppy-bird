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
  new_mating_pool->average_fitness = 0.0;
  new_mating_pool->innovation = 0;

  return new_mating_pool;
}

/*!
* \brief Free the given MatingPool
* \param[out] pool the MatingPool to be freed
*/
void freeMatingPool(MatingPool * pool) {
  int i;
  int j;

  for (i = 0; i < pool->nb_species; ++i) {
    for (j = 0; j < pool->species[i].nb_genomes; ++j) {
      freeList(pool->species[i].genomes[j].network);
    }
  }

  free(pool);
}

/*!
* \brief Populate a MatingPool with new Genome elements
* \param[out] pool the MatingPool to populate with new Genome elements
*/
void populateMatingPool(MatingPool * pool) {
  int i;

  if (pool->nb_species == 0)
    addSpeciesToMatingPool(pool);

  for (i = 0; i < POPULATION; ++i)
    addGenomeToSpecies(&pool->species[0]);
}

/*!
* \brief Add the given Species to the given MatingPool
* \param[out] pool the MatingPool where to insert the Species
* \return int 1 if the Species was successfully added to the MatingPool, 0 otherwise
*/
int addSpeciesToMatingPool(MatingPool * pool) {
  if (pool->nb_species == N_MAX_SPECIES) {
    fprintf(stderr, "Can't add new Species to MatingPool : reached limit (max=%d)\n", N_MAX_SPECIES);
    return 0;
  }

  pool->species[pool->nb_species].id = pool->nb_species;
  pool->species[pool->nb_species].nb_genomes = 0;
  pool->species[pool->nb_species].max_fitness = 0.0;
  pool->species[pool->nb_species].innovation = &pool->innovation;

  pool->nb_species++;

  return 1;
}

int generateNewGeneration(MatingPool * pool) {
  int i;

  for (i = 0; i < pool->nb_species; ++i)
    computeAverageFitness(&pool->species[i]);

  ++pool->generation;

  return 1;
}

static int compareFitness(const void * genome_1, const void * genome_2) {
  return (((Genome*) genome_1)->fitness - ((Genome*) genome_2)->fitness);
}

/*!
* \brief Compute the global rank of all Genome elements from all Species
* \param[out] pool the MatingPool whose global ranks of Genomes elements have to be calculated
*/
void computeGlobalRanks(MatingPool * pool) {
  int i;
  int j;
  int k;
  Genome * genomes[N_MAX_SPECIES * N_MAX_GENOMES];

  // we're storing the addresses of all genomes from all species in a single array

  for (j = 0; j < pool->nb_species; ++j)
    for (i = 0; i < pool->species[j].nb_genomes; ++i)
      genomes[j * pool->nb_species + i] = &pool->species[j].genomes[i];

  // we're sorting the genomes from their fitness

  qsort(genomes, j * pool->nb_species + i, sizeof(Genome *), compareFitness);

  // we're calculating the global rank for the current genome

  for (k = 0; k <= j * pool->nb_species + i; ++k)
    genomes[k]->global_rank = k;
}

/*!
* \brief Compute the average fitness of the given MatingPool
* \param[out] pool The MatingPool whose the average fitness has to be calculated
*/
void computeGlobalAverageFitness(MatingPool * pool) {
  int i;
  double sum = 0.0;

  for (i = 0; i < pool->nb_species; ++i)
    sum += pool->species[i].average_fitness;

  pool->average_fitness = sum / pool->nb_species;
}

/*!
* \brief Add a Genome to the appropriate Species of the MatingPool
* \param[out] pool the MatingPool where Genome elements are added
* \return int 1 if the Genome was successfully added to the Species, 0 otherwise
*/
int addGenomeToSpecies(Species * species) {
  Network * network = NULL;

  if (species->nb_genomes == N_MAX_GENOMES) {
    fprintf(stderr, "Can't add Genome to Species : reached limit (max=%d)\n", N_MAX_GENOMES);
    return 0;
  }

  network = newList(freeNeuron);

  if (network == NULL)
    return 0;

  initList(network);

  species->genomes[species->nb_genomes].network = network;
  species->genomes[species->nb_genomes].nb_neurons = 0;
  species->genomes[species->nb_genomes].nb_connection_genes = 0;
  species->genomes[species->nb_genomes].fitness = 0.0;

  // initializing mutation rates

  species->genomes[species->nb_genomes].mutation_rates[0] = POINT_MUTATION_RATE;
  species->genomes[species->nb_genomes].mutation_rates[1] = LINK_MUTATION_RATE;
  species->genomes[species->nb_genomes].mutation_rates[2] = NODE_MUTATION_RATE;
  species->genomes[species->nb_genomes].mutation_rates[3] = ENABLE_DISABLE_MUTATION_RATE;

  species->genomes[species->nb_genomes].global_rank = 0;
  species->genomes[species->nb_genomes].innovation = species->innovation;

  species->nb_genomes++;

  return 1;
}

int addGenomeToProperSpecies(Genome * genome, MatingPool * pool) {
  int i;

  for (i = 0; i < pool->nb_species; ++i) {
    if (pool->species[i].nb_genomes > 0) {
      if (sameSpecies(genome, &pool->species[i].genomes[0])) {
        addGenomeToSpecies(&pool->species[i]);
        return 1;
      }
    }
  }

  if (!addSpeciesToMatingPool(pool))
    return 0;

  return 1;
}

/*!
* \brief Compute the average fitness of the given Species
* \param[out] species The Species whose the average fitness has to be calculated
*/
void computeAverageFitness(Species * species) {
  int i;
  double sum = 0.0;

  for (i = 0; i < species->nb_genomes; ++i)
    sum += species->genomes[i].global_rank;

  species->average_fitness = sum / species->nb_genomes;
}

/*!
* \brief Return a random Genome from the given Species.
* \param[in] species the Species to choose a Genome from
* \return Return a random Genome
*/
Genome * getRandomGenome(Species * species) {
  int count = species->nb_genomes;
  return &species->genomes[count];
}
