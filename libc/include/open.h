#ifndef _LIBC_OPEN_H_
#define _LIBC_OPEN_H_

#define O_ACCMODE	   0003
#define O_RDONLY	     00
#define O_WRONLY	     01
#define O_RDWR		     02
#define O_CREAT		   0100
#define O_EXCL		   0200
#define O_NOCTTY	   0400
#define O_TRUNC		  01000
#define O_APPEND	  02000
#define O_NONBLOCK	  04000
#define O_NDELAY	O_NONBLOCK
#define O_SYNC	       04010000
#define O_FSYNC		 O_SYNC
#define O_ASYNC		 020000
#define O_LARGEFILE	0100000

/* mode */
#define S_IRWXU  00700 /* user (file owner) has read, write and
			  execute permission */
#define S_IRUSR  00400 /* user has read permission */
#define S_IWUSR  00200 /* user has write permission */
#define S_IXUSR  00100 /* user has execute permission */
#define S_IRWXG  00070 /* group has read, write and execute permission */
#define S_IRGRP  00040 /* group has read permission */
#define S_IWGRP  00020 /* group has write permission */
#define S_IXGRP  00010 /* group has execute permission */
#define S_IRWXO  00007 /* others have read, write and execute permission */
#define S_IROTH  00004 /* others have read permission */
#define S_IWOTH  00002 /* others have write permission */
#define S_IXOTH  00001 /* others have execute permission */

typedef unsigned int mode_t;

int open(const char *pathname, int flags, ...);

#endif
