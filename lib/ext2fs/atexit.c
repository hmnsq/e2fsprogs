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

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdlib.h>

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"
#अन्तर्भाव "ext2fsP.h"

संरचना exit_data {
	ext2_exit_fn func;
	शून्य *data;
};

स्थिर संरचना exit_data *वस्तूनि;
स्थिर size_t nr_वस्तूनि;

स्थिर शून्य handle_exit(शून्य)
{
	संरचना exit_data *ed;

	प्रति (ed = वस्तूनि + nr_वस्तूनि - 1; ed >= वस्तूनि; ed--) {
		जइ (ed->func == रिक्त)
			अनुवर्त;
		ed->func(ed->data);
	}

	ext2fs_free_mem(&वस्तूनि);
	nr_वस्तूनि = 0;
}

/*
 * Schedule a function to be called at (normal) program termination.
 * If you want this to be called during a signal exit, you must capture
 * the signal and call exit() yourself!
 */
errcode_t ext2fs_add_exit_fn(ext2_exit_fn func, शून्य *data)
{
	संरचना exit_data *ed, *free_ed = रिक्त;
	size_t x;
	errcode_t ret;

	जइ (func == रिक्त)
		प्रत्यागम EXT2_ET_INVALID_ARGUMENT;

	प्रति (x = 0, ed = वस्तूनि; x < nr_वस्तूनि; x++, ed++) {
		जइ (ed->func == func && ed->data == data)
			प्रत्यागम EXT2_ET_FILE_EXISTS;
		जइ (ed->func == रिक्त)
			free_ed = ed;
	}

	जइ (free_ed) {
		free_ed->func = func;
		free_ed->data = data;
		प्रत्यागम 0;
	}

	जइ (nr_वस्तूनि == 0) {
		ret = atexit(handle_exit);
		जइ (ret)
			प्रत्यागम ret;
	}

	ret = ext2fs_resize_mem(0, (nr_वस्तूनि + 1) * परिमाण(संरचना exit_data),
				&वस्तूनि);
	जइ (ret)
		प्रत्यागम ret;

	वस्तूनि[nr_वस्तूनि].func = func;
	वस्तूनि[nr_वस्तूनि].data = data;
	nr_वस्तूनि++;

	प्रत्यागम 0;
}

/* Remove a function from the exit cleanup list. */
errcode_t ext2fs_remove_exit_fn(ext2_exit_fn func, शून्य *data)
{
	संरचना exit_data *ed;
	size_t x;

	जइ (func == रिक्त)
		प्रत्यागम EXT2_ET_INVALID_ARGUMENT;

	प्रति (x = 0, ed = वस्तूनि; x < nr_वस्तूनि; x++, ed++) {
		जइ (ed->func == रिक्त)
			प्रत्यागम 0;
		जइ (ed->func == func && ed->data == data) {
			size_t sz = (nr_वस्तूनि - (x + 1)) *
				    परिमाण(संरचना exit_data);
			memmove(ed, ed + 1, sz);
			memset(वस्तूनि + nr_वस्तूनि - 1, 0,
			       परिमाण(संरचना exit_data));
		}
	}

	प्रत्यागम 0;
}
