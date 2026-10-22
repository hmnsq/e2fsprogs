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
स्थिर time_t चरमः_update;

संरचना ext2fs_प्रगतिः_ops ext2fs_numeric_प्रगतिः_ops = {
	.init		= ext2fs_numeric_प्रगतिः_init,
	.update		= ext2fs_numeric_प्रगतिः_update,
	.close		= ext2fs_numeric_प्रगतिः_close,
};

शून्य ext2fs_numeric_प्रगतिः_init(ext2_filsys fs,
				  संरचना ext2fs_numeric_प्रगतिः_struct * प्रगतिः,
				  ध्रुव वर्ण *label, __u64 max)
{
	/*
	 * The PRINT_PROGRESS flag turns on or off ALL
	 * progress-related messages, whereas the SKIP_PROGRESS
	 * environment variable prints the start and end messages but
	 * not the numeric countdown in the middle.
	 */
	यदि (!(fs->flags & EXT2_FLAG_PRINT_प्रगतिः))
		प्रत्यागम;

	स्मृतिं_पूरय(spaces, ' ', परिमाण(spaces)-1);
	spaces[परिमाण(spaces)-1] = 0;
	स्मृतिं_पूरय(backspaces, '\b', परिमाण(backspaces)-1);
	backspaces[परिमाण(backspaces)-1] = 0;

	स्मृतिं_पूरय(प्रगतिः, 0, परिमाण(*प्रगतिः));
	यदि (ext2fs_safe_getenv("E2FSPROGS_SKIP_PROGRESS"))
		प्रगतिः->skip_प्रगतिः++;


	/*
	 * Figure out how many digits we need (always at least one)
	 */
	प्रगतिः->max = max;
	प्रगतिः->log_max = ext2fs_log10_u64(max) + 1;

	यदि (label) {
		fputs(label, stdout);
		fflush(stdout);
	}
	चरमः_update = 0;
}

शून्य ext2fs_numeric_प्रगतिः_update(ext2_filsys fs,
				    संरचना ext2fs_numeric_प्रगतिः_struct * प्रगतिः,
				    __u64 val)
{
	time_t now;

	यदि (!(fs->flags & EXT2_FLAG_PRINT_प्रगतिः))
		प्रत्यागम;
	यदि (प्रगतिः->skip_प्रगतिः)
		प्रत्यागम;
	now = time(0);
	यदि (now == चरमः_update)
		प्रत्यागम;
	चरमः_update = now;

	प्रारूप_लिख("%*llu/%*llu", प्रगतिः->log_max, (अचिह्नित दीर्घ दीर्घ) val,
	       प्रगतिः->log_max, (अचिह्नित दीर्घ दीर्घ) प्रगतिः->max);
	fprintf(stdout, "%.*s", (2*प्रगतिः->log_max)+1, backspaces);
}

शून्य ext2fs_numeric_प्रगतिः_close(ext2_filsys fs,
				   संरचना ext2fs_numeric_प्रगतिः_struct * प्रगतिः,
				   ध्रुव वर्ण *सन्देशः_3)
{
	यदि (!(fs->flags & EXT2_FLAG_PRINT_प्रगतिः))
		प्रत्यागम;
	fprintf(stdout, "%.*s", (2*प्रगतिः->log_max)+1, spaces);
	fprintf(stdout, "%.*s", (2*प्रगतिः->log_max)+1, backspaces);
	यदि (सन्देशः_3)
		fputs(सन्देशः_3, stdout);
}
