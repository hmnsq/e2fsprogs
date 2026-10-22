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
#إذا !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__)
#define _XOPEN_SOURCE 600
#define _DARWIN_C_SOURCE
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#endif

#ضمن "config.h"
#ضمن <stdlib.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#endif
#ifdef HAVE_SYS_PRCTL_H
#ضمن <sys/prctl.h>
#وإلا
#define PR_GET_DUMPABLE 3
#endif
#إذا (!defined(HAVE_PRCTL) && defined(linux))
#ضمن <sys/syscall.h>
#endif

#ضمن "ext2fs.h"

حرف *ext2fs_safe_getenv(ثابت حرف *arg)
{
	إذا ((getuid() != geteuid()) || (getgid() != getegid()))
		أرجع عدم;
#ifdef HAVE_PRCTL
	إذا (prctl(PR_GET_DUMPABLE, 0, 0, 0, 0) == 0)
		أرجع عدم;
#وإلا
#إذا (defined(linux) && defined(SYS_prctl))
	إذا (syscall(SYS_prctl, PR_GET_DUMPABLE, 0, 0, 0, 0) == 0)
		أرجع عدم;
#endif
#endif

#إذا defined(HAVE_SECURE_GETENV)
	أرجع secure_getenv(arg);
#وإلاإذا defined(HAVE___SECURE_GETENV)
	أرجع __secure_getenv(arg);
#وإلا
	أرجع getenv(arg);
#endif
}
