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

#شامل "config.h"
#شامل <stdio.h>
#شامل <string.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#endif
#شامل <fcntl.h>
#شامل <time.h>
#اگر HAVE_SYS_STAT_H
#شامل <sys/stat.h>
#endif
#اگر HAVE_SYS_TYPES_H
#شامل <sys/types.h>
#endif

#شامل "ext2_fs.h"
#شامل "ext2fs.h"

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
صحیح ext2fs_reserve_super_and_bgd(ext2_filsys fs,
				 dgrp_t group,
				 ext2fs_block_bitmap bmap)
{
	blk64_t	super_blk, old_desc_blk, new_desc_blk;
	blk_t	used_blks, old_desc_blocks, رقم_blocks;

	ext2fs_super_and_bgd_loc2(fs, group, &super_blk,
				  &old_desc_blk, &new_desc_blk, &used_blks);

	اگر (ext2fs_has_feature_meta_bg(fs->super))
		old_desc_blocks = fs->super->s_first_meta_bg;
	وگرنه
		old_desc_blocks =
			fs->desc_blocks + fs->super->s_reserved_gdt_blocks;

	اگر (super_blk || (group == 0))
		ext2fs_mark_block_bitmap2(bmap, super_blk);
	اگر ((group == 0) && (fs->blocksize == 1024) &&
	    EXT2FS_CLUSTER_RATIO(fs) > 1)
		ext2fs_mark_block_bitmap2(bmap, 0);

	اگر (old_desc_blk) {
		رقم_blocks = old_desc_blocks;
		اگر (old_desc_blk + رقم_blocks >= ext2fs_blocks_شمار(fs->super))
			رقم_blocks = ext2fs_blocks_شمار(fs->super) -
				old_desc_blk;
		ext2fs_mark_block_bitmap_range2(bmap, old_desc_blk, رقم_blocks);
	}
	اگر (new_desc_blk)
		ext2fs_mark_block_bitmap2(bmap, new_desc_blk);

	رقم_blocks = ext2fs_group_blocks_شمار(fs, group);
	رقم_blocks -= 2 + fs->inode_blocks_per_group + used_blks;

	بازگشت رقم_blocks  ;
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
	blk64_t	رقم_blocks;
	errcode_t retval = 0;

	ext2fs_reserve_super_and_bgd(fs, group, bmap);

	retval = ext2fs_شمار_used_blocks(fs,
					ext2fs_group_first_block2(fs, group),
					ext2fs_group_last_block2(fs, group),
					&رقم_blocks);
	اگر (!retval)
		*desc_blocks = رقم_blocks;

	بازگشت retval;
}
