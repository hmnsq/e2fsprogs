/*
 * alloc_tables.c --- Allocate tables for a newly initialized
 * filesystem.  Used by mke2fs when initializing a filesystem
 *
 * Copyright (C) 1996 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#include "config.h"
#include <stdio.h>
#include <string.h>
#nếu HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>
#include <time.h>
#nếu HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#nếu HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include "ext2_fs.h"
#include "ext2fs.h"
#include "ext2fsP.h"

/*
 * This routine searches for free blocks that can allocate a full
 * group of bitmaps or inode tables for a flexbg group.  Returns the
 * block number with a correct offset were the bitmaps and inode
 * tables can be allocated continuously and in order.
 */
static blk64_t flexbg_offset(ext2_filsys fs, dgrp_t group, blk64_t start_blk,
			     ext2fs_block_bitmap bmap, số_nguyên rem_grp,
			     số_nguyên elem_size)
{
	số_nguyên		flexbg, flexbg_size, size;
	blk64_t		last_blk, first_free = 0;
	dgrp_t	       	last_grp;

	flexbg_size = 1U << fs->super->s_log_groups_per_flex;
	flexbg = group / flexbg_size;
	size = rem_grp * elem_size;

	nếu (size > (số_nguyên) (fs->super->s_blocks_per_group / 4))
		size = (số_nguyên) fs->super->s_blocks_per_group / 4;

	/*
	 * Don't do a long search if the previous block search is still valid,
	 * but skip minor obstructions such as group descriptor backups.
	 */
	nếu (start_blk && start_blk < ext2fs_blocks_count(fs->super) &&
	    ext2fs_get_free_blocks2(fs, start_blk, start_blk + size, elem_size,
				    bmap, &first_free) == 0)
		trả_về first_free;

	start_blk = ext2fs_group_first_block2(fs, flexbg_size * flexbg);
	last_grp = group | (flexbg_size - 1);
	nếu (last_grp > fs->group_desc_count-1)
		last_grp = fs->group_desc_count-1;
	last_blk = ext2fs_group_last_block2(fs, last_grp);

	/* Find the first available block */
	nếu (ext2fs_get_free_blocks2(fs, start_blk, last_blk, size,
				    bmap, &first_free) == 0)
		trả_về first_free;

	nếu (ext2fs_get_free_blocks2(fs, start_blk, last_blk, elem_size,
				   bmap, &first_free) == 0)
		trả_về first_free;

	nếu (ext2fs_get_free_blocks2(fs, 0, last_blk, elem_size, bmap,
				    &first_free) == 0)
		trả_về first_free;

	trả_về first_free;
}

errcode_t ext2fs_allocate_group_table(ext2_filsys fs, dgrp_t group,
				      ext2fs_block_bitmap bmap)
{
	errcode_t	retval;
	blk64_t		group_blk, start_blk, last_blk, new_blk;
	dgrp_t		last_grp = 0;
	số_nguyên		rem_grps = 0, flexbg_size = 0, table_offset = 0;

	group_blk = ext2fs_group_first_block2(fs, group);
	last_blk = ext2fs_group_last_block2(fs, group);

	nếu (!bmap)
		bmap = fs->block_map;

	nếu (ext2fs_has_feature_flex_bg(fs->super) &&
	    fs->super->s_log_groups_per_flex) {
		flexbg_size = 1U << fs->super->s_log_groups_per_flex;
		last_grp = group | (flexbg_size - 1);
		nếu (last_grp > fs->group_desc_count-1)
			last_grp = fs->group_desc_count-1;
		rem_grps = last_grp - group + 1;
	}

	/*
	 * Allocate the block and inode bitmaps, if necessary
	 */
	nếu (fs->stride && !flexbg_size) {
		retval = ext2fs_get_free_blocks2(fs, group_blk, last_blk,
						 1, bmap, &start_blk);
		nếu (retval)
			trả_về retval;
		start_blk += fs->inode_blocks_per_group;
		start_blk += ((fs->stride * group) %
			      (last_blk - start_blk + 1));
		nếu (start_blk >= last_blk)
			start_blk = group_blk;
	} không_thì
		start_blk = group_blk;

	nếu (flexbg_size) {
		blk64_t prev_block = 0;

		table_offset = flexbg_size;
		nếu (group % flexbg_size)
			prev_block = ext2fs_block_bitmap_loc(fs, group - 1) + 1;
		không_thì nếu (last_grp == fs->group_desc_count-1) {
			/*
			 * If we are allocating for the last flex_bg
			 * keep the metadata tables contiguous
			 */
			table_offset = last_grp & (flexbg_size - 1);
			nếu (table_offset == 0)
				table_offset = flexbg_size;
			không_thì
				table_offset++;
		}
		/* FIXME: Take backup group descriptor blocks into account
		 * if the flexbg allocations will grow to overlap them... */
		start_blk = flexbg_offset(fs, group, prev_block, bmap,
					  rem_grps, 1);
		last_blk = ext2fs_group_last_block2(fs, last_grp);
	}

	nếu (!ext2fs_block_bitmap_loc(fs, group)) {
		retval = ext2fs_get_free_blocks2(fs, start_blk, last_blk,
						 1, bmap, &new_blk);
		nếu (retval == EXT2_ET_BLOCK_ALLOC_FAIL)
			retval = ext2fs_get_free_blocks2(fs, group_blk,
					last_blk, 1, bmap, &new_blk);
		nếu (retval)
			trả_về retval;
		ext2fs_mark_block_bitmap2(bmap, new_blk);
		ext2fs_block_bitmap_loc_set(fs, group, new_blk);
		nếu (flexbg_size) {
			dgrp_t gr = ext2fs_group_of_blk2(fs, new_blk);
			ext2fs_bg_free_blocks_count_set(fs, gr, ext2fs_bg_free_blocks_count(fs, gr) - 1);
			ext2fs_free_blocks_count_add(fs->super, -1);
			ext2fs_bg_flags_clear(fs, gr, EXT2_BG_BLOCK_UNINIT);
			ext2fs_group_desc_csum_set(fs, gr);
		}
	}

	nếu (flexbg_size) {
		blk64_t prev_block = 0;
		nếu (group % flexbg_size)
			prev_block = ext2fs_inode_bitmap_loc(fs, group - 1) + 1;
		không_thì
			prev_block = ext2fs_block_bitmap_loc(fs, group) +
				table_offset;
		/* FIXME: Take backup group descriptor blocks into account
		 * if the flexbg allocations will grow to overlap them... */
		start_blk = flexbg_offset(fs, group, prev_block, bmap,
					  rem_grps, 1);
		last_blk = ext2fs_group_last_block2(fs, last_grp);
	}

	nếu (!ext2fs_inode_bitmap_loc(fs, group)) {
		retval = ext2fs_get_free_blocks2(fs, start_blk, last_blk,
						 1, bmap, &new_blk);
		nếu (retval == EXT2_ET_BLOCK_ALLOC_FAIL)
			retval = ext2fs_get_free_blocks2(fs, group_blk,
					 last_blk, 1, bmap, &new_blk);
		nếu (retval)
			trả_về retval;
		ext2fs_mark_block_bitmap2(bmap, new_blk);
		ext2fs_inode_bitmap_loc_set(fs, group, new_blk);
		nếu (flexbg_size) {
			dgrp_t gr = ext2fs_group_of_blk2(fs, new_blk);
			ext2fs_bg_free_blocks_count_set(fs, gr, ext2fs_bg_free_blocks_count(fs, gr) - 1);
			ext2fs_free_blocks_count_add(fs->super, -1);
			ext2fs_bg_flags_clear(fs, gr, EXT2_BG_BLOCK_UNINIT);
			ext2fs_group_desc_csum_set(fs, gr);
		}
	}

	/*
	 * Allocate the inode table
	 */
	nếu (flexbg_size) {
		blk64_t prev_block = 0;

		nếu (group % flexbg_size)
			prev_block = ext2fs_inode_table_loc(fs, group - 1) +
				fs->inode_blocks_per_group;
		không_thì
			prev_block = ext2fs_inode_bitmap_loc(fs, group) +
				table_offset;

		/* FIXME: Take backup group descriptor blocks into account
		 * if the flexbg allocations will grow to overlap them... */
		group_blk = flexbg_offset(fs, group, prev_block, bmap,
					  rem_grps, fs->inode_blocks_per_group);
		last_blk = ext2fs_group_last_block2(fs, last_grp);
	}

	nếu (!ext2fs_inode_table_loc(fs, group)) {
		retval = ext2fs_get_free_blocks2(fs, group_blk, last_blk,
						fs->inode_blocks_per_group,
						bmap, &new_blk);
		nếu (retval)
			trả_về retval;

		ext2fs_mark_block_bitmap_range2(bmap,
			new_blk, fs->inode_blocks_per_group);
		nếu (flexbg_size) {
			blk64_t num, blk;
			num = fs->inode_blocks_per_group;
			blk = new_blk;
			trong_khi (num) {
				số_nguyên gr = ext2fs_group_of_blk2(fs, blk);
				last_blk = ext2fs_group_last_block2(fs, gr);
				blk64_t n = num;

				nếu (blk + num > last_blk)
					n = last_blk - blk + 1;

				ext2fs_bg_free_blocks_count_set(fs, gr,
					ext2fs_bg_free_blocks_count(fs, gr) -
					n/EXT2FS_CLUSTER_RATIO(fs));
				ext2fs_bg_flags_clear(fs, gr,
					EXT2_BG_BLOCK_UNINIT);
				ext2fs_group_desc_csum_set(fs, gr);
				ext2fs_free_blocks_count_add(fs->super, -n);
				blk += n;
				num -= n;
			}
		}
		ext2fs_inode_table_loc_set(fs, group, new_blk);
	}
	ext2fs_group_desc_csum_set(fs, group);
	trả_về 0;
}

errcode_t ext2fs_allocate_tables(ext2_filsys fs)
{
	errcode_t	retval;
	dgrp_t		i;
	struct ext2fs_numeric_progress_struct progress;

	nếu (fs->progress_ops && fs->progress_ops->init)
		(fs->progress_ops->init)(fs, &progress, NULL,
					 fs->group_desc_count);

	cho (i = 0; i < fs->group_desc_count; i++) {
		nếu (fs->progress_ops && fs->progress_ops->update)
			(fs->progress_ops->update)(fs, &progress, i);
		retval = ext2fs_allocate_group_table(fs, i, fs->block_map);
		nếu (retval)
			trả_về retval;
	}
	nếu (fs->progress_ops && fs->progress_ops->close)
		(fs->progress_ops->close)(fs, &progress, NULL);
	trả_về 0;
}

