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

ਪੂਰਨ ਮੁੱਖ(ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
	ਪੂਰਨ	i;
	ਪੂਰਨ	errors = 0;

	ਲਿਖੋ("Testing ext2fs_swab16\n");
	i=0;
	ਕਰੋ {
		ਲਿਖੋ("swab16(0x%04x) = 0x%04x\n", test1[i],
		       ext2fs_swab16(test1[i]));
		ਜੇ (ext2fs_swab16(test1[i]) != test1[i+1]) {
			ਲਿਖੋ("Error!!!   %04x != %04x\n",
			       ext2fs_swab16(test1[i]), test1[i+1]);
			errors++;
		}
		ਜੇ (ext2fs_swab16(test1[i+1]) != test1[i]) {
			ਲਿਖੋ("Error!!!   %04x != %04x\n",
			       ext2fs_swab16(test1[i+1]), test1[i]);
			errors++;
		}
		i += 2;
	} ਜਦੋਂ (test1[i] != 0);

	ਲਿਖੋ("Testing ext2fs_swab32\n");
	i = 0;
	ਕਰੋ {
		ਲਿਖੋ("swab32(0x%08x) = 0x%08x\n", test2[i],
		       ext2fs_swab32(test2[i]));
		ਜੇ (ext2fs_swab32(test2[i]) != test2[i+1]) {
			ਲਿਖੋ("Error!!!   %04x != %04x\n",
			       ext2fs_swab32(test2[i]), test2[i+1]);
			errors++;
		}
		ਜੇ (ext2fs_swab32(test2[i+1]) != test2[i]) {
			ਲਿਖੋ("Error!!!   %04x != %04x\n",
			       ext2fs_swab32(test2[i+1]), test2[i]);
			errors++;
		}
		i += 2;
	} ਜਦੋਂ (test2[i] != 0);

	ਜੇ (!errors)
		ਲਿਖੋ("No errors found in the byteswap implementation!\n");

	ਵਾਪਸ errors;
}
