/*
 * getenv.c --- implement a safe getenv for use by the ext2fs library
 *
 * Copyright (C) 1993, 1994, 1995, 1996, 1997, 1998, 1999, 2000, 2001,
 *	2002 by Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */
#اگر !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__)
#define _XOPEN_SOURCE 600
#define _DARWIN_C_SOURCE
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#endif

#شامل "config.h"
#شامل <stdlib.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#endif
#ifdef HAVE_SYS_PRCTL_H
#شامل <sys/prctl.h>
#وگرنه
#define PR_GET_DUMPABLE 3
#endif
#اگر (!defined(HAVE_PRCTL) && defined(linux))
#شامل <sys/syscall.h>
#endif

#شامل "ext2fs.h"

نویسه *ext2fs_safe_getenv(ثابت نویسه *arg)
{
	اگر ((getuid() != geteuid()) || (getgid() != getegid()))
		بازگشت پوچ;
#ifdef HAVE_PRCTL
	اگر (prctl(PR_GET_DUMPABLE, 0, 0, 0, 0) == 0)
		بازگشت پوچ;
#وگرنه
#اگر (defined(linux) && defined(SYS_prctl))
	اگر (syscall(SYS_prctl, PR_GET_DUMPABLE, 0, 0, 0, 0) == 0)
		بازگشت پوچ;
#endif
#endif

#اگر defined(HAVE_SECURE_GETENV)
	بازگشت secure_getenv(arg);
#وگرنهاگر defined(HAVE___SECURE_GETENV)
	بازگشت __secure_getenv(arg);
#وگرنه
	بازگشت getenv(arg);
#endif
}
