/*
 * version.c --- Return the version of the ext2 library
 *
 * Copyright (C) 1997 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#అన్తర్భావ "config.h"
#ఒకవేళ HAVE_UNISTD_H
#అన్తర్భావ <unistd.h>
#అయితే_ముగింపు
#అన్తర్భావ <string.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <ctype.h>

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fs.h"

#అన్తర్భావ "../../version.h"

స్థిర స్థిరరాశి అక్షరం *lib_సంస్కరణ = E2_FSPROGS_సంస్కరణ;
స్థిర స్థిరరాశి అక్షరం *lib_date = E2FSPROGS_DATE;

పూర్ణాంకం ext2fs_parse_సంస్కరణ_string(స్థిరరాశి అక్షరం *ver_string)
{
	స్థిరరాశి అక్షరం *cp;
	పూర్ణాంకం సంస్కరణ = 0, dot_లెక్క = 0;

	కొరకు (cp = ver_string; *cp; cp++) {
		ఒకవేళ (*cp == '.') {
			ఒకవేళ (dot_లెక్క++)
				ఆపు;
			లేకపోతే
				కొనసాగు;
		}
		ఒకవేళ (!అంకెనా(*cp))
			ఆపు;
		సంస్కరణ = (సంస్కరణ * 10) + (*cp - '0');
	}
	తిరిగి సంస్కరణ;
}


పూర్ణాంకం ext2fs_get_library_సంస్కరణ(స్థిరరాశి అక్షరం **ver_string,
			       స్థిరరాశి అక్షరం **date_string)
{
	ఒకవేళ (ver_string)
		*ver_string = lib_సంస్కరణ;
	ఒకవేళ (date_string)
		*date_string = lib_date;

	తిరిగి ext2fs_parse_సంస్కరణ_string(lib_సంస్కరణ);
}
