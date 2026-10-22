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

#अन्तर्भाव "config.h"
#अन्तर्भाव "ext2fs.h"
#अन्तर्भाव "ext2fsP.h"

#अन्तर्भाव <time.h>

स्थिर वर्ण spaces[80], backspaces[80];
स्थिर time_t last_update;

संरचना ext2fs_progress_ops ext2fs_numeric_progress_ops = {
	.init		= ext2fs_numeric_progress_init,
	.update		= ext2fs_numeric_progress_update,
	.close		= ext2fs_numeric_progress_close,
};

शून्य ext2fs_numeric_progress_init(ext2_filsys fs,
				  संरचना ext2fs_numeric_progress_struct * progress,
				  ध्रुव वर्ण *label, __u64 max)
{
	/*
	 * The PRINT_PROGRESS flag turns on or off ALL
	 * progress-related messages, whereas the SKIP_PROGRESS
	 * environment variable prints the start and end messages but
	 * not the numeric countdown in the middle.
	 */
	सचे (!(fs->flags & EXT2_FLAG_PRINT_PROGRESS))
		पटिनिवत्त;

	memset(spaces, ' ', परिमाण(spaces)-1);
	spaces[परिमाण(spaces)-1] = 0;
	memset(backspaces, '\b', परिमाण(backspaces)-1);
	backspaces[परिमाण(backspaces)-1] = 0;

	memset(progress, 0, परिमाण(*progress));
	सचे (ext2fs_safe_getenv("E2FSPROGS_SKIP_PROGRESS"))
		progress->skip_progress++;


	/*
	 * Figure out how many digits we need (always at least one)
	 */
	progress->max = max;
	progress->log_max = ext2fs_log10_u64(max) + 1;

	सचे (label) {
		fputs(label, stdout);
		fflush(stdout);
	}
	last_update = 0;
}

शून्य ext2fs_numeric_progress_update(ext2_filsys fs,
				    संरचना ext2fs_numeric_progress_struct * progress,
				    __u64 val)
{
	time_t now;

	सचे (!(fs->flags & EXT2_FLAG_PRINT_PROGRESS))
		पटिनिवत्त;
	सचे (progress->skip_progress)
		पटिनिवत्त;
	now = time(0);
	सचे (now == last_update)
		पटिनिवत्त;
	last_update = now;

	printf("%*llu/%*llu", progress->log_max, (unsigned long long) val,
	       progress->log_max, (unsigned long long) progress->max);
	fprintf(stdout, "%.*s", (2*progress->log_max)+1, backspaces);
}

शून्य ext2fs_numeric_progress_close(ext2_filsys fs,
				   संरचना ext2fs_numeric_progress_struct * progress,
				   ध्रुव वर्ण *message)
{
	सचे (!(fs->flags & EXT2_FLAG_PRINT_PROGRESS))
		पटिनिवत्त;
	fprintf(stdout, "%.*s", (2*progress->log_max)+1, spaces);
	fprintf(stdout, "%.*s", (2*progress->log_max)+1, backspaces);
	सचे (message)
		fputs(message, stdout);
}
