#include <stdio.h>
#include <stdlib.h>

#include "../../src/elf_utility.h"

char *pieFilename = "pie.elf";
char *dstFilename = "shifted_pie.elf";

int main(int argc, char const *argv[])
{
	FILE *pieFile = fopen(pieFilename, "rb");
	if (!CheckIsElf(pieFile)) {
		perror("not a ELF file");
		return -1;
	}
	Elf64_Ehdr elfHeader;
	ReadElfHeader(pieFile, &elfHeader);

	Elf64_Phdr *elfPhdrTab = (Elf64_Phdr *) malloc(sizeof(Elf64_Phdr) * elfHeader.e_phnum);
	ReadElfPhdrTab(pieFile, &elfHeader, elfPhdrTab);
	struct OffInSeg entPoint;
	void **content = (void **) malloc(sizeof(void *) * elfHeader.e_phnum);
	for (int i = 0; i < elfHeader.e_phnum; ++i) {
		content[i] = malloc(elfPhdrTab[i].p_filesz);
		if (ReadElfProgram(pieFile, &elfPhdrTab[i], content[i])) {
			printf("Error Occurs at Program %d\n", i);
		}
		if (elfPhdrTab[i].p_vaddr <= elfHeader.e_entry && elfHeader.e_entry <= elfPhdrTab[i].p_vaddr + elfPhdrTab[i].p_memsz) {
			entPoint.index = i;
			entPoint.off = elfHeader.e_entry - elfPhdrTab[i].p_vaddr;
		}
		printf("Seg %i, File Offset Range:[0x%llX, 0x%llX]\n", i, elfPhdrTab[i].p_offset, elfPhdrTab[i].p_offset + elfPhdrTab[i].p_filesz);
	}
	fclose(pieFile);
	for (int i = 0; i < elfHeader.e_phnum; ++i) {
		printf("Program $%d:\n", i);
		printf("\tPrevious Virtual Address: 0x%016llX\n", elfPhdrTab[i].p_vaddr);
		elfPhdrTab[i].p_vaddr = elfPhdrTab[i].p_paddr = elfPhdrTab[i].p_vaddr + 0x300000;
		printf("\tModified Virtual Address: 0x%016llX\n", elfPhdrTab[i].p_vaddr);
	}

	elfHeader.e_entry = entPoint.off + elfPhdrTab[entPoint.index].p_vaddr;
	elfHeader.e_shoff = 0ULL;
	elfHeader.e_shnum = 0;
	elfHeader.e_type = ET_EXEC;
	
	FILE *shiftedPieFile = fopen(dstFilename, "wb");

	for (int i = 0; i < elfHeader.e_phnum; ++i) {
		WriteElfProgram(shiftedPieFile, &elfPhdrTab[i], content[i]);
	}

	WriteElfHeader(shiftedPieFile, &elfHeader);

	WriteElfPhdrTab(shiftedPieFile, &elfHeader, elfPhdrTab);

	free(elfPhdrTab);
	for (int i = 0; i < elfHeader.e_phnum; ++i) {
		free(content[i]);
	}
	free(content);
	fclose(shiftedPieFile);
	return 0;
}