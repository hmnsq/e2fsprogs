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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ LARGEFILE_ಮೂಲ
#ವ್ಯಾಖ್ಯಾನಿಸು LARGEFILE_ಮೂಲ
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ LARGEFILE64_ಮೂಲ
#ವ್ಯಾಖ್ಯಾನಿಸು LARGEFILE64_ಮೂಲ
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdlib.h>

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fsP.h"

ರಚನೆ exit_ದತ್ತಾಂಶ {
	ext2_exit_fn func;
	ಶೂನ್ಯ *ದತ್ತಾಂಶ;
};

ಸ್ಥಿರ ರಚನೆ exit_ದತ್ತಾಂಶ *ವಸ್ತುಗಳು;
ಸ್ಥಿರ size_t nr_ವಸ್ತುಗಳು;

ಸ್ಥಿರ ಶೂನ್ಯ handle_exit(ಶೂನ್ಯ)
{
	ರಚನೆ exit_ದತ್ತಾಂಶ *ed;

	ಗಾಗಿ (ed = ವಸ್ತುಗಳು + nr_ವಸ್ತುಗಳು - 1; ed >= ವಸ್ತುಗಳು; ed--) {
		ಆದರೆ (ed->func == ಖಾಲಿ)
			ಮುಂದುವರಿ;
		ed->func(ed->ದತ್ತಾಂಶ);
	}

	ext2fs_free_mem(&ವಸ್ತುಗಳು);
	nr_ವಸ್ತುಗಳು = 0;
}

/*
 * Schedule a function to be called at (normal) program termination.
 * If you want this to be called during a signal exit, you must capture
 * the signal and call exit() yourself!
 */
errcode_t ext2fs_ಕೂಡಿಸು_exit_fn(ext2_exit_fn func, ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
	ರಚನೆ exit_ದತ್ತಾಂಶ *ed, *free_ed = ಖಾಲಿ;
	size_t x;
	errcode_t ret;

	ಆದರೆ (func == ಖಾಲಿ)
		ಹಿಂತಿರುಗಿ EXT2_ET_INVALID_ವಾದ;

	ಗಾಗಿ (x = 0, ed = ವಸ್ತುಗಳು; x < nr_ವಸ್ತುಗಳು; x++, ed++) {
		ಆದರೆ (ed->func == func && ed->ದತ್ತಾಂಶ == ದತ್ತಾಂಶ)
			ಹಿಂತಿರುಗಿ EXT2_ET_FILE_EXISTS;
		ಆದರೆ (ed->func == ಖಾಲಿ)
			free_ed = ed;
	}

	ಆದರೆ (free_ed) {
		free_ed->func = func;
		free_ed->ದತ್ತಾಂಶ = ದತ್ತಾಂಶ;
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ (nr_ವಸ್ತುಗಳು == 0) {
		ret = atexit(handle_exit);
		ಆದರೆ (ret)
			ಹಿಂತಿರುಗಿ ret;
	}

	ret = ext2fs_resize_mem(0, (nr_ವಸ್ತುಗಳು + 1) * ಗಾತ್ರ(ರಚನೆ exit_ದತ್ತಾಂಶ),
				&ವಸ್ತುಗಳು);
	ಆದರೆ (ret)
		ಹಿಂತಿರುಗಿ ret;

	ವಸ್ತುಗಳು[nr_ವಸ್ತುಗಳು].func = func;
	ವಸ್ತುಗಳು[nr_ವಸ್ತುಗಳು].ದತ್ತಾಂಶ = ದತ್ತಾಂಶ;
	nr_ವಸ್ತುಗಳು++;

	ಹಿಂತಿರುಗಿ 0;
}

/* Remove a function from the exit cleanup list. */
errcode_t ext2fs_remove_exit_fn(ext2_exit_fn func, ಶೂನ್ಯ *ದತ್ತಾಂಶ)
{
	ರಚನೆ exit_ದತ್ತಾಂಶ *ed;
	size_t x;

	ಆದರೆ (func == ಖಾಲಿ)
		ಹಿಂತಿರುಗಿ EXT2_ET_INVALID_ವಾದ;

	ಗಾಗಿ (x = 0, ed = ವಸ್ತುಗಳು; x < nr_ವಸ್ತುಗಳು; x++, ed++) {
		ಆದರೆ (ed->func == ಖಾಲಿ)
			ಹಿಂತಿರುಗಿ 0;
		ಆದರೆ (ed->func == func && ed->ದತ್ತಾಂಶ == ದತ್ತಾಂಶ) {
			size_t sz = (nr_ವಸ್ತುಗಳು - (x + 1)) *
				    ಗಾತ್ರ(ರಚನೆ exit_ದತ್ತಾಂಶ);
			memmove(ed, ed + 1, sz);
			memset(ವಸ್ತುಗಳು + nr_ವಸ್ತುಗಳು - 1, 0,
			       ಗಾತ್ರ(ರಚನೆ exit_ದತ್ತಾಂಶ));
		}
	}

	ಹಿಂತಿರುಗಿ 0;
}
