/*
 * alloc_sb.c --- Allocate the superblock and block group descriptors for a
 * newly initialized filesystem.  Used by mke2fs when initializing a filesystem
 *
 * Copyright (C) 1994, 1995, 1996, 2003 Theodore Ts'o.
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

/*
 * This function reserves the superblock and block group descriptors
 * for a given block group.  It currently returns the number of free
 * blocks assuming that inode table and allocation bitmaps will be in
 * the group.  This is not necessarily the case when the flex_bg
 * feature is enabled, so callers should take care!  It was only
 * really intended for use by mke2fs, and even there it's not that
 * useful.  In the future, when we redo this function for 64-bit block
 * numbers, we should probably return the number of blocks used by the
 * super block and group descriptors instead.
 *
 * See also the comment for ext2fs_super_and_bgd_loc()
 */
ಪೂರ್ಣಾಂಕ ext2fs_reserve_super_and_bgd(ext2_filsys fs,
				 dgrp_t ಗುಂಪು,
				 ext2fs_ಖಂಡ_bitmap bmap)
{
	blk64_t	super_blk, old_desc_blk, new_desc_blk;
	blk_t	ಬಳಸಿದ_blks, old_desc_ಖಂಡಗಳು, ಅಂಕೆ_ಖಂಡಗಳು;

	ext2fs_super_and_bgd_loc2(fs, ಗುಂಪು, &super_blk,
				  &old_desc_blk, &new_desc_blk, &ಬಳಸಿದ_blks);

	ಆದರೆ (ext2fs_has_feature_meta_bg(fs->super))
		old_desc_ಖಂಡಗಳು = fs->super->s_ಮೊದಲ_meta_bg;
	ಇಲ್ಲದಿದ್ದರೆ
		old_desc_ಖಂಡಗಳು =
			fs->desc_ಖಂಡಗಳು + fs->super->s_reserved_gdt_ಖಂಡಗಳು;

	ಆದರೆ (super_blk || (ಗುಂಪು == 0))
		ext2fs_mark_ಖಂಡ_bitmap2(bmap, super_blk);
	ಆದರೆ ((ಗುಂಪು == 0) && (fs->blocksize == 1024) &&
	    EXT2_FS_CLUSTER_ಅನುಪಾತ(fs) > 1)
		ext2fs_mark_ಖಂಡ_bitmap2(bmap, 0);

	ಆದರೆ (old_desc_blk) {
		ಅಂಕೆ_ಖಂಡಗಳು = old_desc_ಖಂಡಗಳು;
		ಆದರೆ (old_desc_blk + ಅಂಕೆ_ಖಂಡಗಳು >= ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(fs->super))
			ಅಂಕೆ_ಖಂಡಗಳು = ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(fs->super) -
				old_desc_blk;
		ext2fs_mark_ಖಂಡ_bitmap_range2(bmap, old_desc_blk, ಅಂಕೆ_ಖಂಡಗಳು);
	}
	ಆದರೆ (new_desc_blk)
		ext2fs_mark_ಖಂಡ_bitmap2(bmap, new_desc_blk);

	ಅಂಕೆ_ಖಂಡಗಳು = ext2fs_ಗುಂಪು_ಖಂಡಗಳು_ಎಣಿಕೆ(fs, ಗುಂಪು);
	ಅಂಕೆ_ಖಂಡಗಳು -= 2 + fs->inode_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು + ಬಳಸಿದ_blks;

	ಹಿಂತಿರುಗಿ ಅಂಕೆ_ಖಂಡಗಳು  ;
}

/*
 * This function reserves the superblock and block group descriptors
 * for a given block group and returns the number of blocks used by the
 * super block and group descriptors by looking up the block bitmap.
 */
errcode_t ext2fs_reserve_super_and_bgd2(ext2_filsys fs,
				        dgrp_t ಗುಂಪು,
				        ext2fs_ಖಂಡ_bitmap bmap,
				        blk_t *desc_ಖಂಡಗಳು)
{
	blk64_t	ಅಂಕೆ_ಖಂಡಗಳು;
	errcode_t retval = 0;

	ext2fs_reserve_super_and_bgd(fs, ಗುಂಪು, bmap);

	retval = ext2fs_ಎಣಿಕೆ_ಬಳಸಿದ_ಖಂಡಗಳು(fs,
					ext2fs_ಗುಂಪು_ಮೊದಲ_block2(fs, ಗುಂಪು),
					ext2fs_ಗುಂಪು_ಕೊನೆಯ_block2(fs, ಗುಂಪು),
					&ಅಂಕೆ_ಖಂಡಗಳು);
	ಆದರೆ (!retval)
		*desc_ಖಂಡಗಳು = ಅಂಕೆ_ಖಂಡಗಳು;

	ಹಿಂತಿರುಗಿ retval;
}
