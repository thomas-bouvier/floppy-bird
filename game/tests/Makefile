CC=gcc -g
CFLAGS=-Wall -fprofile-arcs -ftest-coverage
LDFLAGS=-lcmocka -fprofile-arcs -ftest-coverage

main: main.o file.o bird.o camera.o obstacle.o pipe.o generic_list.o 
#game.o
	$(CC) $^ $(LDFLAGS) -o $@

main.o: main.c
	$(CC) $(CFLAGS) $< -c

game.o: ../src/game.c ../src/game.h ../src/constants.h ../src/bird.h ../src/obstacle.h ../src/camera.h
	$(CC) $(CFLAGS) $< -c

file.o: ../src/file.c ../src/file.h ../src/constants.h
	$(CC) $(CFLAGS) $< -c

bird.o: ../src/bird.c ../src/bird.h ../src/constants.h
	$(CC) $(CFLAGS) $< -c

camera.o: ../src/camera.c ../src/camera.h ../src/constants.h
	$(CC) $(CFLAGS) $< -c

obstacle.o : ../src/obstacle.c ../src/obstacle.h ../src/constants.h ../src/pipe.h ../src/bird.h
	$(CC) $(CFLAGS) $< -c

pipe.o: ../src/pipe.c ../src/pipe.h ../src/constants.h
	$(CC) $(CFLAGS) $< -c

generic_list.o: ../../ai/neat/src/generic_list.c ../../ai/neat/src/generic_list.h
	$(CC) $(CFLAGS) $< -c



clean:
	rm -f *.o *.~ ../tests/*.gcno ../tests/*.gcda *.gcov
	rm -f main

move:
	mv *.gcno ../src
	mv *.gcda ../src
