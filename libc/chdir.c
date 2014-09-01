#include <chdir.h>
#include <syscall.h>


int chdir(char const *p)
{
	return syscall(__NR_chdir, p);
}
