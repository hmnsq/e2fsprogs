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
#यदि !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__)
#परिभाषय _XOPEN_SOURCE 600
#परिभाषय _DARWIN_C_SOURCE
#यदि_अपरिभाषितम् _GNU_SOURCE
#परिभाषय _GNU_SOURCE
#यदि_अन्तः
#यदि_अन्तः

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdlib.h>
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#यदि_अन्तः
#यदि_परिभाषितम् HAVE_SYS_PRCTL_H
#अन्तर्भाव <sys/prctl.h>
#अन्यथा
#परिभाषय PR_GET_DUMPABLE 3
#यदि_अन्तः
#यदि (!defined(HAVE_PRCTL) && defined(linux))
#अन्तर्भाव <sys/syscall.h>
#यदि_अन्तः

#अन्तर्भाव "ext2fs.h"

वर्ण *ext2fs_safe_getenv(ध्रुव वर्ण *arg)
{
	यदि ((getuid() != geteuid()) || (getgid() != getegid()))
		प्रत्यागम रिक्त;
#यदि_परिभाषितम् HAVE_PRCTL
	यदि (prctl(PR_GET_DUMPABLE, 0, 0, 0, 0) == 0)
		प्रत्यागम रिक्त;
#अन्यथा
#यदि (defined(linux) && defined(SYS_prctl))
	यदि (syscall(SYS_prctl, PR_GET_DUMPABLE, 0, 0, 0, 0) == 0)
		प्रत्यागम रिक्त;
#यदि_अन्तः
#यदि_अन्तः

#यदि defined(HAVE_SECURE_GETENV)
	प्रत्यागम secure_getenv(arg);
#नोचेद्यदि defined(HAVE___SECURE_GETENV)
	प्रत्यागम __secure_getenv(arg);
#अन्यथा
	प्रत्यागम getenv(arg);
#यदि_अन्तः
}
