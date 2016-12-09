all: test_elf_reader

test_elf_reader: ../../src/elf_reader.h ../../src/elf_reader.c test_elf_reader.cc
	gcc ../../src/elf_reader.c -o elf_reader.o -c
	g++ test_elf_reader.cc -o test_elf_reader.o -c
	g++ test_elf_reader.o elf_reader.o -o test_elf_reader

clean:
	rm test_elf_reader test_elf_reader.o elf_reader.o