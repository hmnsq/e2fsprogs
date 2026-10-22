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

கட்டமைப்பு ext2_bmap_statistics {
	முழு		type;
	கட்டமைப்பு timeval	created;

#ifdef ENABLE_BMAP_STATS_OPS
	unsigned long	copy_மொத்தம்;
	unsigned long	resize_மொத்தம்;
	unsigned long	mark_மொத்தம்;
	unsigned long	unmark_மொத்தம்;
	unsigned long	test_மொத்தம்;
	unsigned long	mark_ext_மொத்தம்;
	unsigned long	unmark_ext_மொத்தம்;
	unsigned long	test_ext_மொத்தம்;
	unsigned long	set_range_மொத்தம்;
	unsigned long	get_range_மொத்தம்;
	unsigned long	clear_மொத்தம்;

	blk64_t		last_marked;
	blk64_t		last_tested;
	blk64_t		mark_back;
	blk64_t		test_back;

	unsigned long	mark_seq;
	unsigned long	test_seq;
#endif /* ENABLE_BMAP_STATS_OPS */
};


கட்டமைப்பு ext2fs_struct_generic_bitmap_64 {
	errcode_t		magic;
	ext2_filsys 		fs;
	கட்டமைப்பு ext2_bitmap_ops	*bitmap_ops;
	முழு			flags;
	__u64			start, end;
	__u64			real_end;
	முழு			cluster_bits;
	எழுத்து			*description;
	வெறுமை			*தனிப்பட்ட;
	errcode_t		base_error_code;
#ifdef ENABLE_BMAP_STATS
	கட்டமைப்பு ext2_bmap_statistics	stats;
#endif
};

வகைவரை கட்டமைப்பு ext2fs_struct_generic_bitmap_64 *ext2fs_generic_bitmap_64;

#define EXT2FS_IS_32_BITMAP(bmap) \
	(((bmap)->magic == EXT2_ET_MAGIC_GENERIC_BITMAP) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_BLOCK_BITMAP) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_INODE_BITMAP))

#define EXT2FS_IS_64_BITMAP(bmap) \
	(((bmap)->magic == EXT2_ET_MAGIC_GENERIC_BITMAP64) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_BLOCK_BITMAP64) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_INODE_BITMAP64))

கட்டமைப்பு ext2_bitmap_ops {
	முழு	type;
	/* Generic bmap operators */
	errcode_t (*new_bmap)(ext2_filsys fs, ext2fs_generic_bitmap_64 bmap);
	வெறுமை	(*free_bmap)(ext2fs_generic_bitmap_64 bitmap);
	errcode_t (*copy_bmap)(ext2fs_generic_bitmap_64 src,
			     ext2fs_generic_bitmap_64 dest);
	errcode_t (*resize_bmap)(ext2fs_generic_bitmap_64 bitmap,
			       __u64 new_end,
			       __u64 new_real_end);
	/* bit set/test operators */
	முழு	(*mark_bmap)(ext2fs_generic_bitmap_64 bitmap, __u64 arg);
	முழு	(*unmark_bmap)(ext2fs_generic_bitmap_64 bitmap, __u64 arg);
	முழு	(*test_bmap)(ext2fs_generic_bitmap_64 bitmap, __u64 arg);
	வெறுமை	(*mark_bmap_extent)(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				    unsigned முழு இலக்கம்);
	வெறுமை	(*unmark_bmap_extent)(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				      unsigned முழு இலக்கம்);
	முழு	(*test_clear_bmap_extent)(ext2fs_generic_bitmap_64 bitmap,
					  __u64 arg, unsigned முழு இலக்கம்);
	errcode_t (*set_bmap_range)(ext2fs_generic_bitmap_64 bitmap,
				    __u64 start, size_t இலக்கம், வெறுமை *in);
	errcode_t (*get_bmap_range)(ext2fs_generic_bitmap_64 bitmap,
				    __u64 start, size_t இலக்கம், வெறுமை *out);
	வெறுமை (*clear_bmap)(ext2fs_generic_bitmap_64 bitmap);
	வெறுமை (*print_stats)(ext2fs_generic_bitmap_64);

	/* Find the first zero bit between start and end, inclusive.
	 * May be NULL, in which case a generic function is used. */
	errcode_t (*find_first_zero)(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, __u64 end, __u64 *out);
	/* Find the first set bit between start and end, inclusive.
	 * May be NULL, in which case a generic function is used. */
	errcode_t (*find_first_set)(ext2fs_generic_bitmap_64 bitmap,
				    __u64 start, __u64 end, __u64 *out);
};

வெளி கட்டமைப்பு ext2_bitmap_ops ext2fs_blkmap64_bitarray;
வெளி கட்டமைப்பு ext2_bitmap_ops ext2fs_blkmap64_rbtree;
