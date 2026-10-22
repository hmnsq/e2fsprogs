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

#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ "ext2fs.h"
#અન્તર્ભાવ "ext2fsP.h"

#અન્તર્ભાવ <time.h>

સ્થિર અક્ષર spaces[80], backspaces[80];
સ્થિર time_t last_update;

સંરચના ext2fs_progress_ops ext2fs_numeric_progress_ops = {
	.init		= ext2fs_numeric_progress_init,
	.update		= ext2fs_numeric_progress_update,
	.close		= ext2fs_numeric_progress_close,
};

શૂન્ય ext2fs_numeric_progress_init(ext2_filsys fs,
				  સંરચના ext2fs_numeric_progress_struct * progress,
				  અચલ અક્ષર *label, __u64 max)
{
	/*
	 * The PRINT_PROGRESS flag turns on or off ALL
	 * progress-related messages, whereas the SKIP_PROGRESS
	 * environment variable prints the start and end messages but
	 * not the numeric countdown in the middle.
	 */
	જો (!(fs->flags & EXT2_FLAG_PRINT_PROGRESS))
		પાછા;

	memset(spaces, ' ', કદ(spaces)-1);
	spaces[કદ(spaces)-1] = 0;
	memset(backspaces, '\b', કદ(backspaces)-1);
	backspaces[કદ(backspaces)-1] = 0;

	memset(progress, 0, કદ(*progress));
	જો (ext2fs_safe_getenv("E2FSPROGS_SKIP_PROGRESS"))
		progress->skip_progress++;


	/*
	 * Figure out how many digits we need (always at least one)
	 */
	progress->max = max;
	progress->log_max = ext2fs_log10_u64(max) + 1;

	જો (label) {
		fputs(label, stdout);
		fflush(stdout);
	}
	last_update = 0;
}

શૂન્ય ext2fs_numeric_progress_update(ext2_filsys fs,
				    સંરચના ext2fs_numeric_progress_struct * progress,
				    __u64 val)
{
	time_t now;

	જો (!(fs->flags & EXT2_FLAG_PRINT_PROGRESS))
		પાછા;
	જો (progress->skip_progress)
		પાછા;
	now = time(0);
	જો (now == last_update)
		પાછા;
	last_update = now;

	printf("%*llu/%*llu", progress->log_max, (unsigned long long) val,
	       progress->log_max, (unsigned long long) progress->max);
	fprintf(stdout, "%.*s", (2*progress->log_max)+1, backspaces);
}

શૂન્ય ext2fs_numeric_progress_close(ext2_filsys fs,
				   સંરચના ext2fs_numeric_progress_struct * progress,
				   અચલ અક્ષર *message)
{
	જો (!(fs->flags & EXT2_FLAG_PRINT_PROGRESS))
		પાછા;
	fprintf(stdout, "%.*s", (2*progress->log_max)+1, spaces);
	fprintf(stdout, "%.*s", (2*progress->log_max)+1, backspaces);
	જો (message)
		fputs(message, stdout);
}
