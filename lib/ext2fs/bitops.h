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

#यदि_परिभाषितम् शब्दाः_BIGENDIAN
#परिभाषय ext2fs_cpu_गन्तव्यम्_le64(x) ((__force __le64)ext2fs_swab64((__u64)(x)))
#परिभाषय ext2fs_le64_गन्तव्यम्_cpu(x) ext2fs_swab64((__force __u64)(__le64)(x))
#परिभाषय ext2fs_cpu_गन्तव्यम्_le32(x) ((__force __le32)ext2fs_swab32((__u32)(x)))
#परिभाषय ext2fs_le32_गन्तव्यम्_cpu(x) ext2fs_swab32((__force __u32)(__le32)(x))
#परिभाषय ext2fs_cpu_गन्तव्यम्_le16(x) ((__force __le16)ext2fs_swab16((__u16)(x)))
#परिभाषय ext2fs_le16_गन्तव्यम्_cpu(x) ext2fs_swab16((__force __u16)(__le16)(x))

#परिभाषय ext2fs_cpu_गन्तव्यम्_be64(x) ((__force __be64)(__u64)(x))
#परिभाषय ext2fs_be64_गन्तव्यम्_cpu(x) ((__force __u64)(__be64)(x))
#परिभाषय ext2fs_cpu_गन्तव्यम्_be32(x) ((__force __be32)(__u32)(x))
#परिभाषय ext2fs_be32_गन्तव्यम्_cpu(x) ((__force __u32)(__be32)(x))
#परिभाषय ext2fs_cpu_गन्तव्यम्_be16(x) ((__force __be16)(__u16)(x))
#परिभाषय ext2fs_be16_गन्तव्यम्_cpu(x) ((__force __u16)(__be16)(x))
#अन्यथा
#परिभाषय ext2fs_cpu_गन्तव्यम्_le64(x) ((__force __le64)(__u64)(x))
#परिभाषय ext2fs_le64_गन्तव्यम्_cpu(x) ((__force __u64)(__le64)(x))
#परिभाषय ext2fs_cpu_गन्तव्यम्_le32(x) ((__force __le32)(__u32)(x))
#परिभाषय ext2fs_le32_गन्तव्यम्_cpu(x) ((__force __u32)(__le32)(x))
#परिभाषय ext2fs_cpu_गन्तव्यम्_le16(x) ((__force __le16)(__u16)(x))
#परिभाषय ext2fs_le16_गन्तव्यम्_cpu(x) ((__force __u16)(__le16)(x))

#परिभाषय ext2fs_cpu_गन्तव्यम्_be64(x) ((__force __be64)ext2fs_swab64((__u64)(x)))
#परिभाषय ext2fs_be64_गन्तव्यम्_cpu(x) ext2fs_swab64((__force __u64)(__be64)(x))
#परिभाषय ext2fs_cpu_गन्तव्यम्_be32(x) ((__force __be32)ext2fs_swab32((__u32)(x)))
#परिभाषय ext2fs_be32_गन्तव्यम्_cpu(x) ext2fs_swab32((__force __u32)(__be32)(x))
#परिभाषय ext2fs_cpu_गन्तव्यम्_be16(x) ((__force __be16)ext2fs_swab16((__u16)(x)))
#परिभाषय ext2fs_be16_गन्तव्यम्_cpu(x) ext2fs_swab16((__force __u16)(__be16)(x))
#यदि_अन्तः

/*
 * EXT2FS bitmap manipulation routines.
 */

/* Support for sending warning messages from the inline subroutines */
बाह्य ध्रुव वर्ण *ext2fs_खण्डम्_string;
बाह्य ध्रुव वर्ण *ext2fs_inode_string;
बाह्य ध्रुव वर्ण *ext2fs_mark_string;
बाह्य ध्रुव वर्ण *ext2fs_unmark_string;
बाह्य ध्रुव वर्ण *ext2fs_test_string;
बाह्य शून्य ext2fs_warn_bitmap(errcode_t errcode, अचिह्नित दीर्घ arg,
			       ध्रुव वर्ण *description);
बाह्य शून्य ext2fs_warn_bitmap2(ext2fs_generic_bitmap bitmap,
				पूर्णाङ्क संहिता, अचिह्नित दीर्घ arg);

#यदि_परिभाषितम् NO_INLINE_FUNCS
बाह्य शून्य ext2fs_fast_समुच्चयः_bit(अचिह्नित पूर्णाङ्क nr,शून्य * addr);
बाह्य शून्य ext2fs_fast_clear_bit(अचिह्नित पूर्णाङ्क nr, शून्य * addr);
बाह्य शून्य ext2fs_fast_समुच्चयः_bit64(__u64 nr,शून्य * addr);
बाह्य शून्य ext2fs_fast_clear_bit64(__u64 nr, शून्य * addr);
बाह्य __u16 ext2fs_swab16(__u16 val);
बाह्य __u32 ext2fs_swab32(__u32 val);
बाह्य __u64 ext2fs_swab64(__u64 val);

बाह्य पूर्णाङ्क ext2fs_mark_खण्डम्_bitmap(ext2fs_खण्डम्_bitmap bitmap, blk_t खण्डम्);
बाह्य पूर्णाङ्क ext2fs_unmark_खण्डम्_bitmap(ext2fs_खण्डम्_bitmap bitmap,
				       blk_t खण्डम्);
बाह्य पूर्णाङ्क ext2fs_test_खण्डम्_bitmap(ext2fs_खण्डम्_bitmap bitmap, blk_t खण्डम्);

बाह्य पूर्णाङ्क ext2fs_mark_inode_bitmap(ext2fs_inode_bitmap bitmap, ext2_ino_t inode);
बाह्य पूर्णाङ्क ext2fs_unmark_inode_bitmap(ext2fs_inode_bitmap bitmap,
				       ext2_ino_t inode);
बाह्य पूर्णाङ्क ext2fs_test_inode_bitmap(ext2fs_inode_bitmap bitmap, ext2_ino_t inode);

बाह्य शून्य ext2fs_fast_mark_खण्डम्_bitmap(ext2fs_खण्डम्_bitmap bitmap,
					  blk_t खण्डम्);
बाह्य शून्य ext2fs_fast_unmark_खण्डम्_bitmap(ext2fs_खण्डम्_bitmap bitmap,
					    blk_t खण्डम्);
बाह्य पूर्णाङ्क ext2fs_fast_test_खण्डम्_bitmap(ext2fs_खण्डम्_bitmap bitmap,
					 blk_t खण्डम्);

बाह्य शून्य ext2fs_fast_mark_inode_bitmap(ext2fs_inode_bitmap bitmap,
					  ext2_ino_t inode);
बाह्य शून्य ext2fs_fast_unmark_inode_bitmap(ext2fs_inode_bitmap bitmap,
					    ext2_ino_t inode);
बाह्य पूर्णाङ्क ext2fs_fast_test_inode_bitmap(ext2fs_inode_bitmap bitmap,
					 ext2_ino_t inode);
बाह्य blk_t ext2fs_get_खण्डम्_bitmap_start(ext2fs_खण्डम्_bitmap bitmap);
बाह्य ext2_ino_t ext2fs_get_inode_bitmap_start(ext2fs_inode_bitmap bitmap);
बाह्य blk_t ext2fs_get_खण्डम्_bitmap_अन्तः(ext2fs_खण्डम्_bitmap bitmap);
बाह्य ext2_ino_t ext2fs_get_inode_bitmap_अन्तः(ext2fs_inode_bitmap bitmap);

बाह्य शून्य ext2fs_fast_mark_खण्डम्_bitmap_range(ext2fs_खण्डम्_bitmap bitmap,
						blk_t खण्डम्, पूर्णाङ्क अङ्कः);
बाह्य शून्य ext2fs_fast_unmark_खण्डम्_bitmap_range(ext2fs_खण्डम्_bitmap bitmap,
						  blk_t खण्डम्, पूर्णाङ्क अङ्कः);
बाह्य पूर्णाङ्क ext2fs_fast_test_खण्डम्_bitmap_range(ext2fs_खण्डम्_bitmap bitmap,
					       blk_t खण्डम्, पूर्णाङ्क अङ्कः);
#यदि_अन्तः

/* These functions routines moved to gen_bitmap.c */
बाह्य शून्य ext2fs_mark_खण्डम्_bitmap_range(ext2fs_खण्डम्_bitmap bitmap,
					   blk_t खण्डम्, पूर्णाङ्क अङ्कः);
बाह्य शून्य ext2fs_unmark_खण्डम्_bitmap_range(ext2fs_खण्डम्_bitmap bitmap,
					     blk_t खण्डम्, पूर्णाङ्क अङ्कः);
बाह्य पूर्णाङ्क ext2fs_test_खण्डम्_bitmap_range(ext2fs_खण्डम्_bitmap bitmap,
					  blk_t खण्डम्, पूर्णाङ्क अङ्कः);
बाह्य पूर्णाङ्क ext2fs_test_inode_bitmap_range(ext2fs_inode_bitmap bitmap,
					  ext2_ino_t inode, पूर्णाङ्क अङ्कः);
बाह्य पूर्णाङ्क ext2fs_mark_generic_bitmap(ext2fs_generic_bitmap bitmap,
					 __u32 bitno);
बाह्य पूर्णाङ्क ext2fs_unmark_generic_bitmap(ext2fs_generic_bitmap bitmap,
					   blk_t bitno);
बाह्य पूर्णाङ्क ext2fs_test_generic_bitmap(ext2fs_generic_bitmap bitmap,
				      blk_t bitno);
बाह्य पूर्णाङ्क ext2fs_test_खण्डम्_bitmap_range(ext2fs_खण्डम्_bitmap bitmap,
					  blk_t खण्डम्, पूर्णाङ्क अङ्कः);
बाह्य शून्य ext2fs_समुच्चयः_bitmap_padding(ext2fs_generic_bitmap map);
बाह्य __u32 ext2fs_get_generic_bitmap_start(ext2fs_generic_bitmap bitmap);
बाह्य __u32 ext2fs_get_generic_bitmap_अन्तः(ext2fs_generic_bitmap bitmap);

/* 64-bit versions */

#यदि_परिभाषितम् NO_INLINE_FUNCS
बाह्य पूर्णाङ्क ext2fs_mark_खण्डम्_bitmap2(ext2fs_खण्डम्_bitmap bitmap,
				     blk64_t खण्डम्);
बाह्य पूर्णाङ्क ext2fs_unmark_खण्डम्_bitmap2(ext2fs_खण्डम्_bitmap bitmap,
				       blk64_t खण्डम्);
बाह्य पूर्णाङ्क ext2fs_test_खण्डम्_bitmap2(ext2fs_खण्डम्_bitmap bitmap,
				     blk64_t खण्डम्);

बाह्य पूर्णाङ्क ext2fs_mark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
				     ext2_ino_t inode);
बाह्य पूर्णाङ्क ext2fs_unmark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
				       ext2_ino_t inode);
बाह्य पूर्णाङ्क ext2fs_test_inode_bitmap2(ext2fs_inode_bitmap bitmap,
				     ext2_ino_t inode);

बाह्य शून्य ext2fs_fast_mark_खण्डम्_bitmap2(ext2fs_खण्डम्_bitmap bitmap,
					   blk64_t खण्डम्);
बाह्य शून्य ext2fs_fast_unmark_खण्डम्_bitmap2(ext2fs_खण्डम्_bitmap bitmap,
					     blk64_t खण्डम्);
बाह्य पूर्णाङ्क ext2fs_fast_test_खण्डम्_bitmap2(ext2fs_खण्डम्_bitmap bitmap,
					  blk64_t खण्डम्);

बाह्य शून्य ext2fs_fast_mark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
					   ext2_ino_t inode);
बाह्य शून्य ext2fs_fast_unmark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
					    ext2_ino_t inode);
बाह्य पूर्णाङ्क ext2fs_fast_test_inode_bitmap2(ext2fs_inode_bitmap bitmap,
					  ext2_ino_t inode);
बाह्य errcode_t ext2fs_find_first_zero_खण्डम्_bitmap2(ext2fs_खण्डम्_bitmap bitmap,
						      blk64_t start,
						      blk64_t अन्तः,
						      blk64_t *बहिर्गमनम्);
बाह्य errcode_t ext2fs_find_first_zero_inode_bitmap2(ext2fs_inode_bitmap bitmap,
						      ext2_ino_t start,
						      ext2_ino_t अन्तः,
						      ext2_ino_t *बहिर्गमनम्);
बाह्य errcode_t ext2fs_find_first_समुच्चयः_खण्डम्_bitmap2(ext2fs_खण्डम्_bitmap bitmap,
						     blk64_t start,
						     blk64_t अन्तः,
						     blk64_t *बहिर्गमनम्);
बाह्य errcode_t ext2fs_find_first_समुच्चयः_inode_bitmap2(ext2fs_inode_bitmap bitmap,
						      ext2_ino_t start,
						      ext2_ino_t अन्तः,
						      ext2_ino_t *बहिर्गमनम्);
बाह्य blk64_t ext2fs_get_खण्डम्_bitmap_start2(ext2fs_खण्डम्_bitmap bitmap);
बाह्य ext2_ino_t ext2fs_get_inode_bitmap_start2(ext2fs_inode_bitmap bitmap);
बाह्य blk64_t ext2fs_get_खण्डम्_bitmap_end2(ext2fs_खण्डम्_bitmap bitmap);
बाह्य ext2_ino_t ext2fs_get_inode_bitmap_end2(ext2fs_inode_bitmap bitmap);

बाह्य पूर्णाङ्क ext2fs_fast_test_खण्डम्_bitmap_range2(ext2fs_खण्डम्_bitmap bitmap,
						blk64_t खण्डम्,
						अचिह्नित पूर्णाङ्क अङ्कः);
बाह्य शून्य ext2fs_fast_mark_खण्डम्_bitmap_range2(ext2fs_खण्डम्_bitmap bitmap,
						 blk64_t खण्डम्,
						 अचिह्नित पूर्णाङ्क अङ्कः);
बाह्य शून्य ext2fs_fast_unmark_खण्डम्_bitmap_range2(ext2fs_खण्डम्_bitmap bitmap,
						   blk64_t खण्डम्,
						   अचिह्नित पूर्णाङ्क अङ्कः);
#यदि_अन्तः

/* These routines moved to gen_bitmap64.c */
बाह्य शून्य ext2fs_clear_generic_bmap(ext2fs_generic_bitmap bitmap);
बाह्य errcode_t ext2fs_compare_generic_bmap(errcode_t neq,
					     ext2fs_generic_bitmap bm1,
					     ext2fs_generic_bitmap bm2);
बाह्य शून्य ext2fs_समुच्चयः_generic_bmap_padding(ext2fs_generic_bitmap bmap);
बाह्य पूर्णाङ्क ext2fs_mark_generic_bmap(ext2fs_generic_bitmap bitmap,
				    blk64_t bitno);
बाह्य पूर्णाङ्क ext2fs_unmark_generic_bmap(ext2fs_generic_bitmap bitmap,
				      blk64_t bitno);
बाह्य पूर्णाङ्क ext2fs_test_generic_bmap(ext2fs_generic_bitmap bitmap,
				    blk64_t bitno);
बाह्य पूर्णाङ्क ext2fs_test_खण्डम्_bitmap_range2(ext2fs_खण्डम्_bitmap bitmap,
					   blk64_t खण्डम्, अचिह्नित पूर्णाङ्क अङ्कः);
बाह्य __u64 ext2fs_get_generic_bmap_start(ext2fs_generic_bitmap bitmap);
बाह्य __u64 ext2fs_get_generic_bmap_अन्तः(ext2fs_generic_bitmap bitmap);
बाह्य पूर्णाङ्क ext2fs_test_खण्डम्_bitmap_range2(ext2fs_खण्डम्_bitmap bitmap,
					   blk64_t खण्डम्, अचिह्नित पूर्णाङ्क अङ्कः);
बाह्य शून्य ext2fs_mark_खण्डम्_bitmap_range2(ext2fs_खण्डम्_bitmap bitmap,
					    blk64_t खण्डम्, अचिह्नित पूर्णाङ्क अङ्कः);
बाह्य शून्य ext2fs_unmark_खण्डम्_bitmap_range2(ext2fs_खण्डम्_bitmap bitmap,
					      blk64_t खण्डम्, अचिह्नित पूर्णाङ्क अङ्कः);
बाह्य errcode_t ext2fs_find_first_zero_generic_bmap(ext2fs_generic_bitmap bitmap,
						     __u64 start, __u64 अन्तः,
						     __u64 *बहिर्गमनम्);
बाह्य errcode_t ext2fs_find_first_समुच्चयः_generic_bmap(ext2fs_generic_bitmap bitmap,
						    __u64 start, __u64 अन्तः,
						    __u64 *बहिर्गमनम्);

/*
 * The inline routines themselves...
 *
 * If NO_INLINE_FUNCS is defined, then we won't try to do inline
 * functions at all; they will be included as normal functions in
 * inline.c
 */

#यदि (defined(INCLUDE_INLINE_FUNCS) || !defined(NO_INLINE_FUNCS))
#यदि_परिभाषितम् INCLUDE_INLINE_FUNCS
#यदि (__STDC_VERSION__ >= 199901L)
#परिभाषय _INLINE_ बाह्य अन्तःस्थ
#अन्यथा
#परिभाषय _INLINE_ अन्तःस्थ
#यदि_अन्तः
#अन्यथा /* !INCLUDE_INLINE FUNCS */
#यदि (__STDC_VERSION__ >= 199901L)
#परिभाषय _INLINE_ अन्तःस्थ
#अन्यथा /* not C99 */
#यदि_परिभाषितम् __GNUC__
#परिभाषय _INLINE_ बाह्य __inline__
#अन्यथा				/* For Watcom C */
#परिभाषय _INLINE_ बाह्य अन्तःस्थ
#यदि_अन्तः /* __GNUC__ */
#यदि_अन्तः /* __STDC_VERSION__ >= 199901L */
#यदि_अन्तः /* INCLUDE_INLINE_FUNCS */

/*
 * Fast bit set/clear functions that doesn't need to return the
 * previous bit value.
 */

_INLINE_ शून्य ext2fs_fast_समुच्चयः_bit(अचिह्नित पूर्णाङ्क nr,शून्य * addr)
{
	अचिह्नित वर्ण	*ADDR = (अचिह्नित वर्ण *) addr;

	ADDR += nr >> 3;
	*ADDR |= (अचिह्नित वर्ण) (1 << (nr & 0x07));
}

_INLINE_ शून्य ext2fs_fast_clear_bit(अचिह्नित पूर्णाङ्क nr, शून्य * addr)
{
	अचिह्नित वर्ण	*ADDR = (अचिह्नित वर्ण *) addr;

	ADDR += nr >> 3;
	*ADDR &= (अचिह्नित वर्ण) ~(1 << (nr & 0x07));
}


_INLINE_ शून्य ext2fs_fast_समुच्चयः_bit64(__u64 nr, शून्य * addr)
{
	अचिह्नित वर्ण	*ADDR = (अचिह्नित वर्ण *) addr;

	ADDR += nr >> 3;
	*ADDR |= (अचिह्नित वर्ण) (1 << (nr & 0x07));
}

_INLINE_ शून्य ext2fs_fast_clear_bit64(__u64 nr, शून्य * addr)
{
	अचिह्नित वर्ण	*ADDR = (अचिह्नित वर्ण *) addr;

	ADDR += nr >> 3;
	*ADDR &= (अचिह्नित वर्ण) ~(1 << (nr & 0x07));
}

_INLINE_ __u16 ext2fs_swab16(__u16 val)
{
	प्रत्यागम (val >> 8) | (__u16) (val << 8);
}

_INLINE_ __u32 ext2fs_swab32(__u32 val)
{
	प्रत्यागम ((val>>24) | ((val>>8)&0xFF00) |
		((val<<8)&0xFF0000) | (val<<24));
}

_INLINE_ __u64 ext2fs_swab64(__u64 val)
{
	प्रत्यागम (ext2fs_swab32((__u32) (val >> 32)) |
		(((__u64)ext2fs_swab32(val & 0xFFFFFFFFUL)) << 32));
}

_INLINE_ पूर्णाङ्क ext2fs_mark_खण्डम्_bitmap(ext2fs_खण्डम्_bitmap bitmap,
				       blk_t खण्डम्)
{
	प्रत्यागम ext2fs_mark_generic_bitmap((ext2fs_generic_bitmap) bitmap,
					  खण्डम्);
}

_INLINE_ पूर्णाङ्क ext2fs_unmark_खण्डम्_bitmap(ext2fs_खण्डम्_bitmap bitmap,
					 blk_t खण्डम्)
{
	प्रत्यागम ext2fs_unmark_generic_bitmap((ext2fs_generic_bitmap) bitmap,
					    खण्डम्);
}

_INLINE_ पूर्णाङ्क ext2fs_test_खण्डम्_bitmap(ext2fs_खण्डम्_bitmap bitmap,
				       blk_t खण्डम्)
{
	प्रत्यागम ext2fs_test_generic_bitmap((ext2fs_generic_bitmap) bitmap,
					  खण्डम्);
}

_INLINE_ पूर्णाङ्क ext2fs_mark_inode_bitmap(ext2fs_inode_bitmap bitmap,
				       ext2_ino_t inode)
{
	प्रत्यागम ext2fs_mark_generic_bitmap((ext2fs_generic_bitmap) bitmap,
					  inode);
}

_INLINE_ पूर्णाङ्क ext2fs_unmark_inode_bitmap(ext2fs_inode_bitmap bitmap,
					 ext2_ino_t inode)
{
	प्रत्यागम ext2fs_unmark_generic_bitmap((ext2fs_generic_bitmap) bitmap,
				     inode);
}

_INLINE_ पूर्णाङ्क ext2fs_test_inode_bitmap(ext2fs_inode_bitmap bitmap,
				       ext2_ino_t inode)
{
	प्रत्यागम ext2fs_test_generic_bitmap((ext2fs_generic_bitmap) bitmap,
					  inode);
}

_INLINE_ शून्य ext2fs_fast_mark_खण्डम्_bitmap(ext2fs_खण्डम्_bitmap bitmap,
					    blk_t खण्डम्)
{
	ext2fs_mark_generic_bitmap((ext2fs_generic_bitmap) bitmap, खण्डम्);
}

_INLINE_ शून्य ext2fs_fast_unmark_खण्डम्_bitmap(ext2fs_खण्डम्_bitmap bitmap,
					      blk_t खण्डम्)
{
	ext2fs_unmark_generic_bitmap((ext2fs_generic_bitmap) bitmap, खण्डम्);
}

_INLINE_ पूर्णाङ्क ext2fs_fast_test_खण्डम्_bitmap(ext2fs_खण्डम्_bitmap bitmap,
					    blk_t खण्डम्)
{
	प्रत्यागम ext2fs_test_generic_bitmap((ext2fs_generic_bitmap) bitmap,
					  खण्डम्);
}

_INLINE_ शून्य ext2fs_fast_mark_inode_bitmap(ext2fs_inode_bitmap bitmap,
					    ext2_ino_t inode)
{
	ext2fs_mark_generic_bitmap((ext2fs_generic_bitmap) bitmap, inode);
}

_INLINE_ शून्य ext2fs_fast_unmark_inode_bitmap(ext2fs_inode_bitmap bitmap,
					      ext2_ino_t inode)
{
	ext2fs_unmark_generic_bitmap((ext2fs_generic_bitmap) bitmap, inode);
}

_INLINE_ पूर्णाङ्क ext2fs_fast_test_inode_bitmap(ext2fs_inode_bitmap bitmap,
					   ext2_ino_t inode)
{
	प्रत्यागम ext2fs_test_generic_bitmap((ext2fs_generic_bitmap) bitmap,
					  inode);
}

_INLINE_ blk_t ext2fs_get_खण्डम्_bitmap_start(ext2fs_खण्डम्_bitmap bitmap)
{
	प्रत्यागम ext2fs_get_generic_bitmap_start((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ ext2_ino_t ext2fs_get_inode_bitmap_start(ext2fs_inode_bitmap bitmap)
{
	प्रत्यागम ext2fs_get_generic_bitmap_start((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ blk_t ext2fs_get_खण्डम्_bitmap_अन्तः(ext2fs_खण्डम्_bitmap bitmap)
{
	प्रत्यागम ext2fs_get_generic_bitmap_अन्तः((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ ext2_ino_t ext2fs_get_inode_bitmap_अन्तः(ext2fs_inode_bitmap bitmap)
{
	प्रत्यागम ext2fs_get_generic_bitmap_अन्तः((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ पूर्णाङ्क ext2fs_fast_test_खण्डम्_bitmap_range(ext2fs_खण्डम्_bitmap bitmap,
						 blk_t खण्डम्, पूर्णाङ्क अङ्कः)
{
	प्रत्यागम ext2fs_test_खण्डम्_bitmap_range(bitmap, खण्डम्, अङ्कः);
}

_INLINE_ शून्य ext2fs_fast_mark_खण्डम्_bitmap_range(ext2fs_खण्डम्_bitmap bitmap,
						  blk_t खण्डम्, पूर्णाङ्क अङ्कः)
{
	ext2fs_mark_खण्डम्_bitmap_range(bitmap, खण्डम्, अङ्कः);
}

_INLINE_ शून्य ext2fs_fast_unmark_खण्डम्_bitmap_range(ext2fs_खण्डम्_bitmap bitmap,
						    blk_t खण्डम्, पूर्णाङ्क अङ्कः)
{
	ext2fs_unmark_खण्डम्_bitmap_range(bitmap, खण्डम्, अङ्कः);
}

/* 64-bit versions */

_INLINE_ पूर्णाङ्क ext2fs_mark_खण्डम्_bitmap2(ext2fs_खण्डम्_bitmap bitmap,
				       blk64_t खण्डम्)
{
	प्रत्यागम ext2fs_mark_generic_bmap((ext2fs_generic_bitmap) bitmap,
					खण्डम्);
}

_INLINE_ पूर्णाङ्क ext2fs_unmark_खण्डम्_bitmap2(ext2fs_खण्डम्_bitmap bitmap,
					 blk64_t खण्डम्)
{
	प्रत्यागम ext2fs_unmark_generic_bmap((ext2fs_generic_bitmap) bitmap, खण्डम्);
}

_INLINE_ पूर्णाङ्क ext2fs_test_खण्डम्_bitmap2(ext2fs_खण्डम्_bitmap bitmap,
				       blk64_t खण्डम्)
{
	प्रत्यागम ext2fs_test_generic_bmap((ext2fs_generic_bitmap) bitmap,
					खण्डम्);
}

_INLINE_ पूर्णाङ्क ext2fs_mark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
				       ext2_ino_t inode)
{
	प्रत्यागम ext2fs_mark_generic_bmap((ext2fs_generic_bitmap) bitmap,
					inode);
}

_INLINE_ पूर्णाङ्क ext2fs_unmark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
					 ext2_ino_t inode)
{
	प्रत्यागम ext2fs_unmark_generic_bmap((ext2fs_generic_bitmap) bitmap,
					  inode);
}

_INLINE_ पूर्णाङ्क ext2fs_test_inode_bitmap2(ext2fs_inode_bitmap bitmap,
				       ext2_ino_t inode)
{
	प्रत्यागम ext2fs_test_generic_bmap((ext2fs_generic_bitmap) bitmap,
					inode);
}

_INLINE_ शून्य ext2fs_fast_mark_खण्डम्_bitmap2(ext2fs_खण्डम्_bitmap bitmap,
					     blk64_t खण्डम्)
{
	ext2fs_mark_generic_bmap((ext2fs_generic_bitmap) bitmap, खण्डम्);
}

_INLINE_ शून्य ext2fs_fast_unmark_खण्डम्_bitmap2(ext2fs_खण्डम्_bitmap bitmap,
					       blk64_t खण्डम्)
{
	ext2fs_unmark_generic_bmap((ext2fs_generic_bitmap) bitmap, खण्डम्);
}

_INLINE_ पूर्णाङ्क ext2fs_fast_test_खण्डम्_bitmap2(ext2fs_खण्डम्_bitmap bitmap,
					    blk64_t खण्डम्)
{
	प्रत्यागम ext2fs_test_generic_bmap((ext2fs_generic_bitmap) bitmap,
					खण्डम्);
}

_INLINE_ शून्य ext2fs_fast_mark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
					     ext2_ino_t inode)
{
	ext2fs_mark_generic_bmap((ext2fs_generic_bitmap) bitmap, inode);
}

_INLINE_ शून्य ext2fs_fast_unmark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
					       ext2_ino_t inode)
{
	ext2fs_unmark_generic_bmap((ext2fs_generic_bitmap) bitmap, inode);
}

_INLINE_ पूर्णाङ्क ext2fs_fast_test_inode_bitmap2(ext2fs_inode_bitmap bitmap,
					    ext2_ino_t inode)
{
	प्रत्यागम ext2fs_test_generic_bmap((ext2fs_generic_bitmap) bitmap,
					inode);
}

_INLINE_ errcode_t ext2fs_find_first_zero_खण्डम्_bitmap2(ext2fs_खण्डम्_bitmap bitmap,
							blk64_t start,
							blk64_t अन्तः,
							blk64_t *बहिर्गमनम्)
{
	__u64 o;
	errcode_t rv;

	rv = ext2fs_find_first_zero_generic_bmap((ext2fs_generic_bitmap) bitmap,
						 start, अन्तः, &o);
	यदि (!rv)
		*बहिर्गमनम् = o;
	प्रत्यागम rv;
}

_INLINE_ errcode_t ext2fs_find_first_zero_inode_bitmap2(ext2fs_inode_bitmap bitmap,
							ext2_ino_t start,
							ext2_ino_t अन्तः,
							ext2_ino_t *बहिर्गमनम्)
{
	__u64 o;
	errcode_t rv;

	rv = ext2fs_find_first_zero_generic_bmap((ext2fs_generic_bitmap) bitmap,
						 start, अन्तः, &o);
	यदि (!rv)
		*बहिर्गमनम् = (ext2_ino_t) o;
	प्रत्यागम rv;
}

_INLINE_ errcode_t ext2fs_find_first_समुच्चयः_खण्डम्_bitmap2(ext2fs_खण्डम्_bitmap bitmap,
						       blk64_t start,
						       blk64_t अन्तः,
						       blk64_t *बहिर्गमनम्)
{
	__u64 o;
	errcode_t rv;

	rv = ext2fs_find_first_समुच्चयः_generic_bmap((ext2fs_generic_bitmap) bitmap,
						start, अन्तः, &o);
	यदि (!rv)
		*बहिर्गमनम् = o;
	प्रत्यागम rv;
}

_INLINE_ errcode_t ext2fs_find_first_समुच्चयः_inode_bitmap2(ext2fs_inode_bitmap bitmap,
						       ext2_ino_t start,
						       ext2_ino_t अन्तः,
						       ext2_ino_t *बहिर्गमनम्)
{
	__u64 o;
	errcode_t rv;

	rv = ext2fs_find_first_समुच्चयः_generic_bmap((ext2fs_generic_bitmap) bitmap,
						start, अन्तः, &o);
	यदि (!rv)
		*बहिर्गमनम् = (ext2_ino_t) o;
	प्रत्यागम rv;
}

_INLINE_ blk64_t ext2fs_get_खण्डम्_bitmap_start2(ext2fs_खण्डम्_bitmap bitmap)
{
	प्रत्यागम ext2fs_get_generic_bmap_start((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ ext2_ino_t ext2fs_get_inode_bitmap_start2(ext2fs_inode_bitmap bitmap)
{
	प्रत्यागम (ext2_ino_t) ext2fs_get_generic_bmap_start((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ blk64_t ext2fs_get_खण्डम्_bitmap_end2(ext2fs_खण्डम्_bitmap bitmap)
{
	प्रत्यागम ext2fs_get_generic_bmap_अन्तः((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ ext2_ino_t ext2fs_get_inode_bitmap_end2(ext2fs_inode_bitmap bitmap)
{
	प्रत्यागम (ext2_ino_t) ext2fs_get_generic_bmap_अन्तः((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ पूर्णाङ्क ext2fs_fast_test_खण्डम्_bitmap_range2(ext2fs_खण्डम्_bitmap bitmap,
						  blk64_t खण्डम्,
						  अचिह्नित पूर्णाङ्क अङ्कः)
{
	प्रत्यागम ext2fs_test_खण्डम्_bitmap_range2(bitmap, खण्डम्, अङ्कः);
}

_INLINE_ शून्य ext2fs_fast_mark_खण्डम्_bitmap_range2(ext2fs_खण्डम्_bitmap bitmap,
						   blk64_t खण्डम्,
						   अचिह्नित पूर्णाङ्क अङ्कः)
{
	ext2fs_mark_खण्डम्_bitmap_range2(bitmap, खण्डम्, अङ्कः);
}

_INLINE_ शून्य ext2fs_fast_unmark_खण्डम्_bitmap_range2(ext2fs_खण्डम्_bitmap bitmap,
						     blk64_t खण्डम्,
						     अचिह्नित पूर्णाङ्क अङ्कः)
{
	ext2fs_unmark_खण्डम्_bitmap_range2(bitmap, खण्डम्, अङ्कः);
}

#अपरिभाषय _INLINE_
#यदि_अन्तः

बाह्य पूर्णाङ्क ext2fs_समुच्चयः_bit(अचिह्नित पूर्णाङ्क nr,शून्य * addr);
बाह्य पूर्णाङ्क ext2fs_clear_bit(अचिह्नित पूर्णाङ्क nr, शून्य * addr);
बाह्य पूर्णाङ्क ext2fs_test_bit(अचिह्नित पूर्णाङ्क nr, ध्रुव शून्य * addr);
बाह्य पूर्णाङ्क ext2fs_समुच्चयः_bit64(__u64 nr,शून्य * addr);
बाह्य पूर्णाङ्क ext2fs_clear_bit64(__u64 nr, शून्य * addr);
बाह्य पूर्णाङ्क ext2fs_test_bit64(__u64 nr, ध्रुव शून्य * addr);
बाह्य अचिह्नित पूर्णाङ्क ext2fs_bitcount(ध्रुव शून्य *addr, अचिह्नित पूर्णाङ्क nbytes);
