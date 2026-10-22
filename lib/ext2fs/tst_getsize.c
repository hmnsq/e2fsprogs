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

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <string.h>
#ఒకవేళ HAVE_UNISTD_H
#అన్తర్భావ <unistd.h>
#అయితే_ముగింపు
#అన్తర్భావ <fcntl.h>
#అన్తర్భావ <time.h>
#అన్తర్భావ <sys/stat.h>
#అన్తర్భావ <sys/types.h>
#ఒకవేళ HAVE_ERRNO_H
#అన్తర్భావ <errno.h>
#అయితే_ముగింపు

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fs.h"

పూర్ణాంకం ప్రధాన(పూర్ణాంకం argc, స్థిరరాశి అక్షరం *argv[])
{
	errcode_t	retval;
	blk64_t		ఖండాలు;

	ఒకవేళ (argc < 2) {
		fprintf(stderr, "%s device\n", argv[0]);
		నిష్క్రమించు(1);
	}
	కూడు_error_పట్టిక(&et_ext2_error_పట్టిక);
	retval = ext2fs_get_device_size2(argv[1], 1024, &ఖండాలు);
	ఒకవేళ (retval) {
		com_err(argv[0], retval, "while getting device size");
		నిష్క్రమించు(1);
	}
	రాయి("%s is device has %llu blocks.\n", argv[1],
	       (గుర్తులేని దీర్ఘ దీర్ఘ) ఖండాలు);
	తిరిగి 0;
}
