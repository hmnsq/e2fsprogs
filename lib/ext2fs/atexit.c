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

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ LARGEFILE_ਸਰੋਤ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LARGEFILE_ਸਰੋਤ
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ LARGEFILE64_ਸਰੋਤ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ LARGEFILE64_ਸਰੋਤ
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fsP.h"

ਬਣਤਰ exit_ਅੰਕੜੇ {
	ext2_exit_fn func;
	ਖਾਲੀ *ਅੰਕੜੇ;
};

ਸਥਿਰ ਬਣਤਰ exit_ਅੰਕੜੇ *ਚੀਜ਼ਾਂ;
ਸਥਿਰ size_t nr_ਚੀਜ਼ਾਂ;

ਸਥਿਰ ਖਾਲੀ handle_exit(ਖਾਲੀ)
{
	ਬਣਤਰ exit_ਅੰਕੜੇ *ed;

	ਲਈ (ed = ਚੀਜ਼ਾਂ + nr_ਚੀਜ਼ਾਂ - 1; ed >= ਚੀਜ਼ਾਂ; ed--) {
		ਜੇ (ed->func == ਨਿਰਾ)
			ਜਾਰੀ;
		ed->func(ed->ਅੰਕੜੇ);
	}

	ext2fs_free_mem(&ਚੀਜ਼ਾਂ);
	nr_ਚੀਜ਼ਾਂ = 0;
}

/*
 * Schedule a function to be called at (normal) program termination.
 * If you want this to be called during a signal exit, you must capture
 * the signal and call exit() yourself!
 */
errcode_t ext2fs_ਜੋੜੋ_exit_fn(ext2_exit_fn func, ਖਾਲੀ *ਅੰਕੜੇ)
{
	ਬਣਤਰ exit_ਅੰਕੜੇ *ed, *free_ed = ਨਿਰਾ;
	size_t x;
	errcode_t ret;

	ਜੇ (func == ਨਿਰਾ)
		ਵਾਪਸ EXT2_ET_INVALID_ਦਲੀਲ;

	ਲਈ (x = 0, ed = ਚੀਜ਼ਾਂ; x < nr_ਚੀਜ਼ਾਂ; x++, ed++) {
		ਜੇ (ed->func == func && ed->ਅੰਕੜੇ == ਅੰਕੜੇ)
			ਵਾਪਸ EXT2_ET_FILE_EXISTS;
		ਜੇ (ed->func == ਨਿਰਾ)
			free_ed = ed;
	}

	ਜੇ (free_ed) {
		free_ed->func = func;
		free_ed->ਅੰਕੜੇ = ਅੰਕੜੇ;
		ਵਾਪਸ 0;
	}

	ਜੇ (nr_ਚੀਜ਼ਾਂ == 0) {
		ret = atexit(handle_exit);
		ਜੇ (ret)
			ਵਾਪਸ ret;
	}

	ret = ext2fs_resize_mem(0, (nr_ਚੀਜ਼ਾਂ + 1) * ਆਕਾਰ(ਬਣਤਰ exit_ਅੰਕੜੇ),
				&ਚੀਜ਼ਾਂ);
	ਜੇ (ret)
		ਵਾਪਸ ret;

	ਚੀਜ਼ਾਂ[nr_ਚੀਜ਼ਾਂ].func = func;
	ਚੀਜ਼ਾਂ[nr_ਚੀਜ਼ਾਂ].ਅੰਕੜੇ = ਅੰਕੜੇ;
	nr_ਚੀਜ਼ਾਂ++;

	ਵਾਪਸ 0;
}

/* Remove a function from the exit cleanup list. */
errcode_t ext2fs_remove_exit_fn(ext2_exit_fn func, ਖਾਲੀ *ਅੰਕੜੇ)
{
	ਬਣਤਰ exit_ਅੰਕੜੇ *ed;
	size_t x;

	ਜੇ (func == ਨਿਰਾ)
		ਵਾਪਸ EXT2_ET_INVALID_ਦਲੀਲ;

	ਲਈ (x = 0, ed = ਚੀਜ਼ਾਂ; x < nr_ਚੀਜ਼ਾਂ; x++, ed++) {
		ਜੇ (ed->func == ਨਿਰਾ)
			ਵਾਪਸ 0;
		ਜੇ (ed->func == func && ed->ਅੰਕੜੇ == ਅੰਕੜੇ) {
			size_t sz = (nr_ਚੀਜ਼ਾਂ - (x + 1)) *
				    ਆਕਾਰ(ਬਣਤਰ exit_ਅੰਕੜੇ);
			memmove(ed, ed + 1, sz);
			memset(ਚੀਜ਼ਾਂ + nr_ਚੀਜ਼ਾਂ - 1, 0,
			       ਆਕਾਰ(ਬਣਤਰ exit_ਅੰਕੜੇ));
		}
	}

	ਵਾਪਸ 0;
}
