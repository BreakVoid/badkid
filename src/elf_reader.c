#include "elf_reader.h"

#include <stdlib.h>

/**
 * check if a file is elf file.
 * @param  elfFile file pointer
 * @return         YES: 1, NO: 0
 */
int CheckIsElf(FILE *elfFile)
{
	if (fseek(elfFile, 0, SEEK_SET) != 0) {
		if (ferror(elfFile)) {
			perror("ReadElfHeader: Error: fseek()\n");
			fprintf(stderr,"fseek() failed in file %s at line # %d\n", __FILE__,__LINE__ - 3);
			return 0;
		}
	}
	char s[4];
	if (fread(s, 1, 4, elfFile) != 4) {
		perror("CheckIsElf: Error: fread() error\n");
		return 0;
	}
	if (s[0] == 0x7f) {
		if (s[1] == 0x45) {
			if (s[2] == 0x4c) {
				if (s[3] == 0x46) {
					return 1;
				}
			}
		}
	}
	return 0;
}

/**
 * read a 64bit LSB ELF file header
 * @param in  elfFile     elf file
 * @param out elf64Header pointer to data destination
 */
int ReadElfHeader(FILE *elfFile, Elf64_Ehdr *elf64Header)
{
	if (fseek(elfFile, 0, SEEK_SET) != 0) {
		if (ferror(elfFile)) {
			perror("ReadElfHeader: Error: fseek()\n");
			fprintf(stderr,"fseek() failed in file %s at line # %d\n", __FILE__,__LINE__ - 3);
			return -1 ;
		}
	}
	if (fread(elf64Header, sizeof(Elf64_Ehdr), 1, elfFile) != 1) {
		perror("ReadElfHeader: Error: fread() error, data may be incorrect.\n");
		return -1;
	}
	return 0;
}

/**
 * read elf program header table
 * @param in  elfFile      elf file
 * @param in  elf64Header  elf header for e_phoff and e_phnum
 * @param out elf64PhdrTab pointer to data destination
 */
int ReadElfPhdrTab(FILE *elfFile, const Elf64_Ehdr *elf64Header, Elf64_Phdr *elf64PhdrTab)
{
	int phnum = elf64Header->e_phnum;
	if (phnum == 0 && elf64Header->e_phoff == 0) {
		/* No Program in the File */
		return -1;
	}
	if (!elf64PhdrTab) {
		perror("ReadElfPhdrTab: Error: elf64PhdrTab points to a invalid area.\n");
		return -1;
	}
	if (fseek(elfFile, elf64Header->e_phoff, SEEK_SET) != 0) {
		if (ferror(elfFile)) {
			perror("ReadElfPhdrTab: Error: fseek()\n");
			fprintf(stderr,"fseek() failed in file %s at line # %d\n", __FILE__,__LINE__ - 3);
			return -1;
		}
	}
	if (fread(elf64PhdrTab, sizeof(Elf64_Phdr), phnum, elfFile) != phnum) {
		perror("ReadElfPhdrTab: Error: fread() error, data may be incorrect.\n");
		return -1;
	}
	return 0;
}

/**
 * read elf section header table
 * @param in  elfFile      elf file
 * @param in  elf64Header  elf header for e_phoff and e_phnum
 * @param out elf64ShdrTab pointer to data destination
 */
int ReadElfShdrTab(FILE *elfFile, const Elf64_Ehdr *elf64Header, Elf64_Shdr *elf64ShdrTab)
{
	int shnum = elf64Header->e_shnum;
	if (shnum == 0 && elf64Header->e_shoff == 0) {
		/* No Sections in the File */
		return -1;
	}
	if (!elf64ShdrTab) {
		perror("ReadElfShdrTab: Error: elf64ShdrTab points to a invalid area.\n");
		return -1;
	}
	if (fseek(elfFile, elf64Header->e_shoff, SEEK_SET) != 0) {
		if (ferror(elfFile)) {
			perror("ReadElfShdrTab: Error: fseek()\n");
			fprintf(stderr,"fseek() failed in file %s at line # %d\n", __FILE__,__LINE__ - 3);
			return -1;
		}
	}
	if (fread(elf64ShdrTab, sizeof(Elf64_Shdr), shnum, elfFile) != shnum) {
		perror("ReadElfShdrTab: Error: fread() error, data may be incorrect.\n");
		return -1;
	}
	return 0;
}

int ReadElfProgram(FILE *elfFile, const Elf64_Phdr *elf64Phdr, void *content)
{
	if (elf64Phdr->p_filesz == 0) {
		return 0;
	}
	if (fseek(elfFile, elf64Phdr->p_offset, SEEK_SET) != 0) {
		if (ferror(elfFile)) {
			perror("ReadElfProgram: Error: fseek()\n");
			fprintf(stderr,"fseek() failed in file %s at line # %d\n", __FILE__, __LINE__ - 3);
			return -1;
		}
	}
	if (elf64Phdr->p_filesz != 0 && !content) {
		perror("WriteElfProgram: Error: content points to an invalid area.\n");
		return -1;
	}
	if (fread(content, elf64Phdr->p_filesz, 1, elfFile) != 1) {
		perror("ReadElfProgram: Error: fread() error, data may be incorrect.\n");
		return -1;
	}
	return 0;
}

int ReadElfSection(FILE *elfFile, const Elf64_Shdr *elf64Shdr, void *content)
{
	if (elf64Shdr->sh_size == 0) {
		return 0;
	}
	if (fseek(elfFile, elf64Shdr->sh_offset, SEEK_SET) != 0) {
		if (ferror(elfFile)) {
			perror("ReadElfSection: Error: fseek()\n");
			fprintf(stderr,"fseek() failed in file %s at line # %d\n", __FILE__, __LINE__ - 3);
			return -1;
		}
	}
	if (elf64Shdr->sh_size != 0 && !content) {
		perror("WriteElfSection: Error: content points to an invalid area.\n");
		return -1;
	}
	if (fread(content, elf64Shdr->sh_size, 1, elfFile) != 1) {
		perror("ReadElfSection: Error: fread() error, data may be incorrect.\n");
		return -1;
	}
	return 0;
}