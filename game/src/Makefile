CC=gcc
CFLAGS=-Wall -g

main: main.o game.o view.o control.o file.o bird.o camera.o obstacle.o pipe.o list.o
	$(CC) -g $^ -lSDL2 -lSDL2main -o $@

main.o: main.c
	$(CC) $(CFLAGS) $< -c

game.o: game.c game.h constants.h bird.h obstacle.h camera.h
	$(CC) $(CFLAGS) $< -c

view.o: view.c view.h constants.h bird.h obstacle.h camera.h
	$(CC) $(CFLAGS) $< -c

control.o: control.c control.h constants.h
	$(CC) $(CFLAGS) $< -c

file.o: file.c file.h constants.h
	$(CC) $(CFLAGS) $< -c

bird.o: bird.c bird.h constants.h
	$(CC) $(CFLAGS) $< -c

camera.o: camera.c camera.h constants.h
	$(CC) $(CFLAGS) $< -c

obstacle.o : obstacle.c obstacle.h constants.h pipe.h bird.h
	$(CC) $(CFLAGS) $< -c

pipe.o: pipe.c pipe.h constants.h
	$(CC) $(CFLAGS) $< -c

list.o: list.c list.h constants.h obstacle.h
	$(CC) $(CFLAGS) $< -c


clean:
	rm -f *.o
	rm -f main
