/*
 * This testing program makes sure the byteswap functions work
 *
 * Copyright (C) 2000 by Theodore Ts'o.
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

__u16 test1[] = {
	0x0001, 0x0100,
	0x1234, 0x3412,
	0xff00, 0x00ff,
	0x4000, 0x0040,
	0xfeff, 0xfffe,
	0x0000, 0x0000
	};

__u32 test2[] = {
	0x00000001, 0x01000000,
	0x80000000, 0x00000080,
	0x12345678, 0x78563412,
	0xffff0000, 0x0000ffff,
	0x00ff0000, 0x0000ff00,
	0xff000000, 0x000000ff,
	0x00000000, 0x00000000
	};

पूर्णांक मुख्य(पूर्णांक argc, अक्षर **argv)
{
	पूर्णांक	i;
	पूर्णांक	errors = 0;

	म_लिखो("Testing ext2fs_swab16\n");
	i=0;
	करो {
		म_लिखो("swab16(0x%04x) = 0x%04x\n", test1[i],
		       ext2fs_swab16(test1[i]));
		अगर (ext2fs_swab16(test1[i]) != test1[i+1]) {
			म_लिखो("Error!!!   %04x != %04x\n",
			       ext2fs_swab16(test1[i]), test1[i+1]);
			errors++;
		}
		अगर (ext2fs_swab16(test1[i+1]) != test1[i]) {
			म_लिखो("Error!!!   %04x != %04x\n",
			       ext2fs_swab16(test1[i+1]), test1[i]);
			errors++;
		}
		i += 2;
	} जबतक (test1[i] != 0);

	म_लिखो("Testing ext2fs_swab32\n");
	i = 0;
	करो {
		म_लिखो("swab32(0x%08x) = 0x%08x\n", test2[i],
		       ext2fs_swab32(test2[i]));
		अगर (ext2fs_swab32(test2[i]) != test2[i+1]) {
			म_लिखो("Error!!!   %04x != %04x\n",
			       ext2fs_swab32(test2[i]), test2[i+1]);
			errors++;
		}
		अगर (ext2fs_swab32(test2[i+1]) != test2[i]) {
			म_लिखो("Error!!!   %04x != %04x\n",
			       ext2fs_swab32(test2[i+1]), test2[i]);
			errors++;
		}
		i += 2;
	} जबतक (test2[i] != 0);

	अगर (!errors)
		म_लिखो("No errors found in the byteswap implementation!\n");

	वापस errors;
}
