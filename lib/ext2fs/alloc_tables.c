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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಆದರೆ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <time.h>
#ಆದರೆ HAVE_SYS_STAT_H
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_SYS_TYPES_H
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fsP.h"

/*
 * This routine searches for free blocks that can allocate a full
 * group of bitmaps or inode tables for a flexbg group.  Returns the
 * block number with a correct offset were the bitmaps and inode
 * tables can be allocated continuously and in order.
 */
ಸ್ಥಿರ blk64_t flexbg_ಸ್ಥಾನಾಂತರ(ext2_filsys fs, dgrp_t ಗುಂಪು, blk64_t ಆರಂಭ_blk,
			     ext2fs_ಖಂಡ_bitmap bmap, ಪೂರ್ಣಾಂಕ rem_grp,
			     ಪೂರ್ಣಾಂಕ elem_ಗಾತ್ರ)
{
	ಪೂರ್ಣಾಂಕ		flexbg, flexbg_ಗಾತ್ರ, ಗಾತ್ರ_2;
	blk64_t		ಕೊನೆಯ_blk, ಮೊದಲ_free = 0;
	dgrp_t	       	ಕೊನೆಯ_grp;

	flexbg_ಗಾತ್ರ = 1U << fs->super->s_log_groups_ಪ್ರತಿ_flex;
	flexbg = ಗುಂಪು / flexbg_ಗಾತ್ರ;
	ಗಾತ್ರ_2 = rem_grp * elem_ಗಾತ್ರ;

	ಆದರೆ (ಗಾತ್ರ_2 > (ಪೂರ್ಣಾಂಕ) (fs->super->s_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು / 4))
		ಗಾತ್ರ_2 = (ಪೂರ್ಣಾಂಕ) fs->super->s_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು / 4;

	/*
	 * Don't do a long search if the previous block search is still valid,
	 * but skip minor obstructions such as group descriptor backups.
	 */
	ಆದರೆ (ಆರಂಭ_blk && ಆರಂಭ_blk < ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(fs->super) &&
	    ext2fs_get_free_blocks2(fs, ಆರಂಭ_blk, ಆರಂಭ_blk + ಗಾತ್ರ_2, elem_ಗಾತ್ರ,
				    bmap, &ಮೊದಲ_free) == 0)
		ಹಿಂತಿರುಗಿ ಮೊದಲ_free;

	ಆರಂಭ_blk = ext2fs_ಗುಂಪು_ಮೊದಲ_block2(fs, flexbg_ಗಾತ್ರ * flexbg);
	ಕೊನೆಯ_grp = ಗುಂಪು | (flexbg_ಗಾತ್ರ - 1);
	ಆದರೆ (ಕೊನೆಯ_grp > fs->ಗುಂಪು_desc_ಎಣಿಕೆ-1)
		ಕೊನೆಯ_grp = fs->ಗುಂಪು_desc_ಎಣಿಕೆ-1;
	ಕೊನೆಯ_blk = ext2fs_ಗುಂಪು_ಕೊನೆಯ_block2(fs, ಕೊನೆಯ_grp);

	/* Find the first available block */
	ಆದರೆ (ext2fs_get_free_blocks2(fs, ಆರಂಭ_blk, ಕೊನೆಯ_blk, ಗಾತ್ರ_2,
				    bmap, &ಮೊದಲ_free) == 0)
		ಹಿಂತಿರುಗಿ ಮೊದಲ_free;

	ಆದರೆ (ext2fs_get_free_blocks2(fs, ಆರಂಭ_blk, ಕೊನೆಯ_blk, elem_ಗಾತ್ರ,
				   bmap, &ಮೊದಲ_free) == 0)
		ಹಿಂತಿರುಗಿ ಮೊದಲ_free;

	ಆದರೆ (ext2fs_get_free_blocks2(fs, 0, ಕೊನೆಯ_blk, elem_ಗಾತ್ರ, bmap,
				    &ಮೊದಲ_free) == 0)
		ಹಿಂತಿರುಗಿ ಮೊದಲ_free;

	ಹಿಂತಿರುಗಿ ಮೊದಲ_free;
}

errcode_t ext2fs_allocate_ಗುಂಪು_ಕೋಷ್ಟಕ(ext2_filsys fs, dgrp_t ಗುಂಪು,
				      ext2fs_ಖಂಡ_bitmap bmap)
{
	errcode_t	retval;
	blk64_t		ಗುಂಪು_blk, ಆರಂಭ_blk, ಕೊನೆಯ_blk, new_blk;
	dgrp_t		ಕೊನೆಯ_grp = 0;
	ಪೂರ್ಣಾಂಕ		rem_grps = 0, flexbg_ಗಾತ್ರ = 0, ಕೋಷ್ಟಕ_ಸ್ಥಾನಾಂತರ = 0;

	ಗುಂಪು_blk = ext2fs_ಗುಂಪು_ಮೊದಲ_block2(fs, ಗುಂಪು);
	ಕೊನೆಯ_blk = ext2fs_ಗುಂಪು_ಕೊನೆಯ_block2(fs, ಗುಂಪು);

	ಆದರೆ (!bmap)
		bmap = fs->ಖಂಡ_map;

	ಆದರೆ (ext2fs_has_feature_flex_bg(fs->super) &&
	    fs->super->s_log_groups_ಪ್ರತಿ_flex) {
		flexbg_ಗಾತ್ರ = 1U << fs->super->s_log_groups_ಪ್ರತಿ_flex;
		ಕೊನೆಯ_grp = ಗುಂಪು | (flexbg_ಗಾತ್ರ - 1);
		ಆದರೆ (ಕೊನೆಯ_grp > fs->ಗುಂಪು_desc_ಎಣಿಕೆ-1)
			ಕೊನೆಯ_grp = fs->ಗುಂಪು_desc_ಎಣಿಕೆ-1;
		rem_grps = ಕೊನೆಯ_grp - ಗುಂಪು + 1;
	}

	/*
	 * Allocate the block and inode bitmaps, if necessary
	 */
	ಆದರೆ (fs->stride && !flexbg_ಗಾತ್ರ) {
		retval = ext2fs_get_free_blocks2(fs, ಗುಂಪು_blk, ಕೊನೆಯ_blk,
						 1, bmap, &ಆರಂಭ_blk);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;
		ಆರಂಭ_blk += fs->inode_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು;
		ಆರಂಭ_blk += ((fs->stride * ಗುಂಪು) %
			      (ಕೊನೆಯ_blk - ಆರಂಭ_blk + 1));
		ಆದರೆ (ಆರಂಭ_blk >= ಕೊನೆಯ_blk)
			ಆರಂಭ_blk = ಗುಂಪು_blk;
	} ಇಲ್ಲದಿದ್ದರೆ
		ಆರಂಭ_blk = ಗುಂಪು_blk;

	ಆದರೆ (flexbg_ಗಾತ್ರ) {
		blk64_t prev_ಖಂಡ = 0;

		ಕೋಷ್ಟಕ_ಸ್ಥಾನಾಂತರ = flexbg_ಗಾತ್ರ;
		ಆದರೆ (ಗುಂಪು % flexbg_ಗಾತ್ರ)
			prev_ಖಂಡ = ext2fs_ಖಂಡ_bitmap_loc(fs, ಗುಂಪು - 1) + 1;
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (ಕೊನೆಯ_grp == fs->ಗುಂಪು_desc_ಎಣಿಕೆ-1) {
			/*
			 * If we are allocating for the last flex_bg
			 * keep the metadata tables contiguous
			 */
			ಕೋಷ್ಟಕ_ಸ್ಥಾನಾಂತರ = ಕೊನೆಯ_grp & (flexbg_ಗಾತ್ರ - 1);
			ಆದರೆ (ಕೋಷ್ಟಕ_ಸ್ಥಾನಾಂತರ == 0)
				ಕೋಷ್ಟಕ_ಸ್ಥಾನಾಂತರ = flexbg_ಗಾತ್ರ;
			ಇಲ್ಲದಿದ್ದರೆ
				ಕೋಷ್ಟಕ_ಸ್ಥಾನಾಂತರ++;
		}
		/* FIXME: Take backup group descriptor blocks into account
		 * if the flexbg allocations will grow to overlap them... */
		ಆರಂಭ_blk = flexbg_ಸ್ಥಾನಾಂತರ(fs, ಗುಂಪು, prev_ಖಂಡ, bmap,
					  rem_grps, 1);
		ಕೊನೆಯ_blk = ext2fs_ಗುಂಪು_ಕೊನೆಯ_block2(fs, ಕೊನೆಯ_grp);
	}

	ಆದರೆ (!ext2fs_ಖಂಡ_bitmap_loc(fs, ಗುಂಪು)) {
		retval = ext2fs_get_free_blocks2(fs, ಆರಂಭ_blk, ಕೊನೆಯ_blk,
						 1, bmap, &new_blk);
		ಆದರೆ (retval == EXT2_ET_ಖಂಡ_ALLOC_FAIL)
			retval = ext2fs_get_free_blocks2(fs, ಗುಂಪು_blk,
					ಕೊನೆಯ_blk, 1, bmap, &new_blk);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;
		ext2fs_mark_ಖಂಡ_bitmap2(bmap, new_blk);
		ext2fs_ಖಂಡ_bitmap_loc_ಗಣ(fs, ಗುಂಪು, new_blk);
		ಆದರೆ (flexbg_ಗಾತ್ರ) {
			dgrp_t gr = ext2fs_ಗುಂಪು_ದ_blk2(fs, new_blk);
			ext2fs_bg_free_ಖಂಡಗಳು_ಎಣಿಕೆ_ಗಣ(fs, gr, ext2fs_bg_free_ಖಂಡಗಳು_ಎಣಿಕೆ(fs, gr) - 1);
			ext2fs_free_ಖಂಡಗಳು_ಎಣಿಕೆ_ಕೂಡಿಸು(fs->super, -1);
			ext2fs_bg_flags_clear(fs, gr, EXT2_BG_ಖಂಡ_UNINIT);
			ext2fs_ಗುಂಪು_desc_csum_ಗಣ(fs, gr);
		}
	}

	ಆದರೆ (flexbg_ಗಾತ್ರ) {
		blk64_t prev_ಖಂಡ = 0;
		ಆದರೆ (ಗುಂಪು % flexbg_ಗಾತ್ರ)
			prev_ಖಂಡ = ext2fs_inode_bitmap_loc(fs, ಗುಂಪು - 1) + 1;
		ಇಲ್ಲದಿದ್ದರೆ
			prev_ಖಂಡ = ext2fs_ಖಂಡ_bitmap_loc(fs, ಗುಂಪು) +
				ಕೋಷ್ಟಕ_ಸ್ಥಾನಾಂತರ;
		/* FIXME: Take backup group descriptor blocks into account
		 * if the flexbg allocations will grow to overlap them... */
		ಆರಂಭ_blk = flexbg_ಸ್ಥಾನಾಂತರ(fs, ಗುಂಪು, prev_ಖಂಡ, bmap,
					  rem_grps, 1);
		ಕೊನೆಯ_blk = ext2fs_ಗುಂಪು_ಕೊನೆಯ_block2(fs, ಕೊನೆಯ_grp);
	}

	ಆದರೆ (!ext2fs_inode_bitmap_loc(fs, ಗುಂಪು)) {
		retval = ext2fs_get_free_blocks2(fs, ಆರಂಭ_blk, ಕೊನೆಯ_blk,
						 1, bmap, &new_blk);
		ಆದರೆ (retval == EXT2_ET_ಖಂಡ_ALLOC_FAIL)
			retval = ext2fs_get_free_blocks2(fs, ಗುಂಪು_blk,
					 ಕೊನೆಯ_blk, 1, bmap, &new_blk);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;
		ext2fs_mark_ಖಂಡ_bitmap2(bmap, new_blk);
		ext2fs_inode_bitmap_loc_ಗಣ(fs, ಗುಂಪು, new_blk);
		ಆದರೆ (flexbg_ಗಾತ್ರ) {
			dgrp_t gr = ext2fs_ಗುಂಪು_ದ_blk2(fs, new_blk);
			ext2fs_bg_free_ಖಂಡಗಳು_ಎಣಿಕೆ_ಗಣ(fs, gr, ext2fs_bg_free_ಖಂಡಗಳು_ಎಣಿಕೆ(fs, gr) - 1);
			ext2fs_free_ಖಂಡಗಳು_ಎಣಿಕೆ_ಕೂಡಿಸು(fs->super, -1);
			ext2fs_bg_flags_clear(fs, gr, EXT2_BG_ಖಂಡ_UNINIT);
			ext2fs_ಗುಂಪು_desc_csum_ಗಣ(fs, gr);
		}
	}

	/*
	 * Allocate the inode table
	 */
	ಆದರೆ (flexbg_ಗಾತ್ರ) {
		blk64_t prev_ಖಂಡ = 0;

		ಆದರೆ (ಗುಂಪು % flexbg_ಗಾತ್ರ)
			prev_ಖಂಡ = ext2fs_inode_ಕೋಷ್ಟಕ_loc(fs, ಗುಂಪು - 1) +
				fs->inode_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು;
		ಇಲ್ಲದಿದ್ದರೆ
			prev_ಖಂಡ = ext2fs_inode_bitmap_loc(fs, ಗುಂಪು) +
				ಕೋಷ್ಟಕ_ಸ್ಥಾನಾಂತರ;

		/* FIXME: Take backup group descriptor blocks into account
		 * if the flexbg allocations will grow to overlap them... */
		ಗುಂಪು_blk = flexbg_ಸ್ಥಾನಾಂತರ(fs, ಗುಂಪು, prev_ಖಂಡ, bmap,
					  rem_grps, fs->inode_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು);
		ಕೊನೆಯ_blk = ext2fs_ಗುಂಪು_ಕೊನೆಯ_block2(fs, ಕೊನೆಯ_grp);
	}

	ಆದರೆ (!ext2fs_inode_ಕೋಷ್ಟಕ_loc(fs, ಗುಂಪು)) {
		retval = ext2fs_get_free_blocks2(fs, ಗುಂಪು_blk, ಕೊನೆಯ_blk,
						fs->inode_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು,
						bmap, &new_blk);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;

		ext2fs_mark_ಖಂಡ_bitmap_range2(bmap,
			new_blk, fs->inode_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು);
		ಆದರೆ (flexbg_ಗಾತ್ರ) {
			blk64_t ಅಂಕೆ, blk;
			ಅಂಕೆ = fs->inode_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು;
			blk = new_blk;
			ಆಗ (ಅಂಕೆ) {
				ಪೂರ್ಣಾಂಕ gr = ext2fs_ಗುಂಪು_ದ_blk2(fs, blk);
				ಕೊನೆಯ_blk = ext2fs_ಗುಂಪು_ಕೊನೆಯ_block2(fs, gr);
				blk64_t n = ಅಂಕೆ;

				ಆದರೆ (blk + ಅಂಕೆ > ಕೊನೆಯ_blk)
					n = ಕೊನೆಯ_blk - blk + 1;

				ext2fs_bg_free_ಖಂಡಗಳು_ಎಣಿಕೆ_ಗಣ(fs, gr,
					ext2fs_bg_free_ಖಂಡಗಳು_ಎಣಿಕೆ(fs, gr) -
					n/EXT2_FS_CLUSTER_ಅನುಪಾತ(fs));
				ext2fs_bg_flags_clear(fs, gr,
					EXT2_BG_ಖಂಡ_UNINIT);
				ext2fs_ಗುಂಪು_desc_csum_ಗಣ(fs, gr);
				ext2fs_free_ಖಂಡಗಳು_ಎಣಿಕೆ_ಕೂಡಿಸು(fs->super, -n);
				blk += n;
				ಅಂಕೆ -= n;
			}
		}
		ext2fs_inode_ಕೋಷ್ಟಕ_loc_ಗಣ(fs, ಗುಂಪು, new_blk);
	}
	ext2fs_ಗುಂಪು_desc_csum_ಗಣ(fs, ಗುಂಪು);
	ಹಿಂತಿರುಗಿ 0;
}

errcode_t ext2fs_allocate_tables(ext2_filsys fs)
{
	errcode_t	retval;
	dgrp_t		i;
	ರಚನೆ ext2fs_numeric_ಪ್ರಗತಿ_struct ಪ್ರಗತಿ;

	ಆದರೆ (fs->ಪ್ರಗತಿ_ops && fs->ಪ್ರಗತಿ_ops->init)
		(fs->ಪ್ರಗತಿ_ops->init)(fs, &ಪ್ರಗತಿ, ಖಾಲಿ,
					 fs->ಗುಂಪು_desc_ಎಣಿಕೆ);

	ಗಾಗಿ (i = 0; i < fs->ಗುಂಪು_desc_ಎಣಿಕೆ; i++) {
		ಆದರೆ (fs->ಪ್ರಗತಿ_ops && fs->ಪ್ರಗತಿ_ops->update)
			(fs->ಪ್ರಗತಿ_ops->update)(fs, &ಪ್ರಗತಿ, i);
		retval = ext2fs_allocate_ಗುಂಪು_ಕೋಷ್ಟಕ(fs, i, fs->ಖಂಡ_map);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;
	}
	ಆದರೆ (fs->ಪ್ರಗತಿ_ops && fs->ಪ್ರಗತಿ_ops->close)
		(fs->ಪ್ರಗತಿ_ops->close)(fs, &ಪ್ರಗತಿ, ಖಾಲಿ);
	ಹಿಂತಿರುಗಿ 0;
}

