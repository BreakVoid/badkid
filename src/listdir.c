#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int list_dir(const char *path) {
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir(path)) != NULL) {
		while ((ent = readdir(dir)) != NULL) {
			if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0) {
				size_t pathlength = strlen(path), dlength = strlen(ent->d_name);
				char *newname = malloc(pathlength + dlength + 2);
				strcpy(newname, path);
				newname[pathlength] = '/';
				strcpy(newname + pathlength + 1, ent->d_name);
				newname[pathlength + dlength + 2] = '\0';
				if (!list_dir(newname)) {
					printf("%s\n", newname);
				}
			}
		}
		closedir(dir);
		return 1;
	} else {
		return 0;
	}
}

int main(int argc, const char *argv[]) {
	if (argc > 1) {
		list_dir(argv[1]);
	}
	return 0;
}
