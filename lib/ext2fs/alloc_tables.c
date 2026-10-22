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

#அந்தர்பாவ "config.h"
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <string.h>
#எனில் HAVE_UNISTD_H
#அந்தர்பாவ <unistd.h>
#endif
#அந்தர்பாவ <fcntl.h>
#அந்தர்பாவ <time.h>
#எனில் HAVE_SYS_STAT_H
#அந்தர்பாவ <sys/stat.h>
#endif
#எனில் HAVE_SYS_TYPES_H
#அந்தர்பாவ <sys/types.h>
#endif

#அந்தர்பாவ "ext2_fs.h"
#அந்தர்பாவ "ext2fs.h"
#அந்தர்பாவ "ext2fsP.h"

/*
 * This routine searches for free blocks that can allocate a full
 * group of bitmaps or inode tables for a flexbg group.  Returns the
 * block number with a correct offset were the bitmaps and inode
 * tables can be allocated continuously and in order.
 */
நிலையான blk64_t flexbg_offset(ext2_filsys fs, dgrp_t group, blk64_t start_blk,
			     ext2fs_block_bitmap bmap, முழு rem_grp,
			     முழு elem_size)
{
	முழு		flexbg, flexbg_size, size;
	blk64_t		last_blk, first_free = 0;
	dgrp_t	       	last_grp;

	flexbg_size = 1U << fs->super->s_log_groups_per_flex;
	flexbg = group / flexbg_size;
	size = rem_grp * elem_size;

	எனில் (size > (முழு) (fs->super->s_blocks_per_group / 4))
		size = (முழு) fs->super->s_blocks_per_group / 4;

	/*
	 * Don't do a long search if the previous block search is still valid,
	 * but skip minor obstructions such as group descriptor backups.
	 */
	எனில் (start_blk && start_blk < ext2fs_blocks_மொத்தம்(fs->super) &&
	    ext2fs_get_free_blocks2(fs, start_blk, start_blk + size, elem_size,
				    bmap, &first_free) == 0)
		திரும்பு first_free;

	start_blk = ext2fs_group_first_block2(fs, flexbg_size * flexbg);
	last_grp = group | (flexbg_size - 1);
	எனில் (last_grp > fs->group_desc_மொத்தம்-1)
		last_grp = fs->group_desc_மொத்தம்-1;
	last_blk = ext2fs_group_last_block2(fs, last_grp);

	/* Find the first available block */
	எனில் (ext2fs_get_free_blocks2(fs, start_blk, last_blk, size,
				    bmap, &first_free) == 0)
		திரும்பு first_free;

	எனில் (ext2fs_get_free_blocks2(fs, start_blk, last_blk, elem_size,
				   bmap, &first_free) == 0)
		திரும்பு first_free;

	எனில் (ext2fs_get_free_blocks2(fs, 0, last_blk, elem_size, bmap,
				    &first_free) == 0)
		திரும்பு first_free;

	திரும்பு first_free;
}

errcode_t ext2fs_allocate_group_table(ext2_filsys fs, dgrp_t group,
				      ext2fs_block_bitmap bmap)
{
	errcode_t	retval;
	blk64_t		group_blk, start_blk, last_blk, new_blk;
	dgrp_t		last_grp = 0;
	முழு		rem_grps = 0, flexbg_size = 0, table_offset = 0;

	group_blk = ext2fs_group_first_block2(fs, group);
	last_blk = ext2fs_group_last_block2(fs, group);

	எனில் (!bmap)
		bmap = fs->block_map;

	எனில் (ext2fs_has_feature_flex_bg(fs->super) &&
	    fs->super->s_log_groups_per_flex) {
		flexbg_size = 1U << fs->super->s_log_groups_per_flex;
		last_grp = group | (flexbg_size - 1);
		எனில் (last_grp > fs->group_desc_மொத்தம்-1)
			last_grp = fs->group_desc_மொத்தம்-1;
		rem_grps = last_grp - group + 1;
	}

	/*
	 * Allocate the block and inode bitmaps, if necessary
	 */
	எனில் (fs->stride && !flexbg_size) {
		retval = ext2fs_get_free_blocks2(fs, group_blk, last_blk,
						 1, bmap, &start_blk);
		எனில் (retval)
			திரும்பு retval;
		start_blk += fs->inode_blocks_per_group;
		start_blk += ((fs->stride * group) %
			      (last_blk - start_blk + 1));
		எனில் (start_blk >= last_blk)
			start_blk = group_blk;
	} இல்லையெனில்
		start_blk = group_blk;

	எனில் (flexbg_size) {
		blk64_t prev_block = 0;

		table_offset = flexbg_size;
		எனில் (group % flexbg_size)
			prev_block = ext2fs_block_bitmap_loc(fs, group - 1) + 1;
		இல்லையெனில் எனில் (last_grp == fs->group_desc_மொத்தம்-1) {
			/*
			 * If we are allocating for the last flex_bg
			 * keep the metadata tables contiguous
			 */
			table_offset = last_grp & (flexbg_size - 1);
			எனில் (table_offset == 0)
				table_offset = flexbg_size;
			இல்லையெனில்
				table_offset++;
		}
		/* FIXME: Take backup group descriptor blocks into account
		 * if the flexbg allocations will grow to overlap them... */
		start_blk = flexbg_offset(fs, group, prev_block, bmap,
					  rem_grps, 1);
		last_blk = ext2fs_group_last_block2(fs, last_grp);
	}

	எனில் (!ext2fs_block_bitmap_loc(fs, group)) {
		retval = ext2fs_get_free_blocks2(fs, start_blk, last_blk,
						 1, bmap, &new_blk);
		எனில் (retval == EXT2_ET_BLOCK_ALLOC_FAIL)
			retval = ext2fs_get_free_blocks2(fs, group_blk,
					last_blk, 1, bmap, &new_blk);
		எனில் (retval)
			திரும்பு retval;
		ext2fs_mark_block_bitmap2(bmap, new_blk);
		ext2fs_block_bitmap_loc_set(fs, group, new_blk);
		எனில் (flexbg_size) {
			dgrp_t gr = ext2fs_group_of_blk2(fs, new_blk);
			ext2fs_bg_free_blocks_மொத்தம்_set(fs, gr, ext2fs_bg_free_blocks_மொத்தம்(fs, gr) - 1);
			ext2fs_free_blocks_மொத்தம்_add(fs->super, -1);
			ext2fs_bg_flags_clear(fs, gr, EXT2_BG_BLOCK_UNINIT);
			ext2fs_group_desc_csum_set(fs, gr);
		}
	}

	எனில் (flexbg_size) {
		blk64_t prev_block = 0;
		எனில் (group % flexbg_size)
			prev_block = ext2fs_inode_bitmap_loc(fs, group - 1) + 1;
		இல்லையெனில்
			prev_block = ext2fs_block_bitmap_loc(fs, group) +
				table_offset;
		/* FIXME: Take backup group descriptor blocks into account
		 * if the flexbg allocations will grow to overlap them... */
		start_blk = flexbg_offset(fs, group, prev_block, bmap,
					  rem_grps, 1);
		last_blk = ext2fs_group_last_block2(fs, last_grp);
	}

	எனில் (!ext2fs_inode_bitmap_loc(fs, group)) {
		retval = ext2fs_get_free_blocks2(fs, start_blk, last_blk,
						 1, bmap, &new_blk);
		எனில் (retval == EXT2_ET_BLOCK_ALLOC_FAIL)
			retval = ext2fs_get_free_blocks2(fs, group_blk,
					 last_blk, 1, bmap, &new_blk);
		எனில் (retval)
			திரும்பு retval;
		ext2fs_mark_block_bitmap2(bmap, new_blk);
		ext2fs_inode_bitmap_loc_set(fs, group, new_blk);
		எனில் (flexbg_size) {
			dgrp_t gr = ext2fs_group_of_blk2(fs, new_blk);
			ext2fs_bg_free_blocks_மொத்தம்_set(fs, gr, ext2fs_bg_free_blocks_மொத்தம்(fs, gr) - 1);
			ext2fs_free_blocks_மொத்தம்_add(fs->super, -1);
			ext2fs_bg_flags_clear(fs, gr, EXT2_BG_BLOCK_UNINIT);
			ext2fs_group_desc_csum_set(fs, gr);
		}
	}

	/*
	 * Allocate the inode table
	 */
	எனில் (flexbg_size) {
		blk64_t prev_block = 0;

		எனில் (group % flexbg_size)
			prev_block = ext2fs_inode_table_loc(fs, group - 1) +
				fs->inode_blocks_per_group;
		இல்லையெனில்
			prev_block = ext2fs_inode_bitmap_loc(fs, group) +
				table_offset;

		/* FIXME: Take backup group descriptor blocks into account
		 * if the flexbg allocations will grow to overlap them... */
		group_blk = flexbg_offset(fs, group, prev_block, bmap,
					  rem_grps, fs->inode_blocks_per_group);
		last_blk = ext2fs_group_last_block2(fs, last_grp);
	}

	எனில் (!ext2fs_inode_table_loc(fs, group)) {
		retval = ext2fs_get_free_blocks2(fs, group_blk, last_blk,
						fs->inode_blocks_per_group,
						bmap, &new_blk);
		எனில் (retval)
			திரும்பு retval;

		ext2fs_mark_block_bitmap_range2(bmap,
			new_blk, fs->inode_blocks_per_group);
		எனில் (flexbg_size) {
			blk64_t இலக்கம், blk;
			இலக்கம் = fs->inode_blocks_per_group;
			blk = new_blk;
			போது (இலக்கம்) {
				முழு gr = ext2fs_group_of_blk2(fs, blk);
				last_blk = ext2fs_group_last_block2(fs, gr);
				blk64_t n = இலக்கம்;

				எனில் (blk + இலக்கம் > last_blk)
					n = last_blk - blk + 1;

				ext2fs_bg_free_blocks_மொத்தம்_set(fs, gr,
					ext2fs_bg_free_blocks_மொத்தம்(fs, gr) -
					n/EXT2FS_CLUSTER_RATIO(fs));
				ext2fs_bg_flags_clear(fs, gr,
					EXT2_BG_BLOCK_UNINIT);
				ext2fs_group_desc_csum_set(fs, gr);
				ext2fs_free_blocks_மொத்தம்_add(fs->super, -n);
				blk += n;
				இலக்கம் -= n;
			}
		}
		ext2fs_inode_table_loc_set(fs, group, new_blk);
	}
	ext2fs_group_desc_csum_set(fs, group);
	திரும்பு 0;
}

errcode_t ext2fs_allocate_tables(ext2_filsys fs)
{
	errcode_t	retval;
	dgrp_t		i;
	கட்டமைப்பு ext2fs_numeric_progress_struct progress;

	எனில் (fs->progress_ops && fs->progress_ops->init)
		(fs->progress_ops->init)(fs, &progress, வெற்று,
					 fs->group_desc_மொத்தம்);

	க்கு (i = 0; i < fs->group_desc_மொத்தம்; i++) {
		எனில் (fs->progress_ops && fs->progress_ops->update)
			(fs->progress_ops->update)(fs, &progress, i);
		retval = ext2fs_allocate_group_table(fs, i, fs->block_map);
		எனில் (retval)
			திரும்பு retval;
	}
	எனில் (fs->progress_ops && fs->progress_ops->close)
		(fs->progress_ops->close)(fs, &progress, வெற்று);
	திரும்பு 0;
}

