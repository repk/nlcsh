#ifndef _LIBC_ENV_H_
#define _LIBC_ENV_H_

void initenv(int argc, char **argv);
char *getenv(char const *name);

#endif
