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

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#جيڪڏهن_پورو
#ضمن <fcntl.h>
#ضمن <time.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#جيڪڏهن_پورو
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#جيڪڏهن_پورو

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

بنية read_bb_رڪارڊ {
	ext2_badblocks_list	bb_list;
	errcode_t	err;
};

/*
 * Helper function for ext2fs_read_bb_inode()
 */
#جيڪڏهن_وصف_ٿيل __TURBOC__
 #هدايت argsused
#جيڪڏهن_پورو
ساكن صحيح mark_bad_بلاڪ(ext2_filsys fs, blk_t *بلاڪ_nr,
			  e2_blkcnt_t blockcnt EXT2FS_ATTR((اڻ_استعمال_ٿيل)),
			  blk_t ref_بلاڪ EXT2FS_ATTR((اڻ_استعمال_ٿيل)),
			  صحيح ref_وٿي EXT2FS_ATTR((اڻ_استعمال_ٿيل)),
			  فراغ *priv_معلومات)
{
	بنية read_bb_رڪارڊ *rb = (بنية read_bb_رڪارڊ *) priv_معلومات;

	إذا (blockcnt < 0)
		أرجع 0;

	إذا ((*بلاڪ_nr < fs->super->s_پهريون_معلومات_بلاڪ) ||
	    (*بلاڪ_nr >= ext2fs_بلاڪون_ڳڻپ(fs->super)))
		أرجع 0;	/* Ignore illegal blocks */

	rb->err = ext2fs_badblocks_list_جوڙيو(rb->bb_list, *بلاڪ_nr);
	إذا (rb->err)
		أرجع بلاڪ_ABORT;
	أرجع 0;
}

/*
 * Reads the current bad blocks from the bad blocks inode.
 */
errcode_t ext2fs_read_bb_inode(ext2_filsys fs, ext2_badblocks_list *bb_list)
{
	errcode_t	retval;
	بنية read_bb_رڪارڊ rb;
	بنية ext2_inode inode;
	blk_t	numblocks;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	إذا (!*bb_list) {
		retval = ext2fs_read_inode(fs, EXT2_BAD_INO, &inode);
		إذا (retval)
			أرجع retval;
		numblocks = inode.i_بلاڪون;
		إذا (!(ext2fs_has_feature_huge_file(fs->super) &&
		      (inode.i_flags & EXT4_HUGE_FILE_FL)))
			numblocks = numblocks / (fs->blocksize / 512);
		numblocks += 20;
		إذا (numblocks < 50)
			numblocks = 50;
		إذا (numblocks > 50000)
			numblocks = 500;
		retval = ext2fs_badblocks_list_create(bb_list, numblocks);
		إذا (retval)
			أرجع retval;
	}

	rb.bb_list = *bb_list;
	rb.err = 0;
	retval = ext2fs_بلاڪ_iterate2(fs, EXT2_BAD_INO, بلاڪ_FLAG_READ_ONLY,
				       0, mark_bad_بلاڪ, &rb);
	إذا (retval)
		أرجع retval;

	أرجع rb.err;
}


