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

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <string.h>
#ఒకవేళ HAVE_UNISTD_H
#అన్తర్భావ <unistd.h>
#అయితే_ముగింపు
#అన్తర్భావ <fcntl.h>
#అన్తర్భావ <time.h>
#అన్తర్భావ <sys/stat.h>
#అన్తర్భావ <sys/types.h>
#ఒకవేళ HAVE_ERRNO_H
#అన్తర్భావ <errno.h>
#అయితే_ముగింపు

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fs.h"

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

పూర్ణాంకం ప్రధాన(పూర్ణాంకం argc, అక్షరం **argv)
{
	పూర్ణాంకం	i;
	పూర్ణాంకం	errors = 0;

	రాయి("Testing ext2fs_swab16\n");
	i=0;
	చేయి {
		రాయి("swab16(0x%04x) = 0x%04x\n", test1[i],
		       ext2fs_swab16(test1[i]));
		ఒకవేళ (ext2fs_swab16(test1[i]) != test1[i+1]) {
			రాయి("Error!!!   %04x != %04x\n",
			       ext2fs_swab16(test1[i]), test1[i+1]);
			errors++;
		}
		ఒకవేళ (ext2fs_swab16(test1[i+1]) != test1[i]) {
			రాయి("Error!!!   %04x != %04x\n",
			       ext2fs_swab16(test1[i+1]), test1[i]);
			errors++;
		}
		i += 2;
	} అయితే (test1[i] != 0);

	రాయి("Testing ext2fs_swab32\n");
	i = 0;
	చేయి {
		రాయి("swab32(0x%08x) = 0x%08x\n", test2[i],
		       ext2fs_swab32(test2[i]));
		ఒకవేళ (ext2fs_swab32(test2[i]) != test2[i+1]) {
			రాయి("Error!!!   %04x != %04x\n",
			       ext2fs_swab32(test2[i]), test2[i+1]);
			errors++;
		}
		ఒకవేళ (ext2fs_swab32(test2[i+1]) != test2[i]) {
			రాయి("Error!!!   %04x != %04x\n",
			       ext2fs_swab32(test2[i+1]), test2[i]);
			errors++;
		}
		i += 2;
	} అయితే (test2[i] != 0);

	ఒకవేళ (!errors)
		రాయి("No errors found in the byteswap implementation!\n");

	తిరిగి errors;
}
