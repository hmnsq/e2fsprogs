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

पूर्णाङ्क main(पूर्णाङ्क argc, ध्रुव वर्ण *argv[])
{
	errcode_t	retval;
	blk64_t		blocks;

	यदि (argc < 2) {
		ख_लिखो(मानक_त्रुटि, "%s device\n", argv[0]);
		निकास(1);
	}
	add_error_table(&et_ext2_error_table);
	retval = ext2fs_get_device_size2(argv[1], 1024, &blocks);
	यदि (retval) {
		com_err(argv[0], retval, "while getting device size");
		निकास(1);
	}
	म_लिखो("%s is device has %llu blocks.\n", argv[1],
	       (अचिन्हित दीर्घ दीर्घ) blocks);
	प्रत्यागम 0;
}
