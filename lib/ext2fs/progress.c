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
ساكن time_t آخري_update;

بنية ext2fs_ترقي_ops ext2fs_numeric_ترقي_ops = {
	.init		= ext2fs_numeric_ترقي_init,
	.update		= ext2fs_numeric_ترقي_update,
	.close		= ext2fs_numeric_ترقي_close,
};

فراغ ext2fs_numeric_ترقي_init(ext2_filsys fs,
				  بنية ext2fs_numeric_ترقي_struct * ترقي,
				  ثابت حرف *label, __u64 max)
{
	/*
	 * The PRINT_PROGRESS flag turns on or off ALL
	 * progress-related messages, whereas the SKIP_PROGRESS
	 * environment variable prints the start and end messages but
	 * not the numeric countdown in the middle.
	 */
	إذا (!(fs->flags & EXT2_FLAG_PRINT_ترقي))
		أرجع;

	memset(spaces, ' ', حجم(spaces)-1);
	spaces[حجم(spaces)-1] = 0;
	memset(backspaces, '\b', حجم(backspaces)-1);
	backspaces[حجم(backspaces)-1] = 0;

	memset(ترقي, 0, حجم(*ترقي));
	إذا (ext2fs_محفوظ_getenv("E2FSPROGS_SKIP_PROGRESS"))
		ترقي->skip_ترقي++;


	/*
	 * Figure out how many digits we need (always at least one)
	 */
	ترقي->max = max;
	ترقي->log_max = ext2fs_log10_u64(max) + 1;

	إذا (label) {
		fputs(label, stdout);
		fflush(stdout);
	}
	آخري_update = 0;
}

فراغ ext2fs_numeric_ترقي_update(ext2_filsys fs,
				    بنية ext2fs_numeric_ترقي_struct * ترقي,
				    __u64 val)
{
	time_t now;

	إذا (!(fs->flags & EXT2_FLAG_PRINT_ترقي))
		أرجع;
	إذا (ترقي->skip_ترقي)
		أرجع;
	now = time(0);
	إذا (now == آخري_update)
		أرجع;
	آخري_update = now;

	لکو("%*llu/%*llu", ترقي->log_max, (بنانشان ڊگهو ڊگهو) val,
	       ترقي->log_max, (بنانشان ڊگهو ڊگهو) ترقي->max);
	fprintf(stdout, "%.*s", (2*ترقي->log_max)+1, backspaces);
}

فراغ ext2fs_numeric_ترقي_close(ext2_filsys fs,
				   بنية ext2fs_numeric_ترقي_struct * ترقي,
				   ثابت حرف *پيغام)
{
	إذا (!(fs->flags & EXT2_FLAG_PRINT_ترقي))
		أرجع;
	fprintf(stdout, "%.*s", (2*ترقي->log_max)+1, spaces);
	fprintf(stdout, "%.*s", (2*ترقي->log_max)+1, backspaces);
	إذا (پيغام)
		fputs(پيغام, stdout);
}
