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
	صحيح	گروه = ext2fs_گروه_از_ino(fs, ino);

	إذا (ino > fs->super->s_inodes_شمار) {
#اگر_تعریف_نشده OMIT_COM_ERR
		com_err("ext2fs_inode_alloc_stats2", 0,
			"Illegal inode number: %lu", (بیعلامت دراز) ino);
#پایان_اگر
		أرجع;
	}
	إذا (inuse > 0)
		ext2fs_mark_inode_bitmap2(fs->inode_map, ino);
	وإلا
		ext2fs_unmark_inode_bitmap2(fs->inode_map, ino);
	ext2fs_bg_free_inodes_شمار_مجموعه(fs, گروه, ext2fs_bg_free_inodes_شمار(fs, گروه) - inuse);
	إذا (isdir)
		ext2fs_bg_استفاده_شده_dirs_شمار_مجموعه(fs, گروه, ext2fs_bg_استفاده_شده_dirs_شمار(fs, گروه) + inuse);

	/* We don't strictly need to be clearing the uninit flag if inuse < 0
	 * (i.e. freeing inodes) but it also means something is bad. */
	ext2fs_bg_flags_clear(fs, گروه, EXT2_BG_INODE_UNINIT);
	إذا (ext2fs_has_گروه_desc_csum(fs)) {
		ext2_ino_t اولی_استفادهنشده_inode =	fs->super->s_inodes_در_هر_گروه -
			ext2fs_bg_itable_استفادهنشده(fs, گروه) +
			گروه * fs->super->s_inodes_در_هر_گروه + 1;

		إذا (ino >= اولی_استفادهنشده_inode)
			ext2fs_bg_itable_استفادهنشده_مجموعه(fs, گروه, گروه * fs->super->s_inodes_در_هر_گروه + fs->super->s_inodes_در_هر_گروه - ino);
		ext2fs_گروه_desc_csum_مجموعه(fs, گروه);
	}

	fs->super->s_free_inodes_شمار -= inuse;
	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_ib_dirty(fs);
}

فراغ ext2fs_inode_alloc_stats(ext2_filsys fs, ext2_ino_t ino, صحيح inuse)
{
	ext2fs_inode_alloc_stats2(fs, ino, inuse, 0);
}

فراغ ext2fs_بلوک_alloc_stats2(ext2_filsys fs, blk64_t blk, صحيح inuse)
{
	صحيح	گروه = ext2fs_گروه_از_blk2(fs, blk);

	إذا (blk < fs->super->s_اولی_داده_بلوک ||
	    blk >= ext2fs_بلوکها_شمار(fs->super)) {
#اگر_تعریف_نشده OMIT_COM_ERR
		com_err("ext2fs_block_alloc_stats", 0,
			"Illegal block number: %lu", (بیعلامت دراز) blk);
#پایان_اگر
		أرجع;
	}
	إذا (inuse > 0)
		ext2fs_mark_بلوک_bitmap2(fs->بلوک_map, blk);
	وإلا
		ext2fs_unmark_بلوک_bitmap2(fs->بلوک_map, blk);
	ext2fs_bg_free_بلوکها_شمار_مجموعه(fs, گروه, ext2fs_bg_free_بلوکها_شمار(fs, گروه) - inuse);
	ext2fs_bg_flags_clear(fs, گروه, EXT2_BG_بلوک_UNINIT);
	ext2fs_گروه_desc_csum_مجموعه(fs, گروه);

	ext2fs_free_بلوکها_شمار_جمع_کن(fs->super,
			     -inuse * (blk64_t) EXT2_FS_CLUSTER_نسبت(fs));
	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_bb_dirty(fs);
	إذا (fs->بلوک_alloc_stats)
		(fs->بلوک_alloc_stats)(fs, (blk64_t) blk, inuse);
}

فراغ ext2fs_بلوک_alloc_stats(ext2_filsys fs, blk_t blk, صحيح inuse)
{
	ext2fs_بلوک_alloc_stats2(fs, blk, inuse);
}

فراغ ext2fs_مجموعه_بلوک_alloc_stats_callback(ext2_filsys fs,
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
		*old = fs->بلوک_alloc_stats;

	fs->بلوک_alloc_stats = func;
}

فراغ ext2fs_بلوک_alloc_stats_range(ext2_filsys fs, blk64_t blk,
				    blk_t رقم, صحيح inuse)
{
	ثابت blk64_t orig_blk = blk;
	ثابت blk_t orig_رقم = رقم;

#اگر_تعریف_نشده OMIT_COM_ERR
	إذا (blk + رقم > ext2fs_بلوکها_شمار(fs->super)) {
		com_err("ext2fs_block_alloc_stats_range", 0,
			"Illegal block range: %llu (%u) ",
			(بیعلامت دراز دراز) blk, رقم);
		أرجع;
	}
#پایان_اگر
	إذا (inuse == 0)
		أرجع;
	إذا (inuse > 0) {
		ext2fs_mark_بلوک_bitmap_range2(fs->بلوک_map, blk, رقم);
		inuse = 1;
	} وإلا {
		ext2fs_unmark_بلوک_bitmap_range2(fs->بلوک_map, blk, رقم);
		inuse = -1;
	}
	طالما (رقم) {
		صحيح گروه = ext2fs_گروه_از_blk2(fs, blk);
		blk64_t آخرین_blk = ext2fs_گروه_آخرین_block2(fs, گروه);
		blk64_t n = رقم;

		إذا (blk + رقم > آخرین_blk)
			n = آخرین_blk - blk + 1;

		ext2fs_bg_free_بلوکها_شمار_مجموعه(fs, گروه,
			ext2fs_bg_free_بلوکها_شمار(fs, گروه) -
			inuse*n/EXT2_FS_CLUSTER_نسبت(fs));
		ext2fs_bg_flags_clear(fs, گروه, EXT2_BG_بلوک_UNINIT);
		ext2fs_گروه_desc_csum_مجموعه(fs, گروه);
		ext2fs_free_بلوکها_شمار_جمع_کن(fs->super, -inuse * (blk64_t) n);
		blk += n;
		رقم -= n;
	}
	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_bb_dirty(fs);
	إذا (fs->بلوک_alloc_stats_range)
		(fs->بلوک_alloc_stats_range)(fs, orig_blk, orig_رقم, inuse);
}

فراغ ext2fs_مجموعه_بلوک_alloc_stats_range_callback(ext2_filsys fs,
	فراغ (*func)(ext2_filsys fs, blk64_t blk,
				    blk_t رقم, صحيح inuse),
	فراغ (**old)(ext2_filsys fs, blk64_t blk,
				    blk_t رقم, صحيح inuse))
{
	إذا (!fs || fs->magic != EXT2_ET_MAGIC_EXT2FS_FILSYS)
		أرجع;
	إذا (old)
		*old = fs->بلوک_alloc_stats_range;

	fs->بلوک_alloc_stats_range = func;
}
