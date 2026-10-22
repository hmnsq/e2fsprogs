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

#که_نه_تعریف_شوی LARGEFILE_سرچینه
#تعریف_کړه LARGEFILE_سرچینه
#د_که_پای
#که_نه_تعریف_شوی LARGEFILE64_سرچینه
#تعریف_کړه LARGEFILE64_سرچینه
#د_که_پای

#ضمن "config.h"
#ضمن <stdlib.h>

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "ext2fsP.h"

بنية exit_معلومات {
	ext2_exit_fn func;
	فراغ *معلومات;
};

ساكن بنية exit_معلومات *توکي;
ساكن size_t nr_توکي;

ساكن فراغ handle_exit(فراغ)
{
	بنية exit_معلومات *ed;

	لكل (ed = توکي + nr_توکي - 1; ed >= توکي; ed--) {
		إذا (ed->func == عدم)
			استمر;
		ed->func(ed->معلومات);
	}

	ext2fs_free_mem(&توکي);
	nr_توکي = 0;
}

/*
 * Schedule a function to be called at (normal) program termination.
 * If you want this to be called during a signal exit, you must capture
 * the signal and call exit() yourself!
 */
errcode_t ext2fs_جمع_کړه_exit_fn(ext2_exit_fn func, فراغ *معلومات)
{
	بنية exit_معلومات *ed, *free_ed = عدم;
	size_t x;
	errcode_t ret;

	إذا (func == عدم)
		أرجع EXT2_ET_INVALID_دلیل;

	لكل (x = 0, ed = توکي; x < nr_توکي; x++, ed++) {
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

	إذا (nr_توکي == 0) {
		ret = atexit(handle_exit);
		إذا (ret)
			أرجع ret;
	}

	ret = ext2fs_resize_mem(0, (nr_توکي + 1) * حجم(بنية exit_معلومات),
				&توکي);
	إذا (ret)
		أرجع ret;

	توکي[nr_توکي].func = func;
	توکي[nr_توکي].معلومات = معلومات;
	nr_توکي++;

	أرجع 0;
}

/* Remove a function from the exit cleanup list. */
errcode_t ext2fs_remove_exit_fn(ext2_exit_fn func, فراغ *معلومات)
{
	بنية exit_معلومات *ed;
	size_t x;

	إذا (func == عدم)
		أرجع EXT2_ET_INVALID_دلیل;

	لكل (x = 0, ed = توکي; x < nr_توکي; x++, ed++) {
		إذا (ed->func == عدم)
			أرجع 0;
		إذا (ed->func == func && ed->معلومات == معلومات) {
			size_t sz = (nr_توکي - (x + 1)) *
				    حجم(بنية exit_معلومات);
			memmove(ed, ed + 1, sz);
			memset(توکي + nr_توکي - 1, 0,
			       حجم(بنية exit_معلومات));
		}
	}

	أرجع 0;
}
