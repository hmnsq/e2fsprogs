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
#ຖ້າ HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>
#include <time.h>
#ຖ້າ HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#ຖ້າ HAVE_SYS_TYPES_H
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
			     ext2fs_block_bitmap bmap, ຈຳນວນເຕັມ rem_grp,
			     ຈຳນວນເຕັມ elem_size)
{
	ຈຳນວນເຕັມ		flexbg, flexbg_size, size;
	blk64_t		last_blk, first_free = 0;
	dgrp_t	       	last_grp;

	flexbg_size = 1U << fs->super->s_log_groups_per_flex;
	flexbg = group / flexbg_size;
	size = rem_grp * elem_size;

	ຖ້າ (size > (ຈຳນວນເຕັມ) (fs->super->s_blocks_per_group / 4))
		size = (ຈຳນວນເຕັມ) fs->super->s_blocks_per_group / 4;

	/*
	 * Don't do a long search if the previous block search is still valid,
	 * but skip minor obstructions such as group descriptor backups.
	 */
	ຖ້າ (start_blk && start_blk < ext2fs_blocks_count(fs->super) &&
	    ext2fs_get_free_blocks2(fs, start_blk, start_blk + size, elem_size,
				    bmap, &first_free) == 0)
		ສົ່ງຄືນ first_free;

	start_blk = ext2fs_group_first_block2(fs, flexbg_size * flexbg);
	last_grp = group | (flexbg_size - 1);
	ຖ້າ (last_grp > fs->group_desc_count-1)
		last_grp = fs->group_desc_count-1;
	last_blk = ext2fs_group_last_block2(fs, last_grp);

	/* Find the first available block */
	ຖ້າ (ext2fs_get_free_blocks2(fs, start_blk, last_blk, size,
				    bmap, &first_free) == 0)
		ສົ່ງຄືນ first_free;

	ຖ້າ (ext2fs_get_free_blocks2(fs, start_blk, last_blk, elem_size,
				   bmap, &first_free) == 0)
		ສົ່ງຄືນ first_free;

	ຖ້າ (ext2fs_get_free_blocks2(fs, 0, last_blk, elem_size, bmap,
				    &first_free) == 0)
		ສົ່ງຄືນ first_free;

	ສົ່ງຄືນ first_free;
}

errcode_t ext2fs_allocate_group_table(ext2_filsys fs, dgrp_t group,
				      ext2fs_block_bitmap bmap)
{
	errcode_t	retval;
	blk64_t		group_blk, start_blk, last_blk, new_blk;
	dgrp_t		last_grp = 0;
	ຈຳນວນເຕັມ		rem_grps = 0, flexbg_size = 0, table_offset = 0;

	group_blk = ext2fs_group_first_block2(fs, group);
	last_blk = ext2fs_group_last_block2(fs, group);

	ຖ້າ (!bmap)
		bmap = fs->block_map;

	ຖ້າ (ext2fs_has_feature_flex_bg(fs->super) &&
	    fs->super->s_log_groups_per_flex) {
		flexbg_size = 1U << fs->super->s_log_groups_per_flex;
		last_grp = group | (flexbg_size - 1);
		ຖ້າ (last_grp > fs->group_desc_count-1)
			last_grp = fs->group_desc_count-1;
		rem_grps = last_grp - group + 1;
	}

	/*
	 * Allocate the block and inode bitmaps, if necessary
	 */
	ຖ້າ (fs->stride && !flexbg_size) {
		retval = ext2fs_get_free_blocks2(fs, group_blk, last_blk,
						 1, bmap, &start_blk);
		ຖ້າ (retval)
			ສົ່ງຄືນ retval;
		start_blk += fs->inode_blocks_per_group;
		start_blk += ((fs->stride * group) %
			      (last_blk - start_blk + 1));
		ຖ້າ (start_blk >= last_blk)
			start_blk = group_blk;
	} ບໍ່ດັ່ງນັ້ນ
		start_blk = group_blk;

	ຖ້າ (flexbg_size) {
		blk64_t prev_block = 0;

		table_offset = flexbg_size;
		ຖ້າ (group % flexbg_size)
			prev_block = ext2fs_block_bitmap_loc(fs, group - 1) + 1;
		ບໍ່ດັ່ງນັ້ນ ຖ້າ (last_grp == fs->group_desc_count-1) {
			/*
			 * If we are allocating for the last flex_bg
			 * keep the metadata tables contiguous
			 */
			table_offset = last_grp & (flexbg_size - 1);
			ຖ້າ (table_offset == 0)
				table_offset = flexbg_size;
			ບໍ່ດັ່ງນັ້ນ
				table_offset++;
		}
		/* FIXME: Take backup group descriptor blocks into account
		 * if the flexbg allocations will grow to overlap them... */
		start_blk = flexbg_offset(fs, group, prev_block, bmap,
					  rem_grps, 1);
		last_blk = ext2fs_group_last_block2(fs, last_grp);
	}

	ຖ້າ (!ext2fs_block_bitmap_loc(fs, group)) {
		retval = ext2fs_get_free_blocks2(fs, start_blk, last_blk,
						 1, bmap, &new_blk);
		ຖ້າ (retval == EXT2_ET_BLOCK_ALLOC_FAIL)
			retval = ext2fs_get_free_blocks2(fs, group_blk,
					last_blk, 1, bmap, &new_blk);
		ຖ້າ (retval)
			ສົ່ງຄືນ retval;
		ext2fs_mark_block_bitmap2(bmap, new_blk);
		ext2fs_block_bitmap_loc_set(fs, group, new_blk);
		ຖ້າ (flexbg_size) {
			dgrp_t gr = ext2fs_group_of_blk2(fs, new_blk);
			ext2fs_bg_free_blocks_count_set(fs, gr, ext2fs_bg_free_blocks_count(fs, gr) - 1);
			ext2fs_free_blocks_count_add(fs->super, -1);
			ext2fs_bg_flags_clear(fs, gr, EXT2_BG_BLOCK_UNINIT);
			ext2fs_group_desc_csum_set(fs, gr);
		}
	}

	ຖ້າ (flexbg_size) {
		blk64_t prev_block = 0;
		ຖ້າ (group % flexbg_size)
			prev_block = ext2fs_inode_bitmap_loc(fs, group - 1) + 1;
		ບໍ່ດັ່ງນັ້ນ
			prev_block = ext2fs_block_bitmap_loc(fs, group) +
				table_offset;
		/* FIXME: Take backup group descriptor blocks into account
		 * if the flexbg allocations will grow to overlap them... */
		start_blk = flexbg_offset(fs, group, prev_block, bmap,
					  rem_grps, 1);
		last_blk = ext2fs_group_last_block2(fs, last_grp);
	}

	ຖ້າ (!ext2fs_inode_bitmap_loc(fs, group)) {
		retval = ext2fs_get_free_blocks2(fs, start_blk, last_blk,
						 1, bmap, &new_blk);
		ຖ້າ (retval == EXT2_ET_BLOCK_ALLOC_FAIL)
			retval = ext2fs_get_free_blocks2(fs, group_blk,
					 last_blk, 1, bmap, &new_blk);
		ຖ້າ (retval)
			ສົ່ງຄືນ retval;
		ext2fs_mark_block_bitmap2(bmap, new_blk);
		ext2fs_inode_bitmap_loc_set(fs, group, new_blk);
		ຖ້າ (flexbg_size) {
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
	ຖ້າ (flexbg_size) {
		blk64_t prev_block = 0;

		ຖ້າ (group % flexbg_size)
			prev_block = ext2fs_inode_table_loc(fs, group - 1) +
				fs->inode_blocks_per_group;
		ບໍ່ດັ່ງນັ້ນ
			prev_block = ext2fs_inode_bitmap_loc(fs, group) +
				table_offset;

		/* FIXME: Take backup group descriptor blocks into account
		 * if the flexbg allocations will grow to overlap them... */
		group_blk = flexbg_offset(fs, group, prev_block, bmap,
					  rem_grps, fs->inode_blocks_per_group);
		last_blk = ext2fs_group_last_block2(fs, last_grp);
	}

	ຖ້າ (!ext2fs_inode_table_loc(fs, group)) {
		retval = ext2fs_get_free_blocks2(fs, group_blk, last_blk,
						fs->inode_blocks_per_group,
						bmap, &new_blk);
		ຖ້າ (retval)
			ສົ່ງຄືນ retval;

		ext2fs_mark_block_bitmap_range2(bmap,
			new_blk, fs->inode_blocks_per_group);
		ຖ້າ (flexbg_size) {
			blk64_t num, blk;
			num = fs->inode_blocks_per_group;
			blk = new_blk;
			ໃນຂະນະທີ່ (num) {
				ຈຳນວນເຕັມ gr = ext2fs_group_of_blk2(fs, blk);
				last_blk = ext2fs_group_last_block2(fs, gr);
				blk64_t n = num;

				ຖ້າ (blk + num > last_blk)
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
	ສົ່ງຄືນ 0;
}

errcode_t ext2fs_allocate_tables(ext2_filsys fs)
{
	errcode_t	retval;
	dgrp_t		i;
	sa_nracanaa ext2fs_numeric_progress_struct progress;

	ຖ້າ (fs->progress_ops && fs->progress_ops->init)
		(fs->progress_ops->init)(fs, &progress, NULL,
					 fs->group_desc_count);

	ສຳລັບ (i = 0; i < fs->group_desc_count; i++) {
		ຖ້າ (fs->progress_ops && fs->progress_ops->update)
			(fs->progress_ops->update)(fs, &progress, i);
		retval = ext2fs_allocate_group_table(fs, i, fs->block_map);
		ຖ້າ (retval)
			ສົ່ງຄືນ retval;
	}
	ຖ້າ (fs->progress_ops && fs->progress_ops->close)
		(fs->progress_ops->close)(fs, &progress, NULL);
	ສົ່ງຄືນ 0;
}

