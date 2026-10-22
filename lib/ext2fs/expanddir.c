/*
 * expand.c --- expand an ext2fs directory
 *
 * Copyright (C) 1993, 1994, 1995, 1996, 1997, 1998, 1999  Theodore Ts'o.
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

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fsP.h"

ರಚನೆ expand_dir_struct {
	ಪೂರ್ಣಾಂಕ		ಮುಗಿದ;
	ಪೂರ್ಣಾಂಕ		newblocks;
	blk64_t		goal;
	errcode_t	err;
	ext2_ino_t	dir;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ expand_dir_proc(ext2_filsys	fs,
			   blk64_t	*blocknr,
			   e2_blkcnt_t	blockcnt,
			   blk64_t	ref_ಖಂಡ EXT2FS_ATTR((ಬಳಸದ)),
			   ಪೂರ್ಣಾಂಕ		ref_ಸ್ಥಾನಾಂತರ EXT2FS_ATTR((ಬಳಸದ)),
			   ಶೂನ್ಯ		*priv_ದತ್ತಾಂಶ)
{
	ರಚನೆ expand_dir_struct *es = (ರಚನೆ expand_dir_struct *) priv_ದತ್ತಾಂಶ;
	blk64_t	new_blk;
	ಅಕ್ಷರ		*ಖಂಡ;
	errcode_t	retval;

	ಆದರೆ (*blocknr) {
		ಆದರೆ (blockcnt >= 0)
			es->goal = *blocknr;
		ಹಿಂತಿರುಗಿ 0;
	}
	ಆದರೆ (blockcnt &&
	    (EXT2FS_B2C(fs, es->goal) == EXT2FS_B2C(fs, es->goal+1)))
		new_blk = es->goal+1;
	ಇಲ್ಲದಿದ್ದರೆ {
		es->goal &= ~EXT2_FS_CLUSTER_ಮುಖವಾಡ(fs);
		retval = ext2fs_new_block2(fs, es->goal, 0, &new_blk);
		ಆದರೆ (retval) {
			es->err = retval;
			ಹಿಂತಿರುಗಿ ಖಂಡ_ABORT;
		}
		es->newblocks++;
		ext2fs_ಖಂಡ_alloc_stats2(fs, new_blk, +1);
	}
	ಆದರೆ (blockcnt > 0) {
		retval = ext2fs_new_dir_ಖಂಡ(fs, 0, 0, &ಖಂಡ);
		ಆದರೆ (retval) {
			es->err = retval;
			ಹಿಂತಿರುಗಿ ಖಂಡ_ABORT;
		}
		es->ಮುಗಿದ = 1;
		retval = ext2fs_write_dir_block4(fs, new_blk, ಖಂಡ, 0,
						 es->dir);
		ext2fs_free_mem(&ಖಂಡ);
	} ಇಲ್ಲದಿದ್ದರೆ
		retval = ext2fs_zero_blocks2(fs, new_blk, 1, ಖಾಲಿ, ಖಾಲಿ);
	ಆದರೆ (blockcnt >= 0)
		es->goal = new_blk;
	ಆದರೆ (retval) {
		es->err = retval;
		ಹಿಂತಿರುಗಿ ಖಂಡ_ABORT;
	}
	*blocknr = new_blk;

	ಆದರೆ (es->ಮುಗಿದ)
		ಹಿಂತಿರುಗಿ (ಖಂಡ_ಬದಲಾಯಿತು | ಖಂಡ_ABORT);
	ಇಲ್ಲದಿದ್ದರೆ
		ಹಿಂತಿರುಗಿ ಖಂಡ_ಬದಲಾಯಿತು;
}

errcode_t ext2fs_expand_dir(ext2_filsys fs, ext2_ino_t dir)
{
	errcode_t	retval;
	ರಚನೆ expand_dir_struct es;
	ರಚನೆ ext2_inode	inode;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ಆದರೆ (!(fs->flags & EXT2_FLAG_RW))
		ಹಿಂತಿರುಗಿ EXT2_ET_RO_FILSYS;

	ಆದರೆ (!fs->ಖಂಡ_map)
		ಹಿಂತಿರುಗಿ EXT2_ET_NO_ಖಂಡ_BITMAP;

	retval = ext2fs_check_directory(fs, dir);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	retval = ext2fs_read_inode(fs, dir, &inode);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	es.ಮುಗಿದ = 0;
	es.err = 0;
	es.goal = ext2fs_ಕಂಡುಹಿಡಿ_inode_goal(fs, dir, &inode, 0);
	es.newblocks = 0;
	es.dir = dir;

	retval = ext2fs_ಖಂಡ_iterate3(fs, dir, ಖಂಡ_FLAG_APPEND,
				       0, expand_dir_proc, &es);
	ಆದರೆ (retval == EXT2_ET_INLINE_ದತ್ತಾಂಶ_CANT_ITERATE)
		ಹಿಂತಿರುಗಿ ext2fs_inline_ದತ್ತಾಂಶ_expand(fs, dir);

	ಆದರೆ (es.err)
		ಹಿಂತಿರುಗಿ es.err;
	ಆದರೆ (!es.ಮುಗಿದ)
		ಹಿಂತಿರುಗಿ EXT2_ET_EXPAND_DIR_ERR;

	/*
	 * Update the size and block count fields in the inode.
	 */
	retval = ext2fs_read_inode(fs, dir, &inode);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	retval = ext2fs_inode_ಗಾತ್ರ_ಗಣ(fs, &inode,
				       EXT2_I_ಗಾತ್ರ(&inode) + fs->blocksize);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;
	ext2fs_iblk_ಕೂಡಿಸು_ಖಂಡಗಳು(fs, &inode, es.newblocks);

	retval = ext2fs_write_inode(fs, dir, &inode);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	ಹಿಂತಿರುಗಿ 0;
}
