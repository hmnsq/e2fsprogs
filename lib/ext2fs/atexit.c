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

#अगर_अघोषित _LARGEFILE_SOURCE
#घोषणा _LARGEFILE_SOURCE
#पूर्ण_अगर
#अगर_अघोषित _LARGEFILE64_SOURCE
#घोषणा _LARGEFILE64_SOURCE
#पूर्ण_अगर

#समावेश "config.h"
#समावेश <stdlib.h>

#समावेश "ext2_fs.h"
#समावेश "ext2fs.h"
#समावेश "ext2fsP.h"

काष्ठा exit_तथ्य {
	ext2_exit_fn func;
	व्योम *तथ्य;
};

अटल काष्ठा exit_तथ्य *वस्तुएँ;
अटल माप_प्रकार nr_वस्तुएँ;

अटल व्योम handle_exit(व्योम)
{
	काष्ठा exit_तथ्य *ed;

	क्रम (ed = वस्तुएँ + nr_वस्तुएँ - 1; ed >= वस्तुएँ; ed--) {
		अगर (ed->func == शून्य)
			जारी;
		ed->func(ed->तथ्य);
	}

	ext2fs_free_mem(&वस्तुएँ);
	nr_वस्तुएँ = 0;
}

/*
 * Schedule a function to be called at (normal) program termination.
 * If you want this to be called during a signal exit, you must capture
 * the signal and call exit() yourself!
 */
errcode_t ext2fs_add_exit_fn(ext2_exit_fn func, व्योम *तथ्य)
{
	काष्ठा exit_तथ्य *ed, *free_ed = शून्य;
	माप_प्रकार x;
	errcode_t ret;

	अगर (func == शून्य)
		वापस EXT2_ET_INVALID_ARGUMENT;

	क्रम (x = 0, ed = वस्तुएँ; x < nr_वस्तुएँ; x++, ed++) {
		अगर (ed->func == func && ed->तथ्य == तथ्य)
			वापस EXT2_ET_FILE_EXISTS;
		अगर (ed->func == शून्य)
			free_ed = ed;
	}

	अगर (free_ed) {
		free_ed->func = func;
		free_ed->तथ्य = तथ्य;
		वापस 0;
	}

	अगर (nr_वस्तुएँ == 0) {
		ret = निकास_पर(handle_exit);
		अगर (ret)
			वापस ret;
	}

	ret = ext2fs_resize_mem(0, (nr_वस्तुएँ + 1) * माप(काष्ठा exit_तथ्य),
				&वस्तुएँ);
	अगर (ret)
		वापस ret;

	वस्तुएँ[nr_वस्तुएँ].func = func;
	वस्तुएँ[nr_वस्तुएँ].तथ्य = तथ्य;
	nr_वस्तुएँ++;

	वापस 0;
}

/* Remove a function from the exit cleanup list. */
errcode_t ext2fs_remove_exit_fn(ext2_exit_fn func, व्योम *तथ्य)
{
	काष्ठा exit_तथ्य *ed;
	माप_प्रकार x;

	अगर (func == शून्य)
		वापस EXT2_ET_INVALID_ARGUMENT;

	क्रम (x = 0, ed = वस्तुएँ; x < nr_वस्तुएँ; x++, ed++) {
		अगर (ed->func == शून्य)
			वापस 0;
		अगर (ed->func == func && ed->तथ्य == तथ्य) {
			माप_प्रकार sz = (nr_वस्तुएँ - (x + 1)) *
				    माप(काष्ठा exit_तथ्य);
			स_हटाओ(ed, ed + 1, sz);
			स_रखो(वस्तुएँ + nr_वस्तुएँ - 1, 0,
			       माप(काष्ठा exit_तथ्य));
		}
	}

	वापस 0;
}
