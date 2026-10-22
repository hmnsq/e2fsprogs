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

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fs.h"

శూన్య ext2fs_inode_alloc_stats2(ext2_filsys fs, ext2_ino_t ino,
			       పూర్ణాంకం inuse, పూర్ణాంకం isdir)
{
	పూర్ణాంకం	సమూహం = ext2fs_సమూహం_యొక్క_ino(fs, ino);

	ఒకవేళ (ino > fs->super->s_inodes_లెక్క) {
#నిర్వచితం_కాకపోతే OMIT_COM_ERR
		com_err("ext2fs_inode_alloc_stats2", 0,
			"Illegal inode number: %lu", (గుర్తులేని దీర్ఘ) ino);
#అయితే_ముగింపు
		తిరిగి;
	}
	ఒకవేళ (inuse > 0)
		ext2fs_mark_inode_bitmap2(fs->inode_map, ino);
	లేకపోతే
		ext2fs_unmark_inode_bitmap2(fs->inode_map, ino);
	ext2fs_bg_free_inodes_లెక్క_సమితి(fs, సమూహం, ext2fs_bg_free_inodes_లెక్క(fs, సమూహం) - inuse);
	ఒకవేళ (isdir)
		ext2fs_bg_వాడిన_dirs_లెక్క_సమితి(fs, సమూహం, ext2fs_bg_వాడిన_dirs_లెక్క(fs, సమూహం) + inuse);

	/* We don't strictly need to be clearing the uninit flag if inuse < 0
	 * (i.e. freeing inodes) but it also means something is bad. */
	ext2fs_bg_flags_clear(fs, సమూహం, EXT2_BG_INODE_UNINIT);
	ఒకవేళ (ext2fs_has_సమూహం_desc_csum(fs)) {
		ext2_ino_t మొదటి_వాడని_inode =	fs->super->s_inodes_చొప్పున_సమూహం -
			ext2fs_bg_itable_వాడని(fs, సమూహం) +
			సమూహం * fs->super->s_inodes_చొప్పున_సమూహం + 1;

		ఒకవేళ (ino >= మొదటి_వాడని_inode)
			ext2fs_bg_itable_వాడని_సమితి(fs, సమూహం, సమూహం * fs->super->s_inodes_చొప్పున_సమూహం + fs->super->s_inodes_చొప్పున_సమూహం - ino);
		ext2fs_సమూహం_desc_csum_సమితి(fs, సమూహం);
	}

	fs->super->s_free_inodes_లెక్క -= inuse;
	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_ib_dirty(fs);
}

శూన్య ext2fs_inode_alloc_stats(ext2_filsys fs, ext2_ino_t ino, పూర్ణాంకం inuse)
{
	ext2fs_inode_alloc_stats2(fs, ino, inuse, 0);
}

శూన్య ext2fs_ఖండం_alloc_stats2(ext2_filsys fs, blk64_t blk, పూర్ణాంకం inuse)
{
	పూర్ణాంకం	సమూహం = ext2fs_సమూహం_యొక్క_blk2(fs, blk);

	ఒకవేళ (blk < fs->super->s_మొదటి_దత్తాంశం_ఖండం ||
	    blk >= ext2fs_ఖండాలు_లెక్క(fs->super)) {
#నిర్వచితం_కాకపోతే OMIT_COM_ERR
		com_err("ext2fs_block_alloc_stats", 0,
			"Illegal block number: %lu", (గుర్తులేని దీర్ఘ) blk);
#అయితే_ముగింపు
		తిరిగి;
	}
	ఒకవేళ (inuse > 0)
		ext2fs_mark_ఖండం_bitmap2(fs->ఖండం_map, blk);
	లేకపోతే
		ext2fs_unmark_ఖండం_bitmap2(fs->ఖండం_map, blk);
	ext2fs_bg_free_ఖండాలు_లెక్క_సమితి(fs, సమూహం, ext2fs_bg_free_ఖండాలు_లెక్క(fs, సమూహం) - inuse);
	ext2fs_bg_flags_clear(fs, సమూహం, EXT2_BG_ఖండం_UNINIT);
	ext2fs_సమూహం_desc_csum_సమితి(fs, సమూహం);

	ext2fs_free_ఖండాలు_లెక్క_కూడు(fs->super,
			     -inuse * (blk64_t) EXT2_FS_CLUSTER_నిష్పత్తి(fs));
	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_bb_dirty(fs);
	ఒకవేళ (fs->ఖండం_alloc_stats)
		(fs->ఖండం_alloc_stats)(fs, (blk64_t) blk, inuse);
}

శూన్య ext2fs_ఖండం_alloc_stats(ext2_filsys fs, blk_t blk, పూర్ణాంకం inuse)
{
	ext2fs_ఖండం_alloc_stats2(fs, blk, inuse);
}

శూన్య ext2fs_సమితి_ఖండం_alloc_stats_callback(ext2_filsys fs,
					   శూన్య (*func)(ext2_filsys fs,
							blk64_t blk,
							పూర్ణాంకం inuse),
					   శూన్య (**old)(ext2_filsys fs,
							blk64_t blk,
							పూర్ణాంకం inuse))
{
	ఒకవేళ (!fs || fs->magic != EXT2_ET_MAGIC_EXT2FS_FILSYS)
		తిరిగి;
	ఒకవేళ (old)
		*old = fs->ఖండం_alloc_stats;

	fs->ఖండం_alloc_stats = func;
}

శూన్య ext2fs_ఖండం_alloc_stats_range(ext2_filsys fs, blk64_t blk,
				    blk_t అంకె, పూర్ణాంకం inuse)
{
	స్థిరరాశి blk64_t orig_blk = blk;
	స్థిరరాశి blk_t orig_అంకె = అంకె;

#నిర్వచితం_కాకపోతే OMIT_COM_ERR
	ఒకవేళ (blk + అంకె > ext2fs_ఖండాలు_లెక్క(fs->super)) {
		com_err("ext2fs_block_alloc_stats_range", 0,
			"Illegal block range: %llu (%u) ",
			(గుర్తులేని దీర్ఘ దీర్ఘ) blk, అంకె);
		తిరిగి;
	}
#అయితే_ముగింపు
	ఒకవేళ (inuse == 0)
		తిరిగి;
	ఒకవేళ (inuse > 0) {
		ext2fs_mark_ఖండం_bitmap_range2(fs->ఖండం_map, blk, అంకె);
		inuse = 1;
	} లేకపోతే {
		ext2fs_unmark_ఖండం_bitmap_range2(fs->ఖండం_map, blk, అంకె);
		inuse = -1;
	}
	అయితే (అంకె) {
		పూర్ణాంకం సమూహం = ext2fs_సమూహం_యొక్క_blk2(fs, blk);
		blk64_t చివరి_blk = ext2fs_సమూహం_చివరి_block2(fs, సమూహం);
		blk64_t n = అంకె;

		ఒకవేళ (blk + అంకె > చివరి_blk)
			n = చివరి_blk - blk + 1;

		ext2fs_bg_free_ఖండాలు_లెక్క_సమితి(fs, సమూహం,
			ext2fs_bg_free_ఖండాలు_లెక్క(fs, సమూహం) -
			inuse*n/EXT2_FS_CLUSTER_నిష్పత్తి(fs));
		ext2fs_bg_flags_clear(fs, సమూహం, EXT2_BG_ఖండం_UNINIT);
		ext2fs_సమూహం_desc_csum_సమితి(fs, సమూహం);
		ext2fs_free_ఖండాలు_లెక్క_కూడు(fs->super, -inuse * (blk64_t) n);
		blk += n;
		అంకె -= n;
	}
	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_bb_dirty(fs);
	ఒకవేళ (fs->ఖండం_alloc_stats_range)
		(fs->ఖండం_alloc_stats_range)(fs, orig_blk, orig_అంకె, inuse);
}

శూన్య ext2fs_సమితి_ఖండం_alloc_stats_range_callback(ext2_filsys fs,
	శూన్య (*func)(ext2_filsys fs, blk64_t blk,
				    blk_t అంకె, పూర్ణాంకం inuse),
	శూన్య (**old)(ext2_filsys fs, blk64_t blk,
				    blk_t అంకె, పూర్ణాంకం inuse))
{
	ఒకవేళ (!fs || fs->magic != EXT2_ET_MAGIC_EXT2FS_FILSYS)
		తిరిగి;
	ఒకవేళ (old)
		*old = fs->ఖండం_alloc_stats_range;

	fs->ఖండం_alloc_stats_range = func;
}
