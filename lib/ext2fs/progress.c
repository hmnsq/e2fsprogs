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

ساکن حرف spaces[80], backspaces[80];
ساکن time_t آخری_update;

ساخت ext2fs_پیش_رفت_ops ext2fs_numeric_پیش_رفت_ops = {
	.init		= ext2fs_numeric_پیش_رفت_init,
	.update		= ext2fs_numeric_پیش_رفت_update,
	.close		= ext2fs_numeric_پیش_رفت_close,
};

خالی ext2fs_numeric_پیش_رفت_init(ext2_filsys fs,
				  ساخت ext2fs_numeric_پیش_رفت_struct * پیش_رفت,
				  ثابت حرف *label, __u64 max)
{
	/*
	 * The PRINT_PROGRESS flag turns on or off ALL
	 * progress-related messages, whereas the SKIP_PROGRESS
	 * environment variable prints the start and end messages but
	 * not the numeric countdown in the middle.
	 */
	اگر (!(fs->flags & EXT2_FLAG_PRINT_پیش_رفت))
		واپس;

	memset(spaces, ' ', حجم(spaces)-1);
	spaces[حجم(spaces)-1] = 0;
	memset(backspaces, '\b', حجم(backspaces)-1);
	backspaces[حجم(backspaces)-1] = 0;

	memset(پیش_رفت, 0, حجم(*پیش_رفت));
	اگر (ext2fs_محفوظ_getenv("E2FSPROGS_SKIP_PROGRESS"))
		پیش_رفت->skip_پیش_رفت++;


	/*
	 * Figure out how many digits we need (always at least one)
	 */
	پیش_رفت->max = max;
	پیش_رفت->log_max = ext2fs_log10_u64(max) + 1;

	اگر (label) {
		fputs(label, stdout);
		fflush(stdout);
	}
	آخری_update = 0;
}

خالی ext2fs_numeric_پیش_رفت_update(ext2_filsys fs,
				    ساخت ext2fs_numeric_پیش_رفت_struct * پیش_رفت,
				    __u64 val)
{
	time_t now;

	اگر (!(fs->flags & EXT2_FLAG_PRINT_پیش_رفت))
		واپس;
	اگر (پیش_رفت->skip_پیش_رفت)
		واپس;
	now = time(0);
	اگر (now == آخری_update)
		واپس;
	آخری_update = now;

	لکھو("%*llu/%*llu", پیش_رفت->log_max, (غیرعلامتی طویل طویل) val,
	       پیش_رفت->log_max, (غیرعلامتی طویل طویل) پیش_رفت->max);
	fprintf(stdout, "%.*s", (2*پیش_رفت->log_max)+1, backspaces);
}

خالی ext2fs_numeric_پیش_رفت_close(ext2_filsys fs,
				   ساخت ext2fs_numeric_پیش_رفت_struct * پیش_رفت,
				   ثابت حرف *پیغام_3)
{
	اگر (!(fs->flags & EXT2_FLAG_PRINT_پیش_رفت))
		واپس;
	fprintf(stdout, "%.*s", (2*پیش_رفت->log_max)+1, spaces);
	fprintf(stdout, "%.*s", (2*پیش_رفت->log_max)+1, backspaces);
	اگر (پیغام_3)
		fputs(پیغام_3, stdout);
}
