#include <brk.h>
#include <syscall.h>


int brk(void *addr)
{
	void *b = (void *)syscall(__NR_brk, addr);
	return (b == addr) ? 0 : -1;
}

void *sbrk(intptr_t increment)
{
	void *new, *old = (void *)syscall(__NR_brk, 0);

	new = (void *)syscall(__NR_brk, ((uintptr_t)old) + increment);

	return (((uintptr_t)new) == (((uintptr_t)old) + increment)) ? old :
		(void *)-1;
}
