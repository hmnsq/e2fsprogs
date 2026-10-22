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

#که_تعریف_شوی ټکي_BIGENDIAN
#تعریف_کړه ext2fs_cpu_منزل_le64(x) ((__force __le64)ext2fs_swab64((__u64)(x)))
#تعریف_کړه ext2fs_le64_منزل_cpu(x) ext2fs_swab64((__force __u64)(__le64)(x))
#تعریف_کړه ext2fs_cpu_منزل_le32(x) ((__force __le32)ext2fs_swab32((__u32)(x)))
#تعریف_کړه ext2fs_le32_منزل_cpu(x) ext2fs_swab32((__force __u32)(__le32)(x))
#تعریف_کړه ext2fs_cpu_منزل_le16(x) ((__force __le16)ext2fs_swab16((__u16)(x)))
#تعریف_کړه ext2fs_le16_منزل_cpu(x) ext2fs_swab16((__force __u16)(__le16)(x))

#تعریف_کړه ext2fs_cpu_منزل_be64(x) ((__force __be64)(__u64)(x))
#تعریف_کړه ext2fs_be64_منزل_cpu(x) ((__force __u64)(__be64)(x))
#تعریف_کړه ext2fs_cpu_منزل_be32(x) ((__force __be32)(__u32)(x))
#تعریف_کړه ext2fs_be32_منزل_cpu(x) ((__force __u32)(__be32)(x))
#تعریف_کړه ext2fs_cpu_منزل_be16(x) ((__force __be16)(__u16)(x))
#تعریف_کړه ext2fs_be16_منزل_cpu(x) ((__force __u16)(__be16)(x))
#وإلا
#تعریف_کړه ext2fs_cpu_منزل_le64(x) ((__force __le64)(__u64)(x))
#تعریف_کړه ext2fs_le64_منزل_cpu(x) ((__force __u64)(__le64)(x))
#تعریف_کړه ext2fs_cpu_منزل_le32(x) ((__force __le32)(__u32)(x))
#تعریف_کړه ext2fs_le32_منزل_cpu(x) ((__force __u32)(__le32)(x))
#تعریف_کړه ext2fs_cpu_منزل_le16(x) ((__force __le16)(__u16)(x))
#تعریف_کړه ext2fs_le16_منزل_cpu(x) ((__force __u16)(__le16)(x))

#تعریف_کړه ext2fs_cpu_منزل_be64(x) ((__force __be64)ext2fs_swab64((__u64)(x)))
#تعریف_کړه ext2fs_be64_منزل_cpu(x) ext2fs_swab64((__force __u64)(__be64)(x))
#تعریف_کړه ext2fs_cpu_منزل_be32(x) ((__force __be32)ext2fs_swab32((__u32)(x)))
#تعریف_کړه ext2fs_be32_منزل_cpu(x) ext2fs_swab32((__force __u32)(__be32)(x))
#تعریف_کړه ext2fs_cpu_منزل_be16(x) ((__force __be16)ext2fs_swab16((__u16)(x)))
#تعریف_کړه ext2fs_be16_منزل_cpu(x) ext2fs_swab16((__force __u16)(__be16)(x))
#د_که_پای

/*
 * EXT2FS bitmap manipulation routines.
 */

/* Support for sending warning messages from the inline subroutines */
خارجي ثابت حرف *ext2fs_بلاک_string;
خارجي ثابت حرف *ext2fs_inode_string;
خارجي ثابت حرف *ext2fs_mark_string;
خارجي ثابت حرف *ext2fs_unmark_string;
خارجي ثابت حرف *ext2fs_test_string;
خارجي فراغ ext2fs_warn_bitmap(errcode_t errcode, بینښه اوږد arg,
			       ثابت حرف *description);
خارجي فراغ ext2fs_warn_bitmap2(ext2fs_generic_bitmap bitmap,
				صحيح کوډ, بینښه اوږد arg);

#که_تعریف_شوی NO_INLINE_FUNCS
خارجي فراغ ext2fs_fast_سېټ_بټ(بینښه صحيح nr,فراغ * addr);
خارجي فراغ ext2fs_fast_clear_بټ(بینښه صحيح nr, فراغ * addr);
خارجي فراغ ext2fs_fast_سېټ_bit64(__u64 nr,فراغ * addr);
خارجي فراغ ext2fs_fast_clear_bit64(__u64 nr, فراغ * addr);
خارجي __u16 ext2fs_swab16(__u16 val);
خارجي __u32 ext2fs_swab32(__u32 val);
خارجي __u64 ext2fs_swab64(__u64 val);

خارجي صحيح ext2fs_mark_بلاک_bitmap(ext2fs_بلاک_bitmap bitmap, blk_t بلاک);
خارجي صحيح ext2fs_unmark_بلاک_bitmap(ext2fs_بلاک_bitmap bitmap,
				       blk_t بلاک);
خارجي صحيح ext2fs_test_بلاک_bitmap(ext2fs_بلاک_bitmap bitmap, blk_t بلاک);

خارجي صحيح ext2fs_mark_inode_bitmap(ext2fs_inode_bitmap bitmap, ext2_ino_t inode);
خارجي صحيح ext2fs_unmark_inode_bitmap(ext2fs_inode_bitmap bitmap,
				       ext2_ino_t inode);
خارجي صحيح ext2fs_test_inode_bitmap(ext2fs_inode_bitmap bitmap, ext2_ino_t inode);

خارجي فراغ ext2fs_fast_mark_بلاک_bitmap(ext2fs_بلاک_bitmap bitmap,
					  blk_t بلاک);
خارجي فراغ ext2fs_fast_unmark_بلاک_bitmap(ext2fs_بلاک_bitmap bitmap,
					    blk_t بلاک);
خارجي صحيح ext2fs_fast_test_بلاک_bitmap(ext2fs_بلاک_bitmap bitmap,
					 blk_t بلاک);

خارجي فراغ ext2fs_fast_mark_inode_bitmap(ext2fs_inode_bitmap bitmap,
					  ext2_ino_t inode);
خارجي فراغ ext2fs_fast_unmark_inode_bitmap(ext2fs_inode_bitmap bitmap,
					    ext2_ino_t inode);
خارجي صحيح ext2fs_fast_test_inode_bitmap(ext2fs_inode_bitmap bitmap,
					 ext2_ino_t inode);
خارجي blk_t ext2fs_get_بلاک_bitmap_پیل(ext2fs_بلاک_bitmap bitmap);
خارجي ext2_ino_t ext2fs_get_inode_bitmap_پیل(ext2fs_inode_bitmap bitmap);
خارجي blk_t ext2fs_get_بلاک_bitmap_پای_ټکی(ext2fs_بلاک_bitmap bitmap);
خارجي ext2_ino_t ext2fs_get_inode_bitmap_پای_ټکی(ext2fs_inode_bitmap bitmap);

خارجي فراغ ext2fs_fast_mark_بلاک_bitmap_range(ext2fs_بلاک_bitmap bitmap,
						blk_t بلاک, صحيح عدد);
خارجي فراغ ext2fs_fast_unmark_بلاک_bitmap_range(ext2fs_بلاک_bitmap bitmap,
						  blk_t بلاک, صحيح عدد);
خارجي صحيح ext2fs_fast_test_بلاک_bitmap_range(ext2fs_بلاک_bitmap bitmap,
					       blk_t بلاک, صحيح عدد);
#د_که_پای

/* These functions routines moved to gen_bitmap.c */
خارجي فراغ ext2fs_mark_بلاک_bitmap_range(ext2fs_بلاک_bitmap bitmap,
					   blk_t بلاک, صحيح عدد);
خارجي فراغ ext2fs_unmark_بلاک_bitmap_range(ext2fs_بلاک_bitmap bitmap,
					     blk_t بلاک, صحيح عدد);
خارجي صحيح ext2fs_test_بلاک_bitmap_range(ext2fs_بلاک_bitmap bitmap,
					  blk_t بلاک, صحيح عدد);
خارجي صحيح ext2fs_test_inode_bitmap_range(ext2fs_inode_bitmap bitmap,
					  ext2_ino_t inode, صحيح عدد);
خارجي صحيح ext2fs_mark_generic_bitmap(ext2fs_generic_bitmap bitmap,
					 __u32 bitno);
خارجي صحيح ext2fs_unmark_generic_bitmap(ext2fs_generic_bitmap bitmap,
					   blk_t bitno);
خارجي صحيح ext2fs_test_generic_bitmap(ext2fs_generic_bitmap bitmap,
				      blk_t bitno);
خارجي صحيح ext2fs_test_بلاک_bitmap_range(ext2fs_بلاک_bitmap bitmap,
					  blk_t بلاک, صحيح عدد);
خارجي فراغ ext2fs_سېټ_bitmap_padding(ext2fs_generic_bitmap map);
خارجي __u32 ext2fs_get_generic_bitmap_پیل(ext2fs_generic_bitmap bitmap);
خارجي __u32 ext2fs_get_generic_bitmap_پای_ټکی(ext2fs_generic_bitmap bitmap);

/* 64-bit versions */

#که_تعریف_شوی NO_INLINE_FUNCS
خارجي صحيح ext2fs_mark_بلاک_bitmap2(ext2fs_بلاک_bitmap bitmap,
				     blk64_t بلاک);
خارجي صحيح ext2fs_unmark_بلاک_bitmap2(ext2fs_بلاک_bitmap bitmap,
				       blk64_t بلاک);
خارجي صحيح ext2fs_test_بلاک_bitmap2(ext2fs_بلاک_bitmap bitmap,
				     blk64_t بلاک);

خارجي صحيح ext2fs_mark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
				     ext2_ino_t inode);
خارجي صحيح ext2fs_unmark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
				       ext2_ino_t inode);
خارجي صحيح ext2fs_test_inode_bitmap2(ext2fs_inode_bitmap bitmap,
				     ext2_ino_t inode);

خارجي فراغ ext2fs_fast_mark_بلاک_bitmap2(ext2fs_بلاک_bitmap bitmap,
					   blk64_t بلاک);
خارجي فراغ ext2fs_fast_unmark_بلاک_bitmap2(ext2fs_بلاک_bitmap bitmap,
					     blk64_t بلاک);
خارجي صحيح ext2fs_fast_test_بلاک_bitmap2(ext2fs_بلاک_bitmap bitmap,
					  blk64_t بلاک);

خارجي فراغ ext2fs_fast_mark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
					   ext2_ino_t inode);
خارجي فراغ ext2fs_fast_unmark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
					    ext2_ino_t inode);
خارجي صحيح ext2fs_fast_test_inode_bitmap2(ext2fs_inode_bitmap bitmap,
					  ext2_ino_t inode);
خارجي errcode_t ext2fs_پیدا_کړه_لومړی_zero_بلاک_bitmap2(ext2fs_بلاک_bitmap bitmap,
						      blk64_t پیل,
						      blk64_t پای_ټکی,
						      blk64_t *بهر);
خارجي errcode_t ext2fs_پیدا_کړه_لومړی_zero_inode_bitmap2(ext2fs_inode_bitmap bitmap,
						      ext2_ino_t پیل,
						      ext2_ino_t پای_ټکی,
						      ext2_ino_t *بهر);
خارجي errcode_t ext2fs_پیدا_کړه_لومړی_سېټ_بلاک_bitmap2(ext2fs_بلاک_bitmap bitmap,
						     blk64_t پیل,
						     blk64_t پای_ټکی,
						     blk64_t *بهر);
خارجي errcode_t ext2fs_پیدا_کړه_لومړی_سېټ_inode_bitmap2(ext2fs_inode_bitmap bitmap,
						      ext2_ino_t پیل,
						      ext2_ino_t پای_ټکی,
						      ext2_ino_t *بهر);
خارجي blk64_t ext2fs_get_بلاک_bitmap_start2(ext2fs_بلاک_bitmap bitmap);
خارجي ext2_ino_t ext2fs_get_inode_bitmap_start2(ext2fs_inode_bitmap bitmap);
خارجي blk64_t ext2fs_get_بلاک_bitmap_end2(ext2fs_بلاک_bitmap bitmap);
خارجي ext2_ino_t ext2fs_get_inode_bitmap_end2(ext2fs_inode_bitmap bitmap);

خارجي صحيح ext2fs_fast_test_بلاک_bitmap_range2(ext2fs_بلاک_bitmap bitmap,
						blk64_t بلاک,
						بینښه صحيح عدد);
خارجي فراغ ext2fs_fast_mark_بلاک_bitmap_range2(ext2fs_بلاک_bitmap bitmap,
						 blk64_t بلاک,
						 بینښه صحيح عدد);
خارجي فراغ ext2fs_fast_unmark_بلاک_bitmap_range2(ext2fs_بلاک_bitmap bitmap,
						   blk64_t بلاک,
						   بینښه صحيح عدد);
#د_که_پای

/* These routines moved to gen_bitmap64.c */
خارجي فراغ ext2fs_clear_generic_bmap(ext2fs_generic_bitmap bitmap);
خارجي errcode_t ext2fs_compare_generic_bmap(errcode_t neq,
					     ext2fs_generic_bitmap bm1,
					     ext2fs_generic_bitmap bm2);
خارجي فراغ ext2fs_سېټ_generic_bmap_padding(ext2fs_generic_bitmap bmap);
خارجي صحيح ext2fs_mark_generic_bmap(ext2fs_generic_bitmap bitmap,
				    blk64_t bitno);
خارجي صحيح ext2fs_unmark_generic_bmap(ext2fs_generic_bitmap bitmap,
				      blk64_t bitno);
خارجي صحيح ext2fs_test_generic_bmap(ext2fs_generic_bitmap bitmap,
				    blk64_t bitno);
خارجي صحيح ext2fs_test_بلاک_bitmap_range2(ext2fs_بلاک_bitmap bitmap,
					   blk64_t بلاک, بینښه صحيح عدد);
خارجي __u64 ext2fs_get_generic_bmap_پیل(ext2fs_generic_bitmap bitmap);
خارجي __u64 ext2fs_get_generic_bmap_پای_ټکی(ext2fs_generic_bitmap bitmap);
خارجي صحيح ext2fs_test_بلاک_bitmap_range2(ext2fs_بلاک_bitmap bitmap,
					   blk64_t بلاک, بینښه صحيح عدد);
خارجي فراغ ext2fs_mark_بلاک_bitmap_range2(ext2fs_بلاک_bitmap bitmap,
					    blk64_t بلاک, بینښه صحيح عدد);
خارجي فراغ ext2fs_unmark_بلاک_bitmap_range2(ext2fs_بلاک_bitmap bitmap,
					      blk64_t بلاک, بینښه صحيح عدد);
خارجي errcode_t ext2fs_پیدا_کړه_لومړی_zero_generic_bmap(ext2fs_generic_bitmap bitmap,
						     __u64 پیل, __u64 پای_ټکی,
						     __u64 *بهر);
خارجي errcode_t ext2fs_پیدا_کړه_لومړی_سېټ_generic_bmap(ext2fs_generic_bitmap bitmap,
						    __u64 پیل, __u64 پای_ټکی,
						    __u64 *بهر);

/*
 * The inline routines themselves...
 *
 * If NO_INLINE_FUNCS is defined, then we won't try to do inline
 * functions at all; they will be included as normal functions in
 * inline.c
 */

#إذا (defined(INCLUDE_INLINE_FUNCS) || !defined(NO_INLINE_FUNCS))
#که_تعریف_شوی INCLUDE_INLINE_FUNCS
#إذا (__STDC_VERSION__ >= 199901L)
#تعریف_کړه _INLINE_ خارجي مضمن
#وإلا
#تعریف_کړه _INLINE_ مضمن
#د_که_پای
#وإلا /* !INCLUDE_INLINE FUNCS */
#إذا (__STDC_VERSION__ >= 199901L)
#تعریف_کړه _INLINE_ مضمن
#وإلا /* not C99 */
#که_تعریف_شوی __GNUC__
#تعریف_کړه _INLINE_ خارجي __inline__
#وإلا				/* For Watcom C */
#تعریف_کړه _INLINE_ خارجي مضمن
#د_که_پای /* __GNUC__ */
#د_که_پای /* __STDC_VERSION__ >= 199901L */
#د_که_پای /* INCLUDE_INLINE_FUNCS */

/*
 * Fast bit set/clear functions that doesn't need to return the
 * previous bit value.
 */

_INLINE_ فراغ ext2fs_fast_سېټ_بټ(بینښه صحيح nr,فراغ * addr)
{
	بینښه حرف	*ADDR = (بینښه حرف *) addr;

	ADDR += nr >> 3;
	*ADDR |= (بینښه حرف) (1 << (nr & 0x07));
}

_INLINE_ فراغ ext2fs_fast_clear_بټ(بینښه صحيح nr, فراغ * addr)
{
	بینښه حرف	*ADDR = (بینښه حرف *) addr;

	ADDR += nr >> 3;
	*ADDR &= (بینښه حرف) ~(1 << (nr & 0x07));
}


_INLINE_ فراغ ext2fs_fast_سېټ_bit64(__u64 nr, فراغ * addr)
{
	بینښه حرف	*ADDR = (بینښه حرف *) addr;

	ADDR += nr >> 3;
	*ADDR |= (بینښه حرف) (1 << (nr & 0x07));
}

_INLINE_ فراغ ext2fs_fast_clear_bit64(__u64 nr, فراغ * addr)
{
	بینښه حرف	*ADDR = (بینښه حرف *) addr;

	ADDR += nr >> 3;
	*ADDR &= (بینښه حرف) ~(1 << (nr & 0x07));
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

_INLINE_ صحيح ext2fs_mark_بلاک_bitmap(ext2fs_بلاک_bitmap bitmap,
				       blk_t بلاک)
{
	أرجع ext2fs_mark_generic_bitmap((ext2fs_generic_bitmap) bitmap,
					  بلاک);
}

_INLINE_ صحيح ext2fs_unmark_بلاک_bitmap(ext2fs_بلاک_bitmap bitmap,
					 blk_t بلاک)
{
	أرجع ext2fs_unmark_generic_bitmap((ext2fs_generic_bitmap) bitmap,
					    بلاک);
}

_INLINE_ صحيح ext2fs_test_بلاک_bitmap(ext2fs_بلاک_bitmap bitmap,
				       blk_t بلاک)
{
	أرجع ext2fs_test_generic_bitmap((ext2fs_generic_bitmap) bitmap,
					  بلاک);
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

_INLINE_ فراغ ext2fs_fast_mark_بلاک_bitmap(ext2fs_بلاک_bitmap bitmap,
					    blk_t بلاک)
{
	ext2fs_mark_generic_bitmap((ext2fs_generic_bitmap) bitmap, بلاک);
}

_INLINE_ فراغ ext2fs_fast_unmark_بلاک_bitmap(ext2fs_بلاک_bitmap bitmap,
					      blk_t بلاک)
{
	ext2fs_unmark_generic_bitmap((ext2fs_generic_bitmap) bitmap, بلاک);
}

_INLINE_ صحيح ext2fs_fast_test_بلاک_bitmap(ext2fs_بلاک_bitmap bitmap,
					    blk_t بلاک)
{
	أرجع ext2fs_test_generic_bitmap((ext2fs_generic_bitmap) bitmap,
					  بلاک);
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

_INLINE_ blk_t ext2fs_get_بلاک_bitmap_پیل(ext2fs_بلاک_bitmap bitmap)
{
	أرجع ext2fs_get_generic_bitmap_پیل((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ ext2_ino_t ext2fs_get_inode_bitmap_پیل(ext2fs_inode_bitmap bitmap)
{
	أرجع ext2fs_get_generic_bitmap_پیل((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ blk_t ext2fs_get_بلاک_bitmap_پای_ټکی(ext2fs_بلاک_bitmap bitmap)
{
	أرجع ext2fs_get_generic_bitmap_پای_ټکی((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ ext2_ino_t ext2fs_get_inode_bitmap_پای_ټکی(ext2fs_inode_bitmap bitmap)
{
	أرجع ext2fs_get_generic_bitmap_پای_ټکی((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ صحيح ext2fs_fast_test_بلاک_bitmap_range(ext2fs_بلاک_bitmap bitmap,
						 blk_t بلاک, صحيح عدد)
{
	أرجع ext2fs_test_بلاک_bitmap_range(bitmap, بلاک, عدد);
}

_INLINE_ فراغ ext2fs_fast_mark_بلاک_bitmap_range(ext2fs_بلاک_bitmap bitmap,
						  blk_t بلاک, صحيح عدد)
{
	ext2fs_mark_بلاک_bitmap_range(bitmap, بلاک, عدد);
}

_INLINE_ فراغ ext2fs_fast_unmark_بلاک_bitmap_range(ext2fs_بلاک_bitmap bitmap,
						    blk_t بلاک, صحيح عدد)
{
	ext2fs_unmark_بلاک_bitmap_range(bitmap, بلاک, عدد);
}

/* 64-bit versions */

_INLINE_ صحيح ext2fs_mark_بلاک_bitmap2(ext2fs_بلاک_bitmap bitmap,
				       blk64_t بلاک)
{
	أرجع ext2fs_mark_generic_bmap((ext2fs_generic_bitmap) bitmap,
					بلاک);
}

_INLINE_ صحيح ext2fs_unmark_بلاک_bitmap2(ext2fs_بلاک_bitmap bitmap,
					 blk64_t بلاک)
{
	أرجع ext2fs_unmark_generic_bmap((ext2fs_generic_bitmap) bitmap, بلاک);
}

_INLINE_ صحيح ext2fs_test_بلاک_bitmap2(ext2fs_بلاک_bitmap bitmap,
				       blk64_t بلاک)
{
	أرجع ext2fs_test_generic_bmap((ext2fs_generic_bitmap) bitmap,
					بلاک);
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

_INLINE_ فراغ ext2fs_fast_mark_بلاک_bitmap2(ext2fs_بلاک_bitmap bitmap,
					     blk64_t بلاک)
{
	ext2fs_mark_generic_bmap((ext2fs_generic_bitmap) bitmap, بلاک);
}

_INLINE_ فراغ ext2fs_fast_unmark_بلاک_bitmap2(ext2fs_بلاک_bitmap bitmap,
					       blk64_t بلاک)
{
	ext2fs_unmark_generic_bmap((ext2fs_generic_bitmap) bitmap, بلاک);
}

_INLINE_ صحيح ext2fs_fast_test_بلاک_bitmap2(ext2fs_بلاک_bitmap bitmap,
					    blk64_t بلاک)
{
	أرجع ext2fs_test_generic_bmap((ext2fs_generic_bitmap) bitmap,
					بلاک);
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

_INLINE_ errcode_t ext2fs_پیدا_کړه_لومړی_zero_بلاک_bitmap2(ext2fs_بلاک_bitmap bitmap,
							blk64_t پیل,
							blk64_t پای_ټکی,
							blk64_t *بهر)
{
	__u64 o;
	errcode_t rv;

	rv = ext2fs_پیدا_کړه_لومړی_zero_generic_bmap((ext2fs_generic_bitmap) bitmap,
						 پیل, پای_ټکی, &o);
	إذا (!rv)
		*بهر = o;
	أرجع rv;
}

_INLINE_ errcode_t ext2fs_پیدا_کړه_لومړی_zero_inode_bitmap2(ext2fs_inode_bitmap bitmap,
							ext2_ino_t پیل,
							ext2_ino_t پای_ټکی,
							ext2_ino_t *بهر)
{
	__u64 o;
	errcode_t rv;

	rv = ext2fs_پیدا_کړه_لومړی_zero_generic_bmap((ext2fs_generic_bitmap) bitmap,
						 پیل, پای_ټکی, &o);
	إذا (!rv)
		*بهر = (ext2_ino_t) o;
	أرجع rv;
}

_INLINE_ errcode_t ext2fs_پیدا_کړه_لومړی_سېټ_بلاک_bitmap2(ext2fs_بلاک_bitmap bitmap,
						       blk64_t پیل,
						       blk64_t پای_ټکی,
						       blk64_t *بهر)
{
	__u64 o;
	errcode_t rv;

	rv = ext2fs_پیدا_کړه_لومړی_سېټ_generic_bmap((ext2fs_generic_bitmap) bitmap,
						پیل, پای_ټکی, &o);
	إذا (!rv)
		*بهر = o;
	أرجع rv;
}

_INLINE_ errcode_t ext2fs_پیدا_کړه_لومړی_سېټ_inode_bitmap2(ext2fs_inode_bitmap bitmap,
						       ext2_ino_t پیل,
						       ext2_ino_t پای_ټکی,
						       ext2_ino_t *بهر)
{
	__u64 o;
	errcode_t rv;

	rv = ext2fs_پیدا_کړه_لومړی_سېټ_generic_bmap((ext2fs_generic_bitmap) bitmap,
						پیل, پای_ټکی, &o);
	إذا (!rv)
		*بهر = (ext2_ino_t) o;
	أرجع rv;
}

_INLINE_ blk64_t ext2fs_get_بلاک_bitmap_start2(ext2fs_بلاک_bitmap bitmap)
{
	أرجع ext2fs_get_generic_bmap_پیل((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ ext2_ino_t ext2fs_get_inode_bitmap_start2(ext2fs_inode_bitmap bitmap)
{
	أرجع (ext2_ino_t) ext2fs_get_generic_bmap_پیل((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ blk64_t ext2fs_get_بلاک_bitmap_end2(ext2fs_بلاک_bitmap bitmap)
{
	أرجع ext2fs_get_generic_bmap_پای_ټکی((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ ext2_ino_t ext2fs_get_inode_bitmap_end2(ext2fs_inode_bitmap bitmap)
{
	أرجع (ext2_ino_t) ext2fs_get_generic_bmap_پای_ټکی((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ صحيح ext2fs_fast_test_بلاک_bitmap_range2(ext2fs_بلاک_bitmap bitmap,
						  blk64_t بلاک,
						  بینښه صحيح عدد)
{
	أرجع ext2fs_test_بلاک_bitmap_range2(bitmap, بلاک, عدد);
}

_INLINE_ فراغ ext2fs_fast_mark_بلاک_bitmap_range2(ext2fs_بلاک_bitmap bitmap,
						   blk64_t بلاک,
						   بینښه صحيح عدد)
{
	ext2fs_mark_بلاک_bitmap_range2(bitmap, بلاک, عدد);
}

_INLINE_ فراغ ext2fs_fast_unmark_بلاک_bitmap_range2(ext2fs_بلاک_bitmap bitmap,
						     blk64_t بلاک,
						     بینښه صحيح عدد)
{
	ext2fs_unmark_بلاک_bitmap_range2(bitmap, بلاک, عدد);
}

#تعریف_لغوه _INLINE_
#د_که_پای

خارجي صحيح ext2fs_سېټ_بټ(بینښه صحيح nr,فراغ * addr);
خارجي صحيح ext2fs_clear_بټ(بینښه صحيح nr, فراغ * addr);
خارجي صحيح ext2fs_test_بټ(بینښه صحيح nr, ثابت فراغ * addr);
خارجي صحيح ext2fs_سېټ_bit64(__u64 nr,فراغ * addr);
خارجي صحيح ext2fs_clear_bit64(__u64 nr, فراغ * addr);
خارجي صحيح ext2fs_test_bit64(__u64 nr, ثابت فراغ * addr);
خارجي بینښه صحيح ext2fs_bitcount(ثابت فراغ *addr, بینښه صحيح nbytes);
