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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fsP.h"

#ਅਨ੍ਤਰ੍ਭਾਵ <time.h>

ਸਥਿਰ ਅੱਖਰ spaces[80], backspaces[80];
ਸਥਿਰ time_t ਆਖਰੀ_update;

ਬਣਤਰ ext2fs_ਤਰੱਕੀ_ops ext2fs_numeric_ਤਰੱਕੀ_ops = {
	.init		= ext2fs_numeric_ਤਰੱਕੀ_init,
	.update		= ext2fs_numeric_ਤਰੱਕੀ_update,
	.close		= ext2fs_numeric_ਤਰੱਕੀ_close,
};

ਖਾਲੀ ext2fs_numeric_ਤਰੱਕੀ_init(ext2_filsys fs,
				  ਬਣਤਰ ext2fs_numeric_ਤਰੱਕੀ_struct * ਤਰੱਕੀ,
				  ਅਚਲ ਅੱਖਰ *label, __u64 max)
{
	/*
	 * The PRINT_PROGRESS flag turns on or off ALL
	 * progress-related messages, whereas the SKIP_PROGRESS
	 * environment variable prints the start and end messages but
	 * not the numeric countdown in the middle.
	 */
	ਜੇ (!(fs->flags & EXT2_FLAG_PRINT_ਤਰੱਕੀ))
		ਵਾਪਸ;

	memset(spaces, ' ', ਆਕਾਰ(spaces)-1);
	spaces[ਆਕਾਰ(spaces)-1] = 0;
	memset(backspaces, '\b', ਆਕਾਰ(backspaces)-1);
	backspaces[ਆਕਾਰ(backspaces)-1] = 0;

	memset(ਤਰੱਕੀ, 0, ਆਕਾਰ(*ਤਰੱਕੀ));
	ਜੇ (ext2fs_ਸੁਰੱਖਿਅਤ_getenv("E2FSPROGS_SKIP_PROGRESS"))
		ਤਰੱਕੀ->skip_ਤਰੱਕੀ++;


	/*
	 * Figure out how many digits we need (always at least one)
	 */
	ਤਰੱਕੀ->max = max;
	ਤਰੱਕੀ->log_max = ext2fs_log10_u64(max) + 1;

	ਜੇ (label) {
		fputs(label, stdout);
		fflush(stdout);
	}
	ਆਖਰੀ_update = 0;
}

ਖਾਲੀ ext2fs_numeric_ਤਰੱਕੀ_update(ext2_filsys fs,
				    ਬਣਤਰ ext2fs_numeric_ਤਰੱਕੀ_struct * ਤਰੱਕੀ,
				    __u64 val)
{
	time_t now;

	ਜੇ (!(fs->flags & EXT2_FLAG_PRINT_ਤਰੱਕੀ))
		ਵਾਪਸ;
	ਜੇ (ਤਰੱਕੀ->skip_ਤਰੱਕੀ)
		ਵਾਪਸ;
	now = time(0);
	ਜੇ (now == ਆਖਰੀ_update)
		ਵਾਪਸ;
	ਆਖਰੀ_update = now;

	ਲਿਖੋ("%*llu/%*llu", ਤਰੱਕੀ->log_max, (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ) val,
	       ਤਰੱਕੀ->log_max, (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ) ਤਰੱਕੀ->max);
	fprintf(stdout, "%.*s", (2*ਤਰੱਕੀ->log_max)+1, backspaces);
}

ਖਾਲੀ ext2fs_numeric_ਤਰੱਕੀ_close(ext2_filsys fs,
				   ਬਣਤਰ ext2fs_numeric_ਤਰੱਕੀ_struct * ਤਰੱਕੀ,
				   ਅਚਲ ਅੱਖਰ *ਸੁਨੇਹਾ_3)
{
	ਜੇ (!(fs->flags & EXT2_FLAG_PRINT_ਤਰੱਕੀ))
		ਵਾਪਸ;
	fprintf(stdout, "%.*s", (2*ਤਰੱਕੀ->log_max)+1, spaces);
	fprintf(stdout, "%.*s", (2*ਤਰੱਕੀ->log_max)+1, backspaces);
	ਜੇ (ਸੁਨੇਹਾ_3)
		fputs(ਸੁਨੇਹਾ_3, stdout);
}
