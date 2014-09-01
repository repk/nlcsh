#ifndef _LIBC_EXECVE_H_
#define _LIBC_EXECVE_H_

int execve(const char *filename, char *const argv[],
		char *const envp[]);

#endif
