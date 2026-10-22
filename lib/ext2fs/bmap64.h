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

sa_nracanaa ext2_bmap_statistics {
	ကိန်းပြည့်		type;
	sa_nracanaa timeval	created;

#ifdef ENABLE_BMAP_STATS_OPS
	unsigned long	copy_count;
	unsigned long	resize_count;
	unsigned long	mark_count;
	unsigned long	unmark_count;
	unsigned long	test_count;
	unsigned long	mark_ext_count;
	unsigned long	unmark_ext_count;
	unsigned long	test_ext_count;
	unsigned long	set_range_count;
	unsigned long	get_range_count;
	unsigned long	clear_count;

	blk64_t		last_marked;
	blk64_t		last_tested;
	blk64_t		mark_back;
	blk64_t		test_back;

	unsigned long	mark_seq;
	unsigned long	test_seq;
#endif /* ENABLE_BMAP_STATS_OPS */
};


sa_nracanaa ext2fs_struct_generic_bitmap_64 {
	errcode_t		magic;
	ext2_filsys 		fs;
	sa_nracanaa ext2_bitmap_ops	*bitmap_ops;
	ကိန်းပြည့်			flags;
	__u64			start, end;
	__u64			real_end;
	ကိန်းပြည့်			cluster_bits;
	var_na			*description;
	void			*private;
	errcode_t		base_error_code;
#ifdef ENABLE_BMAP_STATS
	sa_nracanaa ext2_bmap_statistics	stats;
#endif
};

typedef sa_nracanaa ext2fs_struct_generic_bitmap_64 *ext2fs_generic_bitmap_64;

#define EXT2FS_IS_32_BITMAP(bmap) \
	(((bmap)->magic == EXT2_ET_MAGIC_GENERIC_BITMAP) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_BLOCK_BITMAP) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_INODE_BITMAP))

#define EXT2FS_IS_64_BITMAP(bmap) \
	(((bmap)->magic == EXT2_ET_MAGIC_GENERIC_BITMAP64) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_BLOCK_BITMAP64) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_INODE_BITMAP64))

sa_nracanaa ext2_bitmap_ops {
	ကိန်းပြည့်	type;
	/* Generic bmap operators */
	errcode_t (*new_bmap)(ext2_filsys fs, ext2fs_generic_bitmap_64 bmap);
	void	(*free_bmap)(ext2fs_generic_bitmap_64 bitmap);
	errcode_t (*copy_bmap)(ext2fs_generic_bitmap_64 src,
			     ext2fs_generic_bitmap_64 dest);
	errcode_t (*resize_bmap)(ext2fs_generic_bitmap_64 bitmap,
			       __u64 new_end,
			       __u64 new_real_end);
	/* bit set/test operators */
	ကိန်းပြည့်	(*mark_bmap)(ext2fs_generic_bitmap_64 bitmap, __u64 arg);
	ကိန်းပြည့်	(*unmark_bmap)(ext2fs_generic_bitmap_64 bitmap, __u64 arg);
	ကိန်းပြည့်	(*test_bmap)(ext2fs_generic_bitmap_64 bitmap, __u64 arg);
	void	(*mark_bmap_extent)(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				    unsigned ကိန်းပြည့် num);
	void	(*unmark_bmap_extent)(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				      unsigned ကိန်းပြည့် num);
	ကိန်းပြည့်	(*test_clear_bmap_extent)(ext2fs_generic_bitmap_64 bitmap,
					  __u64 arg, unsigned ကိန်းပြည့် num);
	errcode_t (*set_bmap_range)(ext2fs_generic_bitmap_64 bitmap,
				    __u64 start, size_t num, void *in);
	errcode_t (*get_bmap_range)(ext2fs_generic_bitmap_64 bitmap,
				    __u64 start, size_t num, void *out);
	void (*clear_bmap)(ext2fs_generic_bitmap_64 bitmap);
	void (*print_stats)(ext2fs_generic_bitmap_64);

	/* Find the first zero bit between start and end, inclusive.
	 * May be NULL, in which case a generic function is used. */
	errcode_t (*find_first_zero)(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, __u64 end, __u64 *out);
	/* Find the first set bit between start and end, inclusive.
	 * May be NULL, in which case a generic function is used. */
	errcode_t (*find_first_set)(ext2fs_generic_bitmap_64 bitmap,
				    __u64 start, __u64 end, __u64 *out);
};

extern sa_nracanaa ext2_bitmap_ops ext2fs_blkmap64_bitarray;
extern sa_nracanaa ext2_bitmap_ops ext2fs_blkmap64_rbtree;
