/*
 * bmap64.h --- 64-bit bitmap structure
 *
 * Copyright (C) 2007, 2008 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Public
 * License.
 * %End-Header%
 */

بنية ext2_bmap_statistics {
	صحيح		type;
	بنية timeval	created;

#اگر_تعریف_شده ENABLE_BMAP_STATS_OPS
	بیعلامت دراز	copy_شمار;
	بیعلامت دراز	resize_شمار;
	بیعلامت دراز	mark_شمار;
	بیعلامت دراز	unmark_شمار;
	بیعلامت دراز	test_شمار;
	بیعلامت دراز	mark_ext_شمار;
	بیعلامت دراز	unmark_ext_شمار;
	بیعلامت دراز	test_ext_شمار;
	بیعلامت دراز	مجموعه_range_شمار;
	بیعلامت دراز	get_range_شمار;
	بیعلامت دراز	clear_شمار;

	blk64_t		آخرین_marked;
	blk64_t		آخرین_tested;
	blk64_t		mark_پشت;
	blk64_t		test_پشت;

	بیعلامت دراز	mark_seq;
	بیعلامت دراز	test_seq;
#پایان_اگر /* ENABLE_BMAP_STATS_OPS */
};


بنية ext2fs_struct_generic_bitmap_64 {
	errcode_t		magic;
	ext2_filsys 		fs;
	بنية ext2_bitmap_ops	*bitmap_ops;
	صحيح			flags;
	__u64			شروع, انتها;
	__u64			real_انتها;
	صحيح			cluster_بیتها;
	حرف			*description;
	فراغ			*خاص;
	errcode_t		پایه_error_کد;
#اگر_تعریف_شده ENABLE_BMAP_STATS
	بنية ext2_bmap_statistics	stats;
#پایان_اگر
};

تعريفنوع بنية ext2fs_struct_generic_bitmap_64 *ext2fs_generic_bitmap_64;

#تعریف_کن EXT2FS_IS_32_BITMAP(bmap) \
	(((bmap)->magic == EXT2_ET_MAGIC_GENERIC_BITMAP) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_بلوک_BITMAP) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_INODE_BITMAP))

#تعریف_کن EXT2FS_IS_64_BITMAP(bmap) \
	(((bmap)->magic == EXT2_ET_MAGIC_GENERIC_BITMAP64) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_بلوک_BITMAP64) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_INODE_BITMAP64))

بنية ext2_bitmap_ops {
	صحيح	type;
	/* Generic bmap operators */
	errcode_t (*new_bmap)(ext2_filsys fs, ext2fs_generic_bitmap_64 bmap);
	فراغ	(*free_bmap)(ext2fs_generic_bitmap_64 bitmap);
	errcode_t (*copy_bmap)(ext2fs_generic_bitmap_64 src,
			     ext2fs_generic_bitmap_64 dest);
	errcode_t (*resize_bmap)(ext2fs_generic_bitmap_64 bitmap,
			       __u64 new_انتها,
			       __u64 new_real_انتها);
	/* bit set/test operators */
	صحيح	(*mark_bmap)(ext2fs_generic_bitmap_64 bitmap, __u64 arg);
	صحيح	(*unmark_bmap)(ext2fs_generic_bitmap_64 bitmap, __u64 arg);
	صحيح	(*test_bmap)(ext2fs_generic_bitmap_64 bitmap, __u64 arg);
	فراغ	(*mark_bmap_extent)(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				    بیعلامت صحيح رقم);
	فراغ	(*unmark_bmap_extent)(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				      بیعلامت صحيح رقم);
	صحيح	(*test_clear_bmap_extent)(ext2fs_generic_bitmap_64 bitmap,
					  __u64 arg, بیعلامت صحيح رقم);
	errcode_t (*مجموعه_bmap_range)(ext2fs_generic_bitmap_64 bitmap,
				    __u64 شروع, size_t رقم, فراغ *درون);
	errcode_t (*get_bmap_range)(ext2fs_generic_bitmap_64 bitmap,
				    __u64 شروع, size_t رقم, فراغ *بیرون);
	فراغ (*clear_bmap)(ext2fs_generic_bitmap_64 bitmap);
	فراغ (*print_stats)(ext2fs_generic_bitmap_64);

	/* Find the first zero bit between start and end, inclusive.
	 * May be NULL, in which case a generic function is used. */
	errcode_t (*پیدا_کن_اولی_zero)(ext2fs_generic_bitmap_64 bitmap,
				     __u64 شروع, __u64 انتها, __u64 *بیرون);
	/* Find the first set bit between start and end, inclusive.
	 * May be NULL, in which case a generic function is used. */
	errcode_t (*پیدا_کن_اولی_مجموعه)(ext2fs_generic_bitmap_64 bitmap,
				    __u64 شروع, __u64 انتها, __u64 *بیرون);
};

خارجي بنية ext2_bitmap_ops ext2fs_blkmap64_bitarray;
خارجي بنية ext2_bitmap_ops ext2fs_blkmap64_rbtree;
