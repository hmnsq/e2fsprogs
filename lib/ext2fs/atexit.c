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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdlib.h>

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fsP.h"

ಸಂರಚನಾ exit_data {
	ext2_exit_fn func;
	ಶೂನ್ಯ *data;
};

ಸ್ಥಿರ ಸಂರಚನಾ exit_data *ವಸ್ತುಗಳು;
ಸ್ಥಿರ size_t nr_ವಸ್ತುಗಳು;

ಸ್ಥಿರ ಶೂನ್ಯ handle_exit(ಶೂನ್ಯ)
{
	ಸಂರಚನಾ exit_data *ed;

	ಪ್ರತಿ (ed = ವಸ್ತುಗಳು + nr_ವಸ್ತುಗಳು - 1; ed >= ವಸ್ತುಗಳು; ed--) {
		ಯದಿ (ed->func == ರಿಕ್ತ)
			ಅನುವರ್ತ;
		ed->func(ed->data);
	}

	ext2fs_free_mem(&ವಸ್ತುಗಳು);
	nr_ವಸ್ತುಗಳು = 0;
}

/*
 * Schedule a function to be called at (normal) program termination.
 * If you want this to be called during a signal exit, you must capture
 * the signal and call exit() yourself!
 */
errcode_t ext2fs_add_exit_fn(ext2_exit_fn func, ಶೂನ್ಯ *data)
{
	ಸಂರಚನಾ exit_data *ed, *free_ed = ರಿಕ್ತ;
	size_t x;
	errcode_t ret;

	ಯದಿ (func == ರಿಕ್ತ)
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_INVALID_ARGUMENT;

	ಪ್ರತಿ (x = 0, ed = ವಸ್ತುಗಳು; x < nr_ವಸ್ತುಗಳು; x++, ed++) {
		ಯದಿ (ed->func == func && ed->data == data)
			ಪ್ರತ್ಯಾಗಮ EXT2_ET_FILE_EXISTS;
		ಯದಿ (ed->func == ರಿಕ್ತ)
			free_ed = ed;
	}

	ಯದಿ (free_ed) {
		free_ed->func = func;
		free_ed->data = data;
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ (nr_ವಸ್ತುಗಳು == 0) {
		ret = atexit(handle_exit);
		ಯದಿ (ret)
			ಪ್ರತ್ಯಾಗಮ ret;
	}

	ret = ext2fs_resize_mem(0, (nr_ವಸ್ತುಗಳು + 1) * ಪರಿಮಾಣ(ಸಂರಚನಾ exit_data),
				&ವಸ್ತುಗಳು);
	ಯದಿ (ret)
		ಪ್ರತ್ಯಾಗಮ ret;

	ವಸ್ತುಗಳು[nr_ವಸ್ತುಗಳು].func = func;
	ವಸ್ತುಗಳು[nr_ವಸ್ತುಗಳು].data = data;
	nr_ವಸ್ತುಗಳು++;

	ಪ್ರತ್ಯಾಗಮ 0;
}

/* Remove a function from the exit cleanup list. */
errcode_t ext2fs_remove_exit_fn(ext2_exit_fn func, ಶೂನ್ಯ *data)
{
	ಸಂರಚನಾ exit_data *ed;
	size_t x;

	ಯದಿ (func == ರಿಕ್ತ)
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_INVALID_ARGUMENT;

	ಪ್ರತಿ (x = 0, ed = ವಸ್ತುಗಳು; x < nr_ವಸ್ತುಗಳು; x++, ed++) {
		ಯದಿ (ed->func == ರಿಕ್ತ)
			ಪ್ರತ್ಯಾಗಮ 0;
		ಯದಿ (ed->func == func && ed->data == data) {
			size_t sz = (nr_ವಸ್ತುಗಳು - (x + 1)) *
				    ಪರಿಮಾಣ(ಸಂರಚನಾ exit_data);
			memmove(ed, ed + 1, sz);
			memset(ವಸ್ತುಗಳು + nr_ವಸ್ತುಗಳು - 1, 0,
			       ಪರಿಮಾಣ(ಸಂರಚನಾ exit_data));
		}
	}

	ಪ್ರತ್ಯಾಗಮ 0;
}
