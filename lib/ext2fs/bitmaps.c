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
#پایان_اگر
#ضمن <fcntl.h>
#ضمن <time.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#پایان_اگر
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#پایان_اگر

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "ext2fsP.h"
#ضمن "bmap64.h"

فراغ ext2fs_free_inode_bitmap(ext2fs_inode_bitmap bitmap)
{
	ext2fs_free_generic_bmap(bitmap);
}

فراغ ext2fs_free_بلوک_bitmap(ext2fs_بلوک_bitmap bitmap)
{
	ext2fs_free_generic_bmap(bitmap);
}

errcode_t ext2fs_copy_bitmap(ext2fs_generic_bitmap src,
			     ext2fs_generic_bitmap *dest)
{
	أرجع (ext2fs_copy_generic_bmap(src, dest));
}
فراغ ext2fs_مجموعه_bitmap_padding(ext2fs_generic_bitmap map)
{
	ext2fs_مجموعه_generic_bmap_padding(map);
}

errcode_t ext2fs_allocate_inode_bitmap(ext2_filsys fs,
				       ثابت حرف *descr,
				       ext2fs_inode_bitmap *ret)
{
	__u64		شروع, انتها, real_انتها;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	إذا (ext2fs_has_feature_journal_dev(fs->super))
		أرجع EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	fs->write_bitmaps = ext2fs_write_bitmaps;

	شروع = 1;
	انتها = fs->super->s_inodes_شمار;
	real_انتها = (__u64)EXT2_INODES_در_هر_گروه(fs->super) *
		fs->گروه_desc_شمار;

	/* Are we permitted to use new-style bitmaps? */
	إذا (fs->flags & EXT2_FLAG_64_بیتها)
		أرجع (ext2fs_alloc_generic_bmap(fs,
				EXT2_ET_MAGIC_INODE_BITMAP64,
				fs->default_bitmap_type,
				شروع, انتها, real_انتها, descr, ret));

	/* Otherwise, check to see if the file system is small enough
	 * to use old-style 32-bit bitmaps */
	إذا ((انتها > ~0U) || (real_انتها > ~0U))
		أرجع EXT2_ET_CANT_USE_LEGACY_BITMAPS;

	أرجع (ext2fs_make_generic_bitmap(EXT2_ET_MAGIC_INODE_BITMAP, fs,
					 شروع, انتها, real_انتها,
					 descr, 0,
					 (ext2fs_generic_bitmap *) ret));
}

errcode_t ext2fs_allocate_بلوک_bitmap(ext2_filsys fs,
				       ثابت حرف *descr,
				       ext2fs_بلوک_bitmap *ret)
{
	__u64		شروع, انتها, real_انتها;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	إذا (ext2fs_has_feature_journal_dev(fs->super))
		أرجع EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	fs->write_bitmaps = ext2fs_write_bitmaps;

	شروع = EXT2FS_B2C(fs, fs->super->s_اولی_داده_بلوک);
	انتها = EXT2FS_B2C(fs, ext2fs_بلوکها_شمار(fs->super)-1);
	real_انتها = ((__u64) EXT2_CLUSTERS_در_هر_گروه(fs->super)
		    * (__u64) fs->گروه_desc_شمار)-1 + شروع;

	إذا (fs->flags & EXT2_FLAG_64_بیتها)
		أرجع (ext2fs_alloc_generic_bmap(fs,
				EXT2_ET_MAGIC_بلوک_BITMAP64,
				fs->default_bitmap_type,
				شروع, انتها, real_انتها, descr, ret));

	إذا ((انتها > ~0U) || (real_انتها > ~0U))
		أرجع EXT2_ET_CANT_USE_LEGACY_BITMAPS;

	أرجع (ext2fs_make_generic_bitmap(EXT2_ET_MAGIC_بلوک_BITMAP, fs,
					   شروع, انتها, real_انتها,
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
					    ext2fs_بلوک_bitmap *ret)
{
	__u64			شروع, انتها, real_انتها;
	ext2fs_generic_bitmap	bmap;
	ext2fs_generic_bitmap_64 bmap64;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	إذا (ext2fs_has_feature_journal_dev(fs->super))
		أرجع EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	fs->write_bitmaps = ext2fs_write_bitmaps;

	إذا (!fs->cluster_نسبت_بیتها)
		أرجع ext2fs_allocate_بلوک_bitmap(fs, descr, ret);

	إذا ((fs->flags & EXT2_FLAG_64_بیتها) == 0)
		أرجع EXT2_ET_CANT_USE_LEGACY_BITMAPS;

	شروع = fs->super->s_اولی_داده_بلوک;
	انتها = ext2fs_بلوکها_شمار(fs->super)-1;
	real_انتها = ((__u64) EXT2_بلوکها_در_هر_گروه(fs->super)
		    * (__u64) fs->گروه_desc_شمار)-1 + شروع;

	retval = ext2fs_alloc_generic_bmap(fs, EXT2_ET_MAGIC_بلوک_BITMAP64,
					   fs->default_bitmap_type, شروع,
					   انتها, real_انتها, descr, &bmap);
	إذا (retval)
		أرجع retval;
	bmap64 = (ext2fs_generic_bitmap_64) bmap;
	bmap64->cluster_بیتها = 0;
	*ret = bmap;
	أرجع 0;
}

صحيح ext2fs_get_bitmap_granularity(ext2fs_بلوک_bitmap bitmap)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) bitmap;

	إذا (!EXT2FS_IS_64_BITMAP(bmap))
		أرجع 0;

	أرجع bmap->cluster_بیتها;
}

errcode_t ext2fs_fudge_inode_bitmap_انتها(ext2fs_inode_bitmap bitmap,
					ext2_ino_t انتها, ext2_ino_t *oend)
{
	__u64 tmp_oend;
	صحيح retval;

	retval = ext2fs_fudge_generic_bmap_انتها((ext2fs_generic_bitmap) bitmap,
					       EXT2_ET_FUDGE_INODE_BITMAP_انتها,
					       انتها, &tmp_oend);
	إذا (oend)
		*oend = tmp_oend;
	أرجع retval;
}

errcode_t ext2fs_fudge_بلوک_bitmap_انتها(ext2fs_بلوک_bitmap bitmap,
					blk_t انتها, blk_t *oend)
{
	أرجع (ext2fs_fudge_generic_bitmap_انتها(bitmap,
						EXT2_ET_MAGIC_بلوک_BITMAP,
						EXT2_ET_FUDGE_بلوک_BITMAP_انتها,
						انتها, oend));
}

errcode_t ext2fs_fudge_بلوک_bitmap_end2(ext2fs_بلوک_bitmap bitmap,
					 blk64_t انتها, blk64_t *oend)
{
	أرجع (ext2fs_fudge_generic_bmap_انتها(bitmap,
					      EXT2_ET_FUDGE_بلوک_BITMAP_انتها,
					      انتها, oend));
}

فراغ ext2fs_clear_inode_bitmap(ext2fs_inode_bitmap bitmap)
{
	ext2fs_clear_generic_bmap(bitmap);
}

فراغ ext2fs_clear_بلوک_bitmap(ext2fs_بلوک_bitmap bitmap)
{
	ext2fs_clear_generic_bmap(bitmap);
}

errcode_t ext2fs_resize_inode_bitmap(__u32 new_انتها, __u32 new_real_انتها,
				     ext2fs_inode_bitmap bmap)
{
	أرجع (ext2fs_resize_generic_bitmap(EXT2_ET_MAGIC_INODE_BITMAP,
					     new_انتها, new_real_انتها, bmap));
}

errcode_t ext2fs_resize_inode_bitmap2(__u64 new_انتها, __u64 new_real_انتها,
				      ext2fs_inode_bitmap bmap)
{
	أرجع (ext2fs_resize_generic_bmap(bmap, new_انتها, new_real_انتها));
}

errcode_t ext2fs_resize_بلوک_bitmap(__u32 new_انتها, __u32 new_real_انتها,
				     ext2fs_بلوک_bitmap bmap)
{
	أرجع (ext2fs_resize_generic_bitmap(EXT2_ET_MAGIC_بلوک_BITMAP,
					     new_انتها, new_real_انتها, bmap));
}

errcode_t ext2fs_resize_بلوک_bitmap2(__u64 new_انتها, __u64 new_real_انتها,
				      ext2fs_بلوک_bitmap bmap)
{
	أرجع (ext2fs_resize_generic_bmap(bmap, new_انتها, new_real_انتها));
}

errcode_t ext2fs_compare_بلوک_bitmap(ext2fs_بلوک_bitmap bm1,
				      ext2fs_بلوک_bitmap bm2)
{
	أرجع (ext2fs_compare_generic_bmap(EXT2_ET_NEQ_بلوک_BITMAP,
					    bm1, bm2));
}

errcode_t ext2fs_compare_inode_bitmap(ext2fs_inode_bitmap bm1,
				      ext2fs_inode_bitmap bm2)
{
	أرجع (ext2fs_compare_generic_bmap(EXT2_ET_NEQ_INODE_BITMAP,
					    bm1, bm2));
}

errcode_t ext2fs_مجموعه_inode_bitmap_range(ext2fs_inode_bitmap bmap,
					ext2_ino_t شروع, بیعلامت صحيح رقم,
					فراغ *درون)
{
	أرجع (ext2fs_مجموعه_generic_bitmap_range(bmap,
						EXT2_ET_MAGIC_INODE_BITMAP,
						شروع, رقم, درون));
}

errcode_t ext2fs_مجموعه_inode_bitmap_range2(ext2fs_inode_bitmap bmap,
					 __u64 شروع, size_t رقم,
					 فراغ *درون)
{
	أرجع (ext2fs_مجموعه_generic_bmap_range(bmap, شروع, رقم, درون));
}

errcode_t ext2fs_get_inode_bitmap_range(ext2fs_inode_bitmap bmap,
					ext2_ino_t شروع, بیعلامت صحيح رقم,
					فراغ *بیرون)
{
	أرجع (ext2fs_get_generic_bitmap_range(bmap,
						EXT2_ET_MAGIC_INODE_BITMAP,
						شروع, رقم, بیرون));
}

errcode_t ext2fs_get_inode_bitmap_range2(ext2fs_inode_bitmap bmap,
					 __u64 شروع, size_t رقم,
					 فراغ *بیرون)
{
	أرجع (ext2fs_get_generic_bmap_range(bmap, شروع, رقم, بیرون));
}

errcode_t ext2fs_مجموعه_بلوک_bitmap_range(ext2fs_بلوک_bitmap bmap,
					blk_t شروع, بیعلامت صحيح رقم,
					فراغ *درون)
{
	أرجع (ext2fs_مجموعه_generic_bitmap_range(bmap,
						EXT2_ET_MAGIC_بلوک_BITMAP,
						شروع, رقم, درون));
}

errcode_t ext2fs_مجموعه_بلوک_bitmap_range2(ext2fs_بلوک_bitmap bmap,
					 blk64_t شروع, size_t رقم,
					 فراغ *درون)
{
	أرجع (ext2fs_مجموعه_generic_bmap_range(bmap, شروع, رقم, درون));
}

errcode_t ext2fs_get_بلوک_bitmap_range(ext2fs_بلوک_bitmap bmap,
					blk_t شروع, بیعلامت صحيح رقم,
					فراغ *بیرون)
{
	أرجع (ext2fs_get_generic_bitmap_range(bmap,
						EXT2_ET_MAGIC_بلوک_BITMAP,
						شروع, رقم, بیرون));
}

errcode_t ext2fs_get_بلوک_bitmap_range2(ext2fs_بلوک_bitmap bmap,
					 blk64_t شروع, size_t رقم,
					 فراغ *بیرون)
{
	أرجع (ext2fs_get_generic_bmap_range(bmap, شروع, رقم, بیرون));
}
