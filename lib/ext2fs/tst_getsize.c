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
#endif
#ضمن <fcntl.h>
#ضمن <time.h>
#ضمن <sys/stat.h>
#ضمن <sys/types.h>
#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#endif

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

صحيح الرئيسية(صحيح argc, ثابت حرف *argv[])
{
	errcode_t	retval;
	blk64_t		blocks;

	إذا (argc < 2) {
		fprintf(stderr, "%s device\n", argv[0]);
		exit(1);
	}
	add_error_table(&et_ext2_error_table);
	retval = ext2fs_get_device_size2(argv[1], 1024, &blocks);
	إذا (retval) {
		com_err(argv[0], retval, "while getting device size");
		exit(1);
	}
	printf("%s is device has %llu blocks.\n", argv[1],
	       (unsigned long long) blocks);
	إرجاع 0;
}
