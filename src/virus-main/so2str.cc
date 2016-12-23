#include <cstdio>

int main()
{
    FILE *soFile = fopen("../libvirus/libvirus.so", "rb");
    FILE *strFile = fopen("data.cc", "w");
    fputs(
        "#include <cctype>\n"
        "#include <cstdio>\n",
        strFile
    );
    fputs(
        "extern const char virusData[] = \n",
        strFile
    );

    size_t byteCount = 0;

    static unsigned char buffer[128] = {0};
    size_t numBytes;
    do {
        numBytes = fread(buffer, 1, 64, soFile);
        fputs("\t\"", strFile);
        for (int i = 0; i < numBytes; ++i) {
            fprintf(strFile, "\\x%x", buffer[i]);
        }
        if (numBytes < 64) {
            fputs("\";\n", strFile);    
        } else {
            fputs("\"\n", strFile);
        }
        byteCount += numBytes;
    } while (numBytes == 64);

    fprintf(strFile, "extern const size_t dataLen = %lu;\n", byteCount);
    fclose(strFile);

    return 0;
}