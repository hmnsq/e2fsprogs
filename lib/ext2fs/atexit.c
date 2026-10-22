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

#اگر_تعریف_نشده LARGEFILE_منبع
#تعریف_کن LARGEFILE_منبع
#پایان_اگر
#اگر_تعریف_نشده LARGEFILE64_منبع
#تعریف_کن LARGEFILE64_منبع
#پایان_اگر

#ضمن "config.h"
#ضمن <stdlib.h>

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "ext2fsP.h"

بنية exit_داده {
	ext2_exit_fn func;
	فراغ *داده;
};

ساكن بنية exit_داده *اقلام;
ساكن size_t nr_اقلام;

ساكن فراغ handle_exit(فراغ)
{
	بنية exit_داده *ed;

	لكل (ed = اقلام + nr_اقلام - 1; ed >= اقلام; ed--) {
		إذا (ed->func == عدم)
			استمر;
		ed->func(ed->داده);
	}

	ext2fs_free_mem(&اقلام);
	nr_اقلام = 0;
}

/*
 * Schedule a function to be called at (normal) program termination.
 * If you want this to be called during a signal exit, you must capture
 * the signal and call exit() yourself!
 */
errcode_t ext2fs_جمع_کن_exit_fn(ext2_exit_fn func, فراغ *داده)
{
	بنية exit_داده *ed, *free_ed = عدم;
	size_t x;
	errcode_t ret;

	إذا (func == عدم)
		أرجع EXT2_ET_INVALID_آرگومان;

	لكل (x = 0, ed = اقلام; x < nr_اقلام; x++, ed++) {
		إذا (ed->func == func && ed->داده == داده)
			أرجع EXT2_ET_FILE_EXISTS;
		إذا (ed->func == عدم)
			free_ed = ed;
	}

	إذا (free_ed) {
		free_ed->func = func;
		free_ed->داده = داده;
		أرجع 0;
	}

	إذا (nr_اقلام == 0) {
		ret = atexit(handle_exit);
		إذا (ret)
			أرجع ret;
	}

	ret = ext2fs_resize_mem(0, (nr_اقلام + 1) * حجم(بنية exit_داده),
				&اقلام);
	إذا (ret)
		أرجع ret;

	اقلام[nr_اقلام].func = func;
	اقلام[nr_اقلام].داده = داده;
	nr_اقلام++;

	أرجع 0;
}

/* Remove a function from the exit cleanup list. */
errcode_t ext2fs_remove_exit_fn(ext2_exit_fn func, فراغ *داده)
{
	بنية exit_داده *ed;
	size_t x;

	إذا (func == عدم)
		أرجع EXT2_ET_INVALID_آرگومان;

	لكل (x = 0, ed = اقلام; x < nr_اقلام; x++, ed++) {
		إذا (ed->func == عدم)
			أرجع 0;
		إذا (ed->func == func && ed->داده == داده) {
			size_t sz = (nr_اقلام - (x + 1)) *
				    حجم(بنية exit_داده);
			memmove(ed, ed + 1, sz);
			memset(اقلام + nr_اقلام - 1, 0,
			       حجم(بنية exit_داده));
		}
	}

	أرجع 0;
}
