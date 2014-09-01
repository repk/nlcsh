#ifndef _LIBC_VARG_H_
#define _LIBC_VARG_H_

#include "stdlib.h"

struct va_list {
	unsigned char *cur;
};

#define va_start(a, last) ((a).cur = (unsigned char *)(&(last)))
#define va_arg(a, type) (*((type *)((a).cur = (a).cur + sizeof(type))))
#define va_end(a) ((a).cur = NULL)


#endif
