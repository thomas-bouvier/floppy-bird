#include "genome.h"

static int compareOutputs(const void * connection_gene_1, const void * connection_gene_2);

/*!
* \brief Create a Genome
* \return Return a new Genome, or NULL if error
*/
Genome * newGenome(int * innovation) {
    GenericList * new_neurons_list = NULL;
    GenericList * new_connection_genes_list = NULL;
    Genome * new_genome = NULL;

    if (innovation == NULL) {
        fprintf(stderr, "Can't create a new Genome without attached innovation number\n");
        return NULL;
    }

    if ((new_genome = malloc(sizeof(Genome))) == (Genome * ) NULL) {
        fprintf(stderr, "Error while allocating memory for new Genome\n");
        return NULL;
    }

    new_neurons_list = newGenericList(NULL, freeNeuron);
    initGenericList(new_neurons_list);

    new_connection_genes_list = newGenericList(cloneConnectionGene, freeConnectionGene);
    initGenericList(new_connection_genes_list);

    new_genome->neurons = new_neurons_list;
    new_genome->connection_genes = new_connection_genes_list;
    new_genome->fitness = 0.0;

    // initializing mutation rates
    new_genome->mutation_rates[0] = POINT_MUTATION_RATE;
    new_genome->mutation_rates[1] = LINK_MUTATION_RATE;
    new_genome->mutation_rates[2] = BIAS_MUTATION_RATE;
    new_genome->mutation_rates[3] = NODE_MUTATION_RATE;
    new_genome->mutation_rates[4] = ENABLE_MUTATION_RATE;
    new_genome->mutation_rates[5] = DISABLE_MUTATION_RATE;

    new_genome->global_rank = 0;
    new_genome->innovation = innovation;
    new_genome->nb_mutations = 0;
    new_genome->max_neurons = 0;

    return new_genome;
}

/*!
* \brief Clone the given Genome
* \param[in] genome the Genome to clone
* \return Return a new Genome, or NULL if error
*/
void * cloneGenome(void * genome) {
    GenericList * new_neurons_list = NULL;
    GenericList * new_connection_genes_list = NULL;
    Genome * new_genome = NULL;

    if ((new_genome = malloc(sizeof(Genome))) == (Genome * ) NULL) {
        fprintf(stderr, "Error while allocating memory for new Genome\n");
        return NULL;
    }

    new_neurons_list = newGenericList(NULL, freeNeuron);
    initGenericList(new_neurons_list);

    new_connection_genes_list = cloneGenericList(((Genome *) genome)->connection_genes);

    new_genome->neurons = new_neurons_list;
    new_genome->connection_genes = new_connection_genes_list;
    new_genome->fitness = 0.0;

    // initializing mutation rates

    new_genome->mutation_rates[0] = ((Genome *) genome)->mutation_rates[0];
    new_genome->mutation_rates[1] = ((Genome *) genome)->mutation_rates[1];
    new_genome->mutation_rates[2] = ((Genome *) genome)->mutation_rates[2];
    new_genome->mutation_rates[3] = ((Genome *) genome)->mutation_rates[3];
    new_genome->mutation_rates[4] = ((Genome *) genome)->mutation_rates[4];
    new_genome->mutation_rates[5] = ((Genome *) genome)->mutation_rates[5];

    new_genome->global_rank = 0;
    new_genome->innovation = ((Genome *) genome)->innovation;
    new_genome->nb_mutations = 0;
    new_genome->max_neurons = ((Genome *) genome)->max_neurons;

    return new_genome;
}

/*!
* \brief Free the given Genome
*/
void freeGenome(void * genome) {
    freeGenericList(((Genome *) genome)->neurons);
    freeGenericList(((Genome *) genome)->connection_genes);
    free(genome);
}

/*!
* \brief Generate a Genome by creating its network elements
* \param[out] genome the Genome to generate
* \return int Return 1 if the Genome was successfully generated, 0 otherwise
*/
int generateGenome(Genome * genome) {
    int i;
    int found_neuron = 0;
    ConnectionGene * current_connection_gene = NULL;
    Neuron * current_neuron = NULL;
    Neuron * new_neuron_1 = NULL;
    Neuron * new_neuron_2 = NULL;

    clearGenericList(genome->neurons);

    // populating neuron list

    for (i = 0; i < N_INPUTS; ++i) {
        if (!addNeuronToGenome(genome, newNeuron(INPUT))) {
            fprintf(stderr, "Can't add new INPUT Neuron to Genome\n");
            return 0;
        }
    }

    for (i = 0; i < N_OUTPUTS; ++i) {
        if (!addNeuronToGenome(genome, newNeuron(OUTPUT))) {
            fprintf(stderr, "Can't add new OUTPUT Neuron to Genome\n");
            return 0;
        }
    }

    /* sorting connection genes */

    sort(genome->connection_genes, compareOutputs);

    /* adding new neurons */

    setOnFirstElement(genome->connection_genes);
    while (!outOfGenericList(genome->connection_genes)) {
        current_connection_gene = (ConnectionGene *) getCurrent(genome->connection_genes);

        if (current_connection_gene->enabled) {
            setOnFirstElement(genome->neurons);
            while (!outOfGenericList(genome->neurons)) {
                current_neuron = (Neuron *) getCurrent(genome->neurons);

                if (current_neuron->id == current_connection_gene->neuron_out_id)
                    found_neuron = 1;

                nextElement(genome->neurons);
            }

            // neuron not found, we're creating it

            if (!found_neuron) {
                new_neuron_1 = newNeuron(BASIC);

                if (!addNeuronToGenome(genome, new_neuron_1)) {
                    /* the Neuron is properly freed */
                    fprintf(stderr, "Can't add new BASIC Neuron to Genome\n");
                    return 0;
                }

                new_neuron_1->id = current_connection_gene->neuron_out_id;
            }

            found_neuron = 0;

            /* we're retrieving the Neuron with the appopriate id in new_neuron */

            setOnFirstElement(genome->neurons);
            while (!outOfGenericList(genome->neurons)) {
                current_neuron = (Neuron *) getCurrent(genome->neurons);

                if (current_neuron->id == current_connection_gene->neuron_out_id)
                    new_neuron_1 = current_neuron;

                nextElement(genome->neurons);
            }

            add(new_neuron_1->connection_genes_input, current_connection_gene);

            setOnFirstElement(genome->neurons);
            while (!outOfGenericList(genome->neurons)) {
                current_neuron = (Neuron *) getCurrent(genome->neurons);

                if (current_neuron->id == current_connection_gene->neuron_in_id)
                    found_neuron = 1;

                nextElement(genome->neurons);
            }

            /* neuron not found, we're creating it */

            if (!found_neuron) {
                new_neuron_2 = newNeuron(BASIC);

                if (!addNeuronToGenome(genome, new_neuron_2)) {
                    /* the Neuron is properly freed */
                    fprintf(stderr, "Can't add new BASIC Neuron to Genome\n");
                    return 0;
                }

                new_neuron_2->id = current_connection_gene->neuron_in_id;
            }

            found_neuron = 0;
        }

        nextElement(genome->connection_genes);
    }

    return 1;
}

/*!
* \brief Add the given Neuron to the given Genome.
* \param[out] genome the Genome to alter
* \param[in] neuron the Neuron to insert
* \return int 1 if the Neuron was successfully inserted, 0 otherwise
*/
int addNeuronToGenome(Genome * genome, Neuron * neuron) {
    if (!addNeuron(genome->neurons, neuron)) {
        freeNeuron(neuron);

        fprintf(stderr, "Couldn't add Neuron to Genome\n");
        return 0;
    }

    return 1;
}

/*!
* \brief Disturb the mutation rates of the given Genome, by increasing or decreasing them.
* \param[out] genome The Genome whose mutation rates have to be updated
*/
static void disturbMutationRates(Genome * genome) {
    int i;

    int count = sizeof(genome->mutation_rates) / sizeof(float);

    for (i = 0; i < count; ++i) {
        if (random01() < DECREASE_PROBABILITY)
            genome->mutation_rates[i] *= 0.95;
        else
            genome->mutation_rates[i] *= 1.05;
    }
}

/*!
* \brief Mutate the given Genome.
* \param[out] genome The Genome to mutate
* \return int 1 if the mutation was successful, 0 otherwise
*
* A Genome has a mutation rate for each type of mutation that can occur.
* These mutation rates are also randomly increased or decreased as the evolution progresses.
*/
int mutate(Genome * genome) {
    if (genome->nb_mutations == N_MAX_MUTATIONS) {
        fprintf(stderr, "Can't perform mutation: reached limit (max=%d)\n", N_MAX_MUTATIONS);
        return 0;
    }

    disturbMutationRates(genome);

    /*
        genome->mutation_rates[0] = POINT_MUTATION_RATE;
        genome->mutation_rates[1] = LINK_MUTATION_RATE;
        genome->mutation_rates[2] = BIAS_MUTATION_RATE;
        genome->mutation_rates[3] = NODE_MUTATION_RATE;
        genome->mutation_rates[4] = ENABLE_MUTATION_RATE;
        genome->mutation_rates[5] = DISABLE_MUTATION_RATE;
    */

    // POINT_MUTATION_RATE

    if (random01() < genome->mutation_rates[0])
        mutatePoint(genome);

    // LINK_MUTATION_RATE

    double r = genome->mutation_rates[1];
    while (r > 0) {
        if (random01() < r) {
            mutateLink(genome, 0);
        }

        r -= 1.0;
    }

    // BIAS_MUTATION_RATE

    r = genome->mutation_rates[2];
    while (r > 0) {
        if (random01() < r) {
            mutateLink(genome, 1);
        }

        r -= 1.0;
    }

    // NODE_MUTATION_RATE

    r = genome->mutation_rates[3];
    while (r > 0) {
        if (random01() < r) {
            mutateNode(genome);
        }

        r -= 1.0;
    }

    // ENABLE_MUTATION_RATE

    r = genome->mutation_rates[4];
    while (r > 0) {
        if (random01() < r)
            mutateEnableFlag(genome, 1);

        r -= 1.0;
    }

    // DISABLE_MUTATION_RATE

    r = genome->mutation_rates[5];
    while (r > 0) {
        if (random01() < r)
            mutateEnableFlag(genome, 0);

        r -= 1.0;
    }

    return 1;
}

/*!
* \brief Randomly update the weight of a randomly selected ConnectionGene from the given Genome
* \param[out] genome The Genome whose a random ConnectionGene has to be updated
* \return int 1 if a ConnectionGene was successfully updated, 0 otherwise
*
* The updates are either:
* New Weight = Old Weight +/- Random number between 0 and pointMutationRate
* or
* New Weight = Random number between -2 and 2
*/
int mutatePoint(Genome * genome) {
    ConnectionGene * current_connection_gene = NULL;

    setOnFirstElement(genome->connection_genes);
    while (!outOfGenericList(genome->connection_genes)) {
        current_connection_gene = (ConnectionGene *) getCurrent(genome->connection_genes);

        if (random01() < POINT_MUTATION_PERTURBATION)
            current_connection_gene->weight += 2.0 * random01() * POINT_MUTATION_RATE - POINT_MUTATION_RATE;
        else
            current_connection_gene->weight = 4.0 * random01() - 2.0;

        nextElement(genome->connection_genes);
    }

    genome->mutations_history[genome->nb_mutations] = 0;
    ++genome->nb_mutations;

    return 1;
}

/*!
* \brief
* \param[out] genome
* \return int
*/
int mutateLink(Genome * genome, int bias) {
    int neuron_1_id;
    int neuron_2_id;
    ConnectionGene * connection_gene = NULL;

    neuron_1_id = getRandomNeuronId(genome, 0, 1);
    neuron_2_id = getRandomNeuronId(genome, 1, 0);

    connection_gene = newConnectionGene(0, 1, 0);

    if (connection_gene == (ConnectionGene *) NULL)
        return 0;

    if (bias)
        connection_gene->neuron_in_id = N_INPUTS - 1;
    else
        connection_gene->neuron_in_id = neuron_1_id;

    connection_gene->neuron_out_id = neuron_2_id;

    if (linked(genome, neuron_1_id, neuron_2_id)) {
        freeConnectionGene(connection_gene);
        return 1;
    }

    ++(*genome->innovation);

    connection_gene->weight = 4.0 * random01() - 2.0;
    connection_gene->innovation = *genome->innovation;

    if (!add(genome->connection_genes, connection_gene)) {
        fprintf(stderr, "Can't add new ConnectionGene to Genome\n");
        return 0;
    }

    if (bias)
        genome->mutations_history[genome->nb_mutations] = 2;
    else
        genome->mutations_history[genome->nb_mutations] = 1;

    ++genome->nb_mutations;

    return 1;
}

/*!
* \brief
* \param[out] genome
* \return int 1 if the mutation was successful, 0 otherwise
*
* This mutation adds a new Neuron to the network by disabling a ConnectionGene,
* replacing it with a ConnectionGene of weight 1, a Neuron and a ConnectionGene with
* the same weight as the disabled ConnectionGene.
*
* In essence it’s been replaced with an identically functioning equivalent.
*/
int mutateNode(Genome * genome) {
    ConnectionGene * connection_gene = NULL;

    ConnectionGene * connection_gene_1 = NULL;
    ConnectionGene * connection_gene_2 = NULL;

    if (emptyGenericList(genome->connection_genes))
        return 1;

    connection_gene = getRandomConnectionGene(genome);
    if (!connection_gene->enabled)
        return 1;

    connection_gene->enabled = 0;

    ++genome->max_neurons;
    ++(*genome->innovation);

    connection_gene_1 = cloneConnectionGene(connection_gene);
    connection_gene_1->neuron_out_id = genome->max_neurons;
    connection_gene_1->weight = 1.0;
    connection_gene_1->enabled = 1;
    connection_gene_1->innovation = *genome->innovation;

    if (!add(genome->connection_genes, connection_gene_1)) {
        fprintf(stderr, "Can't add new ConnectionGene to Genome\n");
        return 0;
    }

    ++(*genome->innovation);

    connection_gene_2 = cloneConnectionGene(connection_gene);
    connection_gene_2->neuron_in_id = genome->max_neurons;
    connection_gene_2->weight = 1.0;
    connection_gene_2->enabled = 1;
    connection_gene_2->innovation = *genome->innovation;

    if (!add(genome->connection_genes, connection_gene_2)) {
        fprintf(stderr, "Can't add new ConnectionGene to Genome\n");
        return 0;
    }

    genome->mutations_history[genome->nb_mutations] = 3;
    ++genome->nb_mutations;

    return 1;
}

/*!
* \brief Randomly enable and disable ConnectionGene elements from the given Genome
* \param[out] genome the Genome whose ConnectionGene elements have to be updated
* \param[in] enable
* \return int 1 if the ConnectionGene elements were successfully updated, 0 otherwise
*/
int mutateEnableFlag(Genome * genome, unsigned char enable) {
    int i = 0;
    int random_connection_gene_index = -1;
    ConnectionGene * current_connection_gene = NULL;
    ConnectionGene * candidates[N_MAX_NEURONS * N_MAX_CONNECTION_GENES];

    setOnFirstElement(genome->connection_genes);
    while (!outOfGenericList(genome->connection_genes)) {
        current_connection_gene = (ConnectionGene *) getCurrent(genome->connection_genes);

        if (current_connection_gene->enabled != enable) {
            candidates[i] = current_connection_gene;
            ++i;
        }

        nextElement(genome->connection_genes);
    }

    if (i == 0)
        return 1;

    random_connection_gene_index = randomLimit(i - 1);
    candidates[random_connection_gene_index]->enabled = !candidates[random_connection_gene_index]->enabled;

    if (enable)
        genome->mutations_history[genome->nb_mutations] = 4;
    else
        genome->mutations_history[genome->nb_mutations] = 5;

    ++genome->nb_mutations;

    return 1;
}

/*!
* \brief Create a new Genome resulting of a crossover between the given Genome elements.
* \param[in] genome_1 the first Genome to cross over
* \param[in] genome_2 the second Genome to cross over
* \return a new Genome resulting of a crossover between the given Genome elements, NULL otherwise
*/
Genome * crossover(Genome * genome_1, Genome * genome_2) {
    int i;
    int skip_continue_instruction = 0;
    int skip_break_instruction = 0;

    Genome * child_genome = NULL;
    Genome * temp_genome = NULL;

    ConnectionGene * current_connection_gene_1 = NULL;
    ConnectionGene * current_connection_gene_2 = NULL;
    ConnectionGene * new_connection_gene = NULL;

    // creating the child genome

    if ((child_genome = newGenome(genome_1->innovation)) == (Genome *) NULL)
        return NULL;

    // genome_1 has to have a greater fitness than genome_2

    if (genome_2->fitness > genome_1->fitness) {
        temp_genome = genome_1;
        genome_1 = genome_2;
        genome_2 = temp_genome;
    }

    setOnFirstElement(genome_1->connection_genes);
    while (!outOfGenericList(genome_1->connection_genes)) {

        current_connection_gene_1 = (ConnectionGene *) getCurrent(genome_1->connection_genes);

        setOnFirstElement(genome_2->connection_genes);
        while (!outOfGenericList(genome_2->connection_genes)) {

            current_connection_gene_2 = (ConnectionGene *) getCurrent(genome_2->connection_genes);

            // we finally have current_connection_gene_1 & current_connection_gene_2
            if (current_connection_gene_1->innovation == current_connection_gene_2->innovation) {
                if (randomBool() && current_connection_gene_2->enabled) {

                    new_connection_gene = cloneConnectionGene(current_connection_gene_2);

                    if (!add(child_genome->connection_genes, new_connection_gene)) {
                        fprintf(stderr, "Can't add new ConnectionGene to child Genome\n");
                        return NULL;
                    }

                    skip_continue_instruction = 1;
                }
                else {
                    skip_break_instruction = 1;
                }
            }

            if (skip_continue_instruction || skip_break_instruction) {
                while (!outOfGenericList(genome_2->connection_genes))
                    nextElement(genome_2->connection_genes);
            }
            else {
                nextElement(genome_2->connection_genes);
            }
        }

        if (!skip_continue_instruction) {
            new_connection_gene = cloneConnectionGene(current_connection_gene_1);

            if (!add(child_genome->connection_genes, new_connection_gene)) {
                fprintf(stderr, "Can't add new ConnectionGene to child Genome\n");
            }
        }

        skip_continue_instruction = 0;
        skip_break_instruction = 0;

        nextElement(genome_1->connection_genes);
    }

    child_genome->max_neurons = fmax(genome_1->max_neurons, genome_2->max_neurons);

    // finally, we're copying mutation rates from genome_1, which has the greater fitness

    for (i = 0; i < 4; ++i)
        child_genome->mutation_rates[i] = genome_1->mutation_rates[i];

    return child_genome;
}

static double computeWeights(Genome * genome_1, Genome * genome_2, int verbose) {
    double sum = 0.0;
    double sameInnovation = 0.0;

    ConnectionGene * current_connection_gene_1 = NULL;
    ConnectionGene * current_connection_gene_2 = NULL;

    int skip_continue_instruction = 0;

    setOnFirstElement(genome_1->connection_genes);
    while (!outOfGenericList(genome_1->connection_genes)) {

        current_connection_gene_1 = (ConnectionGene *) getCurrent(genome_1->connection_genes);

        setOnFirstElement(genome_2->connection_genes);
        while (!outOfGenericList(genome_2->connection_genes)) {

            current_connection_gene_2 = (ConnectionGene *) getCurrent(genome_2->connection_genes);

            if (current_connection_gene_1->innovation == current_connection_gene_2->innovation) {
                sum += fabs(current_connection_gene_1->weight - current_connection_gene_2->weight);
                sameInnovation += 1.0;

                skip_continue_instruction = 1;
            }

            if (skip_continue_instruction) {
                while (!outOfGenericList(genome_2->connection_genes))
                    nextElement(genome_2->connection_genes);
            }
            else
                nextElement(genome_2->connection_genes);
        }

        skip_continue_instruction = 0;

        nextElement(genome_1->connection_genes);
    }

    if (verbose) {
        printf("computeWeights\n");
        printf("sum: %f\n", sum);
        printf("sameInnovation: %f\n", sameInnovation);
        printf("res: %f\n\n", sum / sameInnovation);
    }

    return sum / sameInnovation;
}

static double computeDisjoint(Genome * genome_1, Genome * genome_2, int verbose) {
    double disjoint_count = 0.0;

    ConnectionGene * current_connection_gene_1 = NULL;
    ConnectionGene * current_connection_gene_2 = NULL;

    int skip_continue_instruction = 0;

    setOnFirstElement(genome_1->connection_genes);
    while (!outOfGenericList(genome_1->connection_genes)) {

        current_connection_gene_1 = (ConnectionGene *) getCurrent(genome_1->connection_genes);

        setOnFirstElement(genome_2->connection_genes);
        while (!outOfGenericList(genome_2->connection_genes)) {

            current_connection_gene_2 = (ConnectionGene *) getCurrent(genome_2->connection_genes);

            if (current_connection_gene_1->innovation == current_connection_gene_2->innovation) {
                nextElement(genome_1->connection_genes);

                skip_continue_instruction = 1;
            }

            if (skip_continue_instruction) {
                while (!outOfGenericList(genome_2->connection_genes))
                    nextElement(genome_2->connection_genes);
            }
            else {
                disjoint_count += 1.0;
                nextElement(genome_2->connection_genes);
            }
        }

        skip_continue_instruction = 0;

        nextElement(genome_1->connection_genes);
    }

    if (verbose) {
        printf("computeDisjoint\n");
        printf("disjoint_count: %f\n", disjoint_count);
        printf("max: %f\n", fmax(count(genome_1->connection_genes), count(genome_2->connection_genes)));
        printf("res: %f\n\n", disjoint_count / fmax(count(genome_1->connection_genes), count(genome_2->connection_genes)));
    }

    return disjoint_count / fmax(count(genome_1->connection_genes), count(genome_2->connection_genes));
}

/*!
* \brief Check if the given Genome elements are from the same Species
* \param[out] genome_1 the first Genome to compare
* \param[out] genome_2 the second Genome to compare
* \return int 1 if the two Genome elements are from the same Species, 0 otherwise
*/
double sameSpecies(Genome * genome_1, Genome * genome_2) {
    double coef = WEIGHT_COEFFICIENT * computeWeights(genome_1, genome_2, 0) + DISJOINT_COEFFICIENT * computeDisjoint(genome_1, genome_2, 0) < SPECIATION_THRESHOLD;
    return coef;
}

/*!
* \brief Check if the two given Neuron elements are linked by a ConnectionGene.
* \param[out] neuron_in The first Neuron
* \param[in] neuron_out The second Neuron
* \return int 1 if the two Neuron elements are linked, 0 otherwise
*/
int linked(Genome * genome, int neuron_in_id, int neuron_out_id) {
    ConnectionGene * current_connection_gene = NULL;

    setOnFirstElement(genome->connection_genes);
    while (!outOfGenericList(genome->connection_genes)) {

        current_connection_gene = (ConnectionGene *) getCurrent(genome->connection_genes);

        if (current_connection_gene->neuron_in_id == neuron_in_id && current_connection_gene->neuron_out_id == neuron_out_id)
            return 1;

        nextElement(genome->connection_genes);
    }

    return 0;
}

double * evaluateGenome(Genome * genome, double * input) {
    int i;
    double sum;
    double * output = NULL;

    Neuron * current_neuron = NULL;
    Neuron * current_neuron_in = NULL;

    ConnectionGene * current_connection_gene = NULL;

    // updating values of the neurons from the given inputs

    for (i = 0; i < N_INPUTS; ++i) {
        setOn(genome->neurons, i);
        ((Neuron *) getCurrent(genome->neurons))->value = input[i];
    }

    // scanning input and output neurons

    setOnFirstElement(genome->neurons);
    while (!outOfGenericList(genome->neurons)) {
        current_neuron = (Neuron *) getCurrent(genome->neurons);

        if (current_neuron->id >= N_INPUTS + N_OUTPUTS) {

            sum = 0.0;

            setOnFirstElement(current_neuron->connection_genes_input);
            while (!outOfGenericList(current_neuron->connection_genes_input)) {

                current_connection_gene = (ConnectionGene *) getCurrent(current_neuron->connection_genes_input);
                current_neuron_in = getNeuron(genome, current_connection_gene->neuron_in_id);

                sum += current_connection_gene->weight * current_neuron_in->value;

                nextElement(current_neuron->connection_genes_input);
            }

            if (!emptyGenericList(current_neuron->connection_genes_input)) {
                current_neuron->value = fast_sigmoid(sum);
            }
        }

        nextElement(genome->neurons);
    }

  // scanning output neurons

    setOnFirstElement(genome->neurons);
    while (!outOfGenericList(genome->neurons)) {
        current_neuron = (Neuron *) getCurrent(genome->neurons);

        if (current_neuron->id >= N_INPUTS && current_neuron->id < N_INPUTS + N_OUTPUTS) {

            sum = 0.0;

            setOnFirstElement(current_neuron->connection_genes_input);
            while (!outOfGenericList(current_neuron->connection_genes_input)) {

                current_connection_gene = (ConnectionGene *) getCurrent(current_neuron->connection_genes_input);
                current_neuron_in = getNeuron(genome, current_connection_gene->neuron_in_id);

                sum += current_connection_gene->weight * current_neuron_in->value;

                nextElement(current_neuron->connection_genes_input);
            }

            if (!emptyGenericList(current_neuron->connection_genes_input))
                current_neuron->value = fast_sigmoid(sum);
        }

        nextElement(genome->neurons);
    }

    // returning output

    if ((output = malloc(sizeof(double) * N_OUTPUTS)) == NULL)
        return NULL;

    for (i = 0; i < N_OUTPUTS; ++i) {
        setOn(genome->neurons, N_INPUTS + i);
        output[i] = ((Neuron *) getCurrent(genome->neurons))->value;
    }

    return output;
}

/*!
* \brief Return a random Neuron id from the given Genome.
* \param[in] genome the Genome to choose a Neuron from
* \return Return a random Neuron
*/
int getRandomNeuronId(Genome * genome, int non_input, int non_output) {
    int i;
    ConnectionGene * current_connection_gene = NULL;
    int count = 0;
    int candidates[N_MAX_NEURONS];

    if (!non_input)
        for (i = 0; i < N_INPUTS; ++i)
            candidates[count++] = i;

    if (!non_output)
        for (i = 0; i < N_OUTPUTS; ++i)
            candidates[count++] = N_INPUTS + i;

    setOnFirstElement(genome->connection_genes);
    while (!outOfGenericList(genome->connection_genes)) {

        current_connection_gene = (ConnectionGene *) getCurrent(genome->connection_genes);

        if (!non_input || current_connection_gene->neuron_in_id >= N_INPUTS)
            if (!non_output || current_connection_gene->neuron_in_id >= N_INPUTS + N_OUTPUTS)
                candidates[count++] = current_connection_gene->neuron_in_id;

        if (!non_input || current_connection_gene->neuron_out_id >= N_INPUTS)
            if (!non_output || current_connection_gene->neuron_out_id >= N_INPUTS + N_OUTPUTS)
                candidates[count++] = current_connection_gene->neuron_out_id;

        nextElement(genome->connection_genes);
    }

    return candidates[randomLimit(count - 1)];
}

/*!
* \brief Return the Neuron matching the given id.
* \param[in] genome the Genome to search a matching Neuron in
* \param[in] id the id of the Neuron to search for
* \return Return the Neuron whose id matches, or NULL if not found
*/
Neuron * getNeuron(Genome * genome, int id) {
    Neuron * current_neuron = NULL;
    Node * save_pos = genome->neurons->current;

    setOnFirstElement(genome->neurons);
    while (!outOfGenericList(genome->neurons)) {
        current_neuron = (Neuron *) getCurrent(genome->neurons);

        if (current_neuron->id == id) {
            genome->neurons->current = save_pos;
            return current_neuron;
        }

        nextElement(genome->neurons);
    }

    genome->neurons->current = save_pos;

    fprintf(stderr, "Neuron with id %d not found\n", id);
    return current_neuron;
}

/*!
* \brief Return a random ConnectionGene from the given Genome.
* \param[in] genome the Genome to choose a ConnectionGene from
* \return Return a random ConnectionGene
*/
ConnectionGene * getRandomConnectionGene(Genome * genome) {
    setOn(genome->connection_genes, randomLimit(count(genome->connection_genes) - 1));
    return ((ConnectionGene *) getCurrent(genome->connection_genes));
}

/*!
* \brief Write a the Network of a Genome to an output file, based on graphviz.
* \param[in] genome the Genome to write
* \param[in] filename the name of the output file
* \return int 1 if the file was successfully written, 0 otherwise
*/
int writeGraphVizGenome(Genome * genome, char * filename) {
    FILE * f = NULL;
    Neuron * current_neuron = NULL;
    ConnectionGene * current_connection_gene = NULL;

    if ((f = (FILE *) fopen(filename, "w")) == (FILE *) NULL) {
        fprintf(stderr, "Error while opening %s\n", filename);
        return 0;
    }

    fprintf(f, "digraph {\n");

    // links

    setOnFirstElement(genome->connection_genes);
    while (!outOfGenericList(genome->connection_genes)) {

        current_connection_gene = (ConnectionGene *) getCurrent(genome->connection_genes);

        if (current_connection_gene->enabled)
            fprintf(f, "\t%d -> %d [label=\"%.1f\\n%d\", weight=%.1f];\n", current_connection_gene->neuron_in_id, current_connection_gene->neuron_out_id, current_connection_gene->weight, current_connection_gene->innovation, current_connection_gene->weight);
        else
            fprintf(f, "\t%d -> %d [label=\"%.1f\\n%d\", weight=%.1f color=red];\n", current_connection_gene->neuron_in_id, current_connection_gene->neuron_out_id, current_connection_gene->weight, current_connection_gene->innovation, current_connection_gene->weight);

        nextElement(genome->connection_genes);
    }

    // nodes

    setOnFirstElement(genome->neurons);
    while (!outOfGenericList(genome->neurons)) {

        current_neuron = (Neuron *) getCurrent(genome->neurons);

        if (current_neuron->type == INPUT)
            fprintf(f, "\t%d [label=\"%d\\n%.3f\", shape=circle, style=filled, fillcolor=yellow];\n", current_neuron->id, current_neuron->id, current_neuron->value);

        else if (current_neuron->type == OUTPUT)
            fprintf(f, "\t%d [label=\"%d\\n%.3f\", shape=circle, style=filled, fillcolor=red];\n", current_neuron->id, current_neuron->id, current_neuron->value);

        else if (current_neuron->type == BASIC)
            fprintf(f, "\t%d [label=\"%d\\n%.3f\", shape=circle];\n", current_neuron->id, current_neuron->id, current_neuron->value);

        else
            fprintf(f, "\t%d [shape=circle];\n", current_neuron->id);

        nextElement(genome->neurons);
    }

    fprintf(f, "}");
    fclose(f);

    return 1;
}

/*!
* \brief Print the given Genome.
* \param[in] genome the Genome to print
*/
void printGenome(Genome * genome) {
    int k;
    ConnectionGene * current_connection_gene = NULL;

    printf("----------------------------------\n");
    printf("Genome\n");
    printf("\t%p\n", genome);

    printf("\t\tnb_neurons: %d ; TYPES:", count(genome->neurons));

    setOnFirstElement(genome->neurons);
    while (!outOfGenericList(genome->neurons)) {
        printf(" %d", ((Neuron *) getCurrent(genome->neurons))->type);

        nextElement(genome->neurons);
    }

    printf(" ; IDs:");

    setOnFirstElement(genome->neurons);
    while (!outOfGenericList(genome->neurons)) {
        printf(" %d", ((Neuron *) getCurrent(genome->neurons))->id);

        nextElement(genome->neurons);
    }

    printf("\n");

    printf("\t\tnb_connection_genes: %d ; NETWORK_IDs:", count(genome->connection_genes));

    setOnFirstElement(genome->connection_genes);
    while (!outOfGenericList(genome->connection_genes)) {

        current_connection_gene = (ConnectionGene *) getCurrent(genome->connection_genes);

        printf(" %d -> %d,", current_connection_gene->neuron_in_id, current_connection_gene->neuron_out_id);

        nextElement(genome->connection_genes);
    }

    printf("\n");

    printf("\t\tmax_neurons: %d\n", genome->max_neurons);
    printf("\t\tfitness: %f\n", genome->fitness);
    printf("\t\tglobal_rank: %d\n", genome->global_rank);
    printf("\n");

    printf("\tMutation rates:\n");

    printf("\t\tPOINT_MUTATION_RATE (0): %f\n", genome->mutation_rates[0]);
    printf("\t\tLINK_MUTATION_RATE (1): %f\n", genome->mutation_rates[1]);
    printf("\t\tBIAS_MUTATION_RATE (2): %f\n", genome->mutation_rates[2]);
    printf("\t\tNODE_MUTATION_RATE (3): %f\n", genome->mutation_rates[3]);
    printf("\t\tENABLE_MUTATION_RATE (4): %f\n", genome->mutation_rates[4]);
    printf("\t\tDISABLE_MUTATION_RATE (5): %f\n", genome->mutation_rates[5]);

    printf("\n");
    printf("\tMutations history: ");

    for (k = 0; k < genome->nb_mutations; ++k)
        printf("%d ", genome->mutations_history[k]);

    printf("\n");
}

static int compareOutputs(const void * connection_gene_1, const void * connection_gene_2) {
    int diff = ((ConnectionGene *) connection_gene_2)->neuron_out_id - ((ConnectionGene *) connection_gene_1)->neuron_out_id;

    if (diff == 0.0)
        return 0;
    else if (diff < 0.0)
        return -1;

    return 1;
}
