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

#ضمن "config.h"
#ضمن <stdio.h>

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"

فراغ ext2fs_inode_alloc_stats2(ext2_filsys fs, ext2_ino_t ino,
			       صحيح inuse, صحيح isdir)
{
	صحيح	ډله = ext2fs_ډله_د_ino(fs, ino);

	إذا (ino > fs->super->s_inodes_شمېر) {
#که_نه_تعریف_شوی OMIT_COM_ERR
		com_err("ext2fs_inode_alloc_stats2", 0,
			"Illegal inode number: %lu", (بینښه اوږد) ino);
#د_که_پای
		أرجع;
	}
	إذا (inuse > 0)
		ext2fs_mark_inode_bitmap2(fs->inode_map, ino);
	وإلا
		ext2fs_unmark_inode_bitmap2(fs->inode_map, ino);
	ext2fs_bg_free_inodes_شمېر_سېټ(fs, ډله, ext2fs_bg_free_inodes_شمېر(fs, ډله) - inuse);
	إذا (isdir)
		ext2fs_bg_کارول_شوی_dirs_شمېر_سېټ(fs, ډله, ext2fs_bg_کارول_شوی_dirs_شمېر(fs, ډله) + inuse);

	/* We don't strictly need to be clearing the uninit flag if inuse < 0
	 * (i.e. freeing inodes) but it also means something is bad. */
	ext2fs_bg_flags_clear(fs, ډله, EXT2_BG_INODE_UNINIT);
	إذا (ext2fs_has_ډله_desc_csum(fs)) {
		ext2_ino_t لومړی_ناکارول_شوی_inode =	fs->super->s_inodes_فی_ډله -
			ext2fs_bg_itable_ناکارول_شوی(fs, ډله) +
			ډله * fs->super->s_inodes_فی_ډله + 1;

		إذا (ino >= لومړی_ناکارول_شوی_inode)
			ext2fs_bg_itable_ناکارول_شوی_سېټ(fs, ډله, ډله * fs->super->s_inodes_فی_ډله + fs->super->s_inodes_فی_ډله - ino);
		ext2fs_ډله_desc_csum_سېټ(fs, ډله);
	}

	fs->super->s_free_inodes_شمېر -= inuse;
	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_ib_dirty(fs);
}

فراغ ext2fs_inode_alloc_stats(ext2_filsys fs, ext2_ino_t ino, صحيح inuse)
{
	ext2fs_inode_alloc_stats2(fs, ino, inuse, 0);
}

فراغ ext2fs_بلاک_alloc_stats2(ext2_filsys fs, blk64_t blk, صحيح inuse)
{
	صحيح	ډله = ext2fs_ډله_د_blk2(fs, blk);

	إذا (blk < fs->super->s_لومړی_معلومات_بلاک ||
	    blk >= ext2fs_بلاکونه_شمېر(fs->super)) {
#که_نه_تعریف_شوی OMIT_COM_ERR
		com_err("ext2fs_block_alloc_stats", 0,
			"Illegal block number: %lu", (بینښه اوږد) blk);
#د_که_پای
		أرجع;
	}
	إذا (inuse > 0)
		ext2fs_mark_بلاک_bitmap2(fs->بلاک_map, blk);
	وإلا
		ext2fs_unmark_بلاک_bitmap2(fs->بلاک_map, blk);
	ext2fs_bg_free_بلاکونه_شمېر_سېټ(fs, ډله, ext2fs_bg_free_بلاکونه_شمېر(fs, ډله) - inuse);
	ext2fs_bg_flags_clear(fs, ډله, EXT2_BG_بلاک_UNINIT);
	ext2fs_ډله_desc_csum_سېټ(fs, ډله);

	ext2fs_free_بلاکونه_شمېر_جمع_کړه(fs->super,
			     -inuse * (blk64_t) EXT2_FS_CLUSTER_نسبت(fs));
	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_bb_dirty(fs);
	إذا (fs->بلاک_alloc_stats)
		(fs->بلاک_alloc_stats)(fs, (blk64_t) blk, inuse);
}

فراغ ext2fs_بلاک_alloc_stats(ext2_filsys fs, blk_t blk, صحيح inuse)
{
	ext2fs_بلاک_alloc_stats2(fs, blk, inuse);
}

فراغ ext2fs_سېټ_بلاک_alloc_stats_callback(ext2_filsys fs,
					   فراغ (*func)(ext2_filsys fs,
							blk64_t blk,
							صحيح inuse),
					   فراغ (**old)(ext2_filsys fs,
							blk64_t blk,
							صحيح inuse))
{
	إذا (!fs || fs->magic != EXT2_ET_MAGIC_EXT2FS_FILSYS)
		أرجع;
	إذا (old)
		*old = fs->بلاک_alloc_stats;

	fs->بلاک_alloc_stats = func;
}

فراغ ext2fs_بلاک_alloc_stats_range(ext2_filsys fs, blk64_t blk,
				    blk_t عدد, صحيح inuse)
{
	ثابت blk64_t orig_blk = blk;
	ثابت blk_t orig_عدد = عدد;

#که_نه_تعریف_شوی OMIT_COM_ERR
	إذا (blk + عدد > ext2fs_بلاکونه_شمېر(fs->super)) {
		com_err("ext2fs_block_alloc_stats_range", 0,
			"Illegal block range: %llu (%u) ",
			(بینښه اوږد اوږد) blk, عدد);
		أرجع;
	}
#د_که_پای
	إذا (inuse == 0)
		أرجع;
	إذا (inuse > 0) {
		ext2fs_mark_بلاک_bitmap_range2(fs->بلاک_map, blk, عدد);
		inuse = 1;
	} وإلا {
		ext2fs_unmark_بلاک_bitmap_range2(fs->بلاک_map, blk, عدد);
		inuse = -1;
	}
	طالما (عدد) {
		صحيح ډله = ext2fs_ډله_د_blk2(fs, blk);
		blk64_t وروستی_blk = ext2fs_ډله_وروستی_block2(fs, ډله);
		blk64_t n = عدد;

		إذا (blk + عدد > وروستی_blk)
			n = وروستی_blk - blk + 1;

		ext2fs_bg_free_بلاکونه_شمېر_سېټ(fs, ډله,
			ext2fs_bg_free_بلاکونه_شمېر(fs, ډله) -
			inuse*n/EXT2_FS_CLUSTER_نسبت(fs));
		ext2fs_bg_flags_clear(fs, ډله, EXT2_BG_بلاک_UNINIT);
		ext2fs_ډله_desc_csum_سېټ(fs, ډله);
		ext2fs_free_بلاکونه_شمېر_جمع_کړه(fs->super, -inuse * (blk64_t) n);
		blk += n;
		عدد -= n;
	}
	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_bb_dirty(fs);
	إذا (fs->بلاک_alloc_stats_range)
		(fs->بلاک_alloc_stats_range)(fs, orig_blk, orig_عدد, inuse);
}

فراغ ext2fs_سېټ_بلاک_alloc_stats_range_callback(ext2_filsys fs,
	فراغ (*func)(ext2_filsys fs, blk64_t blk,
				    blk_t عدد, صحيح inuse),
	فراغ (**old)(ext2_filsys fs, blk64_t blk,
				    blk_t عدد, صحيح inuse))
{
	إذا (!fs || fs->magic != EXT2_ET_MAGIC_EXT2FS_FILSYS)
		أرجع;
	إذا (old)
		*old = fs->بلاک_alloc_stats_range;

	fs->بلاک_alloc_stats_range = func;
}
