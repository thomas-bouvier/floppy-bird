CC = gcc
CFLAGS = -Wall
LDFLAGS = -lm

main: main.o generic_list.o genome.o population.o neat_utils.o network.o
	$(CC) $^ $(LDFLAGS) -o $@

main.o: main.c
genome.o: genome.c genome.h
population.o: population.c population.h
neat_utils.o: neat_utils.c neat_utils.h
network.o: network.c network.h
generic_list.o: generic_list.c generic_list.h
