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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਜੇ HAVE_UNISTD_H
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <fcntl.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <time.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਜੇ HAVE_ERRNO_H
#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fs.h"

ਪੂਰਨ ਮੁੱਖ(ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
	ਪੂਰਨ	lsectsize, psectsize;
	ਪੂਰਨ	retval;
	ਪੂਰਨ	fd;

	ਜੇ (argc < 2) {
		fprintf(stderr, "Usage: %s device\n", argv[0]);
		ਬਾਹਰ_ਨਿਕਲੋ(1);
	}

	retval = ext2fs_get_device_sectsize(argv[1], &lsectsize);
	ਜੇ (retval) {
		com_err(argv[0], retval,
			"while calling ext2fs_get_device_sectsize");
		ਬਾਹਰ_ਨਿਕਲੋ(1);
	}
	retval = ext2fs_get_device_phys_sectsize(argv[1], &psectsize);
	ਜੇ (retval) {
		com_err(argv[0], retval,
			"while calling ext2fs_get_device_phys_sectsize");
		ਬਾਹਰ_ਨਿਕਲੋ(1);
	}
	ਲਿਖੋ("Device %s has logical/physical sector size of %d/%d.\n",
	       argv[1], lsectsize, psectsize);
	fd = open(argv[1], O_RDONLY);
	ਜੇ (fd < 0) {
		perror("open");
		ਬਾਹਰ_ਨਿਕਲੋ(1);
	}
	ਲਿਖੋ("The device's DIO alignment is %d\n",
	       ext2fs_get_dio_alignment(fd));
	close(fd);
	ਬਾਹਰ_ਨਿਕਲੋ(0);
}
