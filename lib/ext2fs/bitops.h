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

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ಪದಗಳು_BIGENDIAN
#ವ್ಯಾಖ್ಯಾನಿಸು ext2fs_cpu_ಗಮ್ಯ_le64(x) ((__force __le64)ext2fs_swab64((__u64)(x)))
#ವ್ಯಾಖ್ಯಾನಿಸು ext2fs_le64_ಗಮ್ಯ_cpu(x) ext2fs_swab64((__force __u64)(__le64)(x))
#ವ್ಯಾಖ್ಯಾನಿಸು ext2fs_cpu_ಗಮ್ಯ_le32(x) ((__force __le32)ext2fs_swab32((__u32)(x)))
#ವ್ಯಾಖ್ಯಾನಿಸು ext2fs_le32_ಗಮ್ಯ_cpu(x) ext2fs_swab32((__force __u32)(__le32)(x))
#ವ್ಯಾಖ್ಯಾನಿಸು ext2fs_cpu_ಗಮ್ಯ_le16(x) ((__force __le16)ext2fs_swab16((__u16)(x)))
#ವ್ಯಾಖ್ಯಾನಿಸು ext2fs_le16_ಗಮ್ಯ_cpu(x) ext2fs_swab16((__force __u16)(__le16)(x))

#ವ್ಯಾಖ್ಯಾನಿಸು ext2fs_cpu_ಗಮ್ಯ_be64(x) ((__force __be64)(__u64)(x))
#ವ್ಯಾಖ್ಯಾನಿಸು ext2fs_be64_ಗಮ್ಯ_cpu(x) ((__force __u64)(__be64)(x))
#ವ್ಯಾಖ್ಯಾನಿಸು ext2fs_cpu_ಗಮ್ಯ_be32(x) ((__force __be32)(__u32)(x))
#ವ್ಯಾಖ್ಯಾನಿಸು ext2fs_be32_ಗಮ್ಯ_cpu(x) ((__force __u32)(__be32)(x))
#ವ್ಯಾಖ್ಯಾನಿಸು ext2fs_cpu_ಗಮ್ಯ_be16(x) ((__force __be16)(__u16)(x))
#ವ್ಯಾಖ್ಯಾನಿಸು ext2fs_be16_ಗಮ್ಯ_cpu(x) ((__force __u16)(__be16)(x))
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು ext2fs_cpu_ಗಮ್ಯ_le64(x) ((__force __le64)(__u64)(x))
#ವ್ಯಾಖ್ಯಾನಿಸು ext2fs_le64_ಗಮ್ಯ_cpu(x) ((__force __u64)(__le64)(x))
#ವ್ಯಾಖ್ಯಾನಿಸು ext2fs_cpu_ಗಮ್ಯ_le32(x) ((__force __le32)(__u32)(x))
#ವ್ಯಾಖ್ಯಾನಿಸು ext2fs_le32_ಗಮ್ಯ_cpu(x) ((__force __u32)(__le32)(x))
#ವ್ಯಾಖ್ಯಾನಿಸು ext2fs_cpu_ಗಮ್ಯ_le16(x) ((__force __le16)(__u16)(x))
#ವ್ಯಾಖ್ಯಾನಿಸು ext2fs_le16_ಗಮ್ಯ_cpu(x) ((__force __u16)(__le16)(x))

#ವ್ಯಾಖ್ಯಾನಿಸು ext2fs_cpu_ಗಮ್ಯ_be64(x) ((__force __be64)ext2fs_swab64((__u64)(x)))
#ವ್ಯಾಖ್ಯಾನಿಸು ext2fs_be64_ಗಮ್ಯ_cpu(x) ext2fs_swab64((__force __u64)(__be64)(x))
#ವ್ಯಾಖ್ಯಾನಿಸು ext2fs_cpu_ಗಮ್ಯ_be32(x) ((__force __be32)ext2fs_swab32((__u32)(x)))
#ವ್ಯಾಖ್ಯಾನಿಸು ext2fs_be32_ಗಮ್ಯ_cpu(x) ext2fs_swab32((__force __u32)(__be32)(x))
#ವ್ಯಾಖ್ಯಾನಿಸು ext2fs_cpu_ಗಮ್ಯ_be16(x) ((__force __be16)ext2fs_swab16((__u16)(x)))
#ವ್ಯಾಖ್ಯಾನಿಸು ext2fs_be16_ಗಮ್ಯ_cpu(x) ext2fs_swab16((__force __u16)(__be16)(x))
#ಆದರೆ_ಅಂತ್ಯ

/*
 * EXT2FS bitmap manipulation routines.
 */

/* Support for sending warning messages from the inline subroutines */
ಬಾಹ್ಯ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ext2fs_ಖಂಡ_string;
ಬಾಹ್ಯ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ext2fs_inode_string;
ಬಾಹ್ಯ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ext2fs_mark_string;
ಬಾಹ್ಯ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ext2fs_unmark_string;
ಬಾಹ್ಯ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ext2fs_test_string;
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_warn_bitmap(errcode_t errcode, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ arg,
			       ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *description);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_warn_bitmap2(ext2fs_generic_bitmap bitmap,
				ಪೂರ್ಣಾಂಕ ಸಂಕೇತ_2, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ arg);

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ NO_INLINE_FUNCS
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_fast_ಗಣ_ಬಿಟ್(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ nr,ಶೂನ್ಯ * addr);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_fast_clear_ಬಿಟ್(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ nr, ಶೂನ್ಯ * addr);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_fast_ಗಣ_bit64(__u64 nr,ಶೂನ್ಯ * addr);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_fast_clear_bit64(__u64 nr, ಶೂನ್ಯ * addr);
ಬಾಹ್ಯ __u16 ext2fs_swab16(__u16 val);
ಬಾಹ್ಯ __u32 ext2fs_swab32(__u32 val);
ಬಾಹ್ಯ __u64 ext2fs_swab64(__u64 val);

ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_mark_ಖಂಡ_bitmap(ext2fs_ಖಂಡ_bitmap bitmap, blk_t ಖಂಡ);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_unmark_ಖಂಡ_bitmap(ext2fs_ಖಂಡ_bitmap bitmap,
				       blk_t ಖಂಡ);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_test_ಖಂಡ_bitmap(ext2fs_ಖಂಡ_bitmap bitmap, blk_t ಖಂಡ);

ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_mark_inode_bitmap(ext2fs_inode_bitmap bitmap, ext2_ino_t inode);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_unmark_inode_bitmap(ext2fs_inode_bitmap bitmap,
				       ext2_ino_t inode);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_test_inode_bitmap(ext2fs_inode_bitmap bitmap, ext2_ino_t inode);

ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_fast_mark_ಖಂಡ_bitmap(ext2fs_ಖಂಡ_bitmap bitmap,
					  blk_t ಖಂಡ);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_fast_unmark_ಖಂಡ_bitmap(ext2fs_ಖಂಡ_bitmap bitmap,
					    blk_t ಖಂಡ);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_fast_test_ಖಂಡ_bitmap(ext2fs_ಖಂಡ_bitmap bitmap,
					 blk_t ಖಂಡ);

ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_fast_mark_inode_bitmap(ext2fs_inode_bitmap bitmap,
					  ext2_ino_t inode);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_fast_unmark_inode_bitmap(ext2fs_inode_bitmap bitmap,
					    ext2_ino_t inode);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_fast_test_inode_bitmap(ext2fs_inode_bitmap bitmap,
					 ext2_ino_t inode);
ಬಾಹ್ಯ blk_t ext2fs_get_ಖಂಡ_bitmap_ಆರಂಭ(ext2fs_ಖಂಡ_bitmap bitmap);
ಬಾಹ್ಯ ext2_ino_t ext2fs_get_inode_bitmap_ಆರಂಭ(ext2fs_inode_bitmap bitmap);
ಬಾಹ್ಯ blk_t ext2fs_get_ಖಂಡ_bitmap_ಕೊನೆ(ext2fs_ಖಂಡ_bitmap bitmap);
ಬಾಹ್ಯ ext2_ino_t ext2fs_get_inode_bitmap_ಕೊನೆ(ext2fs_inode_bitmap bitmap);

ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_fast_mark_ಖಂಡ_bitmap_range(ext2fs_ಖಂಡ_bitmap bitmap,
						blk_t ಖಂಡ, ಪೂರ್ಣಾಂಕ ಅಂಕೆ);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_fast_unmark_ಖಂಡ_bitmap_range(ext2fs_ಖಂಡ_bitmap bitmap,
						  blk_t ಖಂಡ, ಪೂರ್ಣಾಂಕ ಅಂಕೆ);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_fast_test_ಖಂಡ_bitmap_range(ext2fs_ಖಂಡ_bitmap bitmap,
					       blk_t ಖಂಡ, ಪೂರ್ಣಾಂಕ ಅಂಕೆ);
#ಆದರೆ_ಅಂತ್ಯ

/* These functions routines moved to gen_bitmap.c */
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_mark_ಖಂಡ_bitmap_range(ext2fs_ಖಂಡ_bitmap bitmap,
					   blk_t ಖಂಡ, ಪೂರ್ಣಾಂಕ ಅಂಕೆ);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_unmark_ಖಂಡ_bitmap_range(ext2fs_ಖಂಡ_bitmap bitmap,
					     blk_t ಖಂಡ, ಪೂರ್ಣಾಂಕ ಅಂಕೆ);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_test_ಖಂಡ_bitmap_range(ext2fs_ಖಂಡ_bitmap bitmap,
					  blk_t ಖಂಡ, ಪೂರ್ಣಾಂಕ ಅಂಕೆ);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_test_inode_bitmap_range(ext2fs_inode_bitmap bitmap,
					  ext2_ino_t inode, ಪೂರ್ಣಾಂಕ ಅಂಕೆ);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_mark_generic_bitmap(ext2fs_generic_bitmap bitmap,
					 __u32 bitno);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_unmark_generic_bitmap(ext2fs_generic_bitmap bitmap,
					   blk_t bitno);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_test_generic_bitmap(ext2fs_generic_bitmap bitmap,
				      blk_t bitno);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_test_ಖಂಡ_bitmap_range(ext2fs_ಖಂಡ_bitmap bitmap,
					  blk_t ಖಂಡ, ಪೂರ್ಣಾಂಕ ಅಂಕೆ);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_ಗಣ_bitmap_padding(ext2fs_generic_bitmap map);
ಬಾಹ್ಯ __u32 ext2fs_get_generic_bitmap_ಆರಂಭ(ext2fs_generic_bitmap bitmap);
ಬಾಹ್ಯ __u32 ext2fs_get_generic_bitmap_ಕೊನೆ(ext2fs_generic_bitmap bitmap);

/* 64-bit versions */

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ NO_INLINE_FUNCS
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_mark_ಖಂಡ_bitmap2(ext2fs_ಖಂಡ_bitmap bitmap,
				     blk64_t ಖಂಡ);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_unmark_ಖಂಡ_bitmap2(ext2fs_ಖಂಡ_bitmap bitmap,
				       blk64_t ಖಂಡ);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_test_ಖಂಡ_bitmap2(ext2fs_ಖಂಡ_bitmap bitmap,
				     blk64_t ಖಂಡ);

ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_mark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
				     ext2_ino_t inode);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_unmark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
				       ext2_ino_t inode);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_test_inode_bitmap2(ext2fs_inode_bitmap bitmap,
				     ext2_ino_t inode);

ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_fast_mark_ಖಂಡ_bitmap2(ext2fs_ಖಂಡ_bitmap bitmap,
					   blk64_t ಖಂಡ);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_fast_unmark_ಖಂಡ_bitmap2(ext2fs_ಖಂಡ_bitmap bitmap,
					     blk64_t ಖಂಡ);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_fast_test_ಖಂಡ_bitmap2(ext2fs_ಖಂಡ_bitmap bitmap,
					  blk64_t ಖಂಡ);

ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_fast_mark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
					   ext2_ino_t inode);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_fast_unmark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
					    ext2_ino_t inode);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_fast_test_inode_bitmap2(ext2fs_inode_bitmap bitmap,
					  ext2_ino_t inode);
ಬಾಹ್ಯ errcode_t ext2fs_ಕಂಡುಹಿಡಿ_ಮೊದಲ_zero_ಖಂಡ_bitmap2(ext2fs_ಖಂಡ_bitmap bitmap,
						      blk64_t ಆರಂಭ,
						      blk64_t ಕೊನೆ,
						      blk64_t *ಹೊರಹೋಗು);
ಬಾಹ್ಯ errcode_t ext2fs_ಕಂಡುಹಿಡಿ_ಮೊದಲ_zero_inode_bitmap2(ext2fs_inode_bitmap bitmap,
						      ext2_ino_t ಆರಂಭ,
						      ext2_ino_t ಕೊನೆ,
						      ext2_ino_t *ಹೊರಹೋಗು);
ಬಾಹ್ಯ errcode_t ext2fs_ಕಂಡುಹಿಡಿ_ಮೊದಲ_ಗಣ_ಖಂಡ_bitmap2(ext2fs_ಖಂಡ_bitmap bitmap,
						     blk64_t ಆರಂಭ,
						     blk64_t ಕೊನೆ,
						     blk64_t *ಹೊರಹೋಗು);
ಬಾಹ್ಯ errcode_t ext2fs_ಕಂಡುಹಿಡಿ_ಮೊದಲ_ಗಣ_inode_bitmap2(ext2fs_inode_bitmap bitmap,
						      ext2_ino_t ಆರಂಭ,
						      ext2_ino_t ಕೊನೆ,
						      ext2_ino_t *ಹೊರಹೋಗು);
ಬಾಹ್ಯ blk64_t ext2fs_get_ಖಂಡ_bitmap_start2(ext2fs_ಖಂಡ_bitmap bitmap);
ಬಾಹ್ಯ ext2_ino_t ext2fs_get_inode_bitmap_start2(ext2fs_inode_bitmap bitmap);
ಬಾಹ್ಯ blk64_t ext2fs_get_ಖಂಡ_bitmap_end2(ext2fs_ಖಂಡ_bitmap bitmap);
ಬಾಹ್ಯ ext2_ino_t ext2fs_get_inode_bitmap_end2(ext2fs_inode_bitmap bitmap);

ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_fast_test_ಖಂಡ_bitmap_range2(ext2fs_ಖಂಡ_bitmap bitmap,
						blk64_t ಖಂಡ,
						ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಅಂಕೆ);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_fast_mark_ಖಂಡ_bitmap_range2(ext2fs_ಖಂಡ_bitmap bitmap,
						 blk64_t ಖಂಡ,
						 ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಅಂಕೆ);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_fast_unmark_ಖಂಡ_bitmap_range2(ext2fs_ಖಂಡ_bitmap bitmap,
						   blk64_t ಖಂಡ,
						   ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಅಂಕೆ);
#ಆದರೆ_ಅಂತ್ಯ

/* These routines moved to gen_bitmap64.c */
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_clear_generic_bmap(ext2fs_generic_bitmap bitmap);
ಬಾಹ್ಯ errcode_t ext2fs_compare_generic_bmap(errcode_t neq,
					     ext2fs_generic_bitmap bm1,
					     ext2fs_generic_bitmap bm2);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_ಗಣ_generic_bmap_padding(ext2fs_generic_bitmap bmap);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_mark_generic_bmap(ext2fs_generic_bitmap bitmap,
				    blk64_t bitno);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_unmark_generic_bmap(ext2fs_generic_bitmap bitmap,
				      blk64_t bitno);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_test_generic_bmap(ext2fs_generic_bitmap bitmap,
				    blk64_t bitno);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_test_ಖಂಡ_bitmap_range2(ext2fs_ಖಂಡ_bitmap bitmap,
					   blk64_t ಖಂಡ, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಅಂಕೆ);
ಬಾಹ್ಯ __u64 ext2fs_get_generic_bmap_ಆರಂಭ(ext2fs_generic_bitmap bitmap);
ಬಾಹ್ಯ __u64 ext2fs_get_generic_bmap_ಕೊನೆ(ext2fs_generic_bitmap bitmap);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_test_ಖಂಡ_bitmap_range2(ext2fs_ಖಂಡ_bitmap bitmap,
					   blk64_t ಖಂಡ, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಅಂಕೆ);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_mark_ಖಂಡ_bitmap_range2(ext2fs_ಖಂಡ_bitmap bitmap,
					    blk64_t ಖಂಡ, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಅಂಕೆ);
ಬಾಹ್ಯ ಶೂನ್ಯ ext2fs_unmark_ಖಂಡ_bitmap_range2(ext2fs_ಖಂಡ_bitmap bitmap,
					      blk64_t ಖಂಡ, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಅಂಕೆ);
ಬಾಹ್ಯ errcode_t ext2fs_ಕಂಡುಹಿಡಿ_ಮೊದಲ_zero_generic_bmap(ext2fs_generic_bitmap bitmap,
						     __u64 ಆರಂಭ, __u64 ಕೊನೆ,
						     __u64 *ಹೊರಹೋಗು);
ಬಾಹ್ಯ errcode_t ext2fs_ಕಂಡುಹಿಡಿ_ಮೊದಲ_ಗಣ_generic_bmap(ext2fs_generic_bitmap bitmap,
						    __u64 ಆರಂಭ, __u64 ಕೊನೆ,
						    __u64 *ಹೊರಹೋಗು);

/*
 * The inline routines themselves...
 *
 * If NO_INLINE_FUNCS is defined, then we won't try to do inline
 * functions at all; they will be included as normal functions in
 * inline.c
 */

#ಆದರೆ (defined(INCLUDE_INLINE_FUNCS) || !defined(NO_INLINE_FUNCS))
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ INCLUDE_INLINE_FUNCS
#ಆದರೆ (__STDC_VERSION__ >= 199901L)
#ವ್ಯಾಖ್ಯಾನಿಸು _INLINE_ ಬಾಹ್ಯ ಸಾಲೊಳಗೆ
#ಇಲ್ಲದಿದ್ದರೆ
#ವ್ಯಾಖ್ಯಾನಿಸು _INLINE_ ಸಾಲೊಳಗೆ
#ಆದರೆ_ಅಂತ್ಯ
#ಇಲ್ಲದಿದ್ದರೆ /* !INCLUDE_INLINE FUNCS */
#ಆದರೆ (__STDC_VERSION__ >= 199901L)
#ವ್ಯಾಖ್ಯಾನಿಸು _INLINE_ ಸಾಲೊಳಗೆ
#ಇಲ್ಲದಿದ್ದರೆ /* not C99 */
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __GNUC__
#ವ್ಯಾಖ್ಯಾನಿಸು _INLINE_ ಬಾಹ್ಯ __inline__
#ಇಲ್ಲದಿದ್ದರೆ				/* For Watcom C */
#ವ್ಯಾಖ್ಯಾನಿಸು _INLINE_ ಬಾಹ್ಯ ಸಾಲೊಳಗೆ
#ಆದರೆ_ಅಂತ್ಯ /* __GNUC__ */
#ಆದರೆ_ಅಂತ್ಯ /* __STDC_VERSION__ >= 199901L */
#ಆದರೆ_ಅಂತ್ಯ /* INCLUDE_INLINE_FUNCS */

/*
 * Fast bit set/clear functions that doesn't need to return the
 * previous bit value.
 */

_INLINE_ ಶೂನ್ಯ ext2fs_fast_ಗಣ_ಬಿಟ್(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ nr,ಶೂನ್ಯ * addr)
{
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ	*ADDR = (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *) addr;

	ADDR += nr >> 3;
	*ADDR |= (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ) (1 << (nr & 0x07));
}

_INLINE_ ಶೂನ್ಯ ext2fs_fast_clear_ಬಿಟ್(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ nr, ಶೂನ್ಯ * addr)
{
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ	*ADDR = (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *) addr;

	ADDR += nr >> 3;
	*ADDR &= (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ) ~(1 << (nr & 0x07));
}


_INLINE_ ಶೂನ್ಯ ext2fs_fast_ಗಣ_bit64(__u64 nr, ಶೂನ್ಯ * addr)
{
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ	*ADDR = (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *) addr;

	ADDR += nr >> 3;
	*ADDR |= (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ) (1 << (nr & 0x07));
}

_INLINE_ ಶೂನ್ಯ ext2fs_fast_clear_bit64(__u64 nr, ಶೂನ್ಯ * addr)
{
	ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ	*ADDR = (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *) addr;

	ADDR += nr >> 3;
	*ADDR &= (ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ) ~(1 << (nr & 0x07));
}

_INLINE_ __u16 ext2fs_swab16(__u16 val)
{
	ಹಿಂತಿರುಗಿ (val >> 8) | (__u16) (val << 8);
}

_INLINE_ __u32 ext2fs_swab32(__u32 val)
{
	ಹಿಂತಿರುಗಿ ((val>>24) | ((val>>8)&0xFF00) |
		((val<<8)&0xFF0000) | (val<<24));
}

_INLINE_ __u64 ext2fs_swab64(__u64 val)
{
	ಹಿಂತಿರುಗಿ (ext2fs_swab32((__u32) (val >> 32)) |
		(((__u64)ext2fs_swab32(val & 0xFFFFFFFFUL)) << 32));
}

_INLINE_ ಪೂರ್ಣಾಂಕ ext2fs_mark_ಖಂಡ_bitmap(ext2fs_ಖಂಡ_bitmap bitmap,
				       blk_t ಖಂಡ)
{
	ಹಿಂತಿರುಗಿ ext2fs_mark_generic_bitmap((ext2fs_generic_bitmap) bitmap,
					  ಖಂಡ);
}

_INLINE_ ಪೂರ್ಣಾಂಕ ext2fs_unmark_ಖಂಡ_bitmap(ext2fs_ಖಂಡ_bitmap bitmap,
					 blk_t ಖಂಡ)
{
	ಹಿಂತಿರುಗಿ ext2fs_unmark_generic_bitmap((ext2fs_generic_bitmap) bitmap,
					    ಖಂಡ);
}

_INLINE_ ಪೂರ್ಣಾಂಕ ext2fs_test_ಖಂಡ_bitmap(ext2fs_ಖಂಡ_bitmap bitmap,
				       blk_t ಖಂಡ)
{
	ಹಿಂತಿರುಗಿ ext2fs_test_generic_bitmap((ext2fs_generic_bitmap) bitmap,
					  ಖಂಡ);
}

_INLINE_ ಪೂರ್ಣಾಂಕ ext2fs_mark_inode_bitmap(ext2fs_inode_bitmap bitmap,
				       ext2_ino_t inode)
{
	ಹಿಂತಿರುಗಿ ext2fs_mark_generic_bitmap((ext2fs_generic_bitmap) bitmap,
					  inode);
}

_INLINE_ ಪೂರ್ಣಾಂಕ ext2fs_unmark_inode_bitmap(ext2fs_inode_bitmap bitmap,
					 ext2_ino_t inode)
{
	ಹಿಂತಿರುಗಿ ext2fs_unmark_generic_bitmap((ext2fs_generic_bitmap) bitmap,
				     inode);
}

_INLINE_ ಪೂರ್ಣಾಂಕ ext2fs_test_inode_bitmap(ext2fs_inode_bitmap bitmap,
				       ext2_ino_t inode)
{
	ಹಿಂತಿರುಗಿ ext2fs_test_generic_bitmap((ext2fs_generic_bitmap) bitmap,
					  inode);
}

_INLINE_ ಶೂನ್ಯ ext2fs_fast_mark_ಖಂಡ_bitmap(ext2fs_ಖಂಡ_bitmap bitmap,
					    blk_t ಖಂಡ)
{
	ext2fs_mark_generic_bitmap((ext2fs_generic_bitmap) bitmap, ಖಂಡ);
}

_INLINE_ ಶೂನ್ಯ ext2fs_fast_unmark_ಖಂಡ_bitmap(ext2fs_ಖಂಡ_bitmap bitmap,
					      blk_t ಖಂಡ)
{
	ext2fs_unmark_generic_bitmap((ext2fs_generic_bitmap) bitmap, ಖಂಡ);
}

_INLINE_ ಪೂರ್ಣಾಂಕ ext2fs_fast_test_ಖಂಡ_bitmap(ext2fs_ಖಂಡ_bitmap bitmap,
					    blk_t ಖಂಡ)
{
	ಹಿಂತಿರುಗಿ ext2fs_test_generic_bitmap((ext2fs_generic_bitmap) bitmap,
					  ಖಂಡ);
}

_INLINE_ ಶೂನ್ಯ ext2fs_fast_mark_inode_bitmap(ext2fs_inode_bitmap bitmap,
					    ext2_ino_t inode)
{
	ext2fs_mark_generic_bitmap((ext2fs_generic_bitmap) bitmap, inode);
}

_INLINE_ ಶೂನ್ಯ ext2fs_fast_unmark_inode_bitmap(ext2fs_inode_bitmap bitmap,
					      ext2_ino_t inode)
{
	ext2fs_unmark_generic_bitmap((ext2fs_generic_bitmap) bitmap, inode);
}

_INLINE_ ಪೂರ್ಣಾಂಕ ext2fs_fast_test_inode_bitmap(ext2fs_inode_bitmap bitmap,
					   ext2_ino_t inode)
{
	ಹಿಂತಿರುಗಿ ext2fs_test_generic_bitmap((ext2fs_generic_bitmap) bitmap,
					  inode);
}

_INLINE_ blk_t ext2fs_get_ಖಂಡ_bitmap_ಆರಂಭ(ext2fs_ಖಂಡ_bitmap bitmap)
{
	ಹಿಂತಿರುಗಿ ext2fs_get_generic_bitmap_ಆರಂಭ((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ ext2_ino_t ext2fs_get_inode_bitmap_ಆರಂಭ(ext2fs_inode_bitmap bitmap)
{
	ಹಿಂತಿರುಗಿ ext2fs_get_generic_bitmap_ಆರಂಭ((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ blk_t ext2fs_get_ಖಂಡ_bitmap_ಕೊನೆ(ext2fs_ಖಂಡ_bitmap bitmap)
{
	ಹಿಂತಿರುಗಿ ext2fs_get_generic_bitmap_ಕೊನೆ((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ ext2_ino_t ext2fs_get_inode_bitmap_ಕೊನೆ(ext2fs_inode_bitmap bitmap)
{
	ಹಿಂತಿರುಗಿ ext2fs_get_generic_bitmap_ಕೊನೆ((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ ಪೂರ್ಣಾಂಕ ext2fs_fast_test_ಖಂಡ_bitmap_range(ext2fs_ಖಂಡ_bitmap bitmap,
						 blk_t ಖಂಡ, ಪೂರ್ಣಾಂಕ ಅಂಕೆ)
{
	ಹಿಂತಿರುಗಿ ext2fs_test_ಖಂಡ_bitmap_range(bitmap, ಖಂಡ, ಅಂಕೆ);
}

_INLINE_ ಶೂನ್ಯ ext2fs_fast_mark_ಖಂಡ_bitmap_range(ext2fs_ಖಂಡ_bitmap bitmap,
						  blk_t ಖಂಡ, ಪೂರ್ಣಾಂಕ ಅಂಕೆ)
{
	ext2fs_mark_ಖಂಡ_bitmap_range(bitmap, ಖಂಡ, ಅಂಕೆ);
}

_INLINE_ ಶೂನ್ಯ ext2fs_fast_unmark_ಖಂಡ_bitmap_range(ext2fs_ಖಂಡ_bitmap bitmap,
						    blk_t ಖಂಡ, ಪೂರ್ಣಾಂಕ ಅಂಕೆ)
{
	ext2fs_unmark_ಖಂಡ_bitmap_range(bitmap, ಖಂಡ, ಅಂಕೆ);
}

/* 64-bit versions */

_INLINE_ ಪೂರ್ಣಾಂಕ ext2fs_mark_ಖಂಡ_bitmap2(ext2fs_ಖಂಡ_bitmap bitmap,
				       blk64_t ಖಂಡ)
{
	ಹಿಂತಿರುಗಿ ext2fs_mark_generic_bmap((ext2fs_generic_bitmap) bitmap,
					ಖಂಡ);
}

_INLINE_ ಪೂರ್ಣಾಂಕ ext2fs_unmark_ಖಂಡ_bitmap2(ext2fs_ಖಂಡ_bitmap bitmap,
					 blk64_t ಖಂಡ)
{
	ಹಿಂತಿರುಗಿ ext2fs_unmark_generic_bmap((ext2fs_generic_bitmap) bitmap, ಖಂಡ);
}

_INLINE_ ಪೂರ್ಣಾಂಕ ext2fs_test_ಖಂಡ_bitmap2(ext2fs_ಖಂಡ_bitmap bitmap,
				       blk64_t ಖಂಡ)
{
	ಹಿಂತಿರುಗಿ ext2fs_test_generic_bmap((ext2fs_generic_bitmap) bitmap,
					ಖಂಡ);
}

_INLINE_ ಪೂರ್ಣಾಂಕ ext2fs_mark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
				       ext2_ino_t inode)
{
	ಹಿಂತಿರುಗಿ ext2fs_mark_generic_bmap((ext2fs_generic_bitmap) bitmap,
					inode);
}

_INLINE_ ಪೂರ್ಣಾಂಕ ext2fs_unmark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
					 ext2_ino_t inode)
{
	ಹಿಂತಿರುಗಿ ext2fs_unmark_generic_bmap((ext2fs_generic_bitmap) bitmap,
					  inode);
}

_INLINE_ ಪೂರ್ಣಾಂಕ ext2fs_test_inode_bitmap2(ext2fs_inode_bitmap bitmap,
				       ext2_ino_t inode)
{
	ಹಿಂತಿರುಗಿ ext2fs_test_generic_bmap((ext2fs_generic_bitmap) bitmap,
					inode);
}

_INLINE_ ಶೂನ್ಯ ext2fs_fast_mark_ಖಂಡ_bitmap2(ext2fs_ಖಂಡ_bitmap bitmap,
					     blk64_t ಖಂಡ)
{
	ext2fs_mark_generic_bmap((ext2fs_generic_bitmap) bitmap, ಖಂಡ);
}

_INLINE_ ಶೂನ್ಯ ext2fs_fast_unmark_ಖಂಡ_bitmap2(ext2fs_ಖಂಡ_bitmap bitmap,
					       blk64_t ಖಂಡ)
{
	ext2fs_unmark_generic_bmap((ext2fs_generic_bitmap) bitmap, ಖಂಡ);
}

_INLINE_ ಪೂರ್ಣಾಂಕ ext2fs_fast_test_ಖಂಡ_bitmap2(ext2fs_ಖಂಡ_bitmap bitmap,
					    blk64_t ಖಂಡ)
{
	ಹಿಂತಿರುಗಿ ext2fs_test_generic_bmap((ext2fs_generic_bitmap) bitmap,
					ಖಂಡ);
}

_INLINE_ ಶೂನ್ಯ ext2fs_fast_mark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
					     ext2_ino_t inode)
{
	ext2fs_mark_generic_bmap((ext2fs_generic_bitmap) bitmap, inode);
}

_INLINE_ ಶೂನ್ಯ ext2fs_fast_unmark_inode_bitmap2(ext2fs_inode_bitmap bitmap,
					       ext2_ino_t inode)
{
	ext2fs_unmark_generic_bmap((ext2fs_generic_bitmap) bitmap, inode);
}

_INLINE_ ಪೂರ್ಣಾಂಕ ext2fs_fast_test_inode_bitmap2(ext2fs_inode_bitmap bitmap,
					    ext2_ino_t inode)
{
	ಹಿಂತಿರುಗಿ ext2fs_test_generic_bmap((ext2fs_generic_bitmap) bitmap,
					inode);
}

_INLINE_ errcode_t ext2fs_ಕಂಡುಹಿಡಿ_ಮೊದಲ_zero_ಖಂಡ_bitmap2(ext2fs_ಖಂಡ_bitmap bitmap,
							blk64_t ಆರಂಭ,
							blk64_t ಕೊನೆ,
							blk64_t *ಹೊರಹೋಗು)
{
	__u64 o;
	errcode_t rv;

	rv = ext2fs_ಕಂಡುಹಿಡಿ_ಮೊದಲ_zero_generic_bmap((ext2fs_generic_bitmap) bitmap,
						 ಆರಂಭ, ಕೊನೆ, &o);
	ಆದರೆ (!rv)
		*ಹೊರಹೋಗು = o;
	ಹಿಂತಿರುಗಿ rv;
}

_INLINE_ errcode_t ext2fs_ಕಂಡುಹಿಡಿ_ಮೊದಲ_zero_inode_bitmap2(ext2fs_inode_bitmap bitmap,
							ext2_ino_t ಆರಂಭ,
							ext2_ino_t ಕೊನೆ,
							ext2_ino_t *ಹೊರಹೋಗು)
{
	__u64 o;
	errcode_t rv;

	rv = ext2fs_ಕಂಡುಹಿಡಿ_ಮೊದಲ_zero_generic_bmap((ext2fs_generic_bitmap) bitmap,
						 ಆರಂಭ, ಕೊನೆ, &o);
	ಆದರೆ (!rv)
		*ಹೊರಹೋಗು = (ext2_ino_t) o;
	ಹಿಂತಿರುಗಿ rv;
}

_INLINE_ errcode_t ext2fs_ಕಂಡುಹಿಡಿ_ಮೊದಲ_ಗಣ_ಖಂಡ_bitmap2(ext2fs_ಖಂಡ_bitmap bitmap,
						       blk64_t ಆರಂಭ,
						       blk64_t ಕೊನೆ,
						       blk64_t *ಹೊರಹೋಗು)
{
	__u64 o;
	errcode_t rv;

	rv = ext2fs_ಕಂಡುಹಿಡಿ_ಮೊದಲ_ಗಣ_generic_bmap((ext2fs_generic_bitmap) bitmap,
						ಆರಂಭ, ಕೊನೆ, &o);
	ಆದರೆ (!rv)
		*ಹೊರಹೋಗು = o;
	ಹಿಂತಿರುಗಿ rv;
}

_INLINE_ errcode_t ext2fs_ಕಂಡುಹಿಡಿ_ಮೊದಲ_ಗಣ_inode_bitmap2(ext2fs_inode_bitmap bitmap,
						       ext2_ino_t ಆರಂಭ,
						       ext2_ino_t ಕೊನೆ,
						       ext2_ino_t *ಹೊರಹೋಗು)
{
	__u64 o;
	errcode_t rv;

	rv = ext2fs_ಕಂಡುಹಿಡಿ_ಮೊದಲ_ಗಣ_generic_bmap((ext2fs_generic_bitmap) bitmap,
						ಆರಂಭ, ಕೊನೆ, &o);
	ಆದರೆ (!rv)
		*ಹೊರಹೋಗು = (ext2_ino_t) o;
	ಹಿಂತಿರುಗಿ rv;
}

_INLINE_ blk64_t ext2fs_get_ಖಂಡ_bitmap_start2(ext2fs_ಖಂಡ_bitmap bitmap)
{
	ಹಿಂತಿರುಗಿ ext2fs_get_generic_bmap_ಆರಂಭ((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ ext2_ino_t ext2fs_get_inode_bitmap_start2(ext2fs_inode_bitmap bitmap)
{
	ಹಿಂತಿರುಗಿ (ext2_ino_t) ext2fs_get_generic_bmap_ಆರಂಭ((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ blk64_t ext2fs_get_ಖಂಡ_bitmap_end2(ext2fs_ಖಂಡ_bitmap bitmap)
{
	ಹಿಂತಿರುಗಿ ext2fs_get_generic_bmap_ಕೊನೆ((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ ext2_ino_t ext2fs_get_inode_bitmap_end2(ext2fs_inode_bitmap bitmap)
{
	ಹಿಂತಿರುಗಿ (ext2_ino_t) ext2fs_get_generic_bmap_ಕೊನೆ((ext2fs_generic_bitmap) bitmap);
}

_INLINE_ ಪೂರ್ಣಾಂಕ ext2fs_fast_test_ಖಂಡ_bitmap_range2(ext2fs_ಖಂಡ_bitmap bitmap,
						  blk64_t ಖಂಡ,
						  ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಅಂಕೆ)
{
	ಹಿಂತಿರುಗಿ ext2fs_test_ಖಂಡ_bitmap_range2(bitmap, ಖಂಡ, ಅಂಕೆ);
}

_INLINE_ ಶೂನ್ಯ ext2fs_fast_mark_ಖಂಡ_bitmap_range2(ext2fs_ಖಂಡ_bitmap bitmap,
						   blk64_t ಖಂಡ,
						   ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಅಂಕೆ)
{
	ext2fs_mark_ಖಂಡ_bitmap_range2(bitmap, ಖಂಡ, ಅಂಕೆ);
}

_INLINE_ ಶೂನ್ಯ ext2fs_fast_unmark_ಖಂಡ_bitmap_range2(ext2fs_ಖಂಡ_bitmap bitmap,
						     blk64_t ಖಂಡ,
						     ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಅಂಕೆ)
{
	ext2fs_unmark_ಖಂಡ_bitmap_range2(bitmap, ಖಂಡ, ಅಂಕೆ);
}

#ವ್ಯಾಖ್ಯಾನ_ರದ್ದು _INLINE_
#ಆದರೆ_ಅಂತ್ಯ

ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_ಗಣ_ಬಿಟ್(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ nr,ಶೂನ್ಯ * addr);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_clear_ಬಿಟ್(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ nr, ಶೂನ್ಯ * addr);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_test_ಬಿಟ್(ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ nr, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ * addr);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_ಗಣ_bit64(__u64 nr,ಶೂನ್ಯ * addr);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_clear_bit64(__u64 nr, ಶೂನ್ಯ * addr);
ಬಾಹ್ಯ ಪೂರ್ಣಾಂಕ ext2fs_test_bit64(__u64 nr, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ * addr);
ಬಾಹ್ಯ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ext2fs_bitcount(ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *addr, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ nbytes);
