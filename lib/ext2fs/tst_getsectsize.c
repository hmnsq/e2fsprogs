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

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#پایان_اگر
#ضمن <fcntl.h>
#ضمن <time.h>
#ضمن <sys/stat.h>
#ضمن <sys/types.h>
#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#پایان_اگر

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

صحيح اصلی(صحيح argc, حرف **argv)
{
	صحيح	lsectsize, psectsize;
	صحيح	retval;
	صحيح	fd;

	إذا (argc < 2) {
		fprintf(stderr, "Usage: %s device\n", argv[0]);
		خروج(1);
	}

	retval = ext2fs_get_device_sectsize(argv[1], &lsectsize);
	إذا (retval) {
		com_err(argv[0], retval,
			"while calling ext2fs_get_device_sectsize");
		خروج(1);
	}
	retval = ext2fs_get_device_phys_sectsize(argv[1], &psectsize);
	إذا (retval) {
		com_err(argv[0], retval,
			"while calling ext2fs_get_device_phys_sectsize");
		خروج(1);
	}
	چاپ_کن("Device %s has logical/physical sector size of %d/%d.\n",
	       argv[1], lsectsize, psectsize);
	fd = open(argv[1], O_RDONLY);
	إذا (fd < 0) {
		perror("open");
		خروج(1);
	}
	چاپ_کن("The device's DIO alignment is %d\n",
	       ext2fs_get_dio_alignment(fd));
	close(fd);
	خروج(0);
}
