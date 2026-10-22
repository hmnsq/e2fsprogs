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

#నిర్వచితం_కాకపోతే LARGEFILE_ఆధారం
#నిర్వచించు LARGEFILE_ఆధారం
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే LARGEFILE64_ఆధారం
#నిర్వచించు LARGEFILE64_ఆధారం
#అయితే_ముగింపు

#అన్తర్భావ "config.h"
#ఒకవేళ HAVE_SYS_TYPES_H
#అన్తర్భావ <sys/types.h>
#అయితే_ముగింపు

#ఒకవేళ HAVE_ERRNO_H
#అన్తర్భావ <errno.h>
#అయితే_ముగింపు
#ఒకవేళ HAVE_UNISTD_H
#అన్తర్భావ <unistd.h>
#అయితే_ముగింపు
#నిర్వచితమైతే __MSDOS__
#అన్తర్భావ <io.h>
#అయితే_ముగింపు
#అన్తర్భావ "et/com_err.h"
#అన్తర్భావ "ext2fs/ext2_io.h"

#నిర్వచితమైతే __linux__

#అన్తర్భావ <sys/syscall.h>

స్థిర ext2_loff_t my_llseek (పూర్ణాంకం fd, ext2_loff_t స్థానభ్రంశం, పూర్ణాంకం origin)
{
#ఒకవేళ SIZEOF_OFF_T >= 8
	తిరిగి lseek(fd, స్థానభ్రంశం, origin);
#లేదావేళ HAVE_LSEEK64_PROTOTYPE
	తిరిగి lseek64(fd, స్థానభ్రంశం, origin);
#లేదావేళ HAVE_LLSEEK_PROTOTYPE
	తిరిగి llseek(fd, స్థానభ్రంశం, origin);
#లేదావేళ defined(__NR__llseek)
	loff_t ఫలితం;
	పూర్ణాంకం retval;
	retval = syscall(__NR__llseek, fd,
		(గుర్తులేని దీర్ఘ)(స్థానభ్రంశం >> 32),
		(గుర్తులేని దీర్ఘ)(స్థానభ్రంశం & 0xffffffff),
		&ఫలితం, origin);
	తిరిగి (retval == -1 ? retval : ఫలితం);
#లేకపోతే
	errno = ENOSYS;
	తిరిగి -1;
#అయితే_ముగింపు
}

ext2_loff_t ext2fs_llseek (పూర్ణాంకం fd, ext2_loff_t స్థానభ్రంశం, పూర్ణాంకం origin)
{
	ext2_loff_t ఫలితం;
	స్థిర పూర్ణాంకం do_compat = 0;

	ఒకవేళ (do_compat)
		వెళ్ళు fallback;

	ఫలితం = my_llseek (fd, స్థానభ్రంశం, origin);
	ఒకవేళ (ఫలితం == -1 && errno == ENOSYS) {
		/*
		 * Just in case this code runs on top of an old kernel
		 * which does not support the llseek system call
		 */
		do_compat++;
	fallback:
		ఒకవేళ (స్థానభ్రంశం < ((ext2_loff_t) 1 << ((పరిమాణం(off_t)*8) -1)))
			తిరిగి lseek(fd, (off_t) స్థానభ్రంశం, origin);
		errno = EINVAL;
		తిరిగి -1;
	}
	తిరిగి ఫలితం;
}

#లేకపోతే /* !linux */

#నిర్వచితం_కాకపోతే EINVAL
#నిర్వచించు EINVAL EXT2_ET_INVALID_వాదం
#అయితే_ముగింపు

ext2_loff_t ext2fs_llseek (పూర్ణాంకం fd, ext2_loff_t స్థానభ్రంశం, పూర్ణాంకం origin)
{
#ఒకవేళ defined(HAVE_LSEEK64) && defined(HAVE_LSEEK64_PROTOTYPE)
	తిరిగి lseek64 (fd, స్థానభ్రంశం, origin);
#లేకపోతే
	ఒకవేళ ((పరిమాణం(off_t) < పరిమాణం(ext2_loff_t)) &&
	    (స్థానభ్రంశం >= ((ext2_loff_t) 1 << ((పరిమాణం(off_t)*8) -1)))) {
		errno = EINVAL;
		తిరిగి -1;
	}
	తిరిగి lseek (fd, (off_t) స్థానభ్రంశం, origin);
#అయితే_ముగింపు
}

#అయితే_ముగింపు 	/* linux */


