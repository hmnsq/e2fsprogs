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

સંરચના ext2_bmap_statistics {
	પૂર્ણાંક		type;
	સંરચના timeval	created;

#ifdef ENABLE_BMAP_STATS_OPS
	unsigned long	copy_ગણતરી;
	unsigned long	resize_ગણતરી;
	unsigned long	mark_ગણતરી;
	unsigned long	unmark_ગણતરી;
	unsigned long	test_ગણતરી;
	unsigned long	mark_ext_ગણતરી;
	unsigned long	unmark_ext_ગણતરી;
	unsigned long	test_ext_ગણતરી;
	unsigned long	set_range_ગણતરી;
	unsigned long	get_range_ગણતરી;
	unsigned long	clear_ગણતરી;

	blk64_t		last_marked;
	blk64_t		last_tested;
	blk64_t		mark_back;
	blk64_t		test_back;

	unsigned long	mark_seq;
	unsigned long	test_seq;
#endif /* ENABLE_BMAP_STATS_OPS */
};


સંરચના ext2fs_struct_generic_bitmap_64 {
	errcode_t		magic;
	ext2_filsys 		fs;
	સંરચના ext2_bitmap_ops	*bitmap_ops;
	પૂર્ણાંક			flags;
	__u64			start, end;
	__u64			real_end;
	પૂર્ણાંક			cluster_bits;
	અક્ષર			*description;
	શૂન્ય			*ખાનગી;
	errcode_t		base_error_code;
#ifdef ENABLE_BMAP_STATS
	સંરચના ext2_bmap_statistics	stats;
#endif
};

પ્રકારવ્યાખ્યા સંરચના ext2fs_struct_generic_bitmap_64 *ext2fs_generic_bitmap_64;

#define EXT2FS_IS_32_BITMAP(bmap) \
	(((bmap)->magic == EXT2_ET_MAGIC_GENERIC_BITMAP) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_BLOCK_BITMAP) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_INODE_BITMAP))

#define EXT2FS_IS_64_BITMAP(bmap) \
	(((bmap)->magic == EXT2_ET_MAGIC_GENERIC_BITMAP64) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_BLOCK_BITMAP64) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_INODE_BITMAP64))

સંરચના ext2_bitmap_ops {
	પૂર્ણાંક	type;
	/* Generic bmap operators */
	errcode_t (*new_bmap)(ext2_filsys fs, ext2fs_generic_bitmap_64 bmap);
	શૂન્ય	(*free_bmap)(ext2fs_generic_bitmap_64 bitmap);
	errcode_t (*copy_bmap)(ext2fs_generic_bitmap_64 src,
			     ext2fs_generic_bitmap_64 dest);
	errcode_t (*resize_bmap)(ext2fs_generic_bitmap_64 bitmap,
			       __u64 new_end,
			       __u64 new_real_end);
	/* bit set/test operators */
	પૂર્ણાંક	(*mark_bmap)(ext2fs_generic_bitmap_64 bitmap, __u64 arg);
	પૂર્ણાંક	(*unmark_bmap)(ext2fs_generic_bitmap_64 bitmap, __u64 arg);
	પૂર્ણાંક	(*test_bmap)(ext2fs_generic_bitmap_64 bitmap, __u64 arg);
	શૂન્ય	(*mark_bmap_extent)(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				    unsigned પૂર્ણાંક અંક);
	શૂન્ય	(*unmark_bmap_extent)(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				      unsigned પૂર્ણાંક અંક);
	પૂર્ણાંક	(*test_clear_bmap_extent)(ext2fs_generic_bitmap_64 bitmap,
					  __u64 arg, unsigned પૂર્ણાંક અંક);
	errcode_t (*set_bmap_range)(ext2fs_generic_bitmap_64 bitmap,
				    __u64 start, size_t અંક, શૂન્ય *in);
	errcode_t (*get_bmap_range)(ext2fs_generic_bitmap_64 bitmap,
				    __u64 start, size_t અંક, શૂન્ય *out);
	શૂન્ય (*clear_bmap)(ext2fs_generic_bitmap_64 bitmap);
	શૂન્ય (*print_stats)(ext2fs_generic_bitmap_64);

	/* Find the first zero bit between start and end, inclusive.
	 * May be NULL, in which case a generic function is used. */
	errcode_t (*find_first_zero)(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, __u64 end, __u64 *out);
	/* Find the first set bit between start and end, inclusive.
	 * May be NULL, in which case a generic function is used. */
	errcode_t (*find_first_set)(ext2fs_generic_bitmap_64 bitmap,
				    __u64 start, __u64 end, __u64 *out);
};

બાહ્ય સંરચના ext2_bitmap_ops ext2fs_blkmap64_bitarray;
બાહ્ય સંરચના ext2_bitmap_ops ext2fs_blkmap64_rbtree;
