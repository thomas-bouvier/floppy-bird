/*!
* \file connectionGeneList.h
* \brief File containing structures and prototypes for the genome
*/
#ifndef CONNECTION_GENE_LIST_H
#define CONNECTION_GENE_LIST_H

#include <stdio.h>
#include <stdlib.h>

#include "network.h"

struct ConnectionGene;
struct ConnectionGeneList;

void initConnectionGeneList(struct ConnectionGeneList * connection_gene_list);
void deleteConnectionGeneList(struct ConnectionGeneList * connection_gene_list);

int emptyConnectionGeneList(struct ConnectionGeneList * connection_gene_list);
int outOfConnectionGeneList(struct ConnectionGeneList * connection_gene_list);

void setOnFirstConnectionGene(struct ConnectionGeneList * connection_gene_list);
void setOnConnectionGene(struct ConnectionGeneList * connection_gene_list, int index);
void nextConnectionGene(struct ConnectionGeneList * connection_gene_list);

struct ConnectionGene * getCurrentConnectionGene(struct ConnectionGeneList * connection_gene_list);

int addConnectionGene(struct ConnectionGeneList * connection_gene_list, struct ConnectionGene * connection_gene);
int deleteConnectionGene(struct ConnectionGeneList * connection_gene_list, struct ConnectionGene * connection_gene);

int findConnectionGene(struct ConnectionGeneList * connection_gene_list, struct ConnectionGene * connection_gene);
int countConnectionGenes(struct ConnectionGeneList * connection_gene_list);

#endif // CONNECTION_GENE_LIST_H
