#include "../../src/elf_utility.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

const char pieFilename[] = "pie.elf";
const char nonPieFilename[] = "nonpie.elf";
const char dstFilename[] = "fat.elf";

int main(int argc, char const *argv[])
{
	FILE *pieFile = fopen(pieFilename, "rb");
	FILE *nonPieFile = fopen(nonPieFilename, "rb");
	Elf64_Ehdr pieHeader, nonPieHeader;
	ReadElfHeader(pieFile, &pieHeader);
	ReadElfHeader(nonPieFile, &nonPieHeader);
	Elf64_Phdr *piePhdrTab = (Elf64_Phdr *)malloc(sizeof(Elf64_Phdr) * pieHeader.e_phnum);
	Elf64_Phdr *nonPiePhdrTab = (Elf64_Phdr *)malloc(sizeof(Elf64_Phdr) * nonPieHeader.e_phnum);
	ReadElfPhdrTab(pieFile, &pieHeader, piePhdrTab);
	ReadElfPhdrTab(nonPieFile, &nonPieHeader, nonPiePhdrTab);

	void **pieContent = malloc(sizeof(void *) * pieHeader.e_phnum);
	void **nonpieContent = malloc(sizeof(void *) * nonPieHeader.e_phnum);
	for (int i = 0; i < pieHeader.e_phnum; ++i) {
		pieContent[i] = malloc(piePhdrTab[i].p_filesz);
		ReadElfProgram(pieFile, &piePhdrTab[i],	pieContent);
	}
	for (int i = 0; i < nonPieHeader.e_phnum; ++i) {
		nonpieContent[i] = malloc(nonPiePhdrTab[i].p_filesz);
		ReadElfProgram(nonPieFile, &nonPiePhdrTab[i], nonpieContent);
	}

	FILE *dstFile = fopen(dstFilename, "wb");
	if (!dstFile) {
		perror("dstFile cannot open.\n");
	}

	Elf64_Ehdr dstHeader;
	memcpy(&dstHeader, &pieHeader, sizeof(Elf64_Ehdr));
	dstHeader.e_phnum = pieHeader.e_phnum + nonPieHeader.e_phnum;
	
	Elf64_Phdr *dstPhdrTab = (Elf64_Phdr *)malloc(sizeof(Elf64_Phdr) * (pieHeader.e_phnum + nonPieHeader.e_phnum));
	memcpy(dstPhdrTab, piePhdrTab, sizeof(Elf64_Phdr) * pieHeader.e_phnum);
	memcpy(&dstPhdrTab[pieHeader.e_phnum], nonPiePhdrTab, sizeof(Elf64_Phdr) * nonPieHeader.e_phnum);

	void **dstContent = malloc(sizeof(void *) * (pieHeader.e_phnum + nonPieHeader.e_phnum));
	memcpy(dstContent, pieContent, sizeof(void *) * pieHeader.e_phnum);
	memcpy(&dstContent[pieHeader.e_phnum], nonpieContent, sizeof(void *) * nonPieHeader.e_phnum);

	unsigned long long curOff = 0x40;

	for (int i = 0; i < dstHeader.e_phnum; ++i) {
		dstPhdrTab[i].p_offset = curOff;
		curOff += dstPhdrTab[i].p_filesz;
		printf("Current Program %d\n", i);
		printf("file size: %llu\n", dstPhdrTab[i].p_filesz);
		WriteElfProgram(dstFile, &dstPhdrTab[i], dstContent[i]);
	}

	dstHeader.e_phoff = curOff;
	WriteElfPhdrTab(dstFile, &dstHeader, dstPhdrTab);

	dstHeader.e_shoff = dstHeader.e_shnum = 0;
	WriteElfHeader(dstFile, &dstHeader);

	for (int i = 0; i < pieHeader.e_phnum; ++i) {
		free(pieContent[i]);
	}
	for (int i = 0; i < nonPieHeader.e_phnum; ++i) {
		free(nonpieContent[i]);
	}

	free(pieContent);
	free(nonpieContent);
	free(dstContent);
	free(piePhdrTab);
	free(nonPiePhdrTab);
	free(dstPhdrTab);

	fclose(pieFile);
	fclose(nonPieFile);
	fclose(dstFile);
	return 0;
}