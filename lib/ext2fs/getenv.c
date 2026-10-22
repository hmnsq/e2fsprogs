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
#ಯದಿ !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__)
#define _XOPEN_SOURCE 600
#define _DARWIN_C_SOURCE
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#endif

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಯದಿ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#endif
#ifdef HAVE_SYS_PRCTL_H
#ಅನ್ತರ್ಭಾವ <sys/prctl.h>
#ಅನ್ಯಥಾ
#define PR_GET_DUMPABLE 3
#endif
#ಯದಿ (!defined(HAVE_PRCTL) && defined(linux))
#ಅನ್ತರ್ಭಾವ <sys/syscall.h>
#endif

#ಅನ್ತರ್ಭಾವ "ext2fs.h"

ವರ್ಣ *ext2fs_safe_getenv(ಧ್ರುವ ವರ್ಣ *arg)
{
	ಯದಿ ((getuid() != geteuid()) || (getgid() != getegid()))
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
#ifdef HAVE_PRCTL
	ಯದಿ (prctl(PR_GET_DUMPABLE, 0, 0, 0, 0) == 0)
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
#ಅನ್ಯಥಾ
#ಯದಿ (defined(linux) && defined(SYS_prctl))
	ಯದಿ (syscall(SYS_prctl, PR_GET_DUMPABLE, 0, 0, 0, 0) == 0)
		ಪ್ರತ್ಯಾಗಮ ರಿಕ್ತ;
#endif
#endif

#ಯದಿ defined(HAVE_SECURE_GETENV)
	ಪ್ರತ್ಯಾಗಮ secure_getenv(arg);
#ನೋಚೇದ್ಯದಿ defined(HAVE___SECURE_GETENV)
	ಪ್ರತ್ಯಾಗಮ __secure_getenv(arg);
#ಅನ್ಯಥಾ
	ಪ್ರತ್ಯಾಗಮ getenv(arg);
#endif
}
