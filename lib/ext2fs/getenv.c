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
#जइ !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__)
#define _XOPEN_SOURCE 600
#define _DARWIN_C_SOURCE
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#endif

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdlib.h>
#जइ HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#ifdef HAVE_SYS_PRCTL_H
#अन्तर्भाव <sys/prctl.h>
#अण्णहा
#define PR_GET_DUMPABLE 3
#endif
#जइ (!defined(HAVE_PRCTL) && defined(linux))
#अन्तर्भाव <sys/syscall.h>
#endif

#अन्तर्भाव "ext2fs.h"

वर्ण *ext2fs_safe_getenv(ध्रुव वर्ण *arg)
{
	जइ ((getuid() != geteuid()) || (getgid() != getegid()))
		प्रत्यागम रिक्त;
#ifdef HAVE_PRCTL
	जइ (prctl(PR_GET_DUMPABLE, 0, 0, 0, 0) == 0)
		प्रत्यागम रिक्त;
#अण्णहा
#जइ (defined(linux) && defined(SYS_prctl))
	जइ (syscall(SYS_prctl, PR_GET_DUMPABLE, 0, 0, 0, 0) == 0)
		प्रत्यागम रिक्त;
#endif
#endif

#जइ defined(HAVE_SECURE_GETENV)
	प्रत्यागम secure_getenv(arg);
#नोचेद्यदि defined(HAVE___SECURE_GETENV)
	प्रत्यागम __secure_getenv(arg);
#अण्णहा
	प्रत्यागम getenv(arg);
#endif
}
