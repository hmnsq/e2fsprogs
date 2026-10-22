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
#𐡄𐡍 HAVE_UNISTD_H
#ضمن <unistd.h>
#endif
#ضمن <fcntl.h>
#ضمن <time.h>
#ضمن <sys/stat.h>
#ضمن <sys/types.h>
#𐡄𐡍 HAVE_ERRNO_H
#ضمن <errno.h>
#endif

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

صحيح main(صحيح argc, حرف **argv)
{
	صحيح	lsectsize, psectsize;
	صحيح	retval;
	صحيح	fd;

	𐡄𐡍 (argc < 2) {
		fprintf(stderr, "Usage: %s device\n", argv[0]);
		exit(1);
	}

	retval = ext2fs_get_device_sectsize(argv[1], &lsectsize);
	𐡄𐡍 (retval) {
		com_err(argv[0], retval,
			"while calling ext2fs_get_device_sectsize");
		exit(1);
	}
	retval = ext2fs_get_device_phys_sectsize(argv[1], &psectsize);
	𐡄𐡍 (retval) {
		com_err(argv[0], retval,
			"while calling ext2fs_get_device_phys_sectsize");
		exit(1);
	}
	printf("Device %s has logical/physical sector size of %d/%d.\n",
	       argv[1], lsectsize, psectsize);
	fd = open(argv[1], O_RDONLY);
	𐡄𐡍 (fd < 0) {
		perror("open");
		exit(1);
	}
	printf("The device's DIO alignment is %d\n",
	       ext2fs_get_dio_alignment(fd));
	close(fd);
	exit(0);
}
