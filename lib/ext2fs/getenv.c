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
#إذا !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__)
#وصف_ڪريو XOPEN_ذريعو 600
#وصف_ڪريو DARWIN_C_ذريعو
#جيڪڏهن_وصف_نه_ٿيل GNU_ذريعو
#وصف_ڪريو GNU_ذريعو
#جيڪڏهن_پورو
#جيڪڏهن_پورو

#ضمن "config.h"
#ضمن <stdlib.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#جيڪڏهن_پورو
#جيڪڏهن_وصف_ٿيل HAVE_SYS_PRCTL_H
#ضمن <sys/prctl.h>
#وإلا
#وصف_ڪريو PR_GET_DUMPABLE 3
#جيڪڏهن_پورو
#إذا (!defined(HAVE_PRCTL) && defined(linux))
#ضمن <sys/syscall.h>
#جيڪڏهن_پورو

#ضمن "ext2fs.h"

حرف *ext2fs_محفوظ_getenv(ثابت حرف *arg)
{
	إذا ((getuid() != geteuid()) || (getgid() != getegid()))
		أرجع عدم;
#جيڪڏهن_وصف_ٿيل HAVE_PRCTL
	إذا (prctl(PR_GET_DUMPABLE, 0, 0, 0, 0) == 0)
		أرجع عدم;
#وإلا
#إذا (defined(linux) && defined(SYS_prctl))
	إذا (syscall(SYS_prctl, PR_GET_DUMPABLE, 0, 0, 0, 0) == 0)
		أرجع عدم;
#جيڪڏهن_پورو
#جيڪڏهن_پورو

#إذا defined(HAVE_SECURE_GETENV)
	أرجع secure_getenv(arg);
#وإلاإذا defined(HAVE___SECURE_GETENV)
	أرجع __secure_getenv(arg);
#وإلا
	أرجع getenv(arg);
#جيڪڏهن_پورو
}
