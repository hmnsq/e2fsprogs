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

#جے_بنا_تعریف LARGEFILE_سروت
#تعریف_کرو LARGEFILE_سروت
#جے_انت
#جے_بنا_تعریف LARGEFILE64_سروت
#تعریف_کرو LARGEFILE64_سروت
#جے_انت

#ضمن "config.h"
#ضمن <stdlib.h>

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "ext2fsP.h"

بنية exit_انکڑے {
	ext2_exit_fn func;
	فراغ *انکڑے;
};

ساكن بنية exit_انکڑے *اشیاء;
ساكن size_t nr_اشیاء;

ساكن فراغ handle_exit(فراغ)
{
	بنية exit_انکڑے *ed;

	لكل (ed = اشیاء + nr_اشیاء - 1; ed >= اشیاء; ed--) {
		إذا (ed->func == عدم)
			استمر;
		ed->func(ed->انکڑے);
	}

	ext2fs_free_mem(&اشیاء);
	nr_اشیاء = 0;
}

/*
 * Schedule a function to be called at (normal) program termination.
 * If you want this to be called during a signal exit, you must capture
 * the signal and call exit() yourself!
 */
errcode_t ext2fs_جوڑو_exit_fn(ext2_exit_fn func, فراغ *انکڑے)
{
	بنية exit_انکڑے *ed, *free_ed = عدم;
	size_t x;
	errcode_t ret;

	إذا (func == عدم)
		أرجع EXT2_ET_INVALID_دلیل;

	لكل (x = 0, ed = اشیاء; x < nr_اشیاء; x++, ed++) {
		إذا (ed->func == func && ed->انکڑے == انکڑے)
			أرجع EXT2_ET_FILE_EXISTS;
		إذا (ed->func == عدم)
			free_ed = ed;
	}

	إذا (free_ed) {
		free_ed->func = func;
		free_ed->انکڑے = انکڑے;
		أرجع 0;
	}

	إذا (nr_اشیاء == 0) {
		ret = atexit(handle_exit);
		إذا (ret)
			أرجع ret;
	}

	ret = ext2fs_resize_mem(0, (nr_اشیاء + 1) * حجم(بنية exit_انکڑے),
				&اشیاء);
	إذا (ret)
		أرجع ret;

	اشیاء[nr_اشیاء].func = func;
	اشیاء[nr_اشیاء].انکڑے = انکڑے;
	nr_اشیاء++;

	أرجع 0;
}

/* Remove a function from the exit cleanup list. */
errcode_t ext2fs_remove_exit_fn(ext2_exit_fn func, فراغ *انکڑے)
{
	بنية exit_انکڑے *ed;
	size_t x;

	إذا (func == عدم)
		أرجع EXT2_ET_INVALID_دلیل;

	لكل (x = 0, ed = اشیاء; x < nr_اشیاء; x++, ed++) {
		إذا (ed->func == عدم)
			أرجع 0;
		إذا (ed->func == func && ed->انکڑے == انکڑے) {
			size_t sz = (nr_اشیاء - (x + 1)) *
				    حجم(بنية exit_انکڑے);
			memmove(ed, ed + 1, sz);
			memset(اشیاء + nr_اشیاء - 1, 0,
			       حجم(بنية exit_انکڑے));
		}
	}

	أرجع 0;
}
