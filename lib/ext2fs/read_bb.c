/*
 * read_bb --- read the bad blocks inode
 *
 * Copyright (C) 1994 Theodore Ts'o.
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

ਬਣਤਰ read_bb_ਰਿਕਾਰਡ {
	ext2_badblocks_list	bb_list;
	errcode_t	err;
};

/*
 * Helper function for ext2fs_read_bb_inode()
 */
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __TURBOC__
 #ਹਦਾਇਤ argsused
#ਜੇ_ਅੰਤ
ਸਥਿਰ ਪੂਰਨ mark_bad_ਖੰਡ(ext2_filsys fs, blk_t *ਖੰਡ_nr,
			  e2_blkcnt_t blockcnt EXT2FS_ATTR((ਅਣਵਰਤਿਆ)),
			  blk_t ref_ਖੰਡ EXT2FS_ATTR((ਅਣਵਰਤਿਆ)),
			  ਪੂਰਨ ref_ਵਿੱਥ EXT2FS_ATTR((ਅਣਵਰਤਿਆ)),
			  ਖਾਲੀ *priv_ਅੰਕੜੇ)
{
	ਬਣਤਰ read_bb_ਰਿਕਾਰਡ *rb = (ਬਣਤਰ read_bb_ਰਿਕਾਰਡ *) priv_ਅੰਕੜੇ;

	ਜੇ (blockcnt < 0)
		ਵਾਪਸ 0;

	ਜੇ ((*ਖੰਡ_nr < fs->super->s_ਪਹਿਲਾ_ਅੰਕੜੇ_ਖੰਡ) ||
	    (*ਖੰਡ_nr >= ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(fs->super)))
		ਵਾਪਸ 0;	/* Ignore illegal blocks */

	rb->err = ext2fs_badblocks_list_ਜੋੜੋ(rb->bb_list, *ਖੰਡ_nr);
	ਜੇ (rb->err)
		ਵਾਪਸ ਖੰਡ_ABORT;
	ਵਾਪਸ 0;
}

/*
 * Reads the current bad blocks from the bad blocks inode.
 */
errcode_t ext2fs_read_bb_inode(ext2_filsys fs, ext2_badblocks_list *bb_list)
{
	errcode_t	retval;
	ਬਣਤਰ read_bb_ਰਿਕਾਰਡ rb;
	ਬਣਤਰ ext2_inode inode;
	blk_t	numblocks;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ਜੇ (!*bb_list) {
		retval = ext2fs_read_inode(fs, EXT2_BAD_INO, &inode);
		ਜੇ (retval)
			ਵਾਪਸ retval;
		numblocks = inode.i_ਖੰਡਾਂ;
		ਜੇ (!(ext2fs_has_feature_huge_file(fs->super) &&
		      (inode.i_flags & EXT4_HUGE_FILE_FL)))
			numblocks = numblocks / (fs->blocksize / 512);
		numblocks += 20;
		ਜੇ (numblocks < 50)
			numblocks = 50;
		ਜੇ (numblocks > 50000)
			numblocks = 500;
		retval = ext2fs_badblocks_list_create(bb_list, numblocks);
		ਜੇ (retval)
			ਵਾਪਸ retval;
	}

	rb.bb_list = *bb_list;
	rb.err = 0;
	retval = ext2fs_ਖੰਡ_iterate2(fs, EXT2_BAD_INO, ਖੰਡ_FLAG_READ_ONLY,
				       0, mark_bad_ਖੰਡ, &rb);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	ਵਾਪਸ rb.err;
}


