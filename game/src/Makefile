CC = gcc
CFLAGS = -Wall -g $(shell sdl2-config --cflags)
LDFLAGS = $(shell sdl2-config --libs)
Q_LEARNING_PATH=../../ai/q_learning/src/
main: main.o game.o view.o control.o file.o bird.o camera.o obstacle.o pipe.o list.o q_learning.o qmatrix.o game_state.o utils.o file_manager.o
	$(CC) $^ -o $@ $(LDFLAGS)

main.o: $(Q_LEARNING_PATH)qmatrix.h $(Q_LEARNING_PATH)game_state.h
q_learning.o: $(Q_LEARNING_PATH)qmatrix.h $(Q_LEARNING_PATH)game_state.h
qmatrix.o: $(Q_LEARNING_PATH)game_state.h $(Q_LEARNING_PATH)utils.h $(Q_LEARNING_PATH)conf.h
file_manager.o: $(Q_LEARNING_PATH)qmatrix.h
game_state.o: $(Q_LEARNING_PATH)conf.h

%.o: %.c
	$(CC) $(CFLAGS) $< -c
%.o:  $(Q_LEARNING_PATH)/%.c
	$(CC) $(CFLAGS) $< -c

clean:
	rm -rf *.o

mrproper: clean
	rm -rf main

