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
ساكن time_t آخری_update;

بنية ext2fs_ترقی_ops ext2fs_numeric_ترقی_ops = {
	.init		= ext2fs_numeric_ترقی_init,
	.update		= ext2fs_numeric_ترقی_update,
	.close		= ext2fs_numeric_ترقی_close,
};

فراغ ext2fs_numeric_ترقی_init(ext2_filsys fs,
				  بنية ext2fs_numeric_ترقی_struct * ترقی,
				  ثابت حرف *label, __u64 max)
{
	/*
	 * The PRINT_PROGRESS flag turns on or off ALL
	 * progress-related messages, whereas the SKIP_PROGRESS
	 * environment variable prints the start and end messages but
	 * not the numeric countdown in the middle.
	 */
	إذا (!(fs->flags & EXT2_FLAG_PRINT_ترقی))
		أرجع;

	memset(spaces, ' ', حجم(spaces)-1);
	spaces[حجم(spaces)-1] = 0;
	memset(backspaces, '\b', حجم(backspaces)-1);
	backspaces[حجم(backspaces)-1] = 0;

	memset(ترقی, 0, حجم(*ترقی));
	إذا (ext2fs_محفوظ_getenv("E2FSPROGS_SKIP_PROGRESS"))
		ترقی->skip_ترقی++;


	/*
	 * Figure out how many digits we need (always at least one)
	 */
	ترقی->max = max;
	ترقی->log_max = ext2fs_log10_u64(max) + 1;

	إذا (label) {
		fputs(label, stdout);
		fflush(stdout);
	}
	آخری_update = 0;
}

فراغ ext2fs_numeric_ترقی_update(ext2_filsys fs,
				    بنية ext2fs_numeric_ترقی_struct * ترقی,
				    __u64 val)
{
	time_t now;

	إذا (!(fs->flags & EXT2_FLAG_PRINT_ترقی))
		أرجع;
	إذا (ترقی->skip_ترقی)
		أرجع;
	now = time(0);
	إذا (now == آخری_update)
		أرجع;
	آخری_update = now;

	لکھو("%*llu/%*llu", ترقی->log_max, (بنانشانی لما لما) val,
	       ترقی->log_max, (بنانشانی لما لما) ترقی->max);
	fprintf(stdout, "%.*s", (2*ترقی->log_max)+1, backspaces);
}

فراغ ext2fs_numeric_ترقی_close(ext2_filsys fs,
				   بنية ext2fs_numeric_ترقی_struct * ترقی,
				   ثابت حرف *سنیہا)
{
	إذا (!(fs->flags & EXT2_FLAG_PRINT_ترقی))
		أرجع;
	fprintf(stdout, "%.*s", (2*ترقی->log_max)+1, spaces);
	fprintf(stdout, "%.*s", (2*ترقی->log_max)+1, backspaces);
	إذا (سنیہا)
		fputs(سنیہا, stdout);
}
