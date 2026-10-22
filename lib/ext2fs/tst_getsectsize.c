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

पूर्णांक मुख्य(पूर्णांक argc, अक्षर **argv)
{
	पूर्णांक	lsectsize, psectsize;
	पूर्णांक	retval;
	पूर्णांक	fd;

	अगर (argc < 2) {
		ख_लिखो(मानक_त्रुटि, "Usage: %s device\n", argv[0]);
		निकास(1);
	}

	retval = ext2fs_get_device_sectsize(argv[1], &lsectsize);
	अगर (retval) {
		com_err(argv[0], retval,
			"while calling ext2fs_get_device_sectsize");
		निकास(1);
	}
	retval = ext2fs_get_device_phys_sectsize(argv[1], &psectsize);
	अगर (retval) {
		com_err(argv[0], retval,
			"while calling ext2fs_get_device_phys_sectsize");
		निकास(1);
	}
	म_लिखो("Device %s has logical/physical sector size of %d/%d.\n",
	       argv[1], lsectsize, psectsize);
	fd = खोलो(argv[1], O_RDONLY);
	अगर (fd < 0) {
		लिखो_त्रुटि("open");
		निकास(1);
	}
	म_लिखो("The device's DIO alignment is %d\n",
	       ext2fs_get_dio_alignment(fd));
	बंद(fd);
	निकास(0);
}
