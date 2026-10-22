/*
 * This testing program makes sure the ext2_types header file
 *
 * Copyright (C) 2006 by Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#شامل "config.h"
#شامل <stdio.h>
#شامل <stdlib.h>
#شامل <unistd.h>

#شامل "ext2fs/ext2_types.h"

صحیح مین(صحیح argc, حرف **argv)
{
	اگر (حجم(__u8) != 1) {
		لکھو("Sizeof(__u8) is %d should be 1\n",
		       (صحیح)حجم(__u8));
		باہر_نکلو(1);
	}
	اگر (حجم(__s8) != 1) {
		لکھو("Sizeof(_s8) is %d should be 1\n",
		       (صحیح)حجم(__s8));
		باہر_نکلو(1);
	}
	اگر (حجم(__u16) != 2) {
		لکھو("Sizeof(__u16) is %d should be 2\n",
		       (صحیح)حجم(__u16));
		باہر_نکلو(1);
	}
	اگر (حجم(__s16) != 2) {
		لکھو("Sizeof(__s16) is %d should be 2\n",
		       (صحیح)حجم(__s16));
		باہر_نکلو(1);
	}
	اگر (حجم(__u32) != 4) {
		لکھو("Sizeof(__u32) is %d should be 4\n",
		       (صحیح)حجم(__u32));
		باہر_نکلو(1);
	}
	اگر (حجم(__s32) != 4) {
		لکھو("Sizeof(__s32) is %d should be 4\n",
		       (صحیح)حجم(__s32));
		باہر_نکلو(1);
	}
	اگر (حجم(__u64) != 8) {
		لکھو("Sizeof(__u64) is %d should be 8\n",
		       (صحیح)حجم(__u64));
		باہر_نکلو(1);
	}
	اگر (حجم(__s64) != 8) {
		لکھو("Sizeof(__s64) is %d should be 8\n",
		       (صحیح)حجم(__s64));
		باہر_نکلو(1);
	}
	لکھو("The ext2_types.h types are correct.\n");
	باہر_نکلو(0);
}

