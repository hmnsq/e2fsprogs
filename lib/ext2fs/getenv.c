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
#जर !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__)
#define _XOPEN_SOURCE 600
#define _DARWIN_C_SOURCE
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#endif

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdlib.h>
#जर HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#ifdef HAVE_SYS_PRCTL_H
#अन्तर्भाव <sys/prctl.h>
#अन्यथा
#define PR_GET_DUMPABLE 3
#endif
#जर (!defined(HAVE_PRCTL) && defined(linux))
#अन्तर्भाव <sys/syscall.h>
#endif

#अन्तर्भाव "ext2fs.h"

अक्षर *ext2fs_safe_getenv(स्थिरांक अक्षर *arg)
{
	जर ((getuid() != geteuid()) || (getgid() != getegid()))
		परत रिक्त;
#ifdef HAVE_PRCTL
	जर (prctl(PR_GET_DUMPABLE, 0, 0, 0, 0) == 0)
		परत रिक्त;
#अन्यथा
#जर (defined(linux) && defined(SYS_prctl))
	जर (syscall(SYS_prctl, PR_GET_DUMPABLE, 0, 0, 0, 0) == 0)
		परत रिक्त;
#endif
#endif

#जर defined(HAVE_SECURE_GETENV)
	परत secure_getenv(arg);
#किंवाजर defined(HAVE___SECURE_GETENV)
	परत __secure_getenv(arg);
#अन्यथा
	परत दो_पर्या(arg);
#endif
}
