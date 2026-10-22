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

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ <stdlib.h>

#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ "ext2fs.h"
#അന്തര്ഭാവ "ext2fsP.h"

ഘടന exit_data {
	ext2_exit_fn func;
	ശൂന്യം *data;
};

സ്ഥിതം ഘടന exit_data *ഇനങ്ങൾ;
സ്ഥിതം size_t nr_ഇനങ്ങൾ;

സ്ഥിതം ശൂന്യം handle_exit(ശൂന്യം)
{
	ഘടന exit_data *ed;

	വേണ്ടി (ed = ഇനങ്ങൾ + nr_ഇനങ്ങൾ - 1; ed >= ഇനങ്ങൾ; ed--) {
		എങ്കിൽ (ed->func == ശൂന്യ)
			തുടരുക;
		ed->func(ed->data);
	}

	ext2fs_free_mem(&ഇനങ്ങൾ);
	nr_ഇനങ്ങൾ = 0;
}

/*
 * Schedule a function to be called at (normal) program termination.
 * If you want this to be called during a signal exit, you must capture
 * the signal and call exit() yourself!
 */
errcode_t ext2fs_add_exit_fn(ext2_exit_fn func, ശൂന്യം *data)
{
	ഘടന exit_data *ed, *free_ed = ശൂന്യ;
	size_t x;
	errcode_t ret;

	എങ്കിൽ (func == ശൂന്യ)
		തിരികെ EXT2_ET_INVALID_ARGUMENT;

	വേണ്ടി (x = 0, ed = ഇനങ്ങൾ; x < nr_ഇനങ്ങൾ; x++, ed++) {
		എങ്കിൽ (ed->func == func && ed->data == data)
			തിരികെ EXT2_ET_FILE_EXISTS;
		എങ്കിൽ (ed->func == ശൂന്യ)
			free_ed = ed;
	}

	എങ്കിൽ (free_ed) {
		free_ed->func = func;
		free_ed->data = data;
		തിരികെ 0;
	}

	എങ്കിൽ (nr_ഇനങ്ങൾ == 0) {
		ret = atexit(handle_exit);
		എങ്കിൽ (ret)
			തിരികെ ret;
	}

	ret = ext2fs_resize_mem(0, (nr_ഇനങ്ങൾ + 1) * വലുപ്പം(ഘടന exit_data),
				&ഇനങ്ങൾ);
	എങ്കിൽ (ret)
		തിരികെ ret;

	ഇനങ്ങൾ[nr_ഇനങ്ങൾ].func = func;
	ഇനങ്ങൾ[nr_ഇനങ്ങൾ].data = data;
	nr_ഇനങ്ങൾ++;

	തിരികെ 0;
}

/* Remove a function from the exit cleanup list. */
errcode_t ext2fs_remove_exit_fn(ext2_exit_fn func, ശൂന്യം *data)
{
	ഘടന exit_data *ed;
	size_t x;

	എങ്കിൽ (func == ശൂന്യ)
		തിരികെ EXT2_ET_INVALID_ARGUMENT;

	വേണ്ടി (x = 0, ed = ഇനങ്ങൾ; x < nr_ഇനങ്ങൾ; x++, ed++) {
		എങ്കിൽ (ed->func == ശൂന്യ)
			തിരികെ 0;
		എങ്കിൽ (ed->func == func && ed->data == data) {
			size_t sz = (nr_ഇനങ്ങൾ - (x + 1)) *
				    വലുപ്പം(ഘടന exit_data);
			memmove(ed, ed + 1, sz);
			memset(ഇനങ്ങൾ + nr_ഇനങ്ങൾ - 1, 0,
			       വലുപ്പം(ഘടന exit_data));
		}
	}

	തിരികെ 0;
}
