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

ସଂରଚନା ext2_bmap_statistics {
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା		type;
	ସଂରଚନା timeval	created;

#ifdef ENABLE_BMAP_STATS_OPS
	unsigned long	copy_ଗଣନା;
	unsigned long	resize_ଗଣନା;
	unsigned long	mark_ଗଣନା;
	unsigned long	unmark_ଗଣନା;
	unsigned long	test_ଗଣନା;
	unsigned long	mark_ext_ଗଣନା;
	unsigned long	unmark_ext_ଗଣନା;
	unsigned long	test_ext_ଗଣନା;
	unsigned long	set_range_ଗଣନା;
	unsigned long	get_range_ଗଣନା;
	unsigned long	clear_ଗଣନା;

	blk64_t		last_marked;
	blk64_t		last_tested;
	blk64_t		mark_back;
	blk64_t		test_back;

	unsigned long	mark_seq;
	unsigned long	test_seq;
#endif /* ENABLE_BMAP_STATS_OPS */
};


ସଂରଚନା ext2fs_struct_generic_bitmap_64 {
	errcode_t		magic;
	ext2_filsys 		fs;
	ସଂରଚନା ext2_bitmap_ops	*bitmap_ops;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			flags;
	__u64			start, end;
	__u64			real_end;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା			cluster_bits;
	ଅକ୍ଷର			*description;
	ଶୂନ୍ୟ			*ବ୍ୟକ୍ତିଗତ;
	errcode_t		base_error_code;
#ifdef ENABLE_BMAP_STATS
	ସଂରଚନା ext2_bmap_statistics	stats;
#endif
};

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା ext2fs_struct_generic_bitmap_64 *ext2fs_generic_bitmap_64;

#define EXT2FS_IS_32_BITMAP(bmap) \
	(((bmap)->magic == EXT2_ET_MAGIC_GENERIC_BITMAP) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_BLOCK_BITMAP) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_INODE_BITMAP))

#define EXT2FS_IS_64_BITMAP(bmap) \
	(((bmap)->magic == EXT2_ET_MAGIC_GENERIC_BITMAP64) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_BLOCK_BITMAP64) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_INODE_BITMAP64))

ସଂରଚନା ext2_bitmap_ops {
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା	type;
	/* Generic bmap operators */
	errcode_t (*new_bmap)(ext2_filsys fs, ext2fs_generic_bitmap_64 bmap);
	ଶୂନ୍ୟ	(*free_bmap)(ext2fs_generic_bitmap_64 bitmap);
	errcode_t (*copy_bmap)(ext2fs_generic_bitmap_64 src,
			     ext2fs_generic_bitmap_64 dest);
	errcode_t (*resize_bmap)(ext2fs_generic_bitmap_64 bitmap,
			       __u64 new_end,
			       __u64 new_real_end);
	/* bit set/test operators */
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା	(*mark_bmap)(ext2fs_generic_bitmap_64 bitmap, __u64 arg);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା	(*unmark_bmap)(ext2fs_generic_bitmap_64 bitmap, __u64 arg);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା	(*test_bmap)(ext2fs_generic_bitmap_64 bitmap, __u64 arg);
	ଶୂନ୍ୟ	(*mark_bmap_extent)(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				    unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ);
	ଶୂନ୍ୟ	(*unmark_bmap_extent)(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				      unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ);
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା	(*test_clear_bmap_extent)(ext2fs_generic_bitmap_64 bitmap,
					  __u64 arg, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ);
	errcode_t (*set_bmap_range)(ext2fs_generic_bitmap_64 bitmap,
				    __u64 start, size_t ଅଙ୍କ, ଶୂନ୍ୟ *in);
	errcode_t (*get_bmap_range)(ext2fs_generic_bitmap_64 bitmap,
				    __u64 start, size_t ଅଙ୍କ, ଶୂନ୍ୟ *out);
	ଶୂନ୍ୟ (*clear_bmap)(ext2fs_generic_bitmap_64 bitmap);
	ଶୂନ୍ୟ (*print_stats)(ext2fs_generic_bitmap_64);

	/* Find the first zero bit between start and end, inclusive.
	 * May be NULL, in which case a generic function is used. */
	errcode_t (*find_first_zero)(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, __u64 end, __u64 *out);
	/* Find the first set bit between start and end, inclusive.
	 * May be NULL, in which case a generic function is used. */
	errcode_t (*find_first_set)(ext2fs_generic_bitmap_64 bitmap,
				    __u64 start, __u64 end, __u64 *out);
};

ବାହ୍ୟ ସଂରଚନା ext2_bitmap_ops ext2fs_blkmap64_bitarray;
ବାହ୍ୟ ସଂରଚନା ext2_bitmap_ops ext2fs_blkmap64_rbtree;
