CC = gcc -g
CFLAGS = -Wall
LDFLAGS = -lcmocka -lm

main: main.o ../src/genome.o ../src/population.o ../src/utils.o ../src/network.o ../src/list.o
	$(CC) $^ $(LDFLAGS) -o $@

main.o: main.c
