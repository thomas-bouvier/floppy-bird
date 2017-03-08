#include "node.h"

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

  new_neuron->value = 0.0;

  return new_neuron;
}
