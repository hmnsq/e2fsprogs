/*
 * inline.c --- Includes the inlined functions defined in the header
 * 	files as standalone functions, in case the application program
 * 	is compiled with inlining turned off.
 *
 * Copyright (C) 1993, 1994 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ XOPEN_ಮೂಲ
#ವ್ಯಾಖ್ಯಾನಿಸು XOPEN_ಮೂಲ 600	/* for posix_memalign() */
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಆದರೆ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <time.h>
#ಆದರೆ HAVE_SYS_STAT_H
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_SYS_TYPES_H
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_MALLOC_H
#ಅನ್ತರ್ಭಾವ <malloc.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ವ್ಯಾಖ್ಯಾನಿಸು INCLUDE_INLINE_FUNCS
#ಅನ್ತರ್ಭಾವ "ext2fs.h"

/*
 * We used to define this as an inline, but since we are now using
 * autoconf-defined #ifdef's, we need to export this as a
 * library-provided function exclusively.
 */
errcode_t ext2fs_get_memalign(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಗಾತ್ರ_2,
			      ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ align, ಶೂನ್ಯ *ptr)
{
	errcode_t retval = 0;
	ಶೂನ್ಯ **p = ptr;

	ಆದರೆ (align < 8)
		align = 8;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_POSIX_MEMALIGN
	retval = posix_memalign(p, align, ಗಾತ್ರ_2);
	ಆದರೆ (retval == ENOMEM)
		ಹಿಂತಿರುಗಿ EXT2_ET_NO_ಸ್ಮರಣೆ;
#ಇಲ್ಲದಿದ್ದರೆ  /* !HAVE_POSIX_MEMALIGN */
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_MEMALIGN
	*p = memalign(align, ಗಾತ್ರ_2);
	ಆದರೆ (*p == ಖಾಲಿ) {
		ಆದರೆ (errno)
			ಹಿಂತಿರುಗಿ errno;
		ಇಲ್ಲದಿದ್ದರೆ
			ಹಿಂತಿರುಗಿ EXT2_ET_NO_ಸ್ಮರಣೆ;
	}
#ಇಲ್ಲದಿದ್ದರೆ  /* !HAVE_MEMALIGN */
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_VALLOC
	ಆದರೆ (align > ಗಾತ್ರ(ದೀರ್ಘ ದೀರ್ಘ))
		*p = valloc(ಗಾತ್ರ_2);
	ಇಲ್ಲದಿದ್ದರೆ
#ಆದರೆ_ಅಂತ್ಯ
		*p = ಸ್ಮರಣೆ_ಕೊಡು(ಗಾತ್ರ_2);
	ಆದರೆ ((uintptr_t) *p & (align - 1)) {
		ಬಿಡುಗಡೆ(*p);
		*p = 0;
	}
	ಆದರೆ (*p == 0)
		ಹಿಂತಿರುಗಿ EXT2_ET_NO_ಸ್ಮರಣೆ;
#ಆದರೆ_ಅಂತ್ಯ	/* HAVE_MEMALIGN */
#ಆದರೆ_ಅಂತ್ಯ	/* HAVE_POSIX_MEMALIGN */
	ಹಿಂತಿರುಗಿ retval;
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ DEBUG
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ isaligned(ಶೂನ್ಯ *ptr, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ align)
{
	ಹಿಂತಿರುಗಿ (((ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ) ptr & (align - 1)) == 0);
}

ಸ್ಥಿರ errcode_t test_memalign(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ align)
{
	ಶೂನ್ಯ *ptr = 0;
	errcode_t retval;

	retval = ext2fs_get_memalign(32, align, &ptr);
	ಆದರೆ (!retval && !isaligned(ptr, align))
		retval = EINVAL;
	ಬಿಡುಗಡೆ(ptr);
	ಬರೆ("tst_memalign(%lu) is %s\n", align,
	       retval ? error_ಸಂದೇಶ(retval) : "OK");
	ಹಿಂತಿರುಗಿ retval;
}

ಪೂರ್ಣಾಂಕ ಮುಖ್ಯ(ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv)
{
	ಪೂರ್ಣಾಂಕ err = 0;

	ಆದರೆ (test_memalign(4))
		err++;
	ಆದರೆ (test_memalign(32))
		err++;
	ಆದರೆ (test_memalign(1024))
		err++;
	ಆದರೆ (test_memalign(4096))
		err++;
	ಹಿಂತಿರುಗಿ err;
}
#ಆದರೆ_ಅಂತ್ಯ
