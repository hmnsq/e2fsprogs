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

#شامل "config.h"
#شامل <stdio.h>

#شامل "ext2_fs.h"
#شامل "ext2fs.h"

خالی ext2fs_inode_alloc_stats2(ext2_filsys fs, ext2_ino_t ino,
			       صحیح inuse, صحیح isdir)
{
	صحیح	گروہ = ext2fs_گروہ_کا_ino(fs, ino);

	اگر (ino > fs->super->s_inodes_گنتی) {
#اگر_غیر_تعریف_شدہ OMIT_COM_ERR
		com_err("ext2fs_inode_alloc_stats2", 0,
			"Illegal inode number: %lu", (غیرعلامتی طویل) ino);
#اگر_ختم
		واپس;
	}
	اگر (inuse > 0)
		ext2fs_mark_inode_bitmap2(fs->inode_map, ino);
	ورنہ
		ext2fs_unmark_inode_bitmap2(fs->inode_map, ino);
	ext2fs_bg_free_inodes_گنتی_سیٹ(fs, گروہ, ext2fs_bg_free_inodes_گنتی(fs, گروہ) - inuse);
	اگر (isdir)
		ext2fs_bg_مستعمل_dirs_گنتی_سیٹ(fs, گروہ, ext2fs_bg_مستعمل_dirs_گنتی(fs, گروہ) + inuse);

	/* We don't strictly need to be clearing the uninit flag if inuse < 0
	 * (i.e. freeing inodes) but it also means something is bad. */
	ext2fs_bg_flags_clear(fs, گروہ, EXT2_BG_INODE_UNINIT);
	اگر (ext2fs_has_گروہ_desc_csum(fs)) {
		ext2_ino_t پہلا_غیر_مستعمل_inode =	fs->super->s_inodes_فی_گروہ -
			ext2fs_bg_itable_غیر_مستعمل(fs, گروہ) +
			گروہ * fs->super->s_inodes_فی_گروہ + 1;

		اگر (ino >= پہلا_غیر_مستعمل_inode)
			ext2fs_bg_itable_غیر_مستعمل_سیٹ(fs, گروہ, گروہ * fs->super->s_inodes_فی_گروہ + fs->super->s_inodes_فی_گروہ - ino);
		ext2fs_گروہ_desc_csum_سیٹ(fs, گروہ);
	}

	fs->super->s_free_inodes_گنتی -= inuse;
	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_ib_dirty(fs);
}

خالی ext2fs_inode_alloc_stats(ext2_filsys fs, ext2_ino_t ino, صحیح inuse)
{
	ext2fs_inode_alloc_stats2(fs, ino, inuse, 0);
}

خالی ext2fs_بلاک_alloc_stats2(ext2_filsys fs, blk64_t blk, صحیح inuse)
{
	صحیح	گروہ = ext2fs_گروہ_کا_blk2(fs, blk);

	اگر (blk < fs->super->s_پہلا_ڈیٹا_بلاک ||
	    blk >= ext2fs_بلاکیں_گنتی(fs->super)) {
#اگر_غیر_تعریف_شدہ OMIT_COM_ERR
		com_err("ext2fs_block_alloc_stats", 0,
			"Illegal block number: %lu", (غیرعلامتی طویل) blk);
#اگر_ختم
		واپس;
	}
	اگر (inuse > 0)
		ext2fs_mark_بلاک_bitmap2(fs->بلاک_map, blk);
	ورنہ
		ext2fs_unmark_بلاک_bitmap2(fs->بلاک_map, blk);
	ext2fs_bg_free_بلاکیں_گنتی_سیٹ(fs, گروہ, ext2fs_bg_free_بلاکیں_گنتی(fs, گروہ) - inuse);
	ext2fs_bg_flags_clear(fs, گروہ, EXT2_BG_بلاک_UNINIT);
	ext2fs_گروہ_desc_csum_سیٹ(fs, گروہ);

	ext2fs_free_بلاکیں_گنتی_جوڑو(fs->super,
			     -inuse * (blk64_t) EXT2_FS_CLUSTER_تناسب(fs));
	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_bb_dirty(fs);
	اگر (fs->بلاک_alloc_stats)
		(fs->بلاک_alloc_stats)(fs, (blk64_t) blk, inuse);
}

خالی ext2fs_بلاک_alloc_stats(ext2_filsys fs, blk_t blk, صحیح inuse)
{
	ext2fs_بلاک_alloc_stats2(fs, blk, inuse);
}

خالی ext2fs_سیٹ_بلاک_alloc_stats_callback(ext2_filsys fs,
					   خالی (*func)(ext2_filsys fs,
							blk64_t blk,
							صحیح inuse),
					   خالی (**old)(ext2_filsys fs,
							blk64_t blk,
							صحیح inuse))
{
	اگر (!fs || fs->magic != EXT2_ET_MAGIC_EXT2FS_FILSYS)
		واپس;
	اگر (old)
		*old = fs->بلاک_alloc_stats;

	fs->بلاک_alloc_stats = func;
}

خالی ext2fs_بلاک_alloc_stats_range(ext2_filsys fs, blk64_t blk,
				    blk_t عدد, صحیح inuse)
{
	ثابت blk64_t orig_blk = blk;
	ثابت blk_t orig_عدد = عدد;

#اگر_غیر_تعریف_شدہ OMIT_COM_ERR
	اگر (blk + عدد > ext2fs_بلاکیں_گنتی(fs->super)) {
		com_err("ext2fs_block_alloc_stats_range", 0,
			"Illegal block range: %llu (%u) ",
			(غیرعلامتی طویل طویل) blk, عدد);
		واپس;
	}
#اگر_ختم
	اگر (inuse == 0)
		واپس;
	اگر (inuse > 0) {
		ext2fs_mark_بلاک_bitmap_range2(fs->بلاک_map, blk, عدد);
		inuse = 1;
	} ورنہ {
		ext2fs_unmark_بلاک_bitmap_range2(fs->بلاک_map, blk, عدد);
		inuse = -1;
	}
	جب (عدد) {
		صحیح گروہ = ext2fs_گروہ_کا_blk2(fs, blk);
		blk64_t آخری_blk = ext2fs_گروہ_آخری_block2(fs, گروہ);
		blk64_t n = عدد;

		اگر (blk + عدد > آخری_blk)
			n = آخری_blk - blk + 1;

		ext2fs_bg_free_بلاکیں_گنتی_سیٹ(fs, گروہ,
			ext2fs_bg_free_بلاکیں_گنتی(fs, گروہ) -
			inuse*n/EXT2_FS_CLUSTER_تناسب(fs));
		ext2fs_bg_flags_clear(fs, گروہ, EXT2_BG_بلاک_UNINIT);
		ext2fs_گروہ_desc_csum_سیٹ(fs, گروہ);
		ext2fs_free_بلاکیں_گنتی_جوڑو(fs->super, -inuse * (blk64_t) n);
		blk += n;
		عدد -= n;
	}
	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_bb_dirty(fs);
	اگر (fs->بلاک_alloc_stats_range)
		(fs->بلاک_alloc_stats_range)(fs, orig_blk, orig_عدد, inuse);
}

خالی ext2fs_سیٹ_بلاک_alloc_stats_range_callback(ext2_filsys fs,
	خالی (*func)(ext2_filsys fs, blk64_t blk,
				    blk_t عدد, صحیح inuse),
	خالی (**old)(ext2_filsys fs, blk64_t blk,
				    blk_t عدد, صحیح inuse))
{
	اگر (!fs || fs->magic != EXT2_ET_MAGIC_EXT2FS_FILSYS)
		واپس;
	اگر (old)
		*old = fs->بلاک_alloc_stats_range;

	fs->بلاک_alloc_stats_range = func;
}
