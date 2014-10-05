#ifndef _LIBC_BRK_H_
#define _LIBC_BRK_H_

#include <stdint.h>

int brk(void *addr);
void *sbrk(intptr_t increment);

#endif
