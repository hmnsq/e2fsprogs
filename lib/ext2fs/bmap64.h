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

ساختار ext2_bmap_statistics {
	صحیح		type;
	ساختار timeval	created;

#ifdef ENABLE_BMAP_STATS_OPS
	unsigned long	copy_شمار;
	unsigned long	resize_شمار;
	unsigned long	mark_شمار;
	unsigned long	unmark_شمار;
	unsigned long	test_شمار;
	unsigned long	mark_ext_شمار;
	unsigned long	unmark_ext_شمار;
	unsigned long	test_ext_شمار;
	unsigned long	set_range_شمار;
	unsigned long	get_range_شمار;
	unsigned long	clear_شمار;

	blk64_t		last_marked;
	blk64_t		last_tested;
	blk64_t		mark_back;
	blk64_t		test_back;

	unsigned long	mark_seq;
	unsigned long	test_seq;
#endif /* ENABLE_BMAP_STATS_OPS */
};


ساختار ext2fs_struct_generic_bitmap_64 {
	errcode_t		magic;
	ext2_filsys 		fs;
	ساختار ext2_bitmap_ops	*bitmap_ops;
	صحیح			flags;
	__u64			start, end;
	__u64			real_end;
	صحیح			cluster_bits;
	نویسه			*description;
	تهی			*خصوصی;
	errcode_t		base_error_code;
#ifdef ENABLE_BMAP_STATS
	ساختار ext2_bmap_statistics	stats;
#endif
};

تعریفنوع ساختار ext2fs_struct_generic_bitmap_64 *ext2fs_generic_bitmap_64;

#define EXT2FS_IS_32_BITMAP(bmap) \
	(((bmap)->magic == EXT2_ET_MAGIC_GENERIC_BITMAP) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_BLOCK_BITMAP) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_INODE_BITMAP))

#define EXT2FS_IS_64_BITMAP(bmap) \
	(((bmap)->magic == EXT2_ET_MAGIC_GENERIC_BITMAP64) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_BLOCK_BITMAP64) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_INODE_BITMAP64))

ساختار ext2_bitmap_ops {
	صحیح	type;
	/* Generic bmap operators */
	errcode_t (*new_bmap)(ext2_filsys fs, ext2fs_generic_bitmap_64 bmap);
	تهی	(*free_bmap)(ext2fs_generic_bitmap_64 bitmap);
	errcode_t (*copy_bmap)(ext2fs_generic_bitmap_64 src,
			     ext2fs_generic_bitmap_64 dest);
	errcode_t (*resize_bmap)(ext2fs_generic_bitmap_64 bitmap,
			       __u64 new_end,
			       __u64 new_real_end);
	/* bit set/test operators */
	صحیح	(*mark_bmap)(ext2fs_generic_bitmap_64 bitmap, __u64 arg);
	صحیح	(*unmark_bmap)(ext2fs_generic_bitmap_64 bitmap, __u64 arg);
	صحیح	(*test_bmap)(ext2fs_generic_bitmap_64 bitmap, __u64 arg);
	تهی	(*mark_bmap_extent)(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				    unsigned صحیح رقم);
	تهی	(*unmark_bmap_extent)(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				      unsigned صحیح رقم);
	صحیح	(*test_clear_bmap_extent)(ext2fs_generic_bitmap_64 bitmap,
					  __u64 arg, unsigned صحیح رقم);
	errcode_t (*set_bmap_range)(ext2fs_generic_bitmap_64 bitmap,
				    __u64 start, size_t رقم, تهی *in);
	errcode_t (*get_bmap_range)(ext2fs_generic_bitmap_64 bitmap,
				    __u64 start, size_t رقم, تهی *out);
	تهی (*clear_bmap)(ext2fs_generic_bitmap_64 bitmap);
	تهی (*print_stats)(ext2fs_generic_bitmap_64);

	/* Find the first zero bit between start and end, inclusive.
	 * May be NULL, in which case a generic function is used. */
	errcode_t (*find_first_zero)(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, __u64 end, __u64 *out);
	/* Find the first set bit between start and end, inclusive.
	 * May be NULL, in which case a generic function is used. */
	errcode_t (*find_first_set)(ext2fs_generic_bitmap_64 bitmap,
				    __u64 start, __u64 end, __u64 *out);
};

بیرونی ساختار ext2_bitmap_ops ext2fs_blkmap64_bitarray;
بیرونی ساختار ext2_bitmap_ops ext2fs_blkmap64_rbtree;
