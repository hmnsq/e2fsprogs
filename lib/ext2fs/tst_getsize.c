/*
 * tst_getsize.c --- this function tests the getsize function
 *
 * Copyright (C) 1997 by Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಆದರೆ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <time.h>
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಆದರೆ HAVE_ERRNO_H
#ಅನ್ತರ್ಭಾವ <errno.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"

ಪೂರ್ಣಾಂಕ ಮುಖ್ಯ(ಪೂರ್ಣಾಂಕ argc, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *argv[])
{
	errcode_t	retval;
	blk64_t		ಖಂಡಗಳು;

	ಆದರೆ (argc < 2) {
		fprintf(stderr, "%s device\n", argv[0]);
		ನಿರ್ಗಮಿಸು(1);
	}
	ಕೂಡಿಸು_error_ಕೋಷ್ಟಕ(&et_ext2_error_ಕೋಷ್ಟಕ);
	retval = ext2fs_get_device_size2(argv[1], 1024, &ಖಂಡಗಳು);
	ಆದರೆ (retval) {
		com_err(argv[0], retval, "while getting device size");
		ನಿರ್ಗಮಿಸು(1);
	}
	ಬರೆ("%s is device has %llu blocks.\n", argv[1],
	       (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ) ಖಂಡಗಳು);
	ಹಿಂತಿರುಗಿ 0;
}
