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
#ᰀᰧ !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__)
#define _XOPEN_SOURCE 600
#define _DARWIN_C_SOURCE
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#endif

#include "config.h"
#include <stdlib.h>
#ᰀᰧ HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_SYS_PRCTL_H
#include <sys/prctl.h>
#ᰀᰧᰀ
#define PR_GET_DUMPABLE 3
#endif
#ᰀᰧ (!defined(HAVE_PRCTL) && defined(linux))
#include <sys/syscall.h>
#endif

#include "ext2fs.h"

var_na *ext2fs_safe_getenv(const var_na *arg)
{
	ᰀᰧ ((getuid() != geteuid()) || (getgid() != getegid()))
		ᰀᰧᰌ NULL;
#ifdef HAVE_PRCTL
	ᰀᰧ (prctl(PR_GET_DUMPABLE, 0, 0, 0, 0) == 0)
		ᰀᰧᰌ NULL;
#ᰀᰧᰀ
#ᰀᰧ (defined(linux) && defined(SYS_prctl))
	ᰀᰧ (syscall(SYS_prctl, PR_GET_DUMPABLE, 0, 0, 0, 0) == 0)
		ᰀᰧᰌ NULL;
#endif
#endif

#ᰀᰧ defined(HAVE_SECURE_GETENV)
	ᰀᰧᰌ secure_getenv(arg);
#elif defined(HAVE___SECURE_GETENV)
	ᰀᰧᰌ __secure_getenv(arg);
#ᰀᰧᰀ
	ᰀᰧᰌ getenv(arg);
#endif
}
