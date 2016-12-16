#include <stdio.h>

int n;

int main(int argc, char **argv, char **envp)
{
	printf("PIE Success\n");
	printf("0x%016llX\n", (unsigned long long)&n);
	printf("0x%016llX\n", (unsigned long long)&main);
	printf("0x%016llX\n", (unsigned long long)argv);
	return 0;
}