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
#если !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__)
#define _XOPEN_SOURCE 600
#define _DARWIN_C_SOURCE
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#endif

#include "config.h"
#include <stdlib.h>
#если HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_SYS_PRCTL_H
#include <sys/prctl.h>
#иначе
#define PR_GET_DUMPABLE 3
#endif
#если (!defined(HAVE_PRCTL) && defined(linux))
#include <sys/syscall.h>
#endif

#include "ext2fs.h"

char *ext2fs_safe_getenv(const char *arg)
{
	если ((getuid() != geteuid()) || (getgid() != getegid()))
		вернуть NULL;
#ifdef HAVE_PRCTL
	если (prctl(PR_GET_DUMPABLE, 0, 0, 0, 0) == 0)
		вернуть NULL;
#иначе
#если (defined(linux) && defined(SYS_prctl))
	если (syscall(SYS_prctl, PR_GET_DUMPABLE, 0, 0, 0, 0) == 0)
		вернуть NULL;
#endif
#endif

#если defined(HAVE_SECURE_GETENV)
	вернуть secure_getenv(arg);
#elif defined(HAVE___SECURE_GETENV)
	вернуть __secure_getenv(arg);
#иначе
	вернуть getenv(arg);
#endif
}
