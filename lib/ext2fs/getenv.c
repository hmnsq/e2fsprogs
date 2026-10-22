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
#如果 !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__)
#define _XOPEN_SOURCE 600
#define _DARWIN_C_SOURCE
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#endif

#include "config.h"
#include <stdlib.h>
#如果 HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_SYS_PRCTL_H
#include <sys/prctl.h>
#否则
#define PR_GET_DUMPABLE 3
#endif
#如果 (!defined(HAVE_PRCTL) && defined(linux))
#include <sys/syscall.h>
#endif

#include "ext2fs.h"

char *ext2fs_safe_getenv(const char *arg)
{
	如果 ((getuid() != geteuid()) || (getgid() != getegid()))
		返回 NULL;
#ifdef HAVE_PRCTL
	如果 (prctl(PR_GET_DUMPABLE, 0, 0, 0, 0) == 0)
		返回 NULL;
#否则
#如果 (defined(linux) && defined(SYS_prctl))
	如果 (syscall(SYS_prctl, PR_GET_DUMPABLE, 0, 0, 0, 0) == 0)
		返回 NULL;
#endif
#endif

#如果 defined(HAVE_SECURE_GETENV)
	返回 secure_getenv(arg);
#elif defined(HAVE___SECURE_GETENV)
	返回 __secure_getenv(arg);
#否则
	返回 getenv(arg);
#endif
}
