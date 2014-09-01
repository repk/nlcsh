#include <env.h>
#include <string.h>
#include <stdlib.h>

static char **environ;

void initenv(int argc, char **argv)
{
	environ = argv + argc + 1;
}

char *getenv(char const *name)
{
	char **p;
	for(p = environ; *p != NULL; ++p) {
		if(strncmp(name, *p, strlen(name)) == 0 &&
				(*p)[strlen(name)] == '=')
			return *p;
	}

	return NULL;
}
