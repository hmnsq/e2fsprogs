/*
 * tst_getsize.c --- this function tests the getsize function
 *
 * Copyright (C) 1997 by Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <string.h>
#ఒకవేళ HAVE_UNISTD_H
#అన్తర్భావ <unistd.h>
#అయితే_ముగింపు
#అన్తర్భావ <fcntl.h>
#అన్తర్భావ <time.h>
#అన్తర్భావ <sys/stat.h>
#అన్తర్భావ <sys/types.h>
#ఒకవేళ HAVE_ERRNO_H
#అన్తర్భావ <errno.h>
#అయితే_ముగింపు

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fs.h"

పూర్ణాంకం ప్రధాన(పూర్ణాంకం argc, అక్షరం **argv)
{
	పూర్ణాంకం	lsectsize, psectsize;
	పూర్ణాంకం	retval;
	పూర్ణాంకం	fd;

	ఒకవేళ (argc < 2) {
		fprintf(stderr, "Usage: %s device\n", argv[0]);
		నిష్క్రమించు(1);
	}

	retval = ext2fs_get_device_sectsize(argv[1], &lsectsize);
	ఒకవేళ (retval) {
		com_err(argv[0], retval,
			"while calling ext2fs_get_device_sectsize");
		నిష్క్రమించు(1);
	}
	retval = ext2fs_get_device_phys_sectsize(argv[1], &psectsize);
	ఒకవేళ (retval) {
		com_err(argv[0], retval,
			"while calling ext2fs_get_device_phys_sectsize");
		నిష్క్రమించు(1);
	}
	రాయి("Device %s has logical/physical sector size of %d/%d.\n",
	       argv[1], lsectsize, psectsize);
	fd = open(argv[1], O_RDONLY);
	ఒకవేళ (fd < 0) {
		perror("open");
		నిష్క్రమించు(1);
	}
	రాయి("The device's DIO alignment is %d\n",
	       ext2fs_get_dio_alignment(fd));
	close(fd);
	నిష్క్రమించు(0);
}
