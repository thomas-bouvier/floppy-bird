CC = gcc
CFLAGS = -Wall
LDFLAGS = -lm

main: main.o genome.o population.o utils.o network.o neuronList.o connectionGeneList.o
	$(CC) $^ $(LDFLAGS) -o $@

main.o: main.c
genome.o: genome.c genome.h
population.o: population.c population.h
utils.o: utils.c utils.h
network.o: network.c network.h
neuronList.o: neuronList.c neuronList.h
connectionGeneList.o: connectionGeneList.c connectionGeneList.h
