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
Elf64_Ehdr *ReadElfHeader(FILE *elfFile);

Elf64_Phdr *ReadElfPhdr(FILE *elfFile, const Elf64_Ehdr *elf64Header);

Elf64_Shdr *ReadElfShdr(FILE *elfFile, const Elf64_Ehdr *elf64Header);
#ifdef __cplusplus
}
#endif

#endif
