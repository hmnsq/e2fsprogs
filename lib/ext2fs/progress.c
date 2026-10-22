/*
 * progress.c - Numeric progress meter
 *
 * Copyright (C) 1994, 1995, 1996, 1997, 1998, 1999, 2000, 2001, 2002,
 * 	2003, 2004, 2005 by Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Public
 * License.
 * %End-Header%
 */

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fsP.h"

#ಅನ್ತರ್ಭಾವ <time.h>

ಸ್ಥಿರ ಅಕ್ಷರ spaces[80], backspaces[80];
ಸ್ಥಿರ time_t ಕೊನೆಯ_update;

ರಚನೆ ext2fs_ಪ್ರಗತಿ_ops ext2fs_numeric_ಪ್ರಗತಿ_ops = {
	.init		= ext2fs_numeric_ಪ್ರಗತಿ_init,
	.update		= ext2fs_numeric_ಪ್ರಗತಿ_update,
	.close		= ext2fs_numeric_ಪ್ರಗತಿ_close,
};

ಶೂನ್ಯ ext2fs_numeric_ಪ್ರಗತಿ_init(ext2_filsys fs,
				  ರಚನೆ ext2fs_numeric_ಪ್ರಗತಿ_struct * ಪ್ರಗತಿ,
				  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *label, __u64 max)
{
	/*
	 * The PRINT_PROGRESS flag turns on or off ALL
	 * progress-related messages, whereas the SKIP_PROGRESS
	 * environment variable prints the start and end messages but
	 * not the numeric countdown in the middle.
	 */
	ಆದರೆ (!(fs->flags & EXT2_FLAG_PRINT_ಪ್ರಗತಿ))
		ಹಿಂತಿರುಗಿ;

	memset(spaces, ' ', ಗಾತ್ರ(spaces)-1);
	spaces[ಗಾತ್ರ(spaces)-1] = 0;
	memset(backspaces, '\b', ಗಾತ್ರ(backspaces)-1);
	backspaces[ಗಾತ್ರ(backspaces)-1] = 0;

	memset(ಪ್ರಗತಿ, 0, ಗಾತ್ರ(*ಪ್ರಗತಿ));
	ಆದರೆ (ext2fs_ಸುರಕ್ಷಿತ_getenv("E2FSPROGS_SKIP_PROGRESS"))
		ಪ್ರಗತಿ->skip_ಪ್ರಗತಿ++;


	/*
	 * Figure out how many digits we need (always at least one)
	 */
	ಪ್ರಗತಿ->max = max;
	ಪ್ರಗತಿ->log_max = ext2fs_log10_u64(max) + 1;

	ಆದರೆ (label) {
		fputs(label, stdout);
		fflush(stdout);
	}
	ಕೊನೆಯ_update = 0;
}

ಶೂನ್ಯ ext2fs_numeric_ಪ್ರಗತಿ_update(ext2_filsys fs,
				    ರಚನೆ ext2fs_numeric_ಪ್ರಗತಿ_struct * ಪ್ರಗತಿ,
				    __u64 val)
{
	time_t now;

	ಆದರೆ (!(fs->flags & EXT2_FLAG_PRINT_ಪ್ರಗತಿ))
		ಹಿಂತಿರುಗಿ;
	ಆದರೆ (ಪ್ರಗತಿ->skip_ಪ್ರಗತಿ)
		ಹಿಂತಿರುಗಿ;
	now = time(0);
	ಆದರೆ (now == ಕೊನೆಯ_update)
		ಹಿಂತಿರುಗಿ;
	ಕೊನೆಯ_update = now;

	ಬರೆ("%*llu/%*llu", ಪ್ರಗತಿ->log_max, (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ) val,
	       ಪ್ರಗತಿ->log_max, (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ) ಪ್ರಗತಿ->max);
	fprintf(stdout, "%.*s", (2*ಪ್ರಗತಿ->log_max)+1, backspaces);
}

ಶೂನ್ಯ ext2fs_numeric_ಪ್ರಗತಿ_close(ext2_filsys fs,
				   ರಚನೆ ext2fs_numeric_ಪ್ರಗತಿ_struct * ಪ್ರಗತಿ,
				   ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಸಂದೇಶ_3)
{
	ಆದರೆ (!(fs->flags & EXT2_FLAG_PRINT_ಪ್ರಗತಿ))
		ಹಿಂತಿರುಗಿ;
	fprintf(stdout, "%.*s", (2*ಪ್ರಗತಿ->log_max)+1, spaces);
	fprintf(stdout, "%.*s", (2*ಪ್ರಗತಿ->log_max)+1, backspaces);
	ಆದರೆ (ಸಂದೇಶ_3)
		fputs(ಸಂದೇಶ_3, stdout);
}
