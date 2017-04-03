CC=gcc
CFLAGS=-W -Wall -pedantic
LDFLAGS=
EXEC=main

all: $(EXEC)

main: main.o qmatrix.o game_state.o utils.o file_manager.o
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

