CC = gcc
CFLAGS = -std=c11 -fPIE

all: 

elf_reader.o: elf_reader.c elf_reader.h
	$(CC) -c -o elf_reader.o elf_reader.c $(CFLAGS)

elf_writer.o: elf_writer.c elf_writer.h
	$(CC) -c -o elf_writer.o elf_writer.c $(CFLAGS)

clean:
	rm *.o
