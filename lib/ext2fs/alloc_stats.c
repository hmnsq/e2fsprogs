/*
 * alloc_stats.c --- Update allocation statistics for ext2fs
 *
 * Copyright (C) 2001 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"

ಶೂನ್ಯ ext2fs_inode_alloc_stats2(ext2_filsys fs, ext2_ino_t ino,
			       ಪೂರ್ಣಾಂಕ inuse, ಪೂರ್ಣಾಂಕ isdir)
{
	ಪೂರ್ಣಾಂಕ	ಗುಂಪು = ext2fs_ಗುಂಪು_ದ_ino(fs, ino);

	ಆದರೆ (ino > fs->super->s_inodes_ಎಣಿಕೆ) {
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ OMIT_COM_ERR
		com_err("ext2fs_inode_alloc_stats2", 0,
			"Illegal inode number: %lu", (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ) ino);
#ಆದರೆ_ಅಂತ್ಯ
		ಹಿಂತಿರುಗಿ;
	}
	ಆದರೆ (inuse > 0)
		ext2fs_mark_inode_bitmap2(fs->inode_map, ino);
	ಇಲ್ಲದಿದ್ದರೆ
		ext2fs_unmark_inode_bitmap2(fs->inode_map, ino);
	ext2fs_bg_free_inodes_ಎಣಿಕೆ_ಗಣ(fs, ಗುಂಪು, ext2fs_bg_free_inodes_ಎಣಿಕೆ(fs, ಗುಂಪು) - inuse);
	ಆದರೆ (isdir)
		ext2fs_bg_ಬಳಸಿದ_dirs_ಎಣಿಕೆ_ಗಣ(fs, ಗುಂಪು, ext2fs_bg_ಬಳಸಿದ_dirs_ಎಣಿಕೆ(fs, ಗುಂಪು) + inuse);

	/* We don't strictly need to be clearing the uninit flag if inuse < 0
	 * (i.e. freeing inodes) but it also means something is bad. */
	ext2fs_bg_flags_clear(fs, ಗುಂಪು, EXT2_BG_INODE_UNINIT);
	ಆದರೆ (ext2fs_has_ಗುಂಪು_desc_csum(fs)) {
		ext2_ino_t ಮೊದಲ_ಬಳಸದ_inode =	fs->super->s_inodes_ಪ್ರತಿ_ಗುಂಪು -
			ext2fs_bg_itable_ಬಳಸದ(fs, ಗುಂಪು) +
			ಗುಂಪು * fs->super->s_inodes_ಪ್ರತಿ_ಗುಂಪು + 1;

		ಆದರೆ (ino >= ಮೊದಲ_ಬಳಸದ_inode)
			ext2fs_bg_itable_ಬಳಸದ_ಗಣ(fs, ಗುಂಪು, ಗುಂಪು * fs->super->s_inodes_ಪ್ರತಿ_ಗುಂಪು + fs->super->s_inodes_ಪ್ರತಿ_ಗುಂಪು - ino);
		ext2fs_ಗುಂಪು_desc_csum_ಗಣ(fs, ಗುಂಪು);
	}

	fs->super->s_free_inodes_ಎಣಿಕೆ -= inuse;
	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_ib_dirty(fs);
}

ಶೂನ್ಯ ext2fs_inode_alloc_stats(ext2_filsys fs, ext2_ino_t ino, ಪೂರ್ಣಾಂಕ inuse)
{
	ext2fs_inode_alloc_stats2(fs, ino, inuse, 0);
}

ಶೂನ್ಯ ext2fs_ಖಂಡ_alloc_stats2(ext2_filsys fs, blk64_t blk, ಪೂರ್ಣಾಂಕ inuse)
{
	ಪೂರ್ಣಾಂಕ	ಗುಂಪು = ext2fs_ಗುಂಪು_ದ_blk2(fs, blk);

	ಆದರೆ (blk < fs->super->s_ಮೊದಲ_ದತ್ತಾಂಶ_ಖಂಡ ||
	    blk >= ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(fs->super)) {
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ OMIT_COM_ERR
		com_err("ext2fs_block_alloc_stats", 0,
			"Illegal block number: %lu", (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ) blk);
#ಆದರೆ_ಅಂತ್ಯ
		ಹಿಂತಿರುಗಿ;
	}
	ಆದರೆ (inuse > 0)
		ext2fs_mark_ಖಂಡ_bitmap2(fs->ಖಂಡ_map, blk);
	ಇಲ್ಲದಿದ್ದರೆ
		ext2fs_unmark_ಖಂಡ_bitmap2(fs->ಖಂಡ_map, blk);
	ext2fs_bg_free_ಖಂಡಗಳು_ಎಣಿಕೆ_ಗಣ(fs, ಗುಂಪು, ext2fs_bg_free_ಖಂಡಗಳು_ಎಣಿಕೆ(fs, ಗುಂಪು) - inuse);
	ext2fs_bg_flags_clear(fs, ಗುಂಪು, EXT2_BG_ಖಂಡ_UNINIT);
	ext2fs_ಗುಂಪು_desc_csum_ಗಣ(fs, ಗುಂಪು);

	ext2fs_free_ಖಂಡಗಳು_ಎಣಿಕೆ_ಕೂಡಿಸು(fs->super,
			     -inuse * (blk64_t) EXT2_FS_CLUSTER_ಅನುಪಾತ(fs));
	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_bb_dirty(fs);
	ಆದರೆ (fs->ಖಂಡ_alloc_stats)
		(fs->ಖಂಡ_alloc_stats)(fs, (blk64_t) blk, inuse);
}

ಶೂನ್ಯ ext2fs_ಖಂಡ_alloc_stats(ext2_filsys fs, blk_t blk, ಪೂರ್ಣಾಂಕ inuse)
{
	ext2fs_ಖಂಡ_alloc_stats2(fs, blk, inuse);
}

ಶೂನ್ಯ ext2fs_ಗಣ_ಖಂಡ_alloc_stats_callback(ext2_filsys fs,
					   ಶೂನ್ಯ (*func)(ext2_filsys fs,
							blk64_t blk,
							ಪೂರ್ಣಾಂಕ inuse),
					   ಶೂನ್ಯ (**old)(ext2_filsys fs,
							blk64_t blk,
							ಪೂರ್ಣಾಂಕ inuse))
{
	ಆದರೆ (!fs || fs->magic != EXT2_ET_MAGIC_EXT2FS_FILSYS)
		ಹಿಂತಿರುಗಿ;
	ಆದರೆ (old)
		*old = fs->ಖಂಡ_alloc_stats;

	fs->ಖಂಡ_alloc_stats = func;
}

ಶೂನ್ಯ ext2fs_ಖಂಡ_alloc_stats_range(ext2_filsys fs, blk64_t blk,
				    blk_t ಅಂಕೆ, ಪೂರ್ಣಾಂಕ inuse)
{
	ಸ್ಥಿರಾಂಕ blk64_t orig_blk = blk;
	ಸ್ಥಿರಾಂಕ blk_t orig_ಅಂಕೆ = ಅಂಕೆ;

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ OMIT_COM_ERR
	ಆದರೆ (blk + ಅಂಕೆ > ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(fs->super)) {
		com_err("ext2fs_block_alloc_stats_range", 0,
			"Illegal block range: %llu (%u) ",
			(ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ) blk, ಅಂಕೆ);
		ಹಿಂತಿರುಗಿ;
	}
#ಆದರೆ_ಅಂತ್ಯ
	ಆದರೆ (inuse == 0)
		ಹಿಂತಿರುಗಿ;
	ಆದರೆ (inuse > 0) {
		ext2fs_mark_ಖಂಡ_bitmap_range2(fs->ಖಂಡ_map, blk, ಅಂಕೆ);
		inuse = 1;
	} ಇಲ್ಲದಿದ್ದರೆ {
		ext2fs_unmark_ಖಂಡ_bitmap_range2(fs->ಖಂಡ_map, blk, ಅಂಕೆ);
		inuse = -1;
	}
	ಆಗ (ಅಂಕೆ) {
		ಪೂರ್ಣಾಂಕ ಗುಂಪು = ext2fs_ಗುಂಪು_ದ_blk2(fs, blk);
		blk64_t ಕೊನೆಯ_blk = ext2fs_ಗುಂಪು_ಕೊನೆಯ_block2(fs, ಗುಂಪು);
		blk64_t n = ಅಂಕೆ;

		ಆದರೆ (blk + ಅಂಕೆ > ಕೊನೆಯ_blk)
			n = ಕೊನೆಯ_blk - blk + 1;

		ext2fs_bg_free_ಖಂಡಗಳು_ಎಣಿಕೆ_ಗಣ(fs, ಗುಂಪು,
			ext2fs_bg_free_ಖಂಡಗಳು_ಎಣಿಕೆ(fs, ಗುಂಪು) -
			inuse*n/EXT2_FS_CLUSTER_ಅನುಪಾತ(fs));
		ext2fs_bg_flags_clear(fs, ಗುಂಪು, EXT2_BG_ಖಂಡ_UNINIT);
		ext2fs_ಗುಂಪು_desc_csum_ಗಣ(fs, ಗುಂಪು);
		ext2fs_free_ಖಂಡಗಳು_ಎಣಿಕೆ_ಕೂಡಿಸು(fs->super, -inuse * (blk64_t) n);
		blk += n;
		ಅಂಕೆ -= n;
	}
	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_bb_dirty(fs);
	ಆದರೆ (fs->ಖಂಡ_alloc_stats_range)
		(fs->ಖಂಡ_alloc_stats_range)(fs, orig_blk, orig_ಅಂಕೆ, inuse);
}

ಶೂನ್ಯ ext2fs_ಗಣ_ಖಂಡ_alloc_stats_range_callback(ext2_filsys fs,
	ಶೂನ್ಯ (*func)(ext2_filsys fs, blk64_t blk,
				    blk_t ಅಂಕೆ, ಪೂರ್ಣಾಂಕ inuse),
	ಶೂನ್ಯ (**old)(ext2_filsys fs, blk64_t blk,
				    blk_t ಅಂಕೆ, ಪೂರ್ಣಾಂಕ inuse))
{
	ಆದರೆ (!fs || fs->magic != EXT2_ET_MAGIC_EXT2FS_FILSYS)
		ಹಿಂತಿರುಗಿ;
	ಆದರೆ (old)
		*old = fs->ಖಂಡ_alloc_stats_range;

	fs->ಖಂಡ_alloc_stats_range = func;
}
