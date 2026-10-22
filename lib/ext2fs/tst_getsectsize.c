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
#endif
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <time.h>
#अन्तर्भाव <sys/stat.h>
#अन्तर्भाव <sys/types.h>
#यदि HAVE_ERRNO_H
#अन्तर्भाव <errno.h>
#endif

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"

पूर्णाङ्क main(पूर्णाङ्क argc, वर्ण **argv)
{
	पूर्णाङ्क	lsectsize, psectsize;
	पूर्णाङ्क	retval;
	पूर्णाङ्क	fd;

	यदि (argc < 2) {
		ख_लिखो(मानक_त्रुटि, "Usage: %s device\n", argv[0]);
		निकास(1);
	}

	retval = ext2fs_get_device_sectsize(argv[1], &lsectsize);
	यदि (retval) {
		com_err(argv[0], retval,
			"while calling ext2fs_get_device_sectsize");
		निकास(1);
	}
	retval = ext2fs_get_device_phys_sectsize(argv[1], &psectsize);
	यदि (retval) {
		com_err(argv[0], retval,
			"while calling ext2fs_get_device_phys_sectsize");
		निकास(1);
	}
	म_लिखो("Device %s has logical/physical sector size of %d/%d.\n",
	       argv[1], lsectsize, psectsize);
	fd = खोलो(argv[1], O_RDONLY);
	यदि (fd < 0) {
		लिखो_त्रुटि("open");
		निकास(1);
	}
	म_लिखो("The device's DIO alignment is %d\n",
	       ext2fs_get_dio_alignment(fd));
	बंद(fd);
	निकास(0);
}
