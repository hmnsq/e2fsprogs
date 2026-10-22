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

#شامل "config.h"
#شامل "ext2fs.h"
#شامل "ext2fsP.h"

#شامل <time.h>

ایستا نویسه spaces[80], backspaces[80];
ایستا time_t last_update;

ساختار ext2fs_progress_ops ext2fs_numeric_progress_ops = {
	.init		= ext2fs_numeric_progress_init,
	.update		= ext2fs_numeric_progress_update,
	.close		= ext2fs_numeric_progress_close,
};

تهی ext2fs_numeric_progress_init(ext2_filsys fs,
				  ساختار ext2fs_numeric_progress_struct * progress,
				  ثابت نویسه *label, __u64 max)
{
	/*
	 * The PRINT_PROGRESS flag turns on or off ALL
	 * progress-related messages, whereas the SKIP_PROGRESS
	 * environment variable prints the start and end messages but
	 * not the numeric countdown in the middle.
	 */
	اگر (!(fs->flags & EXT2_FLAG_PRINT_PROGRESS))
		بازگشت;

	memset(spaces, ' ', اندازه(spaces)-1);
	spaces[اندازه(spaces)-1] = 0;
	memset(backspaces, '\b', اندازه(backspaces)-1);
	backspaces[اندازه(backspaces)-1] = 0;

	memset(progress, 0, اندازه(*progress));
	اگر (ext2fs_safe_getenv("E2FSPROGS_SKIP_PROGRESS"))
		progress->skip_progress++;


	/*
	 * Figure out how many digits we need (always at least one)
	 */
	progress->max = max;
	progress->log_max = ext2fs_log10_u64(max) + 1;

	اگر (label) {
		fputs(label, stdout);
		fflush(stdout);
	}
	last_update = 0;
}

تهی ext2fs_numeric_progress_update(ext2_filsys fs,
				    ساختار ext2fs_numeric_progress_struct * progress,
				    __u64 val)
{
	time_t now;

	اگر (!(fs->flags & EXT2_FLAG_PRINT_PROGRESS))
		بازگشت;
	اگر (progress->skip_progress)
		بازگشت;
	now = time(0);
	اگر (now == last_update)
		بازگشت;
	last_update = now;

	printf("%*llu/%*llu", progress->log_max, (unsigned long long) val,
	       progress->log_max, (unsigned long long) progress->max);
	fprintf(stdout, "%.*s", (2*progress->log_max)+1, backspaces);
}

تهی ext2fs_numeric_progress_close(ext2_filsys fs,
				   ساختار ext2fs_numeric_progress_struct * progress,
				   ثابت نویسه *message)
{
	اگر (!(fs->flags & EXT2_FLAG_PRINT_PROGRESS))
		بازگشت;
	fprintf(stdout, "%.*s", (2*progress->log_max)+1, spaces);
	fprintf(stdout, "%.*s", (2*progress->log_max)+1, backspaces);
	اگر (message)
		fputs(message, stdout);
}
