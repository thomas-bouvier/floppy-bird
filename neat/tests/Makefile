CC=gcc -g
CFLAGS=-Wall
LDFLAGS=-lcmocka

main: main.o genome.o population.o utils.o network.o neuronList.o connectionGeneList.o
	$(CC) $^ $(LDFLAGS) -o $@

main.o: main.c
	$(CC) $(CFLAGS) -c $<

genome.o: ../src/genome.c ../src/genome.h
	$(CC) $(CFLAGS) -c $<

population.o: ../src/population.c ../src/population.h
	$(CC) $(CFLAGS) -c $<

utils.o: ../src/utils.c ../src/utils.h
	$(CC) $(CFLAGS) -c $<

network.o: ../src/network.c ../src/network.h
	$(CC) $(CFLAGS) -c $<

neuronList.o: ../src/neuronList.c ../src/neuronList.h
	$(CC) $(CFLAGS) -c $<

connectionGeneList.o: ../src/connectionGeneList.c ../src/connectionGeneList.h
	$(CC) $(CFLAGS) -c $<

clean:
	rm -f *~
	rm -f main
