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

#यदि_परिभाषितम् ENABLE_BMAP_STATS_OPS
	अचिह्नित दीर्घ	copy_गणनम्;
	अचिह्नित दीर्घ	resize_गणनम्;
	अचिह्नित दीर्घ	mark_गणनम्;
	अचिह्नित दीर्घ	unmark_गणनम्;
	अचिह्नित दीर्घ	test_गणनम्;
	अचिह्नित दीर्घ	mark_ext_गणनम्;
	अचिह्नित दीर्घ	unmark_ext_गणनम्;
	अचिह्नित दीर्घ	test_ext_गणनम्;
	अचिह्नित दीर्घ	समुच्चयः_range_गणनम्;
	अचिह्नित दीर्घ	get_range_गणनम्;
	अचिह्नित दीर्घ	clear_गणनम्;

	blk64_t		चरमः_marked;
	blk64_t		चरमः_tested;
	blk64_t		mark_पृष्ठभागः;
	blk64_t		test_पृष्ठभागः;

	अचिह्नित दीर्घ	mark_seq;
	अचिह्नित दीर्घ	test_seq;
#यदि_अन्तः /* ENABLE_BMAP_STATS_OPS */
};


संरचना ext2fs_struct_generic_bitmap_64 {
	errcode_t		magic;
	ext2_filsys 		fs;
	संरचना ext2_bitmap_ops	*bitmap_ops;
	पूर्णाङ्क			flags;
	__u64			start, अन्तः;
	__u64			real_अन्तः;
	पूर्णाङ्क			cluster_द्वयङ्काः;
	वर्ण			*description;
	शून्य			*गुप्त;
	errcode_t		आधारः_error_संहिता;
#यदि_परिभाषितम् ENABLE_BMAP_STATS
	संरचना ext2_bmap_statistics	stats;
#यदि_अन्तः
};

प्रकारनिर्देश संरचना ext2fs_struct_generic_bitmap_64 *ext2fs_generic_bitmap_64;

#परिभाषय EXT2FS_IS_32_BITMAP(bmap) \
	(((bmap)->magic == EXT2_ET_MAGIC_GENERIC_BITMAP) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_खण्डम्_BITMAP) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_INODE_BITMAP))

#परिभाषय EXT2FS_IS_64_BITMAP(bmap) \
	(((bmap)->magic == EXT2_ET_MAGIC_GENERIC_BITMAP64) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_खण्डम्_BITMAP64) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_INODE_BITMAP64))

संरचना ext2_bitmap_ops {
	पूर्णाङ्क	type;
	/* Generic bmap operators */
	errcode_t (*new_bmap)(ext2_filsys fs, ext2fs_generic_bitmap_64 bmap);
	शून्य	(*free_bmap)(ext2fs_generic_bitmap_64 bitmap);
	errcode_t (*copy_bmap)(ext2fs_generic_bitmap_64 src,
			     ext2fs_generic_bitmap_64 dest);
	errcode_t (*resize_bmap)(ext2fs_generic_bitmap_64 bitmap,
			       __u64 new_अन्तः,
			       __u64 new_real_अन्तः);
	/* bit set/test operators */
	पूर्णाङ्क	(*mark_bmap)(ext2fs_generic_bitmap_64 bitmap, __u64 arg);
	पूर्णाङ्क	(*unmark_bmap)(ext2fs_generic_bitmap_64 bitmap, __u64 arg);
	पूर्णाङ्क	(*test_bmap)(ext2fs_generic_bitmap_64 bitmap, __u64 arg);
	शून्य	(*mark_bmap_extent)(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				    अचिह्नित पूर्णाङ्क अङ्कः);
	शून्य	(*unmark_bmap_extent)(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				      अचिह्नित पूर्णाङ्क अङ्कः);
	पूर्णाङ्क	(*test_clear_bmap_extent)(ext2fs_generic_bitmap_64 bitmap,
					  __u64 arg, अचिह्नित पूर्णाङ्क अङ्कः);
	errcode_t (*समुच्चयः_bmap_range)(ext2fs_generic_bitmap_64 bitmap,
				    __u64 start, size_t अङ्कः, शून्य *अन्तर्);
	errcode_t (*get_bmap_range)(ext2fs_generic_bitmap_64 bitmap,
				    __u64 start, size_t अङ्कः, शून्य *बहिर्गमनम्);
	शून्य (*clear_bmap)(ext2fs_generic_bitmap_64 bitmap);
	शून्य (*print_stats)(ext2fs_generic_bitmap_64);

	/* Find the first zero bit between start and end, inclusive.
	 * May be NULL, in which case a generic function is used. */
	errcode_t (*find_first_zero)(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, __u64 अन्तः, __u64 *बहिर्गमनम्);
	/* Find the first set bit between start and end, inclusive.
	 * May be NULL, in which case a generic function is used. */
	errcode_t (*find_first_समुच्चयः)(ext2fs_generic_bitmap_64 bitmap,
				    __u64 start, __u64 अन्तः, __u64 *बहिर्गमनम्);
};

बाह्य संरचना ext2_bitmap_ops ext2fs_blkmap64_bitarray;
बाह्य संरचना ext2_bitmap_ops ext2fs_blkmap64_rbtree;
