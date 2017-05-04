CC = gcc -g
CFLAGS = -Wall
LDFLAGS = -lcmocka -lm

main: main.o ../src/genome.o ../src/population.o ../src/neat_utils.o ../src/network.o ../src/generic_list.o
	$(CC) $^ $(LDFLAGS) -o $@

main.o: main.c
