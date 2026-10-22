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

#شامل "config.h"
#شامل <stdio.h>
#شامل <string.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#اگر_ختم
#شامل <fcntl.h>
#شامل <time.h>
#اگر HAVE_SYS_STAT_H
#شامل <sys/stat.h>
#اگر_ختم
#اگر HAVE_SYS_TYPES_H
#شامل <sys/types.h>
#اگر_ختم

#شامل "ext2_fs.h"
#شامل "ext2fs.h"
#شامل "ext2fsP.h"
#شامل "bmap64.h"

خالی ext2fs_free_inode_bitmap(ext2fs_inode_bitmap bitmap)
{
	ext2fs_free_generic_bmap(bitmap);
}

خالی ext2fs_free_بلاک_bitmap(ext2fs_بلاک_bitmap bitmap)
{
	ext2fs_free_generic_bmap(bitmap);
}

errcode_t ext2fs_copy_bitmap(ext2fs_generic_bitmap src,
			     ext2fs_generic_bitmap *dest)
{
	واپس (ext2fs_copy_generic_bmap(src, dest));
}
خالی ext2fs_سیٹ_bitmap_padding(ext2fs_generic_bitmap map)
{
	ext2fs_سیٹ_generic_bmap_padding(map);
}

errcode_t ext2fs_allocate_inode_bitmap(ext2_filsys fs,
				       ثابت حرف *descr,
				       ext2fs_inode_bitmap *ret)
{
	__u64		آغاز, انتہا, real_انتہا;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	اگر (ext2fs_has_feature_journal_dev(fs->super))
		واپس EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	fs->write_bitmaps = ext2fs_write_bitmaps;

	آغاز = 1;
	انتہا = fs->super->s_inodes_گنتی;
	real_انتہا = (__u64)EXT2_INODES_فی_گروہ(fs->super) *
		fs->گروہ_desc_گنتی;

	/* Are we permitted to use new-style bitmaps? */
	اگر (fs->flags & EXT2_FLAG_64_بٹ)
		واپس (ext2fs_alloc_generic_bmap(fs,
				EXT2_ET_MAGIC_INODE_BITMAP64,
				fs->default_bitmap_type,
				آغاز, انتہا, real_انتہا, descr, ret));

	/* Otherwise, check to see if the file system is small enough
	 * to use old-style 32-bit bitmaps */
	اگر ((انتہا > ~0U) || (real_انتہا > ~0U))
		واپس EXT2_ET_CANT_USE_LEGACY_BITMAPS;

	واپس (ext2fs_make_generic_bitmap(EXT2_ET_MAGIC_INODE_BITMAP, fs,
					 آغاز, انتہا, real_انتہا,
					 descr, 0,
					 (ext2fs_generic_bitmap *) ret));
}

errcode_t ext2fs_allocate_بلاک_bitmap(ext2_filsys fs,
				       ثابت حرف *descr,
				       ext2fs_بلاک_bitmap *ret)
{
	__u64		آغاز, انتہا, real_انتہا;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	اگر (ext2fs_has_feature_journal_dev(fs->super))
		واپس EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	fs->write_bitmaps = ext2fs_write_bitmaps;

	آغاز = EXT2FS_B2C(fs, fs->super->s_پہلا_ڈیٹا_بلاک);
	انتہا = EXT2FS_B2C(fs, ext2fs_بلاکیں_گنتی(fs->super)-1);
	real_انتہا = ((__u64) EXT2_CLUSTERS_فی_گروہ(fs->super)
		    * (__u64) fs->گروہ_desc_گنتی)-1 + آغاز;

	اگر (fs->flags & EXT2_FLAG_64_بٹ)
		واپس (ext2fs_alloc_generic_bmap(fs,
				EXT2_ET_MAGIC_بلاک_BITMAP64,
				fs->default_bitmap_type,
				آغاز, انتہا, real_انتہا, descr, ret));

	اگر ((انتہا > ~0U) || (real_انتہا > ~0U))
		واپس EXT2_ET_CANT_USE_LEGACY_BITMAPS;

	واپس (ext2fs_make_generic_bitmap(EXT2_ET_MAGIC_بلاک_BITMAP, fs,
					   آغاز, انتہا, real_انتہا,
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
	__u64			آغاز, انتہا, real_انتہا;
	ext2fs_generic_bitmap	bmap;
	ext2fs_generic_bitmap_64 bmap64;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	اگر (ext2fs_has_feature_journal_dev(fs->super))
		واپس EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	fs->write_bitmaps = ext2fs_write_bitmaps;

	اگر (!fs->cluster_تناسب_بٹ)
		واپس ext2fs_allocate_بلاک_bitmap(fs, descr, ret);

	اگر ((fs->flags & EXT2_FLAG_64_بٹ) == 0)
		واپس EXT2_ET_CANT_USE_LEGACY_BITMAPS;

	آغاز = fs->super->s_پہلا_ڈیٹا_بلاک;
	انتہا = ext2fs_بلاکیں_گنتی(fs->super)-1;
	real_انتہا = ((__u64) EXT2_بلاکیں_فی_گروہ(fs->super)
		    * (__u64) fs->گروہ_desc_گنتی)-1 + آغاز;

	retval = ext2fs_alloc_generic_bmap(fs, EXT2_ET_MAGIC_بلاک_BITMAP64,
					   fs->default_bitmap_type, آغاز,
					   انتہا, real_انتہا, descr, &bmap);
	اگر (retval)
		واپس retval;
	bmap64 = (ext2fs_generic_bitmap_64) bmap;
	bmap64->cluster_بٹ = 0;
	*ret = bmap;
	واپس 0;
}

صحیح ext2fs_get_bitmap_granularity(ext2fs_بلاک_bitmap bitmap)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) bitmap;

	اگر (!EXT2FS_IS_64_BITMAP(bmap))
		واپس 0;

	واپس bmap->cluster_بٹ;
}

errcode_t ext2fs_fudge_inode_bitmap_انتہا(ext2fs_inode_bitmap bitmap,
					ext2_ino_t انتہا, ext2_ino_t *oend)
{
	__u64 tmp_oend;
	صحیح retval;

	retval = ext2fs_fudge_generic_bmap_انتہا((ext2fs_generic_bitmap) bitmap,
					       EXT2_ET_FUDGE_INODE_BITMAP_انتہا,
					       انتہا, &tmp_oend);
	اگر (oend)
		*oend = tmp_oend;
	واپس retval;
}

errcode_t ext2fs_fudge_بلاک_bitmap_انتہا(ext2fs_بلاک_bitmap bitmap,
					blk_t انتہا, blk_t *oend)
{
	واپس (ext2fs_fudge_generic_bitmap_انتہا(bitmap,
						EXT2_ET_MAGIC_بلاک_BITMAP,
						EXT2_ET_FUDGE_بلاک_BITMAP_انتہا,
						انتہا, oend));
}

errcode_t ext2fs_fudge_بلاک_bitmap_end2(ext2fs_بلاک_bitmap bitmap,
					 blk64_t انتہا, blk64_t *oend)
{
	واپس (ext2fs_fudge_generic_bmap_انتہا(bitmap,
					      EXT2_ET_FUDGE_بلاک_BITMAP_انتہا,
					      انتہا, oend));
}

خالی ext2fs_clear_inode_bitmap(ext2fs_inode_bitmap bitmap)
{
	ext2fs_clear_generic_bmap(bitmap);
}

خالی ext2fs_clear_بلاک_bitmap(ext2fs_بلاک_bitmap bitmap)
{
	ext2fs_clear_generic_bmap(bitmap);
}

errcode_t ext2fs_resize_inode_bitmap(__u32 new_انتہا, __u32 new_real_انتہا,
				     ext2fs_inode_bitmap bmap)
{
	واپس (ext2fs_resize_generic_bitmap(EXT2_ET_MAGIC_INODE_BITMAP,
					     new_انتہا, new_real_انتہا, bmap));
}

errcode_t ext2fs_resize_inode_bitmap2(__u64 new_انتہا, __u64 new_real_انتہا,
				      ext2fs_inode_bitmap bmap)
{
	واپس (ext2fs_resize_generic_bmap(bmap, new_انتہا, new_real_انتہا));
}

errcode_t ext2fs_resize_بلاک_bitmap(__u32 new_انتہا, __u32 new_real_انتہا,
				     ext2fs_بلاک_bitmap bmap)
{
	واپس (ext2fs_resize_generic_bitmap(EXT2_ET_MAGIC_بلاک_BITMAP,
					     new_انتہا, new_real_انتہا, bmap));
}

errcode_t ext2fs_resize_بلاک_bitmap2(__u64 new_انتہا, __u64 new_real_انتہا,
				      ext2fs_بلاک_bitmap bmap)
{
	واپس (ext2fs_resize_generic_bmap(bmap, new_انتہا, new_real_انتہا));
}

errcode_t ext2fs_compare_بلاک_bitmap(ext2fs_بلاک_bitmap bm1,
				      ext2fs_بلاک_bitmap bm2)
{
	واپس (ext2fs_compare_generic_bmap(EXT2_ET_NEQ_بلاک_BITMAP,
					    bm1, bm2));
}

errcode_t ext2fs_compare_inode_bitmap(ext2fs_inode_bitmap bm1,
				      ext2fs_inode_bitmap bm2)
{
	واپس (ext2fs_compare_generic_bmap(EXT2_ET_NEQ_INODE_BITMAP,
					    bm1, bm2));
}

errcode_t ext2fs_سیٹ_inode_bitmap_range(ext2fs_inode_bitmap bmap,
					ext2_ino_t آغاز, غیرعلامتی صحیح عدد,
					خالی *اندر)
{
	واپس (ext2fs_سیٹ_generic_bitmap_range(bmap,
						EXT2_ET_MAGIC_INODE_BITMAP,
						آغاز, عدد, اندر));
}

errcode_t ext2fs_سیٹ_inode_bitmap_range2(ext2fs_inode_bitmap bmap,
					 __u64 آغاز, size_t عدد,
					 خالی *اندر)
{
	واپس (ext2fs_سیٹ_generic_bmap_range(bmap, آغاز, عدد, اندر));
}

errcode_t ext2fs_get_inode_bitmap_range(ext2fs_inode_bitmap bmap,
					ext2_ino_t آغاز, غیرعلامتی صحیح عدد,
					خالی *برآمد)
{
	واپس (ext2fs_get_generic_bitmap_range(bmap,
						EXT2_ET_MAGIC_INODE_BITMAP,
						آغاز, عدد, برآمد));
}

errcode_t ext2fs_get_inode_bitmap_range2(ext2fs_inode_bitmap bmap,
					 __u64 آغاز, size_t عدد,
					 خالی *برآمد)
{
	واپس (ext2fs_get_generic_bmap_range(bmap, آغاز, عدد, برآمد));
}

errcode_t ext2fs_سیٹ_بلاک_bitmap_range(ext2fs_بلاک_bitmap bmap,
					blk_t آغاز, غیرعلامتی صحیح عدد,
					خالی *اندر)
{
	واپس (ext2fs_سیٹ_generic_bitmap_range(bmap,
						EXT2_ET_MAGIC_بلاک_BITMAP,
						آغاز, عدد, اندر));
}

errcode_t ext2fs_سیٹ_بلاک_bitmap_range2(ext2fs_بلاک_bitmap bmap,
					 blk64_t آغاز, size_t عدد,
					 خالی *اندر)
{
	واپس (ext2fs_سیٹ_generic_bmap_range(bmap, آغاز, عدد, اندر));
}

errcode_t ext2fs_get_بلاک_bitmap_range(ext2fs_بلاک_bitmap bmap,
					blk_t آغاز, غیرعلامتی صحیح عدد,
					خالی *برآمد)
{
	واپس (ext2fs_get_generic_bitmap_range(bmap,
						EXT2_ET_MAGIC_بلاک_BITMAP,
						آغاز, عدد, برآمد));
}

errcode_t ext2fs_get_بلاک_bitmap_range2(ext2fs_بلاک_bitmap bmap,
					 blk64_t آغاز, size_t عدد,
					 خالی *برآمد)
{
	واپس (ext2fs_get_generic_bmap_range(bmap, آغاز, عدد, برآمد));
}
