#include <stdlib.h>
#include <stdio.h>
#include <gets.h>
#include <syscall.h>

char *gets(char *s, size_t l)
{
	int nr;

	if(l == 0)
		return NULL;

	nr = read(STDIN_FILENO, s, l - 1);
	if(nr < 0)
		return NULL;

	s[nr] = '\0';

	return s;
}
