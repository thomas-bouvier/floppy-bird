#include "genome.h"

/*!
* \brief Create a Genome
* \return Return a new Genome, or NULL if error
*/
Genome * newGenome(int * innovation) {
    GenericList * new_neurons_list = NULL;
    GenericList * new_connection_genes_list = NULL;
    Genome * new_genome = NULL;

    if (innovation == NULL) {
        fprintf(stderr, "Can't create a new Genome without attached innovation number\n");
        return NULL;
    }

    if ((new_genome = malloc(sizeof(Genome))) == (Genome * ) NULL) {
        fprintf(stderr, "Error while allocating memory for new Genome\n");
        return NULL;
    }

    new_neurons_list = newGenericList(cloneNeuron, freeNeuron);
    initGenericList(new_neurons_list);

    new_connection_genes_list = newGenericList(cloneConnectionGene, freeConnectionGene);
    initGenericList(new_connection_genes_list);

    new_genome->neurons = new_neurons_list;
    new_genome->nb_neurons = 0;
    new_genome->connection_genes = new_connection_genes_list;
    new_genome->nb_connection_genes = 0;
    new_genome->fitness = 0.0;

    // initializing mutation rates

    new_genome->mutation_rates[0] = POINT_MUTATION_RATE;
    new_genome->mutation_rates[1] = LINK_MUTATION_RATE;
    new_genome->mutation_rates[2] = NODE_MUTATION_RATE;
    new_genome->mutation_rates[3] = ENABLE_DISABLE_MUTATION_RATE;

    new_genome->global_rank = 0;
    new_genome->innovation = innovation;
    new_genome->nb_mutations = 0;
    new_genome->max_neurons = 0;

    return new_genome;
}

/*!
* \brief Clone the given Genome
* \param[in] genome the Genome to clone
* \return Return a new Genome, or NULL if error
*/
void * cloneGenome(void * genome) {
    GenericList * new_neurons_list = NULL;
    GenericList * new_connection_genes_list = NULL;
    Genome * new_genome = NULL;

    if ((new_genome = malloc(sizeof(Genome))) == (Genome * ) NULL) {
        fprintf(stderr, "Error while allocating memory for new Genome\n");
        return NULL;
    }

    new_neurons_list = cloneGenericList(((Genome *) genome)->neurons);
    new_connection_genes_list = cloneGenericList(((Genome *) genome)->connection_genes);

    new_genome->neurons = new_neurons_list;
    new_genome->nb_neurons = ((Genome *) genome)->nb_neurons;
    new_genome->connection_genes = new_connection_genes_list;
    new_genome->nb_connection_genes = ((Genome *) genome)->nb_connection_genes;
    new_genome->fitness = 0.0;

    // initializing mutation rates

    new_genome->mutation_rates[0] = ((Genome *) genome)->mutation_rates[0];
    new_genome->mutation_rates[1] = ((Genome *) genome)->mutation_rates[1];
    new_genome->mutation_rates[2] = ((Genome *) genome)->mutation_rates[2];
    new_genome->mutation_rates[3] = ((Genome *) genome)->mutation_rates[3];

    new_genome->global_rank = 0;
    new_genome->innovation = ((Genome *) genome)->innovation;
    new_genome->nb_mutations = 0;
    new_genome->max_neurons = 0;

    return new_genome;
}

/*!
* \brief Free the given Genome
*/
void freeGenome(void * genome) {
    freeGenericList(((Genome *) genome)->neurons);
    freeGenericList(((Genome *) genome)->connection_genes);
    free(genome);
}

/*!
* \brief Generate a Genome by creating its network elements
* \param[out] genome the Genome to generate
* \return int Return 1 if the Genome was successfully generated, 0 otherwise
*/
int generateGenome(Genome * genome) {
    int i;

    setOnFirstElement(genome->neurons);
    while (!outOfGenericList(genome->neurons)) {
        delete(genome->neurons, getCurrent(genome->neurons));
        nextElement(genome->neurons);
    }

    // populating neuron list

    for (i = 0; i < N_INPUTS; ++i)
        if (!addNeuronToGenome(genome, newNeuron(INPUT)))
            return 0;

    for (i = 0; i < N_OUTPUTS; ++i)
        if (!addNeuronToGenome(genome, newNeuron(OUTPUT)))
            return 0;

    //TODO

    return 1;
}

/*!
* \brief Add the given Neuron to the given Genome
* \param[out] genome the Genome to modify
* \param[in] neuron the Neuron to insert
* \return int 1 if the Neuron was succesfully inserted, 0 otherwise
*/
int addNeuronToGenome(Genome * genome, Neuron * neuron) {
    if (!addNeuron(genome->neurons, neuron)) {
        freeNeuron(neuron);
        return 0;
    }

    ++genome->nb_neurons;

    return 1;
}

int addConnectionGeneToGenome(Genome * genome, Neuron * neuron_1, Neuron * neuron_2, ConnectionGene * connection_gene) {
    if (!addConnectionGeneToNeurons(neuron_1, neuron_2, connection_gene))
        return 0;

    ++genome->nb_connection_genes;

    return 1;
}

/*!
* \brief Disturb the mutation rates of the given Genome, by increasing or decreasing them.
* \param[out] genome The Genome whose mutation rates have to be updated
*/
static void disturbMutationRates(Genome * genome) {
    int i;

    int count = sizeof(genome->mutation_rates) / sizeof(float);

    for (i = 0; i < count; ++i) {
        if (random01() < DECREASE_PROBABILITY)
            genome->mutation_rates[i] *= 0.95;
        else
            genome->mutation_rates[i] *= 1.05;
    }
}

/*!
* \brief Mutate the given Genome.
* \param[out] genome The Genome to mutate
* \return int 1 if the mutation was successful, 0 otherwise
*
* A Genome has a mutation rate for each type of mutation that can occur.
* These mutation rates are also randomly increased or decreased as the evolution progresses.
*/
int mutate(Genome * genome) {
    if (genome->nb_mutations == N_MAX_MUTATIONS) {
        fprintf(stderr, "Can't perform mutation: reached limit (max=%d)\n", N_MAX_MUTATIONS);
        return 0;
    }

    disturbMutationRates(genome);

    /*
        genome->mutation_rates[0] = POINT_MUTATION_RATE;
        genome->mutation_rates[1] = LINK_MUTATION_RATE;
        genome->mutation_rates[2] = NODE_MUTATION_RATE;
        genome->mutation_rates[3] = ENABLE_DISABLE_MUTATION_RATE;
    */

    double r = genome->mutation_rates[0];
    while (r > 0) {
    if (random01() < r)
        mutatePoint(genome);

        r -= 1.0;
    }

    r = genome->mutation_rates[1];
    while (r > 0) {
    if (random01() < r)
        mutateLink(genome);

        r -= 1.0;
    }

    r = genome->mutation_rates[2];
    while (r > 0) {
    if (random01() < r)
        mutateNode(genome);

        r -= 1.0;
    }

    r = genome->mutation_rates[3];
    while (r > 0) {
    if (random01() < r)
        mutateEnableFlag(genome, 0);

        r -= 1.0;
    }

    return 1;
}

/*!
* \brief Randomly update the weight of a randomly selected ConnectionGene from the given Genome
* \param[out] genome The Genome whose a random ConnectionGene has to be updated
* \return int 1 if a ConnectionGene was successfully updated, 0 otherwise
*
* The updates are either:
* New Weight = Old Weight +/- Random number between 0 and pointMutationRate
* or
* New Weight = Random number between -2 and 2
*/
int mutatePoint(Genome * genome) {
    ConnectionGene * current_connection_gene = NULL;

    setOnFirstElement(genome->connection_genes);
    while (!outOfGenericList(genome->connection_genes)) {
        current_connection_gene = (ConnectionGene *) getCurrent(genome->connection_genes);

        if (random01() < POINT_MUTATION_PERTURBATION)
            current_connection_gene->weight += 2.0 * random01() * POINT_MUTATION_RATE - POINT_MUTATION_RATE;
        else
            current_connection_gene->weight = 4.0 * random01() - 2.0;

        nextElement(genome->connection_genes);
    }
}

/*!
* \brief
* \param[out] genome
* \return int
*/
int mutateLink(Genome * genome) {
    Neuron * neuron_1 = NULL;
    Neuron * neuron_2 = NULL;
    ConnectionGene * connection_gene = NULL;

    if (count(genome->neurons) <= 1)
        return 1;

    neuron_1 = getRandomNeuron(genome);
    neuron_2 = getRandomNeuron(genome);

    /*
    while (neuron_1 == neuron_2)
        neuron_2 = getRandomNeuron(genome);
    */

    connection_gene = newConnectionGene(0, 1, *genome->innovation);

    if (connection_gene == NULL)
        return 0;


    if (!addConnectionGeneToGenome(genome, neuron_1, neuron_2, connection_gene))
        return 0;

    genome->mutations_history[genome->nb_mutations] = 1;
    ++genome->nb_mutations;

    if (linked(neuron_1, neuron_2))
        return 1;

    ++(*genome->innovation);

    connection_gene->weight = 4.0 * random01() - 2.0;
    connection_gene->innovation = *genome->innovation;

    return 1;
}

/*!
* \brief
* \param[out] genome
* \return int 1 if the mutation was successful, 0 otherwise
*
* This mutation adds a new Neuron to the network by disabling a ConnectionGene,
* replacing it with a ConnectionGene of weight 1, a Neuron and a ConnectionGene with
* the same weight as the disabled ConnectionGene.
*
* In essence it’s been replaced with an identically functioning equivalent.
*/
int mutateNode(Genome * genome) {
    ConnectionGene * connection_gene = NULL;

    ConnectionGene * connection_gene_1 = NULL;
    ConnectionGene * connection_gene_2 = NULL;

    if (emptyGenericList(genome->connections))
        return 0;

    connection_gene = getRandomConnectionGene(genome);
    if (!connection_gene->enabled)
        return 0;
    connection_gene->enabled = 0;

    genome->mutations_history[genome->nb_mutations] = 2;
    ++genome->nb_mutations;
    ++genome->max_neurons;

    ++(*genome->innovation);

    connection_gene_1 = cloneConnectionGene(connection_gene);
    connection_gene_1->neuron_out_id = genome->max_neurons;
    connection_gene_1->weight = 1.0;
    connection_gene_1->enabled = 1;
    connection_gene_1->innovation = *genome->innovation;
    add(genome->connection_genes, connection_gene_1);

    ++(*genome->innovation);

    connection_gene_2 = cloneConnectionGene(connection_gene);
    connection_gene_2->neuron_in_id = genome->max_neurons;
    connection_gene_2->weight = 1.0;
    connection_gene_2->enabled = 1;
    connection_gene_2->innovation = *genome->innovation;
    add(genome->connection_genes, connection_gene_2);
}

/*!
* \brief Randomly enable and disable ConnectionGene elements from the given Genome
* \param[out] genome the Genome whose ConnectionGene elements have to be updated
* \param[in] enable
* \return int 1 if the ConnectionGene elements were successfully updated, 0 otherwise
*/
int mutateEnableFlag(Genome * genome, unsigned char enable) {
  int i = 0;
  int random_connection_gene_index;
  Neuron * current_neuron = NULL;
  ConnectionGene * current_connection_gene = NULL;
  ConnectionGene * candidates[N_MAX_NEURONS * N_MAX_CONNECTION_GENES];

  setOnFirstElement(genome->neurons);
  while (!outOfGenericList(genome->neurons)) {

    current_neuron = (Neuron *) getCurrent(genome->neurons);

    if (current_neuron == NULL)
      return 0;

    setOnFirstElement(current_neuron->connection_genes);
    while (!outOfGenericList(current_neuron->connection_genes)) {

      current_connection_gene = (ConnectionGene *) getCurrent(current_neuron->connection_genes);

      if (current_connection_gene == NULL)
        return 0;

      if (current_connection_gene->enabled != enable) {
        candidates[i] = current_connection_gene;
        ++i;
      }

      nextElement(current_neuron->connection_genes);
    }

    nextElement(genome->neurons);
  }

  if (i == 0)
    return 1;

  random_connection_gene_index = randomLimit(i - 1);
  candidates[random_connection_gene_index]->enabled = !candidates[random_connection_gene_index]->enabled;

  genome->mutations_history[genome->nb_mutations] = 3;
  ++genome->nb_mutations;

  return 1;
}

/*!
* \brief Create a new Genome resulting of a crossover between the given Genome elements.
* \param[in] genome_1 the first Genome to cross over
* \param[in] genome_2 the second Genome to cross over
* \return a new Genome resulting of a crossover between the given Genome elements, NULL otherwise
*/
Genome * crossover(Genome * genome_1, Genome * genome_2) {
  int i;
  Genome * child = NULL;

  Neuron * current_neuron_1 = NULL;
  Neuron * current_neuron_2 = NULL;

  ConnectionGene * current_connection_gene_1 = NULL;
  ConnectionGene * current_connection_gene_2 = NULL;

  Neuron * new_neuron_1 = NULL;
  Neuron * new_neuron_2 = NULL;

  ConnectionGene * new_connection_gene = NULL;

  int break_inner_loop = 0;

  Genome * genome_temp = NULL;

  if ((child = newGenome(genome_1->innovation)) == (Genome *) NULL)
    return NULL;

  // genome_1 has to have a greater fitness than genome_2

  if (genome_2->fitness > genome_1->fitness) {
    genome_temp = genome_1;
    genome_1 = genome_2;
    genome_2 = genome_temp;
  }

  // first connection gene

  setOnFirstElement(genome_1->neurons);
  outer_loop: while (!outOfGenericList(genome_1->neurons)) {

    current_neuron_1 = (Neuron *) getCurrent(genome_1->neurons);
    if (current_neuron_1 == (Neuron *) NULL) {
      fprintf(stderr, "Error in crossover function: current Neuron of genome 1 is NULL\n");
      return NULL;
    }

    setOnFirstElement(current_neuron_1->connection_genes);
    while (!outOfGenericList(current_neuron_1->connection_genes)) {

      current_connection_gene_1 = (ConnectionGene *) getCurrent(current_neuron_1->connection_genes);
      if (current_connection_gene_1 == (ConnectionGene *) NULL) {
        fprintf(stderr, "Error in crossover function: current ConnectionGene of genome 1 is NULL\n");
        return NULL;
      }

      // second connection gene

      setOnFirstElement(genome_2->neurons);
      while (!outOfGenericList(genome_2->neurons)) {

        current_neuron_2 = (Neuron *) getCurrent(genome_2->neurons);
        if (current_neuron_2 == (Neuron *) NULL) {
          fprintf(stderr, "Error in crossover function: current Neuron of genome 2 is NULL\n");
          return NULL;
        }

        setOnFirstElement(current_neuron_2->connection_genes);
        while (!outOfGenericList(current_neuron_2->connection_genes)) {

          current_connection_gene_2 = (ConnectionGene *) getCurrent(current_neuron_2->connection_genes);
          if (current_connection_gene_2 == (ConnectionGene *) NULL) {
            fprintf(stderr, "Error in crossover function: current ConnectionGene of genome 2 is NULL\n");
            return NULL;
          }

          // we finally have current_connection_gene_1 and current_connection_gene_2

          if (current_connection_gene_1->innovation == current_connection_gene_2->innovation) {
            if (randomBool() && current_connection_gene_2->enabled) {
              new_neuron_1 = cloneNeuronWithoutConnections(current_connection_gene_2->neuron_in_id);
              new_neuron_2 = cloneNeuronWithoutConnections(current_connection_gene_2->neuron_out_id);

              new_connection_gene = cloneConnectionGene(current_connection_gene_2);
              addConnectionGeneToGenome(child, new_neuron_1, new_neuron_2, new_connection_gene);

              nextElement(genome_1->neurons);
              goto outer_loop;
            }
            else
              break_inner_loop = 1;
          }

          if (break_inner_loop) {
            setOnFirstElement(current_neuron_2->connection_genes);
            while (!outOfGenericList(current_neuron_2->connection_genes))
                nextElement(current_neuron_2->connection_genes);
          } else
            nextElement(current_neuron_2->connection_genes);
        }

        if (break_inner_loop) {
            setOnFirstElement(genome_2->neurons);
            while (!outOfGenericList(genome_2->neurons))
                nextElement(genome_2->neurons);

            break_inner_loop = 0;
        } else
            nextElement(genome_2->neurons);
      }

      new_neuron_1 = cloneNeuronWithoutConnections(current_connection_gene_1->neuron_in_id);
      new_neuron_2 = cloneNeuronWithoutConnections(current_connection_gene_1->neuron_out_id);

      new_connection_gene = cloneConnectionGene(current_connection_gene_1);
      addConnectionGeneToGenome(child, new_neuron_1, new_neuron_2, new_connection_gene);

      nextElement(current_neuron_1->connection_genes);
    }

    nextElement(genome_1->neurons);
  }

  // finally, we're copying mutation rates from genome_1, which has the greater fitness

  for (i = 0; i < 4; ++i)
    child->mutation_rates[i] = genome_1->mutation_rates[i];

  return child;
}

static double computeWeights(Genome * genome_1, Genome * genome_2, int verbose) {
  double sum = 0.0;
  double sameInnovation = 0.0;

  Neuron * current_neuron_1 = NULL;
  Neuron * current_neuron_2 = NULL;

  ConnectionGene * current_connection_gene_1 = NULL;
  ConnectionGene * current_connection_gene_2 = NULL;

  // first connection gene

  setOnFirstElement(genome_1->neurons);
  while (!outOfGenericList(genome_1->neurons)) {

    current_neuron_1 = (Neuron *) getCurrent(genome_1->neurons);
    if (current_neuron_1 == NULL) {
      fprintf(stderr, "Error in computeWeights function: current Neuron of genome 1 is NULL\n");
      return -DBL_MAX;
    }

    setOnFirstElement(current_neuron_1->connection_genes);
    while (!outOfGenericList(current_neuron_1->connection_genes)) {

      current_connection_gene_1 = (ConnectionGene *) getCurrent(current_neuron_1->connection_genes);
      if (current_connection_gene_1 == NULL) {
        fprintf(stderr, "Error in computeWeights function: current ConnectionGene of genome 1 is NULL\n");
        return -DBL_MAX;
      }

      // second connection gene

      setOnFirstElement(genome_2->neurons);
      while (!outOfGenericList(genome_2->neurons)) {

        current_neuron_2 = (Neuron *) getCurrent(genome_2->neurons);
        if (current_neuron_2 == NULL) {
          fprintf(stderr, "Error in computeWeights function: current Neuron of genome 2 is NULL\n");
          return -DBL_MAX;
        }

        setOnFirstElement(current_neuron_2->connection_genes);
        while (!outOfGenericList(current_neuron_2->connection_genes)) {

          current_connection_gene_2 = (ConnectionGene *) getCurrent(current_neuron_2->connection_genes);
          if (current_connection_gene_2 == NULL) {
            fprintf(stderr, "Error in computeWeights function: current ConnectionGene of genome 2 is NULL\n");
            return -DBL_MAX;
          }

          // we finally have current_connection_gene_1 and current_connection_gene_2

          if (current_connection_gene_1->innovation == current_connection_gene_2->innovation) {
            sum += fabs(current_connection_gene_1->weight - current_connection_gene_2->weight);
            sameInnovation += 1.0;
          }

          nextElement(current_neuron_2->connection_genes);
        }

        nextElement(genome_2->neurons);
      }

      nextElement(current_neuron_1->connection_genes);
    }

    nextElement(genome_1->neurons);
  }

  if (verbose) {
    printf("computeWeights\n");
    printf("sum: %f\n", sum);
    printf("sameInnovation: %f\n", sameInnovation);
    printf("res: %f\n\n", sum / sameInnovation);
  }

  return sum / sameInnovation;
}

static double computeDisjoint(Genome * genome_1, Genome * genome_2, int verbose) {
  double disjoint_count = 0.0;

  Neuron * current_neuron_1 = NULL;
  Neuron * current_neuron_2 = NULL;

  ConnectionGene * current_connection_gene_1 = NULL;
  ConnectionGene * current_connection_gene_2 = NULL;

  // first connection gene

  setOnFirstElement(genome_1->neurons);
  while (!outOfGenericList(genome_1->neurons)) {

    current_neuron_1 = (Neuron *) getCurrent(genome_1->neurons);
    if (current_neuron_1 == NULL) {
      fprintf(stderr, "Error in computeDisjoint function: current Neuron of genome 1 is NULL\n");
      return -DBL_MAX;
    }

    setOnFirstElement(current_neuron_1->connection_genes);
    while (!outOfGenericList(current_neuron_1->connection_genes)) {

      current_connection_gene_1 = (ConnectionGene *) getCurrent(current_neuron_1->connection_genes);
      if (current_connection_gene_1 == NULL) {
        fprintf(stderr, "Error in computeDisjoint function: current ConnectionGene of genome 1 is NULL\n");
        return -DBL_MAX;
      }

      // second connection gene

      setOnFirstElement(genome_2->neurons);
      while (!outOfGenericList(genome_2->neurons)) {

        current_neuron_2 = (Neuron *) getCurrent(genome_2->neurons);
        if (current_neuron_2 == NULL) {
          fprintf(stderr, "Error in computeDisjoint function: current Neuron of genome 2 is NULL\n");
          return -DBL_MAX;
        }

        setOnFirstElement(current_neuron_2->connection_genes);
        while (!outOfGenericList(current_neuron_2->connection_genes)) {

          current_connection_gene_2 = (ConnectionGene *) getCurrent(current_neuron_2->connection_genes);
          if (current_connection_gene_2 == NULL) {
            fprintf(stderr, "Error in computeDisjoint function: current ConnectionGene of genome 2 is NULL\n");
            return -DBL_MAX;
          }

          // we finally have current_connection_gene_1 and current_connection_gene_2

          if (current_connection_gene_1->innovation == current_connection_gene_2->innovation)
            disjoint_count += 1.0;

          nextElement(current_neuron_2->connection_genes);
        }

        nextElement(genome_2->neurons);
      }

      nextElement(current_neuron_1->connection_genes);
    }

    nextElement(genome_1->neurons);
  }

  if (verbose) {
    printf("computeDisjoint\n");
    printf("disjoint_count: %f\n", disjoint_count);
    printf("max: %f\n", fmax(genome_1->nb_connection_genes, genome_2->nb_connection_genes));
    printf("res: %f\n\n", disjoint_count / fmax(genome_1->nb_connection_genes, genome_2->nb_connection_genes));
  }

  return disjoint_count / fmax(genome_1->nb_connection_genes, genome_2->nb_connection_genes);
}

/*!
* \brief Check if the given Genome elements are from the same Species
* \param[out] genome_1 the first Genome to compare
* \param[out] genome_2 the second Genome to compare
* \return int 1 if the two Genome elements are from the same Species, 0 otherwise
*/
double sameSpecies(Genome * genome_1, Genome * genome_2) {
    double coef = WEIGHT_COEFFICIENT * computeWeights(genome_1, genome_2, 0) + DISJOINT_COEFFICIENT * computeDisjoint(genome_1, genome_2, 0) < SPECIATION_THRESHOLD;
    return coef;
}

/*!
* \brief Check if the two given Neuron elements are linked by a ConnectionGene
* \param[out] neuron_in The first Neuron
* \param[in] neuron_out The second Neuron
* \return int 1 if the two Neuron elements are linked, 0 otherwise
*/
int linked(Neuron * neuron_in, Neuron * neuron_out) {
  setOnFirstElement(neuron_in->connection_genes);
  while (!outOfGenericList(neuron_in->connection_genes)) {
    if (((ConnectionGene *) getCurrent(neuron_in->connection_genes))->neuron_out_id == neuron_out)
      return 1;

    nextElement(neuron_in->connection_genes);
  }

  return 0;
}

double * evaluateGenome(Genome * genome, double * input) {
  int i;
  double sum;
  double * output = NULL;

  Neuron * current_neuron = NULL;
  Neuron * current_neuron_out = NULL;

  ConnectionGene * current_connection_gene = NULL;

  // updating values of the neurons from the given inputs

  for (i = 0; i < N_INPUTS; ++i) {
    setOn(genome->neurons, i);
    ((Neuron *) getCurrent(genome->neurons))->value = input[i];
  }

  // scanning input and output neurons

  setOnFirstElement(genome->neurons);
  while (!outOfGenericList(genome->neurons)) {
    current_neuron = (Neuron *) getCurrent(genome->neurons);

    if (current_neuron->id >= N_INPUTS + N_OUTPUTS) {

      sum = 0.0;

      setOnFirstElement(current_neuron->connection_genes);
      while (!outOfGenericList(current_neuron->connection_genes)) {
        current_connection_gene = (ConnectionGene *) getCurrent(current_neuron->connection_genes);
        current_neuron_out = current_connection_gene->neuron_out_id;

        sum += current_connection_gene->weight * current_neuron_out->value;

        nextElement(current_neuron->connection_genes);
      }

      if (!emptyGenericList(current_neuron->connection_genes))
        current_neuron->value = fast_sigmoid(sum);
    }

    nextElement(genome->neurons);
  }

  // scanning output neurons

  setOnFirstElement(genome->neurons);
  while (!outOfGenericList(genome->neurons)) {
    current_neuron = (Neuron *) getCurrent(genome->neurons);

    if (current_neuron->id >= N_INPUTS && current_neuron->id < N_INPUTS + N_OUTPUTS) {

      sum = 0.0;

      setOnFirstElement(current_neuron->connection_genes);
      while (!outOfGenericList(current_neuron->connection_genes)) {
        current_connection_gene = (ConnectionGene *) getCurrent(current_neuron->connection_genes);
        current_neuron_out = current_connection_gene->neuron_out_id;

        sum += current_connection_gene->weight * current_neuron_out->value;

        nextElement(current_neuron->connection_genes);
      }

      if (!emptyGenericList(current_neuron->connection_genes))
        current_neuron->value = fast_sigmoid(sum);
    }

    nextElement(genome->neurons);
  }

  // returning output

  if ((output = malloc(sizeof(double) * N_OUTPUTS)) == NULL)
    return NULL;

  for (i = 0; i < N_OUTPUTS; ++i) {
    setOn(genome->neurons, N_INPUTS + i);
    output[i] = ((Neuron *) getCurrent(genome->neurons))->value;
  }

  return output;
}

/*!
* \brief Return a random Neuron from the given Genome.
* \param[in] genome the Genome to choose a Neuron from
* \return Return a random Neuron
*/
Neuron * getRandomNeuron(Genome * genome) {
    setOn(genome->neurons, randomLimit(genome->nb_neurons - 1));
    return ((Neuron *) getCurrent(genome->neurons));
}

/*!
* \brief Return a random ConnectionGene from the given Genome.
* \param[in] genome the Genome to choose a ConnectionGene from
* \return Return a random ConnectionGene
*/
ConnectionGene * getRandomConnectionGene(Genome * genome) {
    setOn(genome->connections, randomLimit(genome->nb_connection_genes - 1));
    return ((ConnectionGene *) getCurrent(genome->connections));
}

/*!
* \brief Write a the Network of a Genome to an output file, based on graphviz.
* \param[in] genome the Genome to write
* \param[in] filename the name of the output file
* \return int 1 if the file was successfully written, 0 otherwise
*/
int writeGraphVizGenome(Genome * genome, char * filename) {
  FILE * f = NULL;
  GenericList * connection_gene_successors = NULL;
  Neuron * current_neuron = NULL;
  ConnectionGene * current_connection_gene = NULL;

  if ((f = (FILE *) fopen(filename, "w")) == (FILE *) NULL) {
      fprintf(stderr, "Error while opening %s\n", filename);
      return 0;
  }

  fprintf(f, "digraph {\n");

  // links

  setOnFirstElement(genome->neurons);
  while (!outOfGenericList(genome->neurons)) {
      connection_gene_successors = ((Neuron *) genome->neurons->current->data)->connection_genes;

      if (emptyGenericList(connection_gene_successors))
        fprintf(f, "\t%d;\n", ((Neuron *) genome->neurons->current->data)->id);
      else {
        setOnFirstElement(connection_gene_successors);
        while (!outOfGenericList(connection_gene_successors)) {
          current_connection_gene = (ConnectionGene *) connection_gene_successors->current->data;

          if (current_connection_gene->enabled)
            fprintf(f, "\t%d -> %d [label=\"%.1f\\n%d\", weight=%.1f];\n", ((Neuron *) genome->neurons->current->data)->id, current_connection_gene->neuron_out_id->id, current_connection_gene->weight, current_connection_gene->innovation, current_connection_gene->weight);
          else
            fprintf(f, "\t%d -> %d [label=\"%.1f\\n%d\", weight=%.1f color=red];\n", ((Neuron *) genome->neurons->current->data)->id, current_connection_gene->neuron_out_id->id, current_connection_gene->weight, current_connection_gene->innovation, current_connection_gene->weight);

          nextElement(connection_gene_successors);
        }
      }

      nextElement(genome->neurons);
  }

  // nodes

  setOnFirstElement(genome->neurons);
  while(!outOfGenericList(genome->neurons)) {
    current_neuron = (Neuron *) genome->neurons->current->data;

    if (current_neuron->type == INPUT)
      fprintf(f, "\t%d [label=\"%d\\n%.1f\", shape=circle, style=filled, fillcolor=yellow];\n", current_neuron->id, current_neuron->id, current_neuron->value);

    else if (current_neuron->type == OUTPUT)
      fprintf(f, "\t%d [label=\"%d\\n%.1f\", shape=circle, style=filled, fillcolor=red];\n", current_neuron->id, current_neuron->id, current_neuron->value);

    else if (current_neuron->type == BASIC)
      fprintf(f, "\t%d [label=\"%d\\n%.1f\", shape=circle];\n", current_neuron->id, current_neuron->id, current_neuron->value);

    else
      fprintf(f, "\t%d [shape=circle];\n", current_neuron->id);

    nextElement(genome->neurons);
  }

  fprintf(f, "}");
  fclose(f);

  return 1;
}

/*!
* \brief Print the given Genome.
* \param[in] genome the Genome to print
*/
void printGenome(Genome * genome) {
  int k;

  printf("----------------------------------\n");
  printf("\t%p\n", genome);

  printf("\t\tnb_neurons: %d\n", genome->nb_neurons);
  printf("\t\tnb_connection_genes: %d\n", genome->nb_connection_genes);
  printf("\n");

  printf("\tMutation rates:\n");

  printf("\t\tPOINT_MUTATION_RATE: %f\n", genome->mutation_rates[0]);
  printf("\t\tLINK_MUTATION_RATE: %f\n", genome->mutation_rates[1]);
  printf("\t\tNODE_MUTATION_RATE: %f\n", genome->mutation_rates[2]);
  printf("\t\tENABLE_DISABLE_MUTATION_RATE: %f\n", genome->mutation_rates[3]);

  printf("\n");
  printf("\tMutations history: ");

  for (k = 0; k < genome->nb_mutations; ++k)
    printf("%d ", genome->mutations_history[k]);

  printf("\n");
}
