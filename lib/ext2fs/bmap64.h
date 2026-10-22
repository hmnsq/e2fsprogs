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

काष्ठा ext2_bmap_statistics {
	पूर्णांक		type;
	काष्ठा timeval	created;

#अगर_घोषित ENABLE_BMAP_STATS_OPS
	अचिन्हित दीर्घ	copy_गिनती;
	अचिन्हित दीर्घ	resize_गिनती;
	अचिन्हित दीर्घ	mark_गिनती;
	अचिन्हित दीर्घ	unmark_गिनती;
	अचिन्हित दीर्घ	test_गिनती;
	अचिन्हित दीर्घ	mark_ext_गिनती;
	अचिन्हित दीर्घ	unmark_ext_गिनती;
	अचिन्हित दीर्घ	test_ext_गिनती;
	अचिन्हित दीर्घ	set_range_गिनती;
	अचिन्हित दीर्घ	get_range_गिनती;
	अचिन्हित दीर्घ	clear_गिनती;

	blk64_t		last_marked;
	blk64_t		last_tested;
	blk64_t		mark_back;
	blk64_t		test_back;

	अचिन्हित दीर्घ	mark_seq;
	अचिन्हित दीर्घ	test_seq;
#पूर्ण_अगर /* ENABLE_BMAP_STATS_OPS */
};


काष्ठा ext2fs_struct_generic_bitmap_64 {
	errcode_t		magic;
	ext2_filsys 		fs;
	काष्ठा ext2_bitmap_ops	*bitmap_ops;
	पूर्णांक			flags;
	__u64			start, end;
	__u64			real_end;
	पूर्णांक			cluster_bits;
	अक्षर			*description;
	व्योम			*निजी;
	errcode_t		base_error_code;
#अगर_घोषित ENABLE_BMAP_STATS
	काष्ठा ext2_bmap_statistics	stats;
#पूर्ण_अगर
};

प्रकार काष्ठा ext2fs_struct_generic_bitmap_64 *ext2fs_generic_bitmap_64;

#घोषणा EXT2FS_IS_32_BITMAP(bmap) \
	(((bmap)->magic == EXT2_ET_MAGIC_GENERIC_BITMAP) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_BLOCK_BITMAP) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_INODE_BITMAP))

#घोषणा EXT2FS_IS_64_BITMAP(bmap) \
	(((bmap)->magic == EXT2_ET_MAGIC_GENERIC_BITMAP64) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_BLOCK_BITMAP64) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_INODE_BITMAP64))

काष्ठा ext2_bitmap_ops {
	पूर्णांक	type;
	/* Generic bmap operators */
	errcode_t (*new_bmap)(ext2_filsys fs, ext2fs_generic_bitmap_64 bmap);
	व्योम	(*free_bmap)(ext2fs_generic_bitmap_64 bitmap);
	errcode_t (*copy_bmap)(ext2fs_generic_bitmap_64 src,
			     ext2fs_generic_bitmap_64 dest);
	errcode_t (*resize_bmap)(ext2fs_generic_bitmap_64 bitmap,
			       __u64 new_end,
			       __u64 new_real_end);
	/* bit set/test operators */
	पूर्णांक	(*mark_bmap)(ext2fs_generic_bitmap_64 bitmap, __u64 arg);
	पूर्णांक	(*unmark_bmap)(ext2fs_generic_bitmap_64 bitmap, __u64 arg);
	पूर्णांक	(*test_bmap)(ext2fs_generic_bitmap_64 bitmap, __u64 arg);
	व्योम	(*mark_bmap_extent)(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				    अचिन्हित पूर्णांक अंक);
	व्योम	(*unmark_bmap_extent)(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				      अचिन्हित पूर्णांक अंक);
	पूर्णांक	(*test_clear_bmap_extent)(ext2fs_generic_bitmap_64 bitmap,
					  __u64 arg, अचिन्हित पूर्णांक अंक);
	errcode_t (*set_bmap_range)(ext2fs_generic_bitmap_64 bitmap,
				    __u64 start, माप_प्रकार अंक, व्योम *in);
	errcode_t (*get_bmap_range)(ext2fs_generic_bitmap_64 bitmap,
				    __u64 start, माप_प्रकार अंक, व्योम *out);
	व्योम (*clear_bmap)(ext2fs_generic_bitmap_64 bitmap);
	व्योम (*print_stats)(ext2fs_generic_bitmap_64);

	/* Find the first zero bit between start and end, inclusive.
	 * May be NULL, in which case a generic function is used. */
	errcode_t (*find_first_zero)(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, __u64 end, __u64 *out);
	/* Find the first set bit between start and end, inclusive.
	 * May be NULL, in which case a generic function is used. */
	errcode_t (*find_first_set)(ext2fs_generic_bitmap_64 bitmap,
				    __u64 start, __u64 end, __u64 *out);
};

बाह्य काष्ठा ext2_bitmap_ops ext2fs_blkmap64_bitarray;
बाह्य काष्ठा ext2_bitmap_ops ext2fs_blkmap64_rbtree;
