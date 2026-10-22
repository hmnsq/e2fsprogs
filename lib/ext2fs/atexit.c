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

#اگر_غیر_تعریف_شدہ LARGEFILE_ماخذ
#تعریف_کرو LARGEFILE_ماخذ
#اگر_ختم
#اگر_غیر_تعریف_شدہ LARGEFILE64_ماخذ
#تعریف_کرو LARGEFILE64_ماخذ
#اگر_ختم

#شامل "config.h"
#شامل <stdlib.h>

#شامل "ext2_fs.h"
#شامل "ext2fs.h"
#شامل "ext2fsP.h"

ساخت exit_ڈیٹا {
	ext2_exit_fn func;
	خالی *ڈیٹا;
};

ساکن ساخت exit_ڈیٹا *اشیاء;
ساکن size_t nr_اشیاء;

ساکن خالی handle_exit(خالی)
{
	ساخت exit_ڈیٹا *ed;

	برائے (ed = اشیاء + nr_اشیاء - 1; ed >= اشیاء; ed--) {
		اگر (ed->func == تہی)
			جاری;
		ed->func(ed->ڈیٹا);
	}

	ext2fs_free_mem(&اشیاء);
	nr_اشیاء = 0;
}

/*
 * Schedule a function to be called at (normal) program termination.
 * If you want this to be called during a signal exit, you must capture
 * the signal and call exit() yourself!
 */
errcode_t ext2fs_جوڑو_exit_fn(ext2_exit_fn func, خالی *ڈیٹا)
{
	ساخت exit_ڈیٹا *ed, *free_ed = تہی;
	size_t x;
	errcode_t ret;

	اگر (func == تہی)
		واپس EXT2_ET_INVALID_دلیل;

	برائے (x = 0, ed = اشیاء; x < nr_اشیاء; x++, ed++) {
		اگر (ed->func == func && ed->ڈیٹا == ڈیٹا)
			واپس EXT2_ET_FILE_EXISTS;
		اگر (ed->func == تہی)
			free_ed = ed;
	}

	اگر (free_ed) {
		free_ed->func = func;
		free_ed->ڈیٹا = ڈیٹا;
		واپس 0;
	}

	اگر (nr_اشیاء == 0) {
		ret = atexit(handle_exit);
		اگر (ret)
			واپس ret;
	}

	ret = ext2fs_resize_mem(0, (nr_اشیاء + 1) * حجم(ساخت exit_ڈیٹا),
				&اشیاء);
	اگر (ret)
		واپس ret;

	اشیاء[nr_اشیاء].func = func;
	اشیاء[nr_اشیاء].ڈیٹا = ڈیٹا;
	nr_اشیاء++;

	واپس 0;
}

/* Remove a function from the exit cleanup list. */
errcode_t ext2fs_remove_exit_fn(ext2_exit_fn func, خالی *ڈیٹا)
{
	ساخت exit_ڈیٹا *ed;
	size_t x;

	اگر (func == تہی)
		واپس EXT2_ET_INVALID_دلیل;

	برائے (x = 0, ed = اشیاء; x < nr_اشیاء; x++, ed++) {
		اگر (ed->func == تہی)
			واپس 0;
		اگر (ed->func == func && ed->ڈیٹا == ڈیٹا) {
			size_t sz = (nr_اشیاء - (x + 1)) *
				    حجم(ساخت exit_ڈیٹا);
			memmove(ed, ed + 1, sz);
			memset(اشیاء + nr_اشیاء - 1, 0,
			       حجم(ساخت exit_ڈیٹا));
		}
	}

	واپس 0;
}
