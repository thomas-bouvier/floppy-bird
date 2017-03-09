/*!
* \file neuronList.h
* \brief File containing structures and prototypes for the genome
*/
#ifndef NEURON_LIST_H
#define NEURON_LIST_H

#include <stdio.h>
#include <stdlib.h>

#include "network.h"

void initNeuronList(NeuronList * neuron_list);
void deleteNeuronList(NeuronList * neuron_list);

int emptyNeuronList(NeuronList * neuron_list);
int outOfNeuronList(NeuronList * neuron_list);

void setOnFirstNeuron(NeuronList * neuron_list);
void nextNeuron(NeuronList * neuron_list);

Neuron * getCurrentNeuron(NeuronList * neuron_list);

int addNeuron(NeuronList * neuron_list, Neuron * neuron);
int deleteNeuron(NeuronList * neuron_list, Neuron * neuron);

int findNeuron(NeuronList * neuron_list, Neuron * neuron);
int countNeurons(NeuronList * neuron_list);

#endif // NEURON_LIST_H
