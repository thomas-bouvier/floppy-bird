CC = gcc
CFLAGS = -Wall -g $(shell sdl2-config --cflags)
LDFLAGS = $(shell sdl2-config --libs) -lSDL2_ttf -lSDL2_mixer

main: main.o game.o view.o control.o file.o bird.o camera.o obstacle.o pipe.o list.o sound.o
	$(CC) $^ -o $@ $(LDFLAGS)


%.o: %.c
	$(CC) $(CFLAGS) $< -c


clean:
	rm -f *.o
	rm -f main
