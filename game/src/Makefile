# Compiler
CC = gcc

# Selection of the OS
ifeq ($(OS),Windows_NT)
	# SDL inclusion and link for Windows
	CFLAGS = -Wall -g -I$(SDLPATH)\include
	LDFLAGS = -L$(SDLPATH)\lib -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf -lSDL2_mixer
else
	# SDL inclusion and link for Linux
	CFLAGS = -Wall -g $(shell sdl2-config --cflags)
	LDFLAGS = $(shell sdl2-config --libs) -lSDL2_ttf -lSDL2_mixer
endif


Release: main
Debug: main

main: main.o bird.o camera.o control.o file.o game.o list.o obstacle.o pipe.o sound.o view.o
	$(CC) $^ -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) $< -c


# Clean command for Codeblocks
cleanRelease: cleanDebug
cleanDebug:
	del *.o
	del main.exe

# Clean command in command line
clean:
	rm -f *.o *~
	rm -f main
