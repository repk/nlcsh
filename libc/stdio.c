#include <stdio.h>
#include <syscall.h>

ssize_t read(int fd, void *buf, size_t count)
{
	return syscall(__NR_read, fd, buf, count);
}

ssize_t write(int fd, const void *buf, size_t count)
{
	return syscall(__NR_write, fd, buf, count);
}
