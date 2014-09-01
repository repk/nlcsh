.globl _start

#sys_execve put argv[] list onto the stack but main need a pointer to
#this list
#_start:
#	call main
#	mov %eax, %ebx
#	mov $1, %eax
#	int $0x80

_start:
	pop %ebx	#get argc
	mov %esp, %eax	#Set eax to argv[] pointer
	push %eax	#Set char **argv as main's last argument
	push %ebx	#Set int argc as main's first argument
	call main	#Call main
	mov %eax, %ebx	#Prepare main's return value as argument to sys_exit()
	mov $1, %eax	#Tell syscall() that we want to exec sys_exit()
	int $0x80	#Raise interrupt to call syscall()
