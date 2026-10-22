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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fsP.h"

#ಅನ್ತರ್ಭಾವ <time.h>

ಸ್ಥಿರ ವರ್ಣ spaces[80], backspaces[80];
ಸ್ಥಿರ time_t last_update;

ಸಂರಚನಾ ext2fs_progress_ops ext2fs_numeric_progress_ops = {
	.init		= ext2fs_numeric_progress_init,
	.update		= ext2fs_numeric_progress_update,
	.close		= ext2fs_numeric_progress_close,
};

ಶೂನ್ಯ ext2fs_numeric_progress_init(ext2_filsys fs,
				  ಸಂರಚನಾ ext2fs_numeric_progress_struct * progress,
				  ಧ್ರುವ ವರ್ಣ *label, __u64 max)
{
	/*
	 * The PRINT_PROGRESS flag turns on or off ALL
	 * progress-related messages, whereas the SKIP_PROGRESS
	 * environment variable prints the start and end messages but
	 * not the numeric countdown in the middle.
	 */
	ಯದಿ (!(fs->flags & EXT2_FLAG_PRINT_PROGRESS))
		ಪ್ರತ್ಯಾಗಮ;

	memset(spaces, ' ', ಪರಿಮಾಣ(spaces)-1);
	spaces[ಪರಿಮಾಣ(spaces)-1] = 0;
	memset(backspaces, '\b', ಪರಿಮಾಣ(backspaces)-1);
	backspaces[ಪರಿಮಾಣ(backspaces)-1] = 0;

	memset(progress, 0, ಪರಿಮಾಣ(*progress));
	ಯದಿ (ext2fs_safe_getenv("E2FSPROGS_SKIP_PROGRESS"))
		progress->skip_progress++;


	/*
	 * Figure out how many digits we need (always at least one)
	 */
	progress->max = max;
	progress->log_max = ext2fs_log10_u64(max) + 1;

	ಯದಿ (label) {
		fputs(label, stdout);
		fflush(stdout);
	}
	last_update = 0;
}

ಶೂನ್ಯ ext2fs_numeric_progress_update(ext2_filsys fs,
				    ಸಂರಚನಾ ext2fs_numeric_progress_struct * progress,
				    __u64 val)
{
	time_t now;

	ಯದಿ (!(fs->flags & EXT2_FLAG_PRINT_PROGRESS))
		ಪ್ರತ್ಯಾಗಮ;
	ಯದಿ (progress->skip_progress)
		ಪ್ರತ್ಯಾಗಮ;
	now = time(0);
	ಯದಿ (now == last_update)
		ಪ್ರತ್ಯಾಗಮ;
	last_update = now;

	printf("%*llu/%*llu", progress->log_max, (unsigned long long) val,
	       progress->log_max, (unsigned long long) progress->max);
	fprintf(stdout, "%.*s", (2*progress->log_max)+1, backspaces);
}

ಶೂನ್ಯ ext2fs_numeric_progress_close(ext2_filsys fs,
				   ಸಂರಚನಾ ext2fs_numeric_progress_struct * progress,
				   ಧ್ರುವ ವರ್ಣ *message)
{
	ಯದಿ (!(fs->flags & EXT2_FLAG_PRINT_PROGRESS))
		ಪ್ರತ್ಯಾಗಮ;
	fprintf(stdout, "%.*s", (2*progress->log_max)+1, spaces);
	fprintf(stdout, "%.*s", (2*progress->log_max)+1, backspaces);
	ಯದಿ (message)
		fputs(message, stdout);
}
