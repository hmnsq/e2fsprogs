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

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ଅନ୍ତର୍ଭାଵ <unistd.h>

#ଅନ୍ତର୍ଭାଵ "ext2fs/ext2_types.h"

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୁଖ୍ୟ(ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv)
{
	ଯଦି (ଆକାର(__u8) != 1) {
		printf("Sizeof(__u8) is %d should be 1\n",
		       (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)ଆକାର(__u8));
		exit(1);
	}
	ଯଦି (ଆକାର(__s8) != 1) {
		printf("Sizeof(_s8) is %d should be 1\n",
		       (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)ଆକାର(__s8));
		exit(1);
	}
	ଯଦି (ଆକାର(__u16) != 2) {
		printf("Sizeof(__u16) is %d should be 2\n",
		       (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)ଆକାର(__u16));
		exit(1);
	}
	ଯଦି (ଆକାର(__s16) != 2) {
		printf("Sizeof(__s16) is %d should be 2\n",
		       (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)ଆକାର(__s16));
		exit(1);
	}
	ଯଦି (ଆକାର(__u32) != 4) {
		printf("Sizeof(__u32) is %d should be 4\n",
		       (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)ଆକାର(__u32));
		exit(1);
	}
	ଯଦି (ଆକାର(__s32) != 4) {
		printf("Sizeof(__s32) is %d should be 4\n",
		       (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)ଆକାର(__s32));
		exit(1);
	}
	ଯଦି (ଆକାର(__u64) != 8) {
		printf("Sizeof(__u64) is %d should be 8\n",
		       (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)ଆକାର(__u64));
		exit(1);
	}
	ଯଦି (ଆକାର(__s64) != 8) {
		printf("Sizeof(__s64) is %d should be 8\n",
		       (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)ଆକାର(__s64));
		exit(1);
	}
	printf("The ext2_types.h types are correct.\n");
	exit(0);
}

