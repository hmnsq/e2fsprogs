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

#جيڪڏهن_وصف_ٿيل لفظ_BIGENDIAN
#وصف_ڪريو ext2fs_cpu_منزل_le64(x) ((__force __le64)ext2fs_swab64((__u64)(x)))
#وصف_ڪريو ext2fs_le64_منزل_cpu(x) ext2fs_swab64((__force __u64)(__le64)(x))
#وصف_ڪريو ext2fs_cpu_منزل_le32(x) ((__force __le32)ext2fs_swab32((__u32)(x)))
#وصف_ڪريو ext2fs_le32_منزل_cpu(x) ext2fs_swab32((__force __u32)(__le32)(x))
#وصف_ڪريو ext2fs_cpu_منزل_le16(x) ((__force __le16)ext2fs_swab16((__u16)(x)))
#وصف_ڪريو ext2fs_le16_منزل_cpu(x) ext2fs_swab16((__force __u16)(__le16)(x))

#وصف_ڪريو ext2fs_cpu_منزل_be64(x) ((__force __be64)(__u64)(x))
#وصف_ڪريو ext2fs_be64_منزل_cpu(x) ((__force __u64)(__be64)(x))
#وصف_ڪريو ext2fs_cpu_منزل_be32(x) ((__force __be32)(__u32)(x))
#وصف_ڪريو ext2fs_be32_منزل_cpu(x) ((__force __u32)(__be32)(x))
#وصف_ڪريو ext2fs_cpu_منزل_be16(x) ((__force __be16)(__u16)(x))
#وصف_ڪريو ext2fs_be16_منزل_cpu(x) ((__force __u16)(__be16)(x))
#وإلا
#وصف_ڪريو ext2fs_cpu_منزل_le64(x) ((__force __le64)(__u64)(x))
#وصف_ڪريو ext2fs_le64_منزل_cpu(x) ((__force __u64)(__le64)(x))
#وصف_ڪريو ext2fs_cpu_منزل_le32(x) ((__force __le32)(__u32)(x))
#وصف_ڪريو ext2fs_le32_منزل_cpu(x) ((__force __u32)(__le32)(x))
#وصف_ڪريو ext2fs_cpu_منزل_le16(x) ((__force __le16)(__u16)(x))
#وصف_ڪريو ext2fs_le16_منزل_cpu(x) ((__force __u16)(__le16)(x))

#وصف_ڪريو ext2fs_cpu_منزل_be64(x) ((__force __be64)ext2fs_swab64((__u64)(x)))
#وصف_ڪريو ext2fs_be64_منزل_cpu(x) ext2fs_swab64((__force __u64)(__be64)(x))
#وصف_ڪريو ext2fs_cpu_منزل_be32(x) ((__force __be32)ext2fs_swab32((__u32)(x)))
#وصف_ڪريو ext2fs_be32_منزل_cpu(x) ext2fs_swab32((__force __u32)(__be32)(x))
#وصف_ڪريو ext2fs_cpu_منزل_be16(x) ((__force __be16)ext2fs_swab16((__u16)(x)))
#وصف_ڪريو ext2fs_be16_منزل_cpu(x) ext2fs_swab16((__force __u16)(__be16)(x))
#جيڪڏهن_پورو

/*
 * EXT2FS bitmap manipulation routines.
 */

/* Support for sending warning messages from the inline subroutines */
خارجي ثابت حرف *ext2fs_بلاڪ_string;
خارجي ثابت حرف *ext2fs_inode_string;
خارجي ثابت حرف *ext2fs_mark_string;
خارجي ثابت حرف *ext2fs_unmark_string;
خارجي ثابت حرف *ext2fs_test_string;
خارجي فراغ ext2fs_warn_bitmap(errcode_t errcode, بنانشان ڊگهو arg,
			       ثابت حرف *description);
خارجي فراغ ext2fs_warn_bitmap2(ext2fs_generic_bitmap bitmap,
				صحيح ڪوڊ, بنانشان ڊگهو arg);

#جيڪڏهن_وصف_ٿيل NO_INLINE_FUNCS
خارجي فراغ ext2fs_fast_سيٽ_ٻٽو_انگ(بنانشان صحيح nr,فراغ * addr);
خارجي فراغ ext2fs_fast_clear_ٻٽو_انگ(بنانشان صحيح nr, فراغ * addr);
خارجي فراغ ext2fs_fast_سيٽ_bit64(__u64 nr,فراغ * addr);
خارجي فراغ ext2fs_fast_clear_bit64(__u64 nr, فراغ * addr);
خارجي __u16 ext2fs_swab16(__u16 val);
خارجي __u32 ext2fs_swab32(__u32 val);
خارجي __u64 ext2fs_swab64(__u64 val);

خارجي صحيح ext2fs_mark_بلاڪ_bitmap(ext2fs_بلاڪ_bitmap bitmap, blk_t بلاڪ);
خارجي صحيح ext2fs_unmark_بلاڪ_bitmap(ext2fs_بلاڪ_bitmap bitmap,
				       blk_t بلاڪ);
خارجي صحيح ext2fs_test_بلاڪ_bitmap(ext2fs_بلاڪ_bitmap bitmap, blk_t بلاڪ);

خارجي صحيح ext2fs_mark_inode_bitmap(ext2fs_inode_bitmap bitmap, ext2_ino_t inode);
خارجي صحيح ext2fs_unmark_inode_bitmap(ext2fs_inode_bitmap bitmap,
				       ext2_ino_t inode);
خارجي صحيح ext2fs_test_inode_bitmap(ext2fs_inode_bitmap bitmap, ext2_ino_t inode);

خارجي فراغ ext2fs_fast_mark_بلاڪ_bitmap(ext2fs_بلاڪ_bitmap bitmap,
					  blk_t بلاڪ);
خارجي فراغ ext2fs_fast_unmark_بلاڪ_bitmap(ext2fs_بلاڪ_bitmap bitmap,
					    blk_t بلاڪ);
خارجي صحيح ext2fs_fast_test_بلاڪ_bitmap(ext2fs_بلاڪ_bitmap bitmap,
					 blk_t بلاڪ);

خارجي فراغ ext2fs_fast_mark_inode_bitmap(ext2fs_inode_bitmap bitmap,
					  ext2_ino_t inode);
خارجي فراغ ext2fs_fast_unmark_inode_bitmap(ext2fs_inode_bitmap bitmap,
					    ext2_ino_t inode);
خارجي صحيح ext2fs_fast_test_inode_bitmap(ext2fs_inode_bitmap bitmap,
					 ext2_ino_t inode);
خارجي blk_t ext2fs_get_بلاڪ_bitmap_شروعات(ext2fs_بلاڪ_bitmap bitmap);
خارجي ext2_ino_t ext2fs_get_inode_bitmap_شروعات(ext2fs_inode_bitmap bitmap);
خارجي blk_t ext2fs_get_بلاڪ_bitmap_پڇاڙي(ext2fs_بلاڪ_bitmap bitmap);
خارجي ext2_ino_t ext2fs_get_inode_bitmap_پڇاڙي(ext2fs_inode_bitmap bitmap);

خارجي فراغ ext2fs_fast_mark_بلاڪ_bitmap_range(ext2fs_بلاڪ_bitmap bitmap,
						blk_t بلاڪ, صحيح انگ);
خارجي فراغ ext2fs_fast_unmark_بلاڪ_bitmap_range(ext2fs_بلاڪ_bitmap bitmap,
						  blk_t بلاڪ, صحيح انگ);
خارجي صحيح ext2fs_fast_test_بلاڪ_bitmap_range(ext2fs_بلاڪ_bitmap bitmap,
					       blk_t بلاڪ, صحيح انگ);
#جيڪڏهن_پورو

/* These functions routines moved to gen_bitmap.c */
خارجي فراغ ext2fs_mark_بلاڪ_bitmap_range(ext2fs_بلاڪ_bitmap bitmap,
					   blk_t بلاڪ, صحيح انگ);
خارجي فراغ ext2fs_unmark_بلاڪ_bitmap_range(ext2fs_بلاڪ_bitmap bitmap,
					     blk_t بلاڪ, صحيح انگ);
خارجي صحيح ext2fs_test_بلاڪ_bitmap_range(ext2fs_بلاڪ_bitmap bitmap,
					  blk_t بلاڪ, صحيح انگ);
خارجي صحيح ext2fs_test_inode_bitmap_range(ext2fs_inode_bitmap bitmap,
					  ext2_ino_t inode, صحيح انگ);
خارجي صحيح ext2fs_mark_generic_bitmap(ext2fs_generic_bitmap bitmap,
					 __u32 bitno);
خارجي صحيح ext2fs_unmark_generic_bitmap(ext2fs_generic_bitmap bitmap,
					   blk_t bitno);
خارجي صحيح ext2fs_test_generic_bitmap(ext2fs_generic_bitmap bitmap,
				      blk_t bitno);
خارجي صحيح ext2fs_test_بلاڪ_bitmap_range(ext2fs_بلاڪ_bitmap bitmap,
					  blk_t بلاڪ, صحيح انگ);
خارجي فراغ ext2fs_سيٽ_bitmap_padding(ext2fs_generic_bitmap map);
خارجي __u32 ext2fs_get_generic_bitmap_شروعات(ext2fs_generic_bitmap bitmap);
خارجي __u32 ext2fs_get_generic_bitmap_پڇاڙي(ext2fs_generic_bitmap bitmap);

/* 64-bit versions */

#جيڪڏهن_وصف_ٿيل NO_INLINE_FUNCS
خارجي صحيح ext2fs_mark_بلاڪ_bitmap2(ext2fs_بلاڪ_bitmap bitmap,
				     blk64_t بلاڪ);
خارجي صحيح ext2fs_unmark_بلاڪ_bitmap2(ext2fs_بلاڪ_bitmap bitmap,
				       blk64_t بلاڪ);
خارجي صحيح ext2fs_test_بلاڪ_bitmap2(ext2fs_بلاڪ_bitmap bitmap,
				     blk64_t بلاڪ);

خارجي صحيح ext2fs_mark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
				     ext2_ino_t inode);
خارجي صحيح ext2fs_unmark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
				       ext2_ino_t inode);
خارجي صحيح ext2fs_test_inode_bitmap2(ext2fs_inode_bitmap bitmap,
				     ext2_ino_t inode);

خارجي فراغ ext2fs_fast_mark_بلاڪ_bitmap2(ext2fs_بلاڪ_bitmap bitmap,
					   blk64_t بلاڪ);
خارجي فراغ ext2fs_fast_unmark_بلاڪ_bitmap2(ext2fs_بلاڪ_bitmap bitmap,
					     blk64_t بلاڪ);
خارجي صحيح ext2fs_fast_test_بلاڪ_bitmap2(ext2fs_بلاڪ_bitmap bitmap,
					  blk64_t بلاڪ);

خارجي فراغ ext2fs_fast_mark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
					   ext2_ino_t inode);
خارجي فراغ ext2fs_fast_unmark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
					    ext2_ino_t inode);
خارجي صحيح ext2fs_fast_test_inode_bitmap2(ext2fs_inode_bitmap bitmap,
					  ext2_ino_t inode);
خارجي errcode_t ext2fs_ڳوليو_پهريون_zero_بلاڪ_bitmap2(ext2fs_بلاڪ_bitmap bitmap,
						      blk64_t شروعات,
						      blk64_t پڇاڙي,
						      blk64_t *ٻاهريون);
خارجي errcode_t ext2fs_ڳوليو_پهريون_zero_inode_bitmap2(ext2fs_inode_bitmap bitmap,
						      ext2_ino_t شروعات,
						      ext2_ino_t پڇاڙي,
						      ext2_ino_t *ٻاهريون);
خارجي errcode_t ext2fs_ڳوليو_پهريون_سيٽ_بلاڪ_bitmap2(ext2fs_بلاڪ_bitmap bitmap,
						     blk64_t شروعات,
						     blk64_t پڇاڙي,
						     blk64_t *ٻاهريون);
خارجي errcode_t ext2fs_ڳوليو_پهريون_سيٽ_inode_bitmap2(ext2fs_inode_bitmap bitmap,
						      ext2_ino_t شروعات,
						      ext2_ino_t پڇاڙي,
						      ext2_ino_t *ٻاهريون);
خارجي blk64_t ext2fs_get_بلاڪ_bitmap_start2(ext2fs_بلاڪ_bitmap bitmap);
خارجي ext2_ino_t ext2fs_get_inode_bitmap_start2(ext2fs_inode_bitmap bitmap);
خارجي blk64_t ext2fs_get_بلاڪ_bitmap_end2(ext2fs_بلاڪ_bitmap bitmap);
خارجي ext2_ino_t ext2fs_get_inode_bitmap_end2(ext2fs_inode_bitmap bitmap);

خارجي صحيح ext2fs_fast_test_بلاڪ_bitmap_range2(ext2fs_بلاڪ_bitmap bitmap,
						blk64_t بلاڪ,
						بنانشان صحيح انگ);
خارجي فراغ ext2fs_fast_mark_بلاڪ_bitmap_range2(ext2fs_بلاڪ_bitmap bitmap,
						 blk64_t بلاڪ,
						 بنانشان صحيح انگ);
خارجي فراغ ext2fs_fast_unmark_بلاڪ_bitmap_range2(ext2fs_بلاڪ_bitmap bitmap,
						   blk64_t بلاڪ,
						   بنانشان صحيح انگ);
#جيڪڏهن_پورو

/* These routines moved to gen_bitmap64.c */
خارجي فراغ ext2fs_clear_generic_bmap(ext2fs_generic_bitmap bitmap);
خارجي errcode_t ext2fs_compare_generic_bmap(errcode_t neq,
					     ext2fs_generic_bitmap bm1,
					     ext2fs_generic_bitmap bm2);
خارجي فراغ ext2fs_سيٽ_generic_bmap_padding(ext2fs_generic_bitmap bmap);
خارجي صحيح ext2fs_mark_generic_bmap(ext2fs_generic_bitmap bitmap,
				    blk64_t bitno);
خارجي صحيح ext2fs_unmark_generic_bmap(ext2fs_generic_bitmap bitmap,
				      blk64_t bitno);
خارجي صحيح ext2fs_test_generic_bmap(ext2fs_generic_bitmap bitmap,
				    blk64_t bitno);
خارجي صحيح ext2fs_test_بلاڪ_bitmap_range2(ext2fs_بلاڪ_bitmap bitmap,
					   blk64_t بلاڪ, بنانشان صحيح انگ);
خارجي __u64 ext2fs_get_generic_bmap_شروعات(ext2fs_generic_bitmap bitmap);
خارجي __u64 ext2fs_get_generic_bmap_پڇاڙي(ext2fs_generic_bitmap bitmap);
خارجي صحيح ext2fs_test_بلاڪ_bitmap_range2(ext2fs_بلاڪ_bitmap bitmap,
					   blk64_t بلاڪ, بنانشان صحيح انگ);
خارجي فراغ ext2fs_mark_بلاڪ_bitmap_range2(ext2fs_بلاڪ_bitmap bitmap,
					    blk64_t بلاڪ, بنانشان صحيح انگ);
خارجي فراغ ext2fs_unmark_بلاڪ_bitmap_range2(ext2fs_بلاڪ_bitmap bitmap,
					      blk64_t بلاڪ, بنانشان صحيح انگ);
خارجي errcode_t ext2fs_ڳوليو_پهريون_zero_generic_bmap(ext2fs_generic_bitmap bitmap,
						     __u64 شروعات, __u64 پڇاڙي,
						     __u64 *ٻاهريون);
خارجي errcode_t ext2fs_ڳوليو_پهريون_سيٽ_generic_bmap(ext2fs_generic_bitmap bitmap,
						    __u64 شروعات, __u64 پڇاڙي,
						    __u64 *ٻاهريون);

/*
 * The inline routines themselves...
 *
 * If NO_INLINE_FUNCS is defined, then we won't try to do inline
 * functions at all; they will be included as normal functions in
 * inline.c
 */

#إذا (defined(INCLUDE_INLINE_FUNCS) || !defined(NO_INLINE_FUNCS))
#جيڪڏهن_وصف_ٿيل INCLUDE_INLINE_FUNCS
#إذا (__STDC_VERSION__ >= 199901L)
#وصف_ڪريو _INLINE_ خارجي مضمن
#وإلا
#وصف_ڪريو _INLINE_ مضمن
#جيڪڏهن_پورو
#وإلا /* !INCLUDE_INLINE FUNCS */
#إذا (__STDC_VERSION__ >= 199901L)
#وصف_ڪريو _INLINE_ مضمن
#وإلا /* not C99 */
#جيڪڏهن_وصف_ٿيل __GNUC__
#وصف_ڪريو _INLINE_ خارجي __inline__
#وإلا				/* For Watcom C */
#وصف_ڪريو _INLINE_ خارجي مضمن
#جيڪڏهن_پورو /* __GNUC__ */
#جيڪڏهن_پورو /* __STDC_VERSION__ >= 199901L */
#جيڪڏهن_پورو /* INCLUDE_INLINE_FUNCS */

/*
 * Fast bit set/clear functions that doesn't need to return the
 * previous bit value.
 */

_INLINE_ فراغ ext2fs_fast_سيٽ_ٻٽو_انگ(بنانشان صحيح nr,فراغ * addr)
{
	بنانشان حرف	*ADDR = (بنانشان حرف *) addr;

	ADDR += nr >> 3;
	*ADDR |= (بنانشان حرف) (1 << (nr & 0x07));
}

_INLINE_ فراغ ext2fs_fast_clear_ٻٽو_انگ(بنانشان صحيح nr, فراغ * addr)
{
	بنانشان حرف	*ADDR = (بنانشان حرف *) addr;

	ADDR += nr >> 3;
	*ADDR &= (بنانشان حرف) ~(1 << (nr & 0x07));
}


_INLINE_ فراغ ext2fs_fast_سيٽ_bit64(__u64 nr, فراغ * addr)
{
	بنانشان حرف	*ADDR = (بنانشان حرف *) addr;

	ADDR += nr >> 3;
	*ADDR |= (بنانشان حرف) (1 << (nr & 0x07));
}

_INLINE_ فراغ ext2fs_fast_clear_bit64(__u64 nr, فراغ * addr)
{
	بنانشان حرف	*ADDR = (بنانشان حرف *) addr;

	ADDR += nr >> 3;
	*ADDR &= (بنانشان حرف) ~(1 << (nr & 0x07));
}

_INLINE_ __u16 ext2fs_swab16(__u16 val)
{
	أرجع (val >> 8) | (__u16) (val << 8);
}

_INLINE_ __u32 ext2fs_swab32(__u32 val)
{
	أرجع ((val>>24) | ((val>>8)&0xFF00) |
		((val<<8)&0xFF0000) | (val<<24));
}

_INLINE_ __u64 ext2fs_swab64(__u64 val)
{
	أرجع (ext2fs_swab32((__u32) (val >> 32)) |
		(((__u64)ext2fs_swab32(val & 0xFFFFFFFFUL)) << 32));
}

_INLINE_ صحيح ext2fs_mark_بلاڪ_bitmap(ext2fs_بلاڪ_bitmap bitmap,
				       blk_t بلاڪ)
{
	أرجع ext2fs_mark_generic_bitmap((ext2fs_generic_bitmap) bitmap,
					  بلاڪ);
}

_INLINE_ صحيح ext2fs_unmark_بلاڪ_bitmap(ext2fs_بلاڪ_bitmap bitmap,
					 blk_t بلاڪ)
{
	أرجع ext2fs_unmark_generic_bitmap((ext2fs_generic_bitmap) bitmap,
					    بلاڪ);
}

_INLINE_ صحيح ext2fs_test_بلاڪ_bitmap(ext2fs_بلاڪ_bitmap bitmap,
				       blk_t بلاڪ)
{
	أرجع ext2fs_test_generic_bitmap((ext2fs_generic_bitmap) bitmap,
					  بلاڪ);
}

_INLINE_ صحيح ext2fs_mark_inode_bitmap(ext2fs_inode_bitmap bitmap,
				       ext2_ino_t inode)
{
	أرجع ext2fs_mark_generic_bitmap((ext2fs_generic_bitmap) bitmap,
					  inode);
}

_INLINE_ صحيح ext2fs_unmark_inode_bitmap(ext2fs_inode_bitmap bitmap,
					 ext2_ino_t inode)
{
	أرجع ext2fs_unmark_generic_bitmap((ext2fs_generic_bitmap) bitmap,
				     inode);
}

_INLINE_ صحيح ext2fs_test_inode_bitmap(ext2fs_inode_bitmap bitmap,
				       ext2_ino_t inode)
{
	أرجع ext2fs_test_generic_bitmap((ext2fs_generic_bitmap) bitmap,
					  inode);
}

_INLINE_ فراغ ext2fs_fast_mark_بلاڪ_bitmap(ext2fs_بلاڪ_bitmap bitmap,
					    blk_t بلاڪ)
{
	ext2fs_mark_generic_bitmap((ext2fs_generic_bitmap) bitmap, بلاڪ);
}

_INLINE_ فراغ ext2fs_fast_unmark_بلاڪ_bitmap(ext2fs_بلاڪ_bitmap bitmap,
					      blk_t بلاڪ)
{
	ext2fs_unmark_generic_bitmap((ext2fs_generic_bitmap) bitmap, بلاڪ);
}

_INLINE_ صحيح ext2fs_fast_test_بلاڪ_bitmap(ext2fs_بلاڪ_bitmap bitmap,
					    blk_t بلاڪ)
{
	أرجع ext2fs_test_generic_bitmap((ext2fs_generic_bitmap) bitmap,
					  بلاڪ);
}

_INLINE_ فراغ ext2fs_fast_mark_inode_bitmap(ext2fs_inode_bitmap bitmap,
					    ext2_ino_t inode)
{
	ext2fs_mark_generic_bitmap((ext2fs_generic_bitmap) bitmap, inode);
}

_INLINE_ فراغ ext2fs_fast_unmark_inode_bitmap(ext2fs_inode_bitmap bitmap,
					      ext2_ino_t inode)
{
	ext2fs_unmark_generic_bitmap((ext2fs_generic_bitmap) bitmap, inode);
}

_INLINE_ صحيح ext2fs_fast_test_inode_bitmap(ext2fs_inode_bitmap bitmap,
					   ext2_ino_t inode)
{
	أرجع ext2fs_test_generic_bitmap((ext2fs_generic_bitmap) bitmap,
					  inode);
}

_INLINE_ blk_t ext2fs_get_بلاڪ_bitmap_شروعات(ext2fs_بلاڪ_bitmap bitmap)
{
	أرجع ext2fs_get_generic_bitmap_شروعات((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ ext2_ino_t ext2fs_get_inode_bitmap_شروعات(ext2fs_inode_bitmap bitmap)
{
	أرجع ext2fs_get_generic_bitmap_شروعات((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ blk_t ext2fs_get_بلاڪ_bitmap_پڇاڙي(ext2fs_بلاڪ_bitmap bitmap)
{
	أرجع ext2fs_get_generic_bitmap_پڇاڙي((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ ext2_ino_t ext2fs_get_inode_bitmap_پڇاڙي(ext2fs_inode_bitmap bitmap)
{
	أرجع ext2fs_get_generic_bitmap_پڇاڙي((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ صحيح ext2fs_fast_test_بلاڪ_bitmap_range(ext2fs_بلاڪ_bitmap bitmap,
						 blk_t بلاڪ, صحيح انگ)
{
	أرجع ext2fs_test_بلاڪ_bitmap_range(bitmap, بلاڪ, انگ);
}

_INLINE_ فراغ ext2fs_fast_mark_بلاڪ_bitmap_range(ext2fs_بلاڪ_bitmap bitmap,
						  blk_t بلاڪ, صحيح انگ)
{
	ext2fs_mark_بلاڪ_bitmap_range(bitmap, بلاڪ, انگ);
}

_INLINE_ فراغ ext2fs_fast_unmark_بلاڪ_bitmap_range(ext2fs_بلاڪ_bitmap bitmap,
						    blk_t بلاڪ, صحيح انگ)
{
	ext2fs_unmark_بلاڪ_bitmap_range(bitmap, بلاڪ, انگ);
}

/* 64-bit versions */

_INLINE_ صحيح ext2fs_mark_بلاڪ_bitmap2(ext2fs_بلاڪ_bitmap bitmap,
				       blk64_t بلاڪ)
{
	أرجع ext2fs_mark_generic_bmap((ext2fs_generic_bitmap) bitmap,
					بلاڪ);
}

_INLINE_ صحيح ext2fs_unmark_بلاڪ_bitmap2(ext2fs_بلاڪ_bitmap bitmap,
					 blk64_t بلاڪ)
{
	أرجع ext2fs_unmark_generic_bmap((ext2fs_generic_bitmap) bitmap, بلاڪ);
}

_INLINE_ صحيح ext2fs_test_بلاڪ_bitmap2(ext2fs_بلاڪ_bitmap bitmap,
				       blk64_t بلاڪ)
{
	أرجع ext2fs_test_generic_bmap((ext2fs_generic_bitmap) bitmap,
					بلاڪ);
}

_INLINE_ صحيح ext2fs_mark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
				       ext2_ino_t inode)
{
	أرجع ext2fs_mark_generic_bmap((ext2fs_generic_bitmap) bitmap,
					inode);
}

_INLINE_ صحيح ext2fs_unmark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
					 ext2_ino_t inode)
{
	أرجع ext2fs_unmark_generic_bmap((ext2fs_generic_bitmap) bitmap,
					  inode);
}

_INLINE_ صحيح ext2fs_test_inode_bitmap2(ext2fs_inode_bitmap bitmap,
				       ext2_ino_t inode)
{
	أرجع ext2fs_test_generic_bmap((ext2fs_generic_bitmap) bitmap,
					inode);
}

_INLINE_ فراغ ext2fs_fast_mark_بلاڪ_bitmap2(ext2fs_بلاڪ_bitmap bitmap,
					     blk64_t بلاڪ)
{
	ext2fs_mark_generic_bmap((ext2fs_generic_bitmap) bitmap, بلاڪ);
}

_INLINE_ فراغ ext2fs_fast_unmark_بلاڪ_bitmap2(ext2fs_بلاڪ_bitmap bitmap,
					       blk64_t بلاڪ)
{
	ext2fs_unmark_generic_bmap((ext2fs_generic_bitmap) bitmap, بلاڪ);
}

_INLINE_ صحيح ext2fs_fast_test_بلاڪ_bitmap2(ext2fs_بلاڪ_bitmap bitmap,
					    blk64_t بلاڪ)
{
	أرجع ext2fs_test_generic_bmap((ext2fs_generic_bitmap) bitmap,
					بلاڪ);
}

_INLINE_ فراغ ext2fs_fast_mark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
					     ext2_ino_t inode)
{
	ext2fs_mark_generic_bmap((ext2fs_generic_bitmap) bitmap, inode);
}

_INLINE_ فراغ ext2fs_fast_unmark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
					       ext2_ino_t inode)
{
	ext2fs_unmark_generic_bmap((ext2fs_generic_bitmap) bitmap, inode);
}

_INLINE_ صحيح ext2fs_fast_test_inode_bitmap2(ext2fs_inode_bitmap bitmap,
					    ext2_ino_t inode)
{
	أرجع ext2fs_test_generic_bmap((ext2fs_generic_bitmap) bitmap,
					inode);
}

_INLINE_ errcode_t ext2fs_ڳوليو_پهريون_zero_بلاڪ_bitmap2(ext2fs_بلاڪ_bitmap bitmap,
							blk64_t شروعات,
							blk64_t پڇاڙي,
							blk64_t *ٻاهريون)
{
	__u64 o;
	errcode_t rv;

	rv = ext2fs_ڳوليو_پهريون_zero_generic_bmap((ext2fs_generic_bitmap) bitmap,
						 شروعات, پڇاڙي, &o);
	إذا (!rv)
		*ٻاهريون = o;
	أرجع rv;
}

_INLINE_ errcode_t ext2fs_ڳوليو_پهريون_zero_inode_bitmap2(ext2fs_inode_bitmap bitmap,
							ext2_ino_t شروعات,
							ext2_ino_t پڇاڙي,
							ext2_ino_t *ٻاهريون)
{
	__u64 o;
	errcode_t rv;

	rv = ext2fs_ڳوليو_پهريون_zero_generic_bmap((ext2fs_generic_bitmap) bitmap,
						 شروعات, پڇاڙي, &o);
	إذا (!rv)
		*ٻاهريون = (ext2_ino_t) o;
	أرجع rv;
}

_INLINE_ errcode_t ext2fs_ڳوليو_پهريون_سيٽ_بلاڪ_bitmap2(ext2fs_بلاڪ_bitmap bitmap,
						       blk64_t شروعات,
						       blk64_t پڇاڙي,
						       blk64_t *ٻاهريون)
{
	__u64 o;
	errcode_t rv;

	rv = ext2fs_ڳوليو_پهريون_سيٽ_generic_bmap((ext2fs_generic_bitmap) bitmap,
						شروعات, پڇاڙي, &o);
	إذا (!rv)
		*ٻاهريون = o;
	أرجع rv;
}

_INLINE_ errcode_t ext2fs_ڳوليو_پهريون_سيٽ_inode_bitmap2(ext2fs_inode_bitmap bitmap,
						       ext2_ino_t شروعات,
						       ext2_ino_t پڇاڙي,
						       ext2_ino_t *ٻاهريون)
{
	__u64 o;
	errcode_t rv;

	rv = ext2fs_ڳوليو_پهريون_سيٽ_generic_bmap((ext2fs_generic_bitmap) bitmap,
						شروعات, پڇاڙي, &o);
	إذا (!rv)
		*ٻاهريون = (ext2_ino_t) o;
	أرجع rv;
}

_INLINE_ blk64_t ext2fs_get_بلاڪ_bitmap_start2(ext2fs_بلاڪ_bitmap bitmap)
{
	أرجع ext2fs_get_generic_bmap_شروعات((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ ext2_ino_t ext2fs_get_inode_bitmap_start2(ext2fs_inode_bitmap bitmap)
{
	أرجع (ext2_ino_t) ext2fs_get_generic_bmap_شروعات((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ blk64_t ext2fs_get_بلاڪ_bitmap_end2(ext2fs_بلاڪ_bitmap bitmap)
{
	أرجع ext2fs_get_generic_bmap_پڇاڙي((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ ext2_ino_t ext2fs_get_inode_bitmap_end2(ext2fs_inode_bitmap bitmap)
{
	أرجع (ext2_ino_t) ext2fs_get_generic_bmap_پڇاڙي((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ صحيح ext2fs_fast_test_بلاڪ_bitmap_range2(ext2fs_بلاڪ_bitmap bitmap,
						  blk64_t بلاڪ,
						  بنانشان صحيح انگ)
{
	أرجع ext2fs_test_بلاڪ_bitmap_range2(bitmap, بلاڪ, انگ);
}

_INLINE_ فراغ ext2fs_fast_mark_بلاڪ_bitmap_range2(ext2fs_بلاڪ_bitmap bitmap,
						   blk64_t بلاڪ,
						   بنانشان صحيح انگ)
{
	ext2fs_mark_بلاڪ_bitmap_range2(bitmap, بلاڪ, انگ);
}

_INLINE_ فراغ ext2fs_fast_unmark_بلاڪ_bitmap_range2(ext2fs_بلاڪ_bitmap bitmap,
						     blk64_t بلاڪ,
						     بنانشان صحيح انگ)
{
	ext2fs_unmark_بلاڪ_bitmap_range2(bitmap, بلاڪ, انگ);
}

#وصف_رد _INLINE_
#جيڪڏهن_پورو

خارجي صحيح ext2fs_سيٽ_ٻٽو_انگ(بنانشان صحيح nr,فراغ * addr);
خارجي صحيح ext2fs_clear_ٻٽو_انگ(بنانشان صحيح nr, فراغ * addr);
خارجي صحيح ext2fs_test_ٻٽو_انگ(بنانشان صحيح nr, ثابت فراغ * addr);
خارجي صحيح ext2fs_سيٽ_bit64(__u64 nr,فراغ * addr);
خارجي صحيح ext2fs_clear_bit64(__u64 nr, فراغ * addr);
خارجي صحيح ext2fs_test_bit64(__u64 nr, ثابت فراغ * addr);
خارجي بنانشان صحيح ext2fs_bitcount(ثابت فراغ *addr, بنانشان صحيح nbytes);
