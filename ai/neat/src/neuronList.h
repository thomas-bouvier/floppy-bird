/*!
* \file neuronList.h
* \brief File containing structures and prototypes for the genome
*/
#ifndef NEURON_LIST_H
#define NEURON_LIST_H

#include <stdio.h>
#include <stdlib.h>

#include "network.h"

struct Neuron;
struct NeuronList;

struct NeuronList * newNeuronList();
void initNeuronList(struct NeuronList * neuron_list);
void freeNeuronList(struct NeuronList * neuron_list);

int emptyNeuronList(struct NeuronList * neuron_list);
int outOfNeuronList(struct NeuronList * neuron_list);

void setOnFirstNeuron(struct NeuronList * neuron_list);
void setOnNeuron(struct NeuronList * neuron_list, int index);
void nextNeuron(struct NeuronList * neuron_list);

struct Neuron * getCurrentNeuron(struct NeuronList * neuron_list);

int addNeuron(struct NeuronList * neuron_list, struct Neuron * neuron);
int deleteNeuron(struct NeuronList * neuron_list, struct Neuron * neuron);

int findNeuron(struct NeuronList * neuron_list, struct Neuron * neuron);
int countNeurons(struct NeuronList * neuron_list);

#endif // NEURON_LIST_H
