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

#include "config.h"
#include <stdio.h>
#include <string.h>
#nếu HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#nếu HAVE_ERRNO_H
#include <errno.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"

số_nguyên main(số_nguyên argc, char **argv)
{
	số_nguyên	lsectsize, psectsize;
	số_nguyên	retval;
	số_nguyên	fd;

	nếu (argc < 2) {
		fprintf(stderr, "Usage: %s device\n", argv[0]);
		exit(1);
	}

	retval = ext2fs_get_device_sectsize(argv[1], &lsectsize);
	nếu (retval) {
		com_err(argv[0], retval,
			"while calling ext2fs_get_device_sectsize");
		exit(1);
	}
	retval = ext2fs_get_device_phys_sectsize(argv[1], &psectsize);
	nếu (retval) {
		com_err(argv[0], retval,
			"while calling ext2fs_get_device_phys_sectsize");
		exit(1);
	}
	printf("Device %s has logical/physical sector size of %d/%d.\n",
	       argv[1], lsectsize, psectsize);
	fd = open(argv[1], O_RDONLY);
	nếu (fd < 0) {
		perror("open");
		exit(1);
	}
	printf("The device's DIO alignment is %d\n",
	       ext2fs_get_dio_alignment(fd));
	close(fd);
	exit(0);
}
