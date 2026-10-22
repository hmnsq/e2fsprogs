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

#ifdef വാക്കുകൾ_BIGENDIAN
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
#അല്ലെങ്കിൽ
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
ബാഹ്യ സ്ഥിരം അക്ഷരം *ext2fs_block_string;
ബാഹ്യ സ്ഥിരം അക്ഷരം *ext2fs_inode_string;
ബാഹ്യ സ്ഥിരം അക്ഷരം *ext2fs_mark_string;
ബാഹ്യ സ്ഥിരം അക്ഷരം *ext2fs_unmark_string;
ബാഹ്യ സ്ഥിരം അക്ഷരം *ext2fs_test_string;
ബാഹ്യ ശൂന്യം ext2fs_warn_bitmap(errcode_t errcode, unsigned long arg,
			       സ്ഥിരം അക്ഷരം *description);
ബാഹ്യ ശൂന്യം ext2fs_warn_bitmap2(ext2fs_generic_bitmap bitmap,
				പൂർണ്ണസംഖ്യ code, unsigned long arg);

#ifdef NO_INLINE_FUNCS
ബാഹ്യ ശൂന്യം ext2fs_fast_set_bit(unsigned പൂർണ്ണസംഖ്യ nr,ശൂന്യം * addr);
ബാഹ്യ ശൂന്യം ext2fs_fast_clear_bit(unsigned പൂർണ്ണസംഖ്യ nr, ശൂന്യം * addr);
ബാഹ്യ ശൂന്യം ext2fs_fast_set_bit64(__u64 nr,ശൂന്യം * addr);
ബാഹ്യ ശൂന്യം ext2fs_fast_clear_bit64(__u64 nr, ശൂന്യം * addr);
ബാഹ്യ __u16 ext2fs_swab16(__u16 val);
ബാഹ്യ __u32 ext2fs_swab32(__u32 val);
ബാഹ്യ __u64 ext2fs_swab64(__u64 val);

ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_mark_block_bitmap(ext2fs_block_bitmap bitmap, blk_t block);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_unmark_block_bitmap(ext2fs_block_bitmap bitmap,
				       blk_t block);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_test_block_bitmap(ext2fs_block_bitmap bitmap, blk_t block);

ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_mark_inode_bitmap(ext2fs_inode_bitmap bitmap, ext2_ino_t inode);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_unmark_inode_bitmap(ext2fs_inode_bitmap bitmap,
				       ext2_ino_t inode);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_test_inode_bitmap(ext2fs_inode_bitmap bitmap, ext2_ino_t inode);

ബാഹ്യ ശൂന്യം ext2fs_fast_mark_block_bitmap(ext2fs_block_bitmap bitmap,
					  blk_t block);
ബാഹ്യ ശൂന്യം ext2fs_fast_unmark_block_bitmap(ext2fs_block_bitmap bitmap,
					    blk_t block);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_fast_test_block_bitmap(ext2fs_block_bitmap bitmap,
					 blk_t block);

ബാഹ്യ ശൂന്യം ext2fs_fast_mark_inode_bitmap(ext2fs_inode_bitmap bitmap,
					  ext2_ino_t inode);
ബാഹ്യ ശൂന്യം ext2fs_fast_unmark_inode_bitmap(ext2fs_inode_bitmap bitmap,
					    ext2_ino_t inode);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_fast_test_inode_bitmap(ext2fs_inode_bitmap bitmap,
					 ext2_ino_t inode);
ബാഹ്യ blk_t ext2fs_get_block_bitmap_start(ext2fs_block_bitmap bitmap);
ബാഹ്യ ext2_ino_t ext2fs_get_inode_bitmap_start(ext2fs_inode_bitmap bitmap);
ബാഹ്യ blk_t ext2fs_get_block_bitmap_end(ext2fs_block_bitmap bitmap);
ബാഹ്യ ext2_ino_t ext2fs_get_inode_bitmap_end(ext2fs_inode_bitmap bitmap);

ബാഹ്യ ശൂന്യം ext2fs_fast_mark_block_bitmap_range(ext2fs_block_bitmap bitmap,
						blk_t block, പൂർണ്ണസംഖ്യ അക്കം);
ബാഹ്യ ശൂന്യം ext2fs_fast_unmark_block_bitmap_range(ext2fs_block_bitmap bitmap,
						  blk_t block, പൂർണ്ണസംഖ്യ അക്കം);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_fast_test_block_bitmap_range(ext2fs_block_bitmap bitmap,
					       blk_t block, പൂർണ്ണസംഖ്യ അക്കം);
#endif

/* These functions routines moved to gen_bitmap.c */
ബാഹ്യ ശൂന്യം ext2fs_mark_block_bitmap_range(ext2fs_block_bitmap bitmap,
					   blk_t block, പൂർണ്ണസംഖ്യ അക്കം);
ബാഹ്യ ശൂന്യം ext2fs_unmark_block_bitmap_range(ext2fs_block_bitmap bitmap,
					     blk_t block, പൂർണ്ണസംഖ്യ അക്കം);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_test_block_bitmap_range(ext2fs_block_bitmap bitmap,
					  blk_t block, പൂർണ്ണസംഖ്യ അക്കം);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_test_inode_bitmap_range(ext2fs_inode_bitmap bitmap,
					  ext2_ino_t inode, പൂർണ്ണസംഖ്യ അക്കം);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_mark_generic_bitmap(ext2fs_generic_bitmap bitmap,
					 __u32 bitno);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_unmark_generic_bitmap(ext2fs_generic_bitmap bitmap,
					   blk_t bitno);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_test_generic_bitmap(ext2fs_generic_bitmap bitmap,
				      blk_t bitno);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_test_block_bitmap_range(ext2fs_block_bitmap bitmap,
					  blk_t block, പൂർണ്ണസംഖ്യ അക്കം);
ബാഹ്യ ശൂന്യം ext2fs_set_bitmap_padding(ext2fs_generic_bitmap map);
ബാഹ്യ __u32 ext2fs_get_generic_bitmap_start(ext2fs_generic_bitmap bitmap);
ബാഹ്യ __u32 ext2fs_get_generic_bitmap_end(ext2fs_generic_bitmap bitmap);

/* 64-bit versions */

#ifdef NO_INLINE_FUNCS
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_mark_block_bitmap2(ext2fs_block_bitmap bitmap,
				     blk64_t block);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_unmark_block_bitmap2(ext2fs_block_bitmap bitmap,
				       blk64_t block);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_test_block_bitmap2(ext2fs_block_bitmap bitmap,
				     blk64_t block);

ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_mark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
				     ext2_ino_t inode);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_unmark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
				       ext2_ino_t inode);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_test_inode_bitmap2(ext2fs_inode_bitmap bitmap,
				     ext2_ino_t inode);

ബാഹ്യ ശൂന്യം ext2fs_fast_mark_block_bitmap2(ext2fs_block_bitmap bitmap,
					   blk64_t block);
ബാഹ്യ ശൂന്യം ext2fs_fast_unmark_block_bitmap2(ext2fs_block_bitmap bitmap,
					     blk64_t block);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_fast_test_block_bitmap2(ext2fs_block_bitmap bitmap,
					  blk64_t block);

ബാഹ്യ ശൂന്യം ext2fs_fast_mark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
					   ext2_ino_t inode);
ബാഹ്യ ശൂന്യം ext2fs_fast_unmark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
					    ext2_ino_t inode);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_fast_test_inode_bitmap2(ext2fs_inode_bitmap bitmap,
					  ext2_ino_t inode);
ബാഹ്യ errcode_t ext2fs_find_first_zero_block_bitmap2(ext2fs_block_bitmap bitmap,
						      blk64_t start,
						      blk64_t end,
						      blk64_t *out);
ബാഹ്യ errcode_t ext2fs_find_first_zero_inode_bitmap2(ext2fs_inode_bitmap bitmap,
						      ext2_ino_t start,
						      ext2_ino_t end,
						      ext2_ino_t *out);
ബാഹ്യ errcode_t ext2fs_find_first_set_block_bitmap2(ext2fs_block_bitmap bitmap,
						     blk64_t start,
						     blk64_t end,
						     blk64_t *out);
ബാഹ്യ errcode_t ext2fs_find_first_set_inode_bitmap2(ext2fs_inode_bitmap bitmap,
						      ext2_ino_t start,
						      ext2_ino_t end,
						      ext2_ino_t *out);
ബാഹ്യ blk64_t ext2fs_get_block_bitmap_start2(ext2fs_block_bitmap bitmap);
ബാഹ്യ ext2_ino_t ext2fs_get_inode_bitmap_start2(ext2fs_inode_bitmap bitmap);
ബാഹ്യ blk64_t ext2fs_get_block_bitmap_end2(ext2fs_block_bitmap bitmap);
ബാഹ്യ ext2_ino_t ext2fs_get_inode_bitmap_end2(ext2fs_inode_bitmap bitmap);

ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_fast_test_block_bitmap_range2(ext2fs_block_bitmap bitmap,
						blk64_t block,
						unsigned പൂർണ്ണസംഖ്യ അക്കം);
ബാഹ്യ ശൂന്യം ext2fs_fast_mark_block_bitmap_range2(ext2fs_block_bitmap bitmap,
						 blk64_t block,
						 unsigned പൂർണ്ണസംഖ്യ അക്കം);
ബാഹ്യ ശൂന്യം ext2fs_fast_unmark_block_bitmap_range2(ext2fs_block_bitmap bitmap,
						   blk64_t block,
						   unsigned പൂർണ്ണസംഖ്യ അക്കം);
#endif

/* These routines moved to gen_bitmap64.c */
ബാഹ്യ ശൂന്യം ext2fs_clear_generic_bmap(ext2fs_generic_bitmap bitmap);
ബാഹ്യ errcode_t ext2fs_compare_generic_bmap(errcode_t neq,
					     ext2fs_generic_bitmap bm1,
					     ext2fs_generic_bitmap bm2);
ബാഹ്യ ശൂന്യം ext2fs_set_generic_bmap_padding(ext2fs_generic_bitmap bmap);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_mark_generic_bmap(ext2fs_generic_bitmap bitmap,
				    blk64_t bitno);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_unmark_generic_bmap(ext2fs_generic_bitmap bitmap,
				      blk64_t bitno);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_test_generic_bmap(ext2fs_generic_bitmap bitmap,
				    blk64_t bitno);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_test_block_bitmap_range2(ext2fs_block_bitmap bitmap,
					   blk64_t block, unsigned പൂർണ്ണസംഖ്യ അക്കം);
ബാഹ്യ __u64 ext2fs_get_generic_bmap_start(ext2fs_generic_bitmap bitmap);
ബാഹ്യ __u64 ext2fs_get_generic_bmap_end(ext2fs_generic_bitmap bitmap);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_test_block_bitmap_range2(ext2fs_block_bitmap bitmap,
					   blk64_t block, unsigned പൂർണ്ണസംഖ്യ അക്കം);
ബാഹ്യ ശൂന്യം ext2fs_mark_block_bitmap_range2(ext2fs_block_bitmap bitmap,
					    blk64_t block, unsigned പൂർണ്ണസംഖ്യ അക്കം);
ബാഹ്യ ശൂന്യം ext2fs_unmark_block_bitmap_range2(ext2fs_block_bitmap bitmap,
					      blk64_t block, unsigned പൂർണ്ണസംഖ്യ അക്കം);
ബാഹ്യ errcode_t ext2fs_find_first_zero_generic_bmap(ext2fs_generic_bitmap bitmap,
						     __u64 start, __u64 end,
						     __u64 *out);
ബാഹ്യ errcode_t ext2fs_find_first_set_generic_bmap(ext2fs_generic_bitmap bitmap,
						    __u64 start, __u64 end,
						    __u64 *out);

/*
 * The inline routines themselves...
 *
 * If NO_INLINE_FUNCS is defined, then we won't try to do inline
 * functions at all; they will be included as normal functions in
 * inline.c
 */

#എങ്കിൽ (defined(INCLUDE_INLINE_FUNCS) || !defined(NO_INLINE_FUNCS))
#ifdef INCLUDE_INLINE_FUNCS
#എങ്കിൽ (__STDC_VERSION__ >= 199901L)
#define _INLINE_ ബാഹ്യ വരിയിൽ
#അല്ലെങ്കിൽ
#define _INLINE_ വരിയിൽ
#endif
#അല്ലെങ്കിൽ /* !INCLUDE_INLINE FUNCS */
#എങ്കിൽ (__STDC_VERSION__ >= 199901L)
#define _INLINE_ വരിയിൽ
#അല്ലെങ്കിൽ /* not C99 */
#ifdef __GNUC__
#define _INLINE_ ബാഹ്യ __inline__
#അല്ലെങ്കിൽ				/* For Watcom C */
#define _INLINE_ ബാഹ്യ വരിയിൽ
#endif /* __GNUC__ */
#endif /* __STDC_VERSION__ >= 199901L */
#endif /* INCLUDE_INLINE_FUNCS */

/*
 * Fast bit set/clear functions that doesn't need to return the
 * previous bit value.
 */

_INLINE_ ശൂന്യം ext2fs_fast_set_bit(unsigned പൂർണ്ണസംഖ്യ nr,ശൂന്യം * addr)
{
	unsigned അക്ഷരം	*ADDR = (unsigned അക്ഷരം *) addr;

	ADDR += nr >> 3;
	*ADDR |= (unsigned അക്ഷരം) (1 << (nr & 0x07));
}

_INLINE_ ശൂന്യം ext2fs_fast_clear_bit(unsigned പൂർണ്ണസംഖ്യ nr, ശൂന്യം * addr)
{
	unsigned അക്ഷരം	*ADDR = (unsigned അക്ഷരം *) addr;

	ADDR += nr >> 3;
	*ADDR &= (unsigned അക്ഷരം) ~(1 << (nr & 0x07));
}


_INLINE_ ശൂന്യം ext2fs_fast_set_bit64(__u64 nr, ശൂന്യം * addr)
{
	unsigned അക്ഷരം	*ADDR = (unsigned അക്ഷരം *) addr;

	ADDR += nr >> 3;
	*ADDR |= (unsigned അക്ഷരം) (1 << (nr & 0x07));
}

_INLINE_ ശൂന്യം ext2fs_fast_clear_bit64(__u64 nr, ശൂന്യം * addr)
{
	unsigned അക്ഷരം	*ADDR = (unsigned അക്ഷരം *) addr;

	ADDR += nr >> 3;
	*ADDR &= (unsigned അക്ഷരം) ~(1 << (nr & 0x07));
}

_INLINE_ __u16 ext2fs_swab16(__u16 val)
{
	തിരികെ (val >> 8) | (__u16) (val << 8);
}

_INLINE_ __u32 ext2fs_swab32(__u32 val)
{
	തിരികെ ((val>>24) | ((val>>8)&0xFF00) |
		((val<<8)&0xFF0000) | (val<<24));
}

_INLINE_ __u64 ext2fs_swab64(__u64 val)
{
	തിരികെ (ext2fs_swab32((__u32) (val >> 32)) |
		(((__u64)ext2fs_swab32(val & 0xFFFFFFFFUL)) << 32));
}

_INLINE_ പൂർണ്ണസംഖ്യ ext2fs_mark_block_bitmap(ext2fs_block_bitmap bitmap,
				       blk_t block)
{
	തിരികെ ext2fs_mark_generic_bitmap((ext2fs_generic_bitmap) bitmap,
					  block);
}

_INLINE_ പൂർണ്ണസംഖ്യ ext2fs_unmark_block_bitmap(ext2fs_block_bitmap bitmap,
					 blk_t block)
{
	തിരികെ ext2fs_unmark_generic_bitmap((ext2fs_generic_bitmap) bitmap,
					    block);
}

_INLINE_ പൂർണ്ണസംഖ്യ ext2fs_test_block_bitmap(ext2fs_block_bitmap bitmap,
				       blk_t block)
{
	തിരികെ ext2fs_test_generic_bitmap((ext2fs_generic_bitmap) bitmap,
					  block);
}

_INLINE_ പൂർണ്ണസംഖ്യ ext2fs_mark_inode_bitmap(ext2fs_inode_bitmap bitmap,
				       ext2_ino_t inode)
{
	തിരികെ ext2fs_mark_generic_bitmap((ext2fs_generic_bitmap) bitmap,
					  inode);
}

_INLINE_ പൂർണ്ണസംഖ്യ ext2fs_unmark_inode_bitmap(ext2fs_inode_bitmap bitmap,
					 ext2_ino_t inode)
{
	തിരികെ ext2fs_unmark_generic_bitmap((ext2fs_generic_bitmap) bitmap,
				     inode);
}

_INLINE_ പൂർണ്ണസംഖ്യ ext2fs_test_inode_bitmap(ext2fs_inode_bitmap bitmap,
				       ext2_ino_t inode)
{
	തിരികെ ext2fs_test_generic_bitmap((ext2fs_generic_bitmap) bitmap,
					  inode);
}

_INLINE_ ശൂന്യം ext2fs_fast_mark_block_bitmap(ext2fs_block_bitmap bitmap,
					    blk_t block)
{
	ext2fs_mark_generic_bitmap((ext2fs_generic_bitmap) bitmap, block);
}

_INLINE_ ശൂന്യം ext2fs_fast_unmark_block_bitmap(ext2fs_block_bitmap bitmap,
					      blk_t block)
{
	ext2fs_unmark_generic_bitmap((ext2fs_generic_bitmap) bitmap, block);
}

_INLINE_ പൂർണ്ണസംഖ്യ ext2fs_fast_test_block_bitmap(ext2fs_block_bitmap bitmap,
					    blk_t block)
{
	തിരികെ ext2fs_test_generic_bitmap((ext2fs_generic_bitmap) bitmap,
					  block);
}

_INLINE_ ശൂന്യം ext2fs_fast_mark_inode_bitmap(ext2fs_inode_bitmap bitmap,
					    ext2_ino_t inode)
{
	ext2fs_mark_generic_bitmap((ext2fs_generic_bitmap) bitmap, inode);
}

_INLINE_ ശൂന്യം ext2fs_fast_unmark_inode_bitmap(ext2fs_inode_bitmap bitmap,
					      ext2_ino_t inode)
{
	ext2fs_unmark_generic_bitmap((ext2fs_generic_bitmap) bitmap, inode);
}

_INLINE_ പൂർണ്ണസംഖ്യ ext2fs_fast_test_inode_bitmap(ext2fs_inode_bitmap bitmap,
					   ext2_ino_t inode)
{
	തിരികെ ext2fs_test_generic_bitmap((ext2fs_generic_bitmap) bitmap,
					  inode);
}

_INLINE_ blk_t ext2fs_get_block_bitmap_start(ext2fs_block_bitmap bitmap)
{
	തിരികെ ext2fs_get_generic_bitmap_start((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ ext2_ino_t ext2fs_get_inode_bitmap_start(ext2fs_inode_bitmap bitmap)
{
	തിരികെ ext2fs_get_generic_bitmap_start((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ blk_t ext2fs_get_block_bitmap_end(ext2fs_block_bitmap bitmap)
{
	തിരികെ ext2fs_get_generic_bitmap_end((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ ext2_ino_t ext2fs_get_inode_bitmap_end(ext2fs_inode_bitmap bitmap)
{
	തിരികെ ext2fs_get_generic_bitmap_end((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ പൂർണ്ണസംഖ്യ ext2fs_fast_test_block_bitmap_range(ext2fs_block_bitmap bitmap,
						 blk_t block, പൂർണ്ണസംഖ്യ അക്കം)
{
	തിരികെ ext2fs_test_block_bitmap_range(bitmap, block, അക്കം);
}

_INLINE_ ശൂന്യം ext2fs_fast_mark_block_bitmap_range(ext2fs_block_bitmap bitmap,
						  blk_t block, പൂർണ്ണസംഖ്യ അക്കം)
{
	ext2fs_mark_block_bitmap_range(bitmap, block, അക്കം);
}

_INLINE_ ശൂന്യം ext2fs_fast_unmark_block_bitmap_range(ext2fs_block_bitmap bitmap,
						    blk_t block, പൂർണ്ണസംഖ്യ അക്കം)
{
	ext2fs_unmark_block_bitmap_range(bitmap, block, അക്കം);
}

/* 64-bit versions */

_INLINE_ പൂർണ്ണസംഖ്യ ext2fs_mark_block_bitmap2(ext2fs_block_bitmap bitmap,
				       blk64_t block)
{
	തിരികെ ext2fs_mark_generic_bmap((ext2fs_generic_bitmap) bitmap,
					block);
}

_INLINE_ പൂർണ്ണസംഖ്യ ext2fs_unmark_block_bitmap2(ext2fs_block_bitmap bitmap,
					 blk64_t block)
{
	തിരികെ ext2fs_unmark_generic_bmap((ext2fs_generic_bitmap) bitmap, block);
}

_INLINE_ പൂർണ്ണസംഖ്യ ext2fs_test_block_bitmap2(ext2fs_block_bitmap bitmap,
				       blk64_t block)
{
	തിരികെ ext2fs_test_generic_bmap((ext2fs_generic_bitmap) bitmap,
					block);
}

_INLINE_ പൂർണ്ണസംഖ്യ ext2fs_mark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
				       ext2_ino_t inode)
{
	തിരികെ ext2fs_mark_generic_bmap((ext2fs_generic_bitmap) bitmap,
					inode);
}

_INLINE_ പൂർണ്ണസംഖ്യ ext2fs_unmark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
					 ext2_ino_t inode)
{
	തിരികെ ext2fs_unmark_generic_bmap((ext2fs_generic_bitmap) bitmap,
					  inode);
}

_INLINE_ പൂർണ്ണസംഖ്യ ext2fs_test_inode_bitmap2(ext2fs_inode_bitmap bitmap,
				       ext2_ino_t inode)
{
	തിരികെ ext2fs_test_generic_bmap((ext2fs_generic_bitmap) bitmap,
					inode);
}

_INLINE_ ശൂന്യം ext2fs_fast_mark_block_bitmap2(ext2fs_block_bitmap bitmap,
					     blk64_t block)
{
	ext2fs_mark_generic_bmap((ext2fs_generic_bitmap) bitmap, block);
}

_INLINE_ ശൂന്യം ext2fs_fast_unmark_block_bitmap2(ext2fs_block_bitmap bitmap,
					       blk64_t block)
{
	ext2fs_unmark_generic_bmap((ext2fs_generic_bitmap) bitmap, block);
}

_INLINE_ പൂർണ്ണസംഖ്യ ext2fs_fast_test_block_bitmap2(ext2fs_block_bitmap bitmap,
					    blk64_t block)
{
	തിരികെ ext2fs_test_generic_bmap((ext2fs_generic_bitmap) bitmap,
					block);
}

_INLINE_ ശൂന്യം ext2fs_fast_mark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
					     ext2_ino_t inode)
{
	ext2fs_mark_generic_bmap((ext2fs_generic_bitmap) bitmap, inode);
}

_INLINE_ ശൂന്യം ext2fs_fast_unmark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
					       ext2_ino_t inode)
{
	ext2fs_unmark_generic_bmap((ext2fs_generic_bitmap) bitmap, inode);
}

_INLINE_ പൂർണ്ണസംഖ്യ ext2fs_fast_test_inode_bitmap2(ext2fs_inode_bitmap bitmap,
					    ext2_ino_t inode)
{
	തിരികെ ext2fs_test_generic_bmap((ext2fs_generic_bitmap) bitmap,
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
	എങ്കിൽ (!rv)
		*out = o;
	തിരികെ rv;
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
	എങ്കിൽ (!rv)
		*out = (ext2_ino_t) o;
	തിരികെ rv;
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
	എങ്കിൽ (!rv)
		*out = o;
	തിരികെ rv;
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
	എങ്കിൽ (!rv)
		*out = (ext2_ino_t) o;
	തിരികെ rv;
}

_INLINE_ blk64_t ext2fs_get_block_bitmap_start2(ext2fs_block_bitmap bitmap)
{
	തിരികെ ext2fs_get_generic_bmap_start((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ ext2_ino_t ext2fs_get_inode_bitmap_start2(ext2fs_inode_bitmap bitmap)
{
	തിരികെ (ext2_ino_t) ext2fs_get_generic_bmap_start((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ blk64_t ext2fs_get_block_bitmap_end2(ext2fs_block_bitmap bitmap)
{
	തിരികെ ext2fs_get_generic_bmap_end((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ ext2_ino_t ext2fs_get_inode_bitmap_end2(ext2fs_inode_bitmap bitmap)
{
	തിരികെ (ext2_ino_t) ext2fs_get_generic_bmap_end((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ പൂർണ്ണസംഖ്യ ext2fs_fast_test_block_bitmap_range2(ext2fs_block_bitmap bitmap,
						  blk64_t block,
						  unsigned പൂർണ്ണസംഖ്യ അക്കം)
{
	തിരികെ ext2fs_test_block_bitmap_range2(bitmap, block, അക്കം);
}

_INLINE_ ശൂന്യം ext2fs_fast_mark_block_bitmap_range2(ext2fs_block_bitmap bitmap,
						   blk64_t block,
						   unsigned പൂർണ്ണസംഖ്യ അക്കം)
{
	ext2fs_mark_block_bitmap_range2(bitmap, block, അക്കം);
}

_INLINE_ ശൂന്യം ext2fs_fast_unmark_block_bitmap_range2(ext2fs_block_bitmap bitmap,
						     blk64_t block,
						     unsigned പൂർണ്ണസംഖ്യ അക്കം)
{
	ext2fs_unmark_block_bitmap_range2(bitmap, block, അക്കം);
}

#undef _INLINE_
#endif

ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_set_bit(unsigned പൂർണ്ണസംഖ്യ nr,ശൂന്യം * addr);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_clear_bit(unsigned പൂർണ്ണസംഖ്യ nr, ശൂന്യം * addr);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_test_bit(unsigned പൂർണ്ണസംഖ്യ nr, സ്ഥിരം ശൂന്യം * addr);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_set_bit64(__u64 nr,ശൂന്യം * addr);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_clear_bit64(__u64 nr, ശൂന്യം * addr);
ബാഹ്യ പൂർണ്ണസംഖ്യ ext2fs_test_bit64(__u64 nr, സ്ഥിരം ശൂന്യം * addr);
ബാഹ്യ unsigned പൂർണ്ണസംഖ്യ ext2fs_bitcount(സ്ഥിരം ശൂന്യം *addr, unsigned പൂർണ്ണസംഖ്യ nbytes);
