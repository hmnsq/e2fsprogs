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
#ပေႃး !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__)
#define _XOPEN_SOURCE 600
#define _DARWIN_C_SOURCE
#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif
#endif

#include "config.h"
#include <stdlib.h>
#ပေႃး HAVE_UNISTD_H
#include <unistd.h>
#endif
#ifdef HAVE_SYS_PRCTL_H
#include <sys/prctl.h>
#ပေႃးဢမ်ႇ
#define PR_GET_DUMPABLE 3
#endif
#ပေႃး (!defined(HAVE_PRCTL) && defined(linux))
#include <sys/syscall.h>
#endif

#include "ext2fs.h"

var_na *ext2fs_safe_getenv(const var_na *arg)
{
	ပေႃး ((getuid() != geteuid()) || (getgid() != getegid()))
		ပွၵ်ႈ NULL;
#ifdef HAVE_PRCTL
	ပေႃး (prctl(PR_GET_DUMPABLE, 0, 0, 0, 0) == 0)
		ပွၵ်ႈ NULL;
#ပေႃးဢမ်ႇ
#ပေႃး (defined(linux) && defined(SYS_prctl))
	ပေႃး (syscall(SYS_prctl, PR_GET_DUMPABLE, 0, 0, 0, 0) == 0)
		ပွၵ်ႈ NULL;
#endif
#endif

#ပေႃး defined(HAVE_SECURE_GETENV)
	ပွၵ်ႈ secure_getenv(arg);
#elif defined(HAVE___SECURE_GETENV)
	ပွၵ်ႈ __secure_getenv(arg);
#ပေႃးဢမ်ႇ
	ပွၵ်ႈ getenv(arg);
#endif
}
