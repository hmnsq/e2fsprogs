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
ساكن time_t وروستی_update;

بنية ext2fs_پرمختګ_ops ext2fs_numeric_پرمختګ_ops = {
	.init		= ext2fs_numeric_پرمختګ_init,
	.update		= ext2fs_numeric_پرمختګ_update,
	.close		= ext2fs_numeric_پرمختګ_close,
};

فراغ ext2fs_numeric_پرمختګ_init(ext2_filsys fs,
				  بنية ext2fs_numeric_پرمختګ_struct * پرمختګ,
				  ثابت حرف *label, __u64 max)
{
	/*
	 * The PRINT_PROGRESS flag turns on or off ALL
	 * progress-related messages, whereas the SKIP_PROGRESS
	 * environment variable prints the start and end messages but
	 * not the numeric countdown in the middle.
	 */
	إذا (!(fs->flags & EXT2_FLAG_PRINT_پرمختګ))
		أرجع;

	memset(spaces, ' ', حجم(spaces)-1);
	spaces[حجم(spaces)-1] = 0;
	memset(backspaces, '\b', حجم(backspaces)-1);
	backspaces[حجم(backspaces)-1] = 0;

	memset(پرمختګ, 0, حجم(*پرمختګ));
	إذا (ext2fs_خوندي_getenv("E2FSPROGS_SKIP_PROGRESS"))
		پرمختګ->skip_پرمختګ++;


	/*
	 * Figure out how many digits we need (always at least one)
	 */
	پرمختګ->max = max;
	پرمختګ->log_max = ext2fs_log10_u64(max) + 1;

	إذا (label) {
		fputs(label, stdout);
		fflush(stdout);
	}
	وروستی_update = 0;
}

فراغ ext2fs_numeric_پرمختګ_update(ext2_filsys fs,
				    بنية ext2fs_numeric_پرمختګ_struct * پرمختګ,
				    __u64 val)
{
	time_t now;

	إذا (!(fs->flags & EXT2_FLAG_PRINT_پرمختګ))
		أرجع;
	إذا (پرمختګ->skip_پرمختګ)
		أرجع;
	now = time(0);
	إذا (now == وروستی_update)
		أرجع;
	وروستی_update = now;

	چاپ_کړه("%*llu/%*llu", پرمختګ->log_max, (بینښه اوږد اوږد) val,
	       پرمختګ->log_max, (بینښه اوږد اوږد) پرمختګ->max);
	fprintf(stdout, "%.*s", (2*پرمختګ->log_max)+1, backspaces);
}

فراغ ext2fs_numeric_پرمختګ_close(ext2_filsys fs,
				   بنية ext2fs_numeric_پرمختګ_struct * پرمختګ,
				   ثابت حرف *پیغام_3)
{
	إذا (!(fs->flags & EXT2_FLAG_PRINT_پرمختګ))
		أرجع;
	fprintf(stdout, "%.*s", (2*پرمختګ->log_max)+1, spaces);
	fprintf(stdout, "%.*s", (2*پرمختګ->log_max)+1, backspaces);
	إذا (پیغام_3)
		fputs(پیغام_3, stdout);
}
