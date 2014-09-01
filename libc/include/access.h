#ifndef _LIBC_ACCESS_H_
#define _LIBC_ACCESS_H_

#define R_OK	4 /* Test for read permission.  */
#define W_OK	2 /* Test for write permission.  */
#define X_OK	1 /* Test for execute permission.  */
#define F_OK	0 /* Test for existence.  */

int access(char const *pathname, int mode);

#endif
