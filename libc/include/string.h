#ifndef _LIBC_STRING_H_
#define _LIBC_STRING_H_

#include "stdint.h"

size_t strlen(char const *s);
int strcmp(char const *s1, char const *s2);
int strncmp(const char *s1, const char *s2, size_t n);
char *strcpy(char *dest, char const *src);
char *strncpy(char *dest, char const *src, size_t n);
char* strcat(char *dest, const char *src);
char* strncat(char *dest, const char *src, size_t n);
int atoi(const char *nptr);

#endif
