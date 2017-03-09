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

  return new_genome;
}

/*!
* \brief Generate a genome by creating its network elements
* \param[in] genome the genome to generate
* \return Return 0 if success, -1 otherwise
*/
int generateGenome(Genome * genome) {
  int i;

  /*
  for (i = 0; i < INPUTS; ++i) {
    if (addToNetwork(genome, newNetworkElement(i, newNeuron())) == -1)
      return -1;
  }
  for (i = 0; i < OUTPUTS; ++i) {
    if (addToNetwork(genome, newNetworkElement(INPUTS + i, newNeuron())) == -1)
      return -1;
  }
  */

  return 0;
}

/*!
* \brief Check if a genome contains the network element with the specified id
* \param[in] genome the genome to check
* \param[in] id the id of the network element to check
* \return Return 1 if found, 0 otherwise
*/
int genomeHasNetworkElement(Genome * genome, short int id) {
  /*
  int i;

  for (i = 0; i < genome->nb_network_elements; ++i) {
    if (genome->network[i].id == id)
      return 1;
  }
  */

  return 0;
}

/*!
* \brief Return a random neuron from the genome
* \param[in] genome the genome to choose a neuron from
* \return Return a random neuron
*/
Neuron * getRandomNeuron(Genome * genome) {
  /*
  if (genome->nb_network_elements == 0)
    return NULL;

  return genome->network[randomAtMost(genome->nb_network_elements - 1)].neuron;
  */

  return NULL;
}
