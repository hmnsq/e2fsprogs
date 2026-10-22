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

ਬਣਤਰ ext2_bmap_statistics {
	ਪੂਰਨ		type;
	ਬਣਤਰ timeval	created;

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ENABLE_BMAP_STATS_OPS
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ	copy_ਗਿਣਤੀ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ	resize_ਗਿਣਤੀ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ	mark_ਗਿਣਤੀ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ	unmark_ਗਿਣਤੀ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ	test_ਗਿਣਤੀ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ	mark_ext_ਗਿਣਤੀ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ	unmark_ext_ਗਿਣਤੀ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ	test_ext_ਗਿਣਤੀ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ	ਸੈੱਟ_range_ਗਿਣਤੀ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ	get_range_ਗਿਣਤੀ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ	clear_ਗਿਣਤੀ;

	blk64_t		ਆਖਰੀ_marked;
	blk64_t		ਆਖਰੀ_tested;
	blk64_t		mark_ਪਿੱਠ;
	blk64_t		test_ਪਿੱਠ;

	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ	mark_seq;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ	test_seq;
#ਜੇ_ਅੰਤ /* ENABLE_BMAP_STATS_OPS */
};


ਬਣਤਰ ext2fs_struct_generic_bitmap_64 {
	errcode_t		magic;
	ext2_filsys 		fs;
	ਬਣਤਰ ext2_bitmap_ops	*bitmap_ops;
	ਪੂਰਨ			flags;
	__u64			ਸ਼ੁਰੂ, ਸਿਰਾ;
	__u64			real_ਸਿਰਾ;
	ਪੂਰਨ			cluster_ਬਿੱਟ;
	ਅੱਖਰ			*description;
	ਖਾਲੀ			*ਨਿਜੀ;
	errcode_t		ਅਧਾਰ_error_ਕੋਡ;
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ENABLE_BMAP_STATS
	ਬਣਤਰ ext2_bmap_statistics	stats;
#ਜੇ_ਅੰਤ
};

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ext2fs_struct_generic_bitmap_64 *ext2fs_generic_bitmap_64;

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2FS_IS_32_BITMAP(bmap) \
	(((bmap)->magic == EXT2_ET_MAGIC_GENERIC_BITMAP) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_ਖੰਡ_BITMAP) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_INODE_BITMAP))

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2FS_IS_64_BITMAP(bmap) \
	(((bmap)->magic == EXT2_ET_MAGIC_GENERIC_BITMAP64) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_ਖੰਡ_BITMAP64) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_INODE_BITMAP64))

ਬਣਤਰ ext2_bitmap_ops {
	ਪੂਰਨ	type;
	/* Generic bmap operators */
	errcode_t (*new_bmap)(ext2_filsys fs, ext2fs_generic_bitmap_64 bmap);
	ਖਾਲੀ	(*free_bmap)(ext2fs_generic_bitmap_64 bitmap);
	errcode_t (*copy_bmap)(ext2fs_generic_bitmap_64 src,
			     ext2fs_generic_bitmap_64 dest);
	errcode_t (*resize_bmap)(ext2fs_generic_bitmap_64 bitmap,
			       __u64 new_ਸਿਰਾ,
			       __u64 new_real_ਸਿਰਾ);
	/* bit set/test operators */
	ਪੂਰਨ	(*mark_bmap)(ext2fs_generic_bitmap_64 bitmap, __u64 arg);
	ਪੂਰਨ	(*unmark_bmap)(ext2fs_generic_bitmap_64 bitmap, __u64 arg);
	ਪੂਰਨ	(*test_bmap)(ext2fs_generic_bitmap_64 bitmap, __u64 arg);
	ਖਾਲੀ	(*mark_bmap_extent)(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				    ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ);
	ਖਾਲੀ	(*unmark_bmap_extent)(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				      ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ);
	ਪੂਰਨ	(*test_clear_bmap_extent)(ext2fs_generic_bitmap_64 bitmap,
					  __u64 arg, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ);
	errcode_t (*ਸੈੱਟ_bmap_range)(ext2fs_generic_bitmap_64 bitmap,
				    __u64 ਸ਼ੁਰੂ, size_t ਅੰਕ, ਖਾਲੀ *ਵਿੱਚ);
	errcode_t (*get_bmap_range)(ext2fs_generic_bitmap_64 bitmap,
				    __u64 ਸ਼ੁਰੂ, size_t ਅੰਕ, ਖਾਲੀ *ਬਾਹਰਲਾ);
	ਖਾਲੀ (*clear_bmap)(ext2fs_generic_bitmap_64 bitmap);
	ਖਾਲੀ (*print_stats)(ext2fs_generic_bitmap_64);

	/* Find the first zero bit between start and end, inclusive.
	 * May be NULL, in which case a generic function is used. */
	errcode_t (*ਲੱਭੋ_ਪਹਿਲਾ_zero)(ext2fs_generic_bitmap_64 bitmap,
				     __u64 ਸ਼ੁਰੂ, __u64 ਸਿਰਾ, __u64 *ਬਾਹਰਲਾ);
	/* Find the first set bit between start and end, inclusive.
	 * May be NULL, in which case a generic function is used. */
	errcode_t (*ਲੱਭੋ_ਪਹਿਲਾ_ਸੈੱਟ)(ext2fs_generic_bitmap_64 bitmap,
				    __u64 ਸ਼ੁਰੂ, __u64 ਸਿਰਾ, __u64 *ਬਾਹਰਲਾ);
};

ਬਾਹਰੀ ਬਣਤਰ ext2_bitmap_ops ext2fs_blkmap64_bitarray;
ਬਾਹਰੀ ਬਣਤਰ ext2_bitmap_ops ext2fs_blkmap64_rbtree;
