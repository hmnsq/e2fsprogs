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
	صحيح	گروہ = ext2fs_گروہ_دا_ino(fs, ino);

	إذا (ino > fs->super->s_inodes_گنتی) {
#جے_بنا_تعریف OMIT_COM_ERR
		com_err("ext2fs_inode_alloc_stats2", 0,
			"Illegal inode number: %lu", (بنانشانی لما) ino);
#جے_انت
		أرجع;
	}
	إذا (inuse > 0)
		ext2fs_mark_inode_bitmap2(fs->inode_map, ino);
	وإلا
		ext2fs_unmark_inode_bitmap2(fs->inode_map, ino);
	ext2fs_bg_free_inodes_گنتی_سیٹ(fs, گروہ, ext2fs_bg_free_inodes_گنتی(fs, گروہ) - inuse);
	إذا (isdir)
		ext2fs_bg_ورتیا_dirs_گنتی_سیٹ(fs, گروہ, ext2fs_bg_ورتیا_dirs_گنتی(fs, گروہ) + inuse);

	/* We don't strictly need to be clearing the uninit flag if inuse < 0
	 * (i.e. freeing inodes) but it also means something is bad. */
	ext2fs_bg_flags_clear(fs, گروہ, EXT2_BG_INODE_UNINIT);
	إذا (ext2fs_has_گروہ_desc_csum(fs)) {
		ext2_ino_t پہلا_بن_ورتیا_inode =	fs->super->s_inodes_فی_گروہ -
			ext2fs_bg_itable_بن_ورتیا(fs, گروہ) +
			گروہ * fs->super->s_inodes_فی_گروہ + 1;

		إذا (ino >= پہلا_بن_ورتیا_inode)
			ext2fs_bg_itable_بن_ورتیا_سیٹ(fs, گروہ, گروہ * fs->super->s_inodes_فی_گروہ + fs->super->s_inodes_فی_گروہ - ino);
		ext2fs_گروہ_desc_csum_سیٹ(fs, گروہ);
	}

	fs->super->s_free_inodes_گنتی -= inuse;
	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_ib_dirty(fs);
}

فراغ ext2fs_inode_alloc_stats(ext2_filsys fs, ext2_ino_t ino, صحيح inuse)
{
	ext2fs_inode_alloc_stats2(fs, ino, inuse, 0);
}

فراغ ext2fs_بلاک_alloc_stats2(ext2_filsys fs, blk64_t blk, صحيح inuse)
{
	صحيح	گروہ = ext2fs_گروہ_دا_blk2(fs, blk);

	إذا (blk < fs->super->s_پہلا_انکڑے_بلاک ||
	    blk >= ext2fs_بلاکاں_گنتی(fs->super)) {
#جے_بنا_تعریف OMIT_COM_ERR
		com_err("ext2fs_block_alloc_stats", 0,
			"Illegal block number: %lu", (بنانشانی لما) blk);
#جے_انت
		أرجع;
	}
	إذا (inuse > 0)
		ext2fs_mark_بلاک_bitmap2(fs->بلاک_map, blk);
	وإلا
		ext2fs_unmark_بلاک_bitmap2(fs->بلاک_map, blk);
	ext2fs_bg_free_بلاکاں_گنتی_سیٹ(fs, گروہ, ext2fs_bg_free_بلاکاں_گنتی(fs, گروہ) - inuse);
	ext2fs_bg_flags_clear(fs, گروہ, EXT2_BG_بلاک_UNINIT);
	ext2fs_گروہ_desc_csum_سیٹ(fs, گروہ);

	ext2fs_free_بلاکاں_گنتی_جوڑو(fs->super,
			     -inuse * (blk64_t) EXT2_FS_CLUSTER_تناسب(fs));
	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_bb_dirty(fs);
	إذا (fs->بلاک_alloc_stats)
		(fs->بلاک_alloc_stats)(fs, (blk64_t) blk, inuse);
}

فراغ ext2fs_بلاک_alloc_stats(ext2_filsys fs, blk_t blk, صحيح inuse)
{
	ext2fs_بلاک_alloc_stats2(fs, blk, inuse);
}

فراغ ext2fs_سیٹ_بلاک_alloc_stats_callback(ext2_filsys fs,
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

#جے_بنا_تعریف OMIT_COM_ERR
	إذا (blk + عدد > ext2fs_بلاکاں_گنتی(fs->super)) {
		com_err("ext2fs_block_alloc_stats_range", 0,
			"Illegal block range: %llu (%u) ",
			(بنانشانی لما لما) blk, عدد);
		أرجع;
	}
#جے_انت
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
		صحيح گروہ = ext2fs_گروہ_دا_blk2(fs, blk);
		blk64_t آخری_blk = ext2fs_گروہ_آخری_block2(fs, گروہ);
		blk64_t n = عدد;

		إذا (blk + عدد > آخری_blk)
			n = آخری_blk - blk + 1;

		ext2fs_bg_free_بلاکاں_گنتی_سیٹ(fs, گروہ,
			ext2fs_bg_free_بلاکاں_گنتی(fs, گروہ) -
			inuse*n/EXT2_FS_CLUSTER_تناسب(fs));
		ext2fs_bg_flags_clear(fs, گروہ, EXT2_BG_بلاک_UNINIT);
		ext2fs_گروہ_desc_csum_سیٹ(fs, گروہ);
		ext2fs_free_بلاکاں_گنتی_جوڑو(fs->super, -inuse * (blk64_t) n);
		blk += n;
		عدد -= n;
	}
	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_bb_dirty(fs);
	إذا (fs->بلاک_alloc_stats_range)
		(fs->بلاک_alloc_stats_range)(fs, orig_blk, orig_عدد, inuse);
}

فراغ ext2fs_سیٹ_بلاک_alloc_stats_range_callback(ext2_filsys fs,
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
