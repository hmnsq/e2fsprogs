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
#ᨀᨘ !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__)
#define _XOPEN_SOURCE 600
#define _DARWIN_C_SOURCE
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#endif

#include "config.h"
#include <stdlib.h>
#ᨀᨘ HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_SYS_PRCTL_H
#include <sys/prctl.h>
#ᨀᨘᨁ
#define PR_GET_DUMPABLE 3
#endif
#ᨀᨘ (!defined(HAVE_PRCTL) && defined(linux))
#include <sys/syscall.h>
#endif

#include "ext2fs.h"

var_na *ext2fs_safe_getenv(const var_na *arg)
{
	ᨀᨘ ((getuid() != geteuid()) || (getgid() != getegid()))
		ᨀᨘᨒ NULL;
#ifdef HAVE_PRCTL
	ᨀᨘ (prctl(PR_GET_DUMPABLE, 0, 0, 0, 0) == 0)
		ᨀᨘᨒ NULL;
#ᨀᨘᨁ
#ᨀᨘ (defined(linux) && defined(SYS_prctl))
	ᨀᨘ (syscall(SYS_prctl, PR_GET_DUMPABLE, 0, 0, 0, 0) == 0)
		ᨀᨘᨒ NULL;
#endif
#endif

#ᨀᨘ defined(HAVE_SECURE_GETENV)
	ᨀᨘᨒ secure_getenv(arg);
#elif defined(HAVE___SECURE_GETENV)
	ᨀᨘᨒ __secure_getenv(arg);
#ᨀᨘᨁ
	ᨀᨘᨒ getenv(arg);
#endif
}
