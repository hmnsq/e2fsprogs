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

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ "ext2fs.h"
#അന്തര്ഭാവ "ext2fsP.h"

#അന്തര്ഭാവ <time.h>

സ്ഥിതം അക്ഷരം spaces[80], backspaces[80];
സ്ഥിതം time_t last_update;

ഘടന ext2fs_progress_ops ext2fs_numeric_progress_ops = {
	.init		= ext2fs_numeric_progress_init,
	.update		= ext2fs_numeric_progress_update,
	.close		= ext2fs_numeric_progress_close,
};

ശൂന്യം ext2fs_numeric_progress_init(ext2_filsys fs,
				  ഘടന ext2fs_numeric_progress_struct * progress,
				  സ്ഥിരം അക്ഷരം *label, __u64 max)
{
	/*
	 * The PRINT_PROGRESS flag turns on or off ALL
	 * progress-related messages, whereas the SKIP_PROGRESS
	 * environment variable prints the start and end messages but
	 * not the numeric countdown in the middle.
	 */
	എങ്കിൽ (!(fs->flags & EXT2_FLAG_PRINT_PROGRESS))
		തിരികെ;

	memset(spaces, ' ', വലുപ്പം(spaces)-1);
	spaces[വലുപ്പം(spaces)-1] = 0;
	memset(backspaces, '\b', വലുപ്പം(backspaces)-1);
	backspaces[വലുപ്പം(backspaces)-1] = 0;

	memset(progress, 0, വലുപ്പം(*progress));
	എങ്കിൽ (ext2fs_safe_getenv("E2FSPROGS_SKIP_PROGRESS"))
		progress->skip_progress++;


	/*
	 * Figure out how many digits we need (always at least one)
	 */
	progress->max = max;
	progress->log_max = ext2fs_log10_u64(max) + 1;

	എങ്കിൽ (label) {
		fputs(label, stdout);
		fflush(stdout);
	}
	last_update = 0;
}

ശൂന്യം ext2fs_numeric_progress_update(ext2_filsys fs,
				    ഘടന ext2fs_numeric_progress_struct * progress,
				    __u64 val)
{
	time_t now;

	എങ്കിൽ (!(fs->flags & EXT2_FLAG_PRINT_PROGRESS))
		തിരികെ;
	എങ്കിൽ (progress->skip_progress)
		തിരികെ;
	now = time(0);
	എങ്കിൽ (now == last_update)
		തിരികെ;
	last_update = now;

	printf("%*llu/%*llu", progress->log_max, (unsigned long long) val,
	       progress->log_max, (unsigned long long) progress->max);
	fprintf(stdout, "%.*s", (2*progress->log_max)+1, backspaces);
}

ശൂന്യം ext2fs_numeric_progress_close(ext2_filsys fs,
				   ഘടന ext2fs_numeric_progress_struct * progress,
				   സ്ഥിരം അക്ഷരം *message)
{
	എങ്കിൽ (!(fs->flags & EXT2_FLAG_PRINT_PROGRESS))
		തിരികെ;
	fprintf(stdout, "%.*s", (2*progress->log_max)+1, spaces);
	fprintf(stdout, "%.*s", (2*progress->log_max)+1, backspaces);
	എങ്കിൽ (message)
		fputs(message, stdout);
}
