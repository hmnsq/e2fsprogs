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
#যদি !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__)
#define _XOPEN_SOURCE 600
#define _DARWIN_C_SOURCE
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#endif

#অন্তর্ভাব "config.h"
#অন্তর্ভাব <stdlib.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাব <unistd.h>
#endif
#ifdef HAVE_SYS_PRCTL_H
#অন্তর্ভাব <sys/prctl.h>
#অন্যথা
#define PR_GET_DUMPABLE 3
#endif
#যদি (!defined(HAVE_PRCTL) && defined(linux))
#অন্তর্ভাব <sys/syscall.h>
#endif

#অন্তর্ভাব "ext2fs.h"

অক্ষর *ext2fs_safe_getenv(ধ্রুবক অক্ষর *arg)
{
	যদি ((getuid() != geteuid()) || (getgid() != getegid()))
		ফেরত ফাঁকা;
#ifdef HAVE_PRCTL
	যদি (prctl(PR_GET_DUMPABLE, 0, 0, 0, 0) == 0)
		ফেরত ফাঁকা;
#অন্যথা
#যদি (defined(linux) && defined(SYS_prctl))
	যদি (syscall(SYS_prctl, PR_GET_DUMPABLE, 0, 0, 0, 0) == 0)
		ফেরত ফাঁকা;
#endif
#endif

#যদি defined(HAVE_SECURE_GETENV)
	ফেরত secure_getenv(arg);
#নতুবা_যদি defined(HAVE___SECURE_GETENV)
	ফেরত __secure_getenv(arg);
#অন্যথা
	ফেরত getenv(arg);
#endif
}
