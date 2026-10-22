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

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ "ext2fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fsP.h"

#ଅନ୍ତର୍ଭାଵ <time.h>

ସ୍ଥିର ଅକ୍ଷର spaces[80], backspaces[80];
ସ୍ଥିର time_t last_update;

ସଂରଚନା ext2fs_progress_ops ext2fs_numeric_progress_ops = {
	.init		= ext2fs_numeric_progress_init,
	.update		= ext2fs_numeric_progress_update,
	.close		= ext2fs_numeric_progress_close,
};

ଶୂନ୍ୟ ext2fs_numeric_progress_init(ext2_filsys fs,
				  ସଂରଚନା ext2fs_numeric_progress_struct * progress,
				  ଅଚଳ ଅକ୍ଷର *label, __u64 max)
{
	/*
	 * The PRINT_PROGRESS flag turns on or off ALL
	 * progress-related messages, whereas the SKIP_PROGRESS
	 * environment variable prints the start and end messages but
	 * not the numeric countdown in the middle.
	 */
	ଯଦି (!(fs->flags & EXT2_FLAG_PRINT_PROGRESS))
		ଫେରନ୍ତୁ;

	memset(spaces, ' ', ଆକାର(spaces)-1);
	spaces[ଆକାର(spaces)-1] = 0;
	memset(backspaces, '\b', ଆକାର(backspaces)-1);
	backspaces[ଆକାର(backspaces)-1] = 0;

	memset(progress, 0, ଆକାର(*progress));
	ଯଦି (ext2fs_safe_getenv("E2FSPROGS_SKIP_PROGRESS"))
		progress->skip_progress++;


	/*
	 * Figure out how many digits we need (always at least one)
	 */
	progress->max = max;
	progress->log_max = ext2fs_log10_u64(max) + 1;

	ଯଦି (label) {
		fputs(label, stdout);
		fflush(stdout);
	}
	last_update = 0;
}

ଶୂନ୍ୟ ext2fs_numeric_progress_update(ext2_filsys fs,
				    ସଂରଚନା ext2fs_numeric_progress_struct * progress,
				    __u64 val)
{
	time_t now;

	ଯଦି (!(fs->flags & EXT2_FLAG_PRINT_PROGRESS))
		ଫେରନ୍ତୁ;
	ଯଦି (progress->skip_progress)
		ଫେରନ୍ତୁ;
	now = time(0);
	ଯଦି (now == last_update)
		ଫେରନ୍ତୁ;
	last_update = now;

	printf("%*llu/%*llu", progress->log_max, (unsigned long long) val,
	       progress->log_max, (unsigned long long) progress->max);
	fprintf(stdout, "%.*s", (2*progress->log_max)+1, backspaces);
}

ଶୂନ୍ୟ ext2fs_numeric_progress_close(ext2_filsys fs,
				   ସଂରଚନା ext2fs_numeric_progress_struct * progress,
				   ଅଚଳ ଅକ୍ଷର *message)
{
	ଯଦି (!(fs->flags & EXT2_FLAG_PRINT_PROGRESS))
		ଫେରନ୍ତୁ;
	fprintf(stdout, "%.*s", (2*progress->log_max)+1, spaces);
	fprintf(stdout, "%.*s", (2*progress->log_max)+1, backspaces);
	ଯଦି (message)
		fputs(message, stdout);
}
