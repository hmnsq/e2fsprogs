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

#ifdef சொற்கள்_BIGENDIAN
#define ext2fs_cpu_to_le64(x) ((__force __le64)ext2fs_swab64((__u64)(x)))
#define ext2fs_le64_to_cpu(x) ext2fs_swab64((__force __u64)(__le64)(x))
#define ext2fs_cpu_to_le32(x) ((__force __le32)ext2fs_swab32((__u32)(x)))
#define ext2fs_le32_to_cpu(x) ext2fs_swab32((__force __u32)(__le32)(x))
#define ext2fs_cpu_to_le16(x) ((__force __le16)ext2fs_swab16((__u16)(x)))
#define ext2fs_le16_to_cpu(x) ext2fs_swab16((__force __u16)(__le16)(x))

#define ext2fs_cpu_to_be64(x) ((__force __be64)(__u64)(x))
#define ext2fs_be64_to_cpu(x) ((__force __u64)(__be64)(x))
#define ext2fs_cpu_to_be32(x) ((__force __be32)(__u32)(x))
#define ext2fs_be32_to_cpu(x) ((__force __u32)(__be32)(x))
#define ext2fs_cpu_to_be16(x) ((__force __be16)(__u16)(x))
#define ext2fs_be16_to_cpu(x) ((__force __u16)(__be16)(x))
#இல்லையெனில்
#define ext2fs_cpu_to_le64(x) ((__force __le64)(__u64)(x))
#define ext2fs_le64_to_cpu(x) ((__force __u64)(__le64)(x))
#define ext2fs_cpu_to_le32(x) ((__force __le32)(__u32)(x))
#define ext2fs_le32_to_cpu(x) ((__force __u32)(__le32)(x))
#define ext2fs_cpu_to_le16(x) ((__force __le16)(__u16)(x))
#define ext2fs_le16_to_cpu(x) ((__force __u16)(__le16)(x))

#define ext2fs_cpu_to_be64(x) ((__force __be64)ext2fs_swab64((__u64)(x)))
#define ext2fs_be64_to_cpu(x) ext2fs_swab64((__force __u64)(__be64)(x))
#define ext2fs_cpu_to_be32(x) ((__force __be32)ext2fs_swab32((__u32)(x)))
#define ext2fs_be32_to_cpu(x) ext2fs_swab32((__force __u32)(__be32)(x))
#define ext2fs_cpu_to_be16(x) ((__force __be16)ext2fs_swab16((__u16)(x)))
#define ext2fs_be16_to_cpu(x) ext2fs_swab16((__force __u16)(__be16)(x))
#endif

/*
 * EXT2FS bitmap manipulation routines.
 */

/* Support for sending warning messages from the inline subroutines */
வெளி மாறிலி எழுத்து *ext2fs_block_string;
வெளி மாறிலி எழுத்து *ext2fs_inode_string;
வெளி மாறிலி எழுத்து *ext2fs_mark_string;
வெளி மாறிலி எழுத்து *ext2fs_unmark_string;
வெளி மாறிலி எழுத்து *ext2fs_test_string;
வெளி வெறுமை ext2fs_warn_bitmap(errcode_t errcode, unsigned long arg,
			       மாறிலி எழுத்து *description);
வெளி வெறுமை ext2fs_warn_bitmap2(ext2fs_generic_bitmap bitmap,
				முழு code, unsigned long arg);

#ifdef NO_INLINE_FUNCS
வெளி வெறுமை ext2fs_fast_set_bit(unsigned முழு nr,வெறுமை * addr);
வெளி வெறுமை ext2fs_fast_clear_bit(unsigned முழு nr, வெறுமை * addr);
வெளி வெறுமை ext2fs_fast_set_bit64(__u64 nr,வெறுமை * addr);
வெளி வெறுமை ext2fs_fast_clear_bit64(__u64 nr, வெறுமை * addr);
வெளி __u16 ext2fs_swab16(__u16 val);
வெளி __u32 ext2fs_swab32(__u32 val);
வெளி __u64 ext2fs_swab64(__u64 val);

வெளி முழு ext2fs_mark_block_bitmap(ext2fs_block_bitmap bitmap, blk_t block);
வெளி முழு ext2fs_unmark_block_bitmap(ext2fs_block_bitmap bitmap,
				       blk_t block);
வெளி முழு ext2fs_test_block_bitmap(ext2fs_block_bitmap bitmap, blk_t block);

வெளி முழு ext2fs_mark_inode_bitmap(ext2fs_inode_bitmap bitmap, ext2_ino_t inode);
வெளி முழு ext2fs_unmark_inode_bitmap(ext2fs_inode_bitmap bitmap,
				       ext2_ino_t inode);
வெளி முழு ext2fs_test_inode_bitmap(ext2fs_inode_bitmap bitmap, ext2_ino_t inode);

வெளி வெறுமை ext2fs_fast_mark_block_bitmap(ext2fs_block_bitmap bitmap,
					  blk_t block);
வெளி வெறுமை ext2fs_fast_unmark_block_bitmap(ext2fs_block_bitmap bitmap,
					    blk_t block);
வெளி முழு ext2fs_fast_test_block_bitmap(ext2fs_block_bitmap bitmap,
					 blk_t block);

வெளி வெறுமை ext2fs_fast_mark_inode_bitmap(ext2fs_inode_bitmap bitmap,
					  ext2_ino_t inode);
வெளி வெறுமை ext2fs_fast_unmark_inode_bitmap(ext2fs_inode_bitmap bitmap,
					    ext2_ino_t inode);
வெளி முழு ext2fs_fast_test_inode_bitmap(ext2fs_inode_bitmap bitmap,
					 ext2_ino_t inode);
வெளி blk_t ext2fs_get_block_bitmap_start(ext2fs_block_bitmap bitmap);
வெளி ext2_ino_t ext2fs_get_inode_bitmap_start(ext2fs_inode_bitmap bitmap);
வெளி blk_t ext2fs_get_block_bitmap_end(ext2fs_block_bitmap bitmap);
வெளி ext2_ino_t ext2fs_get_inode_bitmap_end(ext2fs_inode_bitmap bitmap);

வெளி வெறுமை ext2fs_fast_mark_block_bitmap_range(ext2fs_block_bitmap bitmap,
						blk_t block, முழு இலக்கம்);
வெளி வெறுமை ext2fs_fast_unmark_block_bitmap_range(ext2fs_block_bitmap bitmap,
						  blk_t block, முழு இலக்கம்);
வெளி முழு ext2fs_fast_test_block_bitmap_range(ext2fs_block_bitmap bitmap,
					       blk_t block, முழு இலக்கம்);
#endif

/* These functions routines moved to gen_bitmap.c */
வெளி வெறுமை ext2fs_mark_block_bitmap_range(ext2fs_block_bitmap bitmap,
					   blk_t block, முழு இலக்கம்);
வெளி வெறுமை ext2fs_unmark_block_bitmap_range(ext2fs_block_bitmap bitmap,
					     blk_t block, முழு இலக்கம்);
வெளி முழு ext2fs_test_block_bitmap_range(ext2fs_block_bitmap bitmap,
					  blk_t block, முழு இலக்கம்);
வெளி முழு ext2fs_test_inode_bitmap_range(ext2fs_inode_bitmap bitmap,
					  ext2_ino_t inode, முழு இலக்கம்);
வெளி முழு ext2fs_mark_generic_bitmap(ext2fs_generic_bitmap bitmap,
					 __u32 bitno);
வெளி முழு ext2fs_unmark_generic_bitmap(ext2fs_generic_bitmap bitmap,
					   blk_t bitno);
வெளி முழு ext2fs_test_generic_bitmap(ext2fs_generic_bitmap bitmap,
				      blk_t bitno);
வெளி முழு ext2fs_test_block_bitmap_range(ext2fs_block_bitmap bitmap,
					  blk_t block, முழு இலக்கம்);
வெளி வெறுமை ext2fs_set_bitmap_padding(ext2fs_generic_bitmap map);
வெளி __u32 ext2fs_get_generic_bitmap_start(ext2fs_generic_bitmap bitmap);
வெளி __u32 ext2fs_get_generic_bitmap_end(ext2fs_generic_bitmap bitmap);

/* 64-bit versions */

#ifdef NO_INLINE_FUNCS
வெளி முழு ext2fs_mark_block_bitmap2(ext2fs_block_bitmap bitmap,
				     blk64_t block);
வெளி முழு ext2fs_unmark_block_bitmap2(ext2fs_block_bitmap bitmap,
				       blk64_t block);
வெளி முழு ext2fs_test_block_bitmap2(ext2fs_block_bitmap bitmap,
				     blk64_t block);

வெளி முழு ext2fs_mark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
				     ext2_ino_t inode);
வெளி முழு ext2fs_unmark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
				       ext2_ino_t inode);
வெளி முழு ext2fs_test_inode_bitmap2(ext2fs_inode_bitmap bitmap,
				     ext2_ino_t inode);

வெளி வெறுமை ext2fs_fast_mark_block_bitmap2(ext2fs_block_bitmap bitmap,
					   blk64_t block);
வெளி வெறுமை ext2fs_fast_unmark_block_bitmap2(ext2fs_block_bitmap bitmap,
					     blk64_t block);
வெளி முழு ext2fs_fast_test_block_bitmap2(ext2fs_block_bitmap bitmap,
					  blk64_t block);

வெளி வெறுமை ext2fs_fast_mark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
					   ext2_ino_t inode);
வெளி வெறுமை ext2fs_fast_unmark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
					    ext2_ino_t inode);
வெளி முழு ext2fs_fast_test_inode_bitmap2(ext2fs_inode_bitmap bitmap,
					  ext2_ino_t inode);
வெளி errcode_t ext2fs_find_first_zero_block_bitmap2(ext2fs_block_bitmap bitmap,
						      blk64_t start,
						      blk64_t end,
						      blk64_t *out);
வெளி errcode_t ext2fs_find_first_zero_inode_bitmap2(ext2fs_inode_bitmap bitmap,
						      ext2_ino_t start,
						      ext2_ino_t end,
						      ext2_ino_t *out);
வெளி errcode_t ext2fs_find_first_set_block_bitmap2(ext2fs_block_bitmap bitmap,
						     blk64_t start,
						     blk64_t end,
						     blk64_t *out);
வெளி errcode_t ext2fs_find_first_set_inode_bitmap2(ext2fs_inode_bitmap bitmap,
						      ext2_ino_t start,
						      ext2_ino_t end,
						      ext2_ino_t *out);
வெளி blk64_t ext2fs_get_block_bitmap_start2(ext2fs_block_bitmap bitmap);
வெளி ext2_ino_t ext2fs_get_inode_bitmap_start2(ext2fs_inode_bitmap bitmap);
வெளி blk64_t ext2fs_get_block_bitmap_end2(ext2fs_block_bitmap bitmap);
வெளி ext2_ino_t ext2fs_get_inode_bitmap_end2(ext2fs_inode_bitmap bitmap);

வெளி முழு ext2fs_fast_test_block_bitmap_range2(ext2fs_block_bitmap bitmap,
						blk64_t block,
						unsigned முழு இலக்கம்);
வெளி வெறுமை ext2fs_fast_mark_block_bitmap_range2(ext2fs_block_bitmap bitmap,
						 blk64_t block,
						 unsigned முழு இலக்கம்);
வெளி வெறுமை ext2fs_fast_unmark_block_bitmap_range2(ext2fs_block_bitmap bitmap,
						   blk64_t block,
						   unsigned முழு இலக்கம்);
#endif

/* These routines moved to gen_bitmap64.c */
வெளி வெறுமை ext2fs_clear_generic_bmap(ext2fs_generic_bitmap bitmap);
வெளி errcode_t ext2fs_compare_generic_bmap(errcode_t neq,
					     ext2fs_generic_bitmap bm1,
					     ext2fs_generic_bitmap bm2);
வெளி வெறுமை ext2fs_set_generic_bmap_padding(ext2fs_generic_bitmap bmap);
வெளி முழு ext2fs_mark_generic_bmap(ext2fs_generic_bitmap bitmap,
				    blk64_t bitno);
வெளி முழு ext2fs_unmark_generic_bmap(ext2fs_generic_bitmap bitmap,
				      blk64_t bitno);
வெளி முழு ext2fs_test_generic_bmap(ext2fs_generic_bitmap bitmap,
				    blk64_t bitno);
வெளி முழு ext2fs_test_block_bitmap_range2(ext2fs_block_bitmap bitmap,
					   blk64_t block, unsigned முழு இலக்கம்);
வெளி __u64 ext2fs_get_generic_bmap_start(ext2fs_generic_bitmap bitmap);
வெளி __u64 ext2fs_get_generic_bmap_end(ext2fs_generic_bitmap bitmap);
வெளி முழு ext2fs_test_block_bitmap_range2(ext2fs_block_bitmap bitmap,
					   blk64_t block, unsigned முழு இலக்கம்);
வெளி வெறுமை ext2fs_mark_block_bitmap_range2(ext2fs_block_bitmap bitmap,
					    blk64_t block, unsigned முழு இலக்கம்);
வெளி வெறுமை ext2fs_unmark_block_bitmap_range2(ext2fs_block_bitmap bitmap,
					      blk64_t block, unsigned முழு இலக்கம்);
வெளி errcode_t ext2fs_find_first_zero_generic_bmap(ext2fs_generic_bitmap bitmap,
						     __u64 start, __u64 end,
						     __u64 *out);
வெளி errcode_t ext2fs_find_first_set_generic_bmap(ext2fs_generic_bitmap bitmap,
						    __u64 start, __u64 end,
						    __u64 *out);

/*
 * The inline routines themselves...
 *
 * If NO_INLINE_FUNCS is defined, then we won't try to do inline
 * functions at all; they will be included as normal functions in
 * inline.c
 */

#எனில் (defined(INCLUDE_INLINE_FUNCS) || !defined(NO_INLINE_FUNCS))
#ifdef INCLUDE_INLINE_FUNCS
#எனில் (__STDC_VERSION__ >= 199901L)
#define _INLINE_ வெளி உள்வரி
#இல்லையெனில்
#define _INLINE_ உள்வரி
#endif
#இல்லையெனில் /* !INCLUDE_INLINE FUNCS */
#எனில் (__STDC_VERSION__ >= 199901L)
#define _INLINE_ உள்வரி
#இல்லையெனில் /* not C99 */
#ifdef __GNUC__
#define _INLINE_ வெளி __inline__
#இல்லையெனில்				/* For Watcom C */
#define _INLINE_ வெளி உள்வரி
#endif /* __GNUC__ */
#endif /* __STDC_VERSION__ >= 199901L */
#endif /* INCLUDE_INLINE_FUNCS */

/*
 * Fast bit set/clear functions that doesn't need to return the
 * previous bit value.
 */

_INLINE_ வெறுமை ext2fs_fast_set_bit(unsigned முழு nr,வெறுமை * addr)
{
	unsigned எழுத்து	*ADDR = (unsigned எழுத்து *) addr;

	ADDR += nr >> 3;
	*ADDR |= (unsigned எழுத்து) (1 << (nr & 0x07));
}

_INLINE_ வெறுமை ext2fs_fast_clear_bit(unsigned முழு nr, வெறுமை * addr)
{
	unsigned எழுத்து	*ADDR = (unsigned எழுத்து *) addr;

	ADDR += nr >> 3;
	*ADDR &= (unsigned எழுத்து) ~(1 << (nr & 0x07));
}


_INLINE_ வெறுமை ext2fs_fast_set_bit64(__u64 nr, வெறுமை * addr)
{
	unsigned எழுத்து	*ADDR = (unsigned எழுத்து *) addr;

	ADDR += nr >> 3;
	*ADDR |= (unsigned எழுத்து) (1 << (nr & 0x07));
}

_INLINE_ வெறுமை ext2fs_fast_clear_bit64(__u64 nr, வெறுமை * addr)
{
	unsigned எழுத்து	*ADDR = (unsigned எழுத்து *) addr;

	ADDR += nr >> 3;
	*ADDR &= (unsigned எழுத்து) ~(1 << (nr & 0x07));
}

_INLINE_ __u16 ext2fs_swab16(__u16 val)
{
	திரும்பு (val >> 8) | (__u16) (val << 8);
}

_INLINE_ __u32 ext2fs_swab32(__u32 val)
{
	திரும்பு ((val>>24) | ((val>>8)&0xFF00) |
		((val<<8)&0xFF0000) | (val<<24));
}

_INLINE_ __u64 ext2fs_swab64(__u64 val)
{
	திரும்பு (ext2fs_swab32((__u32) (val >> 32)) |
		(((__u64)ext2fs_swab32(val & 0xFFFFFFFFUL)) << 32));
}

_INLINE_ முழு ext2fs_mark_block_bitmap(ext2fs_block_bitmap bitmap,
				       blk_t block)
{
	திரும்பு ext2fs_mark_generic_bitmap((ext2fs_generic_bitmap) bitmap,
					  block);
}

_INLINE_ முழு ext2fs_unmark_block_bitmap(ext2fs_block_bitmap bitmap,
					 blk_t block)
{
	திரும்பு ext2fs_unmark_generic_bitmap((ext2fs_generic_bitmap) bitmap,
					    block);
}

_INLINE_ முழு ext2fs_test_block_bitmap(ext2fs_block_bitmap bitmap,
				       blk_t block)
{
	திரும்பு ext2fs_test_generic_bitmap((ext2fs_generic_bitmap) bitmap,
					  block);
}

_INLINE_ முழு ext2fs_mark_inode_bitmap(ext2fs_inode_bitmap bitmap,
				       ext2_ino_t inode)
{
	திரும்பு ext2fs_mark_generic_bitmap((ext2fs_generic_bitmap) bitmap,
					  inode);
}

_INLINE_ முழு ext2fs_unmark_inode_bitmap(ext2fs_inode_bitmap bitmap,
					 ext2_ino_t inode)
{
	திரும்பு ext2fs_unmark_generic_bitmap((ext2fs_generic_bitmap) bitmap,
				     inode);
}

_INLINE_ முழு ext2fs_test_inode_bitmap(ext2fs_inode_bitmap bitmap,
				       ext2_ino_t inode)
{
	திரும்பு ext2fs_test_generic_bitmap((ext2fs_generic_bitmap) bitmap,
					  inode);
}

_INLINE_ வெறுமை ext2fs_fast_mark_block_bitmap(ext2fs_block_bitmap bitmap,
					    blk_t block)
{
	ext2fs_mark_generic_bitmap((ext2fs_generic_bitmap) bitmap, block);
}

_INLINE_ வெறுமை ext2fs_fast_unmark_block_bitmap(ext2fs_block_bitmap bitmap,
					      blk_t block)
{
	ext2fs_unmark_generic_bitmap((ext2fs_generic_bitmap) bitmap, block);
}

_INLINE_ முழு ext2fs_fast_test_block_bitmap(ext2fs_block_bitmap bitmap,
					    blk_t block)
{
	திரும்பு ext2fs_test_generic_bitmap((ext2fs_generic_bitmap) bitmap,
					  block);
}

_INLINE_ வெறுமை ext2fs_fast_mark_inode_bitmap(ext2fs_inode_bitmap bitmap,
					    ext2_ino_t inode)
{
	ext2fs_mark_generic_bitmap((ext2fs_generic_bitmap) bitmap, inode);
}

_INLINE_ வெறுமை ext2fs_fast_unmark_inode_bitmap(ext2fs_inode_bitmap bitmap,
					      ext2_ino_t inode)
{
	ext2fs_unmark_generic_bitmap((ext2fs_generic_bitmap) bitmap, inode);
}

_INLINE_ முழு ext2fs_fast_test_inode_bitmap(ext2fs_inode_bitmap bitmap,
					   ext2_ino_t inode)
{
	திரும்பு ext2fs_test_generic_bitmap((ext2fs_generic_bitmap) bitmap,
					  inode);
}

_INLINE_ blk_t ext2fs_get_block_bitmap_start(ext2fs_block_bitmap bitmap)
{
	திரும்பு ext2fs_get_generic_bitmap_start((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ ext2_ino_t ext2fs_get_inode_bitmap_start(ext2fs_inode_bitmap bitmap)
{
	திரும்பு ext2fs_get_generic_bitmap_start((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ blk_t ext2fs_get_block_bitmap_end(ext2fs_block_bitmap bitmap)
{
	திரும்பு ext2fs_get_generic_bitmap_end((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ ext2_ino_t ext2fs_get_inode_bitmap_end(ext2fs_inode_bitmap bitmap)
{
	திரும்பு ext2fs_get_generic_bitmap_end((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ முழு ext2fs_fast_test_block_bitmap_range(ext2fs_block_bitmap bitmap,
						 blk_t block, முழு இலக்கம்)
{
	திரும்பு ext2fs_test_block_bitmap_range(bitmap, block, இலக்கம்);
}

_INLINE_ வெறுமை ext2fs_fast_mark_block_bitmap_range(ext2fs_block_bitmap bitmap,
						  blk_t block, முழு இலக்கம்)
{
	ext2fs_mark_block_bitmap_range(bitmap, block, இலக்கம்);
}

_INLINE_ வெறுமை ext2fs_fast_unmark_block_bitmap_range(ext2fs_block_bitmap bitmap,
						    blk_t block, முழு இலக்கம்)
{
	ext2fs_unmark_block_bitmap_range(bitmap, block, இலக்கம்);
}

/* 64-bit versions */

_INLINE_ முழு ext2fs_mark_block_bitmap2(ext2fs_block_bitmap bitmap,
				       blk64_t block)
{
	திரும்பு ext2fs_mark_generic_bmap((ext2fs_generic_bitmap) bitmap,
					block);
}

_INLINE_ முழு ext2fs_unmark_block_bitmap2(ext2fs_block_bitmap bitmap,
					 blk64_t block)
{
	திரும்பு ext2fs_unmark_generic_bmap((ext2fs_generic_bitmap) bitmap, block);
}

_INLINE_ முழு ext2fs_test_block_bitmap2(ext2fs_block_bitmap bitmap,
				       blk64_t block)
{
	திரும்பு ext2fs_test_generic_bmap((ext2fs_generic_bitmap) bitmap,
					block);
}

_INLINE_ முழு ext2fs_mark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
				       ext2_ino_t inode)
{
	திரும்பு ext2fs_mark_generic_bmap((ext2fs_generic_bitmap) bitmap,
					inode);
}

_INLINE_ முழு ext2fs_unmark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
					 ext2_ino_t inode)
{
	திரும்பு ext2fs_unmark_generic_bmap((ext2fs_generic_bitmap) bitmap,
					  inode);
}

_INLINE_ முழு ext2fs_test_inode_bitmap2(ext2fs_inode_bitmap bitmap,
				       ext2_ino_t inode)
{
	திரும்பு ext2fs_test_generic_bmap((ext2fs_generic_bitmap) bitmap,
					inode);
}

_INLINE_ வெறுமை ext2fs_fast_mark_block_bitmap2(ext2fs_block_bitmap bitmap,
					     blk64_t block)
{
	ext2fs_mark_generic_bmap((ext2fs_generic_bitmap) bitmap, block);
}

_INLINE_ வெறுமை ext2fs_fast_unmark_block_bitmap2(ext2fs_block_bitmap bitmap,
					       blk64_t block)
{
	ext2fs_unmark_generic_bmap((ext2fs_generic_bitmap) bitmap, block);
}

_INLINE_ முழு ext2fs_fast_test_block_bitmap2(ext2fs_block_bitmap bitmap,
					    blk64_t block)
{
	திரும்பு ext2fs_test_generic_bmap((ext2fs_generic_bitmap) bitmap,
					block);
}

_INLINE_ வெறுமை ext2fs_fast_mark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
					     ext2_ino_t inode)
{
	ext2fs_mark_generic_bmap((ext2fs_generic_bitmap) bitmap, inode);
}

_INLINE_ வெறுமை ext2fs_fast_unmark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
					       ext2_ino_t inode)
{
	ext2fs_unmark_generic_bmap((ext2fs_generic_bitmap) bitmap, inode);
}

_INLINE_ முழு ext2fs_fast_test_inode_bitmap2(ext2fs_inode_bitmap bitmap,
					    ext2_ino_t inode)
{
	திரும்பு ext2fs_test_generic_bmap((ext2fs_generic_bitmap) bitmap,
					inode);
}

_INLINE_ errcode_t ext2fs_find_first_zero_block_bitmap2(ext2fs_block_bitmap bitmap,
							blk64_t start,
							blk64_t end,
							blk64_t *out)
{
	__u64 o;
	errcode_t rv;

	rv = ext2fs_find_first_zero_generic_bmap((ext2fs_generic_bitmap) bitmap,
						 start, end, &o);
	எனில் (!rv)
		*out = o;
	திரும்பு rv;
}

_INLINE_ errcode_t ext2fs_find_first_zero_inode_bitmap2(ext2fs_inode_bitmap bitmap,
							ext2_ino_t start,
							ext2_ino_t end,
							ext2_ino_t *out)
{
	__u64 o;
	errcode_t rv;

	rv = ext2fs_find_first_zero_generic_bmap((ext2fs_generic_bitmap) bitmap,
						 start, end, &o);
	எனில் (!rv)
		*out = (ext2_ino_t) o;
	திரும்பு rv;
}

_INLINE_ errcode_t ext2fs_find_first_set_block_bitmap2(ext2fs_block_bitmap bitmap,
						       blk64_t start,
						       blk64_t end,
						       blk64_t *out)
{
	__u64 o;
	errcode_t rv;

	rv = ext2fs_find_first_set_generic_bmap((ext2fs_generic_bitmap) bitmap,
						start, end, &o);
	எனில் (!rv)
		*out = o;
	திரும்பு rv;
}

_INLINE_ errcode_t ext2fs_find_first_set_inode_bitmap2(ext2fs_inode_bitmap bitmap,
						       ext2_ino_t start,
						       ext2_ino_t end,
						       ext2_ino_t *out)
{
	__u64 o;
	errcode_t rv;

	rv = ext2fs_find_first_set_generic_bmap((ext2fs_generic_bitmap) bitmap,
						start, end, &o);
	எனில் (!rv)
		*out = (ext2_ino_t) o;
	திரும்பு rv;
}

_INLINE_ blk64_t ext2fs_get_block_bitmap_start2(ext2fs_block_bitmap bitmap)
{
	திரும்பு ext2fs_get_generic_bmap_start((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ ext2_ino_t ext2fs_get_inode_bitmap_start2(ext2fs_inode_bitmap bitmap)
{
	திரும்பு (ext2_ino_t) ext2fs_get_generic_bmap_start((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ blk64_t ext2fs_get_block_bitmap_end2(ext2fs_block_bitmap bitmap)
{
	திரும்பு ext2fs_get_generic_bmap_end((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ ext2_ino_t ext2fs_get_inode_bitmap_end2(ext2fs_inode_bitmap bitmap)
{
	திரும்பு (ext2_ino_t) ext2fs_get_generic_bmap_end((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ முழு ext2fs_fast_test_block_bitmap_range2(ext2fs_block_bitmap bitmap,
						  blk64_t block,
						  unsigned முழு இலக்கம்)
{
	திரும்பு ext2fs_test_block_bitmap_range2(bitmap, block, இலக்கம்);
}

_INLINE_ வெறுமை ext2fs_fast_mark_block_bitmap_range2(ext2fs_block_bitmap bitmap,
						   blk64_t block,
						   unsigned முழு இலக்கம்)
{
	ext2fs_mark_block_bitmap_range2(bitmap, block, இலக்கம்);
}

_INLINE_ வெறுமை ext2fs_fast_unmark_block_bitmap_range2(ext2fs_block_bitmap bitmap,
						     blk64_t block,
						     unsigned முழு இலக்கம்)
{
	ext2fs_unmark_block_bitmap_range2(bitmap, block, இலக்கம்);
}

#undef _INLINE_
#endif

வெளி முழு ext2fs_set_bit(unsigned முழு nr,வெறுமை * addr);
வெளி முழு ext2fs_clear_bit(unsigned முழு nr, வெறுமை * addr);
வெளி முழு ext2fs_test_bit(unsigned முழு nr, மாறிலி வெறுமை * addr);
வெளி முழு ext2fs_set_bit64(__u64 nr,வெறுமை * addr);
வெளி முழு ext2fs_clear_bit64(__u64 nr, வெறுமை * addr);
வெளி முழு ext2fs_test_bit64(__u64 nr, மாறிலி வெறுமை * addr);
வெளி unsigned முழு ext2fs_bitcount(மாறிலி வெறுமை *addr, unsigned முழு nbytes);
