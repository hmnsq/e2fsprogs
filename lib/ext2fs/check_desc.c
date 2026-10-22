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
 * This routine sanity checks the group descriptors
 */
errcode_t ext2fs_check_desc(ext2_filsys fs)
{
	ext2fs_खण्डम्_bitmap bmap;
	errcode_t retval;
	dgrp_t i;
	blk64_t first_खण्डम् = fs->super->s_first_दत्त_खण्डम्;
	blk64_t चरमः_खण्डम् = ext2fs_खण्डानि_गणनम्(fs->super)-1;
	blk64_t blk, b;
	अचिह्नित पूर्णाङ्क j;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	यदि (EXT2_DESC_आकार(fs->super) & (EXT2_DESC_आकार(fs->super) - 1))
		प्रत्यागम EXT2_ET_BAD_DESC_आकार;

	retval = ext2fs_allocate_subcluster_bitmap(fs, "check_desc map", &bmap);
	यदि (retval)
		प्रत्यागम retval;

	प्रति (i = 0; i < fs->गणः_desc_गणनम्; i++)
		ext2fs_reserve_super_and_bgd(fs, i, bmap);

	प्रति (i = 0; i < fs->गणः_desc_गणनम्; i++) {
		यदि (!ext2fs_has_feature_flex_bg(fs->super)) {
			first_खण्डम् = ext2fs_गणः_first_block2(fs, i);
			चरमः_खण्डम् = ext2fs_गणः_चरमः_block2(fs, i);
		}

		/*
		 * Check to make sure the block bitmap for group is sane
		 */
		blk = ext2fs_खण्डम्_bitmap_loc(fs, i);
		यदि (blk < first_खण्डम् || blk > चरमः_खण्डम् ||
		    ext2fs_test_खण्डम्_bitmap2(bmap, blk)) {
			retval = EXT2_ET_GDESC_BAD_खण्डम्_MAP;
			गच्छ errout;
		}
		ext2fs_mark_खण्डम्_bitmap2(bmap, blk);

		/*
		 * Check to make sure the inode bitmap for group is sane
		 */
		blk = ext2fs_inode_bitmap_loc(fs, i);
		यदि (blk < first_खण्डम् || blk > चरमः_खण्डम् ||
		    ext2fs_test_खण्डम्_bitmap2(bmap, blk)) {
			retval = EXT2_ET_GDESC_BAD_INODE_MAP;
			गच्छ errout;
		}
		ext2fs_mark_खण्डम्_bitmap2(bmap, blk);

		/*
		 * Check to make sure the inode table for group is sane
		 */
		blk = ext2fs_inode_सारणी_loc(fs, i);
		यदि (blk < first_खण्डम् ||
		    ((blk + fs->inode_खण्डानि_प्रत्येकम्_गणः - 1) > चरमः_खण्डम्)) {
			retval = EXT2_ET_GDESC_BAD_INODE_सारणी;
			गच्छ errout;
		}
		प्रति (j = 0, b = blk; j < fs->inode_खण्डानि_प्रत्येकम्_गणः;
		     j++, b++) {
			यदि (ext2fs_test_खण्डम्_bitmap2(bmap, b)) {
				retval = EXT2_ET_GDESC_BAD_INODE_सारणी;
				गच्छ errout;
			}
			ext2fs_mark_खण्डम्_bitmap2(bmap, b);
		}
	}
errout:
	ext2fs_free_खण्डम्_bitmap(bmap);
	प्रत्यागम retval;
}
