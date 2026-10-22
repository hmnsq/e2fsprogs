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

#अन्तर्भाव "config.h"
#अन्तर्भाव "ext2fs.h"
#अन्तर्भाव "ext2fsP.h"

#अन्तर्भाव <time.h>

स्थिर वर्ण spaces[80], backspaces[80];
स्थिर समय_प्रकार last_update;

संरचना ext2fs_progress_ops ext2fs_numeric_progress_ops = {
	.init		= ext2fs_numeric_progress_init,
	.update		= ext2fs_numeric_progress_update,
	.बंद		= ext2fs_numeric_progress_close,
};

शून्य ext2fs_numeric_progress_init(ext2_filsys fs,
				  संरचना ext2fs_numeric_progress_struct * progress,
				  ध्रुव वर्ण *label, __u64 max)
{
	/*
	 * The PRINT_PROGRESS flag turns on or off ALL
	 * progress-related messages, whereas the SKIP_PROGRESS
	 * environment variable prints the start and end messages but
	 * not the numeric countdown in the middle.
	 */
	यदि (!(fs->flags & EXT2_FLAG_PRINT_PROGRESS))
		प्रत्यागम;

	स_रखो(spaces, ' ', परिमाण(spaces)-1);
	spaces[परिमाण(spaces)-1] = 0;
	स_रखो(backspaces, '\b', परिमाण(backspaces)-1);
	backspaces[परिमाण(backspaces)-1] = 0;

	स_रखो(progress, 0, परिमाण(*progress));
	यदि (ext2fs_safe_getenv("E2FSPROGS_SKIP_PROGRESS"))
		progress->skip_progress++;


	/*
	 * Figure out how many digits we need (always at least one)
	 */
	progress->max = max;
	progress->log_max = ext2fs_log10_u64(max) + 1;

	यदि (label) {
		ख_माला_दो(label, मानक_निकास);
		ख_साफ(मानक_निकास);
	}
	last_update = 0;
}

शून्य ext2fs_numeric_progress_update(ext2_filsys fs,
				    संरचना ext2fs_numeric_progress_struct * progress,
				    __u64 val)
{
	समय_प्रकार now;

	यदि (!(fs->flags & EXT2_FLAG_PRINT_PROGRESS))
		प्रत्यागम;
	यदि (progress->skip_progress)
		प्रत्यागम;
	now = समय(0);
	यदि (now == last_update)
		प्रत्यागम;
	last_update = now;

	म_लिखो("%*llu/%*llu", progress->log_max, (अचिन्हित दीर्घ दीर्घ) val,
	       progress->log_max, (अचिन्हित दीर्घ दीर्घ) progress->max);
	ख_लिखो(मानक_निकास, "%.*s", (2*progress->log_max)+1, backspaces);
}

शून्य ext2fs_numeric_progress_close(ext2_filsys fs,
				   संरचना ext2fs_numeric_progress_struct * progress,
				   ध्रुव वर्ण *message)
{
	यदि (!(fs->flags & EXT2_FLAG_PRINT_PROGRESS))
		प्रत्यागम;
	ख_लिखो(मानक_निकास, "%.*s", (2*progress->log_max)+1, spaces);
	ख_लिखो(मानक_निकास, "%.*s", (2*progress->log_max)+1, backspaces);
	यदि (message)
		ख_माला_दो(message, मानक_निकास);
}
