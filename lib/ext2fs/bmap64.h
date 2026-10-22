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

ರಚನೆ ext2_bmap_statistics {
	ಪೂರ್ಣಾಂಕ		type;
	ರಚನೆ timeval	created;

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ENABLE_BMAP_STATS_OPS
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ	copy_ಎಣಿಕೆ;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ	resize_ಎಣಿಕೆ;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ	mark_ಎಣಿಕೆ;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ	unmark_ಎಣಿಕೆ;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ	test_ಎಣಿಕೆ;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ	mark_ext_ಎಣಿಕೆ;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ	unmark_ext_ಎಣಿಕೆ;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ	test_ext_ಎಣಿಕೆ;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ	ಗಣ_range_ಎಣಿಕೆ;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ	get_range_ಎಣಿಕೆ;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ	clear_ಎಣಿಕೆ;

	blk64_t		ಕೊನೆಯ_marked;
	blk64_t		ಕೊನೆಯ_tested;
	blk64_t		mark_ಹಿಂಭಾಗ;
	blk64_t		test_ಹಿಂಭಾಗ;

	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ	mark_seq;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ	test_seq;
#ಆದರೆ_ಅಂತ್ಯ /* ENABLE_BMAP_STATS_OPS */
};


ರಚನೆ ext2fs_struct_generic_bitmap_64 {
	errcode_t		magic;
	ext2_filsys 		fs;
	ರಚನೆ ext2_bitmap_ops	*bitmap_ops;
	ಪೂರ್ಣಾಂಕ			flags;
	__u64			ಆರಂಭ, ಕೊನೆ;
	__u64			real_ಕೊನೆ;
	ಪೂರ್ಣಾಂಕ			cluster_ಬಿಟ್ಟುಗಳು;
	ಅಕ್ಷರ			*description;
	ಶೂನ್ಯ			*ಖಾಸಗಿ;
	errcode_t		ತಳ_error_ಸಂಕೇತ;
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ENABLE_BMAP_STATS
	ರಚನೆ ext2_bmap_statistics	stats;
#ಆದರೆ_ಅಂತ್ಯ
};

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ ext2fs_struct_generic_bitmap_64 *ext2fs_generic_bitmap_64;

#ವ್ಯಾಖ್ಯಾನಿಸು EXT2FS_IS_32_BITMAP(bmap) \
	(((bmap)->magic == EXT2_ET_MAGIC_GENERIC_BITMAP) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_ಖಂಡ_BITMAP) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_INODE_BITMAP))

#ವ್ಯಾಖ್ಯಾನಿಸು EXT2FS_IS_64_BITMAP(bmap) \
	(((bmap)->magic == EXT2_ET_MAGIC_GENERIC_BITMAP64) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_ಖಂಡ_BITMAP64) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_INODE_BITMAP64))

ರಚನೆ ext2_bitmap_ops {
	ಪೂರ್ಣಾಂಕ	type;
	/* Generic bmap operators */
	errcode_t (*new_bmap)(ext2_filsys fs, ext2fs_generic_bitmap_64 bmap);
	ಶೂನ್ಯ	(*free_bmap)(ext2fs_generic_bitmap_64 bitmap);
	errcode_t (*copy_bmap)(ext2fs_generic_bitmap_64 src,
			     ext2fs_generic_bitmap_64 dest);
	errcode_t (*resize_bmap)(ext2fs_generic_bitmap_64 bitmap,
			       __u64 new_ಕೊನೆ,
			       __u64 new_real_ಕೊನೆ);
	/* bit set/test operators */
	ಪೂರ್ಣಾಂಕ	(*mark_bmap)(ext2fs_generic_bitmap_64 bitmap, __u64 arg);
	ಪೂರ್ಣಾಂಕ	(*unmark_bmap)(ext2fs_generic_bitmap_64 bitmap, __u64 arg);
	ಪೂರ್ಣಾಂಕ	(*test_bmap)(ext2fs_generic_bitmap_64 bitmap, __u64 arg);
	ಶೂನ್ಯ	(*mark_bmap_extent)(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				    ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಅಂಕೆ);
	ಶೂನ್ಯ	(*unmark_bmap_extent)(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				      ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಅಂಕೆ);
	ಪೂರ್ಣಾಂಕ	(*test_clear_bmap_extent)(ext2fs_generic_bitmap_64 bitmap,
					  __u64 arg, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಅಂಕೆ);
	errcode_t (*ಗಣ_bmap_range)(ext2fs_generic_bitmap_64 bitmap,
				    __u64 ಆರಂಭ, size_t ಅಂಕೆ, ಶೂನ್ಯ *ಒಳ);
	errcode_t (*get_bmap_range)(ext2fs_generic_bitmap_64 bitmap,
				    __u64 ಆರಂಭ, size_t ಅಂಕೆ, ಶೂನ್ಯ *ಹೊರಹೋಗು);
	ಶೂನ್ಯ (*clear_bmap)(ext2fs_generic_bitmap_64 bitmap);
	ಶೂನ್ಯ (*print_stats)(ext2fs_generic_bitmap_64);

	/* Find the first zero bit between start and end, inclusive.
	 * May be NULL, in which case a generic function is used. */
	errcode_t (*ಕಂಡುಹಿಡಿ_ಮೊದಲ_zero)(ext2fs_generic_bitmap_64 bitmap,
				     __u64 ಆರಂಭ, __u64 ಕೊನೆ, __u64 *ಹೊರಹೋಗು);
	/* Find the first set bit between start and end, inclusive.
	 * May be NULL, in which case a generic function is used. */
	errcode_t (*ಕಂಡುಹಿಡಿ_ಮೊದಲ_ಗಣ)(ext2fs_generic_bitmap_64 bitmap,
				    __u64 ಆರಂಭ, __u64 ಕೊನೆ, __u64 *ಹೊರಹೋಗು);
};

ಬಾಹ್ಯ ರಚನೆ ext2_bitmap_ops ext2fs_blkmap64_bitarray;
ಬಾಹ್ಯ ರಚನೆ ext2_bitmap_ops ext2fs_blkmap64_rbtree;
