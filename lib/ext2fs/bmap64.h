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

#که_تعریف_شوی ENABLE_BMAP_STATS_OPS
	بینښه اوږد	copy_شمېر;
	بینښه اوږد	resize_شمېر;
	بینښه اوږد	mark_شمېر;
	بینښه اوږد	unmark_شمېر;
	بینښه اوږد	test_شمېر;
	بینښه اوږد	mark_ext_شمېر;
	بینښه اوږد	unmark_ext_شمېر;
	بینښه اوږد	test_ext_شمېر;
	بینښه اوږد	سېټ_range_شمېر;
	بینښه اوږد	get_range_شمېر;
	بینښه اوږد	clear_شمېر;

	blk64_t		وروستی_marked;
	blk64_t		وروستی_tested;
	blk64_t		mark_شا;
	blk64_t		test_شا;

	بینښه اوږد	mark_seq;
	بینښه اوږد	test_seq;
#د_که_پای /* ENABLE_BMAP_STATS_OPS */
};


بنية ext2fs_struct_generic_bitmap_64 {
	errcode_t		magic;
	ext2_filsys 		fs;
	بنية ext2_bitmap_ops	*bitmap_ops;
	صحيح			flags;
	__u64			پیل, پای_ټکی;
	__u64			real_پای_ټکی;
	صحيح			cluster_بټونه;
	حرف			*description;
	فراغ			*خاص;
	errcode_t		بنسټ_error_کوډ;
#که_تعریف_شوی ENABLE_BMAP_STATS
	بنية ext2_bmap_statistics	stats;
#د_که_پای
};

تعريفنوع بنية ext2fs_struct_generic_bitmap_64 *ext2fs_generic_bitmap_64;

#تعریف_کړه EXT2FS_IS_32_BITMAP(bmap) \
	(((bmap)->magic == EXT2_ET_MAGIC_GENERIC_BITMAP) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_بلاک_BITMAP) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_INODE_BITMAP))

#تعریف_کړه EXT2FS_IS_64_BITMAP(bmap) \
	(((bmap)->magic == EXT2_ET_MAGIC_GENERIC_BITMAP64) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_بلاک_BITMAP64) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_INODE_BITMAP64))

بنية ext2_bitmap_ops {
	صحيح	type;
	/* Generic bmap operators */
	errcode_t (*new_bmap)(ext2_filsys fs, ext2fs_generic_bitmap_64 bmap);
	فراغ	(*free_bmap)(ext2fs_generic_bitmap_64 bitmap);
	errcode_t (*copy_bmap)(ext2fs_generic_bitmap_64 src,
			     ext2fs_generic_bitmap_64 dest);
	errcode_t (*resize_bmap)(ext2fs_generic_bitmap_64 bitmap,
			       __u64 new_پای_ټکی,
			       __u64 new_real_پای_ټکی);
	/* bit set/test operators */
	صحيح	(*mark_bmap)(ext2fs_generic_bitmap_64 bitmap, __u64 arg);
	صحيح	(*unmark_bmap)(ext2fs_generic_bitmap_64 bitmap, __u64 arg);
	صحيح	(*test_bmap)(ext2fs_generic_bitmap_64 bitmap, __u64 arg);
	فراغ	(*mark_bmap_extent)(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				    بینښه صحيح عدد);
	فراغ	(*unmark_bmap_extent)(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				      بینښه صحيح عدد);
	صحيح	(*test_clear_bmap_extent)(ext2fs_generic_bitmap_64 bitmap,
					  __u64 arg, بینښه صحيح عدد);
	errcode_t (*سېټ_bmap_range)(ext2fs_generic_bitmap_64 bitmap,
				    __u64 پیل, size_t عدد, فراغ *دننه);
	errcode_t (*get_bmap_range)(ext2fs_generic_bitmap_64 bitmap,
				    __u64 پیل, size_t عدد, فراغ *بهر);
	فراغ (*clear_bmap)(ext2fs_generic_bitmap_64 bitmap);
	فراغ (*print_stats)(ext2fs_generic_bitmap_64);

	/* Find the first zero bit between start and end, inclusive.
	 * May be NULL, in which case a generic function is used. */
	errcode_t (*پیدا_کړه_لومړی_zero)(ext2fs_generic_bitmap_64 bitmap,
				     __u64 پیل, __u64 پای_ټکی, __u64 *بهر);
	/* Find the first set bit between start and end, inclusive.
	 * May be NULL, in which case a generic function is used. */
	errcode_t (*پیدا_کړه_لومړی_سېټ)(ext2fs_generic_bitmap_64 bitmap,
				    __u64 پیل, __u64 پای_ټکی, __u64 *بهر);
};

خارجي بنية ext2_bitmap_ops ext2fs_blkmap64_bitarray;
خارجي بنية ext2_bitmap_ops ext2fs_blkmap64_rbtree;
