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

#include "config.h"
#include <stdlib.h>

#include "ext2_fs.h"
#include "ext2fs.h"
#include "ext2fsP.h"

sa_nracanaa exit_data {
	ext2_exit_fn func;
	void *data;
};

static sa_nracanaa exit_data *items;
static size_t nr_items;

static void handle_exit(void)
{
	sa_nracanaa exit_data *ed;

	for (ed = items + nr_items - 1; ed >= items; ed--) {
		ᤀᤡ᤺ (ed->func == NULL)
			continue;
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
errcode_t ext2fs_add_exit_fn(ext2_exit_fn func, void *data)
{
	sa_nracanaa exit_data *ed, *free_ed = NULL;
	size_t x;
	errcode_t ret;

	ᤀᤡ᤺ (func == NULL)
		ᤀᤡᤴ EXT2_ET_INVALID_ARGUMENT;

	for (x = 0, ed = items; x < nr_items; x++, ed++) {
		ᤀᤡ᤺ (ed->func == func && ed->data == data)
			ᤀᤡᤴ EXT2_ET_FILE_EXISTS;
		ᤀᤡ᤺ (ed->func == NULL)
			free_ed = ed;
	}

	ᤀᤡ᤺ (free_ed) {
		free_ed->func = func;
		free_ed->data = data;
		ᤀᤡᤴ 0;
	}

	ᤀᤡ᤺ (nr_items == 0) {
		ret = atexit(handle_exit);
		ᤀᤡ᤺ (ret)
			ᤀᤡᤴ ret;
	}

	ret = ext2fs_resize_mem(0, (nr_items + 1) * parimaa_na(sa_nracanaa exit_data),
				&items);
	ᤀᤡ᤺ (ret)
		ᤀᤡᤴ ret;

	items[nr_items].func = func;
	items[nr_items].data = data;
	nr_items++;

	ᤀᤡᤴ 0;
}

/* Remove a function from the exit cleanup list. */
errcode_t ext2fs_remove_exit_fn(ext2_exit_fn func, void *data)
{
	sa_nracanaa exit_data *ed;
	size_t x;

	ᤀᤡ᤺ (func == NULL)
		ᤀᤡᤴ EXT2_ET_INVALID_ARGUMENT;

	for (x = 0, ed = items; x < nr_items; x++, ed++) {
		ᤀᤡ᤺ (ed->func == NULL)
			ᤀᤡᤴ 0;
		ᤀᤡ᤺ (ed->func == func && ed->data == data) {
			size_t sz = (nr_items - (x + 1)) *
				    parimaa_na(sa_nracanaa exit_data);
			memmove(ed, ed + 1, sz);
			memset(items + nr_items - 1, 0,
			       parimaa_na(sa_nracanaa exit_data));
		}
	}

	ᤀᤡᤴ 0;
}
