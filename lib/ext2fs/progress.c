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

#অন্তর্ভাব "config.h"
#অন্তর্ভাব "ext2fs.h"
#অন্তর্ভাব "ext2fsP.h"

#অন্তর্ভাব <time.h>

স্থির অক্ষর spaces[80], backspaces[80];
স্থির time_t last_update;

কাঠামো ext2fs_progress_ops ext2fs_numeric_progress_ops = {
	.init		= ext2fs_numeric_progress_init,
	.update		= ext2fs_numeric_progress_update,
	.close		= ext2fs_numeric_progress_close,
};

শূন্য ext2fs_numeric_progress_init(ext2_filsys fs,
				  কাঠামো ext2fs_numeric_progress_struct * progress,
				  ধ্রুবক অক্ষর *label, __u64 max)
{
	/*
	 * The PRINT_PROGRESS flag turns on or off ALL
	 * progress-related messages, whereas the SKIP_PROGRESS
	 * environment variable prints the start and end messages but
	 * not the numeric countdown in the middle.
	 */
	যদি (!(fs->flags & EXT2_FLAG_PRINT_PROGRESS))
		ফেরত;

	memset(spaces, ' ', আকার(spaces)-1);
	spaces[আকার(spaces)-1] = 0;
	memset(backspaces, '\b', আকার(backspaces)-1);
	backspaces[আকার(backspaces)-1] = 0;

	memset(progress, 0, আকার(*progress));
	যদি (ext2fs_safe_getenv("E2FSPROGS_SKIP_PROGRESS"))
		progress->skip_progress++;


	/*
	 * Figure out how many digits we need (always at least one)
	 */
	progress->max = max;
	progress->log_max = ext2fs_log10_u64(max) + 1;

	যদি (label) {
		fputs(label, stdout);
		fflush(stdout);
	}
	last_update = 0;
}

শূন্য ext2fs_numeric_progress_update(ext2_filsys fs,
				    কাঠামো ext2fs_numeric_progress_struct * progress,
				    __u64 val)
{
	time_t now;

	যদি (!(fs->flags & EXT2_FLAG_PRINT_PROGRESS))
		ফেরত;
	যদি (progress->skip_progress)
		ফেরত;
	now = time(0);
	যদি (now == last_update)
		ফেরত;
	last_update = now;

	printf("%*llu/%*llu", progress->log_max, (unsigned long long) val,
	       progress->log_max, (unsigned long long) progress->max);
	fprintf(stdout, "%.*s", (2*progress->log_max)+1, backspaces);
}

শূন্য ext2fs_numeric_progress_close(ext2_filsys fs,
				   কাঠামো ext2fs_numeric_progress_struct * progress,
				   ধ্রুবক অক্ষর *message)
{
	যদি (!(fs->flags & EXT2_FLAG_PRINT_PROGRESS))
		ফেরত;
	fprintf(stdout, "%.*s", (2*progress->log_max)+1, spaces);
	fprintf(stdout, "%.*s", (2*progress->log_max)+1, backspaces);
	যদি (message)
		fputs(message, stdout);
}
