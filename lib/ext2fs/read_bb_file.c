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

#شامل "config.h"
#شامل <stdio.h>
#شامل <string.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#اگر_ختم
#شامل <fcntl.h>
#شامل <time.h>
#اگر HAVE_SYS_STAT_H
#شامل <sys/stat.h>
#اگر_ختم
#اگر HAVE_SYS_TYPES_H
#شامل <sys/types.h>
#اگر_ختم

#شامل "ext2_fs.h"
#شامل "ext2fs.h"

/*
 * Reads a list of bad blocks from  a FILE *
 */
errcode_t ext2fs_read_bb_FILE2(ext2_filsys fs, FILE *f,
			       ext2_badblocks_list *bb_list,
			       خالی *priv_ڈیٹا,
			       خالی (*invalid)(ext2_filsys fs,
					       blk_t blk,
					       حرف *badstr,
					       خالی *priv_ڈیٹا))
{
	errcode_t	retval;
	غیرعلامتی طویل طویل blockno;
	صحیح		گنتی;
	حرف		buf[128];

	اگر (fs)
		EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	اگر (!*bb_list) {
		retval = ext2fs_badblocks_list_create(bb_list, 10);
		اگر (retval)
			واپس retval;
	}

	جب (!feof (f)) {
		اگر (fgets(buf, حجم(buf), f) == تہی)
			توڑو;
		گنتی = لڑی_سے_پڑھو(buf, "%llu", &blockno);
		اگر (گنتی <= 0)
			جاری;
		/* Badblocks isn't going to be updated for 64bit */
		اگر (blockno >> 32)
			واپس EOVERFLOW;
		اگر (fs &&
		    ((blockno < fs->super->s_پہلا_ڈیٹا_بلاک) ||
		     (blockno >= ext2fs_بلاکیں_گنتی(fs->super)))) {
			اگر (invalid)
				(invalid)(fs, (blk64_t) blockno, buf, priv_ڈیٹا);
			جاری;
		}
		retval = ext2fs_badblocks_list_جوڑو(*bb_list, (blk64_t) blockno);
		اگر (retval)
			واپس retval;
	}
	واپس 0;
}

ساخت compat_struct {
	خالی (*invalid)(ext2_filsys, blk_t);
};

ساکن خالی call_compat_invalid(ext2_filsys fs, blk_t blk,
				حرف *badstr EXT2FS_ATTR((غیر_مستعمل)),
				خالی *priv_ڈیٹا)
{
	ساخت compat_struct *st;

	st = (ساخت compat_struct *) priv_ڈیٹا;
	اگر (st->invalid)
		(st->invalid)(fs, blk);
}


/*
 * Reads a list of bad blocks from  a FILE *
 */
errcode_t ext2fs_read_bb_FILE(ext2_filsys fs, FILE *f,
			      ext2_badblocks_list *bb_list,
			      خالی (*invalid)(ext2_filsys fs, blk_t blk))
{
	ساخت compat_struct st;

	st.invalid = invalid;

	واپس ext2fs_read_bb_FILE2(fs, f, bb_list, &st,
				    call_compat_invalid);
}


