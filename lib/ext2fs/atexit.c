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

स्थिर संरचना exit_data *वस्तुहरू;
स्थिर माप_प्रकार nr_वस्तुहरू;

स्थिर शून्य handle_exit(शून्य)
{
	संरचना exit_data *ed;

	लागि (ed = वस्तुहरू + nr_वस्तुहरू - 1; ed >= वस्तुहरू; ed--) {
		यदि (ed->func == रिक्त)
			जारी;
		ed->func(ed->data);
	}

	ext2fs_free_mem(&वस्तुहरू);
	nr_वस्तुहरू = 0;
}

/*
 * Schedule a function to be called at (normal) program termination.
 * If you want this to be called during a signal exit, you must capture
 * the signal and call exit() yourself!
 */
errcode_t ext2fs_add_exit_fn(ext2_exit_fn func, शून्य *data)
{
	संरचना exit_data *ed, *free_ed = रिक्त;
	माप_प्रकार x;
	errcode_t ret;

	यदि (func == रिक्त)
		फर्काऊ EXT2_ET_INVALID_ARGUMENT;

	लागि (x = 0, ed = वस्तुहरू; x < nr_वस्तुहरू; x++, ed++) {
		यदि (ed->func == func && ed->data == data)
			फर्काऊ EXT2_ET_FILE_EXISTS;
		यदि (ed->func == रिक्त)
			free_ed = ed;
	}

	यदि (free_ed) {
		free_ed->func = func;
		free_ed->data = data;
		फर्काऊ 0;
	}

	यदि (nr_वस्तुहरू == 0) {
		ret = निकास_पर(handle_exit);
		यदि (ret)
			फर्काऊ ret;
	}

	ret = ext2fs_resize_mem(0, (nr_वस्तुहरू + 1) * परिमाण(संरचना exit_data),
				&वस्तुहरू);
	यदि (ret)
		फर्काऊ ret;

	वस्तुहरू[nr_वस्तुहरू].func = func;
	वस्तुहरू[nr_वस्तुहरू].data = data;
	nr_वस्तुहरू++;

	फर्काऊ 0;
}

/* Remove a function from the exit cleanup list. */
errcode_t ext2fs_remove_exit_fn(ext2_exit_fn func, शून्य *data)
{
	संरचना exit_data *ed;
	माप_प्रकार x;

	यदि (func == रिक्त)
		फर्काऊ EXT2_ET_INVALID_ARGUMENT;

	लागि (x = 0, ed = वस्तुहरू; x < nr_वस्तुहरू; x++, ed++) {
		यदि (ed->func == रिक्त)
			फर्काऊ 0;
		यदि (ed->func == func && ed->data == data) {
			माप_प्रकार sz = (nr_वस्तुहरू - (x + 1)) *
				    परिमाण(संरचना exit_data);
			स_हटाओ(ed, ed + 1, sz);
			स_रखो(वस्तुहरू + nr_वस्तुहरू - 1, 0,
			       परिमाण(संरचना exit_data));
		}
	}

	फर्काऊ 0;
}
