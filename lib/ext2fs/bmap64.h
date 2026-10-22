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

संरचना ext2_bmap_statistics {
	पूर्णाङ्क		type;
	संरचना timeval	created;

#ifdef ENABLE_BMAP_STATS_OPS
	unsigned long	copy_मोजणी;
	unsigned long	resize_मोजणी;
	unsigned long	mark_मोजणी;
	unsigned long	unmark_मोजणी;
	unsigned long	test_मोजणी;
	unsigned long	mark_ext_मोजणी;
	unsigned long	unmark_ext_मोजणी;
	unsigned long	test_ext_मोजणी;
	unsigned long	set_range_मोजणी;
	unsigned long	get_range_मोजणी;
	unsigned long	clear_मोजणी;

	blk64_t		last_marked;
	blk64_t		last_tested;
	blk64_t		mark_back;
	blk64_t		test_back;

	unsigned long	mark_seq;
	unsigned long	test_seq;
#endif /* ENABLE_BMAP_STATS_OPS */
};


संरचना ext2fs_struct_generic_bitmap_64 {
	errcode_t		magic;
	ext2_filsys 		fs;
	संरचना ext2_bitmap_ops	*bitmap_ops;
	पूर्णाङ्क			flags;
	__u64			start, end;
	__u64			real_end;
	पूर्णाङ्क			cluster_bits;
	वर्ण			*description;
	शून्य			*गुप्त;
	errcode_t		base_error_code;
#ifdef ENABLE_BMAP_STATS
	संरचना ext2_bmap_statistics	stats;
#endif
};

प्रकारनिर्देश संरचना ext2fs_struct_generic_bitmap_64 *ext2fs_generic_bitmap_64;

#define EXT2FS_IS_32_BITMAP(bmap) \
	(((bmap)->magic == EXT2_ET_MAGIC_GENERIC_BITMAP) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_BLOCK_BITMAP) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_INODE_BITMAP))

#define EXT2FS_IS_64_BITMAP(bmap) \
	(((bmap)->magic == EXT2_ET_MAGIC_GENERIC_BITMAP64) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_BLOCK_BITMAP64) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_INODE_BITMAP64))

संरचना ext2_bitmap_ops {
	पूर्णाङ्क	type;
	/* Generic bmap operators */
	errcode_t (*new_bmap)(ext2_filsys fs, ext2fs_generic_bitmap_64 bmap);
	शून्य	(*free_bmap)(ext2fs_generic_bitmap_64 bitmap);
	errcode_t (*copy_bmap)(ext2fs_generic_bitmap_64 src,
			     ext2fs_generic_bitmap_64 dest);
	errcode_t (*resize_bmap)(ext2fs_generic_bitmap_64 bitmap,
			       __u64 new_end,
			       __u64 new_real_end);
	/* bit set/test operators */
	पूर्णाङ्क	(*mark_bmap)(ext2fs_generic_bitmap_64 bitmap, __u64 arg);
	पूर्णाङ्क	(*unmark_bmap)(ext2fs_generic_bitmap_64 bitmap, __u64 arg);
	पूर्णाङ्क	(*test_bmap)(ext2fs_generic_bitmap_64 bitmap, __u64 arg);
	शून्य	(*mark_bmap_extent)(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				    unsigned पूर्णाङ्क अंक);
	शून्य	(*unmark_bmap_extent)(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				      unsigned पूर्णाङ्क अंक);
	पूर्णाङ्क	(*test_clear_bmap_extent)(ext2fs_generic_bitmap_64 bitmap,
					  __u64 arg, unsigned पूर्णाङ्क अंक);
	errcode_t (*set_bmap_range)(ext2fs_generic_bitmap_64 bitmap,
				    __u64 start, size_t अंक, शून्य *in);
	errcode_t (*get_bmap_range)(ext2fs_generic_bitmap_64 bitmap,
				    __u64 start, size_t अंक, शून्य *out);
	शून्य (*clear_bmap)(ext2fs_generic_bitmap_64 bitmap);
	शून्य (*print_stats)(ext2fs_generic_bitmap_64);

	/* Find the first zero bit between start and end, inclusive.
	 * May be NULL, in which case a generic function is used. */
	errcode_t (*find_first_zero)(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, __u64 end, __u64 *out);
	/* Find the first set bit between start and end, inclusive.
	 * May be NULL, in which case a generic function is used. */
	errcode_t (*find_first_set)(ext2fs_generic_bitmap_64 bitmap,
				    __u64 start, __u64 end, __u64 *out);
};

बाह्य संरचना ext2_bitmap_ops ext2fs_blkmap64_bitarray;
बाह्य संरचना ext2_bitmap_ops ext2fs_blkmap64_rbtree;
