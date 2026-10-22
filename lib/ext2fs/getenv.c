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
#எனில் !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__)
#define _XOPEN_SOURCE 600
#define _DARWIN_C_SOURCE
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#endif

#அந்தர்பாவ "config.h"
#அந்தர்பாவ <stdlib.h>
#எனில் HAVE_UNISTD_H
#அந்தர்பாவ <unistd.h>
#endif
#ifdef HAVE_SYS_PRCTL_H
#அந்தர்பாவ <sys/prctl.h>
#இல்லையெனில்
#define PR_GET_DUMPABLE 3
#endif
#எனில் (!defined(HAVE_PRCTL) && defined(linux))
#அந்தர்பாவ <sys/syscall.h>
#endif

#அந்தர்பாவ "ext2fs.h"

எழுத்து *ext2fs_safe_getenv(மாறிலி எழுத்து *arg)
{
	எனில் ((getuid() != geteuid()) || (getgid() != getegid()))
		திரும்பு வெற்று;
#ifdef HAVE_PRCTL
	எனில் (prctl(PR_GET_DUMPABLE, 0, 0, 0, 0) == 0)
		திரும்பு வெற்று;
#இல்லையெனில்
#எனில் (defined(linux) && defined(SYS_prctl))
	எனில் (syscall(SYS_prctl, PR_GET_DUMPABLE, 0, 0, 0, 0) == 0)
		திரும்பு வெற்று;
#endif
#endif

#எனில் defined(HAVE_SECURE_GETENV)
	திரும்பு secure_getenv(arg);
#அல்லதெனில் defined(HAVE___SECURE_GETENV)
	திரும்பு __secure_getenv(arg);
#இல்லையெனில்
	திரும்பு getenv(arg);
#endif
}
