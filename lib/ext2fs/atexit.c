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

#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ <stdlib.h>

#অন্তর্ভাৱ "ext2_fs.h"
#অন্তর্ভাৱ "ext2fs.h"
#অন্তর্ভাৱ "ext2fsP.h"

সংরচনা exit_data {
	ext2_exit_fn func;
	শূন্য *data;
};

স্থির সংরচনা exit_data *বস্তুসমূহ;
স্থির size_t nr_বস্তুসমূহ;

স্থির শূন্য handle_exit(শূন্য)
{
	সংরচনা exit_data *ed;

	বাবে (ed = বস্তুসমূহ + nr_বস্তুসমূহ - 1; ed >= বস্তুসমূহ; ed--) {
		যদি (ed->func == রিক্ত)
			অনুৱর্ত;
		ed->func(ed->data);
	}

	ext2fs_free_mem(&বস্তুসমূহ);
	nr_বস্তুসমূহ = 0;
}

/*
 * Schedule a function to be called at (normal) program termination.
 * If you want this to be called during a signal exit, you must capture
 * the signal and call exit() yourself!
 */
errcode_t ext2fs_add_exit_fn(ext2_exit_fn func, শূন্য *data)
{
	সংরচনা exit_data *ed, *free_ed = রিক্ত;
	size_t x;
	errcode_t ret;

	যদি (func == রিক্ত)
		ঘূৰাই EXT2_ET_INVALID_ARGUMENT;

	বাবে (x = 0, ed = বস্তুসমূহ; x < nr_বস্তুসমূহ; x++, ed++) {
		যদি (ed->func == func && ed->data == data)
			ঘূৰাই EXT2_ET_FILE_EXISTS;
		যদি (ed->func == রিক্ত)
			free_ed = ed;
	}

	যদি (free_ed) {
		free_ed->func = func;
		free_ed->data = data;
		ঘূৰাই 0;
	}

	যদি (nr_বস্তুসমূহ == 0) {
		ret = atexit(handle_exit);
		যদি (ret)
			ঘূৰাই ret;
	}

	ret = ext2fs_resize_mem(0, (nr_বস্তুসমূহ + 1) * পরিমাণ(সংরচনা exit_data),
				&বস্তুসমূহ);
	যদি (ret)
		ঘূৰাই ret;

	বস্তুসমূহ[nr_বস্তুসমূহ].func = func;
	বস্তুসমূহ[nr_বস্তুসমূহ].data = data;
	nr_বস্তুসমূহ++;

	ঘূৰাই 0;
}

/* Remove a function from the exit cleanup list. */
errcode_t ext2fs_remove_exit_fn(ext2_exit_fn func, শূন্য *data)
{
	সংরচনা exit_data *ed;
	size_t x;

	যদি (func == রিক্ত)
		ঘূৰাই EXT2_ET_INVALID_ARGUMENT;

	বাবে (x = 0, ed = বস্তুসমূহ; x < nr_বস্তুসমূহ; x++, ed++) {
		যদি (ed->func == রিক্ত)
			ঘূৰাই 0;
		যদি (ed->func == func && ed->data == data) {
			size_t sz = (nr_বস্তুসমূহ - (x + 1)) *
				    পরিমাণ(সংরচনা exit_data);
			memmove(ed, ed + 1, sz);
			memset(বস্তুসমূহ + nr_বস্তুসমূহ - 1, 0,
			       পরিমাণ(সংরচনা exit_data));
		}
	}

	ঘূৰাই 0;
}
