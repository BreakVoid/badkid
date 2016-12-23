//
// readself.h
// Created by 柯嵩宇 on 2016/12/20.
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

extern char selfName[2048] = {0};

void GetSelf()
{
	readlink("/proc/self/exe", selfName, 2048);
}

size_t GetSelfSize()
{
	struct stat fileStat;
	stat(selfName, &fileStat);
	return fileStat.st_size;
}

void ReadSelf(void *buffer)
{
	FILE *self = fopen(selfName, "rb");
	while (fread(buffer, 1, 64, self) == 64) {
		buffer += 64;
	}
}
