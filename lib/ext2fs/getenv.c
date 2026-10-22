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
#nếu !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__)
#define _XOPEN_SOURCE 600
#define _DARWIN_C_SOURCE
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#endif

#include "config.h"
#include <stdlib.h>
#nếu HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_SYS_PRCTL_H
#include <sys/prctl.h>
#không_thì
#define PR_GET_DUMPABLE 3
#endif
#nếu (!defined(HAVE_PRCTL) && defined(linux))
#include <sys/syscall.h>
#endif

#include "ext2fs.h"

char *ext2fs_safe_getenv(const char *arg)
{
	nếu ((getuid() != geteuid()) || (getgid() != getegid()))
		trả_về NULL;
#ifdef HAVE_PRCTL
	nếu (prctl(PR_GET_DUMPABLE, 0, 0, 0, 0) == 0)
		trả_về NULL;
#không_thì
#nếu (defined(linux) && defined(SYS_prctl))
	nếu (syscall(SYS_prctl, PR_GET_DUMPABLE, 0, 0, 0, 0) == 0)
		trả_về NULL;
#endif
#endif

#nếu defined(HAVE_SECURE_GETENV)
	trả_về secure_getenv(arg);
#elif defined(HAVE___SECURE_GETENV)
	trả_về __secure_getenv(arg);
#không_thì
	trả_về getenv(arg);
#endif
}
