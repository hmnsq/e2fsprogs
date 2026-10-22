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
#જો !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__)
#define _XOPEN_SOURCE 600
#define _DARWIN_C_SOURCE
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#endif

#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ <stdlib.h>
#જો HAVE_UNISTD_H
#અન્તર્ભાવ <unistd.h>
#endif
#ifdef HAVE_SYS_PRCTL_H
#અન્તર્ભાવ <sys/prctl.h>
#નહીંતર
#define PR_GET_DUMPABLE 3
#endif
#જો (!defined(HAVE_PRCTL) && defined(linux))
#અન્તર્ભાવ <sys/syscall.h>
#endif

#અન્તર્ભાવ "ext2fs.h"

અક્ષર *ext2fs_safe_getenv(અચલ અક્ષર *arg)
{
	જો ((getuid() != geteuid()) || (getgid() != getegid()))
		પાછા ખાલી;
#ifdef HAVE_PRCTL
	જો (prctl(PR_GET_DUMPABLE, 0, 0, 0, 0) == 0)
		પાછા ખાલી;
#નહીંતર
#જો (defined(linux) && defined(SYS_prctl))
	જો (syscall(SYS_prctl, PR_GET_DUMPABLE, 0, 0, 0, 0) == 0)
		પાછા ખાલી;
#endif
#endif

#જો defined(HAVE_SECURE_GETENV)
	પાછા secure_getenv(arg);
#અથવાજો defined(HAVE___SECURE_GETENV)
	પાછા __secure_getenv(arg);
#નહીંતર
	પાછા getenv(arg);
#endif
}
