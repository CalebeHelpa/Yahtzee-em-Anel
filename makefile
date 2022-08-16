CFLAGS = -g
LDFLAGS = -lm

all: game

game: main.o messages.o game.o
	gcc -o game main.o messages.o game.o $(CFLAGS) $(LDFLAGS)

main.o: main.c messages.h game.o
	gcc -c main.c -o main.o $(CFLAGS)

game.o: game.c game.h
	gcc -c game.c $(CFLAGS)

messages.o: messages.c messages.h
	gcc -c messages.c $(CFLAGS)

clean:
	-rm -f *~ *.o
	
purge: clean
	-rm -f game