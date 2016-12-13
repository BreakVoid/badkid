#ifndef ELF_UTILITY_H_
#define ELF_UTILITY_H_

#include "elf_reader.h"
#include "elf_writer.h"

#ifdef __cplusplus
extern "C" {
#endif

struct OffInSeg {
	int index;
	unsigned long long off;
};

#ifdef __cplusplus
}
#endif

#endif