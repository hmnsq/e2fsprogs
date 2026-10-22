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
#എങ്കിൽ !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__)
#define _XOPEN_SOURCE 600
#define _DARWIN_C_SOURCE
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#endif

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ <stdlib.h>
#എങ്കിൽ HAVE_UNISTD_H
#അന്തര്ഭാവ <unistd.h>
#endif
#ifdef HAVE_SYS_PRCTL_H
#അന്തര്ഭാവ <sys/prctl.h>
#അല്ലെങ്കിൽ
#define PR_GET_DUMPABLE 3
#endif
#എങ്കിൽ (!defined(HAVE_PRCTL) && defined(linux))
#അന്തര്ഭാവ <sys/syscall.h>
#endif

#അന്തര്ഭാവ "ext2fs.h"

അക്ഷരം *ext2fs_safe_getenv(സ്ഥിരം അക്ഷരം *arg)
{
	എങ്കിൽ ((getuid() != geteuid()) || (getgid() != getegid()))
		തിരികെ ശൂന്യ;
#ifdef HAVE_PRCTL
	എങ്കിൽ (prctl(PR_GET_DUMPABLE, 0, 0, 0, 0) == 0)
		തിരികെ ശൂന്യ;
#അല്ലെങ്കിൽ
#എങ്കിൽ (defined(linux) && defined(SYS_prctl))
	എങ്കിൽ (syscall(SYS_prctl, PR_GET_DUMPABLE, 0, 0, 0, 0) == 0)
		തിരികെ ശൂന്യ;
#endif
#endif

#എങ്കിൽ defined(HAVE_SECURE_GETENV)
	തിരികെ secure_getenv(arg);
#അല്ലെങ്കിലെങ്കിൽ defined(HAVE___SECURE_GETENV)
	തിരികെ __secure_getenv(arg);
#അല്ലെങ്കിൽ
	തിരികെ getenv(arg);
#endif
}
