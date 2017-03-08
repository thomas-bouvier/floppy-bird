#include <stdio.h>
#include <stdlib.h>

#include "population.h"
#include "species.h"
#include "genome.h"

int main() {
  int i;
  int j;
  MatingPool * pool = NULL;

  pool = newMatingPool();
  populateMatingPool(pool);

  for (i = 0; i < pool->nb_species; ++i) {
    for (j = 0; j < pool->species[i].nb_genomes; ++j) {
      generateGenome(&(pool->species[i].genomes[j]));
    }
  }

  return 0;
}
