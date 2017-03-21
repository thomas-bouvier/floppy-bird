#include <stdio.h>
#include <stdlib.h>

#include "network.h"
#include "genome.h"
#include "population.h"

int main() {
  int i;
  int j;
  MatingPool * pool = NULL;

  pool = newMatingPool();

  if (pool == (MatingPool *) NULL) {
    fprintf(stderr, "Error\n");
    return EXIT_FAILURE;
  }

  populateMatingPool(pool);

  /* new level */

  for (i = 0; i < pool->nb_species; ++i) {
    for (j = 0; j < pool->species[i].nb_genomes; ++j) {
      generateGenome(&(pool->species[i].genomes[j]));
    }
  }

  ConnectionGene * connection_gene_1 = newConnectionGene(34, 2, 1);
  ConnectionGene * connection_gene_2 = newConnectionGene(12, 4, 0);

  if (connection_gene_1 == (ConnectionGene *) NULL) {
    fprintf(stderr, "Error\n");
    return EXIT_FAILURE;
  }

  if (connection_gene_2 == (ConnectionGene *) NULL) {
    fprintf(stderr, "Error\n");
    return EXIT_FAILURE;
  }

  if (!addConnectionGeneToNeurons(pool->species[0].genomes[0].network->first, pool->species[0].genomes[0].network->last, connection_gene_1)) {
    fprintf(stderr, "Error\n");
    return EXIT_FAILURE;
  }

  setOnNeuron(pool->species[0].genomes[0].network, 2);

  if (!addConnectionGeneToNeurons(getCurrentNeuron(pool->species[0].genomes[0].network), pool->species[0].genomes[0].network->last, connection_gene_2)) {
    fprintf(stderr, "Error\n");
    return EXIT_FAILURE;
  }

  writeGraphVizGenome(&(pool->species[0].genomes[0]), "graph.dot");

  freeMatingPool(pool);

  return EXIT_SUCCESS;
}
