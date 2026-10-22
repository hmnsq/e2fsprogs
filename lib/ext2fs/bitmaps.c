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

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#यदि_अन्तः
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <time.h>
#यदि HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#यदि_अन्तः
#यदि HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#यदि_अन्तः

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fs.h"
#अन्तर्भाव "ext2fsP.h"
#अन्तर्भाव "bmap64.h"

शून्य ext2fs_free_inode_bitmap(ext2fs_inode_bitmap bitmap)
{
	ext2fs_free_generic_bmap(bitmap);
}

शून्य ext2fs_free_खण्डम्_bitmap(ext2fs_खण्डम्_bitmap bitmap)
{
	ext2fs_free_generic_bmap(bitmap);
}

errcode_t ext2fs_copy_bitmap(ext2fs_generic_bitmap src,
			     ext2fs_generic_bitmap *dest)
{
	प्रत्यागम (ext2fs_copy_generic_bmap(src, dest));
}
शून्य ext2fs_समुच्चयः_bitmap_padding(ext2fs_generic_bitmap map)
{
	ext2fs_समुच्चयः_generic_bmap_padding(map);
}

errcode_t ext2fs_allocate_inode_bitmap(ext2_filsys fs,
				       ध्रुव वर्ण *descr,
				       ext2fs_inode_bitmap *ret)
{
	__u64		start, अन्तः, real_अन्तः;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	यदि (ext2fs_has_feature_journal_dev(fs->super))
		प्रत्यागम EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	fs->write_bitmaps = ext2fs_write_bitmaps;

	start = 1;
	अन्तः = fs->super->s_inodes_गणनम्;
	real_अन्तः = (__u64)EXT2_INODES_प्रत्येकम्_गणः(fs->super) *
		fs->गणः_desc_गणनम्;

	/* Are we permitted to use new-style bitmaps? */
	यदि (fs->flags & EXT2_FLAG_64_द्वयङ्काः)
		प्रत्यागम (ext2fs_alloc_generic_bmap(fs,
				EXT2_ET_MAGIC_INODE_BITMAP64,
				fs->default_bitmap_type,
				start, अन्तः, real_अन्तः, descr, ret));

	/* Otherwise, check to see if the file system is small enough
	 * to use old-style 32-bit bitmaps */
	यदि ((अन्तः > ~0U) || (real_अन्तः > ~0U))
		प्रत्यागम EXT2_ET_CANT_USE_LEGACY_BITMAPS;

	प्रत्यागम (ext2fs_make_generic_bitmap(EXT2_ET_MAGIC_INODE_BITMAP, fs,
					 start, अन्तः, real_अन्तः,
					 descr, 0,
					 (ext2fs_generic_bitmap *) ret));
}

errcode_t ext2fs_allocate_खण्डम्_bitmap(ext2_filsys fs,
				       ध्रुव वर्ण *descr,
				       ext2fs_खण्डम्_bitmap *ret)
{
	__u64		start, अन्तः, real_अन्तः;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	यदि (ext2fs_has_feature_journal_dev(fs->super))
		प्रत्यागम EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	fs->write_bitmaps = ext2fs_write_bitmaps;

	start = EXT2FS_B2C(fs, fs->super->s_first_दत्त_खण्डम्);
	अन्तः = EXT2FS_B2C(fs, ext2fs_खण्डानि_गणनम्(fs->super)-1);
	real_अन्तः = ((__u64) EXT2_CLUSTERS_प्रत्येकम्_गणः(fs->super)
		    * (__u64) fs->गणः_desc_गणनम्)-1 + start;

	यदि (fs->flags & EXT2_FLAG_64_द्वयङ्काः)
		प्रत्यागम (ext2fs_alloc_generic_bmap(fs,
				EXT2_ET_MAGIC_खण्डम्_BITMAP64,
				fs->default_bitmap_type,
				start, अन्तः, real_अन्तः, descr, ret));

	यदि ((अन्तः > ~0U) || (real_अन्तः > ~0U))
		प्रत्यागम EXT2_ET_CANT_USE_LEGACY_BITMAPS;

	प्रत्यागम (ext2fs_make_generic_bitmap(EXT2_ET_MAGIC_खण्डम्_BITMAP, fs,
					   start, अन्तः, real_अन्तः,
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
					    ध्रुव वर्ण *descr,
					    ext2fs_खण्डम्_bitmap *ret)
{
	__u64			start, अन्तः, real_अन्तः;
	ext2fs_generic_bitmap	bmap;
	ext2fs_generic_bitmap_64 bmap64;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	यदि (ext2fs_has_feature_journal_dev(fs->super))
		प्रत्यागम EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	fs->write_bitmaps = ext2fs_write_bitmaps;

	यदि (!fs->cluster_अनुपातः_द्वयङ्काः)
		प्रत्यागम ext2fs_allocate_खण्डम्_bitmap(fs, descr, ret);

	यदि ((fs->flags & EXT2_FLAG_64_द्वयङ्काः) == 0)
		प्रत्यागम EXT2_ET_CANT_USE_LEGACY_BITMAPS;

	start = fs->super->s_first_दत्त_खण्डम्;
	अन्तः = ext2fs_खण्डानि_गणनम्(fs->super)-1;
	real_अन्तः = ((__u64) EXT2_खण्डानि_प्रत्येकम्_गणः(fs->super)
		    * (__u64) fs->गणः_desc_गणनम्)-1 + start;

	retval = ext2fs_alloc_generic_bmap(fs, EXT2_ET_MAGIC_खण्डम्_BITMAP64,
					   fs->default_bitmap_type, start,
					   अन्तः, real_अन्तः, descr, &bmap);
	यदि (retval)
		प्रत्यागम retval;
	bmap64 = (ext2fs_generic_bitmap_64) bmap;
	bmap64->cluster_द्वयङ्काः = 0;
	*ret = bmap;
	प्रत्यागम 0;
}

पूर्णाङ्क ext2fs_get_bitmap_granularity(ext2fs_खण्डम्_bitmap bitmap)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) bitmap;

	यदि (!EXT2FS_IS_64_BITMAP(bmap))
		प्रत्यागम 0;

	प्रत्यागम bmap->cluster_द्वयङ्काः;
}

errcode_t ext2fs_fudge_inode_bitmap_अन्तः(ext2fs_inode_bitmap bitmap,
					ext2_ino_t अन्तः, ext2_ino_t *oend)
{
	__u64 tmp_oend;
	पूर्णाङ्क retval;

	retval = ext2fs_fudge_generic_bmap_अन्तः((ext2fs_generic_bitmap) bitmap,
					       EXT2_ET_FUDGE_INODE_BITMAP_अन्तः,
					       अन्तः, &tmp_oend);
	यदि (oend)
		*oend = tmp_oend;
	प्रत्यागम retval;
}

errcode_t ext2fs_fudge_खण्डम्_bitmap_अन्तः(ext2fs_खण्डम्_bitmap bitmap,
					blk_t अन्तः, blk_t *oend)
{
	प्रत्यागम (ext2fs_fudge_generic_bitmap_अन्तः(bitmap,
						EXT2_ET_MAGIC_खण्डम्_BITMAP,
						EXT2_ET_FUDGE_खण्डम्_BITMAP_अन्तः,
						अन्तः, oend));
}

errcode_t ext2fs_fudge_खण्डम्_bitmap_end2(ext2fs_खण्डम्_bitmap bitmap,
					 blk64_t अन्तः, blk64_t *oend)
{
	प्रत्यागम (ext2fs_fudge_generic_bmap_अन्तः(bitmap,
					      EXT2_ET_FUDGE_खण्डम्_BITMAP_अन्तः,
					      अन्तः, oend));
}

शून्य ext2fs_clear_inode_bitmap(ext2fs_inode_bitmap bitmap)
{
	ext2fs_clear_generic_bmap(bitmap);
}

शून्य ext2fs_clear_खण्डम्_bitmap(ext2fs_खण्डम्_bitmap bitmap)
{
	ext2fs_clear_generic_bmap(bitmap);
}

errcode_t ext2fs_resize_inode_bitmap(__u32 new_अन्तः, __u32 new_real_अन्तः,
				     ext2fs_inode_bitmap bmap)
{
	प्रत्यागम (ext2fs_resize_generic_bitmap(EXT2_ET_MAGIC_INODE_BITMAP,
					     new_अन्तः, new_real_अन्तः, bmap));
}

errcode_t ext2fs_resize_inode_bitmap2(__u64 new_अन्तः, __u64 new_real_अन्तः,
				      ext2fs_inode_bitmap bmap)
{
	प्रत्यागम (ext2fs_resize_generic_bmap(bmap, new_अन्तः, new_real_अन्तः));
}

errcode_t ext2fs_resize_खण्डम्_bitmap(__u32 new_अन्तः, __u32 new_real_अन्तः,
				     ext2fs_खण्डम्_bitmap bmap)
{
	प्रत्यागम (ext2fs_resize_generic_bitmap(EXT2_ET_MAGIC_खण्डम्_BITMAP,
					     new_अन्तः, new_real_अन्तः, bmap));
}

errcode_t ext2fs_resize_खण्डम्_bitmap2(__u64 new_अन्तः, __u64 new_real_अन्तः,
				      ext2fs_खण्डम्_bitmap bmap)
{
	प्रत्यागम (ext2fs_resize_generic_bmap(bmap, new_अन्तः, new_real_अन्तः));
}

errcode_t ext2fs_compare_खण्डम्_bitmap(ext2fs_खण्डम्_bitmap bm1,
				      ext2fs_खण्डम्_bitmap bm2)
{
	प्रत्यागम (ext2fs_compare_generic_bmap(EXT2_ET_NEQ_खण्डम्_BITMAP,
					    bm1, bm2));
}

errcode_t ext2fs_compare_inode_bitmap(ext2fs_inode_bitmap bm1,
				      ext2fs_inode_bitmap bm2)
{
	प्रत्यागम (ext2fs_compare_generic_bmap(EXT2_ET_NEQ_INODE_BITMAP,
					    bm1, bm2));
}

errcode_t ext2fs_समुच्चयः_inode_bitmap_range(ext2fs_inode_bitmap bmap,
					ext2_ino_t start, अचिह्नित पूर्णाङ्क अङ्कः,
					शून्य *अन्तर्)
{
	प्रत्यागम (ext2fs_समुच्चयः_generic_bitmap_range(bmap,
						EXT2_ET_MAGIC_INODE_BITMAP,
						start, अङ्कः, अन्तर्));
}

errcode_t ext2fs_समुच्चयः_inode_bitmap_range2(ext2fs_inode_bitmap bmap,
					 __u64 start, size_t अङ्कः,
					 शून्य *अन्तर्)
{
	प्रत्यागम (ext2fs_समुच्चयः_generic_bmap_range(bmap, start, अङ्कः, अन्तर्));
}

errcode_t ext2fs_get_inode_bitmap_range(ext2fs_inode_bitmap bmap,
					ext2_ino_t start, अचिह्नित पूर्णाङ्क अङ्कः,
					शून्य *बहिर्गमनम्)
{
	प्रत्यागम (ext2fs_get_generic_bitmap_range(bmap,
						EXT2_ET_MAGIC_INODE_BITMAP,
						start, अङ्कः, बहिर्गमनम्));
}

errcode_t ext2fs_get_inode_bitmap_range2(ext2fs_inode_bitmap bmap,
					 __u64 start, size_t अङ्कः,
					 शून्य *बहिर्गमनम्)
{
	प्रत्यागम (ext2fs_get_generic_bmap_range(bmap, start, अङ्कः, बहिर्गमनम्));
}

errcode_t ext2fs_समुच्चयः_खण्डम्_bitmap_range(ext2fs_खण्डम्_bitmap bmap,
					blk_t start, अचिह्नित पूर्णाङ्क अङ्कः,
					शून्य *अन्तर्)
{
	प्रत्यागम (ext2fs_समुच्चयः_generic_bitmap_range(bmap,
						EXT2_ET_MAGIC_खण्डम्_BITMAP,
						start, अङ्कः, अन्तर्));
}

errcode_t ext2fs_समुच्चयः_खण्डम्_bitmap_range2(ext2fs_खण्डम्_bitmap bmap,
					 blk64_t start, size_t अङ्कः,
					 शून्य *अन्तर्)
{
	प्रत्यागम (ext2fs_समुच्चयः_generic_bmap_range(bmap, start, अङ्कः, अन्तर्));
}

errcode_t ext2fs_get_खण्डम्_bitmap_range(ext2fs_खण्डम्_bitmap bmap,
					blk_t start, अचिह्नित पूर्णाङ्क अङ्कः,
					शून्य *बहिर्गमनम्)
{
	प्रत्यागम (ext2fs_get_generic_bitmap_range(bmap,
						EXT2_ET_MAGIC_खण्डम्_BITMAP,
						start, अङ्कः, बहिर्गमनम्));
}

errcode_t ext2fs_get_खण्डम्_bitmap_range2(ext2fs_खण्डम्_bitmap bmap,
					 blk64_t start, size_t अङ्कः,
					 शून्य *बहिर्गमनम्)
{
	प्रत्यागम (ext2fs_get_generic_bmap_range(bmap, start, अङ्कः, बहिर्गमनम्));
}
