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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಆದರೆ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <time.h>
#ಆದರೆ HAVE_SYS_STAT_H
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_SYS_TYPES_H
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fsP.h"
#ಅನ್ತರ್ಭಾವ "bmap64.h"

ಶೂನ್ಯ ext2fs_free_inode_bitmap(ext2fs_inode_bitmap bitmap)
{
	ext2fs_free_generic_bmap(bitmap);
}

ಶೂನ್ಯ ext2fs_free_ಖಂಡ_bitmap(ext2fs_ಖಂಡ_bitmap bitmap)
{
	ext2fs_free_generic_bmap(bitmap);
}

errcode_t ext2fs_copy_bitmap(ext2fs_generic_bitmap src,
			     ext2fs_generic_bitmap *dest)
{
	ಹಿಂತಿರುಗಿ (ext2fs_copy_generic_bmap(src, dest));
}
ಶೂನ್ಯ ext2fs_ಗಣ_bitmap_padding(ext2fs_generic_bitmap map)
{
	ext2fs_ಗಣ_generic_bmap_padding(map);
}

errcode_t ext2fs_allocate_inode_bitmap(ext2_filsys fs,
				       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *descr,
				       ext2fs_inode_bitmap *ret)
{
	__u64		ಆರಂಭ, ಕೊನೆ, real_ಕೊನೆ;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ಆದರೆ (ext2fs_has_feature_journal_dev(fs->super))
		ಹಿಂತಿರುಗಿ EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	fs->write_bitmaps = ext2fs_write_bitmaps;

	ಆರಂಭ = 1;
	ಕೊನೆ = fs->super->s_inodes_ಎಣಿಕೆ;
	real_ಕೊನೆ = (__u64)EXT2_INODES_ಪ್ರತಿ_ಗುಂಪು(fs->super) *
		fs->ಗುಂಪು_desc_ಎಣಿಕೆ;

	/* Are we permitted to use new-style bitmaps? */
	ಆದರೆ (fs->flags & EXT2_FLAG_64_ಬಿಟ್ಟುಗಳು)
		ಹಿಂತಿರುಗಿ (ext2fs_alloc_generic_bmap(fs,
				EXT2_ET_MAGIC_INODE_BITMAP64,
				fs->default_bitmap_type,
				ಆರಂಭ, ಕೊನೆ, real_ಕೊನೆ, descr, ret));

	/* Otherwise, check to see if the file system is small enough
	 * to use old-style 32-bit bitmaps */
	ಆದರೆ ((ಕೊನೆ > ~0U) || (real_ಕೊನೆ > ~0U))
		ಹಿಂತಿರುಗಿ EXT2_ET_CANT_USE_LEGACY_BITMAPS;

	ಹಿಂತಿರುಗಿ (ext2fs_make_generic_bitmap(EXT2_ET_MAGIC_INODE_BITMAP, fs,
					 ಆರಂಭ, ಕೊನೆ, real_ಕೊನೆ,
					 descr, 0,
					 (ext2fs_generic_bitmap *) ret));
}

errcode_t ext2fs_allocate_ಖಂಡ_bitmap(ext2_filsys fs,
				       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *descr,
				       ext2fs_ಖಂಡ_bitmap *ret)
{
	__u64		ಆರಂಭ, ಕೊನೆ, real_ಕೊನೆ;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ಆದರೆ (ext2fs_has_feature_journal_dev(fs->super))
		ಹಿಂತಿರುಗಿ EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	fs->write_bitmaps = ext2fs_write_bitmaps;

	ಆರಂಭ = EXT2FS_B2C(fs, fs->super->s_ಮೊದಲ_ದತ್ತಾಂಶ_ಖಂಡ);
	ಕೊನೆ = EXT2FS_B2C(fs, ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(fs->super)-1);
	real_ಕೊನೆ = ((__u64) EXT2_CLUSTERS_ಪ್ರತಿ_ಗುಂಪು(fs->super)
		    * (__u64) fs->ಗುಂಪು_desc_ಎಣಿಕೆ)-1 + ಆರಂಭ;

	ಆದರೆ (fs->flags & EXT2_FLAG_64_ಬಿಟ್ಟುಗಳು)
		ಹಿಂತಿರುಗಿ (ext2fs_alloc_generic_bmap(fs,
				EXT2_ET_MAGIC_ಖಂಡ_BITMAP64,
				fs->default_bitmap_type,
				ಆರಂಭ, ಕೊನೆ, real_ಕೊನೆ, descr, ret));

	ಆದರೆ ((ಕೊನೆ > ~0U) || (real_ಕೊನೆ > ~0U))
		ಹಿಂತಿರುಗಿ EXT2_ET_CANT_USE_LEGACY_BITMAPS;

	ಹಿಂತಿರುಗಿ (ext2fs_make_generic_bitmap(EXT2_ET_MAGIC_ಖಂಡ_BITMAP, fs,
					   ಆರಂಭ, ಕೊನೆ, real_ಕೊನೆ,
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
					    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *descr,
					    ext2fs_ಖಂಡ_bitmap *ret)
{
	__u64			ಆರಂಭ, ಕೊನೆ, real_ಕೊನೆ;
	ext2fs_generic_bitmap	bmap;
	ext2fs_generic_bitmap_64 bmap64;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ಆದರೆ (ext2fs_has_feature_journal_dev(fs->super))
		ಹಿಂತಿರುಗಿ EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	fs->write_bitmaps = ext2fs_write_bitmaps;

	ಆದರೆ (!fs->cluster_ಅನುಪಾತ_ಬಿಟ್ಟುಗಳು)
		ಹಿಂತಿರುಗಿ ext2fs_allocate_ಖಂಡ_bitmap(fs, descr, ret);

	ಆದರೆ ((fs->flags & EXT2_FLAG_64_ಬಿಟ್ಟುಗಳು) == 0)
		ಹಿಂತಿರುಗಿ EXT2_ET_CANT_USE_LEGACY_BITMAPS;

	ಆರಂಭ = fs->super->s_ಮೊದಲ_ದತ್ತಾಂಶ_ಖಂಡ;
	ಕೊನೆ = ext2fs_ಖಂಡಗಳು_ಎಣಿಕೆ(fs->super)-1;
	real_ಕೊನೆ = ((__u64) EXT2_ಖಂಡಗಳು_ಪ್ರತಿ_ಗುಂಪು(fs->super)
		    * (__u64) fs->ಗುಂಪು_desc_ಎಣಿಕೆ)-1 + ಆರಂಭ;

	retval = ext2fs_alloc_generic_bmap(fs, EXT2_ET_MAGIC_ಖಂಡ_BITMAP64,
					   fs->default_bitmap_type, ಆರಂಭ,
					   ಕೊನೆ, real_ಕೊನೆ, descr, &bmap);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;
	bmap64 = (ext2fs_generic_bitmap_64) bmap;
	bmap64->cluster_ಬಿಟ್ಟುಗಳು = 0;
	*ret = bmap;
	ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ ext2fs_get_bitmap_granularity(ext2fs_ಖಂಡ_bitmap bitmap)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) bitmap;

	ಆದರೆ (!EXT2FS_IS_64_BITMAP(bmap))
		ಹಿಂತಿರುಗಿ 0;

	ಹಿಂತಿರುಗಿ bmap->cluster_ಬಿಟ್ಟುಗಳು;
}

errcode_t ext2fs_fudge_inode_bitmap_ಕೊನೆ(ext2fs_inode_bitmap bitmap,
					ext2_ino_t ಕೊನೆ, ext2_ino_t *oend)
{
	__u64 tmp_oend;
	ಪೂರ್ಣಾಂಕ retval;

	retval = ext2fs_fudge_generic_bmap_ಕೊನೆ((ext2fs_generic_bitmap) bitmap,
					       EXT2_ET_FUDGE_INODE_BITMAP_ಕೊನೆ,
					       ಕೊನೆ, &tmp_oend);
	ಆದರೆ (oend)
		*oend = tmp_oend;
	ಹಿಂತಿರುಗಿ retval;
}

errcode_t ext2fs_fudge_ಖಂಡ_bitmap_ಕೊನೆ(ext2fs_ಖಂಡ_bitmap bitmap,
					blk_t ಕೊನೆ, blk_t *oend)
{
	ಹಿಂತಿರುಗಿ (ext2fs_fudge_generic_bitmap_ಕೊನೆ(bitmap,
						EXT2_ET_MAGIC_ಖಂಡ_BITMAP,
						EXT2_ET_FUDGE_ಖಂಡ_BITMAP_ಕೊನೆ,
						ಕೊನೆ, oend));
}

errcode_t ext2fs_fudge_ಖಂಡ_bitmap_end2(ext2fs_ಖಂಡ_bitmap bitmap,
					 blk64_t ಕೊನೆ, blk64_t *oend)
{
	ಹಿಂತಿರುಗಿ (ext2fs_fudge_generic_bmap_ಕೊನೆ(bitmap,
					      EXT2_ET_FUDGE_ಖಂಡ_BITMAP_ಕೊನೆ,
					      ಕೊನೆ, oend));
}

ಶೂನ್ಯ ext2fs_clear_inode_bitmap(ext2fs_inode_bitmap bitmap)
{
	ext2fs_clear_generic_bmap(bitmap);
}

ಶೂನ್ಯ ext2fs_clear_ಖಂಡ_bitmap(ext2fs_ಖಂಡ_bitmap bitmap)
{
	ext2fs_clear_generic_bmap(bitmap);
}

errcode_t ext2fs_resize_inode_bitmap(__u32 new_ಕೊನೆ, __u32 new_real_ಕೊನೆ,
				     ext2fs_inode_bitmap bmap)
{
	ಹಿಂತಿರುಗಿ (ext2fs_resize_generic_bitmap(EXT2_ET_MAGIC_INODE_BITMAP,
					     new_ಕೊನೆ, new_real_ಕೊನೆ, bmap));
}

errcode_t ext2fs_resize_inode_bitmap2(__u64 new_ಕೊನೆ, __u64 new_real_ಕೊನೆ,
				      ext2fs_inode_bitmap bmap)
{
	ಹಿಂತಿರುಗಿ (ext2fs_resize_generic_bmap(bmap, new_ಕೊನೆ, new_real_ಕೊನೆ));
}

errcode_t ext2fs_resize_ಖಂಡ_bitmap(__u32 new_ಕೊನೆ, __u32 new_real_ಕೊನೆ,
				     ext2fs_ಖಂಡ_bitmap bmap)
{
	ಹಿಂತಿರುಗಿ (ext2fs_resize_generic_bitmap(EXT2_ET_MAGIC_ಖಂಡ_BITMAP,
					     new_ಕೊನೆ, new_real_ಕೊನೆ, bmap));
}

errcode_t ext2fs_resize_ಖಂಡ_bitmap2(__u64 new_ಕೊನೆ, __u64 new_real_ಕೊನೆ,
				      ext2fs_ಖಂಡ_bitmap bmap)
{
	ಹಿಂತಿರುಗಿ (ext2fs_resize_generic_bmap(bmap, new_ಕೊನೆ, new_real_ಕೊನೆ));
}

errcode_t ext2fs_compare_ಖಂಡ_bitmap(ext2fs_ಖಂಡ_bitmap bm1,
				      ext2fs_ಖಂಡ_bitmap bm2)
{
	ಹಿಂತಿರುಗಿ (ext2fs_compare_generic_bmap(EXT2_ET_NEQ_ಖಂಡ_BITMAP,
					    bm1, bm2));
}

errcode_t ext2fs_compare_inode_bitmap(ext2fs_inode_bitmap bm1,
				      ext2fs_inode_bitmap bm2)
{
	ಹಿಂತಿರುಗಿ (ext2fs_compare_generic_bmap(EXT2_ET_NEQ_INODE_BITMAP,
					    bm1, bm2));
}

errcode_t ext2fs_ಗಣ_inode_bitmap_range(ext2fs_inode_bitmap bmap,
					ext2_ino_t ಆರಂಭ, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಅಂಕೆ,
					ಶೂನ್ಯ *ಒಳ)
{
	ಹಿಂತಿರುಗಿ (ext2fs_ಗಣ_generic_bitmap_range(bmap,
						EXT2_ET_MAGIC_INODE_BITMAP,
						ಆರಂಭ, ಅಂಕೆ, ಒಳ));
}

errcode_t ext2fs_ಗಣ_inode_bitmap_range2(ext2fs_inode_bitmap bmap,
					 __u64 ಆರಂಭ, size_t ಅಂಕೆ,
					 ಶೂನ್ಯ *ಒಳ)
{
	ಹಿಂತಿರುಗಿ (ext2fs_ಗಣ_generic_bmap_range(bmap, ಆರಂಭ, ಅಂಕೆ, ಒಳ));
}

errcode_t ext2fs_get_inode_bitmap_range(ext2fs_inode_bitmap bmap,
					ext2_ino_t ಆರಂಭ, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಅಂಕೆ,
					ಶೂನ್ಯ *ಹೊರಹೋಗು)
{
	ಹಿಂತಿರುಗಿ (ext2fs_get_generic_bitmap_range(bmap,
						EXT2_ET_MAGIC_INODE_BITMAP,
						ಆರಂಭ, ಅಂಕೆ, ಹೊರಹೋಗು));
}

errcode_t ext2fs_get_inode_bitmap_range2(ext2fs_inode_bitmap bmap,
					 __u64 ಆರಂಭ, size_t ಅಂಕೆ,
					 ಶೂನ್ಯ *ಹೊರಹೋಗು)
{
	ಹಿಂತಿರುಗಿ (ext2fs_get_generic_bmap_range(bmap, ಆರಂಭ, ಅಂಕೆ, ಹೊರಹೋಗು));
}

errcode_t ext2fs_ಗಣ_ಖಂಡ_bitmap_range(ext2fs_ಖಂಡ_bitmap bmap,
					blk_t ಆರಂಭ, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಅಂಕೆ,
					ಶೂನ್ಯ *ಒಳ)
{
	ಹಿಂತಿರುಗಿ (ext2fs_ಗಣ_generic_bitmap_range(bmap,
						EXT2_ET_MAGIC_ಖಂಡ_BITMAP,
						ಆರಂಭ, ಅಂಕೆ, ಒಳ));
}

errcode_t ext2fs_ಗಣ_ಖಂಡ_bitmap_range2(ext2fs_ಖಂಡ_bitmap bmap,
					 blk64_t ಆರಂಭ, size_t ಅಂಕೆ,
					 ಶೂನ್ಯ *ಒಳ)
{
	ಹಿಂತಿರುಗಿ (ext2fs_ಗಣ_generic_bmap_range(bmap, ಆರಂಭ, ಅಂಕೆ, ಒಳ));
}

errcode_t ext2fs_get_ಖಂಡ_bitmap_range(ext2fs_ಖಂಡ_bitmap bmap,
					blk_t ಆರಂಭ, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಅಂಕೆ,
					ಶೂನ್ಯ *ಹೊರಹೋಗು)
{
	ಹಿಂತಿರುಗಿ (ext2fs_get_generic_bitmap_range(bmap,
						EXT2_ET_MAGIC_ಖಂಡ_BITMAP,
						ಆರಂಭ, ಅಂಕೆ, ಹೊರಹೋಗು));
}

errcode_t ext2fs_get_ಖಂಡ_bitmap_range2(ext2fs_ಖಂಡ_bitmap bmap,
					 blk64_t ಆರಂಭ, size_t ಅಂಕೆ,
					 ಶೂನ್ಯ *ಹೊರಹೋಗು)
{
	ಹಿಂತಿರುಗಿ (ext2fs_get_generic_bmap_range(bmap, ಆರಂಭ, ಅಂಕೆ, ಹೊರಹೋಗು));
}
