#ifndef _COMMAND_H_
#define _COMMAND_H_

#define LINESZ 256
#define ARGMAX 256

struct cmd {
	struct cmd *next;
	unsigned int argc;
	int in;
	int out;
	char *argv[ARGMAX];
	int (*f)(struct cmd *c);
};

int getcmd(struct cmd **c);
int runcmd(struct cmd *c);

#endif
