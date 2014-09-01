#include <stdlib.h>
#include <stdio.h>
#include <print.h>
#include <env.h>
#include "command.h"

#define PROMPT "> "

static void prompt(void)
{
	print(PROMPT);
}

/**
 * Ether fix crt.S to push $esp in order for argv to be a pointer to arg list or
 * use char **env = (char **)(&argv + argc + 1) to get env
 */
int main(int argc, char *argv[])
{
	struct cmd *c = NULL;

	initenv(argc, argv);

	prompt();

	while(getcmd(&c) >= 0) {
		runcmd(c);
		prompt();
	}

	return 0;
}
