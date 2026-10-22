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

صحیح مین(صحیح argc, ثابت حرف *argv[])
{
	errcode_t	retval;
	blk64_t		بلاکیں;

	اگر (argc < 2) {
		fprintf(stderr, "%s device\n", argv[0]);
		باہر_نکلو(1);
	}
	جوڑو_error_جدول(&et_ext2_error_جدول);
	retval = ext2fs_get_device_size2(argv[1], 1024, &بلاکیں);
	اگر (retval) {
		com_err(argv[0], retval, "while getting device size");
		باہر_نکلو(1);
	}
	لکھو("%s is device has %llu blocks.\n", argv[1],
	       (غیرعلامتی طویل طویل) بلاکیں);
	واپس 0;
}
