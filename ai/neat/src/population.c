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
    new_mating_pool->sum_average_fitnesses = 0.0;
    new_mating_pool->average_fitness = 0.0;
    new_mating_pool->innovation = N_OUTPUTS;

    return new_mating_pool;
}

/*!
* \brief Free the given MatingPool
* \param[out] pool the MatingPool to be freed
*/
void freeMatingPool(MatingPool * pool) {
    int i;

    for (i = 0; i < pool->nb_species; ++i)
        freeGenericList(pool->species[i].genomes);

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

        genome->max_neurons = N_INPUTS;
        mutate(genome);

        if (!addGenomeToProperSpecies(genome, pool))
            return 0;
    }

    return 1;
}

/*!
* \brief Add a new blank Species to the given MatingPool
* \param[out] pool the MatingPool where to insert a new blank Species
* \return int 1 if the new blank Species was successfully added to the MatingPool, 0 otherwise
*/
int addSpeciesToMatingPool(MatingPool * pool) {
    GenericList * genomes = NULL;

    if (pool->nb_species == N_MAX_SPECIES) {
        fprintf(stderr, "Can't add new Species to MatingPool : reached limit (max=%d)\n", N_MAX_SPECIES);
        return 0;
    }

    if ((genomes = newGenericList(cloneGenome, freeGenome)) == (GenericList *) NULL)
        return 0;

    initGenericList(genomes);
    pool->species[pool->nb_species].genomes = genomes;

    pool->species[pool->nb_species].id = pool->nb_species;
    pool->species[pool->nb_species].nb_genomes = 0;
    pool->species[pool->nb_species].max_fitness = 0.0;
    pool->species[pool->nb_species].average_fitness = 0.0;
    pool->species[pool->nb_species].staleness = 0;
    pool->species[pool->nb_species].innovation = &pool->innovation;

    ++pool->nb_species;

    return 1;
}

/*!
* \brief Remove the Species whose id is given from the given MatingPool
* \param[out] pool the MatingPool where the Species to remove is contained
* \param[in] id the id of the Species to remove
* \return int 1 if the Species was successfully removed from the MatingPool, 0 otherwise
*/
int removeSpecies(MatingPool * pool, short int id) {
  int i;
  int index = -1;

  for (i = 0; i < pool->nb_species; ++i) {
    if (pool->species[i].id == id) {
      index = i;
      break;
    }
  }

  if (index == -1) {
    fprintf(stderr, "No Species found with id %d\n", id);
    return 0;
  }

  freeGenericList(pool->species[i].genomes);

  for (i = index; i < pool->nb_species - 1; ++i)
    pool->species[i] = pool->species[i + 1];

  --pool->nb_species;

  return 1;
}

/*!
* \brief Generate the next generation of Species
* \param[out] pool the MatingPool whose next generation of Species has to be generated
* \param[in] verbose bool value indicating whether an output has to be printed in the console
* \return int 1 if the new generation was successfully created, 0 otherwise
*
* A new generation is created by culling Species elements which contain weak Genome elements,
* before breeding each remaining Species and keeping the stronger Genome element.
*/
int newGeneration(MatingPool * pool, int verbose) {
  int i;
  int j;
  int count;
  double breed;
  Genome * children[POPULATION];
  Genome * child;

  if (verbose) {
    printf("==================================\n");
    printf("==================================\n");
    printf("==================================\n");
    printf("New generation...\n");
  }

  cullSpecies(pool, 0);

  computeGlobalRanks(pool);
  removeStaleSpecies(pool);

  computeGlobalRanks(pool);
  removeWeakSpecies(pool, 0);

  count = 0;
  for (i = 0; i < pool->nb_species; ++i) {
    if (verbose) {
      printf("\n\n\n");
      printf("Species no %d\n", i);
    }

    breed = floor(pool->species[i].average_fitness / pool->sum_average_fitnesses * POPULATION) - 1.0;

    for (j = 0; j < breed; ++j) {
      child = breedGenome(&pool->species[i], verbose);
      if (!child) {
        fprintf(stderr, "Child Genome can't be NULL\n");
        return 0;
      }

      children[count] = child;
      ++count;
    }
  }

  cullSpecies(pool, 1);

  while (count + pool->nb_species < POPULATION) {
    children[count] = breedGenome(&pool->species[randomLimit(pool->nb_species - 1)], verbose);
    ++count;
  }

  for (i = 0; i < count; ++i)
    if (!addGenomeToProperSpecies(children[i], pool))
      return 0;

  ++pool->generation;

  return 1;
}

/*!
* \brief Compare two Genome elements based on their fitness
* \param[in] genome_1 the first Genome to compare
* \param[in] genome_2 the second Genome to compare
* \return int 0 if the fitnesses are equal, 1 if the first Genome has a greater fitness than the second Genome, -1 otherwise
*/
static int compareFitnessCulling(const void * genome_1, const void * genome_2) {
  int diff = ((Genome *) genome_1)->fitness - ((Genome *) genome_2)->fitness;

  if (diff == 0.0)
    return 0;
  else if (diff < 0.0)
    return -1;

  return 1;
}

void cullSpecies(MatingPool * pool, int cut_to_one) {
  int i;
  double remaining;

  for (i = 0; i < pool->nb_species; ++i) {
    sort(pool->species[i].genomes, compareFitnessCulling);

    if (cut_to_one)
      remaining = 1.0;
    else
      remaining = ceil(pool->species[i].nb_genomes / 2.0);

    setOnFirstElement(pool->species[i].genomes);
    while (!outOfGenericList(pool->species[i].genomes) && pool->species[i].nb_genomes > remaining) {
      delete(pool->species[i].genomes, (Genome *) getCurrent(pool->species[i].genomes));
      setOnFirstElement(pool->species[i].genomes);
      --pool->species[i].nb_genomes;
    }
  }
}

/*!
* \brief Remove weak Species from the given MatingPool ie. Species that don't reach WEAK_SPECIES_THRESHOLD
* \param[out] pool the MatingPool whose weak Species have to be removed
* \param[in] verbose bool value indicating whether an output has to be printed in the console
*
* A Species is considered as weak when the ratio its average fitness to the average fitness of the MatingPool,
* times the number of Genome elements in a Species ie. POPULATION is lower than WEAK_SPECIES_THRESHOLD.
*/
void removeWeakSpecies(MatingPool * pool, int verbose) {
  int i;
  double breed;

  for (i = 0; i < pool->nb_species; ++i)
    computeAverageFitness(&pool->species[i]);

  computeGlobalAverageFitness(pool);

  for (i = 0; i < pool->nb_species; ++i) {
    breed = pool->species[i].average_fitness / pool->sum_average_fitnesses * POPULATION;

    if (verbose) {
      printf("species average fitness: %f\n", pool->species[i].average_fitness);
      printf("pool average fitness: %f\n", pool->average_fitness);
      printf("breed: %f\n", breed);
    }

    if (breed < WEAK_SPECIES_THRESHOLD)
      removeSpecies(pool, pool->species[i].id);
  }
}

/*!
* \brief Remove stale Species from the given MatingPool ie. Species that don't reach WEAK_SPECIES_THRESHOLD
* \param[out] pool the MatingPool whose stale Species have to be removed
*
* A Species is considered as stale when
*/
void removeStaleSpecies(MatingPool * pool) {
  int i;

  for (i = 0; i < pool->nb_species; ++i) {
    if (!emptyGenericList(pool->species[i].genomes)) {
      sort(pool->species[i].genomes, compareFitnessCulling);

      setOnFirstElement(pool->species[i].genomes);
      if (((Genome *) getCurrent(pool->species[i].genomes))->fitness > pool->species[i].max_fitness) {
        pool->species[i].max_fitness = ((Genome *) getCurrent(pool->species[i].genomes))->fitness;
        pool->species[i].staleness = 0;
      }
      else
        ++pool->species[i].staleness;

      if (pool->species[i].staleness >= STALE_SPECIES_THRESHOLD)
        removeSpecies(pool, pool->species[i].id);
    }
  }
}

/*!
* \brief Breed a new Genome element by randomly selecting Genome elements in the given MatingPool
* \param[out] species
* \param[in] verbose bool value indicating whether an output has to be printed in the console
*/
Genome * breedGenome(Species * species, int verbose) {
  double p;
  Genome * child = NULL;

  Genome * genome_1 = NULL;
  Genome * genome_2 = NULL;

  if (verbose) {
    printf("\n");
    printf("Breeding genome...\n");
    printf("----------------------------------\n");
    printf("----------------------------------\n");
  }

  p = random01();

  if (verbose) {
    printf("CROSSOVER_RATE: %f\n", CROSSOVER_RATE);
    printf("prob: %f\n", p);
  }

  if (p < CROSSOVER_RATE) {
    genome_1 = getRandomGenome(species);
    genome_2 = getRandomGenome(species);

    if (verbose) {
      printf("\n");
      printf("Crossover...\n");
      printf("----------------------------------\n");
      printf("----------------------------------\n");

      printf("\n");
      printf("Genome no 1\n");
      printGenome(genome_1);

      printf("\n");
      printf("Genome no 2\n");
      printGenome(genome_2);
    }

    child = crossover(genome_1, genome_2);
  }
  else {
    genome_1 = getRandomGenome(species);

    if (verbose) {
        printf("\n");
        printf("Cloning random genome...\n");
        printf("----------------------------------\n");
        printf("----------------------------------\n");

        printf("\n");
        printf("Genome\n");
        printGenome(genome_1);
    }

    child = (Genome *) cloneGenome(genome_1);
  }

  mutate(child);

  return child;
}

/*!
* \brief Compare two Genome elements based on their fitness
* \param[in] genome_1 the first Genome to compare
* \param[in] genome_2 the second Genome to compare
* \return int a positive integer if the first Genome has a greater fitness, a negative number otherwise
*/
static int compareFitnessGlobalRanks(const void * genome_1, const void * genome_2) {
  return (*(Genome **) genome_1)->fitness - (*(Genome **) genome_2)->fitness;
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
    setOnFirstElement(pool->species[i].genomes);
    while (!outOfGenericList(pool->species[i].genomes)) {
      genomes[j] = ((Genome *) getCurrent(pool->species[i].genomes));

      ++j;
      nextElement(pool->species[i].genomes);
    }
  }

  // we're sorting the genomes from their fitness

  qsort(genomes, j, sizeof(Genome *), compareFitnessGlobalRanks);

  // we're calculating the global rank for the current genome

  for (k = 0; k < j; ++k)
    genomes[k]->global_rank = k;
}

/*!
* \brief Compute the average fitness of the given MatingPool
* \param[out] pool the MatingPool whose the average fitness has to be calculated
*/
void computeGlobalAverageFitness(MatingPool * pool) {
  int i;
  double sum = 0.0;

  for (i = 0; i < pool->nb_species; ++i)
    sum += pool->species[i].average_fitness;

  pool->sum_average_fitnesses = sum;
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
*
* The advantage of splitting the Genome elements into Species is that during the genetic evolution
* step where weak Genome elements are culled, each Genome element has to fight against Genome
* elements of the same Species, rather than against every other Genome elements in the entire
* MatingPool. This way Species which form a new topological innovation that might not have a high
* fitness yet due to not having its weights optimised will survive the culling.
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
* \param[out] species the Species whose the average fitness has to be calculated
*/
void computeAverageFitness(Species * species) {
  double sum = 0.0;

  setOnFirstElement(species->genomes);
  while (!outOfGenericList(species->genomes)) {
    sum += ((Genome *) species->genomes->current->data)->global_rank;

    nextElement(species->genomes);
  }

  species->average_fitness = sum / species->nb_genomes;
}

/*!
* \brief Return a random Genome from the given Species
* \param[in] species the Species to choose a Genome from
* \return a random Genome element
*/
Genome * getRandomGenome(Species * species) {
  setOn(species->genomes, randomLimit(species->nb_genomes - 1));
  return getCurrent(species->genomes);
}

/*!
* \brief Print the given MatingPool
* \param[in] pool the MatingPool to print
*/
void printMatingPool(MatingPool * pool) {
    int i;

    printf("==================================\n");
    printf("==================================\n");
    printf("==================================\n");
    printf("Printing MatingPool...\n");

    printf("\n");
    printf("nb_species: %d\n", pool->nb_species);
    printf("generation: %d\n", pool->generation);

    for (i = 0; i < pool->nb_species; ++i) {
        printf("\n");
        printf("==================================\n");
        printf("==================================\n");
        printf("Species no %d\n", i);
        printf("\tnb_genomes: %d\n", pool->species[i].nb_genomes);

        setOnFirstElement(pool->species[i].genomes);
        while (!outOfGenericList(pool->species[i].genomes)) {
            printGenome((Genome *) getCurrent(pool->species[i].genomes));

            nextElement(pool->species[i].genomes);
        }
    }
}
