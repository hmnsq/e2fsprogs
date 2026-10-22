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

#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ <stdlib.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাৱ <unistd.h>
#endif
#ifdef HAVE_SYS_PRCTL_H
#অন্তর্ভাৱ <sys/prctl.h>
#অন্যথা
#define PR_GET_DUMPABLE 3
#endif
#যদি (!defined(HAVE_PRCTL) && defined(linux))
#অন্তর্ভাৱ <sys/syscall.h>
#endif

#অন্তর্ভাৱ "ext2fs.h"

ৱর্ণ *ext2fs_safe_getenv(ধ্রুৱ ৱর্ণ *arg)
{
	যদি ((getuid() != geteuid()) || (getgid() != getegid()))
		ঘূৰাই রিক্ত;
#ifdef HAVE_PRCTL
	যদি (prctl(PR_GET_DUMPABLE, 0, 0, 0, 0) == 0)
		ঘূৰাই রিক্ত;
#অন্যথা
#যদি (defined(linux) && defined(SYS_prctl))
	যদি (syscall(SYS_prctl, PR_GET_DUMPABLE, 0, 0, 0, 0) == 0)
		ঘূৰাই রিক্ত;
#endif
#endif

#যদি defined(HAVE_SECURE_GETENV)
	ঘূৰাই secure_getenv(arg);
#নোচেদ্যদি defined(HAVE___SECURE_GETENV)
	ঘূৰাই __secure_getenv(arg);
#অন্যথা
	ঘূৰাই getenv(arg);
#endif
}
