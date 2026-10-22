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

ساخت ext2_bmap_statistics {
	صحیح		type;
	ساخت timeval	created;

#اگر_تعریف_شدہ ENABLE_BMAP_STATS_OPS
	غیرعلامتی طویل	copy_گنتی;
	غیرعلامتی طویل	resize_گنتی;
	غیرعلامتی طویل	mark_گنتی;
	غیرعلامتی طویل	unmark_گنتی;
	غیرعلامتی طویل	test_گنتی;
	غیرعلامتی طویل	mark_ext_گنتی;
	غیرعلامتی طویل	unmark_ext_گنتی;
	غیرعلامتی طویل	test_ext_گنتی;
	غیرعلامتی طویل	سیٹ_range_گنتی;
	غیرعلامتی طویل	get_range_گنتی;
	غیرعلامتی طویل	clear_گنتی;

	blk64_t		آخری_marked;
	blk64_t		آخری_tested;
	blk64_t		mark_عقب;
	blk64_t		test_عقب;

	غیرعلامتی طویل	mark_seq;
	غیرعلامتی طویل	test_seq;
#اگر_ختم /* ENABLE_BMAP_STATS_OPS */
};


ساخت ext2fs_struct_generic_bitmap_64 {
	errcode_t		magic;
	ext2_filsys 		fs;
	ساخت ext2_bitmap_ops	*bitmap_ops;
	صحیح			flags;
	__u64			آغاز, انتہا;
	__u64			real_انتہا;
	صحیح			cluster_بٹ;
	حرف			*description;
	خالی			*نجی;
	errcode_t		بنیاد_error_رمز;
#اگر_تعریف_شدہ ENABLE_BMAP_STATS
	ساخت ext2_bmap_statistics	stats;
#اگر_ختم
};

قسمتعریف ساخت ext2fs_struct_generic_bitmap_64 *ext2fs_generic_bitmap_64;

#تعریف_کرو EXT2FS_IS_32_BITMAP(bmap) \
	(((bmap)->magic == EXT2_ET_MAGIC_GENERIC_BITMAP) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_بلاک_BITMAP) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_INODE_BITMAP))

#تعریف_کرو EXT2FS_IS_64_BITMAP(bmap) \
	(((bmap)->magic == EXT2_ET_MAGIC_GENERIC_BITMAP64) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_بلاک_BITMAP64) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_INODE_BITMAP64))

ساخت ext2_bitmap_ops {
	صحیح	type;
	/* Generic bmap operators */
	errcode_t (*new_bmap)(ext2_filsys fs, ext2fs_generic_bitmap_64 bmap);
	خالی	(*free_bmap)(ext2fs_generic_bitmap_64 bitmap);
	errcode_t (*copy_bmap)(ext2fs_generic_bitmap_64 src,
			     ext2fs_generic_bitmap_64 dest);
	errcode_t (*resize_bmap)(ext2fs_generic_bitmap_64 bitmap,
			       __u64 new_انتہا,
			       __u64 new_real_انتہا);
	/* bit set/test operators */
	صحیح	(*mark_bmap)(ext2fs_generic_bitmap_64 bitmap, __u64 arg);
	صحیح	(*unmark_bmap)(ext2fs_generic_bitmap_64 bitmap, __u64 arg);
	صحیح	(*test_bmap)(ext2fs_generic_bitmap_64 bitmap, __u64 arg);
	خالی	(*mark_bmap_extent)(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				    غیرعلامتی صحیح عدد);
	خالی	(*unmark_bmap_extent)(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				      غیرعلامتی صحیح عدد);
	صحیح	(*test_clear_bmap_extent)(ext2fs_generic_bitmap_64 bitmap,
					  __u64 arg, غیرعلامتی صحیح عدد);
	errcode_t (*سیٹ_bmap_range)(ext2fs_generic_bitmap_64 bitmap,
				    __u64 آغاز, size_t عدد, خالی *اندر);
	errcode_t (*get_bmap_range)(ext2fs_generic_bitmap_64 bitmap,
				    __u64 آغاز, size_t عدد, خالی *برآمد);
	خالی (*clear_bmap)(ext2fs_generic_bitmap_64 bitmap);
	خالی (*print_stats)(ext2fs_generic_bitmap_64);

	/* Find the first zero bit between start and end, inclusive.
	 * May be NULL, in which case a generic function is used. */
	errcode_t (*ڈھونڈو_پہلا_zero)(ext2fs_generic_bitmap_64 bitmap,
				     __u64 آغاز, __u64 انتہا, __u64 *برآمد);
	/* Find the first set bit between start and end, inclusive.
	 * May be NULL, in which case a generic function is used. */
	errcode_t (*ڈھونڈو_پہلا_سیٹ)(ext2fs_generic_bitmap_64 bitmap,
				    __u64 آغاز, __u64 انتہا, __u64 *برآمد);
};

خارجی ساخت ext2_bitmap_ops ext2fs_blkmap64_bitarray;
خارجی ساخت ext2_bitmap_ops ext2fs_blkmap64_rbtree;
