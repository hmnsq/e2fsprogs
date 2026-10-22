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
#सचे !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__)
#define _XOPEN_SOURCE 600
#define _DARWIN_C_SOURCE
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#endif

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdlib.h>
#सचे HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#ifdef HAVE_SYS_PRCTL_H
#अन्तर्भाव <sys/prctl.h>
#अञ्ञथा
#define PR_GET_DUMPABLE 3
#endif
#सचे (!defined(HAVE_PRCTL) && defined(linux))
#अन्तर्भाव <sys/syscall.h>
#endif

#अन्तर्भाव "ext2fs.h"

वर्ण *ext2fs_safe_getenv(ध्रुव वर्ण *arg)
{
	सचे ((getuid() != geteuid()) || (getgid() != getegid()))
		पटिनिवत्त रिक्त;
#ifdef HAVE_PRCTL
	सचे (prctl(PR_GET_DUMPABLE, 0, 0, 0, 0) == 0)
		पटिनिवत्त रिक्त;
#अञ्ञथा
#सचे (defined(linux) && defined(SYS_prctl))
	सचे (syscall(SYS_prctl, PR_GET_DUMPABLE, 0, 0, 0, 0) == 0)
		पटिनिवत्त रिक्त;
#endif
#endif

#सचे defined(HAVE_SECURE_GETENV)
	पटिनिवत्त secure_getenv(arg);
#नोचेद्यदि defined(HAVE___SECURE_GETENV)
	पटिनिवत्त __secure_getenv(arg);
#अञ्ञथा
	पटिनिवत्त getenv(arg);
#endif
}
