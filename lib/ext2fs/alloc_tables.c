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

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <time.h>
#यदि HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#endif
#यदि HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#endif

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"
#अन्तर्भाव "ext2fsP.h"

/*
 * This routine searches for free blocks that can allocate a full
 * group of bitmaps or inode tables for a flexbg group.  Returns the
 * block number with a correct offset were the bitmaps and inode
 * tables can be allocated continuously and in order.
 */
स्थिर blk64_t flexbg_offset(ext2_filsys fs, dgrp_t group, blk64_t start_blk,
			     ext2fs_block_bitmap bmap, पूर्णाङ्क rem_grp,
			     पूर्णाङ्क elem_size)
{
	पूर्णाङ्क		flexbg, flexbg_size, size;
	blk64_t		last_blk, first_free = 0;
	dgrp_t	       	last_grp;

	flexbg_size = 1U << fs->super->s_log_groups_per_flex;
	flexbg = group / flexbg_size;
	size = rem_grp * elem_size;

	यदि (size > (पूर्णाङ्क) (fs->super->s_blocks_per_group / 4))
		size = (पूर्णाङ्क) fs->super->s_blocks_per_group / 4;

	/*
	 * Don't do a long search if the previous block search is still valid,
	 * but skip minor obstructions such as group descriptor backups.
	 */
	यदि (start_blk && start_blk < ext2fs_blocks_गिनती(fs->super) &&
	    ext2fs_get_free_blocks2(fs, start_blk, start_blk + size, elem_size,
				    bmap, &first_free) == 0)
		प्रत्यागम first_free;

	start_blk = ext2fs_group_first_block2(fs, flexbg_size * flexbg);
	last_grp = group | (flexbg_size - 1);
	यदि (last_grp > fs->group_desc_गिनती-1)
		last_grp = fs->group_desc_गिनती-1;
	last_blk = ext2fs_group_last_block2(fs, last_grp);

	/* Find the first available block */
	यदि (ext2fs_get_free_blocks2(fs, start_blk, last_blk, size,
				    bmap, &first_free) == 0)
		प्रत्यागम first_free;

	यदि (ext2fs_get_free_blocks2(fs, start_blk, last_blk, elem_size,
				   bmap, &first_free) == 0)
		प्रत्यागम first_free;

	यदि (ext2fs_get_free_blocks2(fs, 0, last_blk, elem_size, bmap,
				    &first_free) == 0)
		प्रत्यागम first_free;

	प्रत्यागम first_free;
}

errcode_t ext2fs_allocate_group_table(ext2_filsys fs, dgrp_t group,
				      ext2fs_block_bitmap bmap)
{
	errcode_t	retval;
	blk64_t		group_blk, start_blk, last_blk, new_blk;
	dgrp_t		last_grp = 0;
	पूर्णाङ्क		rem_grps = 0, flexbg_size = 0, table_offset = 0;

	group_blk = ext2fs_group_first_block2(fs, group);
	last_blk = ext2fs_group_last_block2(fs, group);

	यदि (!bmap)
		bmap = fs->block_map;

	यदि (ext2fs_has_feature_flex_bg(fs->super) &&
	    fs->super->s_log_groups_per_flex) {
		flexbg_size = 1U << fs->super->s_log_groups_per_flex;
		last_grp = group | (flexbg_size - 1);
		यदि (last_grp > fs->group_desc_गिनती-1)
			last_grp = fs->group_desc_गिनती-1;
		rem_grps = last_grp - group + 1;
	}

	/*
	 * Allocate the block and inode bitmaps, if necessary
	 */
	यदि (fs->stride && !flexbg_size) {
		retval = ext2fs_get_free_blocks2(fs, group_blk, last_blk,
						 1, bmap, &start_blk);
		यदि (retval)
			प्रत्यागम retval;
		start_blk += fs->inode_blocks_per_group;
		start_blk += ((fs->stride * group) %
			      (last_blk - start_blk + 1));
		यदि (start_blk >= last_blk)
			start_blk = group_blk;
	} अन्यथा
		start_blk = group_blk;

	यदि (flexbg_size) {
		blk64_t prev_block = 0;

		table_offset = flexbg_size;
		यदि (group % flexbg_size)
			prev_block = ext2fs_block_bitmap_loc(fs, group - 1) + 1;
		अन्यथा यदि (last_grp == fs->group_desc_गिनती-1) {
			/*
			 * If we are allocating for the last flex_bg
			 * keep the metadata tables contiguous
			 */
			table_offset = last_grp & (flexbg_size - 1);
			यदि (table_offset == 0)
				table_offset = flexbg_size;
			अन्यथा
				table_offset++;
		}
		/* FIXME: Take backup group descriptor blocks into account
		 * if the flexbg allocations will grow to overlap them... */
		start_blk = flexbg_offset(fs, group, prev_block, bmap,
					  rem_grps, 1);
		last_blk = ext2fs_group_last_block2(fs, last_grp);
	}

	यदि (!ext2fs_block_bitmap_loc(fs, group)) {
		retval = ext2fs_get_free_blocks2(fs, start_blk, last_blk,
						 1, bmap, &new_blk);
		यदि (retval == EXT2_ET_BLOCK_ALLOC_FAIL)
			retval = ext2fs_get_free_blocks2(fs, group_blk,
					last_blk, 1, bmap, &new_blk);
		यदि (retval)
			प्रत्यागम retval;
		ext2fs_mark_block_bitmap2(bmap, new_blk);
		ext2fs_block_bitmap_loc_set(fs, group, new_blk);
		यदि (flexbg_size) {
			dgrp_t gr = ext2fs_group_of_blk2(fs, new_blk);
			ext2fs_bg_free_blocks_गिनती_set(fs, gr, ext2fs_bg_free_blocks_गिनती(fs, gr) - 1);
			ext2fs_free_blocks_गिनती_add(fs->super, -1);
			ext2fs_bg_flags_clear(fs, gr, EXT2_BG_BLOCK_UNINIT);
			ext2fs_group_desc_csum_set(fs, gr);
		}
	}

	यदि (flexbg_size) {
		blk64_t prev_block = 0;
		यदि (group % flexbg_size)
			prev_block = ext2fs_inode_bitmap_loc(fs, group - 1) + 1;
		अन्यथा
			prev_block = ext2fs_block_bitmap_loc(fs, group) +
				table_offset;
		/* FIXME: Take backup group descriptor blocks into account
		 * if the flexbg allocations will grow to overlap them... */
		start_blk = flexbg_offset(fs, group, prev_block, bmap,
					  rem_grps, 1);
		last_blk = ext2fs_group_last_block2(fs, last_grp);
	}

	यदि (!ext2fs_inode_bitmap_loc(fs, group)) {
		retval = ext2fs_get_free_blocks2(fs, start_blk, last_blk,
						 1, bmap, &new_blk);
		यदि (retval == EXT2_ET_BLOCK_ALLOC_FAIL)
			retval = ext2fs_get_free_blocks2(fs, group_blk,
					 last_blk, 1, bmap, &new_blk);
		यदि (retval)
			प्रत्यागम retval;
		ext2fs_mark_block_bitmap2(bmap, new_blk);
		ext2fs_inode_bitmap_loc_set(fs, group, new_blk);
		यदि (flexbg_size) {
			dgrp_t gr = ext2fs_group_of_blk2(fs, new_blk);
			ext2fs_bg_free_blocks_गिनती_set(fs, gr, ext2fs_bg_free_blocks_गिनती(fs, gr) - 1);
			ext2fs_free_blocks_गिनती_add(fs->super, -1);
			ext2fs_bg_flags_clear(fs, gr, EXT2_BG_BLOCK_UNINIT);
			ext2fs_group_desc_csum_set(fs, gr);
		}
	}

	/*
	 * Allocate the inode table
	 */
	यदि (flexbg_size) {
		blk64_t prev_block = 0;

		यदि (group % flexbg_size)
			prev_block = ext2fs_inode_table_loc(fs, group - 1) +
				fs->inode_blocks_per_group;
		अन्यथा
			prev_block = ext2fs_inode_bitmap_loc(fs, group) +
				table_offset;

		/* FIXME: Take backup group descriptor blocks into account
		 * if the flexbg allocations will grow to overlap them... */
		group_blk = flexbg_offset(fs, group, prev_block, bmap,
					  rem_grps, fs->inode_blocks_per_group);
		last_blk = ext2fs_group_last_block2(fs, last_grp);
	}

	यदि (!ext2fs_inode_table_loc(fs, group)) {
		retval = ext2fs_get_free_blocks2(fs, group_blk, last_blk,
						fs->inode_blocks_per_group,
						bmap, &new_blk);
		यदि (retval)
			प्रत्यागम retval;

		ext2fs_mark_block_bitmap_range2(bmap,
			new_blk, fs->inode_blocks_per_group);
		यदि (flexbg_size) {
			blk64_t अंक, blk;
			अंक = fs->inode_blocks_per_group;
			blk = new_blk;
			यावत् (अंक) {
				पूर्णाङ्क gr = ext2fs_group_of_blk2(fs, blk);
				last_blk = ext2fs_group_last_block2(fs, gr);
				blk64_t n = अंक;

				यदि (blk + अंक > last_blk)
					n = last_blk - blk + 1;

				ext2fs_bg_free_blocks_गिनती_set(fs, gr,
					ext2fs_bg_free_blocks_गिनती(fs, gr) -
					n/EXT2FS_CLUSTER_RATIO(fs));
				ext2fs_bg_flags_clear(fs, gr,
					EXT2_BG_BLOCK_UNINIT);
				ext2fs_group_desc_csum_set(fs, gr);
				ext2fs_free_blocks_गिनती_add(fs->super, -n);
				blk += n;
				अंक -= n;
			}
		}
		ext2fs_inode_table_loc_set(fs, group, new_blk);
	}
	ext2fs_group_desc_csum_set(fs, group);
	प्रत्यागम 0;
}

errcode_t ext2fs_allocate_tables(ext2_filsys fs)
{
	errcode_t	retval;
	dgrp_t		i;
	संरचना ext2fs_numeric_progress_struct progress;

	यदि (fs->progress_ops && fs->progress_ops->init)
		(fs->progress_ops->init)(fs, &progress, रिक्त,
					 fs->group_desc_गिनती);

	प्रति (i = 0; i < fs->group_desc_गिनती; i++) {
		यदि (fs->progress_ops && fs->progress_ops->update)
			(fs->progress_ops->update)(fs, &progress, i);
		retval = ext2fs_allocate_group_table(fs, i, fs->block_map);
		यदि (retval)
			प्रत्यागम retval;
	}
	यदि (fs->progress_ops && fs->progress_ops->close)
		(fs->progress_ops->close)(fs, &progress, रिक्त);
	प्रत्यागम 0;
}

