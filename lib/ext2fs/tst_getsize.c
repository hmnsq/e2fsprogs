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
#جيڪڏهن_پورو
#ضمن <fcntl.h>
#ضمن <time.h>
#ضمن <sys/stat.h>
#ضمن <sys/types.h>
#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#جيڪڏهن_پورو

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

صحيح مکيه(صحيح argc, ثابت حرف *argv[])
{
	errcode_t	retval;
	blk64_t		بلاڪون;

	إذا (argc < 2) {
		fprintf(stderr, "%s device\n", argv[0]);
		ٻاهر_نڪرو(1);
	}
	جوڙيو_error_جدول(&et_ext2_error_جدول);
	retval = ext2fs_get_device_size2(argv[1], 1024, &بلاڪون);
	إذا (retval) {
		com_err(argv[0], retval, "while getting device size");
		ٻاهر_نڪرو(1);
	}
	لکو("%s is device has %llu blocks.\n", argv[1],
	       (بنانشان ڊگهو ڊگهو) بلاڪون);
	أرجع 0;
}
