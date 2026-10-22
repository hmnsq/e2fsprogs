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

#جيڪڏهن_وصف_ٿيل ENABLE_BMAP_STATS_OPS
	بنانشان ڊگهو	copy_ڳڻپ;
	بنانشان ڊگهو	resize_ڳڻپ;
	بنانشان ڊگهو	mark_ڳڻپ;
	بنانشان ڊگهو	unmark_ڳڻپ;
	بنانشان ڊگهو	test_ڳڻپ;
	بنانشان ڊگهو	mark_ext_ڳڻپ;
	بنانشان ڊگهو	unmark_ext_ڳڻپ;
	بنانشان ڊگهو	test_ext_ڳڻپ;
	بنانشان ڊگهو	سيٽ_range_ڳڻپ;
	بنانشان ڊگهو	get_range_ڳڻپ;
	بنانشان ڊگهو	clear_ڳڻپ;

	blk64_t		آخري_marked;
	blk64_t		آخري_tested;
	blk64_t		mark_پٺ;
	blk64_t		test_پٺ;

	بنانشان ڊگهو	mark_seq;
	بنانشان ڊگهو	test_seq;
#جيڪڏهن_پورو /* ENABLE_BMAP_STATS_OPS */
};


بنية ext2fs_struct_generic_bitmap_64 {
	errcode_t		magic;
	ext2_filsys 		fs;
	بنية ext2_bitmap_ops	*bitmap_ops;
	صحيح			flags;
	__u64			شروعات, پڇاڙي;
	__u64			real_پڇاڙي;
	صحيح			cluster_بٽ;
	حرف			*description;
	فراغ			*خاص;
	errcode_t		بنياد_error_ڪوڊ;
#جيڪڏهن_وصف_ٿيل ENABLE_BMAP_STATS
	بنية ext2_bmap_statistics	stats;
#جيڪڏهن_پورو
};

تعريفنوع بنية ext2fs_struct_generic_bitmap_64 *ext2fs_generic_bitmap_64;

#وصف_ڪريو EXT2FS_IS_32_BITMAP(bmap) \
	(((bmap)->magic == EXT2_ET_MAGIC_GENERIC_BITMAP) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_بلاڪ_BITMAP) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_INODE_BITMAP))

#وصف_ڪريو EXT2FS_IS_64_BITMAP(bmap) \
	(((bmap)->magic == EXT2_ET_MAGIC_GENERIC_BITMAP64) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_بلاڪ_BITMAP64) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_INODE_BITMAP64))

بنية ext2_bitmap_ops {
	صحيح	type;
	/* Generic bmap operators */
	errcode_t (*new_bmap)(ext2_filsys fs, ext2fs_generic_bitmap_64 bmap);
	فراغ	(*free_bmap)(ext2fs_generic_bitmap_64 bitmap);
	errcode_t (*copy_bmap)(ext2fs_generic_bitmap_64 src,
			     ext2fs_generic_bitmap_64 dest);
	errcode_t (*resize_bmap)(ext2fs_generic_bitmap_64 bitmap,
			       __u64 new_پڇاڙي,
			       __u64 new_real_پڇاڙي);
	/* bit set/test operators */
	صحيح	(*mark_bmap)(ext2fs_generic_bitmap_64 bitmap, __u64 arg);
	صحيح	(*unmark_bmap)(ext2fs_generic_bitmap_64 bitmap, __u64 arg);
	صحيح	(*test_bmap)(ext2fs_generic_bitmap_64 bitmap, __u64 arg);
	فراغ	(*mark_bmap_extent)(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				    بنانشان صحيح انگ);
	فراغ	(*unmark_bmap_extent)(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				      بنانشان صحيح انگ);
	صحيح	(*test_clear_bmap_extent)(ext2fs_generic_bitmap_64 bitmap,
					  __u64 arg, بنانشان صحيح انگ);
	errcode_t (*سيٽ_bmap_range)(ext2fs_generic_bitmap_64 bitmap,
				    __u64 شروعات, size_t انگ, فراغ *اندريون);
	errcode_t (*get_bmap_range)(ext2fs_generic_bitmap_64 bitmap,
				    __u64 شروعات, size_t انگ, فراغ *ٻاهريون);
	فراغ (*clear_bmap)(ext2fs_generic_bitmap_64 bitmap);
	فراغ (*print_stats)(ext2fs_generic_bitmap_64);

	/* Find the first zero bit between start and end, inclusive.
	 * May be NULL, in which case a generic function is used. */
	errcode_t (*ڳوليو_پهريون_zero)(ext2fs_generic_bitmap_64 bitmap,
				     __u64 شروعات, __u64 پڇاڙي, __u64 *ٻاهريون);
	/* Find the first set bit between start and end, inclusive.
	 * May be NULL, in which case a generic function is used. */
	errcode_t (*ڳوليو_پهريون_سيٽ)(ext2fs_generic_bitmap_64 bitmap,
				    __u64 شروعات, __u64 پڇاڙي, __u64 *ٻاهريون);
};

خارجي بنية ext2_bitmap_ops ext2fs_blkmap64_bitarray;
خارجي بنية ext2_bitmap_ops ext2fs_blkmap64_rbtree;
