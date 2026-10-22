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

स्थिर संरचना exit_data *वस्तुसंच;
स्थिर माप_प्रकार nr_वस्तुसंच;

स्थिर शून्य handle_exit(शून्य)
{
	संरचना exit_data *ed;

	साठी (ed = वस्तुसंच + nr_वस्तुसंच - 1; ed >= वस्तुसंच; ed--) {
		जर (ed->func == रिक्त)
			सुरूठेवा;
		ed->func(ed->data);
	}

	ext2fs_free_mem(&वस्तुसंच);
	nr_वस्तुसंच = 0;
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

	जर (func == रिक्त)
		परत EXT2_ET_INVALID_ARGUMENT;

	साठी (x = 0, ed = वस्तुसंच; x < nr_वस्तुसंच; x++, ed++) {
		जर (ed->func == func && ed->data == data)
			परत EXT2_ET_FILE_EXISTS;
		जर (ed->func == रिक्त)
			free_ed = ed;
	}

	जर (free_ed) {
		free_ed->func = func;
		free_ed->data = data;
		परत 0;
	}

	जर (nr_वस्तुसंच == 0) {
		ret = निकास_पर(handle_exit);
		जर (ret)
			परत ret;
	}

	ret = ext2fs_resize_mem(0, (nr_वस्तुसंच + 1) * आकार(संरचना exit_data),
				&वस्तुसंच);
	जर (ret)
		परत ret;

	वस्तुसंच[nr_वस्तुसंच].func = func;
	वस्तुसंच[nr_वस्तुसंच].data = data;
	nr_वस्तुसंच++;

	परत 0;
}

/* Remove a function from the exit cleanup list. */
errcode_t ext2fs_remove_exit_fn(ext2_exit_fn func, शून्य *data)
{
	संरचना exit_data *ed;
	माप_प्रकार x;

	जर (func == रिक्त)
		परत EXT2_ET_INVALID_ARGUMENT;

	साठी (x = 0, ed = वस्तुसंच; x < nr_वस्तुसंच; x++, ed++) {
		जर (ed->func == रिक्त)
			परत 0;
		जर (ed->func == func && ed->data == data) {
			माप_प्रकार sz = (nr_वस्तुसंच - (x + 1)) *
				    आकार(संरचना exit_data);
			स_हटाओ(ed, ed + 1, sz);
			स_रखो(वस्तुसंच + nr_वस्तुसंच - 1, 0,
			       आकार(संरचना exit_data));
		}
	}

	परत 0;
}
