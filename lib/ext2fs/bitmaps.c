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

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <string.h>
#ఒకవేళ HAVE_UNISTD_H
#అన్తర్భావ <unistd.h>
#అయితే_ముగింపు
#అన్తర్భావ <fcntl.h>
#అన్తర్భావ <time.h>
#ఒకవేళ HAVE_SYS_STAT_H
#అన్తర్భావ <sys/stat.h>
#అయితే_ముగింపు
#ఒకవేళ HAVE_SYS_TYPES_H
#అన్తర్భావ <sys/types.h>
#అయితే_ముగింపు

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fs.h"
#అన్తర్భావ "ext2fsP.h"
#అన్తర్భావ "bmap64.h"

శూన్య ext2fs_free_inode_bitmap(ext2fs_inode_bitmap bitmap)
{
	ext2fs_free_generic_bmap(bitmap);
}

శూన్య ext2fs_free_ఖండం_bitmap(ext2fs_ఖండం_bitmap bitmap)
{
	ext2fs_free_generic_bmap(bitmap);
}

errcode_t ext2fs_copy_bitmap(ext2fs_generic_bitmap src,
			     ext2fs_generic_bitmap *dest)
{
	తిరిగి (ext2fs_copy_generic_bmap(src, dest));
}
శూన్య ext2fs_సమితి_bitmap_padding(ext2fs_generic_bitmap map)
{
	ext2fs_సమితి_generic_bmap_padding(map);
}

errcode_t ext2fs_allocate_inode_bitmap(ext2_filsys fs,
				       స్థిరరాశి అక్షరం *descr,
				       ext2fs_inode_bitmap *ret)
{
	__u64		ప్రారంభం, చివర, real_చివర;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ఒకవేళ (ext2fs_has_feature_journal_dev(fs->super))
		తిరిగి EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	fs->write_bitmaps = ext2fs_write_bitmaps;

	ప్రారంభం = 1;
	చివర = fs->super->s_inodes_లెక్క;
	real_చివర = (__u64)EXT2_INODES_చొప్పున_సమూహం(fs->super) *
		fs->సమూహం_desc_లెక్క;

	/* Are we permitted to use new-style bitmaps? */
	ఒకవేళ (fs->flags & EXT2_FLAG_64_బిట్లు)
		తిరిగి (ext2fs_alloc_generic_bmap(fs,
				EXT2_ET_MAGIC_INODE_BITMAP64,
				fs->default_bitmap_type,
				ప్రారంభం, చివర, real_చివర, descr, ret));

	/* Otherwise, check to see if the file system is small enough
	 * to use old-style 32-bit bitmaps */
	ఒకవేళ ((చివర > ~0U) || (real_చివర > ~0U))
		తిరిగి EXT2_ET_CANT_USE_LEGACY_BITMAPS;

	తిరిగి (ext2fs_make_generic_bitmap(EXT2_ET_MAGIC_INODE_BITMAP, fs,
					 ప్రారంభం, చివర, real_చివర,
					 descr, 0,
					 (ext2fs_generic_bitmap *) ret));
}

errcode_t ext2fs_allocate_ఖండం_bitmap(ext2_filsys fs,
				       స్థిరరాశి అక్షరం *descr,
				       ext2fs_ఖండం_bitmap *ret)
{
	__u64		ప్రారంభం, చివర, real_చివర;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ఒకవేళ (ext2fs_has_feature_journal_dev(fs->super))
		తిరిగి EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	fs->write_bitmaps = ext2fs_write_bitmaps;

	ప్రారంభం = EXT2FS_B2C(fs, fs->super->s_మొదటి_దత్తాంశం_ఖండం);
	చివర = EXT2FS_B2C(fs, ext2fs_ఖండాలు_లెక్క(fs->super)-1);
	real_చివర = ((__u64) EXT2_CLUSTERS_చొప్పున_సమూహం(fs->super)
		    * (__u64) fs->సమూహం_desc_లెక్క)-1 + ప్రారంభం;

	ఒకవేళ (fs->flags & EXT2_FLAG_64_బిట్లు)
		తిరిగి (ext2fs_alloc_generic_bmap(fs,
				EXT2_ET_MAGIC_ఖండం_BITMAP64,
				fs->default_bitmap_type,
				ప్రారంభం, చివర, real_చివర, descr, ret));

	ఒకవేళ ((చివర > ~0U) || (real_చివర > ~0U))
		తిరిగి EXT2_ET_CANT_USE_LEGACY_BITMAPS;

	తిరిగి (ext2fs_make_generic_bitmap(EXT2_ET_MAGIC_ఖండం_BITMAP, fs,
					   ప్రారంభం, చివర, real_చివర,
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
					    స్థిరరాశి అక్షరం *descr,
					    ext2fs_ఖండం_bitmap *ret)
{
	__u64			ప్రారంభం, చివర, real_చివర;
	ext2fs_generic_bitmap	bmap;
	ext2fs_generic_bitmap_64 bmap64;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	ఒకవేళ (ext2fs_has_feature_journal_dev(fs->super))
		తిరిగి EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	fs->write_bitmaps = ext2fs_write_bitmaps;

	ఒకవేళ (!fs->cluster_నిష్పత్తి_బిట్లు)
		తిరిగి ext2fs_allocate_ఖండం_bitmap(fs, descr, ret);

	ఒకవేళ ((fs->flags & EXT2_FLAG_64_బిట్లు) == 0)
		తిరిగి EXT2_ET_CANT_USE_LEGACY_BITMAPS;

	ప్రారంభం = fs->super->s_మొదటి_దత్తాంశం_ఖండం;
	చివర = ext2fs_ఖండాలు_లెక్క(fs->super)-1;
	real_చివర = ((__u64) EXT2_ఖండాలు_చొప్పున_సమూహం(fs->super)
		    * (__u64) fs->సమూహం_desc_లెక్క)-1 + ప్రారంభం;

	retval = ext2fs_alloc_generic_bmap(fs, EXT2_ET_MAGIC_ఖండం_BITMAP64,
					   fs->default_bitmap_type, ప్రారంభం,
					   చివర, real_చివర, descr, &bmap);
	ఒకవేళ (retval)
		తిరిగి retval;
	bmap64 = (ext2fs_generic_bitmap_64) bmap;
	bmap64->cluster_బిట్లు = 0;
	*ret = bmap;
	తిరిగి 0;
}

పూర్ణాంకం ext2fs_get_bitmap_granularity(ext2fs_ఖండం_bitmap bitmap)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) bitmap;

	ఒకవేళ (!EXT2FS_IS_64_BITMAP(bmap))
		తిరిగి 0;

	తిరిగి bmap->cluster_బిట్లు;
}

errcode_t ext2fs_fudge_inode_bitmap_చివర(ext2fs_inode_bitmap bitmap,
					ext2_ino_t చివర, ext2_ino_t *oend)
{
	__u64 tmp_oend;
	పూర్ణాంకం retval;

	retval = ext2fs_fudge_generic_bmap_చివర((ext2fs_generic_bitmap) bitmap,
					       EXT2_ET_FUDGE_INODE_BITMAP_చివర,
					       చివర, &tmp_oend);
	ఒకవేళ (oend)
		*oend = tmp_oend;
	తిరిగి retval;
}

errcode_t ext2fs_fudge_ఖండం_bitmap_చివర(ext2fs_ఖండం_bitmap bitmap,
					blk_t చివర, blk_t *oend)
{
	తిరిగి (ext2fs_fudge_generic_bitmap_చివర(bitmap,
						EXT2_ET_MAGIC_ఖండం_BITMAP,
						EXT2_ET_FUDGE_ఖండం_BITMAP_చివర,
						చివర, oend));
}

errcode_t ext2fs_fudge_ఖండం_bitmap_end2(ext2fs_ఖండం_bitmap bitmap,
					 blk64_t చివర, blk64_t *oend)
{
	తిరిగి (ext2fs_fudge_generic_bmap_చివర(bitmap,
					      EXT2_ET_FUDGE_ఖండం_BITMAP_చివర,
					      చివర, oend));
}

శూన్య ext2fs_clear_inode_bitmap(ext2fs_inode_bitmap bitmap)
{
	ext2fs_clear_generic_bmap(bitmap);
}

శూన్య ext2fs_clear_ఖండం_bitmap(ext2fs_ఖండం_bitmap bitmap)
{
	ext2fs_clear_generic_bmap(bitmap);
}

errcode_t ext2fs_resize_inode_bitmap(__u32 new_చివర, __u32 new_real_చివర,
				     ext2fs_inode_bitmap bmap)
{
	తిరిగి (ext2fs_resize_generic_bitmap(EXT2_ET_MAGIC_INODE_BITMAP,
					     new_చివర, new_real_చివర, bmap));
}

errcode_t ext2fs_resize_inode_bitmap2(__u64 new_చివర, __u64 new_real_చివర,
				      ext2fs_inode_bitmap bmap)
{
	తిరిగి (ext2fs_resize_generic_bmap(bmap, new_చివర, new_real_చివర));
}

errcode_t ext2fs_resize_ఖండం_bitmap(__u32 new_చివర, __u32 new_real_చివర,
				     ext2fs_ఖండం_bitmap bmap)
{
	తిరిగి (ext2fs_resize_generic_bitmap(EXT2_ET_MAGIC_ఖండం_BITMAP,
					     new_చివర, new_real_చివర, bmap));
}

errcode_t ext2fs_resize_ఖండం_bitmap2(__u64 new_చివర, __u64 new_real_చివర,
				      ext2fs_ఖండం_bitmap bmap)
{
	తిరిగి (ext2fs_resize_generic_bmap(bmap, new_చివర, new_real_చివర));
}

errcode_t ext2fs_compare_ఖండం_bitmap(ext2fs_ఖండం_bitmap bm1,
				      ext2fs_ఖండం_bitmap bm2)
{
	తిరిగి (ext2fs_compare_generic_bmap(EXT2_ET_NEQ_ఖండం_BITMAP,
					    bm1, bm2));
}

errcode_t ext2fs_compare_inode_bitmap(ext2fs_inode_bitmap bm1,
				      ext2fs_inode_bitmap bm2)
{
	తిరిగి (ext2fs_compare_generic_bmap(EXT2_ET_NEQ_INODE_BITMAP,
					    bm1, bm2));
}

errcode_t ext2fs_సమితి_inode_bitmap_range(ext2fs_inode_bitmap bmap,
					ext2_ino_t ప్రారంభం, గుర్తులేని పూర్ణాంకం అంకె,
					శూన్య *లోపల)
{
	తిరిగి (ext2fs_సమితి_generic_bitmap_range(bmap,
						EXT2_ET_MAGIC_INODE_BITMAP,
						ప్రారంభం, అంకె, లోపల));
}

errcode_t ext2fs_సమితి_inode_bitmap_range2(ext2fs_inode_bitmap bmap,
					 __u64 ప్రారంభం, size_t అంకె,
					 శూన్య *లోపల)
{
	తిరిగి (ext2fs_సమితి_generic_bmap_range(bmap, ప్రారంభం, అంకె, లోపల));
}

errcode_t ext2fs_get_inode_bitmap_range(ext2fs_inode_bitmap bmap,
					ext2_ino_t ప్రారంభం, గుర్తులేని పూర్ణాంకం అంకె,
					శూన్య *బహిర్గతం)
{
	తిరిగి (ext2fs_get_generic_bitmap_range(bmap,
						EXT2_ET_MAGIC_INODE_BITMAP,
						ప్రారంభం, అంకె, బహిర్గతం));
}

errcode_t ext2fs_get_inode_bitmap_range2(ext2fs_inode_bitmap bmap,
					 __u64 ప్రారంభం, size_t అంకె,
					 శూన్య *బహిర్గతం)
{
	తిరిగి (ext2fs_get_generic_bmap_range(bmap, ప్రారంభం, అంకె, బహిర్గతం));
}

errcode_t ext2fs_సమితి_ఖండం_bitmap_range(ext2fs_ఖండం_bitmap bmap,
					blk_t ప్రారంభం, గుర్తులేని పూర్ణాంకం అంకె,
					శూన్య *లోపల)
{
	తిరిగి (ext2fs_సమితి_generic_bitmap_range(bmap,
						EXT2_ET_MAGIC_ఖండం_BITMAP,
						ప్రారంభం, అంకె, లోపల));
}

errcode_t ext2fs_సమితి_ఖండం_bitmap_range2(ext2fs_ఖండం_bitmap bmap,
					 blk64_t ప్రారంభం, size_t అంకె,
					 శూన్య *లోపల)
{
	తిరిగి (ext2fs_సమితి_generic_bmap_range(bmap, ప్రారంభం, అంకె, లోపల));
}

errcode_t ext2fs_get_ఖండం_bitmap_range(ext2fs_ఖండం_bitmap bmap,
					blk_t ప్రారంభం, గుర్తులేని పూర్ణాంకం అంకె,
					శూన్య *బహిర్గతం)
{
	తిరిగి (ext2fs_get_generic_bitmap_range(bmap,
						EXT2_ET_MAGIC_ఖండం_BITMAP,
						ప్రారంభం, అంకె, బహిర్గతం));
}

errcode_t ext2fs_get_ఖండం_bitmap_range2(ext2fs_ఖండం_bitmap bmap,
					 blk64_t ప్రారంభం, size_t అంకె,
					 శూన్య *బహిర్గతం)
{
	తిరిగి (ext2fs_get_generic_bmap_range(bmap, ప్రారంభం, అంకె, బహిర్గతం));
}
