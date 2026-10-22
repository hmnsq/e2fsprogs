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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಆದರೆ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <time.h>
#ಆದರೆ HAVE_SYS_STAT_H
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_SYS_TYPES_H
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"

ರಚನೆ read_bb_ದಾಖಲೆ {
	ext2_badblocks_list	bb_list;
	errcode_t	err;
};

/*
 * Helper function for ext2fs_read_bb_inode()
 */
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __TURBOC__
 #ಸೂಚನೆ argsused
#ಆದರೆ_ಅಂತ್ಯ
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ mark_bad_ಖಂಡ(ext2_filsys fs, blk_t *ಖಂಡ_nr,
			  e2_blkcnt_t blockcnt EXT2FS_ATTR((ಬಳಸದ)),
			  blk_t ref_ಖಂಡ EXT2FS_ATTR((ಬಳಸದ)),
			  ಪೂರ್ಣಾಂಕ ref_ಸ್ಥಾನಾಂತರ EXT2FS_ATTR((ಬಳಸದ)),
			  ಶೂನ್ಯ *priv_ದತ್ತಾಂಶ)
{
	ರಚನೆ read_bb_ದಾಖಲೆ *rb = (ರಚನೆ read_bb_ದಾಖಲೆ *) priv_ದತ್ತಾಂಶ;

	ಆದರೆ (blockcnt < 0)
		ಹಿಂತಿರುಗಿ 0;

	ಆದರೆ ((*ಖಂಡ_nr < fs->super->s_ಮೊದಲ_ದತ್ತಾಂಶ_ಖಂಡ) ||
	    (*ಖಂಡ_nr >= ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(fs->super)))
		ಹಿಂತಿರುಗಿ 0;	/* Ignore illegal blocks */

	rb->err = ext2fs_badblocks_list_ಕೂಡಿಸು(rb->bb_list, *ಖಂಡ_nr);
	ಆದರೆ (rb->err)
		ಹಿಂತಿರುಗಿ ಖಂಡ_ABORT;
	ಹಿಂತಿರುಗಿ 0;
}

/*
 * Reads the current bad blocks from the bad blocks inode.
 */
errcode_t ext2fs_read_bb_inode(ext2_filsys fs, ext2_badblocks_list *bb_list)
{
	errcode_t	retval;
	ರಚನೆ read_bb_ದಾಖಲೆ rb;
	ರಚನೆ ext2_inode inode;
	blk_t	numblocks;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ಆದರೆ (!*bb_list) {
		retval = ext2fs_read_inode(fs, EXT2_BAD_INO, &inode);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;
		numblocks = inode.i_ಖಂಡಗಳು;
		ಆದರೆ (!(ext2fs_has_feature_huge_file(fs->super) &&
		      (inode.i_flags & EXT4_HUGE_FILE_FL)))
			numblocks = numblocks / (fs->blocksize / 512);
		numblocks += 20;
		ಆದರೆ (numblocks < 50)
			numblocks = 50;
		ಆದರೆ (numblocks > 50000)
			numblocks = 500;
		retval = ext2fs_badblocks_list_create(bb_list, numblocks);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;
	}

	rb.bb_list = *bb_list;
	rb.err = 0;
	retval = ext2fs_ಖಂಡ_iterate2(fs, EXT2_BAD_INO, ಖಂಡ_FLAG_READ_ONLY,
				       0, mark_bad_ಖಂಡ, &rb);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	ಹಿಂತಿರುಗಿ rb.err;
}


