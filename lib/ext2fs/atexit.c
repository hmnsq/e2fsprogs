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

#ifndef _LARGEFILE_SOURCE
#define _LARGEFILE_SOURCE
#endif
#ifndef _LARGEFILE64_SOURCE
#define _LARGEFILE64_SOURCE
#endif

#ضمن "config.h"
#ضمن <stdlib.h>

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "ext2fsP.h"

بنية exit_data {
	ext2_exit_fn func;
	فراغ *data;
};

ساكن بنية exit_data *بۇيۇملار;
ساكن size_t nr_بۇيۇملار;

ساكن فراغ handle_exit(فراغ)
{
	بنية exit_data *ed;

	لكل (ed = بۇيۇملار + nr_بۇيۇملار - 1; ed >= بۇيۇملار; ed--) {
		إذا (ed->func == عدم)
			استمر;
		ed->func(ed->data);
	}

	ext2fs_free_mem(&بۇيۇملار);
	nr_بۇيۇملار = 0;
}

/*
 * Schedule a function to be called at (normal) program termination.
 * If you want this to be called during a signal exit, you must capture
 * the signal and call exit() yourself!
 */
errcode_t ext2fs_add_exit_fn(ext2_exit_fn func, فراغ *data)
{
	بنية exit_data *ed, *free_ed = عدم;
	size_t x;
	errcode_t ret;

	إذا (func == عدم)
		أرجع EXT2_ET_INVALID_ARGUMENT;

	لكل (x = 0, ed = بۇيۇملار; x < nr_بۇيۇملار; x++, ed++) {
		إذا (ed->func == func && ed->data == data)
			أرجع EXT2_ET_FILE_EXISTS;
		إذا (ed->func == عدم)
			free_ed = ed;
	}

	إذا (free_ed) {
		free_ed->func = func;
		free_ed->data = data;
		أرجع 0;
	}

	إذا (nr_بۇيۇملار == 0) {
		ret = atexit(handle_exit);
		إذا (ret)
			أرجع ret;
	}

	ret = ext2fs_resize_mem(0, (nr_بۇيۇملار + 1) * حجم(بنية exit_data),
				&بۇيۇملار);
	إذا (ret)
		أرجع ret;

	بۇيۇملار[nr_بۇيۇملار].func = func;
	بۇيۇملار[nr_بۇيۇملار].data = data;
	nr_بۇيۇملار++;

	أرجع 0;
}

/* Remove a function from the exit cleanup list. */
errcode_t ext2fs_remove_exit_fn(ext2_exit_fn func, فراغ *data)
{
	بنية exit_data *ed;
	size_t x;

	إذا (func == عدم)
		أرجع EXT2_ET_INVALID_ARGUMENT;

	لكل (x = 0, ed = بۇيۇملار; x < nr_بۇيۇملار; x++, ed++) {
		إذا (ed->func == عدم)
			أرجع 0;
		إذا (ed->func == func && ed->data == data) {
			size_t sz = (nr_بۇيۇملار - (x + 1)) *
				    حجم(بنية exit_data);
			memmove(ed, ed + 1, sz);
			memset(بۇيۇملار + nr_بۇيۇملار - 1, 0,
			       حجم(بنية exit_data));
		}
	}

	أرجع 0;
}
