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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਜੇ HAVE_UNISTD_H
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <fcntl.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <time.h>
#ਜੇ HAVE_SYS_STAT_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_SYS_TYPES_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fsP.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "bmap64.h"

ਖਾਲੀ ext2fs_free_inode_bitmap(ext2fs_inode_bitmap bitmap)
{
	ext2fs_free_generic_bmap(bitmap);
}

ਖਾਲੀ ext2fs_free_ਖੰਡ_bitmap(ext2fs_ਖੰਡ_bitmap bitmap)
{
	ext2fs_free_generic_bmap(bitmap);
}

errcode_t ext2fs_copy_bitmap(ext2fs_generic_bitmap src,
			     ext2fs_generic_bitmap *dest)
{
	ਵਾਪਸ (ext2fs_copy_generic_bmap(src, dest));
}
ਖਾਲੀ ext2fs_ਸੈੱਟ_bitmap_padding(ext2fs_generic_bitmap map)
{
	ext2fs_ਸੈੱਟ_generic_bmap_padding(map);
}

errcode_t ext2fs_allocate_inode_bitmap(ext2_filsys fs,
				       ਅਚਲ ਅੱਖਰ *descr,
				       ext2fs_inode_bitmap *ret)
{
	__u64		ਸ਼ੁਰੂ, ਸਿਰਾ, real_ਸਿਰਾ;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ਜੇ (ext2fs_has_feature_journal_dev(fs->super))
		ਵਾਪਸ EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	fs->write_bitmaps = ext2fs_write_bitmaps;

	ਸ਼ੁਰੂ = 1;
	ਸਿਰਾ = fs->super->s_inodes_ਗਿਣਤੀ;
	real_ਸਿਰਾ = (__u64)EXT2_INODES_ਪ੍ਰਤੀ_ਸਮੂਹ(fs->super) *
		fs->ਸਮੂਹ_desc_ਗਿਣਤੀ;

	/* Are we permitted to use new-style bitmaps? */
	ਜੇ (fs->flags & EXT2_FLAG_64_ਬਿੱਟ)
		ਵਾਪਸ (ext2fs_alloc_generic_bmap(fs,
				EXT2_ET_MAGIC_INODE_BITMAP64,
				fs->default_bitmap_type,
				ਸ਼ੁਰੂ, ਸਿਰਾ, real_ਸਿਰਾ, descr, ret));

	/* Otherwise, check to see if the file system is small enough
	 * to use old-style 32-bit bitmaps */
	ਜੇ ((ਸਿਰਾ > ~0U) || (real_ਸਿਰਾ > ~0U))
		ਵਾਪਸ EXT2_ET_CANT_USE_LEGACY_BITMAPS;

	ਵਾਪਸ (ext2fs_make_generic_bitmap(EXT2_ET_MAGIC_INODE_BITMAP, fs,
					 ਸ਼ੁਰੂ, ਸਿਰਾ, real_ਸਿਰਾ,
					 descr, 0,
					 (ext2fs_generic_bitmap *) ret));
}

errcode_t ext2fs_allocate_ਖੰਡ_bitmap(ext2_filsys fs,
				       ਅਚਲ ਅੱਖਰ *descr,
				       ext2fs_ਖੰਡ_bitmap *ret)
{
	__u64		ਸ਼ੁਰੂ, ਸਿਰਾ, real_ਸਿਰਾ;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ਜੇ (ext2fs_has_feature_journal_dev(fs->super))
		ਵਾਪਸ EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	fs->write_bitmaps = ext2fs_write_bitmaps;

	ਸ਼ੁਰੂ = EXT2FS_B2C(fs, fs->super->s_ਪਹਿਲਾ_ਅੰਕੜੇ_ਖੰਡ);
	ਸਿਰਾ = EXT2FS_B2C(fs, ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(fs->super)-1);
	real_ਸਿਰਾ = ((__u64) EXT2_CLUSTERS_ਪ੍ਰਤੀ_ਸਮੂਹ(fs->super)
		    * (__u64) fs->ਸਮੂਹ_desc_ਗਿਣਤੀ)-1 + ਸ਼ੁਰੂ;

	ਜੇ (fs->flags & EXT2_FLAG_64_ਬਿੱਟ)
		ਵਾਪਸ (ext2fs_alloc_generic_bmap(fs,
				EXT2_ET_MAGIC_ਖੰਡ_BITMAP64,
				fs->default_bitmap_type,
				ਸ਼ੁਰੂ, ਸਿਰਾ, real_ਸਿਰਾ, descr, ret));

	ਜੇ ((ਸਿਰਾ > ~0U) || (real_ਸਿਰਾ > ~0U))
		ਵਾਪਸ EXT2_ET_CANT_USE_LEGACY_BITMAPS;

	ਵਾਪਸ (ext2fs_make_generic_bitmap(EXT2_ET_MAGIC_ਖੰਡ_BITMAP, fs,
					   ਸ਼ੁਰੂ, ਸਿਰਾ, real_ਸਿਰਾ,
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
					    ਅਚਲ ਅੱਖਰ *descr,
					    ext2fs_ਖੰਡ_bitmap *ret)
{
	__u64			ਸ਼ੁਰੂ, ਸਿਰਾ, real_ਸਿਰਾ;
	ext2fs_generic_bitmap	bmap;
	ext2fs_generic_bitmap_64 bmap64;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ਜੇ (ext2fs_has_feature_journal_dev(fs->super))
		ਵਾਪਸ EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	fs->write_bitmaps = ext2fs_write_bitmaps;

	ਜੇ (!fs->cluster_ਅਨੁਪਾਤ_ਬਿੱਟ)
		ਵਾਪਸ ext2fs_allocate_ਖੰਡ_bitmap(fs, descr, ret);

	ਜੇ ((fs->flags & EXT2_FLAG_64_ਬਿੱਟ) == 0)
		ਵਾਪਸ EXT2_ET_CANT_USE_LEGACY_BITMAPS;

	ਸ਼ੁਰੂ = fs->super->s_ਪਹਿਲਾ_ਅੰਕੜੇ_ਖੰਡ;
	ਸਿਰਾ = ext2fs_ਖੰਡਾਂ_ਗਿਣਤੀ(fs->super)-1;
	real_ਸਿਰਾ = ((__u64) EXT2_ਖੰਡਾਂ_ਪ੍ਰਤੀ_ਸਮੂਹ(fs->super)
		    * (__u64) fs->ਸਮੂਹ_desc_ਗਿਣਤੀ)-1 + ਸ਼ੁਰੂ;

	retval = ext2fs_alloc_generic_bmap(fs, EXT2_ET_MAGIC_ਖੰਡ_BITMAP64,
					   fs->default_bitmap_type, ਸ਼ੁਰੂ,
					   ਸਿਰਾ, real_ਸਿਰਾ, descr, &bmap);
	ਜੇ (retval)
		ਵਾਪਸ retval;
	bmap64 = (ext2fs_generic_bitmap_64) bmap;
	bmap64->cluster_ਬਿੱਟ = 0;
	*ret = bmap;
	ਵਾਪਸ 0;
}

ਪੂਰਨ ext2fs_get_bitmap_granularity(ext2fs_ਖੰਡ_bitmap bitmap)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) bitmap;

	ਜੇ (!EXT2FS_IS_64_BITMAP(bmap))
		ਵਾਪਸ 0;

	ਵਾਪਸ bmap->cluster_ਬਿੱਟ;
}

errcode_t ext2fs_fudge_inode_bitmap_ਸਿਰਾ(ext2fs_inode_bitmap bitmap,
					ext2_ino_t ਸਿਰਾ, ext2_ino_t *oend)
{
	__u64 tmp_oend;
	ਪੂਰਨ retval;

	retval = ext2fs_fudge_generic_bmap_ਸਿਰਾ((ext2fs_generic_bitmap) bitmap,
					       EXT2_ET_FUDGE_INODE_BITMAP_ਸਿਰਾ,
					       ਸਿਰਾ, &tmp_oend);
	ਜੇ (oend)
		*oend = tmp_oend;
	ਵਾਪਸ retval;
}

errcode_t ext2fs_fudge_ਖੰਡ_bitmap_ਸਿਰਾ(ext2fs_ਖੰਡ_bitmap bitmap,
					blk_t ਸਿਰਾ, blk_t *oend)
{
	ਵਾਪਸ (ext2fs_fudge_generic_bitmap_ਸਿਰਾ(bitmap,
						EXT2_ET_MAGIC_ਖੰਡ_BITMAP,
						EXT2_ET_FUDGE_ਖੰਡ_BITMAP_ਸਿਰਾ,
						ਸਿਰਾ, oend));
}

errcode_t ext2fs_fudge_ਖੰਡ_bitmap_end2(ext2fs_ਖੰਡ_bitmap bitmap,
					 blk64_t ਸਿਰਾ, blk64_t *oend)
{
	ਵਾਪਸ (ext2fs_fudge_generic_bmap_ਸਿਰਾ(bitmap,
					      EXT2_ET_FUDGE_ਖੰਡ_BITMAP_ਸਿਰਾ,
					      ਸਿਰਾ, oend));
}

ਖਾਲੀ ext2fs_clear_inode_bitmap(ext2fs_inode_bitmap bitmap)
{
	ext2fs_clear_generic_bmap(bitmap);
}

ਖਾਲੀ ext2fs_clear_ਖੰਡ_bitmap(ext2fs_ਖੰਡ_bitmap bitmap)
{
	ext2fs_clear_generic_bmap(bitmap);
}

errcode_t ext2fs_resize_inode_bitmap(__u32 new_ਸਿਰਾ, __u32 new_real_ਸਿਰਾ,
				     ext2fs_inode_bitmap bmap)
{
	ਵਾਪਸ (ext2fs_resize_generic_bitmap(EXT2_ET_MAGIC_INODE_BITMAP,
					     new_ਸਿਰਾ, new_real_ਸਿਰਾ, bmap));
}

errcode_t ext2fs_resize_inode_bitmap2(__u64 new_ਸਿਰਾ, __u64 new_real_ਸਿਰਾ,
				      ext2fs_inode_bitmap bmap)
{
	ਵਾਪਸ (ext2fs_resize_generic_bmap(bmap, new_ਸਿਰਾ, new_real_ਸਿਰਾ));
}

errcode_t ext2fs_resize_ਖੰਡ_bitmap(__u32 new_ਸਿਰਾ, __u32 new_real_ਸਿਰਾ,
				     ext2fs_ਖੰਡ_bitmap bmap)
{
	ਵਾਪਸ (ext2fs_resize_generic_bitmap(EXT2_ET_MAGIC_ਖੰਡ_BITMAP,
					     new_ਸਿਰਾ, new_real_ਸਿਰਾ, bmap));
}

errcode_t ext2fs_resize_ਖੰਡ_bitmap2(__u64 new_ਸਿਰਾ, __u64 new_real_ਸਿਰਾ,
				      ext2fs_ਖੰਡ_bitmap bmap)
{
	ਵਾਪਸ (ext2fs_resize_generic_bmap(bmap, new_ਸਿਰਾ, new_real_ਸਿਰਾ));
}

errcode_t ext2fs_compare_ਖੰਡ_bitmap(ext2fs_ਖੰਡ_bitmap bm1,
				      ext2fs_ਖੰਡ_bitmap bm2)
{
	ਵਾਪਸ (ext2fs_compare_generic_bmap(EXT2_ET_NEQ_ਖੰਡ_BITMAP,
					    bm1, bm2));
}

errcode_t ext2fs_compare_inode_bitmap(ext2fs_inode_bitmap bm1,
				      ext2fs_inode_bitmap bm2)
{
	ਵਾਪਸ (ext2fs_compare_generic_bmap(EXT2_ET_NEQ_INODE_BITMAP,
					    bm1, bm2));
}

errcode_t ext2fs_ਸੈੱਟ_inode_bitmap_range(ext2fs_inode_bitmap bmap,
					ext2_ino_t ਸ਼ੁਰੂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ,
					ਖਾਲੀ *ਵਿੱਚ)
{
	ਵਾਪਸ (ext2fs_ਸੈੱਟ_generic_bitmap_range(bmap,
						EXT2_ET_MAGIC_INODE_BITMAP,
						ਸ਼ੁਰੂ, ਅੰਕ, ਵਿੱਚ));
}

errcode_t ext2fs_ਸੈੱਟ_inode_bitmap_range2(ext2fs_inode_bitmap bmap,
					 __u64 ਸ਼ੁਰੂ, size_t ਅੰਕ,
					 ਖਾਲੀ *ਵਿੱਚ)
{
	ਵਾਪਸ (ext2fs_ਸੈੱਟ_generic_bmap_range(bmap, ਸ਼ੁਰੂ, ਅੰਕ, ਵਿੱਚ));
}

errcode_t ext2fs_get_inode_bitmap_range(ext2fs_inode_bitmap bmap,
					ext2_ino_t ਸ਼ੁਰੂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ,
					ਖਾਲੀ *ਬਾਹਰਲਾ)
{
	ਵਾਪਸ (ext2fs_get_generic_bitmap_range(bmap,
						EXT2_ET_MAGIC_INODE_BITMAP,
						ਸ਼ੁਰੂ, ਅੰਕ, ਬਾਹਰਲਾ));
}

errcode_t ext2fs_get_inode_bitmap_range2(ext2fs_inode_bitmap bmap,
					 __u64 ਸ਼ੁਰੂ, size_t ਅੰਕ,
					 ਖਾਲੀ *ਬਾਹਰਲਾ)
{
	ਵਾਪਸ (ext2fs_get_generic_bmap_range(bmap, ਸ਼ੁਰੂ, ਅੰਕ, ਬਾਹਰਲਾ));
}

errcode_t ext2fs_ਸੈੱਟ_ਖੰਡ_bitmap_range(ext2fs_ਖੰਡ_bitmap bmap,
					blk_t ਸ਼ੁਰੂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ,
					ਖਾਲੀ *ਵਿੱਚ)
{
	ਵਾਪਸ (ext2fs_ਸੈੱਟ_generic_bitmap_range(bmap,
						EXT2_ET_MAGIC_ਖੰਡ_BITMAP,
						ਸ਼ੁਰੂ, ਅੰਕ, ਵਿੱਚ));
}

errcode_t ext2fs_ਸੈੱਟ_ਖੰਡ_bitmap_range2(ext2fs_ਖੰਡ_bitmap bmap,
					 blk64_t ਸ਼ੁਰੂ, size_t ਅੰਕ,
					 ਖਾਲੀ *ਵਿੱਚ)
{
	ਵਾਪਸ (ext2fs_ਸੈੱਟ_generic_bmap_range(bmap, ਸ਼ੁਰੂ, ਅੰਕ, ਵਿੱਚ));
}

errcode_t ext2fs_get_ਖੰਡ_bitmap_range(ext2fs_ਖੰਡ_bitmap bmap,
					blk_t ਸ਼ੁਰੂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ,
					ਖਾਲੀ *ਬਾਹਰਲਾ)
{
	ਵਾਪਸ (ext2fs_get_generic_bitmap_range(bmap,
						EXT2_ET_MAGIC_ਖੰਡ_BITMAP,
						ਸ਼ੁਰੂ, ਅੰਕ, ਬਾਹਰਲਾ));
}

errcode_t ext2fs_get_ਖੰਡ_bitmap_range2(ext2fs_ਖੰਡ_bitmap bmap,
					 blk64_t ਸ਼ੁਰੂ, size_t ਅੰਕ,
					 ਖਾਲੀ *ਬਾਹਰਲਾ)
{
	ਵਾਪਸ (ext2fs_get_generic_bmap_range(bmap, ਸ਼ੁਰੂ, ਅੰਕ, ਬਾਹਰਲਾ));
}
