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
#ଯଦି !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__)
#define _XOPEN_SOURCE 600
#define _DARWIN_C_SOURCE
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#endif

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ଯଦି HAVE_UNISTD_H
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#endif
#ifdef HAVE_SYS_PRCTL_H
#ଅନ୍ତର୍ଭାଵ <sys/prctl.h>
#ଅନ୍ୟଥା
#define PR_GET_DUMPABLE 3
#endif
#ଯଦି (!defined(HAVE_PRCTL) && defined(linux))
#ଅନ୍ତର୍ଭାଵ <sys/syscall.h>
#endif

#ଅନ୍ତର୍ଭାଵ "ext2fs.h"

ଅକ୍ଷର *ext2fs_safe_getenv(ଅଚଳ ଅକ୍ଷର *arg)
{
	ଯଦି ((getuid() != geteuid()) || (getgid() != getegid()))
		ଫେରନ୍ତୁ ଖାଲି;
#ifdef HAVE_PRCTL
	ଯଦି (prctl(PR_GET_DUMPABLE, 0, 0, 0, 0) == 0)
		ଫେରନ୍ତୁ ଖାଲି;
#ଅନ୍ୟଥା
#ଯଦି (defined(linux) && defined(SYS_prctl))
	ଯଦି (syscall(SYS_prctl, PR_GET_DUMPABLE, 0, 0, 0, 0) == 0)
		ଫେରନ୍ତୁ ଖାଲି;
#endif
#endif

#ଯଦି defined(HAVE_SECURE_GETENV)
	ଫେରନ୍ତୁ secure_getenv(arg);
#ନଚେତଯଦି defined(HAVE___SECURE_GETENV)
	ଫେରନ୍ତୁ __secure_getenv(arg);
#ଅନ୍ୟଥା
	ଫେରନ୍ତୁ getenv(arg);
#endif
}
