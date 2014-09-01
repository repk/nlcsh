#include <access.h>
#include <syscall.h>


int access(char const *pathname, int mode)
{
	return syscall(__NR_access, pathname, mode);
}
