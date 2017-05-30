CC = gcc -g
CFLAGS = -Wall -fprofile-arcs -ftest-coverage
LDFLAGS = -lcmocka -lm -fprofile-arcs -ftest-coverage

main: main.o ../src/genome.o ../src/population.o ../src/neat_utils.o ../src/network.o ../src/generic_list.o
	$(CC) $^ $(LDFLAGS) -o $@

main.o: main.c
