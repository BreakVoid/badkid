#include "elf_reader.h"

#include <stdlib.h>

Elf64_Ehdr *ReadElfHeader(FILE *elfFile)
{
	Elf64_Ehdr *elf64Header = malloc(sizeof(Elf64_Ehdr));
	fseek(elfFile, 0, SEEK_SET);
	fread(elf64Header, sizeof(Elf64_Ehdr), 1, elfFile);
	return elf64Header;
}

Elf64_Phdr *ReadElfPhdr(FILE *elfFile, const Elf64_Ehdr *elf64Header)
{
	int phnum = elf64Header->e_phnum;
	if (phnum == 0 && elf64Header->e_phoff == 0) {
		/* No Program in the File */
		return NULL;
	}
	Elf64_Phdr *elf64PhdrTab = malloc(sizeof(Elf64_Phdr) * phnum);
	if (fseek(elfFile, elf64Header->e_phoff, SEEK_SET) != 0) {
		if (ferror(elfFile)) {
			perror("ReadElfPhdr: Error: fseek()\n");
			fprintf(stderr,"fseek() failed in file %s at line # %d\n", __FILE__,__LINE__ - 3);
		}
	}
	fread(elf64PhdrTab, sizeof(Elf64_Phdr), phnum, elfFile);
	return elf64PhdrTab;
}

Elf64_Shdr *ReadElfShdr(FILE *elfFile, const Elf64_Ehdr *elf64Header)
{
	int shnum = elf64Header->e_shnum;
	if (shnum == 0 && elf64Header->e_shoff == 0) {
		/* No Sections in the File */
		return NULL;
	}
	Elf64_Shdr *elf64ShdrTab = malloc(sizeof(Elf64_Shdr) * shnum);
	if (fseek(elfFile, elf64Header->e_shoff, SEEK_SET) != 0) {
		if (ferror(elfFile)) {
			perror("ReadElfShdr: Error: fseek()\n");
			fprintf(stderr,"fseek() failed in file %s at line # %d\n", __FILE__,__LINE__ - 3);
		}
	}
	fread(elf64ShdrTab, sizeof(Elf64_Shdr), shnum, elfFile);
	return elf64ShdrTab;
}

void *ReadElfProgram(FILE *elfFile, const Elf64_Phdr *elf64Phdr)
{
	void *content = malloc(elf64Phdr->p_filesz);
	if (fseek(elfFile, elf64Phdr->p_offset, SEEK_SET) != 0) {
		if (ferror(elfFile)) {
			perror("ReadElfPhdr: Error: fseek()\n");
			fprintf(stderr,"fseek() failed in file %s at line # %d\n", __FILE__, __LINE__ - 3);
		}
	}
	fread(content, elf64Phdr->p_filesz, 1, elfFile);
	return content;
}

void *ReadElfSection(FILE *elfFile, const Elf64_Shdr *elf64Shdr)
{
	void *content = malloc(elf64Shdr->sh_size);
	if (fseek(elfFile, elf64Shdr->sh_offset, SEEK_SET) != 0) {
		if (ferror(elfFile)) {
			perror("ReadElfPhdr: Error: fseek()\n");
			fprintf(stderr,"fseek() failed in file %s at line # %d\n", __FILE__, __LINE__ - 3);
		}
	}
	fread(content, elf64Shdr->sh_size, 1, elfFile);
	return content;
}