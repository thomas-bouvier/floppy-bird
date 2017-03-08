#include <stdio.h>
#include <stdlib.h>

#include "population.h"
#include "species.h"
#include "genome.h"

int main() {
  MatingPool * pool = newMatingPool();

  populateMatingPool(pool);

  return 0;
}
