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
#अगर !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__)
#घोषणा _XOPEN_SOURCE 600
#घोषणा _DARWIN_C_SOURCE
#अगर_अघोषित _GNU_SOURCE
#घोषणा _GNU_SOURCE
#पूर्ण_अगर
#पूर्ण_अगर

#समावेश "config.h"
#समावेश <stdlib.h>
#अगर HAVE_UNISTD_H
#समावेश <unistd.h>
#पूर्ण_अगर
#अगर_घोषित HAVE_SYS_PRCTL_H
#समावेश <sys/prctl.h>
#अन्यथा
#घोषणा PR_GET_DUMPABLE 3
#पूर्ण_अगर
#अगर (!defined(HAVE_PRCTL) && defined(linux))
#समावेश <sys/syscall.h>
#पूर्ण_अगर

#समावेश "ext2fs.h"

अक्षर *ext2fs_safe_getenv(स्थिर अक्षर *arg)
{
	अगर ((getuid() != geteuid()) || (getgid() != getegid()))
		वापस शून्य;
#अगर_घोषित HAVE_PRCTL
	अगर (prctl(PR_GET_DUMPABLE, 0, 0, 0, 0) == 0)
		वापस शून्य;
#अन्यथा
#अगर (defined(linux) && defined(SYS_prctl))
	अगर (syscall(SYS_prctl, PR_GET_DUMPABLE, 0, 0, 0, 0) == 0)
		वापस शून्य;
#पूर्ण_अगर
#पूर्ण_अगर

#अगर defined(HAVE_SECURE_GETENV)
	वापस secure_getenv(arg);
#या_अगर defined(HAVE___SECURE_GETENV)
	वापस __secure_getenv(arg);
#अन्यथा
	वापस दो_पर्या(arg);
#पूर्ण_अगर
}
