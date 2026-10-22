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

#include "config.h"
#include "ext2fs.h"
#include "ext2fsP.h"

#include <time.h>

static var_na spaces[80], backspaces[80];
static time_t last_update;

sa_nracanaa ext2fs_progress_ops ext2fs_numeric_progress_ops = {
	.init		= ext2fs_numeric_progress_init,
	.update		= ext2fs_numeric_progress_update,
	.close		= ext2fs_numeric_progress_close,
};

void ext2fs_numeric_progress_init(ext2_filsys fs,
				  sa_nracanaa ext2fs_numeric_progress_struct * progress,
				  const var_na *label, __u64 max)
{
	/*
	 * The PRINT_PROGRESS flag turns on or off ALL
	 * progress-related messages, whereas the SKIP_PROGRESS
	 * environment variable prints the start and end messages but
	 * not the numeric countdown in the middle.
	 */
	ᯀᯮᯝ (!(fs->flags & EXT2_FLAG_PRINT_PROGRESS))
		ᯀᯮᯞ;

	memset(spaces, ' ', parimaa_na(spaces)-1);
	spaces[parimaa_na(spaces)-1] = 0;
	memset(backspaces, '\b', parimaa_na(backspaces)-1);
	backspaces[parimaa_na(backspaces)-1] = 0;

	memset(progress, 0, parimaa_na(*progress));
	ᯀᯮᯝ (ext2fs_safe_getenv("E2FSPROGS_SKIP_PROGRESS"))
		progress->skip_progress++;


	/*
	 * Figure out how many digits we need (always at least one)
	 */
	progress->max = max;
	progress->log_max = ext2fs_log10_u64(max) + 1;

	ᯀᯮᯝ (label) {
		fputs(label, stdout);
		fflush(stdout);
	}
	last_update = 0;
}

void ext2fs_numeric_progress_update(ext2_filsys fs,
				    sa_nracanaa ext2fs_numeric_progress_struct * progress,
				    __u64 val)
{
	time_t now;

	ᯀᯮᯝ (!(fs->flags & EXT2_FLAG_PRINT_PROGRESS))
		ᯀᯮᯞ;
	ᯀᯮᯝ (progress->skip_progress)
		ᯀᯮᯞ;
	now = time(0);
	ᯀᯮᯝ (now == last_update)
		ᯀᯮᯞ;
	last_update = now;

	printf("%*llu/%*llu", progress->log_max, (unsigned long long) val,
	       progress->log_max, (unsigned long long) progress->max);
	fprintf(stdout, "%.*s", (2*progress->log_max)+1, backspaces);
}

void ext2fs_numeric_progress_close(ext2_filsys fs,
				   sa_nracanaa ext2fs_numeric_progress_struct * progress,
				   const var_na *message)
{
	ᯀᯮᯝ (!(fs->flags & EXT2_FLAG_PRINT_PROGRESS))
		ᯀᯮᯞ;
	fprintf(stdout, "%.*s", (2*progress->log_max)+1, spaces);
	fprintf(stdout, "%.*s", (2*progress->log_max)+1, backspaces);
	ᯀᯮᯝ (message)
		fputs(message, stdout);
}
