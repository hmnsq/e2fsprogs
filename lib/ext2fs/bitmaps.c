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

#অন্তর্ভাব "config.h"
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <string.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাব <unistd.h>
#endif
#অন্তর্ভাব <fcntl.h>
#অন্তর্ভাব <time.h>
#যদি HAVE_SYS_STAT_H
#অন্তর্ভাব <sys/stat.h>
#endif
#যদি HAVE_SYS_TYPES_H
#অন্তর্ভাব <sys/types.h>
#endif

#অন্তর্ভাব "ext2_fs.h"
#অন্তর্ভাব "ext2fs.h"
#অন্তর্ভাব "ext2fsP.h"
#অন্তর্ভাব "bmap64.h"

শূন্য ext2fs_free_inode_bitmap(ext2fs_inode_bitmap bitmap)
{
	ext2fs_free_generic_bmap(bitmap);
}

শূন্য ext2fs_free_block_bitmap(ext2fs_block_bitmap bitmap)
{
	ext2fs_free_generic_bmap(bitmap);
}

errcode_t ext2fs_copy_bitmap(ext2fs_generic_bitmap src,
			     ext2fs_generic_bitmap *dest)
{
	প্রত্যাগম (ext2fs_copy_generic_bmap(src, dest));
}
শূন্য ext2fs_set_bitmap_padding(ext2fs_generic_bitmap map)
{
	ext2fs_set_generic_bmap_padding(map);
}

errcode_t ext2fs_allocate_inode_bitmap(ext2_filsys fs,
				       ধ্রুব বর্ণ *descr,
				       ext2fs_inode_bitmap *ret)
{
	__u64		start, end, real_end;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	যদি (ext2fs_has_feature_journal_dev(fs->super))
		প্রত্যাগম EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	fs->write_bitmaps = ext2fs_write_bitmaps;

	start = 1;
	end = fs->super->s_inodes_গুনতি;
	real_end = (__u64)EXT2_INODES_PER_GROUP(fs->super) *
		fs->group_desc_গুনতি;

	/* Are we permitted to use new-style bitmaps? */
	যদি (fs->flags & EXT2_FLAG_64BITS)
		প্রত্যাগম (ext2fs_alloc_generic_bmap(fs,
				EXT2_ET_MAGIC_INODE_BITMAP64,
				fs->default_bitmap_type,
				start, end, real_end, descr, ret));

	/* Otherwise, check to see if the file system is small enough
	 * to use old-style 32-bit bitmaps */
	যদি ((end > ~0U) || (real_end > ~0U))
		প্রত্যাগম EXT2_ET_CANT_USE_LEGACY_BITMAPS;

	প্রত্যাগম (ext2fs_make_generic_bitmap(EXT2_ET_MAGIC_INODE_BITMAP, fs,
					 start, end, real_end,
					 descr, 0,
					 (ext2fs_generic_bitmap *) ret));
}

errcode_t ext2fs_allocate_block_bitmap(ext2_filsys fs,
				       ধ্রুব বর্ণ *descr,
				       ext2fs_block_bitmap *ret)
{
	__u64		start, end, real_end;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	যদি (ext2fs_has_feature_journal_dev(fs->super))
		প্রত্যাগম EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	fs->write_bitmaps = ext2fs_write_bitmaps;

	start = EXT2FS_B2C(fs, fs->super->s_first_data_block);
	end = EXT2FS_B2C(fs, ext2fs_blocks_গুনতি(fs->super)-1);
	real_end = ((__u64) EXT2_CLUSTERS_PER_GROUP(fs->super)
		    * (__u64) fs->group_desc_গুনতি)-1 + start;

	যদি (fs->flags & EXT2_FLAG_64BITS)
		প্রত্যাগম (ext2fs_alloc_generic_bmap(fs,
				EXT2_ET_MAGIC_BLOCK_BITMAP64,
				fs->default_bitmap_type,
				start, end, real_end, descr, ret));

	যদি ((end > ~0U) || (real_end > ~0U))
		প্রত্যাগম EXT2_ET_CANT_USE_LEGACY_BITMAPS;

	প্রত্যাগম (ext2fs_make_generic_bitmap(EXT2_ET_MAGIC_BLOCK_BITMAP, fs,
					   start, end, real_end,
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
					    ধ্রুব বর্ণ *descr,
					    ext2fs_block_bitmap *ret)
{
	__u64			start, end, real_end;
	ext2fs_generic_bitmap	bmap;
	ext2fs_generic_bitmap_64 bmap64;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(fs, EXT2_ET_MAGIC_EXT2FS_FILSYS);

	যদি (ext2fs_has_feature_journal_dev(fs->super))
		প্রত্যাগম EXT2_ET_EXTERNAL_JOURNAL_NOSUPP;

	fs->write_bitmaps = ext2fs_write_bitmaps;

	যদি (!fs->cluster_ratio_bits)
		প্রত্যাগম ext2fs_allocate_block_bitmap(fs, descr, ret);

	যদি ((fs->flags & EXT2_FLAG_64BITS) == 0)
		প্রত্যাগম EXT2_ET_CANT_USE_LEGACY_BITMAPS;

	start = fs->super->s_first_data_block;
	end = ext2fs_blocks_গুনতি(fs->super)-1;
	real_end = ((__u64) EXT2_BLOCKS_PER_GROUP(fs->super)
		    * (__u64) fs->group_desc_গুনতি)-1 + start;

	retval = ext2fs_alloc_generic_bmap(fs, EXT2_ET_MAGIC_BLOCK_BITMAP64,
					   fs->default_bitmap_type, start,
					   end, real_end, descr, &bmap);
	যদি (retval)
		প্রত্যাগম retval;
	bmap64 = (ext2fs_generic_bitmap_64) bmap;
	bmap64->cluster_bits = 0;
	*ret = bmap;
	প্রত্যাগম 0;
}

পূর্ণাঙ্ক ext2fs_get_bitmap_granularity(ext2fs_block_bitmap bitmap)
{
	ext2fs_generic_bitmap_64 bmap = (ext2fs_generic_bitmap_64) bitmap;

	যদি (!EXT2FS_IS_64_BITMAP(bmap))
		প্রত্যাগম 0;

	প্রত্যাগম bmap->cluster_bits;
}

errcode_t ext2fs_fudge_inode_bitmap_end(ext2fs_inode_bitmap bitmap,
					ext2_ino_t end, ext2_ino_t *oend)
{
	__u64 tmp_oend;
	পূর্ণাঙ্ক retval;

	retval = ext2fs_fudge_generic_bmap_end((ext2fs_generic_bitmap) bitmap,
					       EXT2_ET_FUDGE_INODE_BITMAP_END,
					       end, &tmp_oend);
	যদি (oend)
		*oend = tmp_oend;
	প্রত্যাগম retval;
}

errcode_t ext2fs_fudge_block_bitmap_end(ext2fs_block_bitmap bitmap,
					blk_t end, blk_t *oend)
{
	প্রত্যাগম (ext2fs_fudge_generic_bitmap_end(bitmap,
						EXT2_ET_MAGIC_BLOCK_BITMAP,
						EXT2_ET_FUDGE_BLOCK_BITMAP_END,
						end, oend));
}

errcode_t ext2fs_fudge_block_bitmap_end2(ext2fs_block_bitmap bitmap,
					 blk64_t end, blk64_t *oend)
{
	প্রত্যাগম (ext2fs_fudge_generic_bmap_end(bitmap,
					      EXT2_ET_FUDGE_BLOCK_BITMAP_END,
					      end, oend));
}

শূন্য ext2fs_clear_inode_bitmap(ext2fs_inode_bitmap bitmap)
{
	ext2fs_clear_generic_bmap(bitmap);
}

শূন্য ext2fs_clear_block_bitmap(ext2fs_block_bitmap bitmap)
{
	ext2fs_clear_generic_bmap(bitmap);
}

errcode_t ext2fs_resize_inode_bitmap(__u32 new_end, __u32 new_real_end,
				     ext2fs_inode_bitmap bmap)
{
	প্রত্যাগম (ext2fs_resize_generic_bitmap(EXT2_ET_MAGIC_INODE_BITMAP,
					     new_end, new_real_end, bmap));
}

errcode_t ext2fs_resize_inode_bitmap2(__u64 new_end, __u64 new_real_end,
				      ext2fs_inode_bitmap bmap)
{
	প্রত্যাগম (ext2fs_resize_generic_bmap(bmap, new_end, new_real_end));
}

errcode_t ext2fs_resize_block_bitmap(__u32 new_end, __u32 new_real_end,
				     ext2fs_block_bitmap bmap)
{
	প্রত্যাগম (ext2fs_resize_generic_bitmap(EXT2_ET_MAGIC_BLOCK_BITMAP,
					     new_end, new_real_end, bmap));
}

errcode_t ext2fs_resize_block_bitmap2(__u64 new_end, __u64 new_real_end,
				      ext2fs_block_bitmap bmap)
{
	প্রত্যাগম (ext2fs_resize_generic_bmap(bmap, new_end, new_real_end));
}

errcode_t ext2fs_compare_block_bitmap(ext2fs_block_bitmap bm1,
				      ext2fs_block_bitmap bm2)
{
	প্রত্যাগম (ext2fs_compare_generic_bmap(EXT2_ET_NEQ_BLOCK_BITMAP,
					    bm1, bm2));
}

errcode_t ext2fs_compare_inode_bitmap(ext2fs_inode_bitmap bm1,
				      ext2fs_inode_bitmap bm2)
{
	প্রত্যাগম (ext2fs_compare_generic_bmap(EXT2_ET_NEQ_INODE_BITMAP,
					    bm1, bm2));
}

errcode_t ext2fs_set_inode_bitmap_range(ext2fs_inode_bitmap bmap,
					ext2_ino_t start, unsigned পূর্ণাঙ্ক অঙ্ক,
					শূন্য *in)
{
	প্রত্যাগম (ext2fs_set_generic_bitmap_range(bmap,
						EXT2_ET_MAGIC_INODE_BITMAP,
						start, অঙ্ক, in));
}

errcode_t ext2fs_set_inode_bitmap_range2(ext2fs_inode_bitmap bmap,
					 __u64 start, size_t অঙ্ক,
					 শূন্য *in)
{
	প্রত্যাগম (ext2fs_set_generic_bmap_range(bmap, start, অঙ্ক, in));
}

errcode_t ext2fs_get_inode_bitmap_range(ext2fs_inode_bitmap bmap,
					ext2_ino_t start, unsigned পূর্ণাঙ্ক অঙ্ক,
					শূন্য *out)
{
	প্রত্যাগম (ext2fs_get_generic_bitmap_range(bmap,
						EXT2_ET_MAGIC_INODE_BITMAP,
						start, অঙ্ক, out));
}

errcode_t ext2fs_get_inode_bitmap_range2(ext2fs_inode_bitmap bmap,
					 __u64 start, size_t অঙ্ক,
					 শূন্য *out)
{
	প্রত্যাগম (ext2fs_get_generic_bmap_range(bmap, start, অঙ্ক, out));
}

errcode_t ext2fs_set_block_bitmap_range(ext2fs_block_bitmap bmap,
					blk_t start, unsigned পূর্ণাঙ্ক অঙ্ক,
					শূন্য *in)
{
	প্রত্যাগম (ext2fs_set_generic_bitmap_range(bmap,
						EXT2_ET_MAGIC_BLOCK_BITMAP,
						start, অঙ্ক, in));
}

errcode_t ext2fs_set_block_bitmap_range2(ext2fs_block_bitmap bmap,
					 blk64_t start, size_t অঙ্ক,
					 শূন্য *in)
{
	প্রত্যাগম (ext2fs_set_generic_bmap_range(bmap, start, অঙ্ক, in));
}

errcode_t ext2fs_get_block_bitmap_range(ext2fs_block_bitmap bmap,
					blk_t start, unsigned পূর্ণাঙ্ক অঙ্ক,
					শূন্য *out)
{
	প্রত্যাগম (ext2fs_get_generic_bitmap_range(bmap,
						EXT2_ET_MAGIC_BLOCK_BITMAP,
						start, অঙ্ক, out));
}

errcode_t ext2fs_get_block_bitmap_range2(ext2fs_block_bitmap bmap,
					 blk64_t start, size_t অঙ্ক,
					 শূন্য *out)
{
	প্রত্যাগম (ext2fs_get_generic_bmap_range(bmap, start, অঙ্ক, out));
}
