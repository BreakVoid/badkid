#include <dlfcn.h>

int main()
{
    void *handle = dlopen("libvirus.so", RTLD_LAZY | RTLD_LOCAL);
    return 0;
}