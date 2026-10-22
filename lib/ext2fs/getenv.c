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
#ಆದರೆ !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__)
#ವ್ಯಾಖ್ಯಾನಿಸು XOPEN_ಮೂಲ 600
#ವ್ಯಾಖ್ಯಾನಿಸು DARWIN_C_ಮೂಲ
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ GNU_ಮೂಲ
#ವ್ಯಾಖ್ಯಾನಿಸು GNU_ಮೂಲ
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಆದರೆ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_SYS_PRCTL_H
#ಅನ್ತರ್ಭಾವ <sys/prctl.h>
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು PR_GET_DUMPABLE 3
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ (!defined(HAVE_PRCTL) && defined(linux))
#ಅನ್ತರ್ಭಾವ <sys/syscall.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "ext2fs.h"

ಅಕ್ಷರ *ext2fs_ಸುರಕ್ಷಿತ_getenv(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *arg)
{
	ಆದರೆ ((getuid() != geteuid()) || (getgid() != getegid()))
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_PRCTL
	ಆದರೆ (prctl(PR_GET_DUMPABLE, 0, 0, 0, 0) == 0)
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;
#ಇಲ್ಲದಿದ್ದರೆ
#ಆದರೆ (defined(linux) && defined(SYS_prctl))
	ಆದರೆ (syscall(SYS_prctl, PR_GET_DUMPABLE, 0, 0, 0, 0) == 0)
		ಹಿಂತಿರುಗಿ ಖಾಲಿ;
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ

#ಆದರೆ defined(HAVE_SECURE_GETENV)
	ಹಿಂತಿರುಗಿ secure_getenv(arg);
#ಅಥವಾಆದರೆ defined(HAVE___SECURE_GETENV)
	ಹಿಂತಿರುಗಿ __secure_getenv(arg);
#ಇಲ್ಲದಿದ್ದರೆ
	ಹಿಂತಿರುಗಿ getenv(arg);
#ಆದರೆ_ಅಂತ್ಯ
}
