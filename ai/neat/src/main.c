#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "network.h"
#include "genome.h"
#include "population.h"

int main() {
  int i;
  time_t t;

  MatingPool * pool = NULL;
  Genome * current_genome = NULL;

  srand((unsigned) time(&t));
  pool = newMatingPool();

  if (pool == (MatingPool *) NULL) {
    fprintf(stderr, "Error\n");
    return EXIT_FAILURE;
  }

  populateMatingPool(pool);

  /* new level */

  for (i = 0; i < pool->nb_species; ++i) {

    setOnFirst(pool->species[i].genomes);
    while (!outOfList(pool->species[i].genomes)) {
      generateGenome(getCurrent(pool->species[i].genomes));

      next(pool->species[i].genomes);
    }
  }

  setOnFirst(pool->species[0].genomes);
  current_genome = getCurrent(pool->species[0].genomes);

  printf("nb_neurons: %d\n", current_genome->nb_neurons);
  printf("nb_connection_genes: %d\n", current_genome->nb_connection_genes);
  printf("Adding 3 connection genes\n");

  ConnectionGene * connection_gene_1 = newConnectionGene(34, 2, pool->innovation);
  ConnectionGene * connection_gene_2 = newConnectionGene(12, 4, pool->innovation);
  ConnectionGene * connection_gene_3 = newConnectionGene(45, 5, pool->innovation);

  if (connection_gene_1 == (ConnectionGene *) NULL) {
    fprintf(stderr, "Error\n");
    return EXIT_FAILURE;
  }

  if (connection_gene_2 == (ConnectionGene *) NULL) {
    fprintf(stderr, "Error\n");
    return EXIT_FAILURE;
  }

  if (connection_gene_3 == (ConnectionGene *) NULL) {
    fprintf(stderr, "Error\n");
    return EXIT_FAILURE;
  }

  if (!addConnectionGeneToGenome(current_genome, current_genome->network->first->data, current_genome->network->last->data, connection_gene_1)) {
    fprintf(stderr, "Error\n");
    return EXIT_FAILURE;
  }

  setOn(current_genome->network, 2);

  if (!addConnectionGeneToGenome(current_genome, (Neuron *) getCurrent(current_genome->network), current_genome->network->last->data, connection_gene_2)) {
    fprintf(stderr, "Error\n");
    return EXIT_FAILURE;
  }

  setOn(current_genome->network, 3);

  if (!addConnectionGeneToGenome(current_genome, (Neuron *) getCurrent(current_genome->network), current_genome->network->last->data, connection_gene_3)) {
    fprintf(stderr, "Error\n");
    return EXIT_FAILURE;
  }

  printf("nb_neurons: %d\n", current_genome->nb_neurons);
  printf("nb_connection_genes: %d\n", current_genome->nb_connection_genes);

  mutate(current_genome);
  mutate(current_genome);
  mutate(current_genome);
  mutate(current_genome);
  mutate(current_genome);
  mutate(current_genome);
  mutate(current_genome);
  mutate(current_genome);
  mutate(current_genome);
  mutate(current_genome);
  mutate(current_genome);
  mutate(current_genome);
  mutate(current_genome);
  mutate(current_genome);
  mutate(current_genome);

  printf("nb_neurons: %d\n", current_genome->nb_neurons);
  printf("nb_connection_genes: %d\n", current_genome->nb_connection_genes);

  writeGraphVizGenome(current_genome, "graph.dot");

  freeMatingPool(pool);

  return EXIT_SUCCESS;
}
