#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <getopt.h>

#define REDIRECT_CREATE		O_APPEND|O_RDWR|O_CREAT
#define REDIRECT_PERMISSION	S_IRUSR|S_IWUSR
#define UNUSED(X) ((void)X)
#define ERROR(...) \
	{\
		fprintf(stderr, "fatal error: %s(function:%s line:%d):\t", __FILE__, __FUNCTION__, __LINE__);\
		fprintf(stderr, __VA_ARGS__); \
		exit(EXIT_FAILURE); \
	}
#define DEV_NULL_PATH "/dev/null"

#define ARG_USAGE "[--stdout <file>] [--stdin <file>] [--stderr <file>] <command [args...]>"
#define HELP					\
	"-h --help			\n"	\
	"	Show this message.	\n" 	\
	"-o <file> --stdout <file>	\n"	\
	"	Redirects the stdout to <file>."\

static struct option long_options[] =
{
	{"stdout",	required_argument,	0, 'o'},
	{"stdin",	required_argument,	0, 'i'},
	{"stderr",	required_argument,	0, 'e'},
	{"help",	no_argument,		0, 'h'},
	{0,	0,	0,	0}
};

struct fork_option {
	char redirect_stdout[BUFSIZ];
	char redirect_stdin[BUFSIZ];
	char redirect_stderr[BUFSIZ];

} fork_option = {DEV_NULL_PATH, DEV_NULL_PATH, DEV_NULL_PATH};

int parse_args(int argc, char* argv[])
{
	int c;
	int option_index = 0;

	while((c = getopt_long (argc,
				argv,
				"o:i:e:h",
				long_options,
				&option_index)) != -1){

		switch(c){
		case 'h':
			printf("USAGE: %s %s\n", argv[0], ARG_USAGE);
			printf("%s\n", HELP);
			return -1;
		case 'o':
			strcpy(fork_option.redirect_stdout, optarg);
			break;
		case 'i':
			strcpy(fork_option.redirect_stdin, optarg);
			break;
		case 'e':
			strcpy(fork_option.redirect_stderr, optarg);
			break;
		default:
			printf("USAGE: %s %s\n", argv[0], ARG_USAGE);
			printf("%s\n", HELP);
			ERROR("Invalid option\n");
			break;
		}
	}

	return optind;
}

void dispatch_child_process(int argc, char* argv[], char* envp[], int opt_index)
{
	UNUSED(argc);
	int in = open(fork_option.redirect_stdin,  REDIRECT_CREATE, REDIRECT_PERMISSION);
	int err= open(fork_option.redirect_stderr, REDIRECT_CREATE|O_TRUNC, REDIRECT_PERMISSION);
	int out= open(fork_option.redirect_stdout, REDIRECT_CREATE|O_TRUNC, REDIRECT_PERMISSION);

	dup2(in, STDIN_FILENO);
	dup2(out, STDOUT_FILENO);
	dup2(err, STDERR_FILENO);

	if(execvpe(argv[opt_index], &argv[opt_index], envp) == -1)
		ERROR("Cannot execute the command %s\n", argv[opt_index]);
}

int main(int argc, char* argv[], char* envp[])
{

	if(argc <= 1)
		ERROR("\nUSAGE: %s %s\n", argv[0], ARG_USAGE);

	int opt_index = parse_args(argc, argv);
	if(opt_index == -1)
		return 0;
	else if(opt_index >= argc)
		ERROR("\nUSAGE: %s %s\n", argv[0], ARG_USAGE);

	pid_t fork_result = fork();

	if(fork_result == -1)
		ERROR("Cannot fork the process\n");
	if(fork_result == 0)
		dispatch_child_process(argc, argv, envp, opt_index);

	return 0;
}
