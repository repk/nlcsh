#include <fork.h>
#include <syscall.h>


pid_t fork(void)
{
	return syscall(__NR_fork);
}
