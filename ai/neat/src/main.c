#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "network.h"
#include "genome.h"
#include "population.h"

int main() {
  int i;
  int j;
  time_t t;
  MatingPool * pool = NULL;

  srand((unsigned) time(&t));
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

  printf("nb_neurons: %d\n", pool->species[0].genomes[0].nb_neurons);
  printf("nb_connection_genes: %d\n", pool->species[0].genomes[0].nb_connection_genes);
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

  if (!addConnectionGeneToGenome(&pool->species[0].genomes[0], pool->species[0].genomes[0].network->first->data, pool->species[0].genomes[0].network->last->data, connection_gene_1)) {
    fprintf(stderr, "Error\n");
    return EXIT_FAILURE;
  }

  setOn(pool->species[0].genomes[0].network, 2);

  if (!addConnectionGeneToGenome(&pool->species[0].genomes[0], (Neuron *) getCurrent(pool->species[0].genomes[0].network), pool->species[0].genomes[0].network->last->data, connection_gene_2)) {
    fprintf(stderr, "Error\n");
    return EXIT_FAILURE;
  }

  setOn(pool->species[0].genomes[0].network, 3);

  if (!addConnectionGeneToGenome(&pool->species[0].genomes[0], (Neuron *) getCurrent(pool->species[0].genomes[0].network), pool->species[0].genomes[0].network->last->data, connection_gene_3)) {
    fprintf(stderr, "Error\n");
    return EXIT_FAILURE;
  }

  printf("nb_neurons: %d\n", pool->species[0].genomes[0].nb_neurons);
  printf("nb_connection_genes: %d\n", pool->species[0].genomes[0].nb_connection_genes);

  mutate(&pool->species[0].genomes[0]);
  mutate(&pool->species[0].genomes[0]);
  mutate(&pool->species[0].genomes[0]);
  mutate(&pool->species[0].genomes[0]);
  mutate(&pool->species[0].genomes[0]);
  mutate(&pool->species[0].genomes[0]);
  mutate(&pool->species[0].genomes[0]);
  mutate(&pool->species[0].genomes[0]);
  mutate(&pool->species[0].genomes[0]);
  mutate(&pool->species[0].genomes[0]);
  mutate(&pool->species[0].genomes[0]);
  mutate(&pool->species[0].genomes[0]);
  mutate(&pool->species[0].genomes[0]);

  printf("nb_neurons: %d\n", pool->species[0].genomes[0].nb_neurons);
  printf("nb_connection_genes: %d\n", pool->species[0].genomes[0].nb_connection_genes);

  writeGraphVizGenome(&(pool->species[0].genomes[0]), "graph.dot");

  freeMatingPool(pool);

  return EXIT_SUCCESS;
}
