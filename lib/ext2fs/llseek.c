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

#अन्तर्भाव "config.h"
#जर HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#endif

#जर HAVE_ERRNO_H
#अन्तर्भाव <errno.h>
#endif
#जर HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#ifdef __MSDOS__
#अन्तर्भाव <io.h>
#endif
#अन्तर्भाव "et/com_err.h"
#अन्तर्भाव "ext2fs/ext2_io.h"

#ifdef __linux__

#अन्तर्भाव <sys/syscall.h>

स्थिर ext2_loff_t my_llseek (पूर्णांक fd, ext2_loff_t offset, पूर्णांक origin)
{
#जर SIZEOF_OFF_T >= 8
	परत lseek(fd, offset, origin);
#किंवाजर HAVE_LSEEK64_PROTOTYPE
	परत lseek64(fd, offset, origin);
#किंवाजर HAVE_LLSEEK_PROTOTYPE
	परत llseek(fd, offset, origin);
#किंवाजर defined(__NR__llseek)
	loff_t result;
	पूर्णांक retval;
	retval = syscall(__NR__llseek, fd,
		(अचिन्हित दीर्घ)(offset >> 32),
		(अचिन्हित दीर्घ)(offset & 0xffffffff),
		&result, origin);
	परत (retval == -1 ? retval : result);
#अन्यथा
	त्रुटि_सं = ENOSYS;
	परत -1;
#endif
}

ext2_loff_t ext2fs_llseek (पूर्णांक fd, ext2_loff_t offset, पूर्णांक origin)
{
	ext2_loff_t result;
	स्थिर पूर्णांक do_compat = 0;

	जर (do_compat)
		जा fallback;

	result = my_llseek (fd, offset, origin);
	जर (result == -1 && त्रुटि_सं == ENOSYS) {
		/*
		 * Just in case this code runs on top of an old kernel
		 * which does not support the llseek system call
		 */
		do_compat++;
	fallback:
		जर (offset < ((ext2_loff_t) 1 << ((आकार(off_t)*8) -1)))
			परत lseek(fd, (off_t) offset, origin);
		त्रुटि_सं = EINVAL;
		परत -1;
	}
	परत result;
}

#अन्यथा /* !linux */

#ifndef EINVAL
#define EINVAL EXT2_ET_INVALID_ARGUMENT
#endif

ext2_loff_t ext2fs_llseek (पूर्णांक fd, ext2_loff_t offset, पूर्णांक origin)
{
#जर defined(HAVE_LSEEK64) && defined(HAVE_LSEEK64_PROTOTYPE)
	परत lseek64 (fd, offset, origin);
#अन्यथा
	जर ((आकार(off_t) < आकार(ext2_loff_t)) &&
	    (offset >= ((ext2_loff_t) 1 << ((आकार(off_t)*8) -1)))) {
		त्रुटि_सं = EINVAL;
		परत -1;
	}
	परत lseek (fd, (off_t) offset, origin);
#endif
}

#endif 	/* linux */


