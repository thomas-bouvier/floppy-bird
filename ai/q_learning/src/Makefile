CC=gcc
CFLAGS=-W -Wall -pedantic
LDFLAGS=
EXEC=q_learning

all: $(EXEC)

q_learning: q_learning.o qmatrix.o game_state.o utils.o file_manager.o
	$(CC) -o $@ $^ $(LDFLAGS)

qmatrix.o: game_state.h utils.h conf.h
file_manager.o: qmatrix.h
game_state.o: conf.h

%.o: %.c
	$(CC) -o $@ -c $< $(CFLAGS)

clean:
	rm -rf *.o

mrproper: clean
	rm -rf $(EXEC)

