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

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <unistd.h>

#अन्तर्भाव "ext2fs/ext2_types.h"

पूर्णाङ्क main(पूर्णाङ्क argc, वर्ण **argv)
{
	यदि (परिमाण(__u8) != 1) {
		म_लिखो("Sizeof(__u8) is %d should be 1\n",
		       (पूर्णाङ्क)परिमाण(__u8));
		निकास(1);
	}
	यदि (परिमाण(__s8) != 1) {
		म_लिखो("Sizeof(_s8) is %d should be 1\n",
		       (पूर्णाङ्क)परिमाण(__s8));
		निकास(1);
	}
	यदि (परिमाण(__u16) != 2) {
		म_लिखो("Sizeof(__u16) is %d should be 2\n",
		       (पूर्णाङ्क)परिमाण(__u16));
		निकास(1);
	}
	यदि (परिमाण(__s16) != 2) {
		म_लिखो("Sizeof(__s16) is %d should be 2\n",
		       (पूर्णाङ्क)परिमाण(__s16));
		निकास(1);
	}
	यदि (परिमाण(__u32) != 4) {
		म_लिखो("Sizeof(__u32) is %d should be 4\n",
		       (पूर्णाङ्क)परिमाण(__u32));
		निकास(1);
	}
	यदि (परिमाण(__s32) != 4) {
		म_लिखो("Sizeof(__s32) is %d should be 4\n",
		       (पूर्णाङ्क)परिमाण(__s32));
		निकास(1);
	}
	यदि (परिमाण(__u64) != 8) {
		म_लिखो("Sizeof(__u64) is %d should be 8\n",
		       (पूर्णाङ्क)परिमाण(__u64));
		निकास(1);
	}
	यदि (परिमाण(__s64) != 8) {
		म_लिखो("Sizeof(__s64) is %d should be 8\n",
		       (पूर्णाङ्क)परिमाण(__s64));
		निकास(1);
	}
	म_लिखो("The ext2_types.h types are correct.\n");
	निकास(0);
}

