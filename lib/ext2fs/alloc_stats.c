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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fs.h"

ਖਾਲੀ ext2fs_inode_alloc_stats2(ext2_filsys fs, ext2_ino_t ino,
			       ਪੂਰਨ inuse, ਪੂਰਨ isdir)
{
	ਪੂਰਨ	ਸਮੂਹ = ext2fs_ਸਮੂਹ_ਦਾ_ino(fs, ino);

	ਜੇ (ino > fs->super->s_inodes_ਗਿਣਤੀ) {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OMIT_COM_ERR
		com_err("ext2fs_inode_alloc_stats2", 0,
			"Illegal inode number: %lu", (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ) ino);
#ਜੇ_ਅੰਤ
		ਵਾਪਸ;
	}
	ਜੇ (inuse > 0)
		ext2fs_mark_inode_bitmap2(fs->inode_map, ino);
	ਨਹੀਂਤਾਂ
		ext2fs_unmark_inode_bitmap2(fs->inode_map, ino);
	ext2fs_bg_free_inodes_ਗਿਣਤੀ_ਸੈੱਟ(fs, ਸਮੂਹ, ext2fs_bg_free_inodes_ਗਿਣਤੀ(fs, ਸਮੂਹ) - inuse);
	ਜੇ (isdir)
		ext2fs_bg_ਵਰਤਿਆ_dirs_ਗਿਣਤੀ_ਸੈੱਟ(fs, ਸਮੂਹ, ext2fs_bg_ਵਰਤਿਆ_dirs_ਗਿਣਤੀ(fs, ਸਮੂਹ) + inuse);

	/* We don't strictly need to be clearing the uninit flag if inuse < 0
	 * (i.e. freeing inodes) but it also means something is bad. */
	ext2fs_bg_flags_clear(fs, ਸਮੂਹ, EXT2_BG_INODE_UNINIT);
	ਜੇ (ext2fs_has_ਸਮੂਹ_desc_csum(fs)) {
		ext2_ino_t ਪਹਿਲਾ_ਅਣਵਰਤਿਆ_inode =	fs->super->s_inodes_ਪ੍ਰਤੀ_ਸਮੂਹ -
			ext2fs_bg_itable_ਅਣਵਰਤਿਆ(fs, ਸਮੂਹ) +
			ਸਮੂਹ * fs->super->s_inodes_ਪ੍ਰਤੀ_ਸਮੂਹ + 1;

		ਜੇ (ino >= ਪਹਿਲਾ_ਅਣਵਰਤਿਆ_inode)
			ext2fs_bg_itable_ਅਣਵਰਤਿਆ_ਸੈੱਟ(fs, ਸਮੂਹ, ਸਮੂਹ * fs->super->s_inodes_ਪ੍ਰਤੀ_ਸਮੂਹ + fs->super->s_inodes_ਪ੍ਰਤੀ_ਸਮੂਹ - ino);
		ext2fs_ਸਮੂਹ_desc_csum_ਸੈੱਟ(fs, ਸਮੂਹ);
	}

	fs->super->s_free_inodes_ਗਿਣਤੀ -= inuse;
	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_ib_dirty(fs);
}

ਖਾਲੀ ext2fs_inode_alloc_stats(ext2_filsys fs, ext2_ino_t ino, ਪੂਰਨ inuse)
{
	ext2fs_inode_alloc_stats2(fs, ino, inuse, 0);
}

ਖਾਲੀ ext2fs_ਖੰਡ_alloc_stats2(ext2_filsys fs, blk64_t blk, ਪੂਰਨ inuse)
{
	ਪੂਰਨ	ਸਮੂਹ = ext2fs_ਸਮੂਹ_ਦਾ_blk2(fs, blk);

	ਜੇ (blk < fs->super->s_ਪਹਿਲਾ_ਅੰਕੜੇ_ਖੰਡ ||
	    blk >= ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(fs->super)) {
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OMIT_COM_ERR
		com_err("ext2fs_block_alloc_stats", 0,
			"Illegal block number: %lu", (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ) blk);
#ਜੇ_ਅੰਤ
		ਵਾਪਸ;
	}
	ਜੇ (inuse > 0)
		ext2fs_mark_ਖੰਡ_bitmap2(fs->ਖੰਡ_map, blk);
	ਨਹੀਂਤਾਂ
		ext2fs_unmark_ਖੰਡ_bitmap2(fs->ਖੰਡ_map, blk);
	ext2fs_bg_free_ਖੰਡਾਂ_ਗਿਣਤੀ_ਸੈੱਟ(fs, ਸਮੂਹ, ext2fs_bg_free_ਖੰਡਾਂ_ਗਿਣਤੀ(fs, ਸਮੂਹ) - inuse);
	ext2fs_bg_flags_clear(fs, ਸਮੂਹ, EXT2_BG_ਖੰਡ_UNINIT);
	ext2fs_ਸਮੂਹ_desc_csum_ਸੈੱਟ(fs, ਸਮੂਹ);

	ext2fs_free_ਖੰਡਾਂ_ਗਿਣਤੀ_ਜੋੜੋ(fs->super,
			     -inuse * (blk64_t) EXT2_FS_CLUSTER_ਅਨੁਪਾਤ(fs));
	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_bb_dirty(fs);
	ਜੇ (fs->ਖੰਡ_alloc_stats)
		(fs->ਖੰਡ_alloc_stats)(fs, (blk64_t) blk, inuse);
}

ਖਾਲੀ ext2fs_ਖੰਡ_alloc_stats(ext2_filsys fs, blk_t blk, ਪੂਰਨ inuse)
{
	ext2fs_ਖੰਡ_alloc_stats2(fs, blk, inuse);
}

ਖਾਲੀ ext2fs_ਸੈੱਟ_ਖੰਡ_alloc_stats_callback(ext2_filsys fs,
					   ਖਾਲੀ (*func)(ext2_filsys fs,
							blk64_t blk,
							ਪੂਰਨ inuse),
					   ਖਾਲੀ (**old)(ext2_filsys fs,
							blk64_t blk,
							ਪੂਰਨ inuse))
{
	ਜੇ (!fs || fs->magic != EXT2_ET_MAGIC_EXT2FS_FILSYS)
		ਵਾਪਸ;
	ਜੇ (old)
		*old = fs->ਖੰਡ_alloc_stats;

	fs->ਖੰਡ_alloc_stats = func;
}

ਖਾਲੀ ext2fs_ਖੰਡ_alloc_stats_range(ext2_filsys fs, blk64_t blk,
				    blk_t ਅੰਕ, ਪੂਰਨ inuse)
{
	ਅਚਲ blk64_t orig_blk = blk;
	ਅਚਲ blk_t orig_ਅੰਕ = ਅੰਕ;

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OMIT_COM_ERR
	ਜੇ (blk + ਅੰਕ > ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(fs->super)) {
		com_err("ext2fs_block_alloc_stats_range", 0,
			"Illegal block range: %llu (%u) ",
			(ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ) blk, ਅੰਕ);
		ਵਾਪਸ;
	}
#ਜੇ_ਅੰਤ
	ਜੇ (inuse == 0)
		ਵਾਪਸ;
	ਜੇ (inuse > 0) {
		ext2fs_mark_ਖੰਡ_bitmap_range2(fs->ਖੰਡ_map, blk, ਅੰਕ);
		inuse = 1;
	} ਨਹੀਂਤਾਂ {
		ext2fs_unmark_ਖੰਡ_bitmap_range2(fs->ਖੰਡ_map, blk, ਅੰਕ);
		inuse = -1;
	}
	ਜਦੋਂ (ਅੰਕ) {
		ਪੂਰਨ ਸਮੂਹ = ext2fs_ਸਮੂਹ_ਦਾ_blk2(fs, blk);
		blk64_t ਆਖਰੀ_blk = ext2fs_ਸਮੂਹ_ਆਖਰੀ_block2(fs, ਸਮੂਹ);
		blk64_t n = ਅੰਕ;

		ਜੇ (blk + ਅੰਕ > ਆਖਰੀ_blk)
			n = ਆਖਰੀ_blk - blk + 1;

		ext2fs_bg_free_ਖੰਡਾਂ_ਗਿਣਤੀ_ਸੈੱਟ(fs, ਸਮੂਹ,
			ext2fs_bg_free_ਖੰਡਾਂ_ਗਿਣਤੀ(fs, ਸਮੂਹ) -
			inuse*n/EXT2_FS_CLUSTER_ਅਨੁਪਾਤ(fs));
		ext2fs_bg_flags_clear(fs, ਸਮੂਹ, EXT2_BG_ਖੰਡ_UNINIT);
		ext2fs_ਸਮੂਹ_desc_csum_ਸੈੱਟ(fs, ਸਮੂਹ);
		ext2fs_free_ਖੰਡਾਂ_ਗਿਣਤੀ_ਜੋੜੋ(fs->super, -inuse * (blk64_t) n);
		blk += n;
		ਅੰਕ -= n;
	}
	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_bb_dirty(fs);
	ਜੇ (fs->ਖੰਡ_alloc_stats_range)
		(fs->ਖੰਡ_alloc_stats_range)(fs, orig_blk, orig_ਅੰਕ, inuse);
}

ਖਾਲੀ ext2fs_ਸੈੱਟ_ਖੰਡ_alloc_stats_range_callback(ext2_filsys fs,
	ਖਾਲੀ (*func)(ext2_filsys fs, blk64_t blk,
				    blk_t ਅੰਕ, ਪੂਰਨ inuse),
	ਖਾਲੀ (**old)(ext2_filsys fs, blk64_t blk,
				    blk_t ਅੰਕ, ਪੂਰਨ inuse))
{
	ਜੇ (!fs || fs->magic != EXT2_ET_MAGIC_EXT2FS_FILSYS)
		ਵਾਪਸ;
	ਜੇ (old)
		*old = fs->ਖੰਡ_alloc_stats_range;

	fs->ਖੰਡ_alloc_stats_range = func;
}
