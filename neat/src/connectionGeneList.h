/*!
* \file connectionGeneList.h
* \brief File containing structures and prototypes for the genome
*/
#ifndef CONNECTION_GENE_LIST_H
#define CONNECTION_GENE_LIST_H

#include <stdio.h>
#include <stdlib.h>

#include "network.h"

void initConnectionGeneList(ConnectionGeneList * connection_gene_list);
void deleteConnectionGeneList(ConnectionGeneList * connection_gene_list);

int emptyConnectionGeneList(ConnectionGeneList * connection_gene_list);
int outOfConnectionGeneList(ConnectionGeneList * connection_gene_list);

void setOnFirstConnectionGene(ConnectionGeneList * connection_gene_list);
void nextConnectionGene(ConnectionGeneList * connection_gene_list);

ConnectionGene * getCurrentConnectionGene(ConnectionGeneList * connection_gene_list);

int addConnectionGene(ConnectionGeneList * connection_gene_list, ConnectionGene * connection_gene);
int deleteConnectionGene(ConnectionGeneList * connection_gene_list, ConnectionGene * connection_gene);

int findConnectionGene(ConnectionGeneList * connection_gene_list, ConnectionGene * connection_gene);
int countConnectionGenes(ConnectionGeneList * connection_gene_list);

#endif // CONNECTION_GENE_LIST_H
