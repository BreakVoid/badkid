all: pie main

pie: 
	$(MAKE) -C ../simplest_pie all
	cp ../simplest_pie/simplest_pie ./pie.elf

main:
	$(MAKE) -C ../../src clean
	$(MAKE) -C ../../src elf_reader.o elf_writer.o
	cp ../../src/*.o ./
	gcc -c -o shift_pie_elf.o shift_pie_elf.c -std=c11 -fPIE
	gcc -o shift_pie_elf shift_pie_elf.o elf_reader.o elf_writer.o -pie

run: all
	./shift_pie_elf

clean:
	-rm pie.elf shift_pie_elf.o elf_utility.o