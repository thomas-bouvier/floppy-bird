#include "genome.h"

/*!
* \brief Generate a Genome by creating its network elements
* \param[out] genome the Genome to generate
* \return int Return 1 if the Genome was successfully generated, 0 otherwise
*/
int generateGenome(Genome * genome) {
  int i;
  Neuron * neuron = NULL;

  // populating neuron list

  for (i = 0; i < N_INPUTS; ++i) {
    neuron = newNeuron(INPUT);
    if (!addNeuronToNetwork(genome->network, neuron)) {
      freeNeuron(neuron);
      return 0;
    }
  }

  for (i = 0; i < N_BIAS; ++i) {
    neuron = newNeuron(BIAS);
    if (!addNeuronToNetwork(genome->network, neuron)) {
      freeNeuron(neuron);
      return 0;
    }
  }

  for (i = 0; i < N_OUTPUTS; ++i) {
    neuron = newNeuron(OUTPUT);
    if (!addNeuronToNetwork(genome->network, neuron)) {
      freeNeuron(neuron);
      return 0;
    }
  }

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

  setOnFirstNeuron(genome->network);
  while (!outOfNeuronList(genome->network)) {

    current_neuron = getCurrentNeuron(genome->network);

    if (current_neuron == NULL)
      return 0;

    setOnFirstConnectionGene(current_neuron->connections);
    while (!outOfConnectionGeneList(current_neuron->connections)) {

      current_connection_gene = getCurrentConnectionGene(current_neuron->connections);

      if (current_connection_gene == NULL)
        return 0;

      if (random01() < POINT_MUTATION_PERTURBATION)
        current_connection_gene->weight += 2.0 * random01() * POINT_MUTATION_RATE - POINT_MUTATION_RATE;
      else
        current_connection_gene->weight = 4.0 * random01() - 2.0;

      nextConnectionGene(current_neuron->connections);
    }

    nextNeuron(genome->network);
  }

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

  if (countNeurons(genome->network) <= 1)
    return 1;

  neuron_1 = getRandomNeuron(genome);
  neuron_2 = getRandomNeuron(genome);
  while (neuron_1 == neuron_2)
    neuron_2 = getRandomNeuron(genome);

  if (linked(neuron_1, neuron_2))
    return 1;

  connection_gene = newConnectionGene(4.0 * random01() - 2.0, 1, genome->innovation);

  if (connection_gene == NULL)
    return 0;

  ++(*genome->innovation);
  addConnectionGeneToNeurons(neuron_1, neuron_2, connection_gene);

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

  setOnFirstNeuron(genome->network);
  while (!outOfNeuronList(genome->network)) {

    current_neuron = getCurrentNeuron(genome->network);

    if (current_neuron == NULL)
      return 0;

    setOnFirstConnectionGene(current_neuron->connections);
    while (!outOfConnectionGeneList(current_neuron->connections)) {

      current_connection_gene = getCurrentConnectionGene(current_neuron->connections);

      if (current_connection_gene == NULL)
        return 0;

      candidates[i] = current_connection_gene;
      ++i;

      nextConnectionGene(current_neuron->connections);
    }

    nextNeuron(genome->network);
  }

  if (i == 0)
    return 1;

  random_connection_gene_index = randomAtMost(i);

  if (candidates[random_connection_gene_index]->enabled == 0)
    return 1;

  candidates[random_connection_gene_index]->enabled = 0;

  new_neuron = newNeuron(BASIC);
  if (!addNeuronToNetwork(genome->network, new_neuron)) {
    freeNeuron(new_neuron);
    return 0;
  }

  new_connection_gene_1 = cloneConnectionGene(candidates[random_connection_gene_index]);

  new_connection_gene_1->weight = 1.0;
  new_connection_gene_1->enabled = 1;

  ++(*genome->innovation);
  addConnectionGeneToNeurons(new_connection_gene_1->neuron_in, new_neuron, new_connection_gene_1);

  new_connection_gene_2 = cloneConnectionGene(candidates[random_connection_gene_index]);

  new_connection_gene_2->enabled = 1;

  ++(*genome->innovation);
  addConnectionGeneToNeurons(new_neuron, new_connection_gene_2->neuron_out, new_connection_gene_2);

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

  setOnFirstNeuron(genome->network);
  while (!outOfNeuronList(genome->network)) {

    current_neuron = getCurrentNeuron(genome->network);

    if (current_neuron == NULL)
      return 0;

    setOnFirstConnectionGene(current_neuron->connections);
    while (!outOfConnectionGeneList(current_neuron->connections)) {

      current_connection_gene = getCurrentConnectionGene(current_neuron->connections);

      if (current_connection_gene == NULL)
        return 0;

      if (current_connection_gene->enabled != enable) {
        candidates[i] = current_connection_gene;
        ++i;
      }

      nextConnectionGene(current_neuron->connections);
    }

    nextNeuron(genome->network);
  }

  if (i == 0)
    return 1;

  random_connection_gene_index = randomAtMost(i);
  candidates[random_connection_gene_index]->enabled = !candidates[random_connection_gene_index]->enabled;

  return 1;
}

Genome * crossover(Genome * genome_1, Genome * genome_2) {
  Genome * genome = NULL;



  return genome;
}

/*!
* \brief Check if the two given Neuron elements are linked by a ConnectionGene
* \param[out] neuron_in The first Neuron
* \param[in] neuron_out The second Neuron
* \return int 1 if the two Neuron elements are linked, 0 otherwise
*/
int linked(Neuron * neuron_in, Neuron * neuron_out) {
  setOnFirstConnectionGene(neuron_in->connections);
  while (!outOfConnectionGeneList(neuron_in->connections)) {
    if (getCurrentConnectionGene(neuron_in->connections)->neuron_out == neuron_out)
      return 1;

    nextConnectionGene(neuron_in->connections);
  }

  return 0;
}

/*!
* \brief Return a random Neuron from the given Genome.
* \param[in] genome the Genome to choose a Neuron from
* \return Return a random Neuron
*/
Neuron * getRandomNeuron(Genome * genome) {
  setOnNeuron(genome->network, randomAtMost(countNeurons(genome->network) - 1));
  return getCurrentNeuron(genome->network);
}

/*!
* \brief Write a the Network of a Genome to an output file, based on graphviz.
* \param[in] genome the Genome to write
* \param[in] filename the name of the output file
* \return int 1 if the file was successfully written, 0 otherwise
*/
int writeGraphVizGenome(Genome * genome, char * filename) {
  FILE * f = NULL;
  ConnectionGeneList * connection_gene_successors = NULL;

  if ((f = (FILE *) fopen(filename, "w")) == (FILE *) NULL) {
      fprintf(stderr, "Error while opening %s\n", filename);
      return 0;
  }

  fprintf(f, "digraph {\n");

  // nodes

  setOnFirstNeuron(genome->network);
  while (!outOfNeuronList(genome->network)) {
      connection_gene_successors = genome->network->current->connections;
      setOnFirstConnectionGene(connection_gene_successors);

      if (emptyConnectionGeneList(connection_gene_successors))
        fprintf(f, "\t%d;\n", genome->network->current->id);

      else {
        while (!outOfConnectionGeneList(connection_gene_successors)) {
          if (connection_gene_successors->current->enabled)
            fprintf(f, "\t%d -> %d [label=\"%.1f\", weight=%.1f];\n", genome->network->current->id, connection_gene_successors->current->neuron_out->id, connection_gene_successors->current->weight, connection_gene_successors->current->weight);
          else
            fprintf(f, "\t%d -> %d [label=\"%.1f\", weight=%.1f color=red];\n", genome->network->current->id, connection_gene_successors->current->neuron_out->id, connection_gene_successors->current->weight, connection_gene_successors->current->weight);

          nextConnectionGene(connection_gene_successors);
        }
      }

      nextNeuron(genome->network);
  }

  // colors

  setOnFirstNeuron(genome->network);
  while(!outOfNeuronList(genome->network)) {
    if (genome->network->current->type == INPUT)
      fprintf(f, "\t%d [label=\"%d\\n%.1f\", shape=circle, style=filled, fillcolor=yellow];\n", genome->network->current->id, genome->network->current->id, genome->network->current->value);

    else if (genome->network->current->type == OUTPUT)
      fprintf(f, "\t%d [label=\"%d\\n%.1f\", shape=circle, style=filled, fillcolor=red];\n", genome->network->current->id, genome->network->current->id, genome->network->current->value);

    else if (genome->network->current->type == BIAS)
      fprintf(f, "\t%d [label=\"%d\\n%.1f\", shape=circle, style=filled, fillcolor=orange];\n", genome->network->current->id, genome->network->current->id, genome->network->current->value);

    else if (genome->network->current->type == BASIC)
      fprintf(f, "\t%d [label=\"%d\\n%.1f\", shape=circle];\n", genome->network->current->id, genome->network->current->id, genome->network->current->value);

    else
      fprintf(f, "\t%d [shape=circle];\n", genome->network->current->id);

    nextNeuron(genome->network);
  }

  fprintf(f, "}");
  fclose(f);

  return 1;
}
