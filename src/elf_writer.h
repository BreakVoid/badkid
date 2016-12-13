#ifndef ELF_WRITER_H_

#define ELF_WRITER_H_
#ifdef __linux__
#include <elf.h>
#else
#include "../include/elf.h"
#endif

#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

int WriteElfHeader(FILE *elfFile, Elf64_Ehdr *elf64Header);

int WriteElfPhdrTab(FILE *elfFile, const Elf64_Ehdr *elf64Header, Elf64_Phdr *elf64PhdrTab);

int WriteElfShdrTab(FILE *elfFile, const Elf64_Ehdr *elf64Header, Elf64_Shdr *elf64ShdrTab);

int WriteElfProgram(FILE *elfFile, const Elf64_Phdr *elf64Phdr, void *content);

int WriteElfSection(FILE *elfFile, const Elf64_Shdr *elf64Shdr, void *content);

#ifdef __cplusplus
}
#endif

#endif
