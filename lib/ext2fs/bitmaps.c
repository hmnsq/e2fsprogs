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
#جيڪڏهن_پورو
#ضمن <fcntl.h>
#ضمن <time.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#جيڪڏهن_پورو
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#جيڪڏهن_پورو

#ضمن "ext2_fs.h"
#ضمن "ext2fs.h"
#ضمن "ext2fsP.h"
#ضمن "bmap64.h"

فراغ ext2fs_free_inode_bitmap(ext2fs_inode_bitmap bitmap)
{
	ext2fs_free_generic_bmap(bitmap);
}

فراغ ext2fs_free_بلاڪ_bitmap(ext2fs_بلاڪ_bitmap bitmap)
{
	ext2fs_free_generic_bmap(bitmap);
}

errcode_t ext2fs_copy_bitmap(ext2fs_generic_bitmap src,
			     ext2fs_generic_bitmap *dest)
{
	أرجع (ext2fs_copy_generic_bmap(src, dest));
}
فراغ ext2fs_سيٽ_bitmap_padding(ext2fs_generic_bitmap map)
{
	ext2fs_سيٽ_generic_bmap_padding(map);
}

errcode_t ext2fs_allocate_inode_bitmap(ext2_filsys fs,
				       ثابت حرف *descr,
				       ext2fs_inode_bitmap *ret)
{
	__u64		شروعات, پڇاڙي, real_پڇاڙي;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	إذا (ext2fs_has_feature_journal_dev(fs->super))
		أرجع EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	fs->write_bitmaps = ext2fs_write_bitmaps;

	شروعات = 1;
	پڇاڙي = fs->super->s_inodes_ڳڻپ;
	real_پڇاڙي = (__u64)EXT2_INODES_في_گروهه(fs->super) *
		fs->گروهه_desc_ڳڻپ;

	/* Are we permitted to use new-style bitmaps? */
	إذا (fs->flags & EXT2_FLAG_64_بٽ)
		أرجع (ext2fs_alloc_generic_bmap(fs,
				EXT2_ET_MAGIC_INODE_BITMAP64,
				fs->default_bitmap_type,
				شروعات, پڇاڙي, real_پڇاڙي, descr, ret));

	/* Otherwise, check to see if the file system is small enough
	 * to use old-style 32-bit bitmaps */
	إذا ((پڇاڙي > ~0U) || (real_پڇاڙي > ~0U))
		أرجع EXT2_ET_CANT_USE_LEGACY_BITMAPS;

	أرجع (ext2fs_make_generic_bitmap(EXT2_ET_MAGIC_INODE_BITMAP, fs,
					 شروعات, پڇاڙي, real_پڇاڙي,
					 descr, 0,
					 (ext2fs_generic_bitmap *) ret));
}

errcode_t ext2fs_allocate_بلاڪ_bitmap(ext2_filsys fs,
				       ثابت حرف *descr,
				       ext2fs_بلاڪ_bitmap *ret)
{
	__u64		شروعات, پڇاڙي, real_پڇاڙي;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	إذا (ext2fs_has_feature_journal_dev(fs->super))
		أرجع EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	fs->write_bitmaps = ext2fs_write_bitmaps;

	شروعات = EXT2FS_B2C(fs, fs->super->s_پهريون_معلومات_بلاڪ);
	پڇاڙي = EXT2FS_B2C(fs, ext2fs_بلاڪون_ڳڻپ(fs->super)-1);
	real_پڇاڙي = ((__u64) EXT2_CLUSTERS_في_گروهه(fs->super)
		    * (__u64) fs->گروهه_desc_ڳڻپ)-1 + شروعات;

	إذا (fs->flags & EXT2_FLAG_64_بٽ)
		أرجع (ext2fs_alloc_generic_bmap(fs,
				EXT2_ET_MAGIC_بلاڪ_BITMAP64,
				fs->default_bitmap_type,
				شروعات, پڇاڙي, real_پڇاڙي, descr, ret));

	إذا ((پڇاڙي > ~0U) || (real_پڇاڙي > ~0U))
		أرجع EXT2_ET_CANT_USE_LEGACY_BITMAPS;

	أرجع (ext2fs_make_generic_bitmap(EXT2_ET_MAGIC_بلاڪ_BITMAP, fs,
					   شروعات, پڇاڙي, real_پڇاڙي,
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
					    ext2fs_بلاڪ_bitmap *ret)
{
	__u64			شروعات, پڇاڙي, real_پڇاڙي;
	ext2fs_generic_bitmap	bmap;
	ext2fs_generic_bitmap_64 bmap64;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	إذا (ext2fs_has_feature_journal_dev(fs->super))
		أرجع EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	fs->write_bitmaps = ext2fs_write_bitmaps;

	إذا (!fs->cluster_تناسب_بٽ)
		أرجع ext2fs_allocate_بلاڪ_bitmap(fs, descr, ret);

	إذا ((fs->flags & EXT2_FLAG_64_بٽ) == 0)
		أرجع EXT2_ET_CANT_USE_LEGACY_BITMAPS;

	شروعات = fs->super->s_پهريون_معلومات_بلاڪ;
	پڇاڙي = ext2fs_بلاڪون_ڳڻپ(fs->super)-1;
	real_پڇاڙي = ((__u64) EXT2_بلاڪون_في_گروهه(fs->super)
		    * (__u64) fs->گروهه_desc_ڳڻپ)-1 + شروعات;

	retval = ext2fs_alloc_generic_bmap(fs, EXT2_ET_MAGIC_بلاڪ_BITMAP64,
					   fs->default_bitmap_type, شروعات,
					   پڇاڙي, real_پڇاڙي, descr, &bmap);
	إذا (retval)
		أرجع retval;
	bmap64 = (ext2fs_generic_bitmap_64) bmap;
	bmap64->cluster_بٽ = 0;
	*ret = bmap;
	أرجع 0;
}

صحيح ext2fs_get_bitmap_granularity(ext2fs_بلاڪ_bitmap bitmap)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) bitmap;

	إذا (!EXT2FS_IS_64_BITMAP(bmap))
		أرجع 0;

	أرجع bmap->cluster_بٽ;
}

errcode_t ext2fs_fudge_inode_bitmap_پڇاڙي(ext2fs_inode_bitmap bitmap,
					ext2_ino_t پڇاڙي, ext2_ino_t *oend)
{
	__u64 tmp_oend;
	صحيح retval;

	retval = ext2fs_fudge_generic_bmap_پڇاڙي((ext2fs_generic_bitmap) bitmap,
					       EXT2_ET_FUDGE_INODE_BITMAP_پڇاڙي,
					       پڇاڙي, &tmp_oend);
	إذا (oend)
		*oend = tmp_oend;
	أرجع retval;
}

errcode_t ext2fs_fudge_بلاڪ_bitmap_پڇاڙي(ext2fs_بلاڪ_bitmap bitmap,
					blk_t پڇاڙي, blk_t *oend)
{
	أرجع (ext2fs_fudge_generic_bitmap_پڇاڙي(bitmap,
						EXT2_ET_MAGIC_بلاڪ_BITMAP,
						EXT2_ET_FUDGE_بلاڪ_BITMAP_پڇاڙي,
						پڇاڙي, oend));
}

errcode_t ext2fs_fudge_بلاڪ_bitmap_end2(ext2fs_بلاڪ_bitmap bitmap,
					 blk64_t پڇاڙي, blk64_t *oend)
{
	أرجع (ext2fs_fudge_generic_bmap_پڇاڙي(bitmap,
					      EXT2_ET_FUDGE_بلاڪ_BITMAP_پڇاڙي,
					      پڇاڙي, oend));
}

فراغ ext2fs_clear_inode_bitmap(ext2fs_inode_bitmap bitmap)
{
	ext2fs_clear_generic_bmap(bitmap);
}

فراغ ext2fs_clear_بلاڪ_bitmap(ext2fs_بلاڪ_bitmap bitmap)
{
	ext2fs_clear_generic_bmap(bitmap);
}

errcode_t ext2fs_resize_inode_bitmap(__u32 new_پڇاڙي, __u32 new_real_پڇاڙي,
				     ext2fs_inode_bitmap bmap)
{
	أرجع (ext2fs_resize_generic_bitmap(EXT2_ET_MAGIC_INODE_BITMAP,
					     new_پڇاڙي, new_real_پڇاڙي, bmap));
}

errcode_t ext2fs_resize_inode_bitmap2(__u64 new_پڇاڙي, __u64 new_real_پڇاڙي,
				      ext2fs_inode_bitmap bmap)
{
	أرجع (ext2fs_resize_generic_bmap(bmap, new_پڇاڙي, new_real_پڇاڙي));
}

errcode_t ext2fs_resize_بلاڪ_bitmap(__u32 new_پڇاڙي, __u32 new_real_پڇاڙي,
				     ext2fs_بلاڪ_bitmap bmap)
{
	أرجع (ext2fs_resize_generic_bitmap(EXT2_ET_MAGIC_بلاڪ_BITMAP,
					     new_پڇاڙي, new_real_پڇاڙي, bmap));
}

errcode_t ext2fs_resize_بلاڪ_bitmap2(__u64 new_پڇاڙي, __u64 new_real_پڇاڙي,
				      ext2fs_بلاڪ_bitmap bmap)
{
	أرجع (ext2fs_resize_generic_bmap(bmap, new_پڇاڙي, new_real_پڇاڙي));
}

errcode_t ext2fs_compare_بلاڪ_bitmap(ext2fs_بلاڪ_bitmap bm1,
				      ext2fs_بلاڪ_bitmap bm2)
{
	أرجع (ext2fs_compare_generic_bmap(EXT2_ET_NEQ_بلاڪ_BITMAP,
					    bm1, bm2));
}

errcode_t ext2fs_compare_inode_bitmap(ext2fs_inode_bitmap bm1,
				      ext2fs_inode_bitmap bm2)
{
	أرجع (ext2fs_compare_generic_bmap(EXT2_ET_NEQ_INODE_BITMAP,
					    bm1, bm2));
}

errcode_t ext2fs_سيٽ_inode_bitmap_range(ext2fs_inode_bitmap bmap,
					ext2_ino_t شروعات, بنانشان صحيح انگ,
					فراغ *اندريون)
{
	أرجع (ext2fs_سيٽ_generic_bitmap_range(bmap,
						EXT2_ET_MAGIC_INODE_BITMAP,
						شروعات, انگ, اندريون));
}

errcode_t ext2fs_سيٽ_inode_bitmap_range2(ext2fs_inode_bitmap bmap,
					 __u64 شروعات, size_t انگ,
					 فراغ *اندريون)
{
	أرجع (ext2fs_سيٽ_generic_bmap_range(bmap, شروعات, انگ, اندريون));
}

errcode_t ext2fs_get_inode_bitmap_range(ext2fs_inode_bitmap bmap,
					ext2_ino_t شروعات, بنانشان صحيح انگ,
					فراغ *ٻاهريون)
{
	أرجع (ext2fs_get_generic_bitmap_range(bmap,
						EXT2_ET_MAGIC_INODE_BITMAP,
						شروعات, انگ, ٻاهريون));
}

errcode_t ext2fs_get_inode_bitmap_range2(ext2fs_inode_bitmap bmap,
					 __u64 شروعات, size_t انگ,
					 فراغ *ٻاهريون)
{
	أرجع (ext2fs_get_generic_bmap_range(bmap, شروعات, انگ, ٻاهريون));
}

errcode_t ext2fs_سيٽ_بلاڪ_bitmap_range(ext2fs_بلاڪ_bitmap bmap,
					blk_t شروعات, بنانشان صحيح انگ,
					فراغ *اندريون)
{
	أرجع (ext2fs_سيٽ_generic_bitmap_range(bmap,
						EXT2_ET_MAGIC_بلاڪ_BITMAP,
						شروعات, انگ, اندريون));
}

errcode_t ext2fs_سيٽ_بلاڪ_bitmap_range2(ext2fs_بلاڪ_bitmap bmap,
					 blk64_t شروعات, size_t انگ,
					 فراغ *اندريون)
{
	أرجع (ext2fs_سيٽ_generic_bmap_range(bmap, شروعات, انگ, اندريون));
}

errcode_t ext2fs_get_بلاڪ_bitmap_range(ext2fs_بلاڪ_bitmap bmap,
					blk_t شروعات, بنانشان صحيح انگ,
					فراغ *ٻاهريون)
{
	أرجع (ext2fs_get_generic_bitmap_range(bmap,
						EXT2_ET_MAGIC_بلاڪ_BITMAP,
						شروعات, انگ, ٻاهريون));
}

errcode_t ext2fs_get_بلاڪ_bitmap_range2(ext2fs_بلاڪ_bitmap bmap,
					 blk64_t شروعات, size_t انگ,
					 فراغ *ٻاهريون)
{
	أرجع (ext2fs_get_generic_bmap_range(bmap, شروعات, انگ, ٻاهريون));
}
