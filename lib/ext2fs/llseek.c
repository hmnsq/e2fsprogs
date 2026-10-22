/*
 * llseek.c -- stub calling the llseek system call
 *
 * Copyright (C) 1994, 1995, 1996, 1997 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#ifndef _LARGEFILE_SOURCE
#define _LARGEFILE_SOURCE
#endif
#ifndef _LARGEFILE64_SOURCE
#define _LARGEFILE64_SOURCE
#endif

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଯଦି HAVE_SYS_TYPES_H
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#endif

#ଯଦି HAVE_ERRNO_H
#ଅନ୍ତର୍ଭାଵ <errno.h>
#endif
#ଯଦି HAVE_UNISTD_H
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#endif
#ifdef __MSDOS__
#ଅନ୍ତର୍ଭାଵ <io.h>
#endif
#ଅନ୍ତର୍ଭାଵ "et/com_err.h"
#ଅନ୍ତର୍ଭାଵ "ext2fs/ext2_io.h"

#ifdef __linux__

#ଅନ୍ତର୍ଭାଵ <sys/syscall.h>

ସ୍ଥିର ext2_loff_t my_llseek (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ext2_loff_t offset, ପୂର୍ଣ୍ଣସଂଖ୍ୟା origin)
{
#ଯଦି SIZEOF_OFF_T >= 8
	ଫେରନ୍ତୁ lseek(fd, offset, origin);
#ନଚେତଯଦି HAVE_LSEEK64_PROTOTYPE
	ଫେରନ୍ତୁ lseek64(fd, offset, origin);
#ନଚେତଯଦି HAVE_LLSEEK_PROTOTYPE
	ଫେରନ୍ତୁ llseek(fd, offset, origin);
#ନଚେତଯଦି defined(__NR__llseek)
	loff_t result;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା retval;
	retval = syscall(__NR__llseek, fd,
		(unsigned long)(offset >> 32),
		(unsigned long)(offset & 0xffffffff),
		&result, origin);
	ଫେରନ୍ତୁ (retval == -1 ? retval : result);
#ଅନ୍ୟଥା
	errno = ENOSYS;
	ଫେରନ୍ତୁ -1;
#endif
}

ext2_loff_t ext2fs_llseek (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ext2_loff_t offset, ପୂର୍ଣ୍ଣସଂଖ୍ୟା origin)
{
	ext2_loff_t result;
	ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା do_compat = 0;

	ଯଦି (do_compat)
		ଯାଅ fallback;

	result = my_llseek (fd, offset, origin);
	ଯଦି (result == -1 && errno == ENOSYS) {
		/*
		 * Just in case this code runs on top of an old kernel
		 * which does not support the llseek system call
		 */
		do_compat++;
	fallback:
		ଯଦି (offset < ((ext2_loff_t) 1 << ((ଆକାର(off_t)*8) -1)))
			ଫେରନ୍ତୁ lseek(fd, (off_t) offset, origin);
		errno = EINVAL;
		ଫେରନ୍ତୁ -1;
	}
	ଫେରନ୍ତୁ result;
}

#ଅନ୍ୟଥା /* !linux */

#ifndef EINVAL
#define EINVAL EXT2_ET_INVALID_ARGUMENT
#endif

ext2_loff_t ext2fs_llseek (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ext2_loff_t offset, ପୂର୍ଣ୍ଣସଂଖ୍ୟା origin)
{
#ଯଦି defined(HAVE_LSEEK64) && defined(HAVE_LSEEK64_PROTOTYPE)
	ଫେରନ୍ତୁ lseek64 (fd, offset, origin);
#ଅନ୍ୟଥା
	ଯଦି ((ଆକାର(off_t) < ଆକାର(ext2_loff_t)) &&
	    (offset >= ((ext2_loff_t) 1 << ((ଆକାର(off_t)*8) -1)))) {
		errno = EINVAL;
		ଫେରନ୍ତୁ -1;
	}
	ଫେରନ୍ତୁ lseek (fd, (off_t) offset, origin);
#endif
}

#endif 	/* linux */


