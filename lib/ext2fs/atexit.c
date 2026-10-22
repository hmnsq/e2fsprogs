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

#شامل "config.h"
#شامل <stdlib.h>

#شامل "ext2_fs.h"
#شامل "ext2fs.h"
#شامل "ext2fsP.h"

ساختار exit_data {
	ext2_exit_fn func;
	تهی *data;
};

ایستا ساختار exit_data *اقلام;
ایستا size_t nr_اقلام;

ایستا تهی handle_exit(تهی)
{
	ساختار exit_data *ed;

	برای (ed = اقلام + nr_اقلام - 1; ed >= اقلام; ed--) {
		اگر (ed->func == پوچ)
			ادامه;
		ed->func(ed->data);
	}

	ext2fs_free_mem(&اقلام);
	nr_اقلام = 0;
}

/*
 * Schedule a function to be called at (normal) program termination.
 * If you want this to be called during a signal exit, you must capture
 * the signal and call exit() yourself!
 */
errcode_t ext2fs_add_exit_fn(ext2_exit_fn func, تهی *data)
{
	ساختار exit_data *ed, *free_ed = پوچ;
	size_t x;
	errcode_t ret;

	اگر (func == پوچ)
		بازگشت EXT2_ET_INVALID_ARGUMENT;

	برای (x = 0, ed = اقلام; x < nr_اقلام; x++, ed++) {
		اگر (ed->func == func && ed->data == data)
			بازگشت EXT2_ET_FILE_EXISTS;
		اگر (ed->func == پوچ)
			free_ed = ed;
	}

	اگر (free_ed) {
		free_ed->func = func;
		free_ed->data = data;
		بازگشت 0;
	}

	اگر (nr_اقلام == 0) {
		ret = atexit(handle_exit);
		اگر (ret)
			بازگشت ret;
	}

	ret = ext2fs_resize_mem(0, (nr_اقلام + 1) * اندازه(ساختار exit_data),
				&اقلام);
	اگر (ret)
		بازگشت ret;

	اقلام[nr_اقلام].func = func;
	اقلام[nr_اقلام].data = data;
	nr_اقلام++;

	بازگشت 0;
}

/* Remove a function from the exit cleanup list. */
errcode_t ext2fs_remove_exit_fn(ext2_exit_fn func, تهی *data)
{
	ساختار exit_data *ed;
	size_t x;

	اگر (func == پوچ)
		بازگشت EXT2_ET_INVALID_ARGUMENT;

	برای (x = 0, ed = اقلام; x < nr_اقلام; x++, ed++) {
		اگر (ed->func == پوچ)
			بازگشت 0;
		اگر (ed->func == func && ed->data == data) {
			size_t sz = (nr_اقلام - (x + 1)) *
				    اندازه(ساختار exit_data);
			memmove(ed, ed + 1, sz);
			memset(اقلام + nr_اقلام - 1, 0,
			       اندازه(ساختار exit_data));
		}
	}

	بازگشت 0;
}
