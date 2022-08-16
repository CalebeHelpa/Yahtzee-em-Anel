CFLAGS = 
LDFLAGS = -lm

all: game

game: main.o messages.o
	gcc -o game main.o messages.o $(CFLAGS) $(LDFLAGS)

main.o: main.c messages.h
	gcc -c main.c -o main.o $(CFLAGS)

messages.o: messages.c messages.h
	gcc -c messages.c $(CFLAGS)

clean:
	-rm -f *~ *.o
	
purge: clean
	-rm -f game