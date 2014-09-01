#include <execve.h>
#include <syscall.h>


int execve(const char *filename, char *const argv[],
		char *const envp[])
{
	return syscall(__NR_execve, filename, argv, envp);
}
