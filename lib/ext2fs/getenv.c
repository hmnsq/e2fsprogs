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
#اگر !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__)
#تعریف_کرو XOPEN_ماخذ 600
#تعریف_کرو DARWIN_C_ماخذ
#اگر_غیر_تعریف_شدہ GNU_ماخذ
#تعریف_کرو GNU_ماخذ
#اگر_ختم
#اگر_ختم

#شامل "config.h"
#شامل <stdlib.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#اگر_ختم
#اگر_تعریف_شدہ HAVE_SYS_PRCTL_H
#شامل <sys/prctl.h>
#ورنہ
#تعریف_کرو PR_GET_DUMPABLE 3
#اگر_ختم
#اگر (!defined(HAVE_PRCTL) && defined(linux))
#شامل <sys/syscall.h>
#اگر_ختم

#شامل "ext2fs.h"

حرف *ext2fs_محفوظ_getenv(ثابت حرف *arg)
{
	اگر ((getuid() != geteuid()) || (getgid() != getegid()))
		واپس تہی;
#اگر_تعریف_شدہ HAVE_PRCTL
	اگر (prctl(PR_GET_DUMPABLE, 0, 0, 0, 0) == 0)
		واپس تہی;
#ورنہ
#اگر (defined(linux) && defined(SYS_prctl))
	اگر (syscall(SYS_prctl, PR_GET_DUMPABLE, 0, 0, 0, 0) == 0)
		واپس تہی;
#اگر_ختم
#اگر_ختم

#اگر defined(HAVE_SECURE_GETENV)
	واپس secure_getenv(arg);
#وگرنہ_اگر defined(HAVE___SECURE_GETENV)
	واپس __secure_getenv(arg);
#ورنہ
	واپس getenv(arg);
#اگر_ختم
}
