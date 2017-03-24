#include "genome.h"

/*!
* \brief Generate a Genome by creating its network elements
* \param[out] genome the Genome to generate
* \return Return 1 if the Genome was successfully generated, 0 otherwise
*/
int generateGenome(Genome * genome) {
  int i;

  for (i = 0; i < N_INPUTS; ++i) {
    if (!addNeuronToNetwork(genome->network, newNeuron(INPUT)))
      return 0;
  }
  for (i = 0; i < N_BIAS; ++i) {
    if (!addNeuronToNetwork(genome->network, newNeuron(BIAS)))
      return 0;
  }
  for (i = 0; i < N_OUTPUTS; ++i) {
    if (!addNeuronToNetwork(genome->network, newNeuron(OUTPUT)))
      return 0;
  }

  return 1;
}

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

  if (i == 0) {
    fprintf(stderr, "There is no ConnectionGene candidate for enable flag mutation\n");
    return 0;
  }

  random_connection_gene_index = randomAtMost(i);
  candidates[random_connection_gene_index]->enabled = !candidates[random_connection_gene_index]->enabled;

  return 1;
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

    else
      fprintf(f, "\t%d [shape=circle];\n", genome->network->current->id);

    nextNeuron(genome->network);
  }

  fprintf(f, "}");
  fclose(f);

  return 1;
}
