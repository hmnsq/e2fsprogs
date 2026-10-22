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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਜੇ HAVE_UNISTD_H
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <fcntl.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <time.h>
#ਜੇ HAVE_SYS_STAT_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_SYS_TYPES_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fs.h"

/*
 * Reads a list of bad blocks from  a FILE *
 */
errcode_t ext2fs_read_bb_FILE2(ext2_filsys fs, FILE *f,
			       ext2_badblocks_list *bb_list,
			       ਖਾਲੀ *priv_ਅੰਕੜੇ,
			       ਖਾਲੀ (*invalid)(ext2_filsys fs,
					       blk_t blk,
					       ਅੱਖਰ *badstr,
					       ਖਾਲੀ *priv_ਅੰਕੜੇ))
{
	errcode_t	retval;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ blockno;
	ਪੂਰਨ		ਗਿਣਤੀ_2;
	ਅੱਖਰ		buf[128];

	ਜੇ (fs)
		EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ਜੇ (!*bb_list) {
		retval = ext2fs_badblocks_list_create(bb_list, 10);
		ਜੇ (retval)
			ਵਾਪਸ retval;
	}

	ਜਦੋਂ (!feof (f)) {
		ਜੇ (fgets(buf, ਆਕਾਰ(buf), f) == ਨਿਰਾ)
			ਰੋਕੋ;
		ਗਿਣਤੀ_2 = ਲੜੀ_ਤੋਂ_ਪੜ੍ਹੋ(buf, "%llu", &blockno);
		ਜੇ (ਗਿਣਤੀ_2 <= 0)
			ਜਾਰੀ;
		/* Badblocks isn't going to be updated for 64bit */
		ਜੇ (blockno >> 32)
			ਵਾਪਸ EOVERFLOW;
		ਜੇ (fs &&
		    ((blockno < fs->super->s_ਪਹਿਲਾ_ਅੰਕੜੇ_ਖੰਡ) ||
		     (blockno >= ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(fs->super)))) {
			ਜੇ (invalid)
				(invalid)(fs, (blk64_t) blockno, buf, priv_ਅੰਕੜੇ);
			ਜਾਰੀ;
		}
		retval = ext2fs_badblocks_list_ਜੋੜੋ(*bb_list, (blk64_t) blockno);
		ਜੇ (retval)
			ਵਾਪਸ retval;
	}
	ਵਾਪਸ 0;
}

ਬਣਤਰ compat_struct {
	ਖਾਲੀ (*invalid)(ext2_filsys, blk_t);
};

ਸਥਿਰ ਖਾਲੀ call_compat_invalid(ext2_filsys fs, blk_t blk,
				ਅੱਖਰ *badstr EXT2FS_ATTR((ਅਣਵਰਤਿਆ)),
				ਖਾਲੀ *priv_ਅੰਕੜੇ)
{
	ਬਣਤਰ compat_struct *st;

	st = (ਬਣਤਰ compat_struct *) priv_ਅੰਕੜੇ;
	ਜੇ (st->invalid)
		(st->invalid)(fs, blk);
}


/*
 * Reads a list of bad blocks from  a FILE *
 */
errcode_t ext2fs_read_bb_FILE(ext2_filsys fs, FILE *f,
			      ext2_badblocks_list *bb_list,
			      ਖਾਲੀ (*invalid)(ext2_filsys fs, blk_t blk))
{
	ਬਣਤਰ compat_struct st;

	st.invalid = invalid;

	ਵਾਪਸ ext2fs_read_bb_FILE2(fs, f, bb_list, &st,
				    call_compat_invalid);
}


