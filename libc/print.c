#include <stdio.h>
#include <print.h>
#include <syscall.h>
#include <string.h>

int print(char const *f)
{
	return write(STDOUT_FILENO, f, strlen(f));
}
