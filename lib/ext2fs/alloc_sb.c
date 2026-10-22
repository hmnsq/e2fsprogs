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

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#endif
#ضمن <fcntl.h>
#ضمن <time.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#endif
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#endif

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

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
صحيح ext2fs_reserve_super_and_bgd(ext2_filsys fs,
				 dgrp_t group,
				 ext2fs_block_bitmap bmap)
{
	blk64_t	super_blk, old_desc_blk, new_desc_blk;
	blk_t	used_blks, old_desc_blocks, ژمارە_blocks;

	ext2fs_super_and_bgd_loc2(fs, group, &super_blk,
				  &old_desc_blk, &new_desc_blk, &used_blks);

	إذا (ext2fs_has_feature_meta_bg(fs->super))
		old_desc_blocks = fs->super->s_first_meta_bg;
	وإلا
		old_desc_blocks =
			fs->desc_blocks + fs->super->s_reserved_gdt_blocks;

	إذا (super_blk || (group == 0))
		ext2fs_mark_block_bitmap2(bmap, super_blk);
	إذا ((group == 0) && (fs->blocksize == 1024) &&
	    EXT2FS_CLUSTER_RATIO(fs) > 1)
		ext2fs_mark_block_bitmap2(bmap, 0);

	إذا (old_desc_blk) {
		ژمارە_blocks = old_desc_blocks;
		إذا (old_desc_blk + ژمارە_blocks >= ext2fs_blocks_ژماردن(fs->super))
			ژمارە_blocks = ext2fs_blocks_ژماردن(fs->super) -
				old_desc_blk;
		ext2fs_mark_block_bitmap_range2(bmap, old_desc_blk, ژمارە_blocks);
	}
	إذا (new_desc_blk)
		ext2fs_mark_block_bitmap2(bmap, new_desc_blk);

	ژمارە_blocks = ext2fs_group_blocks_ژماردن(fs, group);
	ژمارە_blocks -= 2 + fs->inode_blocks_per_group + used_blks;

	أرجع ژمارە_blocks  ;
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
	blk64_t	ژمارە_blocks;
	errcode_t retval = 0;

	ext2fs_reserve_super_and_bgd(fs, group, bmap);

	retval = ext2fs_ژماردن_used_blocks(fs,
					ext2fs_group_first_block2(fs, group),
					ext2fs_group_last_block2(fs, group),
					&ژمارە_blocks);
	إذا (!retval)
		*desc_blocks = ژمارە_blocks;

	أرجع retval;
}
