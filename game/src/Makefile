# Compiler
CC = gcc
CFLAGS = -Wall -g $(shell sdl2-config --cflags)
LDFLAGS = $(shell sdl2-config --libs)
Q_LEARNING_PATH=../../ai/q_learning/src/

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

main: main.o bird.o camera.o control.o file.o game.o list.o menu.o obstacle.o pipe.o sound.o view.o q_learning.o qmatrix.o game_state.o utils.o file_manager.o
	$(CC) $^ -o $@ $(LDFLAGS)

main.o: $(Q_LEARNING_PATH)qmatrix.h $(Q_LEARNING_PATH)game_state.h $(Q_LEARNING_PATH)conf.h
q_learning.o: $(Q_LEARNING_PATH)qmatrix.h $(Q_LEARNING_PATH)game_state.h $(Q_LEARNING_PATH)conf.h
qmatrix.o: $(Q_LEARNING_PATH)game_state.h $(Q_LEARNING_PATH)utils.h $(Q_LEARNING_PATH)conf.h
file_manager.o: $(Q_LEARNING_PATH)qmatrix.h
game_state.o: $(Q_LEARNING_PATH)conf.h

%.o: %.c
	$(CC) $(CFLAGS) $< -c
%.o:  $(Q_LEARNING_PATH)/%.c
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
