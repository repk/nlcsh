#include <close.h>
#include <syscall.h>

int close(int fd)
{
	return syscall(__NR_close, fd);
}
