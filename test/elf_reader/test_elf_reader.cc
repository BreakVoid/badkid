#include "../../src/elf_reader.h"
#include <iostream>

int main(int argc, char const *argv[])
{
	FILE *elfFile = fopen("CF-267B.elf", "rb");
	if (!elfFile) {
		printf("Open File Failed.\n");
		return 0;
	}
	Elf64_Ehdr *elfHeader = ReadElfHeader(elfFile);

	const char *wordWidth, *endian;
	if (elfHeader->e_ident[0] != 0x7F 
		|| elfHeader->e_ident[1] != 0x45 
		|| elfHeader->e_ident[2] != 0x4C 
		|| elfHeader->e_ident[3] != 0x46) {
		printf("Error: not a elf file.");
		return 0;
	}

	int flag = 0;
	if (elfHeader->e_ident[4] == 0x1) {
		wordWidth = "32 bit";
		flag = 1;
	} else if (elfHeader->e_ident[4] == 0x2) {
		wordWidth = "64 bit";
	} else {
		wordWidth = "Unknown Word Width";
		flag = 1;
	}
	if (elfHeader->e_ident[5] == 0x1) {
		endian = "Little Endian";
	} else if (elfHeader->e_ident[5] == 0x2) {
		endian = "Big Endian";
		flag = 1;
	} else {
		endian = "Unknown Endian";
		flag = 1;
	}

	printf("File Format: %s, %s ELF File\n", wordWidth, endian);

	if (flag) {
		printf("Error: not a file with 64 bit and little endian.\n");
		return 0;
	}

	Elf64_Phdr *elfPhdrTab = ReadElfPhdr(elfFile, elfHeader);

	if (elfPhdrTab) {
		printf("Number of Programs: %d\n", elfHeader->e_phnum);
		for (int i = 0; i < elfHeader->e_phnum; ++i) {
			printf("Program $%d\n", i);
			printf("\tFile Offset       0x%016llX\n", elfPhdrTab[i].p_offset);
			printf("\tVirtual Address   0x%016llX\n", elfPhdrTab[i].p_vaddr);
			printf("\tPhysical Address  0x%016llX\n", elfPhdrTab[i].p_paddr);
			printf("\tFile Size         0x%016llX\n", elfPhdrTab[i].p_filesz);
			printf("\tMemory Size       0x%016llX\n", elfPhdrTab[i].p_memsz);
			char readbit = ' ', writebit = ' ', execbit = ' ';
			if (elfPhdrTab[i].p_flags & 4) {
				readbit = 'R';
			}
			if (elfPhdrTab[i].p_flags & 2) {
				writebit = 'W';
			}
			if (elfPhdrTab[i].p_flags & 1) {
				execbit = 'E';
			}
			printf("\tFlags             %c%c%c\n", readbit, writebit, execbit);
		}
	}

	Elf64_Shdr *elfShdrTab = ReadElfShdr(elfFile, elfHeader);
	if (elfShdrTab) {
		printf("Number of Sections: %d\n", elfHeader->e_shnum);
		for (int i = 0; i < elfHeader->e_phnum; ++i) {
			printf("Sections $%d\n", i);
			printf("\tFile Offset       0x%016llX\n", elfPhdrTab[i].p_offset);
			printf("\tVirtual Address   0x%016llX\n", elfPhdrTab[i].p_vaddr);
			printf("\tPhysical Address  0x%016llX\n", elfPhdrTab[i].p_paddr);
			printf("\tFile Size         0x%016llX\n", elfPhdrTab[i].p_filesz);
			printf("\tMemory Size       0x%016llX\n", elfPhdrTab[i].p_memsz);
			char readbit = ' ', writebit = ' ', execbit = ' ';
			if (elfPhdrTab[i].p_flags & 4) {
				readbit = 'R';
			}
			if (elfPhdrTab[i].p_flags & 2) {
				writebit = 'W';
			}
			if (elfPhdrTab[i].p_flags & 1) {
				execbit = 'E';
			}
			printf("\tFlags             %c%c%c\n", readbit, writebit, execbit);
		}
	}
	return 0;
}