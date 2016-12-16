#include <iostream>
#include <unistd.h>

using namespace std;

int main(int argc, char **argv)
{
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
		dup2(pipefds[0], STDIN_FILENO);
		dup2(pipefds[1], STDOUT_FILENO);
		execvp("np1", argv);
	}
	return 0;
}