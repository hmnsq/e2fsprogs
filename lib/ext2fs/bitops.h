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

#నిర్వచితమైతే పదాలు_BIGENDIAN
#నిర్వచించు ext2fs_cpu_గమ్యం_le64(x) ((__force __le64)ext2fs_swab64((__u64)(x)))
#నిర్వచించు ext2fs_le64_గమ్యం_cpu(x) ext2fs_swab64((__force __u64)(__le64)(x))
#నిర్వచించు ext2fs_cpu_గమ్యం_le32(x) ((__force __le32)ext2fs_swab32((__u32)(x)))
#నిర్వచించు ext2fs_le32_గమ్యం_cpu(x) ext2fs_swab32((__force __u32)(__le32)(x))
#నిర్వచించు ext2fs_cpu_గమ్యం_le16(x) ((__force __le16)ext2fs_swab16((__u16)(x)))
#నిర్వచించు ext2fs_le16_గమ్యం_cpu(x) ext2fs_swab16((__force __u16)(__le16)(x))

#నిర్వచించు ext2fs_cpu_గమ్యం_be64(x) ((__force __be64)(__u64)(x))
#నిర్వచించు ext2fs_be64_గమ్యం_cpu(x) ((__force __u64)(__be64)(x))
#నిర్వచించు ext2fs_cpu_గమ్యం_be32(x) ((__force __be32)(__u32)(x))
#నిర్వచించు ext2fs_be32_గమ్యం_cpu(x) ((__force __u32)(__be32)(x))
#నిర్వచించు ext2fs_cpu_గమ్యం_be16(x) ((__force __be16)(__u16)(x))
#నిర్వచించు ext2fs_be16_గమ్యం_cpu(x) ((__force __u16)(__be16)(x))
#లేకపోతే
#నిర్వచించు ext2fs_cpu_గమ్యం_le64(x) ((__force __le64)(__u64)(x))
#నిర్వచించు ext2fs_le64_గమ్యం_cpu(x) ((__force __u64)(__le64)(x))
#నిర్వచించు ext2fs_cpu_గమ్యం_le32(x) ((__force __le32)(__u32)(x))
#నిర్వచించు ext2fs_le32_గమ్యం_cpu(x) ((__force __u32)(__le32)(x))
#నిర్వచించు ext2fs_cpu_గమ్యం_le16(x) ((__force __le16)(__u16)(x))
#నిర్వచించు ext2fs_le16_గమ్యం_cpu(x) ((__force __u16)(__le16)(x))

#నిర్వచించు ext2fs_cpu_గమ్యం_be64(x) ((__force __be64)ext2fs_swab64((__u64)(x)))
#నిర్వచించు ext2fs_be64_గమ్యం_cpu(x) ext2fs_swab64((__force __u64)(__be64)(x))
#నిర్వచించు ext2fs_cpu_గమ్యం_be32(x) ((__force __be32)ext2fs_swab32((__u32)(x)))
#నిర్వచించు ext2fs_be32_గమ్యం_cpu(x) ext2fs_swab32((__force __u32)(__be32)(x))
#నిర్వచించు ext2fs_cpu_గమ్యం_be16(x) ((__force __be16)ext2fs_swab16((__u16)(x)))
#నిర్వచించు ext2fs_be16_గమ్యం_cpu(x) ext2fs_swab16((__force __u16)(__be16)(x))
#అయితే_ముగింపు

/*
 * EXT2FS bitmap manipulation routines.
 */

/* Support for sending warning messages from the inline subroutines */
బాహ్య స్థిరరాశి అక్షరం *ext2fs_ఖండం_string;
బాహ్య స్థిరరాశి అక్షరం *ext2fs_inode_string;
బాహ్య స్థిరరాశి అక్షరం *ext2fs_mark_string;
బాహ్య స్థిరరాశి అక్షరం *ext2fs_unmark_string;
బాహ్య స్థిరరాశి అక్షరం *ext2fs_test_string;
బాహ్య శూన్య ext2fs_warn_bitmap(errcode_t errcode, గుర్తులేని దీర్ఘ arg,
			       స్థిరరాశి అక్షరం *description);
బాహ్య శూన్య ext2fs_warn_bitmap2(ext2fs_generic_bitmap bitmap,
				పూర్ణాంకం సంకేతం_2, గుర్తులేని దీర్ఘ arg);

#నిర్వచితమైతే NO_INLINE_FUNCS
బాహ్య శూన్య ext2fs_fast_సమితి_బిట్టు(గుర్తులేని పూర్ణాంకం nr,శూన్య * addr);
బాహ్య శూన్య ext2fs_fast_clear_బిట్టు(గుర్తులేని పూర్ణాంకం nr, శూన్య * addr);
బాహ్య శూన్య ext2fs_fast_సమితి_bit64(__u64 nr,శూన్య * addr);
బాహ్య శూన్య ext2fs_fast_clear_bit64(__u64 nr, శూన్య * addr);
బాహ్య __u16 ext2fs_swab16(__u16 val);
బాహ్య __u32 ext2fs_swab32(__u32 val);
బాహ్య __u64 ext2fs_swab64(__u64 val);

బాహ్య పూర్ణాంకం ext2fs_mark_ఖండం_bitmap(ext2fs_ఖండం_bitmap bitmap, blk_t ఖండం);
బాహ్య పూర్ణాంకం ext2fs_unmark_ఖండం_bitmap(ext2fs_ఖండం_bitmap bitmap,
				       blk_t ఖండం);
బాహ్య పూర్ణాంకం ext2fs_test_ఖండం_bitmap(ext2fs_ఖండం_bitmap bitmap, blk_t ఖండం);

బాహ్య పూర్ణాంకం ext2fs_mark_inode_bitmap(ext2fs_inode_bitmap bitmap, ext2_ino_t inode);
బాహ్య పూర్ణాంకం ext2fs_unmark_inode_bitmap(ext2fs_inode_bitmap bitmap,
				       ext2_ino_t inode);
బాహ్య పూర్ణాంకం ext2fs_test_inode_bitmap(ext2fs_inode_bitmap bitmap, ext2_ino_t inode);

బాహ్య శూన్య ext2fs_fast_mark_ఖండం_bitmap(ext2fs_ఖండం_bitmap bitmap,
					  blk_t ఖండం);
బాహ్య శూన్య ext2fs_fast_unmark_ఖండం_bitmap(ext2fs_ఖండం_bitmap bitmap,
					    blk_t ఖండం);
బాహ్య పూర్ణాంకం ext2fs_fast_test_ఖండం_bitmap(ext2fs_ఖండం_bitmap bitmap,
					 blk_t ఖండం);

బాహ్య శూన్య ext2fs_fast_mark_inode_bitmap(ext2fs_inode_bitmap bitmap,
					  ext2_ino_t inode);
బాహ్య శూన్య ext2fs_fast_unmark_inode_bitmap(ext2fs_inode_bitmap bitmap,
					    ext2_ino_t inode);
బాహ్య పూర్ణాంకం ext2fs_fast_test_inode_bitmap(ext2fs_inode_bitmap bitmap,
					 ext2_ino_t inode);
బాహ్య blk_t ext2fs_get_ఖండం_bitmap_ప్రారంభం(ext2fs_ఖండం_bitmap bitmap);
బాహ్య ext2_ino_t ext2fs_get_inode_bitmap_ప్రారంభం(ext2fs_inode_bitmap bitmap);
బాహ్య blk_t ext2fs_get_ఖండం_bitmap_చివర(ext2fs_ఖండం_bitmap bitmap);
బాహ్య ext2_ino_t ext2fs_get_inode_bitmap_చివర(ext2fs_inode_bitmap bitmap);

బాహ్య శూన్య ext2fs_fast_mark_ఖండం_bitmap_range(ext2fs_ఖండం_bitmap bitmap,
						blk_t ఖండం, పూర్ణాంకం అంకె);
బాహ్య శూన్య ext2fs_fast_unmark_ఖండం_bitmap_range(ext2fs_ఖండం_bitmap bitmap,
						  blk_t ఖండం, పూర్ణాంకం అంకె);
బాహ్య పూర్ణాంకం ext2fs_fast_test_ఖండం_bitmap_range(ext2fs_ఖండం_bitmap bitmap,
					       blk_t ఖండం, పూర్ణాంకం అంకె);
#అయితే_ముగింపు

/* These functions routines moved to gen_bitmap.c */
బాహ్య శూన్య ext2fs_mark_ఖండం_bitmap_range(ext2fs_ఖండం_bitmap bitmap,
					   blk_t ఖండం, పూర్ణాంకం అంకె);
బాహ్య శూన్య ext2fs_unmark_ఖండం_bitmap_range(ext2fs_ఖండం_bitmap bitmap,
					     blk_t ఖండం, పూర్ణాంకం అంకె);
బాహ్య పూర్ణాంకం ext2fs_test_ఖండం_bitmap_range(ext2fs_ఖండం_bitmap bitmap,
					  blk_t ఖండం, పూర్ణాంకం అంకె);
బాహ్య పూర్ణాంకం ext2fs_test_inode_bitmap_range(ext2fs_inode_bitmap bitmap,
					  ext2_ino_t inode, పూర్ణాంకం అంకె);
బాహ్య పూర్ణాంకం ext2fs_mark_generic_bitmap(ext2fs_generic_bitmap bitmap,
					 __u32 bitno);
బాహ్య పూర్ణాంకం ext2fs_unmark_generic_bitmap(ext2fs_generic_bitmap bitmap,
					   blk_t bitno);
బాహ్య పూర్ణాంకం ext2fs_test_generic_bitmap(ext2fs_generic_bitmap bitmap,
				      blk_t bitno);
బాహ్య పూర్ణాంకం ext2fs_test_ఖండం_bitmap_range(ext2fs_ఖండం_bitmap bitmap,
					  blk_t ఖండం, పూర్ణాంకం అంకె);
బాహ్య శూన్య ext2fs_సమితి_bitmap_padding(ext2fs_generic_bitmap map);
బాహ్య __u32 ext2fs_get_generic_bitmap_ప్రారంభం(ext2fs_generic_bitmap bitmap);
బాహ్య __u32 ext2fs_get_generic_bitmap_చివర(ext2fs_generic_bitmap bitmap);

/* 64-bit versions */

#నిర్వచితమైతే NO_INLINE_FUNCS
బాహ్య పూర్ణాంకం ext2fs_mark_ఖండం_bitmap2(ext2fs_ఖండం_bitmap bitmap,
				     blk64_t ఖండం);
బాహ్య పూర్ణాంకం ext2fs_unmark_ఖండం_bitmap2(ext2fs_ఖండం_bitmap bitmap,
				       blk64_t ఖండం);
బాహ్య పూర్ణాంకం ext2fs_test_ఖండం_bitmap2(ext2fs_ఖండం_bitmap bitmap,
				     blk64_t ఖండం);

బాహ్య పూర్ణాంకం ext2fs_mark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
				     ext2_ino_t inode);
బాహ్య పూర్ణాంకం ext2fs_unmark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
				       ext2_ino_t inode);
బాహ్య పూర్ణాంకం ext2fs_test_inode_bitmap2(ext2fs_inode_bitmap bitmap,
				     ext2_ino_t inode);

బాహ్య శూన్య ext2fs_fast_mark_ఖండం_bitmap2(ext2fs_ఖండం_bitmap bitmap,
					   blk64_t ఖండం);
బాహ్య శూన్య ext2fs_fast_unmark_ఖండం_bitmap2(ext2fs_ఖండం_bitmap bitmap,
					     blk64_t ఖండం);
బాహ్య పూర్ణాంకం ext2fs_fast_test_ఖండం_bitmap2(ext2fs_ఖండం_bitmap bitmap,
					  blk64_t ఖండం);

బాహ్య శూన్య ext2fs_fast_mark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
					   ext2_ino_t inode);
బాహ్య శూన్య ext2fs_fast_unmark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
					    ext2_ino_t inode);
బాహ్య పూర్ణాంకం ext2fs_fast_test_inode_bitmap2(ext2fs_inode_bitmap bitmap,
					  ext2_ino_t inode);
బాహ్య errcode_t ext2fs_కనుగొను_మొదటి_zero_ఖండం_bitmap2(ext2fs_ఖండం_bitmap bitmap,
						      blk64_t ప్రారంభం,
						      blk64_t చివర,
						      blk64_t *బహిర్గతం);
బాహ్య errcode_t ext2fs_కనుగొను_మొదటి_zero_inode_bitmap2(ext2fs_inode_bitmap bitmap,
						      ext2_ino_t ప్రారంభం,
						      ext2_ino_t చివర,
						      ext2_ino_t *బహిర్గతం);
బాహ్య errcode_t ext2fs_కనుగొను_మొదటి_సమితి_ఖండం_bitmap2(ext2fs_ఖండం_bitmap bitmap,
						     blk64_t ప్రారంభం,
						     blk64_t చివర,
						     blk64_t *బహిర్గతం);
బాహ్య errcode_t ext2fs_కనుగొను_మొదటి_సమితి_inode_bitmap2(ext2fs_inode_bitmap bitmap,
						      ext2_ino_t ప్రారంభం,
						      ext2_ino_t చివర,
						      ext2_ino_t *బహిర్గతం);
బాహ్య blk64_t ext2fs_get_ఖండం_bitmap_start2(ext2fs_ఖండం_bitmap bitmap);
బాహ్య ext2_ino_t ext2fs_get_inode_bitmap_start2(ext2fs_inode_bitmap bitmap);
బాహ్య blk64_t ext2fs_get_ఖండం_bitmap_end2(ext2fs_ఖండం_bitmap bitmap);
బాహ్య ext2_ino_t ext2fs_get_inode_bitmap_end2(ext2fs_inode_bitmap bitmap);

బాహ్య పూర్ణాంకం ext2fs_fast_test_ఖండం_bitmap_range2(ext2fs_ఖండం_bitmap bitmap,
						blk64_t ఖండం,
						గుర్తులేని పూర్ణాంకం అంకె);
బాహ్య శూన్య ext2fs_fast_mark_ఖండం_bitmap_range2(ext2fs_ఖండం_bitmap bitmap,
						 blk64_t ఖండం,
						 గుర్తులేని పూర్ణాంకం అంకె);
బాహ్య శూన్య ext2fs_fast_unmark_ఖండం_bitmap_range2(ext2fs_ఖండం_bitmap bitmap,
						   blk64_t ఖండం,
						   గుర్తులేని పూర్ణాంకం అంకె);
#అయితే_ముగింపు

/* These routines moved to gen_bitmap64.c */
బాహ్య శూన్య ext2fs_clear_generic_bmap(ext2fs_generic_bitmap bitmap);
బాహ్య errcode_t ext2fs_compare_generic_bmap(errcode_t neq,
					     ext2fs_generic_bitmap bm1,
					     ext2fs_generic_bitmap bm2);
బాహ్య శూన్య ext2fs_సమితి_generic_bmap_padding(ext2fs_generic_bitmap bmap);
బాహ్య పూర్ణాంకం ext2fs_mark_generic_bmap(ext2fs_generic_bitmap bitmap,
				    blk64_t bitno);
బాహ్య పూర్ణాంకం ext2fs_unmark_generic_bmap(ext2fs_generic_bitmap bitmap,
				      blk64_t bitno);
బాహ్య పూర్ణాంకం ext2fs_test_generic_bmap(ext2fs_generic_bitmap bitmap,
				    blk64_t bitno);
బాహ్య పూర్ణాంకం ext2fs_test_ఖండం_bitmap_range2(ext2fs_ఖండం_bitmap bitmap,
					   blk64_t ఖండం, గుర్తులేని పూర్ణాంకం అంకె);
బాహ్య __u64 ext2fs_get_generic_bmap_ప్రారంభం(ext2fs_generic_bitmap bitmap);
బాహ్య __u64 ext2fs_get_generic_bmap_చివర(ext2fs_generic_bitmap bitmap);
బాహ్య పూర్ణాంకం ext2fs_test_ఖండం_bitmap_range2(ext2fs_ఖండం_bitmap bitmap,
					   blk64_t ఖండం, గుర్తులేని పూర్ణాంకం అంకె);
బాహ్య శూన్య ext2fs_mark_ఖండం_bitmap_range2(ext2fs_ఖండం_bitmap bitmap,
					    blk64_t ఖండం, గుర్తులేని పూర్ణాంకం అంకె);
బాహ్య శూన్య ext2fs_unmark_ఖండం_bitmap_range2(ext2fs_ఖండం_bitmap bitmap,
					      blk64_t ఖండం, గుర్తులేని పూర్ణాంకం అంకె);
బాహ్య errcode_t ext2fs_కనుగొను_మొదటి_zero_generic_bmap(ext2fs_generic_bitmap bitmap,
						     __u64 ప్రారంభం, __u64 చివర,
						     __u64 *బహిర్గతం);
బాహ్య errcode_t ext2fs_కనుగొను_మొదటి_సమితి_generic_bmap(ext2fs_generic_bitmap bitmap,
						    __u64 ప్రారంభం, __u64 చివర,
						    __u64 *బహిర్గతం);

/*
 * The inline routines themselves...
 *
 * If NO_INLINE_FUNCS is defined, then we won't try to do inline
 * functions at all; they will be included as normal functions in
 * inline.c
 */

#ఒకవేళ (defined(INCLUDE_INLINE_FUNCS) || !defined(NO_INLINE_FUNCS))
#నిర్వచితమైతే INCLUDE_INLINE_FUNCS
#ఒకవేళ (__STDC_VERSION__ >= 199901L)
#నిర్వచించు _INLINE_ బాహ్య అంతఃవరుస
#లేకపోతే
#నిర్వచించు _INLINE_ అంతఃవరుస
#అయితే_ముగింపు
#లేకపోతే /* !INCLUDE_INLINE FUNCS */
#ఒకవేళ (__STDC_VERSION__ >= 199901L)
#నిర్వచించు _INLINE_ అంతఃవరుస
#లేకపోతే /* not C99 */
#నిర్వచితమైతే __GNUC__
#నిర్వచించు _INLINE_ బాహ్య __inline__
#లేకపోతే				/* For Watcom C */
#నిర్వచించు _INLINE_ బాహ్య అంతఃవరుస
#అయితే_ముగింపు /* __GNUC__ */
#అయితే_ముగింపు /* __STDC_VERSION__ >= 199901L */
#అయితే_ముగింపు /* INCLUDE_INLINE_FUNCS */

/*
 * Fast bit set/clear functions that doesn't need to return the
 * previous bit value.
 */

_INLINE_ శూన్య ext2fs_fast_సమితి_బిట్టు(గుర్తులేని పూర్ణాంకం nr,శూన్య * addr)
{
	గుర్తులేని అక్షరం	*ADDR = (గుర్తులేని అక్షరం *) addr;

	ADDR += nr >> 3;
	*ADDR |= (గుర్తులేని అక్షరం) (1 << (nr & 0x07));
}

_INLINE_ శూన్య ext2fs_fast_clear_బిట్టు(గుర్తులేని పూర్ణాంకం nr, శూన్య * addr)
{
	గుర్తులేని అక్షరం	*ADDR = (గుర్తులేని అక్షరం *) addr;

	ADDR += nr >> 3;
	*ADDR &= (గుర్తులేని అక్షరం) ~(1 << (nr & 0x07));
}


_INLINE_ శూన్య ext2fs_fast_సమితి_bit64(__u64 nr, శూన్య * addr)
{
	గుర్తులేని అక్షరం	*ADDR = (గుర్తులేని అక్షరం *) addr;

	ADDR += nr >> 3;
	*ADDR |= (గుర్తులేని అక్షరం) (1 << (nr & 0x07));
}

_INLINE_ శూన్య ext2fs_fast_clear_bit64(__u64 nr, శూన్య * addr)
{
	గుర్తులేని అక్షరం	*ADDR = (గుర్తులేని అక్షరం *) addr;

	ADDR += nr >> 3;
	*ADDR &= (గుర్తులేని అక్షరం) ~(1 << (nr & 0x07));
}

_INLINE_ __u16 ext2fs_swab16(__u16 val)
{
	తిరిగి (val >> 8) | (__u16) (val << 8);
}

_INLINE_ __u32 ext2fs_swab32(__u32 val)
{
	తిరిగి ((val>>24) | ((val>>8)&0xFF00) |
		((val<<8)&0xFF0000) | (val<<24));
}

_INLINE_ __u64 ext2fs_swab64(__u64 val)
{
	తిరిగి (ext2fs_swab32((__u32) (val >> 32)) |
		(((__u64)ext2fs_swab32(val & 0xFFFFFFFFUL)) << 32));
}

_INLINE_ పూర్ణాంకం ext2fs_mark_ఖండం_bitmap(ext2fs_ఖండం_bitmap bitmap,
				       blk_t ఖండం)
{
	తిరిగి ext2fs_mark_generic_bitmap((ext2fs_generic_bitmap) bitmap,
					  ఖండం);
}

_INLINE_ పూర్ణాంకం ext2fs_unmark_ఖండం_bitmap(ext2fs_ఖండం_bitmap bitmap,
					 blk_t ఖండం)
{
	తిరిగి ext2fs_unmark_generic_bitmap((ext2fs_generic_bitmap) bitmap,
					    ఖండం);
}

_INLINE_ పూర్ణాంకం ext2fs_test_ఖండం_bitmap(ext2fs_ఖండం_bitmap bitmap,
				       blk_t ఖండం)
{
	తిరిగి ext2fs_test_generic_bitmap((ext2fs_generic_bitmap) bitmap,
					  ఖండం);
}

_INLINE_ పూర్ణాంకం ext2fs_mark_inode_bitmap(ext2fs_inode_bitmap bitmap,
				       ext2_ino_t inode)
{
	తిరిగి ext2fs_mark_generic_bitmap((ext2fs_generic_bitmap) bitmap,
					  inode);
}

_INLINE_ పూర్ణాంకం ext2fs_unmark_inode_bitmap(ext2fs_inode_bitmap bitmap,
					 ext2_ino_t inode)
{
	తిరిగి ext2fs_unmark_generic_bitmap((ext2fs_generic_bitmap) bitmap,
				     inode);
}

_INLINE_ పూర్ణాంకం ext2fs_test_inode_bitmap(ext2fs_inode_bitmap bitmap,
				       ext2_ino_t inode)
{
	తిరిగి ext2fs_test_generic_bitmap((ext2fs_generic_bitmap) bitmap,
					  inode);
}

_INLINE_ శూన్య ext2fs_fast_mark_ఖండం_bitmap(ext2fs_ఖండం_bitmap bitmap,
					    blk_t ఖండం)
{
	ext2fs_mark_generic_bitmap((ext2fs_generic_bitmap) bitmap, ఖండం);
}

_INLINE_ శూన్య ext2fs_fast_unmark_ఖండం_bitmap(ext2fs_ఖండం_bitmap bitmap,
					      blk_t ఖండం)
{
	ext2fs_unmark_generic_bitmap((ext2fs_generic_bitmap) bitmap, ఖండం);
}

_INLINE_ పూర్ణాంకం ext2fs_fast_test_ఖండం_bitmap(ext2fs_ఖండం_bitmap bitmap,
					    blk_t ఖండం)
{
	తిరిగి ext2fs_test_generic_bitmap((ext2fs_generic_bitmap) bitmap,
					  ఖండం);
}

_INLINE_ శూన్య ext2fs_fast_mark_inode_bitmap(ext2fs_inode_bitmap bitmap,
					    ext2_ino_t inode)
{
	ext2fs_mark_generic_bitmap((ext2fs_generic_bitmap) bitmap, inode);
}

_INLINE_ శూన్య ext2fs_fast_unmark_inode_bitmap(ext2fs_inode_bitmap bitmap,
					      ext2_ino_t inode)
{
	ext2fs_unmark_generic_bitmap((ext2fs_generic_bitmap) bitmap, inode);
}

_INLINE_ పూర్ణాంకం ext2fs_fast_test_inode_bitmap(ext2fs_inode_bitmap bitmap,
					   ext2_ino_t inode)
{
	తిరిగి ext2fs_test_generic_bitmap((ext2fs_generic_bitmap) bitmap,
					  inode);
}

_INLINE_ blk_t ext2fs_get_ఖండం_bitmap_ప్రారంభం(ext2fs_ఖండం_bitmap bitmap)
{
	తిరిగి ext2fs_get_generic_bitmap_ప్రారంభం((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ ext2_ino_t ext2fs_get_inode_bitmap_ప్రారంభం(ext2fs_inode_bitmap bitmap)
{
	తిరిగి ext2fs_get_generic_bitmap_ప్రారంభం((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ blk_t ext2fs_get_ఖండం_bitmap_చివర(ext2fs_ఖండం_bitmap bitmap)
{
	తిరిగి ext2fs_get_generic_bitmap_చివర((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ ext2_ino_t ext2fs_get_inode_bitmap_చివర(ext2fs_inode_bitmap bitmap)
{
	తిరిగి ext2fs_get_generic_bitmap_చివర((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ పూర్ణాంకం ext2fs_fast_test_ఖండం_bitmap_range(ext2fs_ఖండం_bitmap bitmap,
						 blk_t ఖండం, పూర్ణాంకం అంకె)
{
	తిరిగి ext2fs_test_ఖండం_bitmap_range(bitmap, ఖండం, అంకె);
}

_INLINE_ శూన్య ext2fs_fast_mark_ఖండం_bitmap_range(ext2fs_ఖండం_bitmap bitmap,
						  blk_t ఖండం, పూర్ణాంకం అంకె)
{
	ext2fs_mark_ఖండం_bitmap_range(bitmap, ఖండం, అంకె);
}

_INLINE_ శూన్య ext2fs_fast_unmark_ఖండం_bitmap_range(ext2fs_ఖండం_bitmap bitmap,
						    blk_t ఖండం, పూర్ణాంకం అంకె)
{
	ext2fs_unmark_ఖండం_bitmap_range(bitmap, ఖండం, అంకె);
}

/* 64-bit versions */

_INLINE_ పూర్ణాంకం ext2fs_mark_ఖండం_bitmap2(ext2fs_ఖండం_bitmap bitmap,
				       blk64_t ఖండం)
{
	తిరిగి ext2fs_mark_generic_bmap((ext2fs_generic_bitmap) bitmap,
					ఖండం);
}

_INLINE_ పూర్ణాంకం ext2fs_unmark_ఖండం_bitmap2(ext2fs_ఖండం_bitmap bitmap,
					 blk64_t ఖండం)
{
	తిరిగి ext2fs_unmark_generic_bmap((ext2fs_generic_bitmap) bitmap, ఖండం);
}

_INLINE_ పూర్ణాంకం ext2fs_test_ఖండం_bitmap2(ext2fs_ఖండం_bitmap bitmap,
				       blk64_t ఖండం)
{
	తిరిగి ext2fs_test_generic_bmap((ext2fs_generic_bitmap) bitmap,
					ఖండం);
}

_INLINE_ పూర్ణాంకం ext2fs_mark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
				       ext2_ino_t inode)
{
	తిరిగి ext2fs_mark_generic_bmap((ext2fs_generic_bitmap) bitmap,
					inode);
}

_INLINE_ పూర్ణాంకం ext2fs_unmark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
					 ext2_ino_t inode)
{
	తిరిగి ext2fs_unmark_generic_bmap((ext2fs_generic_bitmap) bitmap,
					  inode);
}

_INLINE_ పూర్ణాంకం ext2fs_test_inode_bitmap2(ext2fs_inode_bitmap bitmap,
				       ext2_ino_t inode)
{
	తిరిగి ext2fs_test_generic_bmap((ext2fs_generic_bitmap) bitmap,
					inode);
}

_INLINE_ శూన్య ext2fs_fast_mark_ఖండం_bitmap2(ext2fs_ఖండం_bitmap bitmap,
					     blk64_t ఖండం)
{
	ext2fs_mark_generic_bmap((ext2fs_generic_bitmap) bitmap, ఖండం);
}

_INLINE_ శూన్య ext2fs_fast_unmark_ఖండం_bitmap2(ext2fs_ఖండం_bitmap bitmap,
					       blk64_t ఖండం)
{
	ext2fs_unmark_generic_bmap((ext2fs_generic_bitmap) bitmap, ఖండం);
}

_INLINE_ పూర్ణాంకం ext2fs_fast_test_ఖండం_bitmap2(ext2fs_ఖండం_bitmap bitmap,
					    blk64_t ఖండం)
{
	తిరిగి ext2fs_test_generic_bmap((ext2fs_generic_bitmap) bitmap,
					ఖండం);
}

_INLINE_ శూన్య ext2fs_fast_mark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
					     ext2_ino_t inode)
{
	ext2fs_mark_generic_bmap((ext2fs_generic_bitmap) bitmap, inode);
}

_INLINE_ శూన్య ext2fs_fast_unmark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
					       ext2_ino_t inode)
{
	ext2fs_unmark_generic_bmap((ext2fs_generic_bitmap) bitmap, inode);
}

_INLINE_ పూర్ణాంకం ext2fs_fast_test_inode_bitmap2(ext2fs_inode_bitmap bitmap,
					    ext2_ino_t inode)
{
	తిరిగి ext2fs_test_generic_bmap((ext2fs_generic_bitmap) bitmap,
					inode);
}

_INLINE_ errcode_t ext2fs_కనుగొను_మొదటి_zero_ఖండం_bitmap2(ext2fs_ఖండం_bitmap bitmap,
							blk64_t ప్రారంభం,
							blk64_t చివర,
							blk64_t *బహిర్గతం)
{
	__u64 o;
	errcode_t rv;

	rv = ext2fs_కనుగొను_మొదటి_zero_generic_bmap((ext2fs_generic_bitmap) bitmap,
						 ప్రారంభం, చివర, &o);
	ఒకవేళ (!rv)
		*బహిర్గతం = o;
	తిరిగి rv;
}

_INLINE_ errcode_t ext2fs_కనుగొను_మొదటి_zero_inode_bitmap2(ext2fs_inode_bitmap bitmap,
							ext2_ino_t ప్రారంభం,
							ext2_ino_t చివర,
							ext2_ino_t *బహిర్గతం)
{
	__u64 o;
	errcode_t rv;

	rv = ext2fs_కనుగొను_మొదటి_zero_generic_bmap((ext2fs_generic_bitmap) bitmap,
						 ప్రారంభం, చివర, &o);
	ఒకవేళ (!rv)
		*బహిర్గతం = (ext2_ino_t) o;
	తిరిగి rv;
}

_INLINE_ errcode_t ext2fs_కనుగొను_మొదటి_సమితి_ఖండం_bitmap2(ext2fs_ఖండం_bitmap bitmap,
						       blk64_t ప్రారంభం,
						       blk64_t చివర,
						       blk64_t *బహిర్గతం)
{
	__u64 o;
	errcode_t rv;

	rv = ext2fs_కనుగొను_మొదటి_సమితి_generic_bmap((ext2fs_generic_bitmap) bitmap,
						ప్రారంభం, చివర, &o);
	ఒకవేళ (!rv)
		*బహిర్గతం = o;
	తిరిగి rv;
}

_INLINE_ errcode_t ext2fs_కనుగొను_మొదటి_సమితి_inode_bitmap2(ext2fs_inode_bitmap bitmap,
						       ext2_ino_t ప్రారంభం,
						       ext2_ino_t చివర,
						       ext2_ino_t *బహిర్గతం)
{
	__u64 o;
	errcode_t rv;

	rv = ext2fs_కనుగొను_మొదటి_సమితి_generic_bmap((ext2fs_generic_bitmap) bitmap,
						ప్రారంభం, చివర, &o);
	ఒకవేళ (!rv)
		*బహిర్గతం = (ext2_ino_t) o;
	తిరిగి rv;
}

_INLINE_ blk64_t ext2fs_get_ఖండం_bitmap_start2(ext2fs_ఖండం_bitmap bitmap)
{
	తిరిగి ext2fs_get_generic_bmap_ప్రారంభం((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ ext2_ino_t ext2fs_get_inode_bitmap_start2(ext2fs_inode_bitmap bitmap)
{
	తిరిగి (ext2_ino_t) ext2fs_get_generic_bmap_ప్రారంభం((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ blk64_t ext2fs_get_ఖండం_bitmap_end2(ext2fs_ఖండం_bitmap bitmap)
{
	తిరిగి ext2fs_get_generic_bmap_చివర((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ ext2_ino_t ext2fs_get_inode_bitmap_end2(ext2fs_inode_bitmap bitmap)
{
	తిరిగి (ext2_ino_t) ext2fs_get_generic_bmap_చివర((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ పూర్ణాంకం ext2fs_fast_test_ఖండం_bitmap_range2(ext2fs_ఖండం_bitmap bitmap,
						  blk64_t ఖండం,
						  గుర్తులేని పూర్ణాంకం అంకె)
{
	తిరిగి ext2fs_test_ఖండం_bitmap_range2(bitmap, ఖండం, అంకె);
}

_INLINE_ శూన్య ext2fs_fast_mark_ఖండం_bitmap_range2(ext2fs_ఖండం_bitmap bitmap,
						   blk64_t ఖండం,
						   గుర్తులేని పూర్ణాంకం అంకె)
{
	ext2fs_mark_ఖండం_bitmap_range2(bitmap, ఖండం, అంకె);
}

_INLINE_ శూన్య ext2fs_fast_unmark_ఖండం_bitmap_range2(ext2fs_ఖండం_bitmap bitmap,
						     blk64_t ఖండం,
						     గుర్తులేని పూర్ణాంకం అంకె)
{
	ext2fs_unmark_ఖండం_bitmap_range2(bitmap, ఖండం, అంకె);
}

#నిర్వచనం_రద్దు _INLINE_
#అయితే_ముగింపు

బాహ్య పూర్ణాంకం ext2fs_సమితి_బిట్టు(గుర్తులేని పూర్ణాంకం nr,శూన్య * addr);
బాహ్య పూర్ణాంకం ext2fs_clear_బిట్టు(గుర్తులేని పూర్ణాంకం nr, శూన్య * addr);
బాహ్య పూర్ణాంకం ext2fs_test_బిట్టు(గుర్తులేని పూర్ణాంకం nr, స్థిరరాశి శూన్య * addr);
బాహ్య పూర్ణాంకం ext2fs_సమితి_bit64(__u64 nr,శూన్య * addr);
బాహ్య పూర్ణాంకం ext2fs_clear_bit64(__u64 nr, శూన్య * addr);
బాహ్య పూర్ణాంకం ext2fs_test_bit64(__u64 nr, స్థిరరాశి శూన్య * addr);
బాహ్య గుర్తులేని పూర్ణాంకం ext2fs_bitcount(స్థిరరాశి శూన్య *addr, గుర్తులేని పూర్ణాంకం nbytes);
