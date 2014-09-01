#include <dup.h>
#include <syscall.h>

int dup(int oldfd)
{
	return syscall(__NR_dup, oldfd);
}
