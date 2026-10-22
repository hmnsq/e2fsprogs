/*
 * native.c --- returns the ext2_flag for a native byte order
 *
 * Copyright (C) 1996 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"

ಪೂರ್ಣಾಂಕ ext2fs_native_flag(ಶೂನ್ಯ)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ಪದಗಳು_BIGENDIAN
	ಹಿಂತಿರುಗಿ EXT2_FLAG_ಅದಲುಬದಲು_ಬೈಟುಗಳು;
#ಇಲ್ಲದಿದ್ದರೆ
	ಹಿಂತಿರುಗಿ 0;
#ಆದರೆ_ಅಂತ್ಯ
}



