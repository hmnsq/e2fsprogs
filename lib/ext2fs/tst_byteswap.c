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

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଯଦି HAVE_UNISTD_H
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#endif
#ଅନ୍ତର୍ଭାଵ <fcntl.h>
#ଅନ୍ତର୍ଭାଵ <time.h>
#ଅନ୍ତର୍ଭାଵ <sys/stat.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଯଦି HAVE_ERRNO_H
#ଅନ୍ତର୍ଭାଵ <errno.h>
#endif

#ଅନ୍ତର୍ଭାଵ "ext2_fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fs.h"

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

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୁଖ୍ୟ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା	i;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା	errors = 0;

	printf("Testing ext2fs_swab16\n");
	i=0;
	କର {
		printf("swab16(0x%04x) = 0x%04x\n", test1[i],
		       ext2fs_swab16(test1[i]));
		ଯଦି (ext2fs_swab16(test1[i]) != test1[i+1]) {
			printf("Error!!!   %04x != %04x\n",
			       ext2fs_swab16(test1[i]), test1[i+1]);
			errors++;
		}
		ଯଦି (ext2fs_swab16(test1[i+1]) != test1[i]) {
			printf("Error!!!   %04x != %04x\n",
			       ext2fs_swab16(test1[i+1]), test1[i]);
			errors++;
		}
		i += 2;
	} ଯେତେବେଳେ (test1[i] != 0);

	printf("Testing ext2fs_swab32\n");
	i = 0;
	କର {
		printf("swab32(0x%08x) = 0x%08x\n", test2[i],
		       ext2fs_swab32(test2[i]));
		ଯଦି (ext2fs_swab32(test2[i]) != test2[i+1]) {
			printf("Error!!!   %04x != %04x\n",
			       ext2fs_swab32(test2[i]), test2[i+1]);
			errors++;
		}
		ଯଦି (ext2fs_swab32(test2[i+1]) != test2[i]) {
			printf("Error!!!   %04x != %04x\n",
			       ext2fs_swab32(test2[i+1]), test2[i]);
			errors++;
		}
		i += 2;
	} ଯେତେବେଳେ (test2[i] != 0);

	ଯଦି (!errors)
		printf("No errors found in the byteswap implementation!\n");

	ଫେରନ୍ତୁ errors;
}
