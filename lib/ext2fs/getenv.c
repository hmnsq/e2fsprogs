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
#ਜੇ !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ XOPEN_ਸਰੋਤ 600
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ DARWIN_C_ਸਰੋਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ GNU_ਸਰੋਤ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GNU_ਸਰੋਤ
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਜੇ HAVE_UNISTD_H
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਜੇ_ਅੰਤ
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_SYS_PRCTL_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/prctl.h>
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ PR_GET_DUMPABLE 3
#ਜੇ_ਅੰਤ
#ਜੇ (!defined(HAVE_PRCTL) && defined(linux))
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/syscall.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fs.h"

ਅੱਖਰ *ext2fs_ਸੁਰੱਖਿਅਤ_getenv(ਅਚਲ ਅੱਖਰ *arg)
{
	ਜੇ ((getuid() != geteuid()) || (getgid() != getegid()))
		ਵਾਪਸ ਨਿਰਾ;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_PRCTL
	ਜੇ (prctl(PR_GET_DUMPABLE, 0, 0, 0, 0) == 0)
		ਵਾਪਸ ਨਿਰਾ;
#ਨਹੀਂਤਾਂ
#ਜੇ (defined(linux) && defined(SYS_prctl))
	ਜੇ (syscall(SYS_prctl, PR_GET_DUMPABLE, 0, 0, 0, 0) == 0)
		ਵਾਪਸ ਨਿਰਾ;
#ਜੇ_ਅੰਤ
#ਜੇ_ਅੰਤ

#ਜੇ defined(HAVE_SECURE_GETENV)
	ਵਾਪਸ secure_getenv(arg);
#ਜਾਂਜੇ defined(HAVE___SECURE_GETENV)
	ਵਾਪਸ __secure_getenv(arg);
#ਨਹੀਂਤਾਂ
	ਵਾਪਸ getenv(arg);
#ਜੇ_ਅੰਤ
}
