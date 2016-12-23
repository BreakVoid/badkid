CC = gcc
CFLAGS = -std=c11 -fPIE

all: virus

debug: virus-debug
virus-debug: main.o-debug readself.o-debug
	$(CC) -o virus main.o readself.o -g

main.o-debug: main.c
	$(CC) -c -o main.o main.c $(CFLAGS) -g

readself.o-debug: readself.c
	$(CC) -c -o readself.o readself.c $(CFLAGS) -g
	
virus: main.o readself.o
	$(CC) -o virus main.o readself.o

main.o: main.c
	$(CC) -c -o main.o main.c $(CFLAGS)

clean:
	rm *.o
