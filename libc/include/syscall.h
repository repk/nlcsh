#ifndef _LIBC_SYSCALL_H_
#define _LIBC_SYSCALL_H_

#include "syscallnr.h"

extern long (*__fsyscall0)(int N);
extern long (*__fsyscall1)(int N, long a);
extern long (*__fsyscall2)(int N, long a, long b);
extern long (*__fsyscall3)(int N, long a, long b, long c);
extern long (*__fsyscall4)(int N, long a, long b, long c, long d);
extern long (*__fsyscall5)(int N, long a, long b, long c, long d, long e);

#define syscall0(N) (__fsyscall0(N))
#define syscall1(N, a) (__fsyscall1(N, (long)(a)))
#define syscall2(N, a, b) (__fsyscall2(N, (long)(a), (long)(b)))
#define syscall3(N, a, b, c) (__fsyscall3(N, (long)(a), (long)(b),	\
            (long)(c)))
#define syscall4(N, a, b, c, d) (__fsyscall4(N, (long)(a),		\
        (long)(b), (long)c, (long)d))
#define syscall5(N, a, b, c, d, e) (__fsyscall5(N, (long)(a),		\
        (long)(b), (long)(c), (long)(d), (long)(e)))
#define syscall6(N, a, b, c, d, e, f) (__fsyscall6(N, (long)(a),	\
        (long)(b), (long)(c), (long)(d), (long)(e), (long)(f)))

#define __SYSCALL_NARGS_X(a, b, c, d, e, f, g, n, ...) n
#define __SYSCALL_NARGS(...) __SYSCALL_NARGS_X(__VA_ARGS__, 6, 5, 4, 3, 2, 1, 0)
#define __SYSCALL_NAME_X(a, b) a##b
#define __SYSCALL_NAME(a, b) __SYSCALL_NAME_X(a, b)
#define __SYSCALL_FUNC(n, ...)						\
	__SYSCALL_NAME(n, __SYSCALL_NARGS(__VA_ARGS__))(__VA_ARGS__)

#define syscall(...) __SYSCALL_FUNC(syscall,__VA_ARGS__)
#endif
