/*
 * check_desc.c --- Check the group descriptors of an ext2 filesystem
 *
 * Copyright (C) 1993, 1994, 1995, 1996 Theodore Ts'o.
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
 * This routine sanity checks the group descriptors
 */
errcode_t ext2fs_check_desc(ext2_filsys fs)
{
	ext2fs_ಖಂಡ_bitmap bmap;
	errcode_t retval;
	dgrp_t i;
	blk64_t ಮೊದಲ_ಖಂಡ = fs->super->s_ಮೊದಲ_ದತ್ತಾಂಶ_ಖಂಡ;
	blk64_t ಕೊನೆಯ_ಖಂಡ = ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(fs->super)-1;
	blk64_t blk, b;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ j;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ಆದರೆ (EXT2_DESC_ಗಾತ್ರ(fs->super) & (EXT2_DESC_ಗಾತ್ರ(fs->super) - 1))
		ಹಿಂತಿರುಗಿ EXT2_ET_BAD_DESC_ಗಾತ್ರ;

	retval = ext2fs_allocate_subcluster_bitmap(fs, "check_desc map", &bmap);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	ಗಾಗಿ (i = 0; i < fs->ಗುಂಪು_desc_ಎಣಿಕೆ; i++)
		ext2fs_reserve_super_and_bgd(fs, i, bmap);

	ಗಾಗಿ (i = 0; i < fs->ಗುಂಪು_desc_ಎಣಿಕೆ; i++) {
		ಆದರೆ (!ext2fs_has_feature_flex_bg(fs->super)) {
			ಮೊದಲ_ಖಂಡ = ext2fs_ಗುಂಪು_ಮೊದಲ_block2(fs, i);
			ಕೊನೆಯ_ಖಂಡ = ext2fs_ಗುಂಪು_ಕೊನೆಯ_block2(fs, i);
		}

		/*
		 * Check to make sure the block bitmap for group is sane
		 */
		blk = ext2fs_ಖಂಡ_bitmap_loc(fs, i);
		ಆದರೆ (blk < ಮೊದಲ_ಖಂಡ || blk > ಕೊನೆಯ_ಖಂಡ ||
		    ext2fs_test_ಖಂಡ_bitmap2(bmap, blk)) {
			retval = EXT2_ET_GDESC_BAD_ಖಂಡ_MAP;
			ಹೋಗು errout;
		}
		ext2fs_mark_ಖಂಡ_bitmap2(bmap, blk);

		/*
		 * Check to make sure the inode bitmap for group is sane
		 */
		blk = ext2fs_inode_bitmap_loc(fs, i);
		ಆದರೆ (blk < ಮೊದಲ_ಖಂಡ || blk > ಕೊನೆಯ_ಖಂಡ ||
		    ext2fs_test_ಖಂಡ_bitmap2(bmap, blk)) {
			retval = EXT2_ET_GDESC_BAD_INODE_MAP;
			ಹೋಗು errout;
		}
		ext2fs_mark_ಖಂಡ_bitmap2(bmap, blk);

		/*
		 * Check to make sure the inode table for group is sane
		 */
		blk = ext2fs_inode_ಕೋಷ್ಟಕ_loc(fs, i);
		ಆದರೆ (blk < ಮೊದಲ_ಖಂಡ ||
		    ((blk + fs->inode_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು - 1) > ಕೊನೆಯ_ಖಂಡ)) {
			retval = EXT2_ET_GDESC_BAD_INODE_ಕೋಷ್ಟಕ;
			ಹೋಗು errout;
		}
		ಗಾಗಿ (j = 0, b = blk; j < fs->inode_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು;
		     j++, b++) {
			ಆದರೆ (ext2fs_test_ಖಂಡ_bitmap2(bmap, b)) {
				retval = EXT2_ET_GDESC_BAD_INODE_ಕೋಷ್ಟಕ;
				ಹೋಗು errout;
			}
			ext2fs_mark_ಖಂಡ_bitmap2(bmap, b);
		}
	}
errout:
	ext2fs_free_ಖಂಡ_bitmap(bmap);
	ಹಿಂತಿರುಗಿ retval;
}
