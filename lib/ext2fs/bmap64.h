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

ഘടന ext2_bmap_statistics {
	പൂർണ്ണസംഖ്യ		type;
	ഘടന timeval	created;

#ifdef ENABLE_BMAP_STATS_OPS
	unsigned long	copy_എണ്ണം;
	unsigned long	resize_എണ്ണം;
	unsigned long	mark_എണ്ണം;
	unsigned long	unmark_എണ്ണം;
	unsigned long	test_എണ്ണം;
	unsigned long	mark_ext_എണ്ണം;
	unsigned long	unmark_ext_എണ്ണം;
	unsigned long	test_ext_എണ്ണം;
	unsigned long	set_range_എണ്ണം;
	unsigned long	get_range_എണ്ണം;
	unsigned long	clear_എണ്ണം;

	blk64_t		last_marked;
	blk64_t		last_tested;
	blk64_t		mark_back;
	blk64_t		test_back;

	unsigned long	mark_seq;
	unsigned long	test_seq;
#endif /* ENABLE_BMAP_STATS_OPS */
};


ഘടന ext2fs_struct_generic_bitmap_64 {
	errcode_t		magic;
	ext2_filsys 		fs;
	ഘടന ext2_bitmap_ops	*bitmap_ops;
	പൂർണ്ണസംഖ്യ			flags;
	__u64			start, end;
	__u64			real_end;
	പൂർണ്ണസംഖ്യ			cluster_bits;
	അക്ഷരം			*description;
	ശൂന്യം			*സ്വകാര്യ;
	errcode_t		base_error_code;
#ifdef ENABLE_BMAP_STATS
	ഘടന ext2_bmap_statistics	stats;
#endif
};

തരനിർവചനം ഘടന ext2fs_struct_generic_bitmap_64 *ext2fs_generic_bitmap_64;

#define EXT2FS_IS_32_BITMAP(bmap) \
	(((bmap)->magic == EXT2_ET_MAGIC_GENERIC_BITMAP) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_BLOCK_BITMAP) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_INODE_BITMAP))

#define EXT2FS_IS_64_BITMAP(bmap) \
	(((bmap)->magic == EXT2_ET_MAGIC_GENERIC_BITMAP64) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_BLOCK_BITMAP64) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_INODE_BITMAP64))

ഘടന ext2_bitmap_ops {
	പൂർണ്ണസംഖ്യ	type;
	/* Generic bmap operators */
	errcode_t (*new_bmap)(ext2_filsys fs, ext2fs_generic_bitmap_64 bmap);
	ശൂന്യം	(*free_bmap)(ext2fs_generic_bitmap_64 bitmap);
	errcode_t (*copy_bmap)(ext2fs_generic_bitmap_64 src,
			     ext2fs_generic_bitmap_64 dest);
	errcode_t (*resize_bmap)(ext2fs_generic_bitmap_64 bitmap,
			       __u64 new_end,
			       __u64 new_real_end);
	/* bit set/test operators */
	പൂർണ്ണസംഖ്യ	(*mark_bmap)(ext2fs_generic_bitmap_64 bitmap, __u64 arg);
	പൂർണ്ണസംഖ്യ	(*unmark_bmap)(ext2fs_generic_bitmap_64 bitmap, __u64 arg);
	പൂർണ്ണസംഖ്യ	(*test_bmap)(ext2fs_generic_bitmap_64 bitmap, __u64 arg);
	ശൂന്യം	(*mark_bmap_extent)(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				    unsigned പൂർണ്ണസംഖ്യ അക്കം);
	ശൂന്യം	(*unmark_bmap_extent)(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				      unsigned പൂർണ്ണസംഖ്യ അക്കം);
	പൂർണ്ണസംഖ്യ	(*test_clear_bmap_extent)(ext2fs_generic_bitmap_64 bitmap,
					  __u64 arg, unsigned പൂർണ്ണസംഖ്യ അക്കം);
	errcode_t (*set_bmap_range)(ext2fs_generic_bitmap_64 bitmap,
				    __u64 start, size_t അക്കം, ശൂന്യം *in);
	errcode_t (*get_bmap_range)(ext2fs_generic_bitmap_64 bitmap,
				    __u64 start, size_t അക്കം, ശൂന്യം *out);
	ശൂന്യം (*clear_bmap)(ext2fs_generic_bitmap_64 bitmap);
	ശൂന്യം (*print_stats)(ext2fs_generic_bitmap_64);

	/* Find the first zero bit between start and end, inclusive.
	 * May be NULL, in which case a generic function is used. */
	errcode_t (*find_first_zero)(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, __u64 end, __u64 *out);
	/* Find the first set bit between start and end, inclusive.
	 * May be NULL, in which case a generic function is used. */
	errcode_t (*find_first_set)(ext2fs_generic_bitmap_64 bitmap,
				    __u64 start, __u64 end, __u64 *out);
};

ബാഹ്യ ഘടന ext2_bitmap_ops ext2fs_blkmap64_bitarray;
ബാഹ്യ ഘടന ext2_bitmap_ops ext2fs_blkmap64_rbtree;
