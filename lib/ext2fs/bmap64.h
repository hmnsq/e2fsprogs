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

সংরচনা ext2_bmap_statistics {
	পূর্ণাঙ্ক		type;
	সংরচনা timeval	created;

#ifdef ENABLE_BMAP_STATS_OPS
	unsigned long	copy_গুনতি;
	unsigned long	resize_গুনতি;
	unsigned long	mark_গুনতি;
	unsigned long	unmark_গুনতি;
	unsigned long	test_গুনতি;
	unsigned long	mark_ext_গুনতি;
	unsigned long	unmark_ext_গুনতি;
	unsigned long	test_ext_গুনতি;
	unsigned long	set_range_গুনতি;
	unsigned long	get_range_গুনতি;
	unsigned long	clear_গুনতি;

	blk64_t		last_marked;
	blk64_t		last_tested;
	blk64_t		mark_back;
	blk64_t		test_back;

	unsigned long	mark_seq;
	unsigned long	test_seq;
#endif /* ENABLE_BMAP_STATS_OPS */
};


সংরচনা ext2fs_struct_generic_bitmap_64 {
	errcode_t		magic;
	ext2_filsys 		fs;
	সংরচনা ext2_bitmap_ops	*bitmap_ops;
	পূর্ণাঙ্ক			flags;
	__u64			start, end;
	__u64			real_end;
	পূর্ণাঙ্ক			cluster_bits;
	ৱর্ণ			*description;
	শূন্য			*গুপ্ত;
	errcode_t		base_error_code;
#ifdef ENABLE_BMAP_STATS
	সংরচনা ext2_bmap_statistics	stats;
#endif
};

প্রকারনির্দেশ সংরচনা ext2fs_struct_generic_bitmap_64 *ext2fs_generic_bitmap_64;

#define EXT2FS_IS_32_BITMAP(bmap) \
	(((bmap)->magic == EXT2_ET_MAGIC_GENERIC_BITMAP) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_BLOCK_BITMAP) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_INODE_BITMAP))

#define EXT2FS_IS_64_BITMAP(bmap) \
	(((bmap)->magic == EXT2_ET_MAGIC_GENERIC_BITMAP64) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_BLOCK_BITMAP64) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_INODE_BITMAP64))

সংরচনা ext2_bitmap_ops {
	পূর্ণাঙ্ক	type;
	/* Generic bmap operators */
	errcode_t (*new_bmap)(ext2_filsys fs, ext2fs_generic_bitmap_64 bmap);
	শূন্য	(*free_bmap)(ext2fs_generic_bitmap_64 bitmap);
	errcode_t (*copy_bmap)(ext2fs_generic_bitmap_64 src,
			     ext2fs_generic_bitmap_64 dest);
	errcode_t (*resize_bmap)(ext2fs_generic_bitmap_64 bitmap,
			       __u64 new_end,
			       __u64 new_real_end);
	/* bit set/test operators */
	পূর্ণাঙ্ক	(*mark_bmap)(ext2fs_generic_bitmap_64 bitmap, __u64 arg);
	পূর্ণাঙ্ক	(*unmark_bmap)(ext2fs_generic_bitmap_64 bitmap, __u64 arg);
	পূর্ণাঙ্ক	(*test_bmap)(ext2fs_generic_bitmap_64 bitmap, __u64 arg);
	শূন্য	(*mark_bmap_extent)(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				    unsigned পূর্ণাঙ্ক অঙ্ক);
	শূন্য	(*unmark_bmap_extent)(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				      unsigned পূর্ণাঙ্ক অঙ্ক);
	পূর্ণাঙ্ক	(*test_clear_bmap_extent)(ext2fs_generic_bitmap_64 bitmap,
					  __u64 arg, unsigned পূর্ণাঙ্ক অঙ্ক);
	errcode_t (*set_bmap_range)(ext2fs_generic_bitmap_64 bitmap,
				    __u64 start, size_t অঙ্ক, শূন্য *in);
	errcode_t (*get_bmap_range)(ext2fs_generic_bitmap_64 bitmap,
				    __u64 start, size_t অঙ্ক, শূন্য *out);
	শূন্য (*clear_bmap)(ext2fs_generic_bitmap_64 bitmap);
	শূন্য (*print_stats)(ext2fs_generic_bitmap_64);

	/* Find the first zero bit between start and end, inclusive.
	 * May be NULL, in which case a generic function is used. */
	errcode_t (*find_first_zero)(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, __u64 end, __u64 *out);
	/* Find the first set bit between start and end, inclusive.
	 * May be NULL, in which case a generic function is used. */
	errcode_t (*find_first_set)(ext2fs_generic_bitmap_64 bitmap,
				    __u64 start, __u64 end, __u64 *out);
};

বাহ্য সংরচনা ext2_bitmap_ops ext2fs_blkmap64_bitarray;
বাহ্য সংরচনা ext2_bitmap_ops ext2fs_blkmap64_rbtree;
