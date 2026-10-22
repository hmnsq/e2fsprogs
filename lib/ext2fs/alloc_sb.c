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

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#यदि_अन्तः
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <time.h>
#यदि HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#यदि_अन्तः
#यदि HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#यदि_अन्तः

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"

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
पूर्णाङ्क ext2fs_reserve_super_and_bgd(ext2_filsys fs,
				 dgrp_t गणः,
				 ext2fs_खण्डम्_bitmap bmap)
{
	blk64_t	super_blk, old_desc_blk, new_desc_blk;
	blk_t	प्रयुक्तम्_blks, old_desc_खण्डानि, अङ्कः_खण्डानि;

	ext2fs_super_and_bgd_loc2(fs, गणः, &super_blk,
				  &old_desc_blk, &new_desc_blk, &प्रयुक्तम्_blks);

	यदि (ext2fs_has_feature_meta_bg(fs->super))
		old_desc_खण्डानि = fs->super->s_first_meta_bg;
	अन्यथा
		old_desc_खण्डानि =
			fs->desc_खण्डानि + fs->super->s_reserved_gdt_खण्डानि;

	यदि (super_blk || (गणः == 0))
		ext2fs_mark_खण्डम्_bitmap2(bmap, super_blk);
	यदि ((गणः == 0) && (fs->blocksize == 1024) &&
	    EXT2_FS_CLUSTER_अनुपातः(fs) > 1)
		ext2fs_mark_खण्डम्_bitmap2(bmap, 0);

	यदि (old_desc_blk) {
		अङ्कः_खण्डानि = old_desc_खण्डानि;
		यदि (old_desc_blk + अङ्कः_खण्डानि >= ext2fs_खण्डानि_गणनम्(fs->super))
			अङ्कः_खण्डानि = ext2fs_खण्डानि_गणनम्(fs->super) -
				old_desc_blk;
		ext2fs_mark_खण्डम्_bitmap_range2(bmap, old_desc_blk, अङ्कः_खण्डानि);
	}
	यदि (new_desc_blk)
		ext2fs_mark_खण्डम्_bitmap2(bmap, new_desc_blk);

	अङ्कः_खण्डानि = ext2fs_गणः_खण्डानि_गणनम्(fs, गणः);
	अङ्कः_खण्डानि -= 2 + fs->inode_खण्डानि_प्रत्येकम्_गणः + प्रयुक्तम्_blks;

	प्रत्यागम अङ्कः_खण्डानि  ;
}

/*
 * This function reserves the superblock and block group descriptors
 * for a given block group and returns the number of blocks used by the
 * super block and group descriptors by looking up the block bitmap.
 */
errcode_t ext2fs_reserve_super_and_bgd2(ext2_filsys fs,
				        dgrp_t गणः,
				        ext2fs_खण्डम्_bitmap bmap,
				        blk_t *desc_खण्डानि)
{
	blk64_t	अङ्कः_खण्डानि;
	errcode_t retval = 0;

	ext2fs_reserve_super_and_bgd(fs, गणः, bmap);

	retval = ext2fs_गणनम्_प्रयुक्तम्_खण्डानि(fs,
					ext2fs_गणः_first_block2(fs, गणः),
					ext2fs_गणः_चरमः_block2(fs, गणः),
					&अङ्कः_खण्डानि);
	यदि (!retval)
		*desc_खण्डानि = अङ्कः_खण्डानि;

	प्रत्यागम retval;
}
