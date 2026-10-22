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

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#جيڪڏهن_پورو
#ضمن <fcntl.h>
#ضمن <time.h>
#ضمن <sys/stat.h>
#ضمن <sys/types.h>
#إذا HAVE_ERRNO_H
#ضمن <errno.h>
#جيڪڏهن_پورو

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

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

صحيح مکيه(صحيح argc, حرف **argv)
{
	صحيح	i;
	صحيح	errors = 0;

	لکو("Testing ext2fs_swab16\n");
	i=0;
	افعل {
		لکو("swab16(0x%04x) = 0x%04x\n", test1[i],
		       ext2fs_swab16(test1[i]));
		إذا (ext2fs_swab16(test1[i]) != test1[i+1]) {
			لکو("Error!!!   %04x != %04x\n",
			       ext2fs_swab16(test1[i]), test1[i+1]);
			errors++;
		}
		إذا (ext2fs_swab16(test1[i+1]) != test1[i]) {
			لکو("Error!!!   %04x != %04x\n",
			       ext2fs_swab16(test1[i+1]), test1[i]);
			errors++;
		}
		i += 2;
	} طالما (test1[i] != 0);

	لکو("Testing ext2fs_swab32\n");
	i = 0;
	افعل {
		لکو("swab32(0x%08x) = 0x%08x\n", test2[i],
		       ext2fs_swab32(test2[i]));
		إذا (ext2fs_swab32(test2[i]) != test2[i+1]) {
			لکو("Error!!!   %04x != %04x\n",
			       ext2fs_swab32(test2[i]), test2[i+1]);
			errors++;
		}
		إذا (ext2fs_swab32(test2[i+1]) != test2[i]) {
			لکو("Error!!!   %04x != %04x\n",
			       ext2fs_swab32(test2[i+1]), test2[i]);
			errors++;
		}
		i += 2;
	} طالما (test2[i] != 0);

	إذا (!errors)
		لکو("No errors found in the byteswap implementation!\n");

	أرجع errors;
}
