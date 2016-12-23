#include <cstdio>

int main()
{
    FILE *soFile = fopen("../libvirus/libvirus.so", "rb");
    FILE *strFile = fopen("data.cc", "w");
    fputs(
        "extern const char virusData[] = \n",
        strFile
    );
    static unsigned char buffer[128] = {0};
    size_t numBytes;
    do {
        numBytes = fread(buffer, 1, 64, soFile);
        fputs("\t\"", strFile);
        for (int i = 0; i < numBytes; ++i) {
            fprintf(strFile, "\\x%x", buffer[i]);
        }
        if (numBytes < 64) {
            fputs("\";", strFile);    
        } else {
            fputs("\"\n", strFile);
        }
    } while (numBytes == 64);

    return 0;
}