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

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <stdlib.h>
#అన్తర్భావ <unistd.h>

#అన్తర్భావ "ext2fs/ext2_types.h"

పూర్ణాంకం ప్రధాన(పూర్ణాంకం argc, అక్షరం **argv)
{
	ఒకవేళ (పరిమాణం(__u8) != 1) {
		రాయి("Sizeof(__u8) is %d should be 1\n",
		       (పూర్ణాంకం)పరిమాణం(__u8));
		నిష్క్రమించు(1);
	}
	ఒకవేళ (పరిమాణం(__s8) != 1) {
		రాయి("Sizeof(_s8) is %d should be 1\n",
		       (పూర్ణాంకం)పరిమాణం(__s8));
		నిష్క్రమించు(1);
	}
	ఒకవేళ (పరిమాణం(__u16) != 2) {
		రాయి("Sizeof(__u16) is %d should be 2\n",
		       (పూర్ణాంకం)పరిమాణం(__u16));
		నిష్క్రమించు(1);
	}
	ఒకవేళ (పరిమాణం(__s16) != 2) {
		రాయి("Sizeof(__s16) is %d should be 2\n",
		       (పూర్ణాంకం)పరిమాణం(__s16));
		నిష్క్రమించు(1);
	}
	ఒకవేళ (పరిమాణం(__u32) != 4) {
		రాయి("Sizeof(__u32) is %d should be 4\n",
		       (పూర్ణాంకం)పరిమాణం(__u32));
		నిష్క్రమించు(1);
	}
	ఒకవేళ (పరిమాణం(__s32) != 4) {
		రాయి("Sizeof(__s32) is %d should be 4\n",
		       (పూర్ణాంకం)పరిమాణం(__s32));
		నిష్క్రమించు(1);
	}
	ఒకవేళ (పరిమాణం(__u64) != 8) {
		రాయి("Sizeof(__u64) is %d should be 8\n",
		       (పూర్ణాంకం)పరిమాణం(__u64));
		నిష్క్రమించు(1);
	}
	ఒకవేళ (పరిమాణం(__s64) != 8) {
		రాయి("Sizeof(__s64) is %d should be 8\n",
		       (పూర్ణాంకం)పరిమాణం(__s64));
		నిష్క్రమించు(1);
	}
	రాయి("The ext2_types.h types are correct.\n");
	నిష్క్రమించు(0);
}

