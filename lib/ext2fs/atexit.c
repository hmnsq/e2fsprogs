/*
 * atexit.c --- Clean things up when we exit normally.
 *
 * Copyright Oracle, 2014
 * Author Darrick J. Wong <darrick.wong@oracle.com>
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#جيڪڏهن_وصف_نه_ٿيل LARGEFILE_ذريعو
#وصف_ڪريو LARGEFILE_ذريعو
#جيڪڏهن_پورو
#جيڪڏهن_وصف_نه_ٿيل LARGEFILE64_ذريعو
#وصف_ڪريو LARGEFILE64_ذريعو
#جيڪڏهن_پورو

#ضمن "config.h"
#ضمن <stdlib.h>

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "ext2fsP.h"

بنية exit_معلومات {
	ext2_exit_fn func;
	فراغ *معلومات;
};

ساكن بنية exit_معلومات *شيون;
ساكن size_t nr_شيون;

ساكن فراغ handle_exit(فراغ)
{
	بنية exit_معلومات *ed;

	لكل (ed = شيون + nr_شيون - 1; ed >= شيون; ed--) {
		إذا (ed->func == عدم)
			استمر;
		ed->func(ed->معلومات);
	}

	ext2fs_free_mem(&شيون);
	nr_شيون = 0;
}

/*
 * Schedule a function to be called at (normal) program termination.
 * If you want this to be called during a signal exit, you must capture
 * the signal and call exit() yourself!
 */
errcode_t ext2fs_جوڙيو_exit_fn(ext2_exit_fn func, فراغ *معلومات)
{
	بنية exit_معلومات *ed, *free_ed = عدم;
	size_t x;
	errcode_t ret;

	إذا (func == عدم)
		أرجع EXT2_ET_INVALID_دليل;

	لكل (x = 0, ed = شيون; x < nr_شيون; x++, ed++) {
		إذا (ed->func == func && ed->معلومات == معلومات)
			أرجع EXT2_ET_FILE_EXISTS;
		إذا (ed->func == عدم)
			free_ed = ed;
	}

	إذا (free_ed) {
		free_ed->func = func;
		free_ed->معلومات = معلومات;
		أرجع 0;
	}

	إذا (nr_شيون == 0) {
		ret = atexit(handle_exit);
		إذا (ret)
			أرجع ret;
	}

	ret = ext2fs_resize_mem(0, (nr_شيون + 1) * حجم(بنية exit_معلومات),
				&شيون);
	إذا (ret)
		أرجع ret;

	شيون[nr_شيون].func = func;
	شيون[nr_شيون].معلومات = معلومات;
	nr_شيون++;

	أرجع 0;
}

/* Remove a function from the exit cleanup list. */
errcode_t ext2fs_remove_exit_fn(ext2_exit_fn func, فراغ *معلومات)
{
	بنية exit_معلومات *ed;
	size_t x;

	إذا (func == عدم)
		أرجع EXT2_ET_INVALID_دليل;

	لكل (x = 0, ed = شيون; x < nr_شيون; x++, ed++) {
		إذا (ed->func == عدم)
			أرجع 0;
		إذا (ed->func == func && ed->معلومات == معلومات) {
			size_t sz = (nr_شيون - (x + 1)) *
				    حجم(بنية exit_معلومات);
			memmove(ed, ed + 1, sz);
			memset(شيون + nr_شيون - 1, 0,
			       حجم(بنية exit_معلومات));
		}
	}

	أرجع 0;
}
