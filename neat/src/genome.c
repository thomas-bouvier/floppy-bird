#include "genome.h"

/*!
* \brief Create a connection gene
* \param[in] input the input node
* \param[in] output the output node
* \param[in] weight the weight
* \param[in] innovation the innovation number
* \param[in] enabled the enabled/disabled flag
* \return Return a connection gene, NULL if error
*/
ConnectionGene * newConnectionGene(short int input, short int output, double weight, short int innovation, unsigned char enabled) {
  ConnectionGene * new_connection_gene = (ConnectionGene *) malloc(sizeof(ConnectionGene));

  if (new_connection_gene == (ConnectionGene *) NULL) {
    fprintf(stderr, "Error while allocating memory for new ConnectionGene\n");
    return NULL;
  }

  new_connection_gene->input = input;
  new_connection_gene->output = output;
  new_connection_gene->weight = weight;
  new_connection_gene->innovation = innovation;
  new_connection_gene->enabled = enabled;

  return new_connection_gene;
}

/*!
* \brief Create a neuron
* \return Return a neuron, NULL if error
*/
Neuron * newNeuron() {
  Neuron * new_neuron = (Neuron *) malloc(sizeof(Neuron));

  if (new_neuron == (Neuron *) NULL) {
    fprintf(stderr, "Error while allocating memory for Neuron\n");
    return NULL;
  }

  new_neuron->nb_connections = 0;
  new_neuron->value = 0.0;

  return new_neuron;
}

/*!
* \brief Create a network element
* \return Return a newtork element, NULL if error
*/
NetworkElement * newNetworkElement(short int id, Neuron * neuron) {
  NetworkElement * new_network_element = (NetworkElement *) malloc(sizeof(NetworkElement));

  if (new_network_element == (NetworkElement *) NULL) {
    fprintf(stderr, "Error while allocating memory for new NetworkElement\n");
    return NULL;
  }

  new_network_element->id = id;
  new_network_element->neuron = neuron;

  return new_network_element;
}

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

  new_genome->nb_genes = 0;
  new_genome->nb_network_elements = 0;

  return new_genome;
}

/*!
* \brief Generate a genome by creating its network elements
* \param[in] genome the genome to generate
* \return Return 0 if success, -1 otherwise
*/
int generateGenome(Genome * genome) {
  int i;

  for (i = 0; i < INPUTS; ++i) {
    if (addToNetwork(genome, newNetworkElement(i, newNeuron())) == -1)
      return -1;
  }
  for (i = 0; i < OUTPUTS; ++i) {
    if (addToNetwork(genome, newNetworkElement(INPUTS + i, newNeuron())) == -1)
      return -1;
  }

  return 0;
}

/*!
* \brief Add a network element to a genome
* \param[in] genome the genome to modify
* \param[out] network_element the network element to insert
* \return Return 0 if success, -1 otherwise
*/
int addToNetwork(Genome * genome, NetworkElement * network_element) {
  if (genome->nb_network_elements + 1 == N_MAX_NETWORK) {
    fprintf(stderr, "Too many network elements in genome\n");
    return -1;
  }

  genome->network[genome->nb_network_elements] = *network_element;
  genome->nb_network_elements++;

  return 0;
}

/*!
* \brief Check if a genome contains the network element with the specified id
* \param[in] genome the genome to check
* \param[in] id the id of the network element to check
* \return Return 1 if found, 0 otherwise
*/
int genomeHasNetworkElement(Genome * genome, short int id) {
  int i;

  for (i = 0; i < genome->nb_network_elements; ++i) {
    if (genome->network[i].id == id)
      return 1;
  }

  return 0;
}
