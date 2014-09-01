#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <print.h>
#include <gets.h>
#include <fork.h>
#include <exit.h>
#include <execve.h>
#include <wait.h>
#include <pipe.h>
#include <close.h>
#include <dup.h>
#include <open.h>
#include <access.h>
#include <env.h>
#include <chdir.h>
#include <getcwd.h>
#include <umask.h>

#include "command.h"

#define CMDMAX 16
#define PATHMAX 1024

/**
 * XXX Could be malloc'd when malloc would be implemented
 */
static struct cmd cmds[CMDMAX];
static char args[ARGMAX][LINESZ];
static int pipes[CMDMAX - 1][2];

struct builtin {
	char *name;
	int (*f)(struct cmd *c);
};

static int changedir(struct cmd *c);
static int getdir(struct cmd *c);
static int changeumask(struct cmd *c);
static int quit(struct cmd *c);

static struct builtin const builtins[] = {
	{"cd", changedir},
	{"pwd", getdir},
	{"umask", changeumask},
	{"quit", quit},
	{NULL, NULL}
};

static inline void initcmd(struct cmd *c)
{
	c->next = NULL;
	c->in = STDIN_FILENO;
	c->out = STDOUT_FILENO;
	c->argc = 0;
}

static int getcmdstring(char c[LINESZ])
{
	if(gets(c, LINESZ) == NULL) {
		return 0;
	}

	return 0;
}

static int parsecmdstring(struct cmd **c, char s[LINESZ])
{
	struct cmd *_c = &cmds[0];
	char *ptr = s;
	size_t i, j, k, len = strlen(s);
	size_t p = 0;

	initcmd(_c);

	for(i = 0, j = 0, k = 0; i < len && j < ARGMAX - 1 && k < ARGMAX; ++i) {
		switch(s[i]) {
		case ' ':
		case '\n':
			/* New command's argument */
			if(ptr != &s[i]) {
				s[i] = '\0';
				strcpy(args[k], ptr);
				_c->argv[j] = args[k];
				++k;
				++j;
			}
			while(i + 1 < len && s[i + 1] == ' ')
				++i;
			ptr = s + i + 1;
			break;
		case '|':
			/* Pipe between two commands */
			if(ptr != &s[i]) {
				s[i] = '\0';
				strcpy(args[k], ptr);
				_c->argv[j] = args[k];
				++k;
			}
			if(pipe(pipes[p]) < 0)
				return -1;
			_c->out = pipes[p][1];
			_c->next = _c + 1;
			++_c;
			initcmd(_c);
			_c->in = pipes[p][0];
			++p;
			j = 0;
			while(i + 1 < len && s[i + 1] == ' ')
				++i;
			ptr = s + i + 1;
			break;
		case '>':
			/* Output to file */
			if(ptr != &s[i]) {
				s[i] = '\0';
				strcpy(args[k], ptr);
				_c->argv[j] = args[k];
				++k;
			}
			while(i + 1 < len && s[i + 1] == ' ')
				++i;
			ptr = s + i + 1;
			++i;
			while(i < len && s[i] != ' ' && s[i] != '\n')
				++i;
			s[i] = '\0';
			_c->out = open(ptr, O_WRONLY | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
			if(_c->out < 0)
				return -1;
			i = len;
			break;
		case '<':
			/* Input from file */
			if(ptr != &s[i]) {
				s[i] = '\0';
				strcpy(args[k], ptr);
				_c->argv[j] = args[k];
				++k;
			}
			while(i + 1 < len && s[i + 1] == ' ')
				++i;
			ptr = s + i + 1;
			++i;
			while(i < len && s[i] != ' ' && s[i] != '\n')
				++i;
			s[i] = '\0';
			_c->in = open(ptr, O_RDONLY | O_CREAT, S_IRWXU | S_IRWXG | S_IRWXO);
			if(_c->in < 0)
				return -1;
			i = len;
			break;
		}
	}
	_c->argv[j] = (char *)NULL;

	*c = &cmds[0];

	return 0;
}

static int pathexist(char const *path)
{
	if(access(path, X_OK) != 0)
		return 0;
	return 1;
}

static int forkcmd(struct cmd *c)
{
	pid_t p;
	int status;

	p = fork();

	switch(p) {
	case -1:
		return -1;
	case 0:
		if(c->out != STDOUT_FILENO) {
			close(STDOUT_FILENO);
			dup(c->out);
		}
		if(c->in != STDIN_FILENO) {
			close(STDIN_FILENO);
			dup(c->in);
		}
		execve(c->argv[0], c->argv, NULL);
		exit(-1);
	default:
		if(c->out != STDOUT_FILENO)
			close(c->out);

		if(c->in != STDIN_FILENO)
			close(c->in);

		if(c->next != NULL)
			runcmd(c->next);

		waitpid(p, &status, 0);

	}

	return 0;
}

static int changedir(struct cmd *c)
{
	if(c->argv[1] == NULL) {
		/* TODO go to home dir */
		return -1;
	}

	return chdir(c->argv[1]);
}

static int getdir(struct cmd *c)
{
	char p[PATHMAX];
	(void)c;

	if(getcwd(p, PATHMAX) == NULL) {
		print("Cannot get working dir\n");
		return -1;
	}

	print(p);
	print("\n");

	return 0;
}

static int changeumask(struct cmd *c)
{
	size_t i;
	int nb;
	mode_t m = 0;
	char ms[6];

	if(c->argv[1] == NULL) {
		m = umask(0);
		umask(m);
		for(i = 0; i < 5 && m > 1; ++i) {
			ms[i] = m % 8 + '0';
			m /= 8;
		}
		ms[i] = '\0';
		print(ms);
		print("\n");
		return 0;
	}


	for(i = 0; c->argv[1][i] != 0; ++i) {
		nb = c->argv[1][i] - '0';
		m = m * 8 + nb;
	}

	umask(m);

	return 0;
}

static int quit(struct cmd *c)
{
	(void)c;
	exit(0);
	return 0;
}

static int getcmdpath(struct cmd *c)
{
	char *prog = c->argv[0];
	char *path;
	char fp[PATHMAX];
	char *ptr, *begin;
	size_t len;
	size_t i;

	if(prog[0] == '.' || prog[0] == '/')
		return 0;

	for(i = 0; builtins[i].name != NULL; ++i) {
		if(strcmp(prog, builtins[i].name) == 0) {
			c->f = builtins[i].f;
			return 0;
		}
	}

	c->f = forkcmd;

	path = getenv("PATH");
	if(path == NULL)
		return -1;

	begin = path + sizeof("PATH=") - 1;
	for(ptr = begin; *ptr != '\0'; ++ptr) {
		if(*ptr != ':')
			continue;
		len = ptr - begin;
		if(len >= PATHMAX - 1)
			return -1;
		strncpy(fp, begin, len);
		fp[len] = '/';
		fp[len + 1] = '\0';
		strncat(fp, prog, PATHMAX - 1);
		if(pathexist(fp)) {
			strcpy(prog, fp);
			return 0;
		}
		begin = ptr + 1;
	}

	return -1;

}

int getcmd(struct cmd **c)
{
	int ret;
	char s[LINESZ];

	ret = getcmdstring(s);
	if(ret < 0)
		return ret;

	return parsecmdstring(c, s);
}

int runcmd(struct cmd *c)
{
	if(c->argv[0] == NULL)
		return -1;

	getcmdpath(c);

	return c->f(c);
}
