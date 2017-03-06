#include <stdio.h>
#include <stdlib.h>

#include "genome.h"

int main() {
  ConnectionGene * connection_gene;

  short int input = 1;
  short int output = 2;
  double weight = 5.0;
  short int innovation = 0;
  unsigned char enabled = 1;

  connection_gene = newConnectionGene(input, output, weight, innovation, enabled);

  return 0;
}
