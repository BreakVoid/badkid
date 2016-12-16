all: test_elf_reader pie

pie:
	$(MAKE) -C ../simplest_pie all
	cp ../simplest_pie/simplest_pie ./pie.elf

test_elf_reader: ../../src/elf_reader.h ../../src/elf_reader.c test_elf_reader.cc
	gcc ../../src/elf_reader.c -o elf_reader.o -c -std=c11
	g++ test_elf_reader.cc -o test_elf_reader.o -c -std=c++11
	g++ test_elf_reader.o elf_reader.o -o test_elf_reader

clean:
	-rm test_elf_reader test_elf_reader.o elf_reader.o pie.elf

run: all
	./test_elf_reader