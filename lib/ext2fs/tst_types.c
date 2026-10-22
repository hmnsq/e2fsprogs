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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fs/ext2_types.h"

ਪੂਰਨ ਮੁੱਖ(ਪੂਰਨ argc, ਅੱਖਰ **argv)
{
	ਜੇ (ਆਕਾਰ(__u8) != 1) {
		ਲਿਖੋ("Sizeof(__u8) is %d should be 1\n",
		       (ਪੂਰਨ)ਆਕਾਰ(__u8));
		ਬਾਹਰ_ਨਿਕਲੋ(1);
	}
	ਜੇ (ਆਕਾਰ(__s8) != 1) {
		ਲਿਖੋ("Sizeof(_s8) is %d should be 1\n",
		       (ਪੂਰਨ)ਆਕਾਰ(__s8));
		ਬਾਹਰ_ਨਿਕਲੋ(1);
	}
	ਜੇ (ਆਕਾਰ(__u16) != 2) {
		ਲਿਖੋ("Sizeof(__u16) is %d should be 2\n",
		       (ਪੂਰਨ)ਆਕਾਰ(__u16));
		ਬਾਹਰ_ਨਿਕਲੋ(1);
	}
	ਜੇ (ਆਕਾਰ(__s16) != 2) {
		ਲਿਖੋ("Sizeof(__s16) is %d should be 2\n",
		       (ਪੂਰਨ)ਆਕਾਰ(__s16));
		ਬਾਹਰ_ਨਿਕਲੋ(1);
	}
	ਜੇ (ਆਕਾਰ(__u32) != 4) {
		ਲਿਖੋ("Sizeof(__u32) is %d should be 4\n",
		       (ਪੂਰਨ)ਆਕਾਰ(__u32));
		ਬਾਹਰ_ਨਿਕਲੋ(1);
	}
	ਜੇ (ਆਕਾਰ(__s32) != 4) {
		ਲਿਖੋ("Sizeof(__s32) is %d should be 4\n",
		       (ਪੂਰਨ)ਆਕਾਰ(__s32));
		ਬਾਹਰ_ਨਿਕਲੋ(1);
	}
	ਜੇ (ਆਕਾਰ(__u64) != 8) {
		ਲਿਖੋ("Sizeof(__u64) is %d should be 8\n",
		       (ਪੂਰਨ)ਆਕਾਰ(__u64));
		ਬਾਹਰ_ਨਿਕਲੋ(1);
	}
	ਜੇ (ਆਕਾਰ(__s64) != 8) {
		ਲਿਖੋ("Sizeof(__s64) is %d should be 8\n",
		       (ਪੂਰਨ)ਆਕਾਰ(__s64));
		ਬਾਹਰ_ਨਿਕਲੋ(1);
	}
	ਲਿਖੋ("The ext2_types.h types are correct.\n");
	ਬਾਹਰ_ਨਿਕਲੋ(0);
}

