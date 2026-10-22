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

#ifdef શબ્દો_BIGENDIAN
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
#નહીંતર
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
બાહ્ય અચલ અક્ષર *ext2fs_block_string;
બાહ્ય અચલ અક્ષર *ext2fs_inode_string;
બાહ્ય અચલ અક્ષર *ext2fs_mark_string;
બાહ્ય અચલ અક્ષર *ext2fs_unmark_string;
બાહ્ય અચલ અક્ષર *ext2fs_test_string;
બાહ્ય શૂન્ય ext2fs_warn_bitmap(errcode_t errcode, unsigned long arg,
			       અચલ અક્ષર *description);
બાહ્ય શૂન્ય ext2fs_warn_bitmap2(ext2fs_generic_bitmap bitmap,
				પૂર્ણાંક code, unsigned long arg);

#ifdef NO_INLINE_FUNCS
બાહ્ય શૂન્ય ext2fs_fast_set_bit(unsigned પૂર્ણાંક nr,શૂન્ય * addr);
બાહ્ય શૂન્ય ext2fs_fast_clear_bit(unsigned પૂર્ણાંક nr, શૂન્ય * addr);
બાહ્ય શૂન્ય ext2fs_fast_set_bit64(__u64 nr,શૂન્ય * addr);
બાહ્ય શૂન્ય ext2fs_fast_clear_bit64(__u64 nr, શૂન્ય * addr);
બાહ્ય __u16 ext2fs_swab16(__u16 val);
બાહ્ય __u32 ext2fs_swab32(__u32 val);
બાહ્ય __u64 ext2fs_swab64(__u64 val);

બાહ્ય પૂર્ણાંક ext2fs_mark_block_bitmap(ext2fs_block_bitmap bitmap, blk_t block);
બાહ્ય પૂર્ણાંક ext2fs_unmark_block_bitmap(ext2fs_block_bitmap bitmap,
				       blk_t block);
બાહ્ય પૂર્ણાંક ext2fs_test_block_bitmap(ext2fs_block_bitmap bitmap, blk_t block);

બાહ્ય પૂર્ણાંક ext2fs_mark_inode_bitmap(ext2fs_inode_bitmap bitmap, ext2_ino_t inode);
બાહ્ય પૂર્ણાંક ext2fs_unmark_inode_bitmap(ext2fs_inode_bitmap bitmap,
				       ext2_ino_t inode);
બાહ્ય પૂર્ણાંક ext2fs_test_inode_bitmap(ext2fs_inode_bitmap bitmap, ext2_ino_t inode);

બાહ્ય શૂન્ય ext2fs_fast_mark_block_bitmap(ext2fs_block_bitmap bitmap,
					  blk_t block);
બાહ્ય શૂન્ય ext2fs_fast_unmark_block_bitmap(ext2fs_block_bitmap bitmap,
					    blk_t block);
બાહ્ય પૂર્ણાંક ext2fs_fast_test_block_bitmap(ext2fs_block_bitmap bitmap,
					 blk_t block);

બાહ્ય શૂન્ય ext2fs_fast_mark_inode_bitmap(ext2fs_inode_bitmap bitmap,
					  ext2_ino_t inode);
બાહ્ય શૂન્ય ext2fs_fast_unmark_inode_bitmap(ext2fs_inode_bitmap bitmap,
					    ext2_ino_t inode);
બાહ્ય પૂર્ણાંક ext2fs_fast_test_inode_bitmap(ext2fs_inode_bitmap bitmap,
					 ext2_ino_t inode);
બાહ્ય blk_t ext2fs_get_block_bitmap_start(ext2fs_block_bitmap bitmap);
બાહ્ય ext2_ino_t ext2fs_get_inode_bitmap_start(ext2fs_inode_bitmap bitmap);
બાહ્ય blk_t ext2fs_get_block_bitmap_end(ext2fs_block_bitmap bitmap);
બાહ્ય ext2_ino_t ext2fs_get_inode_bitmap_end(ext2fs_inode_bitmap bitmap);

બાહ્ય શૂન્ય ext2fs_fast_mark_block_bitmap_range(ext2fs_block_bitmap bitmap,
						blk_t block, પૂર્ણાંક અંક);
બાહ્ય શૂન્ય ext2fs_fast_unmark_block_bitmap_range(ext2fs_block_bitmap bitmap,
						  blk_t block, પૂર્ણાંક અંક);
બાહ્ય પૂર્ણાંક ext2fs_fast_test_block_bitmap_range(ext2fs_block_bitmap bitmap,
					       blk_t block, પૂર્ણાંક અંક);
#endif

/* These functions routines moved to gen_bitmap.c */
બાહ્ય શૂન્ય ext2fs_mark_block_bitmap_range(ext2fs_block_bitmap bitmap,
					   blk_t block, પૂર્ણાંક અંક);
બાહ્ય શૂન્ય ext2fs_unmark_block_bitmap_range(ext2fs_block_bitmap bitmap,
					     blk_t block, પૂર્ણાંક અંક);
બાહ્ય પૂર્ણાંક ext2fs_test_block_bitmap_range(ext2fs_block_bitmap bitmap,
					  blk_t block, પૂર્ણાંક અંક);
બાહ્ય પૂર્ણાંક ext2fs_test_inode_bitmap_range(ext2fs_inode_bitmap bitmap,
					  ext2_ino_t inode, પૂર્ણાંક અંક);
બાહ્ય પૂર્ણાંક ext2fs_mark_generic_bitmap(ext2fs_generic_bitmap bitmap,
					 __u32 bitno);
બાહ્ય પૂર્ણાંક ext2fs_unmark_generic_bitmap(ext2fs_generic_bitmap bitmap,
					   blk_t bitno);
બાહ્ય પૂર્ણાંક ext2fs_test_generic_bitmap(ext2fs_generic_bitmap bitmap,
				      blk_t bitno);
બાહ્ય પૂર્ણાંક ext2fs_test_block_bitmap_range(ext2fs_block_bitmap bitmap,
					  blk_t block, પૂર્ણાંક અંક);
બાહ્ય શૂન્ય ext2fs_set_bitmap_padding(ext2fs_generic_bitmap map);
બાહ્ય __u32 ext2fs_get_generic_bitmap_start(ext2fs_generic_bitmap bitmap);
બાહ્ય __u32 ext2fs_get_generic_bitmap_end(ext2fs_generic_bitmap bitmap);

/* 64-bit versions */

#ifdef NO_INLINE_FUNCS
બાહ્ય પૂર્ણાંક ext2fs_mark_block_bitmap2(ext2fs_block_bitmap bitmap,
				     blk64_t block);
બાહ્ય પૂર્ણાંક ext2fs_unmark_block_bitmap2(ext2fs_block_bitmap bitmap,
				       blk64_t block);
બાહ્ય પૂર્ણાંક ext2fs_test_block_bitmap2(ext2fs_block_bitmap bitmap,
				     blk64_t block);

બાહ્ય પૂર્ણાંક ext2fs_mark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
				     ext2_ino_t inode);
બાહ્ય પૂર્ણાંક ext2fs_unmark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
				       ext2_ino_t inode);
બાહ્ય પૂર્ણાંક ext2fs_test_inode_bitmap2(ext2fs_inode_bitmap bitmap,
				     ext2_ino_t inode);

બાહ્ય શૂન્ય ext2fs_fast_mark_block_bitmap2(ext2fs_block_bitmap bitmap,
					   blk64_t block);
બાહ્ય શૂન્ય ext2fs_fast_unmark_block_bitmap2(ext2fs_block_bitmap bitmap,
					     blk64_t block);
બાહ્ય પૂર્ણાંક ext2fs_fast_test_block_bitmap2(ext2fs_block_bitmap bitmap,
					  blk64_t block);

બાહ્ય શૂન્ય ext2fs_fast_mark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
					   ext2_ino_t inode);
બાહ્ય શૂન્ય ext2fs_fast_unmark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
					    ext2_ino_t inode);
બાહ્ય પૂર્ણાંક ext2fs_fast_test_inode_bitmap2(ext2fs_inode_bitmap bitmap,
					  ext2_ino_t inode);
બાહ્ય errcode_t ext2fs_find_first_zero_block_bitmap2(ext2fs_block_bitmap bitmap,
						      blk64_t start,
						      blk64_t end,
						      blk64_t *out);
બાહ્ય errcode_t ext2fs_find_first_zero_inode_bitmap2(ext2fs_inode_bitmap bitmap,
						      ext2_ino_t start,
						      ext2_ino_t end,
						      ext2_ino_t *out);
બાહ્ય errcode_t ext2fs_find_first_set_block_bitmap2(ext2fs_block_bitmap bitmap,
						     blk64_t start,
						     blk64_t end,
						     blk64_t *out);
બાહ્ય errcode_t ext2fs_find_first_set_inode_bitmap2(ext2fs_inode_bitmap bitmap,
						      ext2_ino_t start,
						      ext2_ino_t end,
						      ext2_ino_t *out);
બાહ્ય blk64_t ext2fs_get_block_bitmap_start2(ext2fs_block_bitmap bitmap);
બાહ્ય ext2_ino_t ext2fs_get_inode_bitmap_start2(ext2fs_inode_bitmap bitmap);
બાહ્ય blk64_t ext2fs_get_block_bitmap_end2(ext2fs_block_bitmap bitmap);
બાહ્ય ext2_ino_t ext2fs_get_inode_bitmap_end2(ext2fs_inode_bitmap bitmap);

બાહ્ય પૂર્ણાંક ext2fs_fast_test_block_bitmap_range2(ext2fs_block_bitmap bitmap,
						blk64_t block,
						unsigned પૂર્ણાંક અંક);
બાહ્ય શૂન્ય ext2fs_fast_mark_block_bitmap_range2(ext2fs_block_bitmap bitmap,
						 blk64_t block,
						 unsigned પૂર્ણાંક અંક);
બાહ્ય શૂન્ય ext2fs_fast_unmark_block_bitmap_range2(ext2fs_block_bitmap bitmap,
						   blk64_t block,
						   unsigned પૂર્ણાંક અંક);
#endif

/* These routines moved to gen_bitmap64.c */
બાહ્ય શૂન્ય ext2fs_clear_generic_bmap(ext2fs_generic_bitmap bitmap);
બાહ્ય errcode_t ext2fs_compare_generic_bmap(errcode_t neq,
					     ext2fs_generic_bitmap bm1,
					     ext2fs_generic_bitmap bm2);
બાહ્ય શૂન્ય ext2fs_set_generic_bmap_padding(ext2fs_generic_bitmap bmap);
બાહ્ય પૂર્ણાંક ext2fs_mark_generic_bmap(ext2fs_generic_bitmap bitmap,
				    blk64_t bitno);
બાહ્ય પૂર્ણાંક ext2fs_unmark_generic_bmap(ext2fs_generic_bitmap bitmap,
				      blk64_t bitno);
બાહ્ય પૂર્ણાંક ext2fs_test_generic_bmap(ext2fs_generic_bitmap bitmap,
				    blk64_t bitno);
બાહ્ય પૂર્ણાંક ext2fs_test_block_bitmap_range2(ext2fs_block_bitmap bitmap,
					   blk64_t block, unsigned પૂર્ણાંક અંક);
બાહ્ય __u64 ext2fs_get_generic_bmap_start(ext2fs_generic_bitmap bitmap);
બાહ્ય __u64 ext2fs_get_generic_bmap_end(ext2fs_generic_bitmap bitmap);
બાહ્ય પૂર્ણાંક ext2fs_test_block_bitmap_range2(ext2fs_block_bitmap bitmap,
					   blk64_t block, unsigned પૂર્ણાંક અંક);
બાહ્ય શૂન્ય ext2fs_mark_block_bitmap_range2(ext2fs_block_bitmap bitmap,
					    blk64_t block, unsigned પૂર્ણાંક અંક);
બાહ્ય શૂન્ય ext2fs_unmark_block_bitmap_range2(ext2fs_block_bitmap bitmap,
					      blk64_t block, unsigned પૂર્ણાંક અંક);
બાહ્ય errcode_t ext2fs_find_first_zero_generic_bmap(ext2fs_generic_bitmap bitmap,
						     __u64 start, __u64 end,
						     __u64 *out);
બાહ્ય errcode_t ext2fs_find_first_set_generic_bmap(ext2fs_generic_bitmap bitmap,
						    __u64 start, __u64 end,
						    __u64 *out);

/*
 * The inline routines themselves...
 *
 * If NO_INLINE_FUNCS is defined, then we won't try to do inline
 * functions at all; they will be included as normal functions in
 * inline.c
 */

#જો (defined(INCLUDE_INLINE_FUNCS) || !defined(NO_INLINE_FUNCS))
#ifdef INCLUDE_INLINE_FUNCS
#જો (__STDC_VERSION__ >= 199901L)
#define _INLINE_ બાહ્ય અંતઃરેખ
#નહીંતર
#define _INLINE_ અંતઃરેખ
#endif
#નહીંતર /* !INCLUDE_INLINE FUNCS */
#જો (__STDC_VERSION__ >= 199901L)
#define _INLINE_ અંતઃરેખ
#નહીંતર /* not C99 */
#ifdef __GNUC__
#define _INLINE_ બાહ્ય __inline__
#નહીંતર				/* For Watcom C */
#define _INLINE_ બાહ્ય અંતઃરેખ
#endif /* __GNUC__ */
#endif /* __STDC_VERSION__ >= 199901L */
#endif /* INCLUDE_INLINE_FUNCS */

/*
 * Fast bit set/clear functions that doesn't need to return the
 * previous bit value.
 */

_INLINE_ શૂન્ય ext2fs_fast_set_bit(unsigned પૂર્ણાંક nr,શૂન્ય * addr)
{
	unsigned અક્ષર	*ADDR = (unsigned અક્ષર *) addr;

	ADDR += nr >> 3;
	*ADDR |= (unsigned અક્ષર) (1 << (nr & 0x07));
}

_INLINE_ શૂન્ય ext2fs_fast_clear_bit(unsigned પૂર્ણાંક nr, શૂન્ય * addr)
{
	unsigned અક્ષર	*ADDR = (unsigned અક્ષર *) addr;

	ADDR += nr >> 3;
	*ADDR &= (unsigned અક્ષર) ~(1 << (nr & 0x07));
}


_INLINE_ શૂન્ય ext2fs_fast_set_bit64(__u64 nr, શૂન્ય * addr)
{
	unsigned અક્ષર	*ADDR = (unsigned અક્ષર *) addr;

	ADDR += nr >> 3;
	*ADDR |= (unsigned અક્ષર) (1 << (nr & 0x07));
}

_INLINE_ શૂન્ય ext2fs_fast_clear_bit64(__u64 nr, શૂન્ય * addr)
{
	unsigned અક્ષર	*ADDR = (unsigned અક્ષર *) addr;

	ADDR += nr >> 3;
	*ADDR &= (unsigned અક્ષર) ~(1 << (nr & 0x07));
}

_INLINE_ __u16 ext2fs_swab16(__u16 val)
{
	પાછા (val >> 8) | (__u16) (val << 8);
}

_INLINE_ __u32 ext2fs_swab32(__u32 val)
{
	પાછા ((val>>24) | ((val>>8)&0xFF00) |
		((val<<8)&0xFF0000) | (val<<24));
}

_INLINE_ __u64 ext2fs_swab64(__u64 val)
{
	પાછા (ext2fs_swab32((__u32) (val >> 32)) |
		(((__u64)ext2fs_swab32(val & 0xFFFFFFFFUL)) << 32));
}

_INLINE_ પૂર્ણાંક ext2fs_mark_block_bitmap(ext2fs_block_bitmap bitmap,
				       blk_t block)
{
	પાછા ext2fs_mark_generic_bitmap((ext2fs_generic_bitmap) bitmap,
					  block);
}

_INLINE_ પૂર્ણાંક ext2fs_unmark_block_bitmap(ext2fs_block_bitmap bitmap,
					 blk_t block)
{
	પાછા ext2fs_unmark_generic_bitmap((ext2fs_generic_bitmap) bitmap,
					    block);
}

_INLINE_ પૂર્ણાંક ext2fs_test_block_bitmap(ext2fs_block_bitmap bitmap,
				       blk_t block)
{
	પાછા ext2fs_test_generic_bitmap((ext2fs_generic_bitmap) bitmap,
					  block);
}

_INLINE_ પૂર્ણાંક ext2fs_mark_inode_bitmap(ext2fs_inode_bitmap bitmap,
				       ext2_ino_t inode)
{
	પાછા ext2fs_mark_generic_bitmap((ext2fs_generic_bitmap) bitmap,
					  inode);
}

_INLINE_ પૂર્ણાંક ext2fs_unmark_inode_bitmap(ext2fs_inode_bitmap bitmap,
					 ext2_ino_t inode)
{
	પાછા ext2fs_unmark_generic_bitmap((ext2fs_generic_bitmap) bitmap,
				     inode);
}

_INLINE_ પૂર્ણાંક ext2fs_test_inode_bitmap(ext2fs_inode_bitmap bitmap,
				       ext2_ino_t inode)
{
	પાછા ext2fs_test_generic_bitmap((ext2fs_generic_bitmap) bitmap,
					  inode);
}

_INLINE_ શૂન્ય ext2fs_fast_mark_block_bitmap(ext2fs_block_bitmap bitmap,
					    blk_t block)
{
	ext2fs_mark_generic_bitmap((ext2fs_generic_bitmap) bitmap, block);
}

_INLINE_ શૂન્ય ext2fs_fast_unmark_block_bitmap(ext2fs_block_bitmap bitmap,
					      blk_t block)
{
	ext2fs_unmark_generic_bitmap((ext2fs_generic_bitmap) bitmap, block);
}

_INLINE_ પૂર્ણાંક ext2fs_fast_test_block_bitmap(ext2fs_block_bitmap bitmap,
					    blk_t block)
{
	પાછા ext2fs_test_generic_bitmap((ext2fs_generic_bitmap) bitmap,
					  block);
}

_INLINE_ શૂન્ય ext2fs_fast_mark_inode_bitmap(ext2fs_inode_bitmap bitmap,
					    ext2_ino_t inode)
{
	ext2fs_mark_generic_bitmap((ext2fs_generic_bitmap) bitmap, inode);
}

_INLINE_ શૂન્ય ext2fs_fast_unmark_inode_bitmap(ext2fs_inode_bitmap bitmap,
					      ext2_ino_t inode)
{
	ext2fs_unmark_generic_bitmap((ext2fs_generic_bitmap) bitmap, inode);
}

_INLINE_ પૂર્ણાંક ext2fs_fast_test_inode_bitmap(ext2fs_inode_bitmap bitmap,
					   ext2_ino_t inode)
{
	પાછા ext2fs_test_generic_bitmap((ext2fs_generic_bitmap) bitmap,
					  inode);
}

_INLINE_ blk_t ext2fs_get_block_bitmap_start(ext2fs_block_bitmap bitmap)
{
	પાછા ext2fs_get_generic_bitmap_start((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ ext2_ino_t ext2fs_get_inode_bitmap_start(ext2fs_inode_bitmap bitmap)
{
	પાછા ext2fs_get_generic_bitmap_start((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ blk_t ext2fs_get_block_bitmap_end(ext2fs_block_bitmap bitmap)
{
	પાછા ext2fs_get_generic_bitmap_end((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ ext2_ino_t ext2fs_get_inode_bitmap_end(ext2fs_inode_bitmap bitmap)
{
	પાછા ext2fs_get_generic_bitmap_end((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ પૂર્ણાંક ext2fs_fast_test_block_bitmap_range(ext2fs_block_bitmap bitmap,
						 blk_t block, પૂર્ણાંક અંક)
{
	પાછા ext2fs_test_block_bitmap_range(bitmap, block, અંક);
}

_INLINE_ શૂન્ય ext2fs_fast_mark_block_bitmap_range(ext2fs_block_bitmap bitmap,
						  blk_t block, પૂર્ણાંક અંક)
{
	ext2fs_mark_block_bitmap_range(bitmap, block, અંક);
}

_INLINE_ શૂન્ય ext2fs_fast_unmark_block_bitmap_range(ext2fs_block_bitmap bitmap,
						    blk_t block, પૂર્ણાંક અંક)
{
	ext2fs_unmark_block_bitmap_range(bitmap, block, અંક);
}

/* 64-bit versions */

_INLINE_ પૂર્ણાંક ext2fs_mark_block_bitmap2(ext2fs_block_bitmap bitmap,
				       blk64_t block)
{
	પાછા ext2fs_mark_generic_bmap((ext2fs_generic_bitmap) bitmap,
					block);
}

_INLINE_ પૂર્ણાંક ext2fs_unmark_block_bitmap2(ext2fs_block_bitmap bitmap,
					 blk64_t block)
{
	પાછા ext2fs_unmark_generic_bmap((ext2fs_generic_bitmap) bitmap, block);
}

_INLINE_ પૂર્ણાંક ext2fs_test_block_bitmap2(ext2fs_block_bitmap bitmap,
				       blk64_t block)
{
	પાછા ext2fs_test_generic_bmap((ext2fs_generic_bitmap) bitmap,
					block);
}

_INLINE_ પૂર્ણાંક ext2fs_mark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
				       ext2_ino_t inode)
{
	પાછા ext2fs_mark_generic_bmap((ext2fs_generic_bitmap) bitmap,
					inode);
}

_INLINE_ પૂર્ણાંક ext2fs_unmark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
					 ext2_ino_t inode)
{
	પાછા ext2fs_unmark_generic_bmap((ext2fs_generic_bitmap) bitmap,
					  inode);
}

_INLINE_ પૂર્ણાંક ext2fs_test_inode_bitmap2(ext2fs_inode_bitmap bitmap,
				       ext2_ino_t inode)
{
	પાછા ext2fs_test_generic_bmap((ext2fs_generic_bitmap) bitmap,
					inode);
}

_INLINE_ શૂન્ય ext2fs_fast_mark_block_bitmap2(ext2fs_block_bitmap bitmap,
					     blk64_t block)
{
	ext2fs_mark_generic_bmap((ext2fs_generic_bitmap) bitmap, block);
}

_INLINE_ શૂન્ય ext2fs_fast_unmark_block_bitmap2(ext2fs_block_bitmap bitmap,
					       blk64_t block)
{
	ext2fs_unmark_generic_bmap((ext2fs_generic_bitmap) bitmap, block);
}

_INLINE_ પૂર્ણાંક ext2fs_fast_test_block_bitmap2(ext2fs_block_bitmap bitmap,
					    blk64_t block)
{
	પાછા ext2fs_test_generic_bmap((ext2fs_generic_bitmap) bitmap,
					block);
}

_INLINE_ શૂન્ય ext2fs_fast_mark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
					     ext2_ino_t inode)
{
	ext2fs_mark_generic_bmap((ext2fs_generic_bitmap) bitmap, inode);
}

_INLINE_ શૂન્ય ext2fs_fast_unmark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
					       ext2_ino_t inode)
{
	ext2fs_unmark_generic_bmap((ext2fs_generic_bitmap) bitmap, inode);
}

_INLINE_ પૂર્ણાંક ext2fs_fast_test_inode_bitmap2(ext2fs_inode_bitmap bitmap,
					    ext2_ino_t inode)
{
	પાછા ext2fs_test_generic_bmap((ext2fs_generic_bitmap) bitmap,
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
	જો (!rv)
		*out = o;
	પાછા rv;
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
	જો (!rv)
		*out = (ext2_ino_t) o;
	પાછા rv;
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
	જો (!rv)
		*out = o;
	પાછા rv;
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
	જો (!rv)
		*out = (ext2_ino_t) o;
	પાછા rv;
}

_INLINE_ blk64_t ext2fs_get_block_bitmap_start2(ext2fs_block_bitmap bitmap)
{
	પાછા ext2fs_get_generic_bmap_start((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ ext2_ino_t ext2fs_get_inode_bitmap_start2(ext2fs_inode_bitmap bitmap)
{
	પાછા (ext2_ino_t) ext2fs_get_generic_bmap_start((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ blk64_t ext2fs_get_block_bitmap_end2(ext2fs_block_bitmap bitmap)
{
	પાછા ext2fs_get_generic_bmap_end((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ ext2_ino_t ext2fs_get_inode_bitmap_end2(ext2fs_inode_bitmap bitmap)
{
	પાછા (ext2_ino_t) ext2fs_get_generic_bmap_end((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ પૂર્ણાંક ext2fs_fast_test_block_bitmap_range2(ext2fs_block_bitmap bitmap,
						  blk64_t block,
						  unsigned પૂર્ણાંક અંક)
{
	પાછા ext2fs_test_block_bitmap_range2(bitmap, block, અંક);
}

_INLINE_ શૂન્ય ext2fs_fast_mark_block_bitmap_range2(ext2fs_block_bitmap bitmap,
						   blk64_t block,
						   unsigned પૂર્ણાંક અંક)
{
	ext2fs_mark_block_bitmap_range2(bitmap, block, અંક);
}

_INLINE_ શૂન્ય ext2fs_fast_unmark_block_bitmap_range2(ext2fs_block_bitmap bitmap,
						     blk64_t block,
						     unsigned પૂર્ણાંક અંક)
{
	ext2fs_unmark_block_bitmap_range2(bitmap, block, અંક);
}

#undef _INLINE_
#endif

બાહ્ય પૂર્ણાંક ext2fs_set_bit(unsigned પૂર્ણાંક nr,શૂન્ય * addr);
બાહ્ય પૂર્ણાંક ext2fs_clear_bit(unsigned પૂર્ણાંક nr, શૂન્ય * addr);
બાહ્ય પૂર્ણાંક ext2fs_test_bit(unsigned પૂર્ણાંક nr, અચલ શૂન્ય * addr);
બાહ્ય પૂર્ણાંક ext2fs_set_bit64(__u64 nr,શૂન્ય * addr);
બાહ્ય પૂર્ણાંક ext2fs_clear_bit64(__u64 nr, શૂન્ય * addr);
બાહ્ય પૂર્ણાંક ext2fs_test_bit64(__u64 nr, અચલ શૂન્ય * addr);
બાહ્ય unsigned પૂર્ણાંક ext2fs_bitcount(અચલ શૂન્ય *addr, unsigned પૂર્ણાંક nbytes);
