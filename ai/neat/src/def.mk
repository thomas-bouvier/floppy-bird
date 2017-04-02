CC = gcc
CFLAGS = -Wall
LDFLAGS = -lm

main: main.o list.o genome.o population.o utils.o network.o
	$(CC) $^ $(LDFLAGS) -o $@

main.o: main.c
genome.o: genome.c genome.h
population.o: population.c population.h
utils.o: utils.c utils.h
network.o: network.c network.h
list.o: list.c list.h
