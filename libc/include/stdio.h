#ifndef _LIBC_STDIO_H_
#define _LIBC_STDIO_H_

#include "stdint.h"

#define STDIN_FILENO 0
#define STDOUT_FILENO 1
#define STDERR_FILENO 2

ssize_t read(int fd, void *buf, size_t count);
ssize_t write(int fd, const void *buf, size_t count);

#endif
