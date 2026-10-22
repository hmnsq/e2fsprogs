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
#جے_انت
#ضمن <fcntl.h>
#ضمن <time.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#جے_انت
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#جے_انت

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
فراغ ext2fs_سیٹ_bitmap_padding(ext2fs_generic_bitmap map)
{
	ext2fs_سیٹ_generic_bmap_padding(map);
}

errcode_t ext2fs_allocate_inode_bitmap(ext2_filsys fs,
				       ثابت حرف *descr,
				       ext2fs_inode_bitmap *ret)
{
	__u64		شروع, سرا, real_سرا;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	إذا (ext2fs_has_feature_journal_dev(fs->super))
		أرجع EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	fs->write_bitmaps = ext2fs_write_bitmaps;

	شروع = 1;
	سرا = fs->super->s_inodes_گنتی;
	real_سرا = (__u64)EXT2_INODES_فی_گروہ(fs->super) *
		fs->گروہ_desc_گنتی;

	/* Are we permitted to use new-style bitmaps? */
	إذا (fs->flags & EXT2_FLAG_64_بٹ)
		أرجع (ext2fs_alloc_generic_bmap(fs,
				EXT2_ET_MAGIC_INODE_BITMAP64,
				fs->default_bitmap_type,
				شروع, سرا, real_سرا, descr, ret));

	/* Otherwise, check to see if the file system is small enough
	 * to use old-style 32-bit bitmaps */
	إذا ((سرا > ~0U) || (real_سرا > ~0U))
		أرجع EXT2_ET_CANT_USE_LEGACY_BITMAPS;

	أرجع (ext2fs_make_generic_bitmap(EXT2_ET_MAGIC_INODE_BITMAP, fs,
					 شروع, سرا, real_سرا,
					 descr, 0,
					 (ext2fs_generic_bitmap *) ret));
}

errcode_t ext2fs_allocate_بلاک_bitmap(ext2_filsys fs,
				       ثابت حرف *descr,
				       ext2fs_بلاک_bitmap *ret)
{
	__u64		شروع, سرا, real_سرا;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	إذا (ext2fs_has_feature_journal_dev(fs->super))
		أرجع EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	fs->write_bitmaps = ext2fs_write_bitmaps;

	شروع = EXT2FS_B2C(fs, fs->super->s_پہلا_انکڑے_بلاک);
	سرا = EXT2FS_B2C(fs, ext2fs_بلاکاں_گنتی(fs->super)-1);
	real_سرا = ((__u64) EXT2_CLUSTERS_فی_گروہ(fs->super)
		    * (__u64) fs->گروہ_desc_گنتی)-1 + شروع;

	إذا (fs->flags & EXT2_FLAG_64_بٹ)
		أرجع (ext2fs_alloc_generic_bmap(fs,
				EXT2_ET_MAGIC_بلاک_BITMAP64,
				fs->default_bitmap_type,
				شروع, سرا, real_سرا, descr, ret));

	إذا ((سرا > ~0U) || (real_سرا > ~0U))
		أرجع EXT2_ET_CANT_USE_LEGACY_BITMAPS;

	أرجع (ext2fs_make_generic_bitmap(EXT2_ET_MAGIC_بلاک_BITMAP, fs,
					   شروع, سرا, real_سرا,
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
	__u64			شروع, سرا, real_سرا;
	ext2fs_generic_bitmap	bmap;
	ext2fs_generic_bitmap_64 bmap64;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	إذا (ext2fs_has_feature_journal_dev(fs->super))
		أرجع EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	fs->write_bitmaps = ext2fs_write_bitmaps;

	إذا (!fs->cluster_تناسب_بٹ)
		أرجع ext2fs_allocate_بلاک_bitmap(fs, descr, ret);

	إذا ((fs->flags & EXT2_FLAG_64_بٹ) == 0)
		أرجع EXT2_ET_CANT_USE_LEGACY_BITMAPS;

	شروع = fs->super->s_پہلا_انکڑے_بلاک;
	سرا = ext2fs_بلاکاں_گنتی(fs->super)-1;
	real_سرا = ((__u64) EXT2_بلاکاں_فی_گروہ(fs->super)
		    * (__u64) fs->گروہ_desc_گنتی)-1 + شروع;

	retval = ext2fs_alloc_generic_bmap(fs, EXT2_ET_MAGIC_بلاک_BITMAP64,
					   fs->default_bitmap_type, شروع,
					   سرا, real_سرا, descr, &bmap);
	إذا (retval)
		أرجع retval;
	bmap64 = (ext2fs_generic_bitmap_64) bmap;
	bmap64->cluster_بٹ = 0;
	*ret = bmap;
	أرجع 0;
}

صحيح ext2fs_get_bitmap_granularity(ext2fs_بلاک_bitmap bitmap)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) bitmap;

	إذا (!EXT2FS_IS_64_BITMAP(bmap))
		أرجع 0;

	أرجع bmap->cluster_بٹ;
}

errcode_t ext2fs_fudge_inode_bitmap_سرا(ext2fs_inode_bitmap bitmap,
					ext2_ino_t سرا, ext2_ino_t *oend)
{
	__u64 tmp_oend;
	صحيح retval;

	retval = ext2fs_fudge_generic_bmap_سرا((ext2fs_generic_bitmap) bitmap,
					       EXT2_ET_FUDGE_INODE_BITMAP_سرا,
					       سرا, &tmp_oend);
	إذا (oend)
		*oend = tmp_oend;
	أرجع retval;
}

errcode_t ext2fs_fudge_بلاک_bitmap_سرا(ext2fs_بلاک_bitmap bitmap,
					blk_t سرا, blk_t *oend)
{
	أرجع (ext2fs_fudge_generic_bitmap_سرا(bitmap,
						EXT2_ET_MAGIC_بلاک_BITMAP,
						EXT2_ET_FUDGE_بلاک_BITMAP_سرا,
						سرا, oend));
}

errcode_t ext2fs_fudge_بلاک_bitmap_end2(ext2fs_بلاک_bitmap bitmap,
					 blk64_t سرا, blk64_t *oend)
{
	أرجع (ext2fs_fudge_generic_bmap_سرا(bitmap,
					      EXT2_ET_FUDGE_بلاک_BITMAP_سرا,
					      سرا, oend));
}

فراغ ext2fs_clear_inode_bitmap(ext2fs_inode_bitmap bitmap)
{
	ext2fs_clear_generic_bmap(bitmap);
}

فراغ ext2fs_clear_بلاک_bitmap(ext2fs_بلاک_bitmap bitmap)
{
	ext2fs_clear_generic_bmap(bitmap);
}

errcode_t ext2fs_resize_inode_bitmap(__u32 new_سرا, __u32 new_real_سرا,
				     ext2fs_inode_bitmap bmap)
{
	أرجع (ext2fs_resize_generic_bitmap(EXT2_ET_MAGIC_INODE_BITMAP,
					     new_سرا, new_real_سرا, bmap));
}

errcode_t ext2fs_resize_inode_bitmap2(__u64 new_سرا, __u64 new_real_سرا,
				      ext2fs_inode_bitmap bmap)
{
	أرجع (ext2fs_resize_generic_bmap(bmap, new_سرا, new_real_سرا));
}

errcode_t ext2fs_resize_بلاک_bitmap(__u32 new_سرا, __u32 new_real_سرا,
				     ext2fs_بلاک_bitmap bmap)
{
	أرجع (ext2fs_resize_generic_bitmap(EXT2_ET_MAGIC_بلاک_BITMAP,
					     new_سرا, new_real_سرا, bmap));
}

errcode_t ext2fs_resize_بلاک_bitmap2(__u64 new_سرا, __u64 new_real_سرا,
				      ext2fs_بلاک_bitmap bmap)
{
	أرجع (ext2fs_resize_generic_bmap(bmap, new_سرا, new_real_سرا));
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

errcode_t ext2fs_سیٹ_inode_bitmap_range(ext2fs_inode_bitmap bmap,
					ext2_ino_t شروع, بنانشانی صحيح عدد,
					فراغ *وچ)
{
	أرجع (ext2fs_سیٹ_generic_bitmap_range(bmap,
						EXT2_ET_MAGIC_INODE_BITMAP,
						شروع, عدد, وچ));
}

errcode_t ext2fs_سیٹ_inode_bitmap_range2(ext2fs_inode_bitmap bmap,
					 __u64 شروع, size_t عدد,
					 فراغ *وچ)
{
	أرجع (ext2fs_سیٹ_generic_bmap_range(bmap, شروع, عدد, وچ));
}

errcode_t ext2fs_get_inode_bitmap_range(ext2fs_inode_bitmap bmap,
					ext2_ino_t شروع, بنانشانی صحيح عدد,
					فراغ *باہرلا)
{
	أرجع (ext2fs_get_generic_bitmap_range(bmap,
						EXT2_ET_MAGIC_INODE_BITMAP,
						شروع, عدد, باہرلا));
}

errcode_t ext2fs_get_inode_bitmap_range2(ext2fs_inode_bitmap bmap,
					 __u64 شروع, size_t عدد,
					 فراغ *باہرلا)
{
	أرجع (ext2fs_get_generic_bmap_range(bmap, شروع, عدد, باہرلا));
}

errcode_t ext2fs_سیٹ_بلاک_bitmap_range(ext2fs_بلاک_bitmap bmap,
					blk_t شروع, بنانشانی صحيح عدد,
					فراغ *وچ)
{
	أرجع (ext2fs_سیٹ_generic_bitmap_range(bmap,
						EXT2_ET_MAGIC_بلاک_BITMAP,
						شروع, عدد, وچ));
}

errcode_t ext2fs_سیٹ_بلاک_bitmap_range2(ext2fs_بلاک_bitmap bmap,
					 blk64_t شروع, size_t عدد,
					 فراغ *وچ)
{
	أرجع (ext2fs_سیٹ_generic_bmap_range(bmap, شروع, عدد, وچ));
}

errcode_t ext2fs_get_بلاک_bitmap_range(ext2fs_بلاک_bitmap bmap,
					blk_t شروع, بنانشانی صحيح عدد,
					فراغ *باہرلا)
{
	أرجع (ext2fs_get_generic_bitmap_range(bmap,
						EXT2_ET_MAGIC_بلاک_BITMAP,
						شروع, عدد, باہرلا));
}

errcode_t ext2fs_get_بلاک_bitmap_range2(ext2fs_بلاک_bitmap bmap,
					 blk64_t شروع, size_t عدد,
					 فراغ *باہرلا)
{
	أرجع (ext2fs_get_generic_bmap_range(bmap, شروع, عدد, باہرلا));
}
