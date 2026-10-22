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

#அந்தர்பாவ "config.h"
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <string.h>
#எனில் HAVE_UNISTD_H
#அந்தர்பாவ <unistd.h>
#endif
#அந்தர்பாவ <fcntl.h>
#அந்தர்பாவ <time.h>
#அந்தர்பாவ <sys/stat.h>
#அந்தர்பாவ <sys/types.h>
#எனில் HAVE_ERRNO_H
#அந்தர்பாவ <errno.h>
#endif

#அந்தர்பாவ "ext2_fs.h"
#அந்தர்பாவ "ext2fs.h"

முழு முதன்மை(முழு argc, மாறிலி எழுத்து *argv[])
{
	errcode_t	retval;
	blk64_t		blocks;

	எனில் (argc < 2) {
		fprintf(stderr, "%s device\n", argv[0]);
		exit(1);
	}
	add_error_table(&et_ext2_error_table);
	retval = ext2fs_get_device_size2(argv[1], 1024, &blocks);
	எனில் (retval) {
		com_err(argv[0], retval, "while getting device size");
		exit(1);
	}
	printf("%s is device has %llu blocks.\n", argv[1],
	       (unsigned long long) blocks);
	திரும்பு 0;
}
