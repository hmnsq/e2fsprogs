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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಆದರೆ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <time.h>
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಆದರೆ HAVE_ERRNO_H
#ಅನ್ತರ್ಭಾವ <errno.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"

ಪೂರ್ಣಾಂಕ ಮುಖ್ಯ(ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
	ಪೂರ್ಣಾಂಕ	lsectsize, psectsize;
	ಪೂರ್ಣಾಂಕ	retval;
	ಪೂರ್ಣಾಂಕ	fd;

	ಆದರೆ (argc < 2) {
		fprintf(stderr, "Usage: %s device\n", argv[0]);
		ನಿರ್ಗಮಿಸು(1);
	}

	retval = ext2fs_get_device_sectsize(argv[1], &lsectsize);
	ಆದರೆ (retval) {
		com_err(argv[0], retval,
			"while calling ext2fs_get_device_sectsize");
		ನಿರ್ಗಮಿಸು(1);
	}
	retval = ext2fs_get_device_phys_sectsize(argv[1], &psectsize);
	ಆದರೆ (retval) {
		com_err(argv[0], retval,
			"while calling ext2fs_get_device_phys_sectsize");
		ನಿರ್ಗಮಿಸು(1);
	}
	ಬರೆ("Device %s has logical/physical sector size of %d/%d.\n",
	       argv[1], lsectsize, psectsize);
	fd = open(argv[1], O_RDONLY);
	ಆದರೆ (fd < 0) {
		perror("open");
		ನಿರ್ಗಮಿಸು(1);
	}
	ಬರೆ("The device's DIO alignment is %d\n",
	       ext2fs_get_dio_alignment(fd));
	close(fd);
	ನಿರ್ಗಮಿಸು(0);
}
