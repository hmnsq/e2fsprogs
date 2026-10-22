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

#ضمن "config.h"
#ضمن "ext2fs.h"
#ضمن "ext2fsP.h"

#ضمن <time.h>

ساكن حرف spaces[80], backspaces[80];
ساكن time_t last_update;

بنية ext2fs_progress_ops ext2fs_numeric_progress_ops = {
	.init		= ext2fs_numeric_progress_init,
	.update		= ext2fs_numeric_progress_update,
	.close		= ext2fs_numeric_progress_close,
};

فراغ ext2fs_numeric_progress_init(ext2_filsys fs,
				  بنية ext2fs_numeric_progress_struct * progress,
				  ثابت حرف *label, __u64 max)
{
	/*
	 * The PRINT_PROGRESS flag turns on or off ALL
	 * progress-related messages, whereas the SKIP_PROGRESS
	 * environment variable prints the start and end messages but
	 * not the numeric countdown in the middle.
	 */
	إذا (!(fs->flags & EXT2_FLAG_PRINT_PROGRESS))
		أرجع;

	memset(spaces, ' ', حجم(spaces)-1);
	spaces[حجم(spaces)-1] = 0;
	memset(backspaces, '\b', حجم(backspaces)-1);
	backspaces[حجم(backspaces)-1] = 0;

	memset(progress, 0, حجم(*progress));
	إذا (ext2fs_safe_getenv("E2FSPROGS_SKIP_PROGRESS"))
		progress->skip_progress++;


	/*
	 * Figure out how many digits we need (always at least one)
	 */
	progress->max = max;
	progress->log_max = ext2fs_log10_u64(max) + 1;

	إذا (label) {
		fputs(label, stdout);
		fflush(stdout);
	}
	last_update = 0;
}

فراغ ext2fs_numeric_progress_update(ext2_filsys fs,
				    بنية ext2fs_numeric_progress_struct * progress,
				    __u64 val)
{
	time_t now;

	إذا (!(fs->flags & EXT2_FLAG_PRINT_PROGRESS))
		أرجع;
	إذا (progress->skip_progress)
		أرجع;
	now = time(0);
	إذا (now == last_update)
		أرجع;
	last_update = now;

	printf("%*llu/%*llu", progress->log_max, (unsigned long long) val,
	       progress->log_max, (unsigned long long) progress->max);
	fprintf(stdout, "%.*s", (2*progress->log_max)+1, backspaces);
}

فراغ ext2fs_numeric_progress_close(ext2_filsys fs,
				   بنية ext2fs_numeric_progress_struct * progress,
				   ثابت حرف *message)
{
	إذا (!(fs->flags & EXT2_FLAG_PRINT_PROGRESS))
		أرجع;
	fprintf(stdout, "%.*s", (2*progress->log_max)+1, spaces);
	fprintf(stdout, "%.*s", (2*progress->log_max)+1, backspaces);
	إذا (message)
		fputs(message, stdout);
}
