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

#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ <stdlib.h>

#અન્તર્ભાવ "ext2_fs.h"
#અન્તર્ભાવ "ext2fs.h"
#અન્તર્ભાવ "ext2fsP.h"

સંરચના exit_data {
	ext2_exit_fn func;
	શૂન્ય *data;
};

સ્થિર સંરચના exit_data *વસ્તુઓ;
સ્થિર size_t nr_વસ્તુઓ;

સ્થિર શૂન્ય handle_exit(શૂન્ય)
{
	સંરચના exit_data *ed;

	માટે (ed = વસ્તુઓ + nr_વસ્તુઓ - 1; ed >= વસ્તુઓ; ed--) {
		જો (ed->func == ખાલી)
			ચાલુ;
		ed->func(ed->data);
	}

	ext2fs_free_mem(&વસ્તુઓ);
	nr_વસ્તુઓ = 0;
}

/*
 * Schedule a function to be called at (normal) program termination.
 * If you want this to be called during a signal exit, you must capture
 * the signal and call exit() yourself!
 */
errcode_t ext2fs_add_exit_fn(ext2_exit_fn func, શૂન્ય *data)
{
	સંરચના exit_data *ed, *free_ed = ખાલી;
	size_t x;
	errcode_t ret;

	જો (func == ખાલી)
		પાછા EXT2_ET_INVALID_ARGUMENT;

	માટે (x = 0, ed = વસ્તુઓ; x < nr_વસ્તુઓ; x++, ed++) {
		જો (ed->func == func && ed->data == data)
			પાછા EXT2_ET_FILE_EXISTS;
		જો (ed->func == ખાલી)
			free_ed = ed;
	}

	જો (free_ed) {
		free_ed->func = func;
		free_ed->data = data;
		પાછા 0;
	}

	જો (nr_વસ્તુઓ == 0) {
		ret = atexit(handle_exit);
		જો (ret)
			પાછા ret;
	}

	ret = ext2fs_resize_mem(0, (nr_વસ્તુઓ + 1) * કદ(સંરચના exit_data),
				&વસ્તુઓ);
	જો (ret)
		પાછા ret;

	વસ્તુઓ[nr_વસ્તુઓ].func = func;
	વસ્તુઓ[nr_વસ્તુઓ].data = data;
	nr_વસ્તુઓ++;

	પાછા 0;
}

/* Remove a function from the exit cleanup list. */
errcode_t ext2fs_remove_exit_fn(ext2_exit_fn func, શૂન્ય *data)
{
	સંરચના exit_data *ed;
	size_t x;

	જો (func == ખાલી)
		પાછા EXT2_ET_INVALID_ARGUMENT;

	માટે (x = 0, ed = વસ્તુઓ; x < nr_વસ્તુઓ; x++, ed++) {
		જો (ed->func == ખાલી)
			પાછા 0;
		જો (ed->func == func && ed->data == data) {
			size_t sz = (nr_વસ્તુઓ - (x + 1)) *
				    કદ(સંરચના exit_data);
			memmove(ed, ed + 1, sz);
			memset(વસ્તુઓ + nr_વસ્તુઓ - 1, 0,
			       કદ(સંરચના exit_data));
		}
	}

	પાછા 0;
}
