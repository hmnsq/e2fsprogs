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
#အကယ်၍ !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__)
#define _XOPEN_SOURCE 600
#define _DARWIN_C_SOURCE
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#endif

#include "config.h"
#include <stdlib.h>
#အကယ်၍ HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_SYS_PRCTL_H
#include <sys/prctl.h>
#မဟုတ်ပါက
#define PR_GET_DUMPABLE 3
#endif
#အကယ်၍ (!defined(HAVE_PRCTL) && defined(linux))
#include <sys/syscall.h>
#endif

#include "ext2fs.h"

var_na *ext2fs_safe_getenv(const var_na *arg)
{
	အကယ်၍ ((getuid() != geteuid()) || (getgid() != getegid()))
		ပြန်ပေး NULL;
#ifdef HAVE_PRCTL
	အကယ်၍ (prctl(PR_GET_DUMPABLE, 0, 0, 0, 0) == 0)
		ပြန်ပေး NULL;
#မဟုတ်ပါက
#အကယ်၍ (defined(linux) && defined(SYS_prctl))
	အကယ်၍ (syscall(SYS_prctl, PR_GET_DUMPABLE, 0, 0, 0, 0) == 0)
		ပြန်ပေး NULL;
#endif
#endif

#အကယ်၍ defined(HAVE_SECURE_GETENV)
	ပြန်ပေး secure_getenv(arg);
#elif defined(HAVE___SECURE_GETENV)
	ပြန်ပေး __secure_getenv(arg);
#မဟုတ်ပါက
	ပြန်ပေး getenv(arg);
#endif
}
