#include "elf_writer.h"

#include <stdlib.h>

int WriteElfHeader(FILE *elfFile, Elf64_Ehdr *elf64Header)
{
	if (fseek(elfFile, 0, SEEK_SET) != 0) {
		if (ferror(elfFile)) {
			perror("WriteElfHeader: Error: fseek()\n");
			fprintf(stderr,"fseek() failed in file %s at line # %d\n", __FILE__,__LINE__ - 3);
			return -1;
		}
	}
	if (fwrite(elf64Header, sizeof(Elf64_Ehdr), 1, elfFile) != 1) {
		perror("WriteElfHeader: Error: fwrite() error.\n");
		return -1;
	}
	return 0;
}

int WriteElfPhdrTab(FILE *elfFile, const Elf64_Ehdr *elf64Header, Elf64_Phdr *elf64PhdrTab)
{
	int phnum = elf64Header->e_phnum;
	if (phnum == 0 && elf64Header->e_phoff == 0) {
		/* No Program in the File */
		return 0;
	}
	if (!elf64PhdrTab) {
		perror("WriteElfPhdrTab: Error: elf64PhdrTab points to a invalid area.\n");
		return -1;
	}
	if (fseek(elfFile, elf64Header->e_phoff, SEEK_SET) != 0) {
		if (ferror(elfFile)) {
			perror("WriteElfPhdrTab: Error: fseek()\n");
			fprintf(stderr,"fseek() failed in file %s at line # %d\n", __FILE__,__LINE__ - 3);
			return -1;
		}
	}
	if (fwrite(elf64PhdrTab, sizeof(Elf64_Phdr), phnum, elfFile) != phnum) {
		perror("WriteElfPhdrTab: Error: fwrite() error.\n");
		return -1;
	}
	return 0;
}

int WriteElfShdrTab(FILE *elfFile, const Elf64_Ehdr *elf64Header, Elf64_Shdr *elf64ShdrTab)
{
	int shnum = elf64Header->e_shnum;
	if (shnum == 0 && elf64Header->e_shoff == 0) {
		/* No Sections in the File */
		return 0;
	}
	if (!elf64ShdrTab) {
		perror("WriteElfShdrTab: Error: elf64ShdrTab points to a invalid area.\n");
		return -1;
	}
	if (fseek(elfFile, elf64Header->e_shoff, SEEK_SET) != 0) {
		if (ferror(elfFile)) {
			perror("WriteElfShdrTab: Error: fseek()\n");
			fprintf(stderr,"fseek() failed in file %s at line # %d\n", __FILE__,__LINE__ - 3);
			return -1;
		}
	}
	if (fwrite(elf64ShdrTab, sizeof(Elf64_Shdr), shnum, elfFile) != shnum) {
		perror("WriteElfShdrTab: Error: fwrite() error.\n");
		return -1;
	}
	return 0;
}

int WriteElfProgram(FILE *elfFile, const Elf64_Phdr *elf64Phdr, void *content)
{
	if (elf64Phdr->p_filesz == 0) {
		return 0;
	}
	if (fseek(elfFile, elf64Phdr->p_offset, SEEK_SET) != 0) {
		if (ferror(elfFile)) {
			perror("WriteElfProgram: Error: fseek()\n");
			fprintf(stderr,"fseek() failed in file %s at line # %d\n", __FILE__, __LINE__ - 3);
			return -1;
		}
	}
	if (elf64Phdr->p_filesz != 0 && !content) {
		perror("WriteElfProgram: Error: content points to an invalid area.\n");
		return -1;
	}
	if (fwrite(content, 1, elf64Phdr->p_filesz, elfFile) != elf64Phdr->p_filesz) {
		perror("WriteElfProgram: Error: fwrite() error.\n");
		return -1;
	}
	return 0;
}

int WriteElfSection(FILE *elfFile, const Elf64_Shdr *elf64Shdr, void *content)
{
	if (elf64Shdr->sh_size == 0) {
		return 0;
	}
	if (fseek(elfFile, elf64Shdr->sh_offset, SEEK_SET) != 0) {
		if (ferror(elfFile)) {
			perror("WriteElfSection: Error: fseek()\n");
			fprintf(stderr,"fseek() failed in file %s at line # %d\n", __FILE__, __LINE__ - 3);
			return -1;
		}
	}
	if (elf64Shdr->sh_size != 0 && !content) {
		perror("WriteElfSection: Error: content points to an invalid area.\n");
		return -1;
	}
	if (fwrite(content, 1, elf64Shdr->sh_size, elfFile) != elf64Shdr->sh_size) {
		perror("WriteElfSection: Error: fwrite() error.\n");
		return -1;
	}
	return 0;
}