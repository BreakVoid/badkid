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

elf_reader.o: elf_reader.c elf_reader.h
	$(CC) -c -o elf_reader.o elf_reader.c $(CFLAGS)

elf_writer.o: elf_writer.c elf_writer.h
	$(CC) -c -o elf_writer.o elf_writer.c $(CFLAGS)

readself.o: readself.c
	$(CC) -c -o readself.o readself.c $(CFLAGS)

main.o: main.c
	$(CC) -c -o main.o main.c $(CFLAGS)

clean:
	rm *.o
