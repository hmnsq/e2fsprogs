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

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <stdlib.h>
#ضمن <unistd.h>

#ضمن "ext2fs/ext2_types.h"

صحيح اصلي(صحيح argc, حرف **argv)
{
	إذا (حجم(__u8) != 1) {
		چاپ_کړه("Sizeof(__u8) is %d should be 1\n",
		       (صحيح)حجم(__u8));
		وتل(1);
	}
	إذا (حجم(__s8) != 1) {
		چاپ_کړه("Sizeof(_s8) is %d should be 1\n",
		       (صحيح)حجم(__s8));
		وتل(1);
	}
	إذا (حجم(__u16) != 2) {
		چاپ_کړه("Sizeof(__u16) is %d should be 2\n",
		       (صحيح)حجم(__u16));
		وتل(1);
	}
	إذا (حجم(__s16) != 2) {
		چاپ_کړه("Sizeof(__s16) is %d should be 2\n",
		       (صحيح)حجم(__s16));
		وتل(1);
	}
	إذا (حجم(__u32) != 4) {
		چاپ_کړه("Sizeof(__u32) is %d should be 4\n",
		       (صحيح)حجم(__u32));
		وتل(1);
	}
	إذا (حجم(__s32) != 4) {
		چاپ_کړه("Sizeof(__s32) is %d should be 4\n",
		       (صحيح)حجم(__s32));
		وتل(1);
	}
	إذا (حجم(__u64) != 8) {
		چاپ_کړه("Sizeof(__u64) is %d should be 8\n",
		       (صحيح)حجم(__u64));
		وتل(1);
	}
	إذا (حجم(__s64) != 8) {
		چاپ_کړه("Sizeof(__s64) is %d should be 8\n",
		       (صحيح)حجم(__s64));
		وتل(1);
	}
	چاپ_کړه("The ext2_types.h types are correct.\n");
	وتل(0);
}

