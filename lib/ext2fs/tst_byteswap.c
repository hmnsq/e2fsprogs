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

পূর্ণসংখ্যা প্রধান(পূর্ণসংখ্যা argc, অক্ষর **argv)
{
	পূর্ণসংখ্যা	i;
	পূর্ণসংখ্যা	errors = 0;

	printf("Testing ext2fs_swab16\n");
	i=0;
	করো {
		printf("swab16(0x%04x) = 0x%04x\n", test1[i],
		       ext2fs_swab16(test1[i]));
		যদি (ext2fs_swab16(test1[i]) != test1[i+1]) {
			printf("Error!!!   %04x != %04x\n",
			       ext2fs_swab16(test1[i]), test1[i+1]);
			errors++;
		}
		যদি (ext2fs_swab16(test1[i+1]) != test1[i]) {
			printf("Error!!!   %04x != %04x\n",
			       ext2fs_swab16(test1[i+1]), test1[i]);
			errors++;
		}
		i += 2;
	} যখন (test1[i] != 0);

	printf("Testing ext2fs_swab32\n");
	i = 0;
	করো {
		printf("swab32(0x%08x) = 0x%08x\n", test2[i],
		       ext2fs_swab32(test2[i]));
		যদি (ext2fs_swab32(test2[i]) != test2[i+1]) {
			printf("Error!!!   %04x != %04x\n",
			       ext2fs_swab32(test2[i]), test2[i+1]);
			errors++;
		}
		যদি (ext2fs_swab32(test2[i+1]) != test2[i]) {
			printf("Error!!!   %04x != %04x\n",
			       ext2fs_swab32(test2[i+1]), test2[i]);
			errors++;
		}
		i += 2;
	} যখন (test2[i] != 0);

	যদি (!errors)
		printf("No errors found in the byteswap implementation!\n");

	ফেরত errors;
}
