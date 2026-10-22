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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <unistd.h>

#ಅನ್ತರ್ಭಾವ "ext2fs/ext2_types.h"

ಪೂರ್ಣಾಂಕ ಮುಖ್ಯ(ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
	ಆದರೆ (ಗಾತ್ರ(__u8) != 1) {
		ಬರೆ("Sizeof(__u8) is %d should be 1\n",
		       (ಪೂರ್ಣಾಂಕ)ಗಾತ್ರ(__u8));
		ನಿರ್ಗಮಿಸು(1);
	}
	ಆದರೆ (ಗಾತ್ರ(__s8) != 1) {
		ಬರೆ("Sizeof(_s8) is %d should be 1\n",
		       (ಪೂರ್ಣಾಂಕ)ಗಾತ್ರ(__s8));
		ನಿರ್ಗಮಿಸು(1);
	}
	ಆದರೆ (ಗಾತ್ರ(__u16) != 2) {
		ಬರೆ("Sizeof(__u16) is %d should be 2\n",
		       (ಪೂರ್ಣಾಂಕ)ಗಾತ್ರ(__u16));
		ನಿರ್ಗಮಿಸು(1);
	}
	ಆದರೆ (ಗಾತ್ರ(__s16) != 2) {
		ಬರೆ("Sizeof(__s16) is %d should be 2\n",
		       (ಪೂರ್ಣಾಂಕ)ಗಾತ್ರ(__s16));
		ನಿರ್ಗಮಿಸು(1);
	}
	ಆದರೆ (ಗಾತ್ರ(__u32) != 4) {
		ಬರೆ("Sizeof(__u32) is %d should be 4\n",
		       (ಪೂರ್ಣಾಂಕ)ಗಾತ್ರ(__u32));
		ನಿರ್ಗಮಿಸು(1);
	}
	ಆದರೆ (ಗಾತ್ರ(__s32) != 4) {
		ಬರೆ("Sizeof(__s32) is %d should be 4\n",
		       (ಪೂರ್ಣಾಂಕ)ಗಾತ್ರ(__s32));
		ನಿರ್ಗಮಿಸು(1);
	}
	ಆದರೆ (ಗಾತ್ರ(__u64) != 8) {
		ಬರೆ("Sizeof(__u64) is %d should be 8\n",
		       (ಪೂರ್ಣಾಂಕ)ಗಾತ್ರ(__u64));
		ನಿರ್ಗಮಿಸು(1);
	}
	ಆದರೆ (ಗಾತ್ರ(__s64) != 8) {
		ಬರೆ("Sizeof(__s64) is %d should be 8\n",
		       (ಪೂರ್ಣಾಂಕ)ಗಾತ್ರ(__s64));
		ನಿರ್ಗಮಿಸು(1);
	}
	ಬರೆ("The ext2_types.h types are correct.\n");
	ನಿರ್ಗಮಿಸು(0);
}

