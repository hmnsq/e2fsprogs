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

#समावेश "config.h"
#समावेश "ext2fs.h"
#समावेश "ext2fsP.h"

#समावेश <time.h>

अटल अक्षर spaces[80], backspaces[80];
अटल समय_प्रकार last_update;

काष्ठा ext2fs_progress_ops ext2fs_numeric_progress_ops = {
	.init		= ext2fs_numeric_progress_init,
	.update		= ext2fs_numeric_progress_update,
	.बंद		= ext2fs_numeric_progress_close,
};

व्योम ext2fs_numeric_progress_init(ext2_filsys fs,
				  काष्ठा ext2fs_numeric_progress_struct * progress,
				  स्थिर अक्षर *label, __u64 max)
{
	/*
	 * The PRINT_PROGRESS flag turns on or off ALL
	 * progress-related messages, whereas the SKIP_PROGRESS
	 * environment variable prints the start and end messages but
	 * not the numeric countdown in the middle.
	 */
	अगर (!(fs->flags & EXT2_FLAG_PRINT_PROGRESS))
		वापस;

	स_रखो(spaces, ' ', माप(spaces)-1);
	spaces[माप(spaces)-1] = 0;
	स_रखो(backspaces, '\b', माप(backspaces)-1);
	backspaces[माप(backspaces)-1] = 0;

	स_रखो(progress, 0, माप(*progress));
	अगर (ext2fs_safe_getenv("E2FSPROGS_SKIP_PROGRESS"))
		progress->skip_progress++;


	/*
	 * Figure out how many digits we need (always at least one)
	 */
	progress->max = max;
	progress->log_max = ext2fs_log10_u64(max) + 1;

	अगर (label) {
		ख_माला_दो(label, मानक_निकास);
		ख_साफ(मानक_निकास);
	}
	last_update = 0;
}

व्योम ext2fs_numeric_progress_update(ext2_filsys fs,
				    काष्ठा ext2fs_numeric_progress_struct * progress,
				    __u64 val)
{
	समय_प्रकार now;

	अगर (!(fs->flags & EXT2_FLAG_PRINT_PROGRESS))
		वापस;
	अगर (progress->skip_progress)
		वापस;
	now = समय(0);
	अगर (now == last_update)
		वापस;
	last_update = now;

	म_लिखो("%*llu/%*llu", progress->log_max, (अचिन्हित दीर्घ दीर्घ) val,
	       progress->log_max, (अचिन्हित दीर्घ दीर्घ) progress->max);
	ख_लिखो(मानक_निकास, "%.*s", (2*progress->log_max)+1, backspaces);
}

व्योम ext2fs_numeric_progress_close(ext2_filsys fs,
				   काष्ठा ext2fs_numeric_progress_struct * progress,
				   स्थिर अक्षर *message)
{
	अगर (!(fs->flags & EXT2_FLAG_PRINT_PROGRESS))
		वापस;
	ख_लिखो(मानक_निकास, "%.*s", (2*progress->log_max)+1, spaces);
	ख_लिखो(मानक_निकास, "%.*s", (2*progress->log_max)+1, backspaces);
	अगर (message)
		ख_माला_दो(message, मानक_निकास);
}
