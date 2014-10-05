#ifndef _LIBC_MALLOC_H_
#define _LIBC_MALLOC_H_

#include <stdint.h>

void free(void *p);
void *malloc(size_t size);
void *calloc(size_t nmemb, size_t size);
void *realloc(void *ptr, size_t size);


#endif
