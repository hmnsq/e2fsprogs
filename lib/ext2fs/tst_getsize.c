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

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <string.h>
#എങ്കിൽ HAVE_UNISTD_H
#അന്തര്ഭാവ <unistd.h>
#endif
#അന്തര്ഭാവ <fcntl.h>
#അന്തര്ഭാവ <time.h>
#അന്തര്ഭാവ <sys/stat.h>
#അന്തര്ഭാവ <sys/types.h>
#എങ്കിൽ HAVE_ERRNO_H
#അന്തര്ഭാവ <errno.h>
#endif

#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ "ext2fs.h"

പൂർണ്ണസംഖ്യ പ്രധാന(പൂർണ്ണസംഖ്യ argc, സ്ഥിരം അക്ഷരം *argv[])
{
	errcode_t	retval;
	blk64_t		blocks;

	എങ്കിൽ (argc < 2) {
		fprintf(stderr, "%s device\n", argv[0]);
		exit(1);
	}
	add_error_table(&et_ext2_error_table);
	retval = ext2fs_get_device_size2(argv[1], 1024, &blocks);
	എങ്കിൽ (retval) {
		com_err(argv[0], retval, "while getting device size");
		exit(1);
	}
	printf("%s is device has %llu blocks.\n", argv[1],
	       (unsigned long long) blocks);
	തിരികെ 0;
}
