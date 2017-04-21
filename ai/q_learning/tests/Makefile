CC=gcc -g
CFLAGS=-Wall -fprofile-arcs -ftest-coverage
LDFLAGS=-lcmocka -fprofile-arcs -ftest-coverage
Q_LEARNING_PATH=../src/

main: main.o q_learning.o qmatrix.o game_state.o utils.o file_manager.o
	$(CC) $^ -o $@ $(LDFLAGS)

main.o: main.c $(Q_LEARNING_PATH)conf.h
	$(CC) $(CFLAGS) $< -c

q_learning.o: $(Q_LEARNING_PATH)q_learning.c $(Q_LEARNING_PATH)file_manager.h $(Q_LEARNING_PATH)qmatrix.h $(Q_LEARNING_PATH)conf.h
	$(CC) $(CFLAGS) $^ -c

qmatrix.o: $(Q_LEARNING_PATH)qmatrix.c $(Q_LEARNING_PATH)utils.h $(Q_LEARNING_PATH)game_state.h $(Q_LEARNING_PATH)conf.h
	$(CC) $(CFLAGS) $^ -c

game_state.o: $(Q_LEARNING_PATH)game_state.c $(Q_LEARNING_PATH)conf.h
	$(CC) $(CFLAGS) $^ -c

utils.o: $(Q_LEARNING_PATH)utils.c
	$(CC) $(CFLAGS) $^ -c

file_manager.o: $(Q_LEARNING_PATH)file_manager.c $(Q_LEARNING_PATH)qmatrix.h
	$(CC) $(CFLAGS) $^ -c

clean:
	rm -f *.~
	rm -f *.o

mrproper: clean
	rm -f main
	rm -f *.gcda
	rm -f *.gcno
	rm -f *.gcov
