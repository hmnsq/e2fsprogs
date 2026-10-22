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

صحیح اصلی(صحیح argc, نویسه **argv)
{
	اگر (اندازه(__u8) != 1) {
		printf("Sizeof(__u8) is %d should be 1\n",
		       (صحیح)اندازه(__u8));
		exit(1);
	}
	اگر (اندازه(__s8) != 1) {
		printf("Sizeof(_s8) is %d should be 1\n",
		       (صحیح)اندازه(__s8));
		exit(1);
	}
	اگر (اندازه(__u16) != 2) {
		printf("Sizeof(__u16) is %d should be 2\n",
		       (صحیح)اندازه(__u16));
		exit(1);
	}
	اگر (اندازه(__s16) != 2) {
		printf("Sizeof(__s16) is %d should be 2\n",
		       (صحیح)اندازه(__s16));
		exit(1);
	}
	اگر (اندازه(__u32) != 4) {
		printf("Sizeof(__u32) is %d should be 4\n",
		       (صحیح)اندازه(__u32));
		exit(1);
	}
	اگر (اندازه(__s32) != 4) {
		printf("Sizeof(__s32) is %d should be 4\n",
		       (صحیح)اندازه(__s32));
		exit(1);
	}
	اگر (اندازه(__u64) != 8) {
		printf("Sizeof(__u64) is %d should be 8\n",
		       (صحیح)اندازه(__u64));
		exit(1);
	}
	اگر (اندازه(__s64) != 8) {
		printf("Sizeof(__s64) is %d should be 8\n",
		       (صحیح)اندازه(__s64));
		exit(1);
	}
	printf("The ext2_types.h types are correct.\n");
	exit(0);
}

