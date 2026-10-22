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

ಸಂರಚನಾ ext2_bmap_statistics {
	ಪೂರ್ಣಾಙ್ಕ		type;
	ಸಂರಚನಾ timeval	created;

#ifdef ENABLE_BMAP_STATS_OPS
	unsigned long	copy_ಎಣಿಕೆ;
	unsigned long	resize_ಎಣಿಕೆ;
	unsigned long	mark_ಎಣಿಕೆ;
	unsigned long	unmark_ಎಣಿಕೆ;
	unsigned long	test_ಎಣಿಕೆ;
	unsigned long	mark_ext_ಎಣಿಕೆ;
	unsigned long	unmark_ext_ಎಣಿಕೆ;
	unsigned long	test_ext_ಎಣಿಕೆ;
	unsigned long	set_range_ಎಣಿಕೆ;
	unsigned long	get_range_ಎಣಿಕೆ;
	unsigned long	clear_ಎಣಿಕೆ;

	blk64_t		last_marked;
	blk64_t		last_tested;
	blk64_t		mark_back;
	blk64_t		test_back;

	unsigned long	mark_seq;
	unsigned long	test_seq;
#endif /* ENABLE_BMAP_STATS_OPS */
};


ಸಂರಚನಾ ext2fs_struct_generic_bitmap_64 {
	errcode_t		magic;
	ext2_filsys 		fs;
	ಸಂರಚನಾ ext2_bitmap_ops	*bitmap_ops;
	ಪೂರ್ಣಾಙ್ಕ			flags;
	__u64			start, end;
	__u64			real_end;
	ಪೂರ್ಣಾಙ್ಕ			cluster_bits;
	ವರ್ಣ			*description;
	ಶೂನ್ಯ			*ಗುಪ್ತ;
	errcode_t		base_error_code;
#ifdef ENABLE_BMAP_STATS
	ಸಂರಚನಾ ext2_bmap_statistics	stats;
#endif
};

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ ext2fs_struct_generic_bitmap_64 *ext2fs_generic_bitmap_64;

#define EXT2FS_IS_32_BITMAP(bmap) \
	(((bmap)->magic == EXT2_ET_MAGIC_GENERIC_BITMAP) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_BLOCK_BITMAP) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_INODE_BITMAP))

#define EXT2FS_IS_64_BITMAP(bmap) \
	(((bmap)->magic == EXT2_ET_MAGIC_GENERIC_BITMAP64) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_BLOCK_BITMAP64) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_INODE_BITMAP64))

ಸಂರಚನಾ ext2_bitmap_ops {
	ಪೂರ್ಣಾಙ್ಕ	type;
	/* Generic bmap operators */
	errcode_t (*new_bmap)(ext2_filsys fs, ext2fs_generic_bitmap_64 bmap);
	ಶೂನ್ಯ	(*free_bmap)(ext2fs_generic_bitmap_64 bitmap);
	errcode_t (*copy_bmap)(ext2fs_generic_bitmap_64 src,
			     ext2fs_generic_bitmap_64 dest);
	errcode_t (*resize_bmap)(ext2fs_generic_bitmap_64 bitmap,
			       __u64 new_end,
			       __u64 new_real_end);
	/* bit set/test operators */
	ಪೂರ್ಣಾಙ್ಕ	(*mark_bmap)(ext2fs_generic_bitmap_64 bitmap, __u64 arg);
	ಪೂರ್ಣಾಙ್ಕ	(*unmark_bmap)(ext2fs_generic_bitmap_64 bitmap, __u64 arg);
	ಪೂರ್ಣಾಙ್ಕ	(*test_bmap)(ext2fs_generic_bitmap_64 bitmap, __u64 arg);
	ಶೂನ್ಯ	(*mark_bmap_extent)(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				    unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ);
	ಶೂನ್ಯ	(*unmark_bmap_extent)(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				      unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ);
	ಪೂರ್ಣಾಙ್ಕ	(*test_clear_bmap_extent)(ext2fs_generic_bitmap_64 bitmap,
					  __u64 arg, unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ);
	errcode_t (*set_bmap_range)(ext2fs_generic_bitmap_64 bitmap,
				    __u64 start, size_t ಅಂಕೆ, ಶೂನ್ಯ *in);
	errcode_t (*get_bmap_range)(ext2fs_generic_bitmap_64 bitmap,
				    __u64 start, size_t ಅಂಕೆ, ಶೂನ್ಯ *out);
	ಶೂನ್ಯ (*clear_bmap)(ext2fs_generic_bitmap_64 bitmap);
	ಶೂನ್ಯ (*print_stats)(ext2fs_generic_bitmap_64);

	/* Find the first zero bit between start and end, inclusive.
	 * May be NULL, in which case a generic function is used. */
	errcode_t (*find_first_zero)(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, __u64 end, __u64 *out);
	/* Find the first set bit between start and end, inclusive.
	 * May be NULL, in which case a generic function is used. */
	errcode_t (*find_first_set)(ext2fs_generic_bitmap_64 bitmap,
				    __u64 start, __u64 end, __u64 *out);
};

ಬಾಹ್ಯ ಸಂರಚನಾ ext2_bitmap_ops ext2fs_blkmap64_bitarray;
ಬಾಹ್ಯ ಸಂರಚನಾ ext2_bitmap_ops ext2fs_blkmap64_rbtree;
