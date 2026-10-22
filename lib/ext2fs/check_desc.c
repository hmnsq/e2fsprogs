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
#सचे HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <time.h>
#सचे HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#endif
#सचे HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#endif

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"

/*
 * This routine sanity checks the group descriptors
 */
errcode_t ext2fs_check_desc(ext2_filsys fs)
{
	ext2fs_block_bitmap bmap;
	errcode_t retval;
	dgrp_t i;
	blk64_t first_block = fs->super->s_first_data_block;
	blk64_t last_block = ext2fs_blocks_गणनम्(fs->super)-1;
	blk64_t blk, b;
	unsigned पूर्णाङ्क j;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	सचे (EXT2_DESC_SIZE(fs->super) & (EXT2_DESC_SIZE(fs->super) - 1))
		पटिनिवत्त EXT2_ET_BAD_DESC_SIZE;

	retval = ext2fs_allocate_subcluster_bitmap(fs, "check_desc map", &bmap);
	सचे (retval)
		पटिनिवत्त retval;

	अत्थाय (i = 0; i < fs->group_desc_गणनम्; i++)
		ext2fs_reserve_super_and_bgd(fs, i, bmap);

	अत्थाय (i = 0; i < fs->group_desc_गणनम्; i++) {
		सचे (!ext2fs_has_feature_flex_bg(fs->super)) {
			first_block = ext2fs_group_first_block2(fs, i);
			last_block = ext2fs_group_last_block2(fs, i);
		}

		/*
		 * Check to make sure the block bitmap for group is sane
		 */
		blk = ext2fs_block_bitmap_loc(fs, i);
		सचे (blk < first_block || blk > last_block ||
		    ext2fs_test_block_bitmap2(bmap, blk)) {
			retval = EXT2_ET_GDESC_BAD_BLOCK_MAP;
			गच्छ errout;
		}
		ext2fs_mark_block_bitmap2(bmap, blk);

		/*
		 * Check to make sure the inode bitmap for group is sane
		 */
		blk = ext2fs_inode_bitmap_loc(fs, i);
		सचे (blk < first_block || blk > last_block ||
		    ext2fs_test_block_bitmap2(bmap, blk)) {
			retval = EXT2_ET_GDESC_BAD_INODE_MAP;
			गच्छ errout;
		}
		ext2fs_mark_block_bitmap2(bmap, blk);

		/*
		 * Check to make sure the inode table for group is sane
		 */
		blk = ext2fs_inode_table_loc(fs, i);
		सचे (blk < first_block ||
		    ((blk + fs->inode_blocks_per_group - 1) > last_block)) {
			retval = EXT2_ET_GDESC_BAD_INODE_TABLE;
			गच्छ errout;
		}
		अत्थाय (j = 0, b = blk; j < fs->inode_blocks_per_group;
		     j++, b++) {
			सचे (ext2fs_test_block_bitmap2(bmap, b)) {
				retval = EXT2_ET_GDESC_BAD_INODE_TABLE;
				गच्छ errout;
			}
			ext2fs_mark_block_bitmap2(bmap, b);
		}
	}
errout:
	ext2fs_free_block_bitmap(bmap);
	पटिनिवत्त retval;
}
