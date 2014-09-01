#include <exit.h>
#include <syscall.h>

void exit(int status)
{
    for(;;)
        syscall(__NR_exit, status);
}
