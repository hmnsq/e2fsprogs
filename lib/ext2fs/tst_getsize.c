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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਜੇ HAVE_UNISTD_H
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <fcntl.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <time.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਜੇ HAVE_ERRNO_H
#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fs.h"

ਪੂਰਨ ਮੁੱਖ(ਪੂਰਨ argc, ਅਚਲ ਅੱਖਰ *argv[])
{
	errcode_t	retval;
	blk64_t		ਖੰਡਾਂ;

	ਜੇ (argc < 2) {
		fprintf(stderr, "%s device\n", argv[0]);
		ਬਾਹਰ_ਨਿਕਲੋ(1);
	}
	ਜੋੜੋ_error_ਸਾਰਣੀ(&et_ext2_error_ਸਾਰਣੀ);
	retval = ext2fs_get_device_size2(argv[1], 1024, &ਖੰਡਾਂ);
	ਜੇ (retval) {
		com_err(argv[0], retval, "while getting device size");
		ਬਾਹਰ_ਨਿਕਲੋ(1);
	}
	ਲਿਖੋ("%s is device has %llu blocks.\n", argv[1],
	       (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ) ਖੰਡਾਂ);
	ਵਾਪਸ 0;
}
