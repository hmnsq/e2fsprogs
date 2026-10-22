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

నిర్మాణం ext2_bmap_statistics {
	పూర్ణాంకం		type;
	నిర్మాణం timeval	created;

#నిర్వచితమైతే ENABLE_BMAP_STATS_OPS
	గుర్తులేని దీర్ఘ	copy_లెక్క;
	గుర్తులేని దీర్ఘ	resize_లెక్క;
	గుర్తులేని దీర్ఘ	mark_లెక్క;
	గుర్తులేని దీర్ఘ	unmark_లెక్క;
	గుర్తులేని దీర్ఘ	test_లెక్క;
	గుర్తులేని దీర్ఘ	mark_ext_లెక్క;
	గుర్తులేని దీర్ఘ	unmark_ext_లెక్క;
	గుర్తులేని దీర్ఘ	test_ext_లెక్క;
	గుర్తులేని దీర్ఘ	సమితి_range_లెక్క;
	గుర్తులేని దీర్ఘ	get_range_లెక్క;
	గుర్తులేని దీర్ఘ	clear_లెక్క;

	blk64_t		చివరి_marked;
	blk64_t		చివరి_tested;
	blk64_t		mark_వెనుక;
	blk64_t		test_వెనుక;

	గుర్తులేని దీర్ఘ	mark_seq;
	గుర్తులేని దీర్ఘ	test_seq;
#అయితే_ముగింపు /* ENABLE_BMAP_STATS_OPS */
};


నిర్మాణం ext2fs_struct_generic_bitmap_64 {
	errcode_t		magic;
	ext2_filsys 		fs;
	నిర్మాణం ext2_bitmap_ops	*bitmap_ops;
	పూర్ణాంకం			flags;
	__u64			ప్రారంభం, చివర;
	__u64			real_చివర;
	పూర్ణాంకం			cluster_బిట్లు;
	అక్షరం			*description;
	శూన్య			*అంతరంగ;
	errcode_t		పునాది_error_సంకేతం;
#నిర్వచితమైతే ENABLE_BMAP_STATS
	నిర్మాణం ext2_bmap_statistics	stats;
#అయితే_ముగింపు
};

రకనిర్వచనం నిర్మాణం ext2fs_struct_generic_bitmap_64 *ext2fs_generic_bitmap_64;

#నిర్వచించు EXT2FS_IS_32_BITMAP(bmap) \
	(((bmap)->magic == EXT2_ET_MAGIC_GENERIC_BITMAP) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_ఖండం_BITMAP) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_INODE_BITMAP))

#నిర్వచించు EXT2FS_IS_64_BITMAP(bmap) \
	(((bmap)->magic == EXT2_ET_MAGIC_GENERIC_BITMAP64) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_ఖండం_BITMAP64) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_INODE_BITMAP64))

నిర్మాణం ext2_bitmap_ops {
	పూర్ణాంకం	type;
	/* Generic bmap operators */
	errcode_t (*new_bmap)(ext2_filsys fs, ext2fs_generic_bitmap_64 bmap);
	శూన్య	(*free_bmap)(ext2fs_generic_bitmap_64 bitmap);
	errcode_t (*copy_bmap)(ext2fs_generic_bitmap_64 src,
			     ext2fs_generic_bitmap_64 dest);
	errcode_t (*resize_bmap)(ext2fs_generic_bitmap_64 bitmap,
			       __u64 new_చివర,
			       __u64 new_real_చివర);
	/* bit set/test operators */
	పూర్ణాంకం	(*mark_bmap)(ext2fs_generic_bitmap_64 bitmap, __u64 arg);
	పూర్ణాంకం	(*unmark_bmap)(ext2fs_generic_bitmap_64 bitmap, __u64 arg);
	పూర్ణాంకం	(*test_bmap)(ext2fs_generic_bitmap_64 bitmap, __u64 arg);
	శూన్య	(*mark_bmap_extent)(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				    గుర్తులేని పూర్ణాంకం అంకె);
	శూన్య	(*unmark_bmap_extent)(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				      గుర్తులేని పూర్ణాంకం అంకె);
	పూర్ణాంకం	(*test_clear_bmap_extent)(ext2fs_generic_bitmap_64 bitmap,
					  __u64 arg, గుర్తులేని పూర్ణాంకం అంకె);
	errcode_t (*సమితి_bmap_range)(ext2fs_generic_bitmap_64 bitmap,
				    __u64 ప్రారంభం, size_t అంకె, శూన్య *లోపల);
	errcode_t (*get_bmap_range)(ext2fs_generic_bitmap_64 bitmap,
				    __u64 ప్రారంభం, size_t అంకె, శూన్య *బహిర్గతం);
	శూన్య (*clear_bmap)(ext2fs_generic_bitmap_64 bitmap);
	శూన్య (*print_stats)(ext2fs_generic_bitmap_64);

	/* Find the first zero bit between start and end, inclusive.
	 * May be NULL, in which case a generic function is used. */
	errcode_t (*కనుగొను_మొదటి_zero)(ext2fs_generic_bitmap_64 bitmap,
				     __u64 ప్రారంభం, __u64 చివర, __u64 *బహిర్గతం);
	/* Find the first set bit between start and end, inclusive.
	 * May be NULL, in which case a generic function is used. */
	errcode_t (*కనుగొను_మొదటి_సమితి)(ext2fs_generic_bitmap_64 bitmap,
				    __u64 ప్రారంభం, __u64 చివర, __u64 *బహిర్గతం);
};

బాహ్య నిర్మాణం ext2_bitmap_ops ext2fs_blkmap64_bitarray;
బాహ్య నిర్మాణం ext2_bitmap_ops ext2fs_blkmap64_rbtree;
