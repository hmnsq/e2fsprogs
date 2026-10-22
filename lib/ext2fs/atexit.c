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

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ <stdlib.h>

#ଅନ୍ତର୍ଭାଵ "ext2_fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fsP.h"

ସଂରଚନା exit_data {
	ext2_exit_fn func;
	ଶୂନ୍ୟ *data;
};

ସ୍ଥିର ସଂରଚନା exit_data *ବସ୍ତୁଗୁଡ଼ିକ;
ସ୍ଥିର size_t nr_ବସ୍ତୁଗୁଡ଼ିକ;

ସ୍ଥିର ଶୂନ୍ୟ handle_exit(ଶୂନ୍ୟ)
{
	ସଂରଚନା exit_data *ed;

	ପାଇଁ (ed = ବସ୍ତୁଗୁଡ଼ିକ + nr_ବସ୍ତୁଗୁଡ଼ିକ - 1; ed >= ବସ୍ତୁଗୁଡ଼ିକ; ed--) {
		ଯଦି (ed->func == ଖାଲି)
			ଜାରି;
		ed->func(ed->data);
	}

	ext2fs_free_mem(&ବସ୍ତୁଗୁଡ଼ିକ);
	nr_ବସ୍ତୁଗୁଡ଼ିକ = 0;
}

/*
 * Schedule a function to be called at (normal) program termination.
 * If you want this to be called during a signal exit, you must capture
 * the signal and call exit() yourself!
 */
errcode_t ext2fs_add_exit_fn(ext2_exit_fn func, ଶୂନ୍ୟ *data)
{
	ସଂରଚନା exit_data *ed, *free_ed = ଖାଲି;
	size_t x;
	errcode_t ret;

	ଯଦି (func == ଖାଲି)
		ଫେରନ୍ତୁ EXT2_ET_INVALID_ARGUMENT;

	ପାଇଁ (x = 0, ed = ବସ୍ତୁଗୁଡ଼ିକ; x < nr_ବସ୍ତୁଗୁଡ଼ିକ; x++, ed++) {
		ଯଦି (ed->func == func && ed->data == data)
			ଫେରନ୍ତୁ EXT2_ET_FILE_EXISTS;
		ଯଦି (ed->func == ଖାଲି)
			free_ed = ed;
	}

	ଯଦି (free_ed) {
		free_ed->func = func;
		free_ed->data = data;
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି (nr_ବସ୍ତୁଗୁଡ଼ିକ == 0) {
		ret = atexit(handle_exit);
		ଯଦି (ret)
			ଫେରନ୍ତୁ ret;
	}

	ret = ext2fs_resize_mem(0, (nr_ବସ୍ତୁଗୁଡ଼ିକ + 1) * ଆକାର(ସଂରଚନା exit_data),
				&ବସ୍ତୁଗୁଡ଼ିକ);
	ଯଦି (ret)
		ଫେରନ୍ତୁ ret;

	ବସ୍ତୁଗୁଡ଼ିକ[nr_ବସ୍ତୁଗୁଡ଼ିକ].func = func;
	ବସ୍ତୁଗୁଡ଼ିକ[nr_ବସ୍ତୁଗୁଡ଼ିକ].data = data;
	nr_ବସ୍ତୁଗୁଡ଼ିକ++;

	ଫେରନ୍ତୁ 0;
}

/* Remove a function from the exit cleanup list. */
errcode_t ext2fs_remove_exit_fn(ext2_exit_fn func, ଶୂନ୍ୟ *data)
{
	ସଂରଚନା exit_data *ed;
	size_t x;

	ଯଦି (func == ଖାଲି)
		ଫେରନ୍ତୁ EXT2_ET_INVALID_ARGUMENT;

	ପାଇଁ (x = 0, ed = ବସ୍ତୁଗୁଡ଼ିକ; x < nr_ବସ୍ତୁଗୁଡ଼ିକ; x++, ed++) {
		ଯଦି (ed->func == ଖାଲି)
			ଫେରନ୍ତୁ 0;
		ଯଦି (ed->func == func && ed->data == data) {
			size_t sz = (nr_ବସ୍ତୁଗୁଡ଼ିକ - (x + 1)) *
				    ଆକାର(ସଂରଚନା exit_data);
			memmove(ed, ed + 1, sz);
			memset(ବସ୍ତୁଗୁଡ଼ିକ + nr_ବସ୍ତୁଗୁଡ଼ିକ - 1, 0,
			       ଆକାର(ସଂରଚନା exit_data));
		}
	}

	ଫେରନ୍ତୁ 0;
}
