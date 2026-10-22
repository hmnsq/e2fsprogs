/*
 * bitmaps.c --- routines to read, write, and manipulate the inode and
 * block bitmaps.
 *
 * Copyright (C) 1993, 1994, 1995, 1996 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#د_که_پای
#ضمن <fcntl.h>
#ضمن <time.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#د_که_پای
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#د_که_پای

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "ext2fsP.h"
#ضمن "bmap64.h"

فراغ ext2fs_free_inode_bitmap(ext2fs_inode_bitmap bitmap)
{
	ext2fs_free_generic_bmap(bitmap);
}

فراغ ext2fs_free_بلاک_bitmap(ext2fs_بلاک_bitmap bitmap)
{
	ext2fs_free_generic_bmap(bitmap);
}

errcode_t ext2fs_copy_bitmap(ext2fs_generic_bitmap src,
			     ext2fs_generic_bitmap *dest)
{
	أرجع (ext2fs_copy_generic_bmap(src, dest));
}
فراغ ext2fs_سېټ_bitmap_padding(ext2fs_generic_bitmap map)
{
	ext2fs_سېټ_generic_bmap_padding(map);
}

errcode_t ext2fs_allocate_inode_bitmap(ext2_filsys fs,
				       ثابت حرف *descr,
				       ext2fs_inode_bitmap *ret)
{
	__u64		پیل, پای_ټکی, real_پای_ټکی;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	إذا (ext2fs_has_feature_journal_dev(fs->super))
		أرجع EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	fs->write_bitmaps = ext2fs_write_bitmaps;

	پیل = 1;
	پای_ټکی = fs->super->s_inodes_شمېر;
	real_پای_ټکی = (__u64)EXT2_INODES_فی_ډله(fs->super) *
		fs->ډله_desc_شمېر;

	/* Are we permitted to use new-style bitmaps? */
	إذا (fs->flags & EXT2_FLAG_64_بټونه)
		أرجع (ext2fs_alloc_generic_bmap(fs,
				EXT2_ET_MAGIC_INODE_BITMAP64,
				fs->default_bitmap_type,
				پیل, پای_ټکی, real_پای_ټکی, descr, ret));

	/* Otherwise, check to see if the file system is small enough
	 * to use old-style 32-bit bitmaps */
	إذا ((پای_ټکی > ~0U) || (real_پای_ټکی > ~0U))
		أرجع EXT2_ET_CANT_USE_LEGACY_BITMAPS;

	أرجع (ext2fs_make_generic_bitmap(EXT2_ET_MAGIC_INODE_BITMAP, fs,
					 پیل, پای_ټکی, real_پای_ټکی,
					 descr, 0,
					 (ext2fs_generic_bitmap *) ret));
}

errcode_t ext2fs_allocate_بلاک_bitmap(ext2_filsys fs,
				       ثابت حرف *descr,
				       ext2fs_بلاک_bitmap *ret)
{
	__u64		پیل, پای_ټکی, real_پای_ټکی;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	إذا (ext2fs_has_feature_journal_dev(fs->super))
		أرجع EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	fs->write_bitmaps = ext2fs_write_bitmaps;

	پیل = EXT2FS_B2C(fs, fs->super->s_لومړی_معلومات_بلاک);
	پای_ټکی = EXT2FS_B2C(fs, ext2fs_بلاکونه_شمېر(fs->super)-1);
	real_پای_ټکی = ((__u64) EXT2_CLUSTERS_فی_ډله(fs->super)
		    * (__u64) fs->ډله_desc_شمېر)-1 + پیل;

	إذا (fs->flags & EXT2_FLAG_64_بټونه)
		أرجع (ext2fs_alloc_generic_bmap(fs,
				EXT2_ET_MAGIC_بلاک_BITMAP64,
				fs->default_bitmap_type,
				پیل, پای_ټکی, real_پای_ټکی, descr, ret));

	إذا ((پای_ټکی > ~0U) || (real_پای_ټکی > ~0U))
		أرجع EXT2_ET_CANT_USE_LEGACY_BITMAPS;

	أرجع (ext2fs_make_generic_bitmap(EXT2_ET_MAGIC_بلاک_BITMAP, fs,
					   پیل, پای_ټکی, real_پای_ټکی,
					   descr, 0,
					   (ext2fs_generic_bitmap *) ret));
}

/*
 * ext2fs_allocate_block_bitmap() really allocates a per-cluster
 * bitmap for backwards compatibility.  This function allocates a
 * block bitmap which is truly per-block, even if clusters/bigalloc
 * are enabled.  mke2fs and e2fsck need this for tracking the
 * allocation of the file system metadata blocks.
 */
errcode_t ext2fs_allocate_subcluster_bitmap(ext2_filsys fs,
					    ثابت حرف *descr,
					    ext2fs_بلاک_bitmap *ret)
{
	__u64			پیل, پای_ټکی, real_پای_ټکی;
	ext2fs_generic_bitmap	bmap;
	ext2fs_generic_bitmap_64 bmap64;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	إذا (ext2fs_has_feature_journal_dev(fs->super))
		أرجع EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	fs->write_bitmaps = ext2fs_write_bitmaps;

	إذا (!fs->cluster_نسبت_بټونه)
		أرجع ext2fs_allocate_بلاک_bitmap(fs, descr, ret);

	إذا ((fs->flags & EXT2_FLAG_64_بټونه) == 0)
		أرجع EXT2_ET_CANT_USE_LEGACY_BITMAPS;

	پیل = fs->super->s_لومړی_معلومات_بلاک;
	پای_ټکی = ext2fs_بلاکونه_شمېر(fs->super)-1;
	real_پای_ټکی = ((__u64) EXT2_بلاکونه_فی_ډله(fs->super)
		    * (__u64) fs->ډله_desc_شمېر)-1 + پیل;

	retval = ext2fs_alloc_generic_bmap(fs, EXT2_ET_MAGIC_بلاک_BITMAP64,
					   fs->default_bitmap_type, پیل,
					   پای_ټکی, real_پای_ټکی, descr, &bmap);
	إذا (retval)
		أرجع retval;
	bmap64 = (ext2fs_generic_bitmap_64) bmap;
	bmap64->cluster_بټونه = 0;
	*ret = bmap;
	أرجع 0;
}

صحيح ext2fs_get_bitmap_granularity(ext2fs_بلاک_bitmap bitmap)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) bitmap;

	إذا (!EXT2FS_IS_64_BITMAP(bmap))
		أرجع 0;

	أرجع bmap->cluster_بټونه;
}

errcode_t ext2fs_fudge_inode_bitmap_پای_ټکی(ext2fs_inode_bitmap bitmap,
					ext2_ino_t پای_ټکی, ext2_ino_t *oend)
{
	__u64 tmp_oend;
	صحيح retval;

	retval = ext2fs_fudge_generic_bmap_پای_ټکی((ext2fs_generic_bitmap) bitmap,
					       EXT2_ET_FUDGE_INODE_BITMAP_پای_ټکی,
					       پای_ټکی, &tmp_oend);
	إذا (oend)
		*oend = tmp_oend;
	أرجع retval;
}

errcode_t ext2fs_fudge_بلاک_bitmap_پای_ټکی(ext2fs_بلاک_bitmap bitmap,
					blk_t پای_ټکی, blk_t *oend)
{
	أرجع (ext2fs_fudge_generic_bitmap_پای_ټکی(bitmap,
						EXT2_ET_MAGIC_بلاک_BITMAP,
						EXT2_ET_FUDGE_بلاک_BITMAP_پای_ټکی,
						پای_ټکی, oend));
}

errcode_t ext2fs_fudge_بلاک_bitmap_end2(ext2fs_بلاک_bitmap bitmap,
					 blk64_t پای_ټکی, blk64_t *oend)
{
	أرجع (ext2fs_fudge_generic_bmap_پای_ټکی(bitmap,
					      EXT2_ET_FUDGE_بلاک_BITMAP_پای_ټکی,
					      پای_ټکی, oend));
}

فراغ ext2fs_clear_inode_bitmap(ext2fs_inode_bitmap bitmap)
{
	ext2fs_clear_generic_bmap(bitmap);
}

فراغ ext2fs_clear_بلاک_bitmap(ext2fs_بلاک_bitmap bitmap)
{
	ext2fs_clear_generic_bmap(bitmap);
}

errcode_t ext2fs_resize_inode_bitmap(__u32 new_پای_ټکی, __u32 new_real_پای_ټکی,
				     ext2fs_inode_bitmap bmap)
{
	أرجع (ext2fs_resize_generic_bitmap(EXT2_ET_MAGIC_INODE_BITMAP,
					     new_پای_ټکی, new_real_پای_ټکی, bmap));
}

errcode_t ext2fs_resize_inode_bitmap2(__u64 new_پای_ټکی, __u64 new_real_پای_ټکی,
				      ext2fs_inode_bitmap bmap)
{
	أرجع (ext2fs_resize_generic_bmap(bmap, new_پای_ټکی, new_real_پای_ټکی));
}

errcode_t ext2fs_resize_بلاک_bitmap(__u32 new_پای_ټکی, __u32 new_real_پای_ټکی,
				     ext2fs_بلاک_bitmap bmap)
{
	أرجع (ext2fs_resize_generic_bitmap(EXT2_ET_MAGIC_بلاک_BITMAP,
					     new_پای_ټکی, new_real_پای_ټکی, bmap));
}

errcode_t ext2fs_resize_بلاک_bitmap2(__u64 new_پای_ټکی, __u64 new_real_پای_ټکی,
				      ext2fs_بلاک_bitmap bmap)
{
	أرجع (ext2fs_resize_generic_bmap(bmap, new_پای_ټکی, new_real_پای_ټکی));
}

errcode_t ext2fs_compare_بلاک_bitmap(ext2fs_بلاک_bitmap bm1,
				      ext2fs_بلاک_bitmap bm2)
{
	أرجع (ext2fs_compare_generic_bmap(EXT2_ET_NEQ_بلاک_BITMAP,
					    bm1, bm2));
}

errcode_t ext2fs_compare_inode_bitmap(ext2fs_inode_bitmap bm1,
				      ext2fs_inode_bitmap bm2)
{
	أرجع (ext2fs_compare_generic_bmap(EXT2_ET_NEQ_INODE_BITMAP,
					    bm1, bm2));
}

errcode_t ext2fs_سېټ_inode_bitmap_range(ext2fs_inode_bitmap bmap,
					ext2_ino_t پیل, بینښه صحيح عدد,
					فراغ *دننه)
{
	أرجع (ext2fs_سېټ_generic_bitmap_range(bmap,
						EXT2_ET_MAGIC_INODE_BITMAP,
						پیل, عدد, دننه));
}

errcode_t ext2fs_سېټ_inode_bitmap_range2(ext2fs_inode_bitmap bmap,
					 __u64 پیل, size_t عدد,
					 فراغ *دننه)
{
	أرجع (ext2fs_سېټ_generic_bmap_range(bmap, پیل, عدد, دننه));
}

errcode_t ext2fs_get_inode_bitmap_range(ext2fs_inode_bitmap bmap,
					ext2_ino_t پیل, بینښه صحيح عدد,
					فراغ *بهر)
{
	أرجع (ext2fs_get_generic_bitmap_range(bmap,
						EXT2_ET_MAGIC_INODE_BITMAP,
						پیل, عدد, بهر));
}

errcode_t ext2fs_get_inode_bitmap_range2(ext2fs_inode_bitmap bmap,
					 __u64 پیل, size_t عدد,
					 فراغ *بهر)
{
	أرجع (ext2fs_get_generic_bmap_range(bmap, پیل, عدد, بهر));
}

errcode_t ext2fs_سېټ_بلاک_bitmap_range(ext2fs_بلاک_bitmap bmap,
					blk_t پیل, بینښه صحيح عدد,
					فراغ *دننه)
{
	أرجع (ext2fs_سېټ_generic_bitmap_range(bmap,
						EXT2_ET_MAGIC_بلاک_BITMAP,
						پیل, عدد, دننه));
}

errcode_t ext2fs_سېټ_بلاک_bitmap_range2(ext2fs_بلاک_bitmap bmap,
					 blk64_t پیل, size_t عدد,
					 فراغ *دننه)
{
	أرجع (ext2fs_سېټ_generic_bmap_range(bmap, پیل, عدد, دننه));
}

errcode_t ext2fs_get_بلاک_bitmap_range(ext2fs_بلاک_bitmap bmap,
					blk_t پیل, بینښه صحيح عدد,
					فراغ *بهر)
{
	أرجع (ext2fs_get_generic_bitmap_range(bmap,
						EXT2_ET_MAGIC_بلاک_BITMAP,
						پیل, عدد, بهر));
}

errcode_t ext2fs_get_بلاک_bitmap_range2(ext2fs_بلاک_bitmap bmap,
					 blk64_t پیل, size_t عدد,
					 فراغ *بهر)
{
	أرجع (ext2fs_get_generic_bmap_range(bmap, پیل, عدد, بهر));
}
