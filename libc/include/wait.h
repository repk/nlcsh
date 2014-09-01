#ifndef _LIBC_WAIT_H_
#define _LIBC_WAIT_H_

#include "pid.h"

pid_t wait(int *status);
pid_t waitpid(pid_t pid, int *status, int options);

#endif
