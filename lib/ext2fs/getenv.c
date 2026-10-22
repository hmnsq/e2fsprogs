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
#ᤀᤡ᤺ !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__)
#define _XOPEN_SOURCE 600
#define _DARWIN_C_SOURCE
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#endif

#include "config.h"
#include <stdlib.h>
#ᤀᤡ᤺ HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_SYS_PRCTL_H
#include <sys/prctl.h>
#ᤀᤡ᤺ᤆᤡ
#define PR_GET_DUMPABLE 3
#endif
#ᤀᤡ᤺ (!defined(HAVE_PRCTL) && defined(linux))
#include <sys/syscall.h>
#endif

#include "ext2fs.h"

var_na *ext2fs_safe_getenv(const var_na *arg)
{
	ᤀᤡ᤺ ((getuid() != geteuid()) || (getgid() != getegid()))
		ᤀᤡᤴ NULL;
#ifdef HAVE_PRCTL
	ᤀᤡ᤺ (prctl(PR_GET_DUMPABLE, 0, 0, 0, 0) == 0)
		ᤀᤡᤴ NULL;
#ᤀᤡ᤺ᤆᤡ
#ᤀᤡ᤺ (defined(linux) && defined(SYS_prctl))
	ᤀᤡ᤺ (syscall(SYS_prctl, PR_GET_DUMPABLE, 0, 0, 0, 0) == 0)
		ᤀᤡᤴ NULL;
#endif
#endif

#ᤀᤡ᤺ defined(HAVE_SECURE_GETENV)
	ᤀᤡᤴ secure_getenv(arg);
#elif defined(HAVE___SECURE_GETENV)
	ᤀᤡᤴ __secure_getenv(arg);
#ᤀᤡ᤺ᤆᤡ
	ᤀᤡᤴ getenv(arg);
#endif
}
