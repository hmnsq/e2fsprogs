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

पूर्णाङ्क मुख्यम्(पूर्णाङ्क argc, ध्रुव वर्ण *argv[])
{
	errcode_t	retval;
	blk64_t		खण्डानि;

	यदि (argc < 2) {
		fprintf(stderr, "%s device\n", argv[0]);
		निर्गच्छ(1);
	}
	सङ्कलय_error_सारणी(&et_ext2_error_सारणी);
	retval = ext2fs_get_device_size2(argv[1], 1024, &खण्डानि);
	यदि (retval) {
		com_err(argv[0], retval, "while getting device size");
		निर्गच्छ(1);
	}
	प्रारूप_लिख("%s is device has %llu blocks.\n", argv[1],
	       (अचिह्नित दीर्घ दीर्घ) खण्डानि);
	प्रत्यागम 0;
}
