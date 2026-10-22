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

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <string.h>
#ఒకవేళ HAVE_UNISTD_H
#అన్తర్భావ <unistd.h>
#అయితే_ముగింపు
#అన్తర్భావ <fcntl.h>
#అన్తర్భావ <time.h>
#ఒకవేళ HAVE_SYS_STAT_H
#అన్తర్భావ <sys/stat.h>
#అయితే_ముగింపు
#ఒకవేళ HAVE_SYS_TYPES_H
#అన్తర్భావ <sys/types.h>
#అయితే_ముగింపు

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fs.h"

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
పూర్ణాంకం ext2fs_reserve_super_and_bgd(ext2_filsys fs,
				 dgrp_t సమూహం,
				 ext2fs_ఖండం_bitmap bmap)
{
	blk64_t	super_blk, old_desc_blk, new_desc_blk;
	blk_t	వాడిన_blks, old_desc_ఖండాలు, అంకె_ఖండాలు;

	ext2fs_super_and_bgd_loc2(fs, సమూహం, &super_blk,
				  &old_desc_blk, &new_desc_blk, &వాడిన_blks);

	ఒకవేళ (ext2fs_has_feature_meta_bg(fs->super))
		old_desc_ఖండాలు = fs->super->s_మొదటి_meta_bg;
	లేకపోతే
		old_desc_ఖండాలు =
			fs->desc_ఖండాలు + fs->super->s_reserved_gdt_ఖండాలు;

	ఒకవేళ (super_blk || (సమూహం == 0))
		ext2fs_mark_ఖండం_bitmap2(bmap, super_blk);
	ఒకవేళ ((సమూహం == 0) && (fs->blocksize == 1024) &&
	    EXT2_FS_CLUSTER_నిష్పత్తి(fs) > 1)
		ext2fs_mark_ఖండం_bitmap2(bmap, 0);

	ఒకవేళ (old_desc_blk) {
		అంకె_ఖండాలు = old_desc_ఖండాలు;
		ఒకవేళ (old_desc_blk + అంకె_ఖండాలు >= ext2fs_ఖండాలు_లెక్క(fs->super))
			అంకె_ఖండాలు = ext2fs_ఖండాలు_లెక్క(fs->super) -
				old_desc_blk;
		ext2fs_mark_ఖండం_bitmap_range2(bmap, old_desc_blk, అంకె_ఖండాలు);
	}
	ఒకవేళ (new_desc_blk)
		ext2fs_mark_ఖండం_bitmap2(bmap, new_desc_blk);

	అంకె_ఖండాలు = ext2fs_సమూహం_ఖండాలు_లెక్క(fs, సమూహం);
	అంకె_ఖండాలు -= 2 + fs->inode_ఖండాలు_చొప్పున_సమూహం + వాడిన_blks;

	తిరిగి అంకె_ఖండాలు  ;
}

/*
 * This function reserves the superblock and block group descriptors
 * for a given block group and returns the number of blocks used by the
 * super block and group descriptors by looking up the block bitmap.
 */
errcode_t ext2fs_reserve_super_and_bgd2(ext2_filsys fs,
				        dgrp_t సమూహం,
				        ext2fs_ఖండం_bitmap bmap,
				        blk_t *desc_ఖండాలు)
{
	blk64_t	అంకె_ఖండాలు;
	errcode_t retval = 0;

	ext2fs_reserve_super_and_bgd(fs, సమూహం, bmap);

	retval = ext2fs_లెక్క_వాడిన_ఖండాలు(fs,
					ext2fs_సమూహం_మొదటి_block2(fs, సమూహం),
					ext2fs_సమూహం_చివరి_block2(fs, సమూహం),
					&అంకె_ఖండాలు);
	ఒకవేళ (!retval)
		*desc_ఖండాలు = అంకె_ఖండాలు;

	తిరిగి retval;
}
