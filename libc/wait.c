#include <wait.h>
#include <syscall.h>

pid_t waitpid(pid_t pid, int *status, int options)
{
	return syscall(__NR_waitpid, pid, status, options);
}

pid_t wait(int *status)
{
	return waitpid(-1, status, 0);
}

