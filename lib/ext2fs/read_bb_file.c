/*
 * read_bb_file.c --- read a list of bad blocks from a FILE *
 *
 * Copyright (C) 1994, 1995, 2000 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#پایان_اگر
#ضمن <fcntl.h>
#ضمن <time.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#پایان_اگر
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#پایان_اگر

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

/*
 * Reads a list of bad blocks from  a FILE *
 */
errcode_t ext2fs_read_bb_FILE2(ext2_filsys fs, FILE *f,
			       ext2_badblocks_list *bb_list,
			       فراغ *priv_داده,
			       فراغ (*invalid)(ext2_filsys fs,
					       blk_t blk,
					       حرف *badstr,
					       فراغ *priv_داده))
{
	errcode_t	retval;
	بیعلامت دراز دراز blockno;
	صحيح		شمار;
	حرف		buf[128];

	إذا (fs)
		EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	إذا (!*bb_list) {
		retval = ext2fs_badblocks_list_create(bb_list, 10);
		إذا (retval)
			أرجع retval;
	}

	طالما (!feof (f)) {
		إذا (fgets(buf, حجم(buf), f) == عدم)
			اكسر;
		شمار = از_رشته_بخوان(buf, "%llu", &blockno);
		إذا (شمار <= 0)
			استمر;
		/* Badblocks isn't going to be updated for 64bit */
		إذا (blockno >> 32)
			أرجع EOVERFLOW;
		إذا (fs &&
		    ((blockno < fs->super->s_اولی_داده_بلوک) ||
		     (blockno >= ext2fs_بلوکها_شمار(fs->super)))) {
			إذا (invalid)
				(invalid)(fs, (blk64_t) blockno, buf, priv_داده);
			استمر;
		}
		retval = ext2fs_badblocks_list_جمع_کن(*bb_list, (blk64_t) blockno);
		إذا (retval)
			أرجع retval;
	}
	أرجع 0;
}

بنية compat_struct {
	فراغ (*invalid)(ext2_filsys, blk_t);
};

ساكن فراغ call_compat_invalid(ext2_filsys fs, blk_t blk,
				حرف *badstr EXT2FS_ATTR((استفادهنشده)),
				فراغ *priv_داده)
{
	بنية compat_struct *st;

	st = (بنية compat_struct *) priv_داده;
	إذا (st->invalid)
		(st->invalid)(fs, blk);
}


/*
 * Reads a list of bad blocks from  a FILE *
 */
errcode_t ext2fs_read_bb_FILE(ext2_filsys fs, FILE *f,
			      ext2_badblocks_list *bb_list,
			      فراغ (*invalid)(ext2_filsys fs, blk_t blk))
{
	بنية compat_struct st;

	st.invalid = invalid;

	أرجع ext2fs_read_bb_FILE2(fs, f, bb_list, &st,
				    call_compat_invalid);
}


