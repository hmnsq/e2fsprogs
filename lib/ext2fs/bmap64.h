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

#جے_تعریف_شدہ ENABLE_BMAP_STATS_OPS
	بنانشانی لما	copy_گنتی;
	بنانشانی لما	resize_گنتی;
	بنانشانی لما	mark_گنتی;
	بنانشانی لما	unmark_گنتی;
	بنانشانی لما	test_گنتی;
	بنانشانی لما	mark_ext_گنتی;
	بنانشانی لما	unmark_ext_گنتی;
	بنانشانی لما	test_ext_گنتی;
	بنانشانی لما	سیٹ_range_گنتی;
	بنانشانی لما	get_range_گنتی;
	بنانشانی لما	clear_گنتی;

	blk64_t		آخری_marked;
	blk64_t		آخری_tested;
	blk64_t		mark_پٹھ;
	blk64_t		test_پٹھ;

	بنانشانی لما	mark_seq;
	بنانشانی لما	test_seq;
#جے_انت /* ENABLE_BMAP_STATS_OPS */
};


بنية ext2fs_struct_generic_bitmap_64 {
	errcode_t		magic;
	ext2_filsys 		fs;
	بنية ext2_bitmap_ops	*bitmap_ops;
	صحيح			flags;
	__u64			شروع, سرا;
	__u64			real_سرا;
	صحيح			cluster_بٹ;
	حرف			*description;
	فراغ			*خاص;
	errcode_t		ادھار_error_کوڈ;
#جے_تعریف_شدہ ENABLE_BMAP_STATS
	بنية ext2_bmap_statistics	stats;
#جے_انت
};

تعريفنوع بنية ext2fs_struct_generic_bitmap_64 *ext2fs_generic_bitmap_64;

#تعریف_کرو EXT2FS_IS_32_BITMAP(bmap) \
	(((bmap)->magic == EXT2_ET_MAGIC_GENERIC_BITMAP) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_بلاک_BITMAP) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_INODE_BITMAP))

#تعریف_کرو EXT2FS_IS_64_BITMAP(bmap) \
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
			       __u64 new_سرا,
			       __u64 new_real_سرا);
	/* bit set/test operators */
	صحيح	(*mark_bmap)(ext2fs_generic_bitmap_64 bitmap, __u64 arg);
	صحيح	(*unmark_bmap)(ext2fs_generic_bitmap_64 bitmap, __u64 arg);
	صحيح	(*test_bmap)(ext2fs_generic_bitmap_64 bitmap, __u64 arg);
	فراغ	(*mark_bmap_extent)(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				    بنانشانی صحيح عدد);
	فراغ	(*unmark_bmap_extent)(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				      بنانشانی صحيح عدد);
	صحيح	(*test_clear_bmap_extent)(ext2fs_generic_bitmap_64 bitmap,
					  __u64 arg, بنانشانی صحيح عدد);
	errcode_t (*سیٹ_bmap_range)(ext2fs_generic_bitmap_64 bitmap,
				    __u64 شروع, size_t عدد, فراغ *وچ);
	errcode_t (*get_bmap_range)(ext2fs_generic_bitmap_64 bitmap,
				    __u64 شروع, size_t عدد, فراغ *باہرلا);
	فراغ (*clear_bmap)(ext2fs_generic_bitmap_64 bitmap);
	فراغ (*print_stats)(ext2fs_generic_bitmap_64);

	/* Find the first zero bit between start and end, inclusive.
	 * May be NULL, in which case a generic function is used. */
	errcode_t (*لبھو_پہلا_zero)(ext2fs_generic_bitmap_64 bitmap,
				     __u64 شروع, __u64 سرا, __u64 *باہرلا);
	/* Find the first set bit between start and end, inclusive.
	 * May be NULL, in which case a generic function is used. */
	errcode_t (*لبھو_پہلا_سیٹ)(ext2fs_generic_bitmap_64 bitmap,
				    __u64 شروع, __u64 سرا, __u64 *باہرلا);
};

خارجي بنية ext2_bitmap_ops ext2fs_blkmap64_bitarray;
خارجي بنية ext2_bitmap_ops ext2fs_blkmap64_rbtree;
