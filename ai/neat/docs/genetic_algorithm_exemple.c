/* undetailed algorithm to optimize a brute force string finder algorithm thanks to genetic */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define length 8
#define nb_members 180 /* size of the population */
#define mutation_percentage 3

struct Members {
    char dna[length+1];
    float fitness;
};

struct Population {
    struct Members memb[nb_members];
};

char * gen_rand_string(unsigned char len);
void find_string_bf(char * stringtofind, unsigned char len);

struct Population * create_population(int nb_memb, unsigned char len);
void fitness(struct Members *source, char *target, unsigned char len);
void pop_fitness(struct Population *pop, char *target, int nb_memb, unsigned char len);
struct Members * fill_matingpool(struct Population *pop, struct Members *matingPool, int nb_memb, int *length_matingPool);
char *one_crossover(struct Members *matingPool, int len, int length_matingPool);
char * mutate(char *child, int len, int muta_perc);
struct Population * gen_newpop(struct Population *pop, struct Members *matingPool, int len, int length_matingPool, int nb_memb);

char * gen_rand_string(unsigned char len)
{
    int i;
    char * sentence;
    sentence = (char*)malloc(len * sizeof(char)+1);

    for(i=0; i<len; ++i)
    {
        sentence[i] = 32+rand()%128;
    }
    sentence[i] = '\0';
    return sentence;
}

void find_string_bf(char * stringtofind, unsigned char len)
{
    char *str_sent = (char*) malloc(length*sizeof(char)+1);

    while(strcmp(str_sent, stringtofind) != 0) printf("%s\n", str_sent = gen_rand_string(len));
}

struct Population * create_population(int nb_memb, unsigned char len)
{
    int i;

    struct Population * pop  = (struct Population *) malloc(sizeof(struct Population));

    for(i=0; i<nb_memb; ++i)
    strcpy(pop->memb[i].dna, gen_rand_string(length));
    pop->memb[i].fitness = 0;

    return pop;
}

void fitness(struct Members *source, char *target, unsigned char len)
{
    int i;
    source->fitness=0;

    for(i=0; i<len; ++i)
    {
        if(!strcmp(source->dna+i, target+i)) source->fitness++;
    }
    source->fitness=0.01+source->fitness/(float)len;
}

void pop_fitness(struct Population *pop, char *target, int nb_memb, unsigned char len)
{
    int i;
    for(i=0; i<nb_memb; ++i)
    {
        fitness(&(pop->memb[i]), target, len);
    }
}

struct Members * fill_matingpool(struct Population *pop, struct Members *matingPool, int nb_memb, int *length_matingPool)
{
    int i,j;
    float nb_byte_realloc=0;
    *length_matingPool=0;
    for(i=0; i<nb_memb; ++i) nb_byte_realloc = nb_byte_realloc+(100*pop->memb[i].fitness)+1;
    matingPool = (struct Members *) malloc(nb_byte_realloc * sizeof(struct Members));
    for(i=0; i<nb_memb; ++i)
    {
        for(j=0; j<(100*pop->memb[i].fitness); ++j)
        {
           *(length_matingPool)+=1;
            strcpy(matingPool[*length_matingPool-1].dna,(const char*) &(pop->memb[i].dna));
            matingPool[*length_matingPool-1].fitness = pop->memb[i].fitness;
        }
    }
    return matingPool;
}

char *one_crossover(struct Members *matingPool, int len, int length_matingPool)
{
    char *parentA = matingPool[rand()%length_matingPool].dna;
    char *parentB = matingPool[rand()%length_matingPool].dna;
    int i;
    /*full random*/
    for(i=0; i<len; ++i)
    {
        if(rand()%1<0.5)strcpy(parentA+i, parentA+i);
        else strcpy(parentA+i, parentB+i);
    }
    /* random midpoint*/
    /*unsigned char random_midpoint = rand()%len;
    int i;

    for(i=0; i<len; ++i)
    {
        if(i<random_midpoint)strcpy(parentA+i, parentA+i);
        else strcpy(parentA+i, parentB+i);
    }*/

    return mutate(parentA, len, mutation_percentage);
}

char * mutate(char *child, int len, int muta_perc)
{
    int i;
    for(i=0; i<len; ++i) if(rand()%100<muta_perc) child[i] = 32+rand()%128;
    return child;
}

struct Population * gen_newpop(struct Population *pop, struct Members *matingPool, int len, int length_matingPool, int nb_memb)
{
    int i;
    for(i=0; i<nb_memb; ++i) strcpy(pop->memb[i].dna, one_crossover(matingPool, length, length_matingPool));
    return pop;
}

int main()
{
    /* INIT*/
    srand(time(NULL));
    struct Population *pop = NULL;
    struct Members *matingPool = NULL;
    int length_matingPool = 0;
    char *target = "Sent nce";
    int running = 1;
    int i;
    /* SETUP */
     pop = create_population(nb_members, length);

    /* LOOP */
    while(running)
    {
        pop_fitness(pop, target, nb_members, length);
        matingPool = fill_matingpool(pop, matingPool, nb_members, &length_matingPool);
        pop = gen_newpop(pop, matingPool, length, length_matingPool, nb_members);
        for(i=0; i<nb_members; ++i)
        {
            if(running) printf("%s\n", pop->memb[i].dna);
            if(!strcmp(pop->memb[i].dna, target)) running = 0;
        }
    }

    /*find_string_bf(target, length)*/

    return 0;
}
