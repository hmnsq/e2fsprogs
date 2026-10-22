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
	صحيح	گروهه = ext2fs_گروهه_جو_ino(fs, ino);

	إذا (ino > fs->super->s_inodes_ڳڻپ) {
#جيڪڏهن_وصف_نه_ٿيل OMIT_COM_ERR
		com_err("ext2fs_inode_alloc_stats2", 0,
			"Illegal inode number: %lu", (بنانشان ڊگهو) ino);
#جيڪڏهن_پورو
		أرجع;
	}
	إذا (inuse > 0)
		ext2fs_mark_inode_bitmap2(fs->inode_map, ino);
	وإلا
		ext2fs_unmark_inode_bitmap2(fs->inode_map, ino);
	ext2fs_bg_free_inodes_ڳڻپ_سيٽ(fs, گروهه, ext2fs_bg_free_inodes_ڳڻپ(fs, گروهه) - inuse);
	إذا (isdir)
		ext2fs_bg_استعمال_ٿيل_dirs_ڳڻپ_سيٽ(fs, گروهه, ext2fs_bg_استعمال_ٿيل_dirs_ڳڻپ(fs, گروهه) + inuse);

	/* We don't strictly need to be clearing the uninit flag if inuse < 0
	 * (i.e. freeing inodes) but it also means something is bad. */
	ext2fs_bg_flags_clear(fs, گروهه, EXT2_BG_INODE_UNINIT);
	إذا (ext2fs_has_گروهه_desc_csum(fs)) {
		ext2_ino_t پهريون_اڻ_استعمال_ٿيل_inode =	fs->super->s_inodes_في_گروهه -
			ext2fs_bg_itable_اڻ_استعمال_ٿيل(fs, گروهه) +
			گروهه * fs->super->s_inodes_في_گروهه + 1;

		إذا (ino >= پهريون_اڻ_استعمال_ٿيل_inode)
			ext2fs_bg_itable_اڻ_استعمال_ٿيل_سيٽ(fs, گروهه, گروهه * fs->super->s_inodes_في_گروهه + fs->super->s_inodes_في_گروهه - ino);
		ext2fs_گروهه_desc_csum_سيٽ(fs, گروهه);
	}

	fs->super->s_free_inodes_ڳڻپ -= inuse;
	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_ib_dirty(fs);
}

فراغ ext2fs_inode_alloc_stats(ext2_filsys fs, ext2_ino_t ino, صحيح inuse)
{
	ext2fs_inode_alloc_stats2(fs, ino, inuse, 0);
}

فراغ ext2fs_بلاڪ_alloc_stats2(ext2_filsys fs, blk64_t blk, صحيح inuse)
{
	صحيح	گروهه = ext2fs_گروهه_جو_blk2(fs, blk);

	إذا (blk < fs->super->s_پهريون_معلومات_بلاڪ ||
	    blk >= ext2fs_بلاڪون_ڳڻپ(fs->super)) {
#جيڪڏهن_وصف_نه_ٿيل OMIT_COM_ERR
		com_err("ext2fs_block_alloc_stats", 0,
			"Illegal block number: %lu", (بنانشان ڊگهو) blk);
#جيڪڏهن_پورو
		أرجع;
	}
	إذا (inuse > 0)
		ext2fs_mark_بلاڪ_bitmap2(fs->بلاڪ_map, blk);
	وإلا
		ext2fs_unmark_بلاڪ_bitmap2(fs->بلاڪ_map, blk);
	ext2fs_bg_free_بلاڪون_ڳڻپ_سيٽ(fs, گروهه, ext2fs_bg_free_بلاڪون_ڳڻپ(fs, گروهه) - inuse);
	ext2fs_bg_flags_clear(fs, گروهه, EXT2_BG_بلاڪ_UNINIT);
	ext2fs_گروهه_desc_csum_سيٽ(fs, گروهه);

	ext2fs_free_بلاڪون_ڳڻپ_جوڙيو(fs->super,
			     -inuse * (blk64_t) EXT2_FS_CLUSTER_تناسب(fs));
	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_bb_dirty(fs);
	إذا (fs->بلاڪ_alloc_stats)
		(fs->بلاڪ_alloc_stats)(fs, (blk64_t) blk, inuse);
}

فراغ ext2fs_بلاڪ_alloc_stats(ext2_filsys fs, blk_t blk, صحيح inuse)
{
	ext2fs_بلاڪ_alloc_stats2(fs, blk, inuse);
}

فراغ ext2fs_سيٽ_بلاڪ_alloc_stats_callback(ext2_filsys fs,
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
		*old = fs->بلاڪ_alloc_stats;

	fs->بلاڪ_alloc_stats = func;
}

فراغ ext2fs_بلاڪ_alloc_stats_range(ext2_filsys fs, blk64_t blk,
				    blk_t انگ, صحيح inuse)
{
	ثابت blk64_t orig_blk = blk;
	ثابت blk_t orig_انگ = انگ;

#جيڪڏهن_وصف_نه_ٿيل OMIT_COM_ERR
	إذا (blk + انگ > ext2fs_بلاڪون_ڳڻپ(fs->super)) {
		com_err("ext2fs_block_alloc_stats_range", 0,
			"Illegal block range: %llu (%u) ",
			(بنانشان ڊگهو ڊگهو) blk, انگ);
		أرجع;
	}
#جيڪڏهن_پورو
	إذا (inuse == 0)
		أرجع;
	إذا (inuse > 0) {
		ext2fs_mark_بلاڪ_bitmap_range2(fs->بلاڪ_map, blk, انگ);
		inuse = 1;
	} وإلا {
		ext2fs_unmark_بلاڪ_bitmap_range2(fs->بلاڪ_map, blk, انگ);
		inuse = -1;
	}
	طالما (انگ) {
		صحيح گروهه = ext2fs_گروهه_جو_blk2(fs, blk);
		blk64_t آخري_blk = ext2fs_گروهه_آخري_block2(fs, گروهه);
		blk64_t n = انگ;

		إذا (blk + انگ > آخري_blk)
			n = آخري_blk - blk + 1;

		ext2fs_bg_free_بلاڪون_ڳڻپ_سيٽ(fs, گروهه,
			ext2fs_bg_free_بلاڪون_ڳڻپ(fs, گروهه) -
			inuse*n/EXT2_FS_CLUSTER_تناسب(fs));
		ext2fs_bg_flags_clear(fs, گروهه, EXT2_BG_بلاڪ_UNINIT);
		ext2fs_گروهه_desc_csum_سيٽ(fs, گروهه);
		ext2fs_free_بلاڪون_ڳڻپ_جوڙيو(fs->super, -inuse * (blk64_t) n);
		blk += n;
		انگ -= n;
	}
	ext2fs_mark_super_dirty(fs);
	ext2fs_mark_bb_dirty(fs);
	إذا (fs->بلاڪ_alloc_stats_range)
		(fs->بلاڪ_alloc_stats_range)(fs, orig_blk, orig_انگ, inuse);
}

فراغ ext2fs_سيٽ_بلاڪ_alloc_stats_range_callback(ext2_filsys fs,
	فراغ (*func)(ext2_filsys fs, blk64_t blk,
				    blk_t انگ, صحيح inuse),
	فراغ (**old)(ext2_filsys fs, blk64_t blk,
				    blk_t انگ, صحيح inuse))
{
	إذا (!fs || fs->magic != EXT2_ET_MAGIC_EXT2FS_FILSYS)
		أرجع;
	إذا (old)
		*old = fs->بلاڪ_alloc_stats_range;

	fs->بلاڪ_alloc_stats_range = func;
}
