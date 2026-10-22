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

#அந்தர்பாவ "config.h"
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <stdlib.h>
#அந்தர்பாவ <unistd.h>

#அந்தர்பாவ "ext2fs/ext2_types.h"

முழு முதன்மை(முழு argc, எழுத்து **argv)
{
	எனில் (அளவு(__u8) != 1) {
		printf("Sizeof(__u8) is %d should be 1\n",
		       (முழு)அளவு(__u8));
		exit(1);
	}
	எனில் (அளவு(__s8) != 1) {
		printf("Sizeof(_s8) is %d should be 1\n",
		       (முழு)அளவு(__s8));
		exit(1);
	}
	எனில் (அளவு(__u16) != 2) {
		printf("Sizeof(__u16) is %d should be 2\n",
		       (முழு)அளவு(__u16));
		exit(1);
	}
	எனில் (அளவு(__s16) != 2) {
		printf("Sizeof(__s16) is %d should be 2\n",
		       (முழு)அளவு(__s16));
		exit(1);
	}
	எனில் (அளவு(__u32) != 4) {
		printf("Sizeof(__u32) is %d should be 4\n",
		       (முழு)அளவு(__u32));
		exit(1);
	}
	எனில் (அளவு(__s32) != 4) {
		printf("Sizeof(__s32) is %d should be 4\n",
		       (முழு)அளவு(__s32));
		exit(1);
	}
	எனில் (அளவு(__u64) != 8) {
		printf("Sizeof(__u64) is %d should be 8\n",
		       (முழு)அளவு(__u64));
		exit(1);
	}
	எனில் (அளவு(__s64) != 8) {
		printf("Sizeof(__s64) is %d should be 8\n",
		       (முழு)அளவு(__s64));
		exit(1);
	}
	printf("The ext2_types.h types are correct.\n");
	exit(0);
}

