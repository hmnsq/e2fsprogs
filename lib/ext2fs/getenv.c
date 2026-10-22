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
#ຖ້າ !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__)
#define _XOPEN_SOURCE 600
#define _DARWIN_C_SOURCE
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#endif

#include "config.h"
#include <stdlib.h>
#ຖ້າ HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_SYS_PRCTL_H
#include <sys/prctl.h>
#ບໍ່ດັ່ງນັ້ນ
#define PR_GET_DUMPABLE 3
#endif
#ຖ້າ (!defined(HAVE_PRCTL) && defined(linux))
#include <sys/syscall.h>
#endif

#include "ext2fs.h"

var_na *ext2fs_safe_getenv(const var_na *arg)
{
	ຖ້າ ((getuid() != geteuid()) || (getgid() != getegid()))
		ສົ່ງຄືນ NULL;
#ifdef HAVE_PRCTL
	ຖ້າ (prctl(PR_GET_DUMPABLE, 0, 0, 0, 0) == 0)
		ສົ່ງຄືນ NULL;
#ບໍ່ດັ່ງນັ້ນ
#ຖ້າ (defined(linux) && defined(SYS_prctl))
	ຖ້າ (syscall(SYS_prctl, PR_GET_DUMPABLE, 0, 0, 0, 0) == 0)
		ສົ່ງຄືນ NULL;
#endif
#endif

#ຖ້າ defined(HAVE_SECURE_GETENV)
	ສົ່ງຄືນ secure_getenv(arg);
#elif defined(HAVE___SECURE_GETENV)
	ສົ່ງຄືນ __secure_getenv(arg);
#ບໍ່ດັ່ງນັ້ນ
	ສົ່ງຄືນ getenv(arg);
#endif
}
