#include "network.h"

/*!
* \brief Init the given Network.
* \param[out] network the Network to init
*/
void initNetwork(Network * network) {
  initNeuronList(network);
}

/*!
* \brief Delete the given Network.
* \param[out] network the Network to delete
*/
void deleteNetwork(Network * network) {
  deleteNeuronList(network);
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

  new_neuron->next = NULL;

  new_neuron->id = -1;
  new_neuron->value = 0.0;

  return new_neuron;
}

/*!
* \brief Add a Neuron to the Network.
* \param[out] network the Network
* \param[in] neuron the Neuron to add
* \return int 1 if the Neuron was successfully added, 0 otherwise
*/
int addNeuronToNetwork(Network * network, Neuron * neuron) {
  return addNeuron(network, neuron);
}

/*!
* \brief Create a connection gene
* \param[in] weight the weight
* \param[in] innovation the innovation number
* \param[in] enabled the enabled/disabled flag
* \return Return a connection gene, NULL if error
*/
ConnectionGene * newConnectionGene(double weight, short int innovation, unsigned char enabled) {
  ConnectionGene * new_connection_gene = (ConnectionGene *) malloc(sizeof(ConnectionGene));

  if (new_connection_gene == (ConnectionGene *) NULL) {
    fprintf(stderr, "Error while allocating memory for new ConnectionGene\n");
    return NULL;
  }

  new_connection_gene->weight = weight;
  new_connection_gene->innovation = innovation;
  new_connection_gene->enabled = enabled;

  return new_connection_gene;
}

/*!
* \brief Add two Neuron elements to a ConnectionGene. The first one is the source, the second the destination.
* \param[out] neuron_src the source Neuron
* \param[in] neuron_dst the destination Neuron
* \param[out] connection_gene the ConnectionGene to modify
* \return int 1 if the two Neuron elements were successfully added, 0 otherwise
*/
int addConnectionGeneToNeurons(Neuron * neuron_src, Neuron * neuron_dst, ConnectionGene * connection_gene) {
  if (countConnectionGenes(&neuron_src->connections) + 1 >= N_MAX_CONNECTION_GENES) {
    fprintf(stderr, "Can't add connection gene to neuron : reached limit (%d)\n", N_MAX_CONNECTION_GENES);
    return 0;
  }

  if (!addConnectionGene(neuron_src->connections, connection_gene))
    return 0;

  connection_gene->neuron = neuron_dst;

  return 1;
}
