# Compiler
CC = gcc
CFLAGS = -Wall -g -fprofile-arcs -ftest-coverage
LDFLAGS = -lSDL2_ttf -lSDL2_mixer -lSDL2_image -lm -fprofile-arcs -ftest-coverage
Q_LEARNING_PATH=../../ai/q_learning/src/
NEAT_PATH=../../ai/neat/src

# Selection of the OS
ifeq ($(OS),Windows_NT)
	# SDL inclusion and link for Windows
	CFLAGS += -I$(SDLPATH)\include
	LDFLAGS += -L$(SDLPATH)\lib -lmingw32 -lSDL2main -lSDL2
else
	# SDL inclusion and link for Linux
	CFLAGS += $(shell sdl2-config --cflags)
	LDFLAGS += $(shell sdl2-config --libs)
endif


Release: main
Debug: main

main: main.o bird.o camera.o control.o file.o game.o menu.o obstacle.o pipe.o sdl_files.o sound.o view.o q_learning.o qmatrix.o game_state.o utils.o file_manager.o generic_list.o genome.o population.o network.o neat_utils.o
	$(CC) $^ -o $@ $(LDFLAGS)

main.o: $(Q_LEARNING_PATH)q_learning.h $(Q_LEARNING_PATH)conf.h $(NEAT_PATH)/generic_list.h $(NEAT_PATH)/population.h $(NEAT_PATH)/neat_conf.h
q_learning.o: $(Q_LEARNING_PATH)qmatrix.h $(Q_LEARNING_PATH)file_manager.h $(Q_LEARNING_PATH)utils.h $(Q_LEARNING_PATH)conf.h
qmatrix.o: $(Q_LEARNING_PATH)game_state.h $(Q_LEARNING_PATH)utils.h $(Q_LEARNING_PATH)conf.h
file_manager.o: $(Q_LEARNING_PATH)qmatrix.h
game_state.o: $(Q_LEARNING_PATH)conf.h
genome.o: $(NEAT_PATH)/neat_conf.h $(NEAT_PATH)/network.h $(NEAT_PATH)/neat_utils.h
population.o: $(NEAT_PATH)/neat_conf.h $(NEAT_PATH)/genome.h $(NEAT_PATH)/generic_list.h
network.o: $(NEAT_PATH)/neat_conf.h $(NEAT_PATH)/generic_list.o

%.o: %.c constants.h
	$(CC) $(CFLAGS) $< -c

%.o: $(Q_LEARNING_PATH)%.c
	$(CC) $(CFLAGS) $< -c

%.o: $(NEAT_PATH)/%.c
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
