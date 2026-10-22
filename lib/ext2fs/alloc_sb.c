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

#অন্তর্ভাব "config.h"
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <string.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাব <unistd.h>
#endif
#অন্তর্ভাব <fcntl.h>
#অন্তর্ভাব <time.h>
#যদি HAVE_SYS_STAT_H
#অন্তর্ভাব <sys/stat.h>
#endif
#যদি HAVE_SYS_TYPES_H
#অন্তর্ভাব <sys/types.h>
#endif

#অন্তর্ভাব "ext2_fs.h"
#অন্তর্ভাব "ext2fs.h"

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
পূর্ণসংখ্যা ext2fs_reserve_super_and_bgd(ext2_filsys fs,
				 dgrp_t group,
				 ext2fs_block_bitmap bmap)
{
	blk64_t	super_blk, old_desc_blk, new_desc_blk;
	blk_t	used_blks, old_desc_blocks, অঙ্ক_blocks;

	ext2fs_super_and_bgd_loc2(fs, group, &super_blk,
				  &old_desc_blk, &new_desc_blk, &used_blks);

	যদি (ext2fs_has_feature_meta_bg(fs->super))
		old_desc_blocks = fs->super->s_first_meta_bg;
	অন্যথা
		old_desc_blocks =
			fs->desc_blocks + fs->super->s_reserved_gdt_blocks;

	যদি (super_blk || (group == 0))
		ext2fs_mark_block_bitmap2(bmap, super_blk);
	যদি ((group == 0) && (fs->blocksize == 1024) &&
	    EXT2FS_CLUSTER_RATIO(fs) > 1)
		ext2fs_mark_block_bitmap2(bmap, 0);

	যদি (old_desc_blk) {
		অঙ্ক_blocks = old_desc_blocks;
		যদি (old_desc_blk + অঙ্ক_blocks >= ext2fs_blocks_গুনতি(fs->super))
			অঙ্ক_blocks = ext2fs_blocks_গুনতি(fs->super) -
				old_desc_blk;
		ext2fs_mark_block_bitmap_range2(bmap, old_desc_blk, অঙ্ক_blocks);
	}
	যদি (new_desc_blk)
		ext2fs_mark_block_bitmap2(bmap, new_desc_blk);

	অঙ্ক_blocks = ext2fs_group_blocks_গুনতি(fs, group);
	অঙ্ক_blocks -= 2 + fs->inode_blocks_per_group + used_blks;

	ফেরত অঙ্ক_blocks  ;
}

/*
 * This function reserves the superblock and block group descriptors
 * for a given block group and returns the number of blocks used by the
 * super block and group descriptors by looking up the block bitmap.
 */
errcode_t ext2fs_reserve_super_and_bgd2(ext2_filsys fs,
				        dgrp_t group,
				        ext2fs_block_bitmap bmap,
				        blk_t *desc_blocks)
{
	blk64_t	অঙ্ক_blocks;
	errcode_t retval = 0;

	ext2fs_reserve_super_and_bgd(fs, group, bmap);

	retval = ext2fs_গুনতি_used_blocks(fs,
					ext2fs_group_first_block2(fs, group),
					ext2fs_group_last_block2(fs, group),
					&অঙ্ক_blocks);
	যদি (!retval)
		*desc_blocks = অঙ্ক_blocks;

	ফেরত retval;
}
