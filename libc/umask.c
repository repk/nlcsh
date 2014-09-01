#include <umask.h>
#include <syscall.h>

mode_t umask(mode_t m)
{
	return syscall(__NR_umask, m);
}
