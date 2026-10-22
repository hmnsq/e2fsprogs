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

#నిర్వచితం_కాకపోతే LARGEFILE_ఆధారం
#నిర్వచించు LARGEFILE_ఆధారం
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే LARGEFILE64_ఆధారం
#నిర్వచించు LARGEFILE64_ఆధారం
#అయితే_ముగింపు

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdlib.h>

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fs.h"
#అన్తర్భావ "ext2fsP.h"

నిర్మాణం exit_దత్తాంశం {
	ext2_exit_fn func;
	శూన్య *దత్తాంశం;
};

స్థిర నిర్మాణం exit_దత్తాంశం *వస్తువులు;
స్థిర size_t nr_వస్తువులు;

స్థిర శూన్య handle_exit(శూన్య)
{
	నిర్మాణం exit_దత్తాంశం *ed;

	కొరకు (ed = వస్తువులు + nr_వస్తువులు - 1; ed >= వస్తువులు; ed--) {
		ఒకవేళ (ed->func == ఖాళీ)
			కొనసాగు;
		ed->func(ed->దత్తాంశం);
	}

	ext2fs_free_mem(&వస్తువులు);
	nr_వస్తువులు = 0;
}

/*
 * Schedule a function to be called at (normal) program termination.
 * If you want this to be called during a signal exit, you must capture
 * the signal and call exit() yourself!
 */
errcode_t ext2fs_కూడు_exit_fn(ext2_exit_fn func, శూన్య *దత్తాంశం)
{
	నిర్మాణం exit_దత్తాంశం *ed, *free_ed = ఖాళీ;
	size_t x;
	errcode_t ret;

	ఒకవేళ (func == ఖాళీ)
		తిరిగి EXT2_ET_INVALID_వాదం;

	కొరకు (x = 0, ed = వస్తువులు; x < nr_వస్తువులు; x++, ed++) {
		ఒకవేళ (ed->func == func && ed->దత్తాంశం == దత్తాంశం)
			తిరిగి EXT2_ET_FILE_EXISTS;
		ఒకవేళ (ed->func == ఖాళీ)
			free_ed = ed;
	}

	ఒకవేళ (free_ed) {
		free_ed->func = func;
		free_ed->దత్తాంశం = దత్తాంశం;
		తిరిగి 0;
	}

	ఒకవేళ (nr_వస్తువులు == 0) {
		ret = atexit(handle_exit);
		ఒకవేళ (ret)
			తిరిగి ret;
	}

	ret = ext2fs_resize_mem(0, (nr_వస్తువులు + 1) * పరిమాణం(నిర్మాణం exit_దత్తాంశం),
				&వస్తువులు);
	ఒకవేళ (ret)
		తిరిగి ret;

	వస్తువులు[nr_వస్తువులు].func = func;
	వస్తువులు[nr_వస్తువులు].దత్తాంశం = దత్తాంశం;
	nr_వస్తువులు++;

	తిరిగి 0;
}

/* Remove a function from the exit cleanup list. */
errcode_t ext2fs_remove_exit_fn(ext2_exit_fn func, శూన్య *దత్తాంశం)
{
	నిర్మాణం exit_దత్తాంశం *ed;
	size_t x;

	ఒకవేళ (func == ఖాళీ)
		తిరిగి EXT2_ET_INVALID_వాదం;

	కొరకు (x = 0, ed = వస్తువులు; x < nr_వస్తువులు; x++, ed++) {
		ఒకవేళ (ed->func == ఖాళీ)
			తిరిగి 0;
		ఒకవేళ (ed->func == func && ed->దత్తాంశం == దత్తాంశం) {
			size_t sz = (nr_వస్తువులు - (x + 1)) *
				    పరిమాణం(నిర్మాణం exit_దత్తాంశం);
			memmove(ed, ed + 1, sz);
			memset(వస్తువులు + nr_వస్తువులు - 1, 0,
			       పరిమాణం(నిర్మాణం exit_దత్తాంశం));
		}
	}

	తిరిగి 0;
}
