/*
 * bitops.h --- Bitmap frobbing code.  The byte swapping routines are
 * 	also included here.
 *
 * Copyright (C) 1993, 1994, 1995, 1996 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ਸ਼ਬਦ_BIGENDIAN
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ext2fs_cpu_ਮੰਜ਼ਿਲ_le64(x) ((__force __le64)ext2fs_swab64((__u64)(x)))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ext2fs_le64_ਮੰਜ਼ਿਲ_cpu(x) ext2fs_swab64((__force __u64)(__le64)(x))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ext2fs_cpu_ਮੰਜ਼ਿਲ_le32(x) ((__force __le32)ext2fs_swab32((__u32)(x)))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ext2fs_le32_ਮੰਜ਼ਿਲ_cpu(x) ext2fs_swab32((__force __u32)(__le32)(x))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ext2fs_cpu_ਮੰਜ਼ਿਲ_le16(x) ((__force __le16)ext2fs_swab16((__u16)(x)))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ext2fs_le16_ਮੰਜ਼ਿਲ_cpu(x) ext2fs_swab16((__force __u16)(__le16)(x))

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ext2fs_cpu_ਮੰਜ਼ਿਲ_be64(x) ((__force __be64)(__u64)(x))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ext2fs_be64_ਮੰਜ਼ਿਲ_cpu(x) ((__force __u64)(__be64)(x))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ext2fs_cpu_ਮੰਜ਼ਿਲ_be32(x) ((__force __be32)(__u32)(x))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ext2fs_be32_ਮੰਜ਼ਿਲ_cpu(x) ((__force __u32)(__be32)(x))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ext2fs_cpu_ਮੰਜ਼ਿਲ_be16(x) ((__force __be16)(__u16)(x))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ext2fs_be16_ਮੰਜ਼ਿਲ_cpu(x) ((__force __u16)(__be16)(x))
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ext2fs_cpu_ਮੰਜ਼ਿਲ_le64(x) ((__force __le64)(__u64)(x))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ext2fs_le64_ਮੰਜ਼ਿਲ_cpu(x) ((__force __u64)(__le64)(x))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ext2fs_cpu_ਮੰਜ਼ਿਲ_le32(x) ((__force __le32)(__u32)(x))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ext2fs_le32_ਮੰਜ਼ਿਲ_cpu(x) ((__force __u32)(__le32)(x))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ext2fs_cpu_ਮੰਜ਼ਿਲ_le16(x) ((__force __le16)(__u16)(x))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ext2fs_le16_ਮੰਜ਼ਿਲ_cpu(x) ((__force __u16)(__le16)(x))

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ext2fs_cpu_ਮੰਜ਼ਿਲ_be64(x) ((__force __be64)ext2fs_swab64((__u64)(x)))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ext2fs_be64_ਮੰਜ਼ਿਲ_cpu(x) ext2fs_swab64((__force __u64)(__be64)(x))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ext2fs_cpu_ਮੰਜ਼ਿਲ_be32(x) ((__force __be32)ext2fs_swab32((__u32)(x)))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ext2fs_be32_ਮੰਜ਼ਿਲ_cpu(x) ext2fs_swab32((__force __u32)(__be32)(x))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ext2fs_cpu_ਮੰਜ਼ਿਲ_be16(x) ((__force __be16)ext2fs_swab16((__u16)(x)))
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ ext2fs_be16_ਮੰਜ਼ਿਲ_cpu(x) ext2fs_swab16((__force __u16)(__be16)(x))
#ਜੇ_ਅੰਤ

/*
 * EXT2FS bitmap manipulation routines.
 */

/* Support for sending warning messages from the inline subroutines */
ਬਾਹਰੀ ਅਚਲ ਅੱਖਰ *ext2fs_ਖੰਡ_string;
ਬਾਹਰੀ ਅਚਲ ਅੱਖਰ *ext2fs_inode_string;
ਬਾਹਰੀ ਅਚਲ ਅੱਖਰ *ext2fs_mark_string;
ਬਾਹਰੀ ਅਚਲ ਅੱਖਰ *ext2fs_unmark_string;
ਬਾਹਰੀ ਅਚਲ ਅੱਖਰ *ext2fs_test_string;
ਬਾਹਰੀ ਖਾਲੀ ext2fs_warn_bitmap(errcode_t errcode, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ arg,
			       ਅਚਲ ਅੱਖਰ *description);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_warn_bitmap2(ext2fs_generic_bitmap bitmap,
				ਪੂਰਨ ਕੋਡ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ arg);

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ NO_INLINE_FUNCS
ਬਾਹਰੀ ਖਾਲੀ ext2fs_fast_ਸੈੱਟ_ਦੋਆਧਾਰੀ_ਅੰਕ(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ nr,ਖਾਲੀ * addr);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_fast_clear_ਦੋਆਧਾਰੀ_ਅੰਕ(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ nr, ਖਾਲੀ * addr);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_fast_ਸੈੱਟ_bit64(__u64 nr,ਖਾਲੀ * addr);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_fast_clear_bit64(__u64 nr, ਖਾਲੀ * addr);
ਬਾਹਰੀ __u16 ext2fs_swab16(__u16 val);
ਬਾਹਰੀ __u32 ext2fs_swab32(__u32 val);
ਬਾਹਰੀ __u64 ext2fs_swab64(__u64 val);

ਬਾਹਰੀ ਪੂਰਨ ext2fs_mark_ਖੰਡ_bitmap(ext2fs_ਖੰਡ_bitmap bitmap, blk_t ਖੰਡ);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_unmark_ਖੰਡ_bitmap(ext2fs_ਖੰਡ_bitmap bitmap,
				       blk_t ਖੰਡ);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_test_ਖੰਡ_bitmap(ext2fs_ਖੰਡ_bitmap bitmap, blk_t ਖੰਡ);

ਬਾਹਰੀ ਪੂਰਨ ext2fs_mark_inode_bitmap(ext2fs_inode_bitmap bitmap, ext2_ino_t inode);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_unmark_inode_bitmap(ext2fs_inode_bitmap bitmap,
				       ext2_ino_t inode);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_test_inode_bitmap(ext2fs_inode_bitmap bitmap, ext2_ino_t inode);

ਬਾਹਰੀ ਖਾਲੀ ext2fs_fast_mark_ਖੰਡ_bitmap(ext2fs_ਖੰਡ_bitmap bitmap,
					  blk_t ਖੰਡ);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_fast_unmark_ਖੰਡ_bitmap(ext2fs_ਖੰਡ_bitmap bitmap,
					    blk_t ਖੰਡ);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_fast_test_ਖੰਡ_bitmap(ext2fs_ਖੰਡ_bitmap bitmap,
					 blk_t ਖੰਡ);

ਬਾਹਰੀ ਖਾਲੀ ext2fs_fast_mark_inode_bitmap(ext2fs_inode_bitmap bitmap,
					  ext2_ino_t inode);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_fast_unmark_inode_bitmap(ext2fs_inode_bitmap bitmap,
					    ext2_ino_t inode);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_fast_test_inode_bitmap(ext2fs_inode_bitmap bitmap,
					 ext2_ino_t inode);
ਬਾਹਰੀ blk_t ext2fs_get_ਖੰਡ_bitmap_ਸ਼ੁਰੂ(ext2fs_ਖੰਡ_bitmap bitmap);
ਬਾਹਰੀ ext2_ino_t ext2fs_get_inode_bitmap_ਸ਼ੁਰੂ(ext2fs_inode_bitmap bitmap);
ਬਾਹਰੀ blk_t ext2fs_get_ਖੰਡ_bitmap_ਸਿਰਾ(ext2fs_ਖੰਡ_bitmap bitmap);
ਬਾਹਰੀ ext2_ino_t ext2fs_get_inode_bitmap_ਸਿਰਾ(ext2fs_inode_bitmap bitmap);

ਬਾਹਰੀ ਖਾਲੀ ext2fs_fast_mark_ਖੰਡ_bitmap_range(ext2fs_ਖੰਡ_bitmap bitmap,
						blk_t ਖੰਡ, ਪੂਰਨ ਅੰਕ);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_fast_unmark_ਖੰਡ_bitmap_range(ext2fs_ਖੰਡ_bitmap bitmap,
						  blk_t ਖੰਡ, ਪੂਰਨ ਅੰਕ);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_fast_test_ਖੰਡ_bitmap_range(ext2fs_ਖੰਡ_bitmap bitmap,
					       blk_t ਖੰਡ, ਪੂਰਨ ਅੰਕ);
#ਜੇ_ਅੰਤ

/* These functions routines moved to gen_bitmap.c */
ਬਾਹਰੀ ਖਾਲੀ ext2fs_mark_ਖੰਡ_bitmap_range(ext2fs_ਖੰਡ_bitmap bitmap,
					   blk_t ਖੰਡ, ਪੂਰਨ ਅੰਕ);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_unmark_ਖੰਡ_bitmap_range(ext2fs_ਖੰਡ_bitmap bitmap,
					     blk_t ਖੰਡ, ਪੂਰਨ ਅੰਕ);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_test_ਖੰਡ_bitmap_range(ext2fs_ਖੰਡ_bitmap bitmap,
					  blk_t ਖੰਡ, ਪੂਰਨ ਅੰਕ);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_test_inode_bitmap_range(ext2fs_inode_bitmap bitmap,
					  ext2_ino_t inode, ਪੂਰਨ ਅੰਕ);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_mark_generic_bitmap(ext2fs_generic_bitmap bitmap,
					 __u32 bitno);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_unmark_generic_bitmap(ext2fs_generic_bitmap bitmap,
					   blk_t bitno);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_test_generic_bitmap(ext2fs_generic_bitmap bitmap,
				      blk_t bitno);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_test_ਖੰਡ_bitmap_range(ext2fs_ਖੰਡ_bitmap bitmap,
					  blk_t ਖੰਡ, ਪੂਰਨ ਅੰਕ);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_ਸੈੱਟ_bitmap_padding(ext2fs_generic_bitmap map);
ਬਾਹਰੀ __u32 ext2fs_get_generic_bitmap_ਸ਼ੁਰੂ(ext2fs_generic_bitmap bitmap);
ਬਾਹਰੀ __u32 ext2fs_get_generic_bitmap_ਸਿਰਾ(ext2fs_generic_bitmap bitmap);

/* 64-bit versions */

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ NO_INLINE_FUNCS
ਬਾਹਰੀ ਪੂਰਨ ext2fs_mark_ਖੰਡ_bitmap2(ext2fs_ਖੰਡ_bitmap bitmap,
				     blk64_t ਖੰਡ);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_unmark_ਖੰਡ_bitmap2(ext2fs_ਖੰਡ_bitmap bitmap,
				       blk64_t ਖੰਡ);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_test_ਖੰਡ_bitmap2(ext2fs_ਖੰਡ_bitmap bitmap,
				     blk64_t ਖੰਡ);

ਬਾਹਰੀ ਪੂਰਨ ext2fs_mark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
				     ext2_ino_t inode);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_unmark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
				       ext2_ino_t inode);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_test_inode_bitmap2(ext2fs_inode_bitmap bitmap,
				     ext2_ino_t inode);

ਬਾਹਰੀ ਖਾਲੀ ext2fs_fast_mark_ਖੰਡ_bitmap2(ext2fs_ਖੰਡ_bitmap bitmap,
					   blk64_t ਖੰਡ);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_fast_unmark_ਖੰਡ_bitmap2(ext2fs_ਖੰਡ_bitmap bitmap,
					     blk64_t ਖੰਡ);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_fast_test_ਖੰਡ_bitmap2(ext2fs_ਖੰਡ_bitmap bitmap,
					  blk64_t ਖੰਡ);

ਬਾਹਰੀ ਖਾਲੀ ext2fs_fast_mark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
					   ext2_ino_t inode);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_fast_unmark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
					    ext2_ino_t inode);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_fast_test_inode_bitmap2(ext2fs_inode_bitmap bitmap,
					  ext2_ino_t inode);
ਬਾਹਰੀ errcode_t ext2fs_ਲੱਭੋ_ਪਹਿਲਾ_zero_ਖੰਡ_bitmap2(ext2fs_ਖੰਡ_bitmap bitmap,
						      blk64_t ਸ਼ੁਰੂ,
						      blk64_t ਸਿਰਾ,
						      blk64_t *ਬਾਹਰਲਾ);
ਬਾਹਰੀ errcode_t ext2fs_ਲੱਭੋ_ਪਹਿਲਾ_zero_inode_bitmap2(ext2fs_inode_bitmap bitmap,
						      ext2_ino_t ਸ਼ੁਰੂ,
						      ext2_ino_t ਸਿਰਾ,
						      ext2_ino_t *ਬਾਹਰਲਾ);
ਬਾਹਰੀ errcode_t ext2fs_ਲੱਭੋ_ਪਹਿਲਾ_ਸੈੱਟ_ਖੰਡ_bitmap2(ext2fs_ਖੰਡ_bitmap bitmap,
						     blk64_t ਸ਼ੁਰੂ,
						     blk64_t ਸਿਰਾ,
						     blk64_t *ਬਾਹਰਲਾ);
ਬਾਹਰੀ errcode_t ext2fs_ਲੱਭੋ_ਪਹਿਲਾ_ਸੈੱਟ_inode_bitmap2(ext2fs_inode_bitmap bitmap,
						      ext2_ino_t ਸ਼ੁਰੂ,
						      ext2_ino_t ਸਿਰਾ,
						      ext2_ino_t *ਬਾਹਰਲਾ);
ਬਾਹਰੀ blk64_t ext2fs_get_ਖੰਡ_bitmap_start2(ext2fs_ਖੰਡ_bitmap bitmap);
ਬਾਹਰੀ ext2_ino_t ext2fs_get_inode_bitmap_start2(ext2fs_inode_bitmap bitmap);
ਬਾਹਰੀ blk64_t ext2fs_get_ਖੰਡ_bitmap_end2(ext2fs_ਖੰਡ_bitmap bitmap);
ਬਾਹਰੀ ext2_ino_t ext2fs_get_inode_bitmap_end2(ext2fs_inode_bitmap bitmap);

ਬਾਹਰੀ ਪੂਰਨ ext2fs_fast_test_ਖੰਡ_bitmap_range2(ext2fs_ਖੰਡ_bitmap bitmap,
						blk64_t ਖੰਡ,
						ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_fast_mark_ਖੰਡ_bitmap_range2(ext2fs_ਖੰਡ_bitmap bitmap,
						 blk64_t ਖੰਡ,
						 ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_fast_unmark_ਖੰਡ_bitmap_range2(ext2fs_ਖੰਡ_bitmap bitmap,
						   blk64_t ਖੰਡ,
						   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ);
#ਜੇ_ਅੰਤ

/* These routines moved to gen_bitmap64.c */
ਬਾਹਰੀ ਖਾਲੀ ext2fs_clear_generic_bmap(ext2fs_generic_bitmap bitmap);
ਬਾਹਰੀ errcode_t ext2fs_compare_generic_bmap(errcode_t neq,
					     ext2fs_generic_bitmap bm1,
					     ext2fs_generic_bitmap bm2);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_ਸੈੱਟ_generic_bmap_padding(ext2fs_generic_bitmap bmap);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_mark_generic_bmap(ext2fs_generic_bitmap bitmap,
				    blk64_t bitno);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_unmark_generic_bmap(ext2fs_generic_bitmap bitmap,
				      blk64_t bitno);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_test_generic_bmap(ext2fs_generic_bitmap bitmap,
				    blk64_t bitno);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_test_ਖੰਡ_bitmap_range2(ext2fs_ਖੰਡ_bitmap bitmap,
					   blk64_t ਖੰਡ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ);
ਬਾਹਰੀ __u64 ext2fs_get_generic_bmap_ਸ਼ੁਰੂ(ext2fs_generic_bitmap bitmap);
ਬਾਹਰੀ __u64 ext2fs_get_generic_bmap_ਸਿਰਾ(ext2fs_generic_bitmap bitmap);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_test_ਖੰਡ_bitmap_range2(ext2fs_ਖੰਡ_bitmap bitmap,
					   blk64_t ਖੰਡ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_mark_ਖੰਡ_bitmap_range2(ext2fs_ਖੰਡ_bitmap bitmap,
					    blk64_t ਖੰਡ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ);
ਬਾਹਰੀ ਖਾਲੀ ext2fs_unmark_ਖੰਡ_bitmap_range2(ext2fs_ਖੰਡ_bitmap bitmap,
					      blk64_t ਖੰਡ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ);
ਬਾਹਰੀ errcode_t ext2fs_ਲੱਭੋ_ਪਹਿਲਾ_zero_generic_bmap(ext2fs_generic_bitmap bitmap,
						     __u64 ਸ਼ੁਰੂ, __u64 ਸਿਰਾ,
						     __u64 *ਬਾਹਰਲਾ);
ਬਾਹਰੀ errcode_t ext2fs_ਲੱਭੋ_ਪਹਿਲਾ_ਸੈੱਟ_generic_bmap(ext2fs_generic_bitmap bitmap,
						    __u64 ਸ਼ੁਰੂ, __u64 ਸਿਰਾ,
						    __u64 *ਬਾਹਰਲਾ);

/*
 * The inline routines themselves...
 *
 * If NO_INLINE_FUNCS is defined, then we won't try to do inline
 * functions at all; they will be included as normal functions in
 * inline.c
 */

#ਜੇ (defined(INCLUDE_INLINE_FUNCS) || !defined(NO_INLINE_FUNCS))
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ INCLUDE_INLINE_FUNCS
#ਜੇ (__STDC_VERSION__ >= 199901L)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _INLINE_ ਬਾਹਰੀ ਸਤਰਅੰਦਰ
#ਨਹੀਂਤਾਂ
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _INLINE_ ਸਤਰਅੰਦਰ
#ਜੇ_ਅੰਤ
#ਨਹੀਂਤਾਂ /* !INCLUDE_INLINE FUNCS */
#ਜੇ (__STDC_VERSION__ >= 199901L)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _INLINE_ ਸਤਰਅੰਦਰ
#ਨਹੀਂਤਾਂ /* not C99 */
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __GNUC__
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _INLINE_ ਬਾਹਰੀ __inline__
#ਨਹੀਂਤਾਂ				/* For Watcom C */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ _INLINE_ ਬਾਹਰੀ ਸਤਰਅੰਦਰ
#ਜੇ_ਅੰਤ /* __GNUC__ */
#ਜੇ_ਅੰਤ /* __STDC_VERSION__ >= 199901L */
#ਜੇ_ਅੰਤ /* INCLUDE_INLINE_FUNCS */

/*
 * Fast bit set/clear functions that doesn't need to return the
 * previous bit value.
 */

_INLINE_ ਖਾਲੀ ext2fs_fast_ਸੈੱਟ_ਦੋਆਧਾਰੀ_ਅੰਕ(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ nr,ਖਾਲੀ * addr)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ	*ADDR = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *) addr;

	ADDR += nr >> 3;
	*ADDR |= (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ) (1 << (nr & 0x07));
}

_INLINE_ ਖਾਲੀ ext2fs_fast_clear_ਦੋਆਧਾਰੀ_ਅੰਕ(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ nr, ਖਾਲੀ * addr)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ	*ADDR = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *) addr;

	ADDR += nr >> 3;
	*ADDR &= (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ) ~(1 << (nr & 0x07));
}


_INLINE_ ਖਾਲੀ ext2fs_fast_ਸੈੱਟ_bit64(__u64 nr, ਖਾਲੀ * addr)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ	*ADDR = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *) addr;

	ADDR += nr >> 3;
	*ADDR |= (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ) (1 << (nr & 0x07));
}

_INLINE_ ਖਾਲੀ ext2fs_fast_clear_bit64(__u64 nr, ਖਾਲੀ * addr)
{
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ	*ADDR = (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *) addr;

	ADDR += nr >> 3;
	*ADDR &= (ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ) ~(1 << (nr & 0x07));
}

_INLINE_ __u16 ext2fs_swab16(__u16 val)
{
	ਵਾਪਸ (val >> 8) | (__u16) (val << 8);
}

_INLINE_ __u32 ext2fs_swab32(__u32 val)
{
	ਵਾਪਸ ((val>>24) | ((val>>8)&0xFF00) |
		((val<<8)&0xFF0000) | (val<<24));
}

_INLINE_ __u64 ext2fs_swab64(__u64 val)
{
	ਵਾਪਸ (ext2fs_swab32((__u32) (val >> 32)) |
		(((__u64)ext2fs_swab32(val & 0xFFFFFFFFUL)) << 32));
}

_INLINE_ ਪੂਰਨ ext2fs_mark_ਖੰਡ_bitmap(ext2fs_ਖੰਡ_bitmap bitmap,
				       blk_t ਖੰਡ)
{
	ਵਾਪਸ ext2fs_mark_generic_bitmap((ext2fs_generic_bitmap) bitmap,
					  ਖੰਡ);
}

_INLINE_ ਪੂਰਨ ext2fs_unmark_ਖੰਡ_bitmap(ext2fs_ਖੰਡ_bitmap bitmap,
					 blk_t ਖੰਡ)
{
	ਵਾਪਸ ext2fs_unmark_generic_bitmap((ext2fs_generic_bitmap) bitmap,
					    ਖੰਡ);
}

_INLINE_ ਪੂਰਨ ext2fs_test_ਖੰਡ_bitmap(ext2fs_ਖੰਡ_bitmap bitmap,
				       blk_t ਖੰਡ)
{
	ਵਾਪਸ ext2fs_test_generic_bitmap((ext2fs_generic_bitmap) bitmap,
					  ਖੰਡ);
}

_INLINE_ ਪੂਰਨ ext2fs_mark_inode_bitmap(ext2fs_inode_bitmap bitmap,
				       ext2_ino_t inode)
{
	ਵਾਪਸ ext2fs_mark_generic_bitmap((ext2fs_generic_bitmap) bitmap,
					  inode);
}

_INLINE_ ਪੂਰਨ ext2fs_unmark_inode_bitmap(ext2fs_inode_bitmap bitmap,
					 ext2_ino_t inode)
{
	ਵਾਪਸ ext2fs_unmark_generic_bitmap((ext2fs_generic_bitmap) bitmap,
				     inode);
}

_INLINE_ ਪੂਰਨ ext2fs_test_inode_bitmap(ext2fs_inode_bitmap bitmap,
				       ext2_ino_t inode)
{
	ਵਾਪਸ ext2fs_test_generic_bitmap((ext2fs_generic_bitmap) bitmap,
					  inode);
}

_INLINE_ ਖਾਲੀ ext2fs_fast_mark_ਖੰਡ_bitmap(ext2fs_ਖੰਡ_bitmap bitmap,
					    blk_t ਖੰਡ)
{
	ext2fs_mark_generic_bitmap((ext2fs_generic_bitmap) bitmap, ਖੰਡ);
}

_INLINE_ ਖਾਲੀ ext2fs_fast_unmark_ਖੰਡ_bitmap(ext2fs_ਖੰਡ_bitmap bitmap,
					      blk_t ਖੰਡ)
{
	ext2fs_unmark_generic_bitmap((ext2fs_generic_bitmap) bitmap, ਖੰਡ);
}

_INLINE_ ਪੂਰਨ ext2fs_fast_test_ਖੰਡ_bitmap(ext2fs_ਖੰਡ_bitmap bitmap,
					    blk_t ਖੰਡ)
{
	ਵਾਪਸ ext2fs_test_generic_bitmap((ext2fs_generic_bitmap) bitmap,
					  ਖੰਡ);
}

_INLINE_ ਖਾਲੀ ext2fs_fast_mark_inode_bitmap(ext2fs_inode_bitmap bitmap,
					    ext2_ino_t inode)
{
	ext2fs_mark_generic_bitmap((ext2fs_generic_bitmap) bitmap, inode);
}

_INLINE_ ਖਾਲੀ ext2fs_fast_unmark_inode_bitmap(ext2fs_inode_bitmap bitmap,
					      ext2_ino_t inode)
{
	ext2fs_unmark_generic_bitmap((ext2fs_generic_bitmap) bitmap, inode);
}

_INLINE_ ਪੂਰਨ ext2fs_fast_test_inode_bitmap(ext2fs_inode_bitmap bitmap,
					   ext2_ino_t inode)
{
	ਵਾਪਸ ext2fs_test_generic_bitmap((ext2fs_generic_bitmap) bitmap,
					  inode);
}

_INLINE_ blk_t ext2fs_get_ਖੰਡ_bitmap_ਸ਼ੁਰੂ(ext2fs_ਖੰਡ_bitmap bitmap)
{
	ਵਾਪਸ ext2fs_get_generic_bitmap_ਸ਼ੁਰੂ((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ ext2_ino_t ext2fs_get_inode_bitmap_ਸ਼ੁਰੂ(ext2fs_inode_bitmap bitmap)
{
	ਵਾਪਸ ext2fs_get_generic_bitmap_ਸ਼ੁਰੂ((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ blk_t ext2fs_get_ਖੰਡ_bitmap_ਸਿਰਾ(ext2fs_ਖੰਡ_bitmap bitmap)
{
	ਵਾਪਸ ext2fs_get_generic_bitmap_ਸਿਰਾ((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ ext2_ino_t ext2fs_get_inode_bitmap_ਸਿਰਾ(ext2fs_inode_bitmap bitmap)
{
	ਵਾਪਸ ext2fs_get_generic_bitmap_ਸਿਰਾ((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ ਪੂਰਨ ext2fs_fast_test_ਖੰਡ_bitmap_range(ext2fs_ਖੰਡ_bitmap bitmap,
						 blk_t ਖੰਡ, ਪੂਰਨ ਅੰਕ)
{
	ਵਾਪਸ ext2fs_test_ਖੰਡ_bitmap_range(bitmap, ਖੰਡ, ਅੰਕ);
}

_INLINE_ ਖਾਲੀ ext2fs_fast_mark_ਖੰਡ_bitmap_range(ext2fs_ਖੰਡ_bitmap bitmap,
						  blk_t ਖੰਡ, ਪੂਰਨ ਅੰਕ)
{
	ext2fs_mark_ਖੰਡ_bitmap_range(bitmap, ਖੰਡ, ਅੰਕ);
}

_INLINE_ ਖਾਲੀ ext2fs_fast_unmark_ਖੰਡ_bitmap_range(ext2fs_ਖੰਡ_bitmap bitmap,
						    blk_t ਖੰਡ, ਪੂਰਨ ਅੰਕ)
{
	ext2fs_unmark_ਖੰਡ_bitmap_range(bitmap, ਖੰਡ, ਅੰਕ);
}

/* 64-bit versions */

_INLINE_ ਪੂਰਨ ext2fs_mark_ਖੰਡ_bitmap2(ext2fs_ਖੰਡ_bitmap bitmap,
				       blk64_t ਖੰਡ)
{
	ਵਾਪਸ ext2fs_mark_generic_bmap((ext2fs_generic_bitmap) bitmap,
					ਖੰਡ);
}

_INLINE_ ਪੂਰਨ ext2fs_unmark_ਖੰਡ_bitmap2(ext2fs_ਖੰਡ_bitmap bitmap,
					 blk64_t ਖੰਡ)
{
	ਵਾਪਸ ext2fs_unmark_generic_bmap((ext2fs_generic_bitmap) bitmap, ਖੰਡ);
}

_INLINE_ ਪੂਰਨ ext2fs_test_ਖੰਡ_bitmap2(ext2fs_ਖੰਡ_bitmap bitmap,
				       blk64_t ਖੰਡ)
{
	ਵਾਪਸ ext2fs_test_generic_bmap((ext2fs_generic_bitmap) bitmap,
					ਖੰਡ);
}

_INLINE_ ਪੂਰਨ ext2fs_mark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
				       ext2_ino_t inode)
{
	ਵਾਪਸ ext2fs_mark_generic_bmap((ext2fs_generic_bitmap) bitmap,
					inode);
}

_INLINE_ ਪੂਰਨ ext2fs_unmark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
					 ext2_ino_t inode)
{
	ਵਾਪਸ ext2fs_unmark_generic_bmap((ext2fs_generic_bitmap) bitmap,
					  inode);
}

_INLINE_ ਪੂਰਨ ext2fs_test_inode_bitmap2(ext2fs_inode_bitmap bitmap,
				       ext2_ino_t inode)
{
	ਵਾਪਸ ext2fs_test_generic_bmap((ext2fs_generic_bitmap) bitmap,
					inode);
}

_INLINE_ ਖਾਲੀ ext2fs_fast_mark_ਖੰਡ_bitmap2(ext2fs_ਖੰਡ_bitmap bitmap,
					     blk64_t ਖੰਡ)
{
	ext2fs_mark_generic_bmap((ext2fs_generic_bitmap) bitmap, ਖੰਡ);
}

_INLINE_ ਖਾਲੀ ext2fs_fast_unmark_ਖੰਡ_bitmap2(ext2fs_ਖੰਡ_bitmap bitmap,
					       blk64_t ਖੰਡ)
{
	ext2fs_unmark_generic_bmap((ext2fs_generic_bitmap) bitmap, ਖੰਡ);
}

_INLINE_ ਪੂਰਨ ext2fs_fast_test_ਖੰਡ_bitmap2(ext2fs_ਖੰਡ_bitmap bitmap,
					    blk64_t ਖੰਡ)
{
	ਵਾਪਸ ext2fs_test_generic_bmap((ext2fs_generic_bitmap) bitmap,
					ਖੰਡ);
}

_INLINE_ ਖਾਲੀ ext2fs_fast_mark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
					     ext2_ino_t inode)
{
	ext2fs_mark_generic_bmap((ext2fs_generic_bitmap) bitmap, inode);
}

_INLINE_ ਖਾਲੀ ext2fs_fast_unmark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
					       ext2_ino_t inode)
{
	ext2fs_unmark_generic_bmap((ext2fs_generic_bitmap) bitmap, inode);
}

_INLINE_ ਪੂਰਨ ext2fs_fast_test_inode_bitmap2(ext2fs_inode_bitmap bitmap,
					    ext2_ino_t inode)
{
	ਵਾਪਸ ext2fs_test_generic_bmap((ext2fs_generic_bitmap) bitmap,
					inode);
}

_INLINE_ errcode_t ext2fs_ਲੱਭੋ_ਪਹਿਲਾ_zero_ਖੰਡ_bitmap2(ext2fs_ਖੰਡ_bitmap bitmap,
							blk64_t ਸ਼ੁਰੂ,
							blk64_t ਸਿਰਾ,
							blk64_t *ਬਾਹਰਲਾ)
{
	__u64 o;
	errcode_t rv;

	rv = ext2fs_ਲੱਭੋ_ਪਹਿਲਾ_zero_generic_bmap((ext2fs_generic_bitmap) bitmap,
						 ਸ਼ੁਰੂ, ਸਿਰਾ, &o);
	ਜੇ (!rv)
		*ਬਾਹਰਲਾ = o;
	ਵਾਪਸ rv;
}

_INLINE_ errcode_t ext2fs_ਲੱਭੋ_ਪਹਿਲਾ_zero_inode_bitmap2(ext2fs_inode_bitmap bitmap,
							ext2_ino_t ਸ਼ੁਰੂ,
							ext2_ino_t ਸਿਰਾ,
							ext2_ino_t *ਬਾਹਰਲਾ)
{
	__u64 o;
	errcode_t rv;

	rv = ext2fs_ਲੱਭੋ_ਪਹਿਲਾ_zero_generic_bmap((ext2fs_generic_bitmap) bitmap,
						 ਸ਼ੁਰੂ, ਸਿਰਾ, &o);
	ਜੇ (!rv)
		*ਬਾਹਰਲਾ = (ext2_ino_t) o;
	ਵਾਪਸ rv;
}

_INLINE_ errcode_t ext2fs_ਲੱਭੋ_ਪਹਿਲਾ_ਸੈੱਟ_ਖੰਡ_bitmap2(ext2fs_ਖੰਡ_bitmap bitmap,
						       blk64_t ਸ਼ੁਰੂ,
						       blk64_t ਸਿਰਾ,
						       blk64_t *ਬਾਹਰਲਾ)
{
	__u64 o;
	errcode_t rv;

	rv = ext2fs_ਲੱਭੋ_ਪਹਿਲਾ_ਸੈੱਟ_generic_bmap((ext2fs_generic_bitmap) bitmap,
						ਸ਼ੁਰੂ, ਸਿਰਾ, &o);
	ਜੇ (!rv)
		*ਬਾਹਰਲਾ = o;
	ਵਾਪਸ rv;
}

_INLINE_ errcode_t ext2fs_ਲੱਭੋ_ਪਹਿਲਾ_ਸੈੱਟ_inode_bitmap2(ext2fs_inode_bitmap bitmap,
						       ext2_ino_t ਸ਼ੁਰੂ,
						       ext2_ino_t ਸਿਰਾ,
						       ext2_ino_t *ਬਾਹਰਲਾ)
{
	__u64 o;
	errcode_t rv;

	rv = ext2fs_ਲੱਭੋ_ਪਹਿਲਾ_ਸੈੱਟ_generic_bmap((ext2fs_generic_bitmap) bitmap,
						ਸ਼ੁਰੂ, ਸਿਰਾ, &o);
	ਜੇ (!rv)
		*ਬਾਹਰਲਾ = (ext2_ino_t) o;
	ਵਾਪਸ rv;
}

_INLINE_ blk64_t ext2fs_get_ਖੰਡ_bitmap_start2(ext2fs_ਖੰਡ_bitmap bitmap)
{
	ਵਾਪਸ ext2fs_get_generic_bmap_ਸ਼ੁਰੂ((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ ext2_ino_t ext2fs_get_inode_bitmap_start2(ext2fs_inode_bitmap bitmap)
{
	ਵਾਪਸ (ext2_ino_t) ext2fs_get_generic_bmap_ਸ਼ੁਰੂ((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ blk64_t ext2fs_get_ਖੰਡ_bitmap_end2(ext2fs_ਖੰਡ_bitmap bitmap)
{
	ਵਾਪਸ ext2fs_get_generic_bmap_ਸਿਰਾ((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ ext2_ino_t ext2fs_get_inode_bitmap_end2(ext2fs_inode_bitmap bitmap)
{
	ਵਾਪਸ (ext2_ino_t) ext2fs_get_generic_bmap_ਸਿਰਾ((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ ਪੂਰਨ ext2fs_fast_test_ਖੰਡ_bitmap_range2(ext2fs_ਖੰਡ_bitmap bitmap,
						  blk64_t ਖੰਡ,
						  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ)
{
	ਵਾਪਸ ext2fs_test_ਖੰਡ_bitmap_range2(bitmap, ਖੰਡ, ਅੰਕ);
}

_INLINE_ ਖਾਲੀ ext2fs_fast_mark_ਖੰਡ_bitmap_range2(ext2fs_ਖੰਡ_bitmap bitmap,
						   blk64_t ਖੰਡ,
						   ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ)
{
	ext2fs_mark_ਖੰਡ_bitmap_range2(bitmap, ਖੰਡ, ਅੰਕ);
}

_INLINE_ ਖਾਲੀ ext2fs_fast_unmark_ਖੰਡ_bitmap_range2(ext2fs_ਖੰਡ_bitmap bitmap,
						     blk64_t ਖੰਡ,
						     ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ)
{
	ext2fs_unmark_ਖੰਡ_bitmap_range2(bitmap, ਖੰਡ, ਅੰਕ);
}

#ਪਰਿਭਾਸ਼ਾ_ਰੱਦ _INLINE_
#ਜੇ_ਅੰਤ

ਬਾਹਰੀ ਪੂਰਨ ext2fs_ਸੈੱਟ_ਦੋਆਧਾਰੀ_ਅੰਕ(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ nr,ਖਾਲੀ * addr);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_clear_ਦੋਆਧਾਰੀ_ਅੰਕ(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ nr, ਖਾਲੀ * addr);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_test_ਦੋਆਧਾਰੀ_ਅੰਕ(ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ nr, ਅਚਲ ਖਾਲੀ * addr);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_ਸੈੱਟ_bit64(__u64 nr,ਖਾਲੀ * addr);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_clear_bit64(__u64 nr, ਖਾਲੀ * addr);
ਬਾਹਰੀ ਪੂਰਨ ext2fs_test_bit64(__u64 nr, ਅਚਲ ਖਾਲੀ * addr);
ਬਾਹਰੀ ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ext2fs_bitcount(ਅਚਲ ਖਾਲੀ *addr, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ nbytes);
