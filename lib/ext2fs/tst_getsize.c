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

#অন্তর্ভাব "config.h"
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <string.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাব <unistd.h>
#endif
#অন্তর্ভাব <fcntl.h>
#অন্তর্ভাব <time.h>
#অন্তর্ভাব <sys/stat.h>
#অন্তর্ভাব <sys/types.h>
#যদি HAVE_ERRNO_H
#অন্তর্ভাব <errno.h>
#endif

#অন্তর্ভাব "ext2_fs.h"
#অন্তর্ভাব "ext2fs.h"

পূর্ণাঙ্ক main(পূর্ণাঙ্ক argc, ধ্রুব বর্ণ *argv[])
{
	errcode_t	retval;
	blk64_t		blocks;

	যদি (argc < 2) {
		fprintf(stderr, "%s device\n", argv[0]);
		exit(1);
	}
	add_error_table(&et_ext2_error_table);
	retval = ext2fs_get_device_size2(argv[1], 1024, &blocks);
	যদি (retval) {
		com_err(argv[0], retval, "while getting device size");
		exit(1);
	}
	printf("%s is device has %llu blocks.\n", argv[1],
	       (unsigned long long) blocks);
	প্রত্যাগম 0;
}
