#include "network.h"

/*!
* \brief Create a neuron
* \param[in] type The NeuronType of the Neuron to be created
* \return Return a neuron, NULL if error
*/
Neuron * newNeuron(NeuronType type) {
  Neuron * new_neuron = (Neuron *) malloc(sizeof(Neuron));

  if (new_neuron == (Neuron *) NULL) {
    fprintf(stderr, "Error while allocating memory for Neuron\n");
    return NULL;
  }

  new_neuron->next = NULL;

  new_neuron->id = -1;
  new_neuron->type = type;

  if (type == BIAS)
    new_neuron->value = 1.0;
  else
    new_neuron->value = 0.0;

  return new_neuron;
}

/*!
* \brief Delete the specified Neuron.
* \param[out] neuron the Neuron to be freed
*/
void freeNeuron(Neuron * neuron) {
  free(neuron);
}

/*!
* \brief Add a Neuron to the Network.
* \param[out] network the Network
* \param[in] neuron the Neuron to add
* \return int 1 if the Neuron was successfully added, 0 otherwise
*/
int addNeuronToNetwork(Network * network, Neuron * neuron) {
  neuron->id = countNeurons(network);

  return addNeuron(network, neuron);
}

/*!
* \brief Create a ConnectionGene.
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
* \brief Delete the specified ConnectionGene.
* \param[out] connection_gene the ConnectionGene to be freed
*/
void freeConnectionGene(ConnectionGene * connection_gene) {
  free(connection_gene);
}

/*!
* \brief Add two Neuron elements to a ConnectionGene. The first one is the source, the second the destination.
* \param[out] neuron_src the source Neuron
* \param[in] neuron_dst the destination Neuron
* \param[out] connection_gene the ConnectionGene to modify
* \return int 1 if the two Neuron elements were successfully added, 0 otherwise
*/
int addConnectionGeneToNeurons(Neuron * neuron_src, Neuron * neuron_dst, ConnectionGene * connection_gene) {
  int count = countConnectionGenes(&neuron_src->connections);
  if (count == N_MAX_CONNECTION_GENES) {
    fprintf(stderr, "Can't add connection gene to neuron : reached limit (%d, max=%d)\n", count, N_MAX_CONNECTION_GENES);
    return 0;
  }

  if (!addConnectionGene(&neuron_src->connections, connection_gene))
    return 0;

  connection_gene->neuron = neuron_dst;

  return 1;
}
