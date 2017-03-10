#include "genome.h"

/*!
* \brief Create a genome
* \return Return a genome, NULL if error
*/
Genome * newGenome() {
  Genome * new_genome = (Genome *) malloc(sizeof(Genome));

  if (new_genome == (Genome *) NULL) {
    fprintf(stderr, "Error while allocating memory for new Genome\n");
    return NULL;
  }

  initNetwork(&new_genome->network);

  return new_genome;
}

/*!
* \brief Generate a Genome by creating its network elements
* \param[out] genome the Genome to generate
* \return Return 1 if the Genome was successfully generated, 0 otherwise
*/
int generateGenome(Genome * genome) {
  int i;

  for (i = 0; i < INPUTS; ++i) {
    if (!addNeuronToNetwork(&genome->network, newNeuron(INPUT)))
      return 0;
  }
  for (i = 0; i < OUTPUTS; ++i) {
    if (!addNeuronToNetwork(&genome->network, newNeuron(OUTPUT)))
      return 0;
  }

  return 1;
}

/*!
* \brief Return a random Neuron from the given Genome.
* \param[in] genome the Genome to choose a Neuron from
* \return Return a random Neuron
*/
Neuron * getRandomNeuron(Genome * genome) {
  setOnNeuron(&genome->network, randomAtMost(countNeurons(&genome->network) - 1));
  return getCurrentNeuron(&genome->network);
}

/*!
* \brief Write a the Network of a Genome to an output file, based on graphviz.
* \param[in] genome the Genome to write
* \param[in] filename the name of the output file
* \return int 1 if the file was successfully written, 0 otherwise
*/
int writeGraphVizGenome(Genome * genome, char * filename) {
  FILE * f = NULL;

  if ((f = (FILE *) fopen(filename, "w")) == (FILE *) NULL) {
      fprintf(stderr, "Error while opening %s\n", filename);
      return 0;
  }

  fprintf(f, "digraph {\n");

  // nodes

  setOnFirstNeuron(&genome->network);
  while (!outOfNeuronList(&genome->network)) {
      ConnectionGeneList * connection_gene_successors = &(genome->network.current->connections);
      setOnFirstConnectionGene(connection_gene_successors);

      if (emptyConnectionGeneList(connection_gene_successors))
        fprintf(f, "\t%d;\n", genome->network.current->id);

      else {
        while (!outOfConnectionGeneList(connection_gene_successors)) {
            fprintf(f, "\t%d -> %d;\n", genome->network.current->id, connection_gene_successors->current->neuron->id);
            nextConnectionGene(connection_gene_successors);
        }
      }

      nextNeuron(&genome->network);
  }

  // colors

  setOnFirstNeuron(&genome->network);
  while(!outOfNeuronList(&genome->network)) {
    if (genome->network.current->type == INPUT)
      fprintf(f, "\t%d [shape=circle, style=filled, fillcolor=purple];\n", genome->network.current->id);

    else if (genome->network.current->type == OUTPUT)
      fprintf(f, "\t%d [shape=circle, style=filled, fillcolor=red];\n", genome->network.current->id);

    else
      fprintf(f, "\t%d [shape=circle];\n", genome->network.current->id);

    nextNeuron(&genome->network);
  }

  fprintf(f, "}");
  fclose(f);

  return 1;
}
