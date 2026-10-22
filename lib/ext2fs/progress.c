/*
 * progress.c - Numeric progress meter
 *
 * Copyright (C) 1994, 1995, 1996, 1997, 1998, 1999, 2000, 2001, 2002,
 * 	2003, 2004, 2005 by Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Public
 * License.
 * %End-Header%
 */

#అన్తర్భావ "config.h"
#అన్తర్భావ "ext2fs.h"
#అన్తర్భావ "ext2fsP.h"

#అన్తర్భావ <time.h>

స్థిర అక్షరం spaces[80], backspaces[80];
స్థిర time_t చివరి_update;

నిర్మాణం ext2fs_పురోగతి_ops ext2fs_numeric_పురోగతి_ops = {
	.init		= ext2fs_numeric_పురోగతి_init,
	.update		= ext2fs_numeric_పురోగతి_update,
	.close		= ext2fs_numeric_పురోగతి_close,
};

శూన్య ext2fs_numeric_పురోగతి_init(ext2_filsys fs,
				  నిర్మాణం ext2fs_numeric_పురోగతి_struct * పురోగతి,
				  స్థిరరాశి అక్షరం *label, __u64 max)
{
	/*
	 * The PRINT_PROGRESS flag turns on or off ALL
	 * progress-related messages, whereas the SKIP_PROGRESS
	 * environment variable prints the start and end messages but
	 * not the numeric countdown in the middle.
	 */
	ఒకవేళ (!(fs->flags & EXT2_FLAG_PRINT_పురోగతి))
		తిరిగి;

	memset(spaces, ' ', పరిమాణం(spaces)-1);
	spaces[పరిమాణం(spaces)-1] = 0;
	memset(backspaces, '\b', పరిమాణం(backspaces)-1);
	backspaces[పరిమాణం(backspaces)-1] = 0;

	memset(పురోగతి, 0, పరిమాణం(*పురోగతి));
	ఒకవేళ (ext2fs_సురక్షిత_getenv("E2FSPROGS_SKIP_PROGRESS"))
		పురోగతి->skip_పురోగతి++;


	/*
	 * Figure out how many digits we need (always at least one)
	 */
	పురోగతి->max = max;
	పురోగతి->log_max = ext2fs_log10_u64(max) + 1;

	ఒకవేళ (label) {
		fputs(label, stdout);
		fflush(stdout);
	}
	చివరి_update = 0;
}

శూన్య ext2fs_numeric_పురోగతి_update(ext2_filsys fs,
				    నిర్మాణం ext2fs_numeric_పురోగతి_struct * పురోగతి,
				    __u64 val)
{
	time_t now;

	ఒకవేళ (!(fs->flags & EXT2_FLAG_PRINT_పురోగతి))
		తిరిగి;
	ఒకవేళ (పురోగతి->skip_పురోగతి)
		తిరిగి;
	now = time(0);
	ఒకవేళ (now == చివరి_update)
		తిరిగి;
	చివరి_update = now;

	రాయి("%*llu/%*llu", పురోగతి->log_max, (గుర్తులేని దీర్ఘ దీర్ఘ) val,
	       పురోగతి->log_max, (గుర్తులేని దీర్ఘ దీర్ఘ) పురోగతి->max);
	fprintf(stdout, "%.*s", (2*పురోగతి->log_max)+1, backspaces);
}

శూన్య ext2fs_numeric_పురోగతి_close(ext2_filsys fs,
				   నిర్మాణం ext2fs_numeric_పురోగతి_struct * పురోగతి,
				   స్థిరరాశి అక్షరం *సందేశం_3)
{
	ఒకవేళ (!(fs->flags & EXT2_FLAG_PRINT_పురోగతి))
		తిరిగి;
	fprintf(stdout, "%.*s", (2*పురోగతి->log_max)+1, spaces);
	fprintf(stdout, "%.*s", (2*పురోగతి->log_max)+1, backspaces);
	ఒకవేళ (సందేశం_3)
		fputs(సందేశం_3, stdout);
}
