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

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#यदि_अन्तः
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <time.h>
#अन्तर्भाव <sys/stat.h>
#अन्तर्भाव <sys/types.h>
#यदि HAVE_ERRNO_H
#अन्तर्भाव <errno.h>
#यदि_अन्तः

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"

पूर्णाङ्क मुख्यम्(पूर्णाङ्क argc, वर्ण **argv)
{
	पूर्णाङ्क	lsectsize, psectsize;
	पूर्णाङ्क	retval;
	पूर्णाङ्क	fd;

	यदि (argc < 2) {
		fprintf(stderr, "Usage: %s device\n", argv[0]);
		निर्गच्छ(1);
	}

	retval = ext2fs_get_device_sectsize(argv[1], &lsectsize);
	यदि (retval) {
		com_err(argv[0], retval,
			"while calling ext2fs_get_device_sectsize");
		निर्गच्छ(1);
	}
	retval = ext2fs_get_device_phys_sectsize(argv[1], &psectsize);
	यदि (retval) {
		com_err(argv[0], retval,
			"while calling ext2fs_get_device_phys_sectsize");
		निर्गच्छ(1);
	}
	प्रारूप_लिख("Device %s has logical/physical sector size of %d/%d.\n",
	       argv[1], lsectsize, psectsize);
	fd = उद्घाटय(argv[1], O_RDONLY);
	यदि (fd < 0) {
		perror("open");
		निर्गच्छ(1);
	}
	प्रारूप_लिख("The device's DIO alignment is %d\n",
	       ext2fs_get_dio_alignment(fd));
	close(fd);
	निर्गच्छ(0);
}
