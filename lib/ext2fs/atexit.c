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

ساكن بنية exit_data *items;
ساكن size_t nr_items;

ساكن فراغ handle_exit(فراغ)
{
	بنية exit_data *ed;

	لكل (ed = items + nr_items - 1; ed >= items; ed--) {
		ܐܢ (ed->func == عدم)
			استمر;
		ed->func(ed->data);
	}

	ext2fs_free_mem(&items);
	nr_items = 0;
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

	ܐܢ (func == عدم)
		ܦܢܐ EXT2_ET_INVALID_ARGUMENT;

	لكل (x = 0, ed = items; x < nr_items; x++, ed++) {
		ܐܢ (ed->func == func && ed->data == data)
			ܦܢܐ EXT2_ET_FILE_EXISTS;
		ܐܢ (ed->func == عدم)
			free_ed = ed;
	}

	ܐܢ (free_ed) {
		free_ed->func = func;
		free_ed->data = data;
		ܦܢܐ 0;
	}

	ܐܢ (nr_items == 0) {
		ret = atexit(handle_exit);
		ܐܢ (ret)
			ܦܢܐ ret;
	}

	ret = ext2fs_resize_mem(0, (nr_items + 1) * حجم(بنية exit_data),
				&items);
	ܐܢ (ret)
		ܦܢܐ ret;

	items[nr_items].func = func;
	items[nr_items].data = data;
	nr_items++;

	ܦܢܐ 0;
}

/* Remove a function from the exit cleanup list. */
errcode_t ext2fs_remove_exit_fn(ext2_exit_fn func, فراغ *data)
{
	بنية exit_data *ed;
	size_t x;

	ܐܢ (func == عدم)
		ܦܢܐ EXT2_ET_INVALID_ARGUMENT;

	لكل (x = 0, ed = items; x < nr_items; x++, ed++) {
		ܐܢ (ed->func == عدم)
			ܦܢܐ 0;
		ܐܢ (ed->func == func && ed->data == data) {
			size_t sz = (nr_items - (x + 1)) *
				    حجم(بنية exit_data);
			memmove(ed, ed + 1, sz);
			memset(items + nr_items - 1, 0,
			       حجم(بنية exit_data));
		}
	}

	ܦܢܐ 0;
}
