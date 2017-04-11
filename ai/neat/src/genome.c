#include "genome.h"

/*!
* \brief Create a Genome
* \return Return a new Genome, or NULL if error
*/
Genome * newGenome(int * innovation) {
  Network * new_network = NULL;
  Genome * new_genome = NULL;

  if ((new_genome = malloc(sizeof(Genome))) == (Genome * ) NULL) {
    fprintf(stderr, "Error while allocating memory for new Genome\n");
    return NULL;
  }

  new_network = newList(freeNeuron);
  initList(new_network);

  new_genome->network = new_network;
  new_genome->nb_neurons = 0;
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

  return new_genome;
}

/*!
* \brief Free the given Genome
*/
void freeGenome(void * genome) {
  freeList(((Genome *) genome)->network);
  free(genome);
}

/*!
* \brief Generate a Genome by creating its network elements
* \param[out] genome the Genome to generate
* \return int Return 1 if the Genome was successfully generated, 0 otherwise
*/
int generateGenome(Genome * genome) {
  int i;

  // populating neuron list

  for (i = 0; i < N_INPUTS; ++i)
    if (!addNeuronToGenome(genome, newNeuron(INPUT)))
      return 0;

  for (i = 0; i < N_BIAS; ++i)
    if (!addNeuronToGenome(genome, newNeuron(BIAS)))
      return 0;

  for (i = 0; i < N_OUTPUTS; ++i)
    if (!addNeuronToGenome(genome, newNeuron(OUTPUT)))
      return 0;

  return 1;
}

/*!
* \brief Add the given Neuron to the given Genome
* \param[out] genome the Genome to modify
* \param[in] neuron the Neuron to insert
* \return int 1 if the Neuron was succesfully inserted, 0 otherwise
*/
int addNeuronToGenome(Genome * genome, Neuron * neuron) {
  if (!addNeuronToNetwork(genome->network, neuron)) {
    freeNeuron(neuron);
    return 0;
  }

  ++genome->nb_neurons;

  return 1;
}

int addConnectionGeneToGenome(Genome * genome, Neuron * neuron_1, Neuron * neuron_2, ConnectionGene * connection_gene) {
  if (!addConnectionGeneToNeurons(neuron_1, neuron_2, connection_gene)) {
    return 0;
  }

  ++genome->nb_connection_genes;

  return 1;
}

/*!
* \brief Disturb the mutation rates of the given Genome, by increasing or decreasing them
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
* \brief Mutate the given Genome
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
  Neuron * current_neuron = NULL;
  ConnectionGene * current_connection_gene = NULL;

  setOnFirst(genome->network);
  while (!outOfList(genome->network)) {

    current_neuron = (Neuron *) getCurrent(genome->network);

    if (current_neuron == NULL)
      return 0;

    setOnFirst(current_neuron->connections);
    while (!outOfList(current_neuron->connections)) {

      current_connection_gene = (ConnectionGene *) getCurrent(current_neuron->connections);

      if (current_connection_gene == NULL)
        return 0;

      if (random01() < POINT_MUTATION_PERTURBATION)
        current_connection_gene->weight += 2.0 * random01() * POINT_MUTATION_RATE - POINT_MUTATION_RATE;
      else
        current_connection_gene->weight = 4.0 * random01() - 2.0;

      next(current_neuron->connections);
    }

    next(genome->network);
  }

  genome->mutations_history[genome->nb_mutations] = 0;
  ++genome->nb_mutations;

  return 1;
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

  if (count(genome->network) <= 1)
    return 1;

  neuron_1 = getRandomNeuron(genome);
  neuron_2 = getRandomNeuron(genome);
  while (neuron_1 == neuron_2)
    neuron_2 = getRandomNeuron(genome);

  if (linked(neuron_1, neuron_2))
    return 1;

  connection_gene = newConnectionGene(4.0 * random01() - 2.0, 1, *genome->innovation);

  if (connection_gene == NULL)
    return 0;

  ++(*genome->innovation);
  if (!addConnectionGeneToGenome(genome, neuron_1, neuron_2, connection_gene)) {
    --(*genome->innovation);
    return 0;
  }

  genome->mutations_history[genome->nb_mutations] = 1;
  ++genome->nb_mutations;

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
  int i = 0;
  int random_connection_gene_index;
  Neuron * current_neuron = NULL;
  ConnectionGene * current_connection_gene = NULL;
  ConnectionGene * candidates[N_MAX_NEURONS * N_MAX_CONNECTION_GENES];

  Neuron * new_neuron = NULL;
  ConnectionGene * new_connection_gene_1 = NULL;
  ConnectionGene * new_connection_gene_2 = NULL;

  setOnFirst(genome->network);
  while (!outOfList(genome->network)) {

    current_neuron = (Neuron *) getCurrent(genome->network);

    if (current_neuron == NULL)
      return 0;

    setOnFirst(current_neuron->connections);
    while (!outOfList(current_neuron->connections)) {

      current_connection_gene = (ConnectionGene *) getCurrent(current_neuron->connections);

      if (current_connection_gene == NULL)
        return 0;

      candidates[i] = current_connection_gene;
      ++i;

      next(current_neuron->connections);
    }

    next(genome->network);
  }

  if (i == 0)
    return 1;

  random_connection_gene_index = randomAtMost(i);

  if (candidates[random_connection_gene_index]->enabled == 0)
    return 1;

  candidates[random_connection_gene_index]->enabled = 0;

  new_neuron = newNeuron(BASIC);
  if (!addNeuronToGenome(genome, new_neuron))
    return 0;

  new_connection_gene_1 = cloneConnectionGene(candidates[random_connection_gene_index]);

  new_connection_gene_1->weight = 1.0;
  new_connection_gene_1->enabled = 1;

  ++(*genome->innovation);
  if (!addConnectionGeneToGenome(genome, new_connection_gene_1->neuron_in, new_neuron, new_connection_gene_1)) {
    --(*genome->innovation);
    return 0;
  }

  new_connection_gene_2 = cloneConnectionGene(candidates[random_connection_gene_index]);

  new_connection_gene_2->enabled = 1;

  ++(*genome->innovation);
  if (!addConnectionGeneToGenome(genome, new_neuron, new_connection_gene_2->neuron_out, new_connection_gene_2)) {
    --(*genome->innovation);
    return 0;
  }

  genome->mutations_history[genome->nb_mutations] = 2;
  ++genome->nb_mutations;

  return 1;
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

  setOnFirst(genome->network);
  while (!outOfList(genome->network)) {

    current_neuron = (Neuron *) getCurrent(genome->network);

    if (current_neuron == NULL)
      return 0;

    setOnFirst(current_neuron->connections);
    while (!outOfList(current_neuron->connections)) {

      current_connection_gene = (ConnectionGene *) getCurrent(current_neuron->connections);

      if (current_connection_gene == NULL)
        return 0;

      if (current_connection_gene->enabled != enable) {
        candidates[i] = current_connection_gene;
        ++i;
      }

      next(current_neuron->connections);
    }

    next(genome->network);
  }

  if (i == 0)
    return 1;

  random_connection_gene_index = randomAtMost(i);
  candidates[random_connection_gene_index]->enabled = !candidates[random_connection_gene_index]->enabled;

  genome->mutations_history[genome->nb_mutations] = 3;
  ++genome->nb_mutations;

  return 1;
}

Genome * crossover(Genome * genome_1, Genome * genome_2) {
  Genome * genome = NULL;



  return genome;
}

static double computeWeights(Genome * genome_1, Genome * genome_2, int verbose) {
  double sum = 0.0;
  double sameInnovation = 0.0;

  Neuron * current_neuron_1 = NULL;
  Neuron * current_neuron_2 = NULL;

  ConnectionGene * current_connection_gene_1 = NULL;
  ConnectionGene * current_connection_gene_2 = NULL;

  // first connection gene

  setOnFirst(genome_1->network);
  while (!outOfList(genome_1->network)) {

    current_neuron_1 = (Neuron *) getCurrent(genome_1->network);
    if (current_neuron_1 == NULL) {
      fprintf(stderr, "Error in computeWeights function: current Neuron of genome 1 is NULL\n");
      return -DBL_MAX;
    }

    setOnFirst(current_neuron_1->connections);
    while (!outOfList(current_neuron_1->connections)) {

      current_connection_gene_1 = (ConnectionGene *) getCurrent(current_neuron_1->connections);
      if (current_connection_gene_1 == NULL) {
        fprintf(stderr, "Error in computeWeights function: current ConnectionGene of genome 1 is NULL\n");
        return -DBL_MAX;
      }

      // second connection gene

      setOnFirst(genome_2->network);
      while (!outOfList(genome_2->network)) {

        current_neuron_2 = (Neuron *) getCurrent(genome_2->network);
        if (current_neuron_2 == NULL) {
          fprintf(stderr, "Error in computeWeights function: current Neuron of genome 2 is NULL\n");
          return -DBL_MAX;
        }

        setOnFirst(current_neuron_2->connections);
        while (!outOfList(current_neuron_2->connections)) {

          current_connection_gene_2 = (ConnectionGene *) getCurrent(current_neuron_2->connections);
          if (current_connection_gene_2 == NULL) {
            fprintf(stderr, "Error in computeWeights function: current ConnectionGene of genome 2 is NULL\n");
            return -DBL_MAX;
          }

          // we finally have current_connection_gene_1 and current_connection_gene_2

          if (current_connection_gene_1->innovation == current_connection_gene_2->innovation) {
            sum += fabs(current_connection_gene_1->weight - current_connection_gene_2->weight);
            sameInnovation += 1.0;
          }

          next(current_neuron_2->connections);
        }

        next(genome_2->network);
      }

      next(current_neuron_1->connections);
    }

    next(genome_1->network);
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

  setOnFirst(genome_1->network);
  while (!outOfList(genome_1->network)) {

    current_neuron_1 = (Neuron *) getCurrent(genome_1->network);
    if (current_neuron_1 == NULL) {
      fprintf(stderr, "Error in computeDisjoint function: current Neuron of genome 1 is NULL\n");
      return -DBL_MAX;
    }

    setOnFirst(current_neuron_1->connections);
    while (!outOfList(current_neuron_1->connections)) {

      current_connection_gene_1 = (ConnectionGene *) getCurrent(current_neuron_1->connections);
      if (current_connection_gene_1 == NULL) {
        fprintf(stderr, "Error in computeDisjoint function: current ConnectionGene of genome 1 is NULL\n");
        return -DBL_MAX;
      }

      // second connection gene

      setOnFirst(genome_2->network);
      while (!outOfList(genome_2->network)) {

        current_neuron_2 = (Neuron *) getCurrent(genome_2->network);
        if (current_neuron_2 == NULL) {
          fprintf(stderr, "Error in computeDisjoint function: current Neuron of genome 2 is NULL\n");
          return -DBL_MAX;
        }

        setOnFirst(current_neuron_2->connections);
        while (!outOfList(current_neuron_2->connections)) {

          current_connection_gene_2 = (ConnectionGene *) getCurrent(current_neuron_2->connections);
          if (current_connection_gene_2 == NULL) {
            fprintf(stderr, "Error in computeDisjoint function: current ConnectionGene of genome 2 is NULL\n");
            return -DBL_MAX;
          }

          // we finally have current_connection_gene_1 and current_connection_gene_2

          if (current_connection_gene_1->innovation == current_connection_gene_2->innovation)
            disjoint_count += 1.0;

          next(current_neuron_2->connections);
        }

        next(genome_2->network);
      }

      next(current_neuron_1->connections);
    }

    next(genome_1->network);
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
int sameSpecies(Genome * genome_1, Genome * genome_2) {
  return WEIGHT_COEFFICIENT * computeWeights(genome_1, genome_2, 0) + DISJOINT_COEFFICIENT * computeDisjoint(genome_1, genome_2, 0) < SPECIATION_THRESHOLD;
}

/*!
* \brief Check if the two given Neuron elements are linked by a ConnectionGene
* \param[out] neuron_in The first Neuron
* \param[in] neuron_out The second Neuron
* \return int 1 if the two Neuron elements are linked, 0 otherwise
*/
int linked(Neuron * neuron_in, Neuron * neuron_out) {
  setOnFirst(neuron_in->connections);
  while (!outOfList(neuron_in->connections)) {
    if (((ConnectionGene *) getCurrent(neuron_in->connections))->neuron_out == neuron_out)
      return 1;

    next(neuron_in->connections);
  }

  return 0;
}

/*!
* \brief Return a random Neuron from the given Genome.
* \param[in] genome the Genome to choose a Neuron from
* \return Return a random Neuron
*/
Neuron * getRandomNeuron(Genome * genome) {
  setOn(genome->network, randomAtMost(count(genome->network) - 1));
  return ((Neuron *) getCurrent(genome->network));
}

/*!
* \brief Write a the Network of a Genome to an output file, based on graphviz.
* \param[in] genome the Genome to write
* \param[in] filename the name of the output file
* \return int 1 if the file was successfully written, 0 otherwise
*/
int writeGraphVizGenome(Genome * genome, char * filename) {
  FILE * f = NULL;
  List * connection_gene_successors = NULL;
  Neuron * current_neuron = NULL;
  ConnectionGene * current_connection_gene = NULL;

  if ((f = (FILE *) fopen(filename, "w")) == (FILE *) NULL) {
      fprintf(stderr, "Error while opening %s\n", filename);
      return 0;
  }

  fprintf(f, "digraph {\n");

  // links

  setOnFirst(genome->network);
  while (!outOfList(genome->network)) {
      connection_gene_successors = ((Neuron *) genome->network->current->data)->connections;

      if (emptyList(connection_gene_successors))
        fprintf(f, "\t%d;\n", ((Neuron *) genome->network->current->data)->id);
      else {
        setOnFirst(connection_gene_successors);
        while (!outOfList(connection_gene_successors)) {
          current_connection_gene = (ConnectionGene *) connection_gene_successors->current->data;

          if (current_connection_gene->enabled)
            fprintf(f, "\t%d -> %d [label=\"%.1f\\n%d\", weight=%.1f];\n", ((Neuron *) genome->network->current->data)->id, current_connection_gene->neuron_out->id, current_connection_gene->weight, current_connection_gene->innovation, current_connection_gene->weight);
          else
            fprintf(f, "\t%d -> %d [label=\"%.1f\\n%d\", weight=%.1f color=red];\n", ((Neuron *) genome->network->current->data)->id, current_connection_gene->neuron_out->id, current_connection_gene->weight, current_connection_gene->innovation, current_connection_gene->weight);

          next(connection_gene_successors);
        }
      }

      next(genome->network);
  }

  // nodes

  setOnFirst(genome->network);
  while(!outOfList(genome->network)) {
    current_neuron = (Neuron *) genome->network->current->data;

    if (current_neuron->type == INPUT)
      fprintf(f, "\t%d [label=\"%d\\n%.1f\", shape=circle, style=filled, fillcolor=yellow];\n", current_neuron->id, current_neuron->id, current_neuron->value);

    else if (current_neuron->type == OUTPUT)
      fprintf(f, "\t%d [label=\"%d\\n%.1f\", shape=circle, style=filled, fillcolor=red];\n", current_neuron->id, current_neuron->id, current_neuron->value);

    else if (current_neuron->type == BIAS)
      fprintf(f, "\t%d [label=\"%d\\n%.1f\", shape=circle, style=filled, fillcolor=orange];\n", current_neuron->id, current_neuron->id, current_neuron->value);

    else if (current_neuron->type == BASIC)
      fprintf(f, "\t%d [label=\"%d\\n%.1f\", shape=circle];\n", current_neuron->id, current_neuron->id, current_neuron->value);

    else
      fprintf(f, "\t%d [shape=circle];\n", current_neuron->id);

    next(genome->network);
  }

  fprintf(f, "}");
  fclose(f);

  return 1;
}
