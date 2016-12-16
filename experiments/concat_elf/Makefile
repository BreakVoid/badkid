all: pie main

pie: 
	$(MAKE) -C ../simplest_pie all
	cp ../simplest_pie/simplest_pie ./pie.elf

main:
	$(MAKE) -C ../../src clean
	$(MAKE) -C ../../src elf_reader.o elf_writer.o
	cp ../../src/*.o ./
	gcc -c -o concat_elf.o concat_elf.c -std=c11 -fPIE -g
	gcc -o concat_elf concat_elf.o elf_reader.o elf_writer.o -pie -g

run: all
	./concat_elf

clean:
	-rm pie.elf *.o concat_elf