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

#اگر_تعریف_شدہ الفاظ_BIGENDIAN
#تعریف_کرو ext2fs_cpu_مقصد_le64(x) ((__force __le64)ext2fs_swab64((__u64)(x)))
#تعریف_کرو ext2fs_le64_مقصد_cpu(x) ext2fs_swab64((__force __u64)(__le64)(x))
#تعریف_کرو ext2fs_cpu_مقصد_le32(x) ((__force __le32)ext2fs_swab32((__u32)(x)))
#تعریف_کرو ext2fs_le32_مقصد_cpu(x) ext2fs_swab32((__force __u32)(__le32)(x))
#تعریف_کرو ext2fs_cpu_مقصد_le16(x) ((__force __le16)ext2fs_swab16((__u16)(x)))
#تعریف_کرو ext2fs_le16_مقصد_cpu(x) ext2fs_swab16((__force __u16)(__le16)(x))

#تعریف_کرو ext2fs_cpu_مقصد_be64(x) ((__force __be64)(__u64)(x))
#تعریف_کرو ext2fs_be64_مقصد_cpu(x) ((__force __u64)(__be64)(x))
#تعریف_کرو ext2fs_cpu_مقصد_be32(x) ((__force __be32)(__u32)(x))
#تعریف_کرو ext2fs_be32_مقصد_cpu(x) ((__force __u32)(__be32)(x))
#تعریف_کرو ext2fs_cpu_مقصد_be16(x) ((__force __be16)(__u16)(x))
#تعریف_کرو ext2fs_be16_مقصد_cpu(x) ((__force __u16)(__be16)(x))
#ورنہ
#تعریف_کرو ext2fs_cpu_مقصد_le64(x) ((__force __le64)(__u64)(x))
#تعریف_کرو ext2fs_le64_مقصد_cpu(x) ((__force __u64)(__le64)(x))
#تعریف_کرو ext2fs_cpu_مقصد_le32(x) ((__force __le32)(__u32)(x))
#تعریف_کرو ext2fs_le32_مقصد_cpu(x) ((__force __u32)(__le32)(x))
#تعریف_کرو ext2fs_cpu_مقصد_le16(x) ((__force __le16)(__u16)(x))
#تعریف_کرو ext2fs_le16_مقصد_cpu(x) ((__force __u16)(__le16)(x))

#تعریف_کرو ext2fs_cpu_مقصد_be64(x) ((__force __be64)ext2fs_swab64((__u64)(x)))
#تعریف_کرو ext2fs_be64_مقصد_cpu(x) ext2fs_swab64((__force __u64)(__be64)(x))
#تعریف_کرو ext2fs_cpu_مقصد_be32(x) ((__force __be32)ext2fs_swab32((__u32)(x)))
#تعریف_کرو ext2fs_be32_مقصد_cpu(x) ext2fs_swab32((__force __u32)(__be32)(x))
#تعریف_کرو ext2fs_cpu_مقصد_be16(x) ((__force __be16)ext2fs_swab16((__u16)(x)))
#تعریف_کرو ext2fs_be16_مقصد_cpu(x) ext2fs_swab16((__force __u16)(__be16)(x))
#اگر_ختم

/*
 * EXT2FS bitmap manipulation routines.
 */

/* Support for sending warning messages from the inline subroutines */
خارجی ثابت حرف *ext2fs_بلاک_string;
خارجی ثابت حرف *ext2fs_inode_string;
خارجی ثابت حرف *ext2fs_mark_string;
خارجی ثابت حرف *ext2fs_unmark_string;
خارجی ثابت حرف *ext2fs_test_string;
خارجی خالی ext2fs_warn_bitmap(errcode_t errcode, غیرعلامتی طویل arg,
			       ثابت حرف *description);
خارجی خالی ext2fs_warn_bitmap2(ext2fs_generic_bitmap bitmap,
				صحیح رمز, غیرعلامتی طویل arg);

#اگر_تعریف_شدہ NO_INLINE_FUNCS
خارجی خالی ext2fs_fast_سیٹ_ثنائی_ہندسہ(غیرعلامتی صحیح nr,خالی * addr);
خارجی خالی ext2fs_fast_clear_ثنائی_ہندسہ(غیرعلامتی صحیح nr, خالی * addr);
خارجی خالی ext2fs_fast_سیٹ_bit64(__u64 nr,خالی * addr);
خارجی خالی ext2fs_fast_clear_bit64(__u64 nr, خالی * addr);
خارجی __u16 ext2fs_swab16(__u16 val);
خارجی __u32 ext2fs_swab32(__u32 val);
خارجی __u64 ext2fs_swab64(__u64 val);

خارجی صحیح ext2fs_mark_بلاک_bitmap(ext2fs_بلاک_bitmap bitmap, blk_t بلاک);
خارجی صحیح ext2fs_unmark_بلاک_bitmap(ext2fs_بلاک_bitmap bitmap,
				       blk_t بلاک);
خارجی صحیح ext2fs_test_بلاک_bitmap(ext2fs_بلاک_bitmap bitmap, blk_t بلاک);

خارجی صحیح ext2fs_mark_inode_bitmap(ext2fs_inode_bitmap bitmap, ext2_ino_t inode);
خارجی صحیح ext2fs_unmark_inode_bitmap(ext2fs_inode_bitmap bitmap,
				       ext2_ino_t inode);
خارجی صحیح ext2fs_test_inode_bitmap(ext2fs_inode_bitmap bitmap, ext2_ino_t inode);

خارجی خالی ext2fs_fast_mark_بلاک_bitmap(ext2fs_بلاک_bitmap bitmap,
					  blk_t بلاک);
خارجی خالی ext2fs_fast_unmark_بلاک_bitmap(ext2fs_بلاک_bitmap bitmap,
					    blk_t بلاک);
خارجی صحیح ext2fs_fast_test_بلاک_bitmap(ext2fs_بلاک_bitmap bitmap,
					 blk_t بلاک);

خارجی خالی ext2fs_fast_mark_inode_bitmap(ext2fs_inode_bitmap bitmap,
					  ext2_ino_t inode);
خارجی خالی ext2fs_fast_unmark_inode_bitmap(ext2fs_inode_bitmap bitmap,
					    ext2_ino_t inode);
خارجی صحیح ext2fs_fast_test_inode_bitmap(ext2fs_inode_bitmap bitmap,
					 ext2_ino_t inode);
خارجی blk_t ext2fs_get_بلاک_bitmap_آغاز(ext2fs_بلاک_bitmap bitmap);
خارجی ext2_ino_t ext2fs_get_inode_bitmap_آغاز(ext2fs_inode_bitmap bitmap);
خارجی blk_t ext2fs_get_بلاک_bitmap_انتہا(ext2fs_بلاک_bitmap bitmap);
خارجی ext2_ino_t ext2fs_get_inode_bitmap_انتہا(ext2fs_inode_bitmap bitmap);

خارجی خالی ext2fs_fast_mark_بلاک_bitmap_range(ext2fs_بلاک_bitmap bitmap,
						blk_t بلاک, صحیح عدد);
خارجی خالی ext2fs_fast_unmark_بلاک_bitmap_range(ext2fs_بلاک_bitmap bitmap,
						  blk_t بلاک, صحیح عدد);
خارجی صحیح ext2fs_fast_test_بلاک_bitmap_range(ext2fs_بلاک_bitmap bitmap,
					       blk_t بلاک, صحیح عدد);
#اگر_ختم

/* These functions routines moved to gen_bitmap.c */
خارجی خالی ext2fs_mark_بلاک_bitmap_range(ext2fs_بلاک_bitmap bitmap,
					   blk_t بلاک, صحیح عدد);
خارجی خالی ext2fs_unmark_بلاک_bitmap_range(ext2fs_بلاک_bitmap bitmap,
					     blk_t بلاک, صحیح عدد);
خارجی صحیح ext2fs_test_بلاک_bitmap_range(ext2fs_بلاک_bitmap bitmap,
					  blk_t بلاک, صحیح عدد);
خارجی صحیح ext2fs_test_inode_bitmap_range(ext2fs_inode_bitmap bitmap,
					  ext2_ino_t inode, صحیح عدد);
خارجی صحیح ext2fs_mark_generic_bitmap(ext2fs_generic_bitmap bitmap,
					 __u32 bitno);
خارجی صحیح ext2fs_unmark_generic_bitmap(ext2fs_generic_bitmap bitmap,
					   blk_t bitno);
خارجی صحیح ext2fs_test_generic_bitmap(ext2fs_generic_bitmap bitmap,
				      blk_t bitno);
خارجی صحیح ext2fs_test_بلاک_bitmap_range(ext2fs_بلاک_bitmap bitmap,
					  blk_t بلاک, صحیح عدد);
خارجی خالی ext2fs_سیٹ_bitmap_padding(ext2fs_generic_bitmap map);
خارجی __u32 ext2fs_get_generic_bitmap_آغاز(ext2fs_generic_bitmap bitmap);
خارجی __u32 ext2fs_get_generic_bitmap_انتہا(ext2fs_generic_bitmap bitmap);

/* 64-bit versions */

#اگر_تعریف_شدہ NO_INLINE_FUNCS
خارجی صحیح ext2fs_mark_بلاک_bitmap2(ext2fs_بلاک_bitmap bitmap,
				     blk64_t بلاک);
خارجی صحیح ext2fs_unmark_بلاک_bitmap2(ext2fs_بلاک_bitmap bitmap,
				       blk64_t بلاک);
خارجی صحیح ext2fs_test_بلاک_bitmap2(ext2fs_بلاک_bitmap bitmap,
				     blk64_t بلاک);

خارجی صحیح ext2fs_mark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
				     ext2_ino_t inode);
خارجی صحیح ext2fs_unmark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
				       ext2_ino_t inode);
خارجی صحیح ext2fs_test_inode_bitmap2(ext2fs_inode_bitmap bitmap,
				     ext2_ino_t inode);

خارجی خالی ext2fs_fast_mark_بلاک_bitmap2(ext2fs_بلاک_bitmap bitmap,
					   blk64_t بلاک);
خارجی خالی ext2fs_fast_unmark_بلاک_bitmap2(ext2fs_بلاک_bitmap bitmap,
					     blk64_t بلاک);
خارجی صحیح ext2fs_fast_test_بلاک_bitmap2(ext2fs_بلاک_bitmap bitmap,
					  blk64_t بلاک);

خارجی خالی ext2fs_fast_mark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
					   ext2_ino_t inode);
خارجی خالی ext2fs_fast_unmark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
					    ext2_ino_t inode);
خارجی صحیح ext2fs_fast_test_inode_bitmap2(ext2fs_inode_bitmap bitmap,
					  ext2_ino_t inode);
خارجی errcode_t ext2fs_ڈھونڈو_پہلا_zero_بلاک_bitmap2(ext2fs_بلاک_bitmap bitmap,
						      blk64_t آغاز,
						      blk64_t انتہا,
						      blk64_t *برآمد);
خارجی errcode_t ext2fs_ڈھونڈو_پہلا_zero_inode_bitmap2(ext2fs_inode_bitmap bitmap,
						      ext2_ino_t آغاز,
						      ext2_ino_t انتہا,
						      ext2_ino_t *برآمد);
خارجی errcode_t ext2fs_ڈھونڈو_پہلا_سیٹ_بلاک_bitmap2(ext2fs_بلاک_bitmap bitmap,
						     blk64_t آغاز,
						     blk64_t انتہا,
						     blk64_t *برآمد);
خارجی errcode_t ext2fs_ڈھونڈو_پہلا_سیٹ_inode_bitmap2(ext2fs_inode_bitmap bitmap,
						      ext2_ino_t آغاز,
						      ext2_ino_t انتہا,
						      ext2_ino_t *برآمد);
خارجی blk64_t ext2fs_get_بلاک_bitmap_start2(ext2fs_بلاک_bitmap bitmap);
خارجی ext2_ino_t ext2fs_get_inode_bitmap_start2(ext2fs_inode_bitmap bitmap);
خارجی blk64_t ext2fs_get_بلاک_bitmap_end2(ext2fs_بلاک_bitmap bitmap);
خارجی ext2_ino_t ext2fs_get_inode_bitmap_end2(ext2fs_inode_bitmap bitmap);

خارجی صحیح ext2fs_fast_test_بلاک_bitmap_range2(ext2fs_بلاک_bitmap bitmap,
						blk64_t بلاک,
						غیرعلامتی صحیح عدد);
خارجی خالی ext2fs_fast_mark_بلاک_bitmap_range2(ext2fs_بلاک_bitmap bitmap,
						 blk64_t بلاک,
						 غیرعلامتی صحیح عدد);
خارجی خالی ext2fs_fast_unmark_بلاک_bitmap_range2(ext2fs_بلاک_bitmap bitmap,
						   blk64_t بلاک,
						   غیرعلامتی صحیح عدد);
#اگر_ختم

/* These routines moved to gen_bitmap64.c */
خارجی خالی ext2fs_clear_generic_bmap(ext2fs_generic_bitmap bitmap);
خارجی errcode_t ext2fs_compare_generic_bmap(errcode_t neq,
					     ext2fs_generic_bitmap bm1,
					     ext2fs_generic_bitmap bm2);
خارجی خالی ext2fs_سیٹ_generic_bmap_padding(ext2fs_generic_bitmap bmap);
خارجی صحیح ext2fs_mark_generic_bmap(ext2fs_generic_bitmap bitmap,
				    blk64_t bitno);
خارجی صحیح ext2fs_unmark_generic_bmap(ext2fs_generic_bitmap bitmap,
				      blk64_t bitno);
خارجی صحیح ext2fs_test_generic_bmap(ext2fs_generic_bitmap bitmap,
				    blk64_t bitno);
خارجی صحیح ext2fs_test_بلاک_bitmap_range2(ext2fs_بلاک_bitmap bitmap,
					   blk64_t بلاک, غیرعلامتی صحیح عدد);
خارجی __u64 ext2fs_get_generic_bmap_آغاز(ext2fs_generic_bitmap bitmap);
خارجی __u64 ext2fs_get_generic_bmap_انتہا(ext2fs_generic_bitmap bitmap);
خارجی صحیح ext2fs_test_بلاک_bitmap_range2(ext2fs_بلاک_bitmap bitmap,
					   blk64_t بلاک, غیرعلامتی صحیح عدد);
خارجی خالی ext2fs_mark_بلاک_bitmap_range2(ext2fs_بلاک_bitmap bitmap,
					    blk64_t بلاک, غیرعلامتی صحیح عدد);
خارجی خالی ext2fs_unmark_بلاک_bitmap_range2(ext2fs_بلاک_bitmap bitmap,
					      blk64_t بلاک, غیرعلامتی صحیح عدد);
خارجی errcode_t ext2fs_ڈھونڈو_پہلا_zero_generic_bmap(ext2fs_generic_bitmap bitmap,
						     __u64 آغاز, __u64 انتہا,
						     __u64 *برآمد);
خارجی errcode_t ext2fs_ڈھونڈو_پہلا_سیٹ_generic_bmap(ext2fs_generic_bitmap bitmap,
						    __u64 آغاز, __u64 انتہا,
						    __u64 *برآمد);

/*
 * The inline routines themselves...
 *
 * If NO_INLINE_FUNCS is defined, then we won't try to do inline
 * functions at all; they will be included as normal functions in
 * inline.c
 */

#اگر (defined(INCLUDE_INLINE_FUNCS) || !defined(NO_INLINE_FUNCS))
#اگر_تعریف_شدہ INCLUDE_INLINE_FUNCS
#اگر (__STDC_VERSION__ >= 199901L)
#تعریف_کرو _INLINE_ خارجی اندرونی
#ورنہ
#تعریف_کرو _INLINE_ اندرونی
#اگر_ختم
#ورنہ /* !INCLUDE_INLINE FUNCS */
#اگر (__STDC_VERSION__ >= 199901L)
#تعریف_کرو _INLINE_ اندرونی
#ورنہ /* not C99 */
#اگر_تعریف_شدہ __GNUC__
#تعریف_کرو _INLINE_ خارجی __inline__
#ورنہ				/* For Watcom C */
#تعریف_کرو _INLINE_ خارجی اندرونی
#اگر_ختم /* __GNUC__ */
#اگر_ختم /* __STDC_VERSION__ >= 199901L */
#اگر_ختم /* INCLUDE_INLINE_FUNCS */

/*
 * Fast bit set/clear functions that doesn't need to return the
 * previous bit value.
 */

_INLINE_ خالی ext2fs_fast_سیٹ_ثنائی_ہندسہ(غیرعلامتی صحیح nr,خالی * addr)
{
	غیرعلامتی حرف	*ADDR = (غیرعلامتی حرف *) addr;

	ADDR += nr >> 3;
	*ADDR |= (غیرعلامتی حرف) (1 << (nr & 0x07));
}

_INLINE_ خالی ext2fs_fast_clear_ثنائی_ہندسہ(غیرعلامتی صحیح nr, خالی * addr)
{
	غیرعلامتی حرف	*ADDR = (غیرعلامتی حرف *) addr;

	ADDR += nr >> 3;
	*ADDR &= (غیرعلامتی حرف) ~(1 << (nr & 0x07));
}


_INLINE_ خالی ext2fs_fast_سیٹ_bit64(__u64 nr, خالی * addr)
{
	غیرعلامتی حرف	*ADDR = (غیرعلامتی حرف *) addr;

	ADDR += nr >> 3;
	*ADDR |= (غیرعلامتی حرف) (1 << (nr & 0x07));
}

_INLINE_ خالی ext2fs_fast_clear_bit64(__u64 nr, خالی * addr)
{
	غیرعلامتی حرف	*ADDR = (غیرعلامتی حرف *) addr;

	ADDR += nr >> 3;
	*ADDR &= (غیرعلامتی حرف) ~(1 << (nr & 0x07));
}

_INLINE_ __u16 ext2fs_swab16(__u16 val)
{
	واپس (val >> 8) | (__u16) (val << 8);
}

_INLINE_ __u32 ext2fs_swab32(__u32 val)
{
	واپس ((val>>24) | ((val>>8)&0xFF00) |
		((val<<8)&0xFF0000) | (val<<24));
}

_INLINE_ __u64 ext2fs_swab64(__u64 val)
{
	واپس (ext2fs_swab32((__u32) (val >> 32)) |
		(((__u64)ext2fs_swab32(val & 0xFFFFFFFFUL)) << 32));
}

_INLINE_ صحیح ext2fs_mark_بلاک_bitmap(ext2fs_بلاک_bitmap bitmap,
				       blk_t بلاک)
{
	واپس ext2fs_mark_generic_bitmap((ext2fs_generic_bitmap) bitmap,
					  بلاک);
}

_INLINE_ صحیح ext2fs_unmark_بلاک_bitmap(ext2fs_بلاک_bitmap bitmap,
					 blk_t بلاک)
{
	واپس ext2fs_unmark_generic_bitmap((ext2fs_generic_bitmap) bitmap,
					    بلاک);
}

_INLINE_ صحیح ext2fs_test_بلاک_bitmap(ext2fs_بلاک_bitmap bitmap,
				       blk_t بلاک)
{
	واپس ext2fs_test_generic_bitmap((ext2fs_generic_bitmap) bitmap,
					  بلاک);
}

_INLINE_ صحیح ext2fs_mark_inode_bitmap(ext2fs_inode_bitmap bitmap,
				       ext2_ino_t inode)
{
	واپس ext2fs_mark_generic_bitmap((ext2fs_generic_bitmap) bitmap,
					  inode);
}

_INLINE_ صحیح ext2fs_unmark_inode_bitmap(ext2fs_inode_bitmap bitmap,
					 ext2_ino_t inode)
{
	واپس ext2fs_unmark_generic_bitmap((ext2fs_generic_bitmap) bitmap,
				     inode);
}

_INLINE_ صحیح ext2fs_test_inode_bitmap(ext2fs_inode_bitmap bitmap,
				       ext2_ino_t inode)
{
	واپس ext2fs_test_generic_bitmap((ext2fs_generic_bitmap) bitmap,
					  inode);
}

_INLINE_ خالی ext2fs_fast_mark_بلاک_bitmap(ext2fs_بلاک_bitmap bitmap,
					    blk_t بلاک)
{
	ext2fs_mark_generic_bitmap((ext2fs_generic_bitmap) bitmap, بلاک);
}

_INLINE_ خالی ext2fs_fast_unmark_بلاک_bitmap(ext2fs_بلاک_bitmap bitmap,
					      blk_t بلاک)
{
	ext2fs_unmark_generic_bitmap((ext2fs_generic_bitmap) bitmap, بلاک);
}

_INLINE_ صحیح ext2fs_fast_test_بلاک_bitmap(ext2fs_بلاک_bitmap bitmap,
					    blk_t بلاک)
{
	واپس ext2fs_test_generic_bitmap((ext2fs_generic_bitmap) bitmap,
					  بلاک);
}

_INLINE_ خالی ext2fs_fast_mark_inode_bitmap(ext2fs_inode_bitmap bitmap,
					    ext2_ino_t inode)
{
	ext2fs_mark_generic_bitmap((ext2fs_generic_bitmap) bitmap, inode);
}

_INLINE_ خالی ext2fs_fast_unmark_inode_bitmap(ext2fs_inode_bitmap bitmap,
					      ext2_ino_t inode)
{
	ext2fs_unmark_generic_bitmap((ext2fs_generic_bitmap) bitmap, inode);
}

_INLINE_ صحیح ext2fs_fast_test_inode_bitmap(ext2fs_inode_bitmap bitmap,
					   ext2_ino_t inode)
{
	واپس ext2fs_test_generic_bitmap((ext2fs_generic_bitmap) bitmap,
					  inode);
}

_INLINE_ blk_t ext2fs_get_بلاک_bitmap_آغاز(ext2fs_بلاک_bitmap bitmap)
{
	واپس ext2fs_get_generic_bitmap_آغاز((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ ext2_ino_t ext2fs_get_inode_bitmap_آغاز(ext2fs_inode_bitmap bitmap)
{
	واپس ext2fs_get_generic_bitmap_آغاز((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ blk_t ext2fs_get_بلاک_bitmap_انتہا(ext2fs_بلاک_bitmap bitmap)
{
	واپس ext2fs_get_generic_bitmap_انتہا((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ ext2_ino_t ext2fs_get_inode_bitmap_انتہا(ext2fs_inode_bitmap bitmap)
{
	واپس ext2fs_get_generic_bitmap_انتہا((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ صحیح ext2fs_fast_test_بلاک_bitmap_range(ext2fs_بلاک_bitmap bitmap,
						 blk_t بلاک, صحیح عدد)
{
	واپس ext2fs_test_بلاک_bitmap_range(bitmap, بلاک, عدد);
}

_INLINE_ خالی ext2fs_fast_mark_بلاک_bitmap_range(ext2fs_بلاک_bitmap bitmap,
						  blk_t بلاک, صحیح عدد)
{
	ext2fs_mark_بلاک_bitmap_range(bitmap, بلاک, عدد);
}

_INLINE_ خالی ext2fs_fast_unmark_بلاک_bitmap_range(ext2fs_بلاک_bitmap bitmap,
						    blk_t بلاک, صحیح عدد)
{
	ext2fs_unmark_بلاک_bitmap_range(bitmap, بلاک, عدد);
}

/* 64-bit versions */

_INLINE_ صحیح ext2fs_mark_بلاک_bitmap2(ext2fs_بلاک_bitmap bitmap,
				       blk64_t بلاک)
{
	واپس ext2fs_mark_generic_bmap((ext2fs_generic_bitmap) bitmap,
					بلاک);
}

_INLINE_ صحیح ext2fs_unmark_بلاک_bitmap2(ext2fs_بلاک_bitmap bitmap,
					 blk64_t بلاک)
{
	واپس ext2fs_unmark_generic_bmap((ext2fs_generic_bitmap) bitmap, بلاک);
}

_INLINE_ صحیح ext2fs_test_بلاک_bitmap2(ext2fs_بلاک_bitmap bitmap,
				       blk64_t بلاک)
{
	واپس ext2fs_test_generic_bmap((ext2fs_generic_bitmap) bitmap,
					بلاک);
}

_INLINE_ صحیح ext2fs_mark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
				       ext2_ino_t inode)
{
	واپس ext2fs_mark_generic_bmap((ext2fs_generic_bitmap) bitmap,
					inode);
}

_INLINE_ صحیح ext2fs_unmark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
					 ext2_ino_t inode)
{
	واپس ext2fs_unmark_generic_bmap((ext2fs_generic_bitmap) bitmap,
					  inode);
}

_INLINE_ صحیح ext2fs_test_inode_bitmap2(ext2fs_inode_bitmap bitmap,
				       ext2_ino_t inode)
{
	واپس ext2fs_test_generic_bmap((ext2fs_generic_bitmap) bitmap,
					inode);
}

_INLINE_ خالی ext2fs_fast_mark_بلاک_bitmap2(ext2fs_بلاک_bitmap bitmap,
					     blk64_t بلاک)
{
	ext2fs_mark_generic_bmap((ext2fs_generic_bitmap) bitmap, بلاک);
}

_INLINE_ خالی ext2fs_fast_unmark_بلاک_bitmap2(ext2fs_بلاک_bitmap bitmap,
					       blk64_t بلاک)
{
	ext2fs_unmark_generic_bmap((ext2fs_generic_bitmap) bitmap, بلاک);
}

_INLINE_ صحیح ext2fs_fast_test_بلاک_bitmap2(ext2fs_بلاک_bitmap bitmap,
					    blk64_t بلاک)
{
	واپس ext2fs_test_generic_bmap((ext2fs_generic_bitmap) bitmap,
					بلاک);
}

_INLINE_ خالی ext2fs_fast_mark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
					     ext2_ino_t inode)
{
	ext2fs_mark_generic_bmap((ext2fs_generic_bitmap) bitmap, inode);
}

_INLINE_ خالی ext2fs_fast_unmark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
					       ext2_ino_t inode)
{
	ext2fs_unmark_generic_bmap((ext2fs_generic_bitmap) bitmap, inode);
}

_INLINE_ صحیح ext2fs_fast_test_inode_bitmap2(ext2fs_inode_bitmap bitmap,
					    ext2_ino_t inode)
{
	واپس ext2fs_test_generic_bmap((ext2fs_generic_bitmap) bitmap,
					inode);
}

_INLINE_ errcode_t ext2fs_ڈھونڈو_پہلا_zero_بلاک_bitmap2(ext2fs_بلاک_bitmap bitmap,
							blk64_t آغاز,
							blk64_t انتہا,
							blk64_t *برآمد)
{
	__u64 o;
	errcode_t rv;

	rv = ext2fs_ڈھونڈو_پہلا_zero_generic_bmap((ext2fs_generic_bitmap) bitmap,
						 آغاز, انتہا, &o);
	اگر (!rv)
		*برآمد = o;
	واپس rv;
}

_INLINE_ errcode_t ext2fs_ڈھونڈو_پہلا_zero_inode_bitmap2(ext2fs_inode_bitmap bitmap,
							ext2_ino_t آغاز,
							ext2_ino_t انتہا,
							ext2_ino_t *برآمد)
{
	__u64 o;
	errcode_t rv;

	rv = ext2fs_ڈھونڈو_پہلا_zero_generic_bmap((ext2fs_generic_bitmap) bitmap,
						 آغاز, انتہا, &o);
	اگر (!rv)
		*برآمد = (ext2_ino_t) o;
	واپس rv;
}

_INLINE_ errcode_t ext2fs_ڈھونڈو_پہلا_سیٹ_بلاک_bitmap2(ext2fs_بلاک_bitmap bitmap,
						       blk64_t آغاز,
						       blk64_t انتہا,
						       blk64_t *برآمد)
{
	__u64 o;
	errcode_t rv;

	rv = ext2fs_ڈھونڈو_پہلا_سیٹ_generic_bmap((ext2fs_generic_bitmap) bitmap,
						آغاز, انتہا, &o);
	اگر (!rv)
		*برآمد = o;
	واپس rv;
}

_INLINE_ errcode_t ext2fs_ڈھونڈو_پہلا_سیٹ_inode_bitmap2(ext2fs_inode_bitmap bitmap,
						       ext2_ino_t آغاز,
						       ext2_ino_t انتہا,
						       ext2_ino_t *برآمد)
{
	__u64 o;
	errcode_t rv;

	rv = ext2fs_ڈھونڈو_پہلا_سیٹ_generic_bmap((ext2fs_generic_bitmap) bitmap,
						آغاز, انتہا, &o);
	اگر (!rv)
		*برآمد = (ext2_ino_t) o;
	واپس rv;
}

_INLINE_ blk64_t ext2fs_get_بلاک_bitmap_start2(ext2fs_بلاک_bitmap bitmap)
{
	واپس ext2fs_get_generic_bmap_آغاز((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ ext2_ino_t ext2fs_get_inode_bitmap_start2(ext2fs_inode_bitmap bitmap)
{
	واپس (ext2_ino_t) ext2fs_get_generic_bmap_آغاز((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ blk64_t ext2fs_get_بلاک_bitmap_end2(ext2fs_بلاک_bitmap bitmap)
{
	واپس ext2fs_get_generic_bmap_انتہا((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ ext2_ino_t ext2fs_get_inode_bitmap_end2(ext2fs_inode_bitmap bitmap)
{
	واپس (ext2_ino_t) ext2fs_get_generic_bmap_انتہا((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ صحیح ext2fs_fast_test_بلاک_bitmap_range2(ext2fs_بلاک_bitmap bitmap,
						  blk64_t بلاک,
						  غیرعلامتی صحیح عدد)
{
	واپس ext2fs_test_بلاک_bitmap_range2(bitmap, بلاک, عدد);
}

_INLINE_ خالی ext2fs_fast_mark_بلاک_bitmap_range2(ext2fs_بلاک_bitmap bitmap,
						   blk64_t بلاک,
						   غیرعلامتی صحیح عدد)
{
	ext2fs_mark_بلاک_bitmap_range2(bitmap, بلاک, عدد);
}

_INLINE_ خالی ext2fs_fast_unmark_بلاک_bitmap_range2(ext2fs_بلاک_bitmap bitmap,
						     blk64_t بلاک,
						     غیرعلامتی صحیح عدد)
{
	ext2fs_unmark_بلاک_bitmap_range2(bitmap, بلاک, عدد);
}

#تعریف_منسوخ _INLINE_
#اگر_ختم

خارجی صحیح ext2fs_سیٹ_ثنائی_ہندسہ(غیرعلامتی صحیح nr,خالی * addr);
خارجی صحیح ext2fs_clear_ثنائی_ہندسہ(غیرعلامتی صحیح nr, خالی * addr);
خارجی صحیح ext2fs_test_ثنائی_ہندسہ(غیرعلامتی صحیح nr, ثابت خالی * addr);
خارجی صحیح ext2fs_سیٹ_bit64(__u64 nr,خالی * addr);
خارجی صحیح ext2fs_clear_bit64(__u64 nr, خالی * addr);
خارجی صحیح ext2fs_test_bit64(__u64 nr, ثابت خالی * addr);
خارجی غیرعلامتی صحیح ext2fs_bitcount(ثابت خالی *addr, غیرعلامتی صحیح nbytes);
