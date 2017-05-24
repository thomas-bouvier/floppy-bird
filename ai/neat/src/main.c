#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "network.h"
#include "genome.h"
#include "population.h"

int main() {
    int i;
    time_t t;

    MatingPool * pool = NULL;

    srand((unsigned) time(&t));
    pool = newMatingPool();

    if (pool == (MatingPool *) NULL) {
        fprintf(stderr, "Error\n");
        return EXIT_FAILURE;
    }

    if (!populateMatingPool(pool))
        return EXIT_FAILURE;

    /* generating genomes */

    for (i = 0; i < pool->nb_species; ++i) {
        setOnFirstElement(pool->species[i].genomes);
        while (!outOfGenericList(pool->species[i].genomes)) {
            generateGenome(getCurrent(pool->species[i].genomes));

            nextElement(pool->species[i].genomes);
        }
    }

    freeMatingPool(pool);

    return EXIT_SUCCESS;
}
