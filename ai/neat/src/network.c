#include "network.h"

/*!
* \brief Create a Neuron.
* \param[in] type The NeuronType of the Neuron to be created
* \return Return a new Neuron, or NULL if error
*/
Neuron * newNeuron(NeuronType type) {
    GenericList * new_connection_genes_list = NULL;
    Neuron * new_neuron = NULL;

    if ((new_neuron = (Neuron *) malloc(sizeof(Neuron))) == (Neuron *) NULL) {
        fprintf(stderr, "Error while allocating memory for new Neuron\n");
        return NULL;
    }

    if ((new_connection_genes_list = newGenericList(cloneConnectionGene, freeConnectionGene)) == (GenericList *) NULL)
        return NULL;

    initGenericList(new_connection_genes_list);
    new_neuron->connection_genes_input = new_connection_genes_list;

    new_neuron->id = -1;
    new_neuron->type = type;
    new_neuron->value = 0.0;

    return new_neuron;
}

/*!
* \brief Clone the given Neuron.
* \param[in] neuron The Neuron to be cloned
* \return Return a new Neuron, or NULL if error
*/
void * cloneNeuron(void * neuron) {
    GenericList * new_connection_genes_list = NULL;
    Neuron * new_neuron = NULL;

    if ((new_neuron = (Neuron *) malloc(sizeof(Neuron))) == (Neuron *) NULL) {
        fprintf(stderr, "Error while allocating memory for new Neuron\n");
        return NULL;
    }

    if ((new_connection_genes_list = cloneGenericList(((Neuron *) neuron)->connection_genes_input)) == (GenericList *) NULL)
        return NULL;

    new_neuron->connection_genes_input = new_connection_genes_list;

    new_neuron->id = ((Neuron *) neuron)->id;
    new_neuron->type = ((Neuron *) neuron)->type;
    new_neuron->value = ((Neuron *) neuron)->value;

    return new_neuron;
}

Neuron * cloneNeuronWithoutConnections(Neuron * neuron) {
    GenericList * new_connection_genes_list = NULL;
    Neuron * new_neuron = NULL;

    if ((new_neuron = (Neuron *) malloc(sizeof(Neuron))) == (Neuron *) NULL) {
        fprintf(stderr, "Error while allocating memory for new Neuron\n");
        return NULL;
    }

    if ((new_connection_genes_list = newGenericList(cloneConnectionGene, freeConnectionGene)) == (GenericList *) NULL)
        return NULL;

    initGenericList(new_connection_genes_list);
    new_neuron->connection_genes_input = new_connection_genes_list;

    new_neuron->id = ((Neuron *) neuron)->id;
    new_neuron->type = ((Neuron *) neuron)->type;
    new_neuron->value = ((Neuron *) neuron)->value;

    return new_neuron;
}

/*!
* \brief Delete the specified Neuron.
* \param[out] neuron the Neuron to be freed
*/
void freeNeuron(void * neuron) {
    //freeGenericList(((Neuron *) neuron)->connection_genes);
    free(neuron);
}

/*!
* \brief Add a Neuron to the Network.
* \param[out] network the Network
* \param[out] neuron the Neuron to add
* \return int 1 if the Neuron was successfully added, 0 otherwise
*/
int addNeuron(GenericList * neurons, Neuron * neuron) {
    int nb_neurons = count(neurons);
    if (nb_neurons == N_MAX_NEURONS) {
        fprintf(stderr, "Can't add neuron to network : reached limit (max=%d)\n", N_MAX_NEURONS);
        return 0;
    }

    neuron->id = nb_neurons;

    return add(neurons, neuron);
}

/*!
* \brief Create a ConnectionGene.
* \param[in] weight the weight
* \param[in] enabled the enabled/disabled flag
* \param[in] innovation the innovation number
* \return Return a ConnectionGene, NULL if error
*/
ConnectionGene * newConnectionGene(double weight, unsigned char enabled, int innovation) {
    ConnectionGene * new_connection_gene = NULL;

    if ((new_connection_gene = (ConnectionGene *) malloc(sizeof(ConnectionGene))) == (ConnectionGene *) NULL) {
        fprintf(stderr, "Error while allocating memory for new ConnectionGene\n");
        return NULL;
    }

    new_connection_gene->weight = weight;
    new_connection_gene->enabled = enabled;

    new_connection_gene->neuron_in_id = 0;
    new_connection_gene->neuron_out_id = 0;

    new_connection_gene->innovation = innovation;

    return new_connection_gene;
}

/*!
* \brief Create a ConnectionGene by copying the attributes of the given ConnectionGene.
* \param[in] connection_gene The ConnectionGene whose atributes have to be copied
* \return Return a ConnectionGene, NULL if error
*/
void * cloneConnectionGene(void * connection_gene) {
    ConnectionGene * new_connection_gene = NULL;

    if ((new_connection_gene = (ConnectionGene *) malloc(sizeof(ConnectionGene))) == (ConnectionGene *) NULL) {
        fprintf(stderr, "Error while allocating memory for new ConnectionGene\n");
        return NULL;
    }

    new_connection_gene->weight = ((ConnectionGene *) connection_gene)->weight;
    new_connection_gene->enabled = ((ConnectionGene *) connection_gene)->enabled;

    new_connection_gene->neuron_in_id = ((ConnectionGene *) connection_gene)->neuron_in_id;
    new_connection_gene->neuron_out_id = ((ConnectionGene *) connection_gene)->neuron_out_id;

    new_connection_gene->innovation = ((ConnectionGene *) connection_gene)->innovation;

    return new_connection_gene;
}

/*!
* \brief Delete the specified ConnectionGene.
* \param[out] connection_gene the ConnectionGene to be freed
*/
void freeConnectionGene(void * connection_gene) {
    free(connection_gene);
}
