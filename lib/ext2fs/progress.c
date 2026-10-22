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
ساكن time_t آخرین_update;

بنية ext2fs_پیشرفت_ops ext2fs_numeric_پیشرفت_ops = {
	.init		= ext2fs_numeric_پیشرفت_init,
	.update		= ext2fs_numeric_پیشرفت_update,
	.close		= ext2fs_numeric_پیشرفت_close,
};

فراغ ext2fs_numeric_پیشرفت_init(ext2_filsys fs,
				  بنية ext2fs_numeric_پیشرفت_struct * پیشرفت,
				  ثابت حرف *label, __u64 max)
{
	/*
	 * The PRINT_PROGRESS flag turns on or off ALL
	 * progress-related messages, whereas the SKIP_PROGRESS
	 * environment variable prints the start and end messages but
	 * not the numeric countdown in the middle.
	 */
	إذا (!(fs->flags & EXT2_FLAG_PRINT_پیشرفت))
		أرجع;

	memset(spaces, ' ', حجم(spaces)-1);
	spaces[حجم(spaces)-1] = 0;
	memset(backspaces, '\b', حجم(backspaces)-1);
	backspaces[حجم(backspaces)-1] = 0;

	memset(پیشرفت, 0, حجم(*پیشرفت));
	إذا (ext2fs_امن_getenv("E2FSPROGS_SKIP_PROGRESS"))
		پیشرفت->skip_پیشرفت++;


	/*
	 * Figure out how many digits we need (always at least one)
	 */
	پیشرفت->max = max;
	پیشرفت->log_max = ext2fs_log10_u64(max) + 1;

	إذا (label) {
		fputs(label, stdout);
		fflush(stdout);
	}
	آخرین_update = 0;
}

فراغ ext2fs_numeric_پیشرفت_update(ext2_filsys fs,
				    بنية ext2fs_numeric_پیشرفت_struct * پیشرفت,
				    __u64 val)
{
	time_t now;

	إذا (!(fs->flags & EXT2_FLAG_PRINT_پیشرفت))
		أرجع;
	إذا (پیشرفت->skip_پیشرفت)
		أرجع;
	now = time(0);
	إذا (now == آخرین_update)
		أرجع;
	آخرین_update = now;

	چاپ_کن("%*llu/%*llu", پیشرفت->log_max, (بیعلامت دراز دراز) val,
	       پیشرفت->log_max, (بیعلامت دراز دراز) پیشرفت->max);
	fprintf(stdout, "%.*s", (2*پیشرفت->log_max)+1, backspaces);
}

فراغ ext2fs_numeric_پیشرفت_close(ext2_filsys fs,
				   بنية ext2fs_numeric_پیشرفت_struct * پیشرفت,
				   ثابت حرف *پیام_3)
{
	إذا (!(fs->flags & EXT2_FLAG_PRINT_پیشرفت))
		أرجع;
	fprintf(stdout, "%.*s", (2*پیشرفت->log_max)+1, spaces);
	fprintf(stdout, "%.*s", (2*پیشرفت->log_max)+1, backspaces);
	إذا (پیام_3)
		fputs(پیام_3, stdout);
}
