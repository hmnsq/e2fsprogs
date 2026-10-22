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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಆದರೆ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <time.h>
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಆದರೆ HAVE_ERRNO_H
#ಅನ್ತರ್ಭಾವ <errno.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"

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

ಪೂರ್ಣಾಂಕ ಮುಖ್ಯ(ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
	ಪೂರ್ಣಾಂಕ	i;
	ಪೂರ್ಣಾಂಕ	errors = 0;

	ಬರೆ("Testing ext2fs_swab16\n");
	i=0;
	ಮಾಡು {
		ಬರೆ("swab16(0x%04x) = 0x%04x\n", test1[i],
		       ext2fs_swab16(test1[i]));
		ಆದರೆ (ext2fs_swab16(test1[i]) != test1[i+1]) {
			ಬರೆ("Error!!!   %04x != %04x\n",
			       ext2fs_swab16(test1[i]), test1[i+1]);
			errors++;
		}
		ಆದರೆ (ext2fs_swab16(test1[i+1]) != test1[i]) {
			ಬರೆ("Error!!!   %04x != %04x\n",
			       ext2fs_swab16(test1[i+1]), test1[i]);
			errors++;
		}
		i += 2;
	} ಆಗ (test1[i] != 0);

	ಬರೆ("Testing ext2fs_swab32\n");
	i = 0;
	ಮಾಡು {
		ಬರೆ("swab32(0x%08x) = 0x%08x\n", test2[i],
		       ext2fs_swab32(test2[i]));
		ಆದರೆ (ext2fs_swab32(test2[i]) != test2[i+1]) {
			ಬರೆ("Error!!!   %04x != %04x\n",
			       ext2fs_swab32(test2[i]), test2[i+1]);
			errors++;
		}
		ಆದರೆ (ext2fs_swab32(test2[i+1]) != test2[i]) {
			ಬರೆ("Error!!!   %04x != %04x\n",
			       ext2fs_swab32(test2[i+1]), test2[i]);
			errors++;
		}
		i += 2;
	} ಆಗ (test2[i] != 0);

	ಆದರೆ (!errors)
		ಬರೆ("No errors found in the byteswap implementation!\n");

	ಹಿಂತಿರುಗಿ errors;
}
