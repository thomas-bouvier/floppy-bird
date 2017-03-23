CC = gcc
CFLAGS = -Wall -MMD
LDFLAGS =

SRC_FILES = $(wildcard *.c)
O_FILES = $(patsubst %.c, build/%.o, $(SRC_FILES))

all: bin/main

bin/%: build/%.o $(O_FILES)
	mkdir -p bin
	$(CC) $^ $(LDFLAGS) -o $@

build/%.o: %.c
	mkdir -p $(@D)
	$(CC) -c $(CFLAGS) $< -o $@

clean:
	rm -rf bin build
