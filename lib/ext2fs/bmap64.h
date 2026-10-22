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

#ifdef ENABLE_BMAP_STATS_OPS
	unsigned long	copy_گنتی;
	unsigned long	resize_گنتی;
	unsigned long	mark_گنتی;
	unsigned long	unmark_گنتی;
	unsigned long	test_گنتی;
	unsigned long	mark_ext_گنتی;
	unsigned long	unmark_ext_گنتی;
	unsigned long	test_ext_گنتی;
	unsigned long	set_range_گنتی;
	unsigned long	get_range_گنتی;
	unsigned long	clear_گنتی;

	blk64_t		last_marked;
	blk64_t		last_tested;
	blk64_t		mark_back;
	blk64_t		test_back;

	unsigned long	mark_seq;
	unsigned long	test_seq;
#endif /* ENABLE_BMAP_STATS_OPS */
};


بنية ext2fs_struct_generic_bitmap_64 {
	errcode_t		magic;
	ext2_filsys 		fs;
	بنية ext2_bitmap_ops	*bitmap_ops;
	صحيح			flags;
	__u64			start, end;
	__u64			real_end;
	صحيح			cluster_bits;
	حرف			*description;
	فراغ			*خاص;
	errcode_t		base_error_code;
#ifdef ENABLE_BMAP_STATS
	بنية ext2_bmap_statistics	stats;
#endif
};

تعريفنوع بنية ext2fs_struct_generic_bitmap_64 *ext2fs_generic_bitmap_64;

#define EXT2FS_IS_32_BITMAP(bmap) \
	(((bmap)->magic == EXT2_ET_MAGIC_GENERIC_BITMAP) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_BLOCK_BITMAP) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_INODE_BITMAP))

#define EXT2FS_IS_64_BITMAP(bmap) \
	(((bmap)->magic == EXT2_ET_MAGIC_GENERIC_BITMAP64) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_BLOCK_BITMAP64) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_INODE_BITMAP64))

بنية ext2_bitmap_ops {
	صحيح	type;
	/* Generic bmap operators */
	errcode_t (*new_bmap)(ext2_filsys fs, ext2fs_generic_bitmap_64 bmap);
	فراغ	(*free_bmap)(ext2fs_generic_bitmap_64 bitmap);
	errcode_t (*copy_bmap)(ext2fs_generic_bitmap_64 src,
			     ext2fs_generic_bitmap_64 dest);
	errcode_t (*resize_bmap)(ext2fs_generic_bitmap_64 bitmap,
			       __u64 new_end,
			       __u64 new_real_end);
	/* bit set/test operators */
	صحيح	(*mark_bmap)(ext2fs_generic_bitmap_64 bitmap, __u64 arg);
	صحيح	(*unmark_bmap)(ext2fs_generic_bitmap_64 bitmap, __u64 arg);
	صحيح	(*test_bmap)(ext2fs_generic_bitmap_64 bitmap, __u64 arg);
	فراغ	(*mark_bmap_extent)(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				    unsigned صحيح عدد);
	فراغ	(*unmark_bmap_extent)(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				      unsigned صحيح عدد);
	صحيح	(*test_clear_bmap_extent)(ext2fs_generic_bitmap_64 bitmap,
					  __u64 arg, unsigned صحيح عدد);
	errcode_t (*set_bmap_range)(ext2fs_generic_bitmap_64 bitmap,
				    __u64 start, size_t عدد, فراغ *in);
	errcode_t (*get_bmap_range)(ext2fs_generic_bitmap_64 bitmap,
				    __u64 start, size_t عدد, فراغ *out);
	فراغ (*clear_bmap)(ext2fs_generic_bitmap_64 bitmap);
	فراغ (*print_stats)(ext2fs_generic_bitmap_64);

	/* Find the first zero bit between start and end, inclusive.
	 * May be NULL, in which case a generic function is used. */
	errcode_t (*find_first_zero)(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, __u64 end, __u64 *out);
	/* Find the first set bit between start and end, inclusive.
	 * May be NULL, in which case a generic function is used. */
	errcode_t (*find_first_set)(ext2fs_generic_bitmap_64 bitmap,
				    __u64 start, __u64 end, __u64 *out);
};

خارجي بنية ext2_bitmap_ops ext2fs_blkmap64_bitarray;
خارجي بنية ext2_bitmap_ops ext2fs_blkmap64_rbtree;
