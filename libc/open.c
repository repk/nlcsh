#include <open.h>
#include <syscall.h>
#include <varg.h>

int open(const char *pathname, int flags, ...)
{
	mode_t mode = 0;

	if(flags & O_CREAT) {
		struct va_list ap;
		va_start(ap, flags);
		mode = va_arg(ap, mode_t);
		va_end(ap);
	}


	return syscall(__NR_open, pathname, flags | O_LARGEFILE, mode);
}
