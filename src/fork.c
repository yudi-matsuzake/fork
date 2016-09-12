#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define USAGE "USAGE %s command [arg0 [arg1 [arg2...]]]\n"
#define UNUSED(X) ((void)X)
#define ERROR(...) \
	{\
		fprintf(stderr, "fatal error: %s(function:%s line:%d):\t", __FILE__, __FUNCTION__, __LINE__);\
		fprintf(stderr, __VA_ARGS__); \
		exit(EXIT_FAILURE); \
	}
#define DEV_NULL_PATH "/dev/null"

void dispatch_child_process(int argc, char* argv[])
{

	UNUSED(argc);
	int dev_null = open(DEV_NULL_PATH, O_APPEND);
	dup2(dev_null, STDOUT_FILENO);
	dup2(dev_null, STDERR_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	if(execvp(argv[1], &argv[1]) == -1)
		ERROR("Cannot execute the command %s\n", argv[1]);
}

int main(int argc, char* argv[])
{

	if(argc <= 1)
		ERROR(USAGE, argv[0]);

	pid_t fork_result = fork();

	if(fork_result == -1)
		ERROR("Cannot fork the process\n");
	if(fork_result == 0)
		dispatch_child_process(argc, argv);

	return 0;
}
