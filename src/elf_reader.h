#ifndef ELF_READER_H_

#define ELF_READER_H_
#ifdef __linux__
#include <elf.h>
#else
#include "../include/elf.h"
#endif

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * check if a file is elf file.
 * @param  elfFile file pointer
 * @return         YES: 1, NO: 0
 */
int CheckIsElf(FILE *elfFile);

/**
 * read a 64bit LSB ELF file header
 * @param in  elfFile     elf file
 * @param out elf64Header pointer to data destination
 */
int ReadElfHeader(FILE *elfFile, Elf64_Ehdr *elf64Header);

/**
 * read elf program header table
 * @param in  elfFile      elf file
 * @param in  elf64Header  elf header for e_phoff and e_phnum
 * @param out elf64PhdrTab pointer to data destination
 */
int ReadElfPhdrTab(FILE *elfFile, const Elf64_Ehdr *elf64Header, Elf64_Phdr *elf64PhdrTab);

/**
 * read elf section header table
 * @param in  elfFile      elf file
 * @param in  elf64Header  elf header for e_phoff and e_phnum
 * @param out elf64ShdrTab pointer to data destination
 */
int ReadElfShdrTab(FILE *elfFile, const Elf64_Ehdr *elf64Header, Elf64_Shdr *elf64ShdrTab);

int ReadElfProgram(FILE *elfFile, const Elf64_Phdr *elf64Phdr, void *buffer);

int ReadElfSection(FILE *elfFile, const Elf64_Shdr *elf64Shdr, void *buffer);

#ifdef __cplusplus
}
#endif

#endif
