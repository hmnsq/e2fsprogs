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

#समावेश "config.h"
#समावेश <stdio.h>
#समावेश <stdlib.h>
#समावेश <unistd.h>

#समावेश "ext2fs/ext2_types.h"

पूर्णांक मुख्य(पूर्णांक argc, अक्षर **argv)
{
	अगर (माप(__u8) != 1) {
		म_लिखो("Sizeof(__u8) is %d should be 1\n",
		       (पूर्णांक)माप(__u8));
		निकास(1);
	}
	अगर (माप(__s8) != 1) {
		म_लिखो("Sizeof(_s8) is %d should be 1\n",
		       (पूर्णांक)माप(__s8));
		निकास(1);
	}
	अगर (माप(__u16) != 2) {
		म_लिखो("Sizeof(__u16) is %d should be 2\n",
		       (पूर्णांक)माप(__u16));
		निकास(1);
	}
	अगर (माप(__s16) != 2) {
		म_लिखो("Sizeof(__s16) is %d should be 2\n",
		       (पूर्णांक)माप(__s16));
		निकास(1);
	}
	अगर (माप(__u32) != 4) {
		म_लिखो("Sizeof(__u32) is %d should be 4\n",
		       (पूर्णांक)माप(__u32));
		निकास(1);
	}
	अगर (माप(__s32) != 4) {
		म_लिखो("Sizeof(__s32) is %d should be 4\n",
		       (पूर्णांक)माप(__s32));
		निकास(1);
	}
	अगर (माप(__u64) != 8) {
		म_लिखो("Sizeof(__u64) is %d should be 8\n",
		       (पूर्णांक)माप(__u64));
		निकास(1);
	}
	अगर (माप(__s64) != 8) {
		म_लिखो("Sizeof(__s64) is %d should be 8\n",
		       (पूर्णांक)माप(__s64));
		निकास(1);
	}
	म_लिखो("The ext2_types.h types are correct.\n");
	निकास(0);
}

