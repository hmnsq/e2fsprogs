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

#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <string.h>
#જો HAVE_UNISTD_H
#અન્તર્ભાવ <unistd.h>
#endif
#અન્તર્ભાવ <fcntl.h>
#અન્તર્ભાવ <time.h>
#અન્તર્ભાવ <sys/stat.h>
#અન્તર્ભાવ <sys/types.h>
#જો HAVE_ERRNO_H
#અન્તર્ભાવ <errno.h>
#endif

#અન્તર્ભાવ "ext2_fs.h"
#અન્તર્ભાવ "ext2fs.h"

પૂર્ણાંક મુખ્ય(પૂર્ણાંક argc, અચલ અક્ષર *argv[])
{
	errcode_t	retval;
	blk64_t		blocks;

	જો (argc < 2) {
		fprintf(stderr, "%s device\n", argv[0]);
		exit(1);
	}
	add_error_table(&et_ext2_error_table);
	retval = ext2fs_get_device_size2(argv[1], 1024, &blocks);
	જો (retval) {
		com_err(argv[0], retval, "while getting device size");
		exit(1);
	}
	printf("%s is device has %llu blocks.\n", argv[1],
	       (unsigned long long) blocks);
	પાછા 0;
}
