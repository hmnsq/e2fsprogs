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

#جے_تعریف_شدہ الفاظ_BIGENDIAN
#تعریف_کرو ext2fs_cpu_منزل_le64(x) ((__force __le64)ext2fs_swab64((__u64)(x)))
#تعریف_کرو ext2fs_le64_منزل_cpu(x) ext2fs_swab64((__force __u64)(__le64)(x))
#تعریف_کرو ext2fs_cpu_منزل_le32(x) ((__force __le32)ext2fs_swab32((__u32)(x)))
#تعریف_کرو ext2fs_le32_منزل_cpu(x) ext2fs_swab32((__force __u32)(__le32)(x))
#تعریف_کرو ext2fs_cpu_منزل_le16(x) ((__force __le16)ext2fs_swab16((__u16)(x)))
#تعریف_کرو ext2fs_le16_منزل_cpu(x) ext2fs_swab16((__force __u16)(__le16)(x))

#تعریف_کرو ext2fs_cpu_منزل_be64(x) ((__force __be64)(__u64)(x))
#تعریف_کرو ext2fs_be64_منزل_cpu(x) ((__force __u64)(__be64)(x))
#تعریف_کرو ext2fs_cpu_منزل_be32(x) ((__force __be32)(__u32)(x))
#تعریف_کرو ext2fs_be32_منزل_cpu(x) ((__force __u32)(__be32)(x))
#تعریف_کرو ext2fs_cpu_منزل_be16(x) ((__force __be16)(__u16)(x))
#تعریف_کرو ext2fs_be16_منزل_cpu(x) ((__force __u16)(__be16)(x))
#وإلا
#تعریف_کرو ext2fs_cpu_منزل_le64(x) ((__force __le64)(__u64)(x))
#تعریف_کرو ext2fs_le64_منزل_cpu(x) ((__force __u64)(__le64)(x))
#تعریف_کرو ext2fs_cpu_منزل_le32(x) ((__force __le32)(__u32)(x))
#تعریف_کرو ext2fs_le32_منزل_cpu(x) ((__force __u32)(__le32)(x))
#تعریف_کرو ext2fs_cpu_منزل_le16(x) ((__force __le16)(__u16)(x))
#تعریف_کرو ext2fs_le16_منزل_cpu(x) ((__force __u16)(__le16)(x))

#تعریف_کرو ext2fs_cpu_منزل_be64(x) ((__force __be64)ext2fs_swab64((__u64)(x)))
#تعریف_کرو ext2fs_be64_منزل_cpu(x) ext2fs_swab64((__force __u64)(__be64)(x))
#تعریف_کرو ext2fs_cpu_منزل_be32(x) ((__force __be32)ext2fs_swab32((__u32)(x)))
#تعریف_کرو ext2fs_be32_منزل_cpu(x) ext2fs_swab32((__force __u32)(__be32)(x))
#تعریف_کرو ext2fs_cpu_منزل_be16(x) ((__force __be16)ext2fs_swab16((__u16)(x)))
#تعریف_کرو ext2fs_be16_منزل_cpu(x) ext2fs_swab16((__force __u16)(__be16)(x))
#جے_انت

/*
 * EXT2FS bitmap manipulation routines.
 */

/* Support for sending warning messages from the inline subroutines */
خارجي ثابت حرف *ext2fs_بلاک_string;
خارجي ثابت حرف *ext2fs_inode_string;
خارجي ثابت حرف *ext2fs_mark_string;
خارجي ثابت حرف *ext2fs_unmark_string;
خارجي ثابت حرف *ext2fs_test_string;
خارجي فراغ ext2fs_warn_bitmap(errcode_t errcode, بنانشانی لما arg,
			       ثابت حرف *description);
خارجي فراغ ext2fs_warn_bitmap2(ext2fs_generic_bitmap bitmap,
				صحيح کوڈ, بنانشانی لما arg);

#جے_تعریف_شدہ NO_INLINE_FUNCS
خارجي فراغ ext2fs_fast_سیٹ_ثنائی_ہندسہ(بنانشانی صحيح nr,فراغ * addr);
خارجي فراغ ext2fs_fast_clear_ثنائی_ہندسہ(بنانشانی صحيح nr, فراغ * addr);
خارجي فراغ ext2fs_fast_سیٹ_bit64(__u64 nr,فراغ * addr);
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
خارجي blk_t ext2fs_get_بلاک_bitmap_شروع(ext2fs_بلاک_bitmap bitmap);
خارجي ext2_ino_t ext2fs_get_inode_bitmap_شروع(ext2fs_inode_bitmap bitmap);
خارجي blk_t ext2fs_get_بلاک_bitmap_سرا(ext2fs_بلاک_bitmap bitmap);
خارجي ext2_ino_t ext2fs_get_inode_bitmap_سرا(ext2fs_inode_bitmap bitmap);

خارجي فراغ ext2fs_fast_mark_بلاک_bitmap_range(ext2fs_بلاک_bitmap bitmap,
						blk_t بلاک, صحيح عدد);
خارجي فراغ ext2fs_fast_unmark_بلاک_bitmap_range(ext2fs_بلاک_bitmap bitmap,
						  blk_t بلاک, صحيح عدد);
خارجي صحيح ext2fs_fast_test_بلاک_bitmap_range(ext2fs_بلاک_bitmap bitmap,
					       blk_t بلاک, صحيح عدد);
#جے_انت

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
خارجي فراغ ext2fs_سیٹ_bitmap_padding(ext2fs_generic_bitmap map);
خارجي __u32 ext2fs_get_generic_bitmap_شروع(ext2fs_generic_bitmap bitmap);
خارجي __u32 ext2fs_get_generic_bitmap_سرا(ext2fs_generic_bitmap bitmap);

/* 64-bit versions */

#جے_تعریف_شدہ NO_INLINE_FUNCS
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
خارجي errcode_t ext2fs_لبھو_پہلا_zero_بلاک_bitmap2(ext2fs_بلاک_bitmap bitmap,
						      blk64_t شروع,
						      blk64_t سرا,
						      blk64_t *باہرلا);
خارجي errcode_t ext2fs_لبھو_پہلا_zero_inode_bitmap2(ext2fs_inode_bitmap bitmap,
						      ext2_ino_t شروع,
						      ext2_ino_t سرا,
						      ext2_ino_t *باہرلا);
خارجي errcode_t ext2fs_لبھو_پہلا_سیٹ_بلاک_bitmap2(ext2fs_بلاک_bitmap bitmap,
						     blk64_t شروع,
						     blk64_t سرا,
						     blk64_t *باہرلا);
خارجي errcode_t ext2fs_لبھو_پہلا_سیٹ_inode_bitmap2(ext2fs_inode_bitmap bitmap,
						      ext2_ino_t شروع,
						      ext2_ino_t سرا,
						      ext2_ino_t *باہرلا);
خارجي blk64_t ext2fs_get_بلاک_bitmap_start2(ext2fs_بلاک_bitmap bitmap);
خارجي ext2_ino_t ext2fs_get_inode_bitmap_start2(ext2fs_inode_bitmap bitmap);
خارجي blk64_t ext2fs_get_بلاک_bitmap_end2(ext2fs_بلاک_bitmap bitmap);
خارجي ext2_ino_t ext2fs_get_inode_bitmap_end2(ext2fs_inode_bitmap bitmap);

خارجي صحيح ext2fs_fast_test_بلاک_bitmap_range2(ext2fs_بلاک_bitmap bitmap,
						blk64_t بلاک,
						بنانشانی صحيح عدد);
خارجي فراغ ext2fs_fast_mark_بلاک_bitmap_range2(ext2fs_بلاک_bitmap bitmap,
						 blk64_t بلاک,
						 بنانشانی صحيح عدد);
خارجي فراغ ext2fs_fast_unmark_بلاک_bitmap_range2(ext2fs_بلاک_bitmap bitmap,
						   blk64_t بلاک,
						   بنانشانی صحيح عدد);
#جے_انت

/* These routines moved to gen_bitmap64.c */
خارجي فراغ ext2fs_clear_generic_bmap(ext2fs_generic_bitmap bitmap);
خارجي errcode_t ext2fs_compare_generic_bmap(errcode_t neq,
					     ext2fs_generic_bitmap bm1,
					     ext2fs_generic_bitmap bm2);
خارجي فراغ ext2fs_سیٹ_generic_bmap_padding(ext2fs_generic_bitmap bmap);
خارجي صحيح ext2fs_mark_generic_bmap(ext2fs_generic_bitmap bitmap,
				    blk64_t bitno);
خارجي صحيح ext2fs_unmark_generic_bmap(ext2fs_generic_bitmap bitmap,
				      blk64_t bitno);
خارجي صحيح ext2fs_test_generic_bmap(ext2fs_generic_bitmap bitmap,
				    blk64_t bitno);
خارجي صحيح ext2fs_test_بلاک_bitmap_range2(ext2fs_بلاک_bitmap bitmap,
					   blk64_t بلاک, بنانشانی صحيح عدد);
خارجي __u64 ext2fs_get_generic_bmap_شروع(ext2fs_generic_bitmap bitmap);
خارجي __u64 ext2fs_get_generic_bmap_سرا(ext2fs_generic_bitmap bitmap);
خارجي صحيح ext2fs_test_بلاک_bitmap_range2(ext2fs_بلاک_bitmap bitmap,
					   blk64_t بلاک, بنانشانی صحيح عدد);
خارجي فراغ ext2fs_mark_بلاک_bitmap_range2(ext2fs_بلاک_bitmap bitmap,
					    blk64_t بلاک, بنانشانی صحيح عدد);
خارجي فراغ ext2fs_unmark_بلاک_bitmap_range2(ext2fs_بلاک_bitmap bitmap,
					      blk64_t بلاک, بنانشانی صحيح عدد);
خارجي errcode_t ext2fs_لبھو_پہلا_zero_generic_bmap(ext2fs_generic_bitmap bitmap,
						     __u64 شروع, __u64 سرا,
						     __u64 *باہرلا);
خارجي errcode_t ext2fs_لبھو_پہلا_سیٹ_generic_bmap(ext2fs_generic_bitmap bitmap,
						    __u64 شروع, __u64 سرا,
						    __u64 *باہرلا);

/*
 * The inline routines themselves...
 *
 * If NO_INLINE_FUNCS is defined, then we won't try to do inline
 * functions at all; they will be included as normal functions in
 * inline.c
 */

#إذا (defined(INCLUDE_INLINE_FUNCS) || !defined(NO_INLINE_FUNCS))
#جے_تعریف_شدہ INCLUDE_INLINE_FUNCS
#إذا (__STDC_VERSION__ >= 199901L)
#تعریف_کرو _INLINE_ خارجي مضمن
#وإلا
#تعریف_کرو _INLINE_ مضمن
#جے_انت
#وإلا /* !INCLUDE_INLINE FUNCS */
#إذا (__STDC_VERSION__ >= 199901L)
#تعریف_کرو _INLINE_ مضمن
#وإلا /* not C99 */
#جے_تعریف_شدہ __GNUC__
#تعریف_کرو _INLINE_ خارجي __inline__
#وإلا				/* For Watcom C */
#تعریف_کرو _INLINE_ خارجي مضمن
#جے_انت /* __GNUC__ */
#جے_انت /* __STDC_VERSION__ >= 199901L */
#جے_انت /* INCLUDE_INLINE_FUNCS */

/*
 * Fast bit set/clear functions that doesn't need to return the
 * previous bit value.
 */

_INLINE_ فراغ ext2fs_fast_سیٹ_ثنائی_ہندسہ(بنانشانی صحيح nr,فراغ * addr)
{
	بنانشانی حرف	*ADDR = (بنانشانی حرف *) addr;

	ADDR += nr >> 3;
	*ADDR |= (بنانشانی حرف) (1 << (nr & 0x07));
}

_INLINE_ فراغ ext2fs_fast_clear_ثنائی_ہندسہ(بنانشانی صحيح nr, فراغ * addr)
{
	بنانشانی حرف	*ADDR = (بنانشانی حرف *) addr;

	ADDR += nr >> 3;
	*ADDR &= (بنانشانی حرف) ~(1 << (nr & 0x07));
}


_INLINE_ فراغ ext2fs_fast_سیٹ_bit64(__u64 nr, فراغ * addr)
{
	بنانشانی حرف	*ADDR = (بنانشانی حرف *) addr;

	ADDR += nr >> 3;
	*ADDR |= (بنانشانی حرف) (1 << (nr & 0x07));
}

_INLINE_ فراغ ext2fs_fast_clear_bit64(__u64 nr, فراغ * addr)
{
	بنانشانی حرف	*ADDR = (بنانشانی حرف *) addr;

	ADDR += nr >> 3;
	*ADDR &= (بنانشانی حرف) ~(1 << (nr & 0x07));
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

_INLINE_ blk_t ext2fs_get_بلاک_bitmap_شروع(ext2fs_بلاک_bitmap bitmap)
{
	أرجع ext2fs_get_generic_bitmap_شروع((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ ext2_ino_t ext2fs_get_inode_bitmap_شروع(ext2fs_inode_bitmap bitmap)
{
	أرجع ext2fs_get_generic_bitmap_شروع((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ blk_t ext2fs_get_بلاک_bitmap_سرا(ext2fs_بلاک_bitmap bitmap)
{
	أرجع ext2fs_get_generic_bitmap_سرا((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ ext2_ino_t ext2fs_get_inode_bitmap_سرا(ext2fs_inode_bitmap bitmap)
{
	أرجع ext2fs_get_generic_bitmap_سرا((ext2fs_generic_bitmap) bitmap);
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

_INLINE_ errcode_t ext2fs_لبھو_پہلا_zero_بلاک_bitmap2(ext2fs_بلاک_bitmap bitmap,
							blk64_t شروع,
							blk64_t سرا,
							blk64_t *باہرلا)
{
	__u64 o;
	errcode_t rv;

	rv = ext2fs_لبھو_پہلا_zero_generic_bmap((ext2fs_generic_bitmap) bitmap,
						 شروع, سرا, &o);
	إذا (!rv)
		*باہرلا = o;
	أرجع rv;
}

_INLINE_ errcode_t ext2fs_لبھو_پہلا_zero_inode_bitmap2(ext2fs_inode_bitmap bitmap,
							ext2_ino_t شروع,
							ext2_ino_t سرا,
							ext2_ino_t *باہرلا)
{
	__u64 o;
	errcode_t rv;

	rv = ext2fs_لبھو_پہلا_zero_generic_bmap((ext2fs_generic_bitmap) bitmap,
						 شروع, سرا, &o);
	إذا (!rv)
		*باہرلا = (ext2_ino_t) o;
	أرجع rv;
}

_INLINE_ errcode_t ext2fs_لبھو_پہلا_سیٹ_بلاک_bitmap2(ext2fs_بلاک_bitmap bitmap,
						       blk64_t شروع,
						       blk64_t سرا,
						       blk64_t *باہرلا)
{
	__u64 o;
	errcode_t rv;

	rv = ext2fs_لبھو_پہلا_سیٹ_generic_bmap((ext2fs_generic_bitmap) bitmap,
						شروع, سرا, &o);
	إذا (!rv)
		*باہرلا = o;
	أرجع rv;
}

_INLINE_ errcode_t ext2fs_لبھو_پہلا_سیٹ_inode_bitmap2(ext2fs_inode_bitmap bitmap,
						       ext2_ino_t شروع,
						       ext2_ino_t سرا,
						       ext2_ino_t *باہرلا)
{
	__u64 o;
	errcode_t rv;

	rv = ext2fs_لبھو_پہلا_سیٹ_generic_bmap((ext2fs_generic_bitmap) bitmap,
						شروع, سرا, &o);
	إذا (!rv)
		*باہرلا = (ext2_ino_t) o;
	أرجع rv;
}

_INLINE_ blk64_t ext2fs_get_بلاک_bitmap_start2(ext2fs_بلاک_bitmap bitmap)
{
	أرجع ext2fs_get_generic_bmap_شروع((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ ext2_ino_t ext2fs_get_inode_bitmap_start2(ext2fs_inode_bitmap bitmap)
{
	أرجع (ext2_ino_t) ext2fs_get_generic_bmap_شروع((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ blk64_t ext2fs_get_بلاک_bitmap_end2(ext2fs_بلاک_bitmap bitmap)
{
	أرجع ext2fs_get_generic_bmap_سرا((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ ext2_ino_t ext2fs_get_inode_bitmap_end2(ext2fs_inode_bitmap bitmap)
{
	أرجع (ext2_ino_t) ext2fs_get_generic_bmap_سرا((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ صحيح ext2fs_fast_test_بلاک_bitmap_range2(ext2fs_بلاک_bitmap bitmap,
						  blk64_t بلاک,
						  بنانشانی صحيح عدد)
{
	أرجع ext2fs_test_بلاک_bitmap_range2(bitmap, بلاک, عدد);
}

_INLINE_ فراغ ext2fs_fast_mark_بلاک_bitmap_range2(ext2fs_بلاک_bitmap bitmap,
						   blk64_t بلاک,
						   بنانشانی صحيح عدد)
{
	ext2fs_mark_بلاک_bitmap_range2(bitmap, بلاک, عدد);
}

_INLINE_ فراغ ext2fs_fast_unmark_بلاک_bitmap_range2(ext2fs_بلاک_bitmap bitmap,
						     blk64_t بلاک,
						     بنانشانی صحيح عدد)
{
	ext2fs_unmark_بلاک_bitmap_range2(bitmap, بلاک, عدد);
}

#تعریف_رد _INLINE_
#جے_انت

خارجي صحيح ext2fs_سیٹ_ثنائی_ہندسہ(بنانشانی صحيح nr,فراغ * addr);
خارجي صحيح ext2fs_clear_ثنائی_ہندسہ(بنانشانی صحيح nr, فراغ * addr);
خارجي صحيح ext2fs_test_ثنائی_ہندسہ(بنانشانی صحيح nr, ثابت فراغ * addr);
خارجي صحيح ext2fs_سیٹ_bit64(__u64 nr,فراغ * addr);
خارجي صحيح ext2fs_clear_bit64(__u64 nr, فراغ * addr);
خارجي صحيح ext2fs_test_bit64(__u64 nr, ثابت فراغ * addr);
خارجي بنانشانی صحيح ext2fs_bitcount(ثابت فراغ *addr, بنانشانی صحيح nbytes);
