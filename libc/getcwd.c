#include <getcwd.h>
#include <syscall.h>

char *getcwd(char *buf, size_t size)
{
	return (char *)syscall(__NR_getcwd, buf, size);
}
