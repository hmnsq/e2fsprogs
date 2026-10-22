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
#ఒకవేళ !defined(__FreeBSD__) && !defined(__NetBSD__) && !defined(__OpenBSD__)
#నిర్వచించు XOPEN_ఆధారం 600
#నిర్వచించు DARWIN_C_ఆధారం
#నిర్వచితం_కాకపోతే GNU_ఆధారం
#నిర్వచించు GNU_ఆధారం
#అయితే_ముగింపు
#అయితే_ముగింపు

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdlib.h>
#ఒకవేళ HAVE_UNISTD_H
#అన్తర్భావ <unistd.h>
#అయితే_ముగింపు
#నిర్వచితమైతే HAVE_SYS_PRCTL_H
#అన్తర్భావ <sys/prctl.h>
#లేకపోతే
#నిర్వచించు PR_GET_DUMPABLE 3
#అయితే_ముగింపు
#ఒకవేళ (!defined(HAVE_PRCTL) && defined(linux))
#అన్తర్భావ <sys/syscall.h>
#అయితే_ముగింపు

#అన్తర్భావ "ext2fs.h"

అక్షరం *ext2fs_సురక్షిత_getenv(స్థిరరాశి అక్షరం *arg)
{
	ఒకవేళ ((getuid() != geteuid()) || (getgid() != getegid()))
		తిరిగి ఖాళీ;
#నిర్వచితమైతే HAVE_PRCTL
	ఒకవేళ (prctl(PR_GET_DUMPABLE, 0, 0, 0, 0) == 0)
		తిరిగి ఖాళీ;
#లేకపోతే
#ఒకవేళ (defined(linux) && defined(SYS_prctl))
	ఒకవేళ (syscall(SYS_prctl, PR_GET_DUMPABLE, 0, 0, 0, 0) == 0)
		తిరిగి ఖాళీ;
#అయితే_ముగింపు
#అయితే_ముగింపు

#ఒకవేళ defined(HAVE_SECURE_GETENV)
	తిరిగి secure_getenv(arg);
#లేదావేళ defined(HAVE___SECURE_GETENV)
	తిరిగి __secure_getenv(arg);
#లేకపోతే
	తిరిగి getenv(arg);
#అయితే_ముగింపు
}
