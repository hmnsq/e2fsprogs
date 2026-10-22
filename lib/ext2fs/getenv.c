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
#གལ་ཏེ !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__)
#define _XOPEN_SOURCE 600
#define _DARWIN_C_SOURCE
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#endif

#include "config.h"
#include <stdlib.h>
#གལ་ཏེ HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_SYS_PRCTL_H
#include <sys/prctl.h>
#དེ་མིན
#define PR_GET_DUMPABLE 3
#endif
#གལ་ཏེ (!defined(HAVE_PRCTL) && defined(linux))
#include <sys/syscall.h>
#endif

#include "ext2fs.h"

var_na *ext2fs_safe_getenv(const var_na *arg)
{
	གལ་ཏེ ((getuid() != geteuid()) || (getgid() != getegid()))
		ཕྱིར NULL;
#ifdef HAVE_PRCTL
	གལ་ཏེ (prctl(PR_GET_DUMPABLE, 0, 0, 0, 0) == 0)
		ཕྱིར NULL;
#དེ་མིན
#གལ་ཏེ (defined(linux) && defined(SYS_prctl))
	གལ་ཏེ (syscall(SYS_prctl, PR_GET_DUMPABLE, 0, 0, 0, 0) == 0)
		ཕྱིར NULL;
#endif
#endif

#གལ་ཏེ defined(HAVE_SECURE_GETENV)
	ཕྱིར secure_getenv(arg);
#elif defined(HAVE___SECURE_GETENV)
	ཕྱིར __secure_getenv(arg);
#དེ་མིན
	ཕྱིར getenv(arg);
#endif
}
