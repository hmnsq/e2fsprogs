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

#அந்தர்பாவ "config.h"
#அந்தர்பாவ <stdlib.h>

#அந்தர்பாவ "ext2_fs.h"
#அந்தர்பாவ "ext2fs.h"
#அந்தர்பாவ "ext2fsP.h"

கட்டமைப்பு exit_data {
	ext2_exit_fn func;
	வெறுமை *data;
};

நிலையான கட்டமைப்பு exit_data *பொருட்கள்;
நிலையான size_t nr_பொருட்கள்;

நிலையான வெறுமை handle_exit(வெறுமை)
{
	கட்டமைப்பு exit_data *ed;

	க்கு (ed = பொருட்கள் + nr_பொருட்கள் - 1; ed >= பொருட்கள்; ed--) {
		எனில் (ed->func == வெற்று)
			தொடர்;
		ed->func(ed->data);
	}

	ext2fs_free_mem(&பொருட்கள்);
	nr_பொருட்கள் = 0;
}

/*
 * Schedule a function to be called at (normal) program termination.
 * If you want this to be called during a signal exit, you must capture
 * the signal and call exit() yourself!
 */
errcode_t ext2fs_add_exit_fn(ext2_exit_fn func, வெறுமை *data)
{
	கட்டமைப்பு exit_data *ed, *free_ed = வெற்று;
	size_t x;
	errcode_t ret;

	எனில் (func == வெற்று)
		திரும்பு EXT2_ET_INVALID_ARGUMENT;

	க்கு (x = 0, ed = பொருட்கள்; x < nr_பொருட்கள்; x++, ed++) {
		எனில் (ed->func == func && ed->data == data)
			திரும்பு EXT2_ET_FILE_EXISTS;
		எனில் (ed->func == வெற்று)
			free_ed = ed;
	}

	எனில் (free_ed) {
		free_ed->func = func;
		free_ed->data = data;
		திரும்பு 0;
	}

	எனில் (nr_பொருட்கள் == 0) {
		ret = atexit(handle_exit);
		எனில் (ret)
			திரும்பு ret;
	}

	ret = ext2fs_resize_mem(0, (nr_பொருட்கள் + 1) * அளவு(கட்டமைப்பு exit_data),
				&பொருட்கள்);
	எனில் (ret)
		திரும்பு ret;

	பொருட்கள்[nr_பொருட்கள்].func = func;
	பொருட்கள்[nr_பொருட்கள்].data = data;
	nr_பொருட்கள்++;

	திரும்பு 0;
}

/* Remove a function from the exit cleanup list. */
errcode_t ext2fs_remove_exit_fn(ext2_exit_fn func, வெறுமை *data)
{
	கட்டமைப்பு exit_data *ed;
	size_t x;

	எனில் (func == வெற்று)
		திரும்பு EXT2_ET_INVALID_ARGUMENT;

	க்கு (x = 0, ed = பொருட்கள்; x < nr_பொருட்கள்; x++, ed++) {
		எனில் (ed->func == வெற்று)
			திரும்பு 0;
		எனில் (ed->func == func && ed->data == data) {
			size_t sz = (nr_பொருட்கள் - (x + 1)) *
				    அளவு(கட்டமைப்பு exit_data);
			memmove(ed, ed + 1, sz);
			memset(பொருட்கள் + nr_பொருட்கள் - 1, 0,
			       அளவு(கட்டமைப்பு exit_data));
		}
	}

	திரும்பு 0;
}
