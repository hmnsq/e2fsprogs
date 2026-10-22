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
#অন্তর্ভাব "ext2fsP.h"

/*
 * This routine searches for free blocks that can allocate a full
 * group of bitmaps or inode tables for a flexbg group.  Returns the
 * block number with a correct offset were the bitmaps and inode
 * tables can be allocated continuously and in order.
 */
স্থির blk64_t flexbg_offset(ext2_filsys fs, dgrp_t group, blk64_t start_blk,
			     ext2fs_block_bitmap bmap, পূর্ণসংখ্যা rem_grp,
			     পূর্ণসংখ্যা elem_আকার)
{
	পূর্ণসংখ্যা		flexbg, flexbg_আকার, আকার_2;
	blk64_t		last_blk, first_free = 0;
	dgrp_t	       	last_grp;

	flexbg_আকার = 1U << fs->super->s_log_groups_per_flex;
	flexbg = group / flexbg_আকার;
	আকার_2 = rem_grp * elem_আকার;

	যদি (আকার_2 > (পূর্ণসংখ্যা) (fs->super->s_blocks_per_group / 4))
		আকার_2 = (পূর্ণসংখ্যা) fs->super->s_blocks_per_group / 4;

	/*
	 * Don't do a long search if the previous block search is still valid,
	 * but skip minor obstructions such as group descriptor backups.
	 */
	যদি (start_blk && start_blk < ext2fs_blocks_গুনতি(fs->super) &&
	    ext2fs_get_free_blocks2(fs, start_blk, start_blk + আকার_2, elem_আকার,
				    bmap, &first_free) == 0)
		ফেরত first_free;

	start_blk = ext2fs_group_first_block2(fs, flexbg_আকার * flexbg);
	last_grp = group | (flexbg_আকার - 1);
	যদি (last_grp > fs->group_desc_গুনতি-1)
		last_grp = fs->group_desc_গুনতি-1;
	last_blk = ext2fs_group_last_block2(fs, last_grp);

	/* Find the first available block */
	যদি (ext2fs_get_free_blocks2(fs, start_blk, last_blk, আকার_2,
				    bmap, &first_free) == 0)
		ফেরত first_free;

	যদি (ext2fs_get_free_blocks2(fs, start_blk, last_blk, elem_আকার,
				   bmap, &first_free) == 0)
		ফেরত first_free;

	যদি (ext2fs_get_free_blocks2(fs, 0, last_blk, elem_আকার, bmap,
				    &first_free) == 0)
		ফেরত first_free;

	ফেরত first_free;
}

errcode_t ext2fs_allocate_group_table(ext2_filsys fs, dgrp_t group,
				      ext2fs_block_bitmap bmap)
{
	errcode_t	retval;
	blk64_t		group_blk, start_blk, last_blk, new_blk;
	dgrp_t		last_grp = 0;
	পূর্ণসংখ্যা		rem_grps = 0, flexbg_আকার = 0, table_offset = 0;

	group_blk = ext2fs_group_first_block2(fs, group);
	last_blk = ext2fs_group_last_block2(fs, group);

	যদি (!bmap)
		bmap = fs->block_map;

	যদি (ext2fs_has_feature_flex_bg(fs->super) &&
	    fs->super->s_log_groups_per_flex) {
		flexbg_আকার = 1U << fs->super->s_log_groups_per_flex;
		last_grp = group | (flexbg_আকার - 1);
		যদি (last_grp > fs->group_desc_গুনতি-1)
			last_grp = fs->group_desc_গুনতি-1;
		rem_grps = last_grp - group + 1;
	}

	/*
	 * Allocate the block and inode bitmaps, if necessary
	 */
	যদি (fs->stride && !flexbg_আকার) {
		retval = ext2fs_get_free_blocks2(fs, group_blk, last_blk,
						 1, bmap, &start_blk);
		যদি (retval)
			ফেরত retval;
		start_blk += fs->inode_blocks_per_group;
		start_blk += ((fs->stride * group) %
			      (last_blk - start_blk + 1));
		যদি (start_blk >= last_blk)
			start_blk = group_blk;
	} অন্যথা
		start_blk = group_blk;

	যদি (flexbg_আকার) {
		blk64_t পূর্ব_block = 0;

		table_offset = flexbg_আকার;
		যদি (group % flexbg_আকার)
			পূর্ব_block = ext2fs_block_bitmap_loc(fs, group - 1) + 1;
		অন্যথা যদি (last_grp == fs->group_desc_গুনতি-1) {
			/*
			 * If we are allocating for the last flex_bg
			 * keep the metadata tables contiguous
			 */
			table_offset = last_grp & (flexbg_আকার - 1);
			যদি (table_offset == 0)
				table_offset = flexbg_আকার;
			অন্যথা
				table_offset++;
		}
		/* FIXME: Take backup group descriptor blocks into account
		 * if the flexbg allocations will grow to overlap them... */
		start_blk = flexbg_offset(fs, group, পূর্ব_block, bmap,
					  rem_grps, 1);
		last_blk = ext2fs_group_last_block2(fs, last_grp);
	}

	যদি (!ext2fs_block_bitmap_loc(fs, group)) {
		retval = ext2fs_get_free_blocks2(fs, start_blk, last_blk,
						 1, bmap, &new_blk);
		যদি (retval == EXT2_ET_BLOCK_ALLOC_FAIL)
			retval = ext2fs_get_free_blocks2(fs, group_blk,
					last_blk, 1, bmap, &new_blk);
		যদি (retval)
			ফেরত retval;
		ext2fs_mark_block_bitmap2(bmap, new_blk);
		ext2fs_block_bitmap_loc_set(fs, group, new_blk);
		যদি (flexbg_আকার) {
			dgrp_t gr = ext2fs_group_of_blk2(fs, new_blk);
			ext2fs_bg_free_blocks_গুনতি_set(fs, gr, ext2fs_bg_free_blocks_গুনতি(fs, gr) - 1);
			ext2fs_free_blocks_গুনতি_add(fs->super, -1);
			ext2fs_bg_flags_clear(fs, gr, EXT2_BG_BLOCK_UNINIT);
			ext2fs_group_desc_csum_set(fs, gr);
		}
	}

	যদি (flexbg_আকার) {
		blk64_t পূর্ব_block = 0;
		যদি (group % flexbg_আকার)
			পূর্ব_block = ext2fs_inode_bitmap_loc(fs, group - 1) + 1;
		অন্যথা
			পূর্ব_block = ext2fs_block_bitmap_loc(fs, group) +
				table_offset;
		/* FIXME: Take backup group descriptor blocks into account
		 * if the flexbg allocations will grow to overlap them... */
		start_blk = flexbg_offset(fs, group, পূর্ব_block, bmap,
					  rem_grps, 1);
		last_blk = ext2fs_group_last_block2(fs, last_grp);
	}

	যদি (!ext2fs_inode_bitmap_loc(fs, group)) {
		retval = ext2fs_get_free_blocks2(fs, start_blk, last_blk,
						 1, bmap, &new_blk);
		যদি (retval == EXT2_ET_BLOCK_ALLOC_FAIL)
			retval = ext2fs_get_free_blocks2(fs, group_blk,
					 last_blk, 1, bmap, &new_blk);
		যদি (retval)
			ফেরত retval;
		ext2fs_mark_block_bitmap2(bmap, new_blk);
		ext2fs_inode_bitmap_loc_set(fs, group, new_blk);
		যদি (flexbg_আকার) {
			dgrp_t gr = ext2fs_group_of_blk2(fs, new_blk);
			ext2fs_bg_free_blocks_গুনতি_set(fs, gr, ext2fs_bg_free_blocks_গুনতি(fs, gr) - 1);
			ext2fs_free_blocks_গুনতি_add(fs->super, -1);
			ext2fs_bg_flags_clear(fs, gr, EXT2_BG_BLOCK_UNINIT);
			ext2fs_group_desc_csum_set(fs, gr);
		}
	}

	/*
	 * Allocate the inode table
	 */
	যদি (flexbg_আকার) {
		blk64_t পূর্ব_block = 0;

		যদি (group % flexbg_আকার)
			পূর্ব_block = ext2fs_inode_table_loc(fs, group - 1) +
				fs->inode_blocks_per_group;
		অন্যথা
			পূর্ব_block = ext2fs_inode_bitmap_loc(fs, group) +
				table_offset;

		/* FIXME: Take backup group descriptor blocks into account
		 * if the flexbg allocations will grow to overlap them... */
		group_blk = flexbg_offset(fs, group, পূর্ব_block, bmap,
					  rem_grps, fs->inode_blocks_per_group);
		last_blk = ext2fs_group_last_block2(fs, last_grp);
	}

	যদি (!ext2fs_inode_table_loc(fs, group)) {
		retval = ext2fs_get_free_blocks2(fs, group_blk, last_blk,
						fs->inode_blocks_per_group,
						bmap, &new_blk);
		যদি (retval)
			ফেরত retval;

		ext2fs_mark_block_bitmap_range2(bmap,
			new_blk, fs->inode_blocks_per_group);
		যদি (flexbg_আকার) {
			blk64_t অঙ্ক, blk;
			অঙ্ক = fs->inode_blocks_per_group;
			blk = new_blk;
			যখন (অঙ্ক) {
				পূর্ণসংখ্যা gr = ext2fs_group_of_blk2(fs, blk);
				last_blk = ext2fs_group_last_block2(fs, gr);
				blk64_t n = অঙ্ক;

				যদি (blk + অঙ্ক > last_blk)
					n = last_blk - blk + 1;

				ext2fs_bg_free_blocks_গুনতি_set(fs, gr,
					ext2fs_bg_free_blocks_গুনতি(fs, gr) -
					n/EXT2FS_CLUSTER_RATIO(fs));
				ext2fs_bg_flags_clear(fs, gr,
					EXT2_BG_BLOCK_UNINIT);
				ext2fs_group_desc_csum_set(fs, gr);
				ext2fs_free_blocks_গুনতি_add(fs->super, -n);
				blk += n;
				অঙ্ক -= n;
			}
		}
		ext2fs_inode_table_loc_set(fs, group, new_blk);
	}
	ext2fs_group_desc_csum_set(fs, group);
	ফেরত 0;
}

errcode_t ext2fs_allocate_tables(ext2_filsys fs)
{
	errcode_t	retval;
	dgrp_t		i;
	কাঠামো ext2fs_numeric_progress_struct progress;

	যদি (fs->progress_ops && fs->progress_ops->init)
		(fs->progress_ops->init)(fs, &progress, ফাঁকা,
					 fs->group_desc_গুনতি);

	জন্য (i = 0; i < fs->group_desc_গুনতি; i++) {
		যদি (fs->progress_ops && fs->progress_ops->update)
			(fs->progress_ops->update)(fs, &progress, i);
		retval = ext2fs_allocate_group_table(fs, i, fs->block_map);
		যদি (retval)
			ফেরত retval;
	}
	যদি (fs->progress_ops && fs->progress_ops->close)
		(fs->progress_ops->close)(fs, &progress, ফাঁকা);
	ফেরত 0;
}

