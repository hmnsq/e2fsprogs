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

#شامل "config.h"
#شامل <stdio.h>
#شامل <string.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#اگر_ختم
#شامل <fcntl.h>
#شامل <time.h>
#شامل <sys/stat.h>
#شامل <sys/types.h>
#اگر HAVE_ERRNO_H
#شامل <errno.h>
#اگر_ختم

#شامل "ext2_fs.h"
#شامل "ext2fs.h"

صحیح مین(صحیح argc, حرف **argv)
{
	صحیح	lsectsize, psectsize;
	صحیح	retval;
	صحیح	fd;

	اگر (argc < 2) {
		fprintf(stderr, "Usage: %s device\n", argv[0]);
		باہر_نکلو(1);
	}

	retval = ext2fs_get_device_sectsize(argv[1], &lsectsize);
	اگر (retval) {
		com_err(argv[0], retval,
			"while calling ext2fs_get_device_sectsize");
		باہر_نکلو(1);
	}
	retval = ext2fs_get_device_phys_sectsize(argv[1], &psectsize);
	اگر (retval) {
		com_err(argv[0], retval,
			"while calling ext2fs_get_device_phys_sectsize");
		باہر_نکلو(1);
	}
	لکھو("Device %s has logical/physical sector size of %d/%d.\n",
	       argv[1], lsectsize, psectsize);
	fd = open(argv[1], O_RDONLY);
	اگر (fd < 0) {
		perror("open");
		باہر_نکلو(1);
	}
	لکھو("The device's DIO alignment is %d\n",
	       ext2fs_get_dio_alignment(fd));
	close(fd);
	باہر_نکلو(0);
}
