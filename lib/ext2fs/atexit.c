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

#অন্তর্ভাব "config.h"
#অন্তর্ভাব <stdlib.h>

#অন্তর্ভাব "ext2_fs.h"
#অন্তর্ভাব "ext2fs.h"
#অন্তর্ভাব "ext2fsP.h"

কাঠামো exit_তথ্য {
	ext2_exit_fn func;
	শূন্য *তথ্য;
};

স্থির কাঠামো exit_তথ্য *বস্তুসমূহ;
স্থির size_t nr_বস্তুসমূহ;

স্থির শূন্য handle_exit(শূন্য)
{
	কাঠামো exit_তথ্য *ed;

	জন্য (ed = বস্তুসমূহ + nr_বস্তুসমূহ - 1; ed >= বস্তুসমূহ; ed--) {
		যদি (ed->func == ফাঁকা)
			চালু;
		ed->func(ed->তথ্য);
	}

	ext2fs_free_mem(&বস্তুসমূহ);
	nr_বস্তুসমূহ = 0;
}

/*
 * Schedule a function to be called at (normal) program termination.
 * If you want this to be called during a signal exit, you must capture
 * the signal and call exit() yourself!
 */
errcode_t ext2fs_add_exit_fn(ext2_exit_fn func, শূন্য *তথ্য)
{
	কাঠামো exit_তথ্য *ed, *free_ed = ফাঁকা;
	size_t x;
	errcode_t ret;

	যদি (func == ফাঁকা)
		ফেরত EXT2_ET_INVALID_ARGUMENT;

	জন্য (x = 0, ed = বস্তুসমূহ; x < nr_বস্তুসমূহ; x++, ed++) {
		যদি (ed->func == func && ed->তথ্য == তথ্য)
			ফেরত EXT2_ET_FILE_EXISTS;
		যদি (ed->func == ফাঁকা)
			free_ed = ed;
	}

	যদি (free_ed) {
		free_ed->func = func;
		free_ed->তথ্য = তথ্য;
		ফেরত 0;
	}

	যদি (nr_বস্তুসমূহ == 0) {
		ret = atexit(handle_exit);
		যদি (ret)
			ফেরত ret;
	}

	ret = ext2fs_resize_mem(0, (nr_বস্তুসমূহ + 1) * আকার(কাঠামো exit_তথ্য),
				&বস্তুসমূহ);
	যদি (ret)
		ফেরত ret;

	বস্তুসমূহ[nr_বস্তুসমূহ].func = func;
	বস্তুসমূহ[nr_বস্তুসমূহ].তথ্য = তথ্য;
	nr_বস্তুসমূহ++;

	ফেরত 0;
}

/* Remove a function from the exit cleanup list. */
errcode_t ext2fs_remove_exit_fn(ext2_exit_fn func, শূন্য *তথ্য)
{
	কাঠামো exit_তথ্য *ed;
	size_t x;

	যদি (func == ফাঁকা)
		ফেরত EXT2_ET_INVALID_ARGUMENT;

	জন্য (x = 0, ed = বস্তুসমূহ; x < nr_বস্তুসমূহ; x++, ed++) {
		যদি (ed->func == ফাঁকা)
			ফেরত 0;
		যদি (ed->func == func && ed->তথ্য == তথ্য) {
			size_t sz = (nr_বস্তুসমূহ - (x + 1)) *
				    আকার(কাঠামো exit_তথ্য);
			memmove(ed, ed + 1, sz);
			memset(বস্তুসমূহ + nr_বস্তুসমূহ - 1, 0,
			       আকার(কাঠামো exit_তথ্য));
		}
	}

	ফেরত 0;
}
