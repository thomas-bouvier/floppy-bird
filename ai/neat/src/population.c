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

  for (i = 0; i < pool->nb_species; ++i)
    freeList(pool->species[i].genomes);

  free(pool);
}

/*!
* \brief Populate a MatingPool with new Genome elements
* \param[out] pool the MatingPool to populate with new Genome elements
* \return int Return 1 if the MatingPool was successfully filled, 0 otherwise
*/
int populateMatingPool(MatingPool * pool) {
  int i;
  Genome * genome = NULL;

  for (i = 0; i < POPULATION; ++i) {
    if ((genome = newGenome(&pool->innovation)) == (Genome *) NULL)
      return 0;

    mutate(genome);

    if (!addGenomeToProperSpecies(genome, pool))
      return 0;
  }

  return 1;
}

/*!
* \brief Add the given Species to the given MatingPool
* \param[out] pool the MatingPool where to insert the Species
* \return int 1 if the Species was successfully added to the MatingPool, 0 otherwise
*/
int addSpeciesToMatingPool(MatingPool * pool) {
  List * genomes = NULL;

  if (pool->nb_species == N_MAX_SPECIES) {
    fprintf(stderr, "Can't add new Species to MatingPool : reached limit (max=%d)\n", N_MAX_SPECIES);
    return 0;
  }

  if ((genomes = newList(freeGenome)) == (List *) NULL)
    return 0;

  initList(genomes);
  pool->species[pool->nb_species].genomes = genomes;

  pool->species[pool->nb_species].id = pool->nb_species;
  pool->species[pool->nb_species].nb_genomes = 0;
  pool->species[pool->nb_species].max_fitness = 0.0;
  pool->species[pool->nb_species].average_fitness = 0.0;
  pool->species[pool->nb_species].innovation = &pool->innovation;

  ++pool->nb_species;

  return 1;
}

int generateNewGeneration(MatingPool * pool) {
  int i;
  int nb_genomes = 0;
  Genome * children[N_MAX_SPECIES * N_MAX_GENOMES];

  for (i = 0; i < pool->nb_species; ++i)
    computeAverageFitness(&pool->species[i]);

  for (i = 0; i < nb_genomes; ++i)
    addGenomeToProperSpecies(children[i], pool);

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

  j = 0;
  for (i = 0; i < pool->nb_species; ++i) {
    setOnFirst(pool->species[i].genomes);
    while (!outOfList(pool->species[i].genomes)) {
      genomes[j * pool->nb_species + i] = getCurrent(pool->species[i].genomes);

      ++j;
      next(pool->species[i].genomes);
    }
  }

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


static int addGenomeToSpecies(Genome * genome, Species * species) {
  if (species->nb_genomes == N_MAX_GENOMES) {
    fprintf(stderr, "Can't add Genome to Species : reached limit (max=%d)\n", N_MAX_GENOMES);
    return 0;
  }

  if (!add(species->genomes, genome))
    return 0;

  ++species->nb_genomes;

  return 1;
}

/*!
* \brief Add the given Genome to the appropriate Species of the MatingPool
* \param[in] genome the Genome to add
* \param[out] pool the MatingPool where Genome elements are added
* \return int 1 if the Genome was successfully added to the Species, 0 otherwise
*/
int addGenomeToProperSpecies(Genome * genome, MatingPool * pool) {
  int i;

  // we're looking for a species that matches the given genome

  for (i = 0; i < pool->nb_species; ++i) {
    if (pool->species[i].nb_genomes > 0) {
      if (sameSpecies(genome, (Genome *) pool->species[i].genomes->first->data)) {
        addGenomeToSpecies(genome, &pool->species[i]);
        return 1;
      }
    }
  }

  // no species matches the given genome

  if (!addSpeciesToMatingPool(pool))
    return 0;

  if (!addGenomeToSpecies(genome, &pool->species[pool->nb_species - 1]))
    return 0;

  return 1;
}

/*!
* \brief Compute the average fitness of the given Species
* \param[out] species The Species whose the average fitness has to be calculated
*/
void computeAverageFitness(Species * species) {
  double sum = 0.0;

  setOnFirst(species->genomes);
  while (!outOfList(species->genomes)) {
    sum += ((Genome *) species->genomes->current->data)->global_rank;

    next(species->genomes);
  }

  species->average_fitness = sum / species->nb_genomes;
}

/*!
* \brief Return a random Genome from the given Species.
* \param[in] species the Species to choose a Genome from
* \return Return a random Genome
*/
Genome * getRandomGenome(Species * species) {
  setOn(species->genomes, randomAtMost(species->nb_genomes));
  return getCurrent(species->genomes);
}

/*!
* \brief Print the given MatingPool.
* \param[in] pool the MatingPool to print
*/
void printMatingPool(MatingPool * pool) {
  int i;
  int j;
  Genome * current_genome = NULL;

  printf("nb_species: %d\n", pool->nb_species);

  for (i = 0; i < pool->nb_species; ++i) {
    printf("\n");
    printf("==================================\n");
    printf("==================================\n");
    printf("Species no %d\n", i);
    printf("nb_genomes: %d\n", pool->species[i].nb_genomes);

    j = 0;
    setOnFirst(pool->species[i].genomes);
    while (!outOfList(pool->species[i].genomes)) {
      current_genome = (Genome *) getCurrent(pool->species[i].genomes);

      printf("----------------------------------\n");
      printf("Genome no %d (%p)\n", j, (void *) current_genome);

      printf("\tPOINT_MUTATION_RATE: %f\n", current_genome->mutation_rates[0]);
      printf("\tLINK_MUTATION_RATE: %f\n", current_genome->mutation_rates[1]);
      printf("\tNODE_MUTATION_RATE: %f\n", current_genome->mutation_rates[2]);
      printf("\tENABLE_DISABLE_MUTATION_RATE: %f\n", current_genome->mutation_rates[3]);

      printf("\tnb_neurons: %d\n", current_genome->nb_neurons);
      printf("\tnb_connection_genes: %d\n", current_genome->nb_connection_genes);
      printf("\n");

      ++j;
      next(pool->species[i].genomes);
    }
  }
}
