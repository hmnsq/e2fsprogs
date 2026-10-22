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

#समावेश "config.h"
#समावेश <stdio.h>
#समावेश <string.h>
#अगर HAVE_UNISTD_H
#समावेश <unistd.h>
#पूर्ण_अगर
#समावेश <fcntl.h>
#समावेश <time.h>
#समावेश <sys/stat.h>
#समावेश <sys/types.h>
#अगर HAVE_ERRNO_H
#समावेश <errno.h>
#पूर्ण_अगर

#समावेश "ext2_fs.h"
#समावेश "ext2fs.h"

पूर्णांक मुख्य(पूर्णांक argc, स्थिर अक्षर *argv[])
{
	errcode_t	retval;
	blk64_t		blocks;

	अगर (argc < 2) {
		ख_लिखो(मानक_त्रुटि, "%s device\n", argv[0]);
		निकास(1);
	}
	add_error_table(&et_ext2_error_table);
	retval = ext2fs_get_device_size2(argv[1], 1024, &blocks);
	अगर (retval) {
		com_err(argv[0], retval, "while getting device size");
		निकास(1);
	}
	म_लिखो("%s is device has %llu blocks.\n", argv[1],
	       (अचिन्हित दीर्घ दीर्घ) blocks);
	वापस 0;
}
