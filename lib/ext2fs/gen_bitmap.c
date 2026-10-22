/*
 * gen_bitmap.c --- Generic (32-bit) bitmap routines
 *
 * Copyright (C) 2001 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */


#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಆದರೆ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <time.h>
#ಆದರೆ HAVE_SYS_STAT_H
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_SYS_TYPES_H
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fsP.h"

ರಚನೆ ext2fs_struct_generic_bitmap_32 {
	errcode_t	magic;
	ext2_filsys 	fs;
	__u32		ಆರಂಭ, ಕೊನೆ;
	__u32		real_ಕೊನೆ;
	ಅಕ್ಷರ	*	description;
	ಅಕ್ಷರ	*	bitmap;
	errcode_t	ತಳ_error_ಸಂಕೇತ;
	__u32		reserved[7];
};

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ ext2fs_struct_generic_bitmap_32 *ext2fs_generic_bitmap_32;

#ವ್ಯಾಖ್ಯಾನಿಸು EXT2FS_IS_32_BITMAP(bmap) \
	(((bmap)->magic == EXT2_ET_MAGIC_GENERIC_BITMAP) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_ಖಂಡ_BITMAP) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_INODE_BITMAP))

#ವ್ಯಾಖ್ಯಾನಿಸು EXT2FS_IS_64_BITMAP(bmap) \
	(((bmap)->magic == EXT2_ET_MAGIC_GENERIC_BITMAP64) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_ಖಂಡ_BITMAP64) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_INODE_BITMAP64))

/*
 * Used by previously inlined function, so we have to export this and
 * not change the function signature
 */
ಶೂನ್ಯ ext2fs_warn_bitmap2(ext2fs_generic_bitmap gen_bitmap,
			    ಪೂರ್ಣಾಂಕ ಸಂಕೇತ_2, ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ arg)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ OMIT_COM_ERR
	ಆದರೆ (bitmap->description)
		com_err(0, bitmap->ತಳ_error_ಸಂಕೇತ+ಸಂಕೇತ_2,
			"#%lu for %s", arg, bitmap->description);
	ಇಲ್ಲದಿದ್ದರೆ
		com_err(0, bitmap->ತಳ_error_ಸಂಕೇತ + ಸಂಕೇತ_2, "#%lu", arg);
#ಆದರೆ_ಅಂತ್ಯ
}

ಸ್ಥಿರ errcode_t check_magic(ext2fs_generic_bitmap bitmap)
{
	ಆದರೆ (!bitmap || !((bitmap->magic == EXT2_ET_MAGIC_GENERIC_BITMAP) ||
			 (bitmap->magic == EXT2_ET_MAGIC_INODE_BITMAP) ||
			 (bitmap->magic == EXT2_ET_MAGIC_ಖಂಡ_BITMAP)))
		ಹಿಂತಿರುಗಿ EXT2_ET_MAGIC_GENERIC_BITMAP;
	ಹಿಂತಿರುಗಿ 0;
}

errcode_t ext2fs_make_generic_bitmap(errcode_t magic, ext2_filsys fs,
				     __u32 ಆರಂಭ, __u32 ಕೊನೆ, __u32 real_ಕೊನೆ,
				     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *descr, ಅಕ್ಷರ *init_map,
				     ext2fs_generic_bitmap *ret)
{
	ext2fs_generic_bitmap_32 bitmap;
	errcode_t		retval;
	size_t			ಗಾತ್ರ_2;

	retval = ext2fs_get_mem(ಗಾತ್ರ(ರಚನೆ ext2fs_struct_generic_bitmap_32),
				&bitmap);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	bitmap->magic = magic;
	bitmap->fs = fs;
	bitmap->ಆರಂಭ = ಆರಂಭ;
	bitmap->ಕೊನೆ = ಕೊನೆ;
	bitmap->real_ಕೊನೆ = real_ಕೊನೆ;
	ಆಯ್ಕೆ (magic) {
	ಸ್ಥಿತಿ EXT2_ET_MAGIC_INODE_BITMAP:
		bitmap->ತಳ_error_ಸಂಕೇತ = EXT2_ET_BAD_INODE_MARK;
		ನಿಲ್ಲಿಸು;
	ಸ್ಥಿತಿ EXT2_ET_MAGIC_ಖಂಡ_BITMAP:
		bitmap->ತಳ_error_ಸಂಕೇತ = EXT2_ET_BAD_ಖಂಡ_MARK;
		ನಿಲ್ಲಿಸು;
	ಪೂರ್ವನಿಯೋಜಿತ:
		bitmap->ತಳ_error_ಸಂಕೇತ = EXT2_ET_BAD_GENERIC_MARK;
	}
	ಆದರೆ (descr) {
		retval = ext2fs_get_mem(ಅಕ್ಷರಸರಣಿ_ಉದ್ದ(descr)+1, &bitmap->description);
		ಆದರೆ (retval) {
			ext2fs_free_mem(&bitmap);
			ಹಿಂತಿರುಗಿ retval;
		}
		strcpy(bitmap->description, descr);
	} ಇಲ್ಲದಿದ್ದರೆ
		bitmap->description = 0;

	ಗಾತ್ರ_2 = (size_t) (((bitmap->real_ಕೊನೆ - bitmap->ಆರಂಭ) / 8) + 1);
	/* Round up to allow for the BT x86 instruction */
	ಗಾತ್ರ_2 = (ಗಾತ್ರ_2 + 7) & ~3;
	retval = ext2fs_get_mem(ಗಾತ್ರ_2, &bitmap->bitmap);
	ಆದರೆ (retval) {
		ext2fs_free_mem(&bitmap->description);
		ext2fs_free_mem(&bitmap);
		ಹಿಂತಿರುಗಿ retval;
	}

	ಆದರೆ (init_map)
		ಸ್ಮರಣೆ_ನಕಲು(bitmap->bitmap, init_map, ಗಾತ್ರ_2);
	ಇಲ್ಲದಿದ್ದರೆ
		memset(bitmap->bitmap, 0, ಗಾತ್ರ_2);
	*ret = (ext2fs_generic_bitmap) bitmap;
	ಹಿಂತಿರುಗಿ 0;
}

errcode_t ext2fs_allocate_generic_bitmap(__u32 ಆರಂಭ,
					 __u32 ಕೊನೆ,
					 __u32 real_ಕೊನೆ,
					 ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *descr,
					 ext2fs_generic_bitmap *ret)
{
	ಹಿಂತಿರುಗಿ ext2fs_make_generic_bitmap(EXT2_ET_MAGIC_GENERIC_BITMAP, 0,
					  ಆರಂಭ, ಕೊನೆ, real_ಕೊನೆ, descr, 0, ret);
}

errcode_t ext2fs_copy_generic_bitmap(ext2fs_generic_bitmap gen_src,
				     ext2fs_generic_bitmap *dest)
{
	ext2fs_generic_bitmap_32 src = (ext2fs_generic_bitmap_32) gen_src;

	ಹಿಂತಿರುಗಿ (ext2fs_make_generic_bitmap(src->magic, src->fs,
					   src->ಆರಂಭ, src->ಕೊನೆ,
					   src->real_ಕೊನೆ,
					   src->description, src->bitmap,
					   dest));
}

ಶೂನ್ಯ ext2fs_free_generic_bitmap(ext2fs_inode_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	ಆದರೆ (check_magic(gen_bitmap))
		ಹಿಂತಿರುಗಿ;

	bitmap->magic = 0;
	ಆದರೆ (bitmap->description) {
		ext2fs_free_mem(&bitmap->description);
		bitmap->description = 0;
	}
	ಆದರೆ (bitmap->bitmap) {
		ext2fs_free_mem(&bitmap->bitmap);
		bitmap->bitmap = 0;
	}
	ext2fs_free_mem(&bitmap);
}

ಪೂರ್ಣಾಂಕ ext2fs_test_generic_bitmap(ext2fs_generic_bitmap bitmap,
					blk_t bitno)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	ಆದರೆ (!EXT2FS_IS_32_BITMAP(bitmap)) {
		ಆದರೆ (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			ಹಿಂತಿರುಗಿ ext2fs_test_generic_bmap(bitmap, bitno);
		}
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"test_bitmap(%lu)", (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ) bitno);
#ಆದರೆ_ಅಂತ್ಯ
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ ((bitno < bitmap32->ಆರಂಭ) || (bitno > bitmap32->ಕೊನೆ)) {
		ext2fs_warn_bitmap2(bitmap, EXT2FS_TEST_ERROR, bitno);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಹಿಂತಿರುಗಿ ext2fs_test_ಬಿಟ್(bitno - bitmap32->ಆರಂಭ, bitmap32->bitmap);
}

ಪೂರ್ಣಾಂಕ ext2fs_mark_generic_bitmap(ext2fs_generic_bitmap bitmap,
					 __u32 bitno)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	ಆದರೆ (!EXT2FS_IS_32_BITMAP(bitmap)) {
		ಆದರೆ (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			ಹಿಂತಿರುಗಿ ext2fs_mark_generic_bmap(bitmap, bitno);
		}
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"mark_bitmap(%lu)", (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ) bitno);
#ಆದರೆ_ಅಂತ್ಯ
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ ((bitno < bitmap32->ಆರಂಭ) || (bitno > bitmap32->ಕೊನೆ)) {
		ext2fs_warn_bitmap2(bitmap, EXT2FS_MARK_ERROR, bitno);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಹಿಂತಿರುಗಿ ext2fs_ಗಣ_ಬಿಟ್(bitno - bitmap32->ಆರಂಭ, bitmap32->bitmap);
}

ಪೂರ್ಣಾಂಕ ext2fs_unmark_generic_bitmap(ext2fs_generic_bitmap bitmap,
					   blk_t bitno)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	ಆದರೆ (!EXT2FS_IS_32_BITMAP(bitmap)) {
		ಆದರೆ (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			ಹಿಂತಿರುಗಿ ext2fs_unmark_generic_bmap(bitmap, bitno);
		}
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"mark_bitmap(%lu)", (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ) bitno);
#ಆದರೆ_ಅಂತ್ಯ
		ಹಿಂತಿರುಗಿ 0;
	}

	ಆದರೆ ((bitno < bitmap32->ಆರಂಭ) || (bitno > bitmap32->ಕೊನೆ)) {
		ext2fs_warn_bitmap2(bitmap, EXT2FS_UNMARK_ERROR, bitno);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಹಿಂತಿರುಗಿ ext2fs_clear_ಬಿಟ್(bitno - bitmap32->ಆರಂಭ, bitmap32->bitmap);
}

__u32 ext2fs_get_generic_bitmap_ಆರಂಭ(ext2fs_generic_bitmap bitmap)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	ಆದರೆ (!EXT2FS_IS_32_BITMAP(bitmap)) {
		ಆದರೆ (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			ಹಿಂತಿರುಗಿ ext2fs_get_generic_bmap_ಆರಂಭ(bitmap);
		}
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"get_bitmap_start");
#ಆದರೆ_ಅಂತ್ಯ
		ಹಿಂತಿರುಗಿ 0;
	}

	ಹಿಂತಿರುಗಿ bitmap32->ಆರಂಭ;
}

__u32 ext2fs_get_generic_bitmap_ಕೊನೆ(ext2fs_generic_bitmap bitmap)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	ಆದರೆ (!EXT2FS_IS_32_BITMAP(bitmap)) {
		ಆದರೆ (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			ಹಿಂತಿರುಗಿ ext2fs_get_generic_bmap_ಕೊನೆ(bitmap);
		}
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"get_bitmap_end");
#ಆದರೆ_ಅಂತ್ಯ
		ಹಿಂತಿರುಗಿ 0;
	}
	ಹಿಂತಿರುಗಿ bitmap32->ಕೊನೆ;
}

ಶೂನ್ಯ ext2fs_clear_generic_bitmap(ext2fs_generic_bitmap bitmap)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	ಆದರೆ (!EXT2FS_IS_32_BITMAP(bitmap)) {
		ಆದರೆ (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			ext2fs_clear_generic_bmap(bitmap);
			ಹಿಂತಿರುಗಿ;
		}
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"clear_generic_bitmap");
#ಆದರೆ_ಅಂತ್ಯ
		ಹಿಂತಿರುಗಿ;
	}

	memset(bitmap32->bitmap, 0,
	       (size_t) (((bitmap32->real_ಕೊನೆ - bitmap32->ಆರಂಭ) / 8) + 1));
}

errcode_t ext2fs_fudge_generic_bitmap_ಕೊನೆ(ext2fs_inode_bitmap gen_bitmap,
					  errcode_t magic, errcode_t neq,
					  ext2_ino_t ಕೊನೆ, ext2_ino_t *oend)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	EXT2_CHECK_MAGIC(bitmap, magic);

	ಆದರೆ (ಕೊನೆ > bitmap->real_ಕೊನೆ)
		ಹಿಂತಿರುಗಿ neq;
	ಆದರೆ (oend)
		*oend = bitmap->ಕೊನೆ;
	bitmap->ಕೊನೆ = ಕೊನೆ;
	ಹಿಂತಿರುಗಿ 0;
}

errcode_t ext2fs_resize_generic_bitmap(errcode_t magic,
				       __u32 new_ಕೊನೆ, __u32 new_real_ಕೊನೆ,
				       ext2fs_generic_bitmap gen_bmap)
{
	ext2fs_generic_bitmap_32 bmap = (ext2fs_generic_bitmap_32) gen_bmap;
	errcode_t	retval;
	size_t		ಗಾತ್ರ_2, new_ಗಾತ್ರ;
	__u32		bitno;

	ಆದರೆ (!bmap || (bmap->magic != magic))
		ಹಿಂತಿರುಗಿ magic;

	/*
	 * If we're expanding the bitmap, make sure all of the new
	 * parts of the bitmap are zero.
	 */
	ಆದರೆ (new_ಕೊನೆ > bmap->ಕೊನೆ) {
		bitno = bmap->real_ಕೊನೆ;
		ಆದರೆ (bitno > new_ಕೊನೆ)
			bitno = new_ಕೊನೆ;
		ಗಾಗಿ (; bitno > bmap->ಕೊನೆ; bitno--)
			ext2fs_clear_ಬಿಟ್(bitno - bmap->ಆರಂಭ, bmap->bitmap);
	}
	ಆದರೆ (new_real_ಕೊನೆ == bmap->real_ಕೊನೆ) {
		bmap->ಕೊನೆ = new_ಕೊನೆ;
		ಹಿಂತಿರುಗಿ 0;
	}

	ಗಾತ್ರ_2 = ((bmap->real_ಕೊನೆ - bmap->ಆರಂಭ) / 8) + 1;
	new_ಗಾತ್ರ = ((new_real_ಕೊನೆ - bmap->ಆರಂಭ) / 8) + 1;

	ಆದರೆ (ಗಾತ್ರ_2 != new_ಗಾತ್ರ) {
		retval = ext2fs_resize_mem(ಗಾತ್ರ_2, new_ಗಾತ್ರ, &bmap->bitmap);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;
	}
	ಆದರೆ (new_ಗಾತ್ರ > ಗಾತ್ರ_2)
		memset(bmap->bitmap + ಗಾತ್ರ_2, 0, new_ಗಾತ್ರ - ಗಾತ್ರ_2);

	bmap->ಕೊನೆ = new_ಕೊನೆ;
	bmap->real_ಕೊನೆ = new_real_ಕೊನೆ;
	ಹಿಂತಿರುಗಿ 0;
}

errcode_t ext2fs_compare_generic_bitmap(errcode_t magic, errcode_t neq,
					ext2fs_generic_bitmap gen_bm1,
					ext2fs_generic_bitmap gen_bm2)
{
	ext2fs_generic_bitmap_32 bm1 = (ext2fs_generic_bitmap_32) gen_bm1;
	ext2fs_generic_bitmap_32 bm2 = (ext2fs_generic_bitmap_32) gen_bm2;
	blk_t	i;

	ಆದರೆ (!bm1 || bm1->magic != magic)
		ಹಿಂತಿರುಗಿ magic;
	ಆದರೆ (!bm2 || bm2->magic != magic)
		ಹಿಂತಿರುಗಿ magic;

	ಆದರೆ ((bm1->ಆರಂಭ != bm2->ಆರಂಭ) ||
	    (bm1->ಕೊನೆ != bm2->ಕೊನೆ) ||
	    (memcmp(bm1->bitmap, bm2->bitmap,
		    (size_t) (bm1->ಕೊನೆ - bm1->ಆರಂಭ)/8)))
		ಹಿಂತಿರುಗಿ neq;

	ಗಾಗಿ (i = bm1->ಕೊನೆ - ((bm1->ಕೊನೆ - bm1->ಆರಂಭ) % 8); i <= bm1->ಕೊನೆ; i++)
		ಆದರೆ (ext2fs_fast_test_ಖಂಡ_bitmap(gen_bm1, i) !=
		    ext2fs_fast_test_ಖಂಡ_bitmap(gen_bm2, i))
			ಹಿಂತಿರುಗಿ neq;

	ಹಿಂತಿರುಗಿ 0;
}

ಶೂನ್ಯ ext2fs_ಗಣ_generic_bitmap_padding(ext2fs_generic_bitmap gen_map)
{
	ext2fs_generic_bitmap_32 map = (ext2fs_generic_bitmap_32) gen_map;
	__u32	i, j;

	/* Protect loop from wrap-around if map->real_end is maxed */
	ಗಾಗಿ (i=map->ಕೊನೆ+1, j = i - map->ಆರಂಭ;
	     i <= map->real_ಕೊನೆ && i > map->ಕೊನೆ;
	     i++, j++)
		ext2fs_ಗಣ_ಬಿಟ್(j, map->bitmap);
}

errcode_t ext2fs_get_generic_bitmap_range(ext2fs_generic_bitmap gen_bmap,
					  errcode_t magic,
					  __u32 ಆರಂಭ, __u32 ಅಂಕೆ,
					  ಶೂನ್ಯ *ಹೊರಹೋಗು)
{
	ext2fs_generic_bitmap_32 bmap = (ext2fs_generic_bitmap_32) gen_bmap;

	ಆದರೆ (!bmap || (bmap->magic != magic))
		ಹಿಂತಿರುಗಿ magic;

	ಆದರೆ ((ಆರಂಭ < bmap->ಆರಂಭ) || (ಆರಂಭ+ಅಂಕೆ-1 > bmap->real_ಕೊನೆ))
		ಹಿಂತಿರುಗಿ EXT2_ET_INVALID_ವಾದ;

	ಸ್ಮರಣೆ_ನಕಲು(ಹೊರಹೋಗು, bmap->bitmap + ((ಆರಂಭ - bmap->ಆರಂಭ) >> 3), (ಅಂಕೆ+7) >> 3);
	ಹಿಂತಿರುಗಿ 0;
}

errcode_t ext2fs_ಗಣ_generic_bitmap_range(ext2fs_generic_bitmap gen_bmap,
					  errcode_t magic,
					  __u32 ಆರಂಭ, __u32 ಅಂಕೆ,
					  ಶೂನ್ಯ *ಒಳ)
{
	ext2fs_generic_bitmap_32 bmap = (ext2fs_generic_bitmap_32) gen_bmap;

	ಆದರೆ (!bmap || (bmap->magic != magic))
		ಹಿಂತಿರುಗಿ magic;

	ಆದರೆ ((ಆರಂಭ < bmap->ಆರಂಭ) || (ಆರಂಭ+ಅಂಕೆ-1 > bmap->real_ಕೊನೆ))
		ಹಿಂತಿರುಗಿ EXT2_ET_INVALID_ವಾದ;

	ಸ್ಮರಣೆ_ನಕಲು(bmap->bitmap + ((ಆರಂಭ - bmap->ಆರಂಭ) >> 3), ಒಳ, (ಅಂಕೆ+7) >> 3);
	ಹಿಂತಿರುಗಿ 0;
}

/*
 * Compare @mem to zero buffer by 256 bytes.
 * Return 1 if @mem is zeroed memory, otherwise return 0.
 */
ಪೂರ್ಣಾಂಕ ext2fs_mem_is_zero(ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *mem, size_t len)
{
	ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ zero_buf[256];

	ಆಗ (len >= ಗಾತ್ರ(zero_buf)) {
		ಆದರೆ (memcmp(mem, zero_buf, ಗಾತ್ರ(zero_buf)))
			ಹಿಂತಿರುಗಿ 0;
		len -= ಗಾತ್ರ(zero_buf);
		mem += ಗಾತ್ರ(zero_buf);
	}
	/* Deal with leftover bytes. */
	ಆದರೆ (len)
		ಹಿಂತಿರುಗಿ !memcmp(mem, zero_buf, len);
	ಹಿಂತಿರುಗಿ 1;
}

/*
 * Return true if all of the bits in a specified range are clear
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ext2fs_test_clear_generic_bitmap_range(ext2fs_generic_bitmap gen_bitmap,
						  ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಆರಂಭ,
						  ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ len)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	size_t ಆರಂಭ_byte, len_byte = len >> 3;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಆರಂಭ_ಬಿಟ್, len_ಬಿಟ್ = len % 8;
	ಪೂರ್ಣಾಂಕ ಮೊದಲ_ಬಿಟ್ = 0;
	ಪೂರ್ಣಾಂಕ ಕೊನೆಯ_ಬಿಟ್  = 0;
	ಪೂರ್ಣಾಂಕ mark_ಎಣಿಕೆ = 0;
	ಪೂರ್ಣಾಂಕ mark_ಬಿಟ್ = 0;
	ಪೂರ್ಣಾಂಕ i;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ADDR = bitmap->bitmap;

	ಆರಂಭ -= bitmap->ಆರಂಭ;
	ಆರಂಭ_byte = ಆರಂಭ >> 3;
	ಆರಂಭ_ಬಿಟ್ = ಆರಂಭ % 8;

	ಆದರೆ (ಆರಂಭ_ಬಿಟ್ != 0) {
		/*
		 * The compared start block number or start inode number
		 * is not the first bit in a byte.
		 */
		mark_ಎಣಿಕೆ = 8 - ಆರಂಭ_ಬಿಟ್;
		ಆದರೆ (len < 8 - ಆರಂಭ_ಬಿಟ್) {
			mark_ಎಣಿಕೆ = (ಪೂರ್ಣಾಂಕ)len;
			mark_ಬಿಟ್ = len + ಆರಂಭ_ಬಿಟ್ - 1;
		} ಇಲ್ಲದಿದ್ದರೆ
			mark_ಬಿಟ್ = 7;

		ಗಾಗಿ (i = mark_ಎಣಿಕೆ; i > 0; i--, mark_ಬಿಟ್--)
			ಮೊದಲ_ಬಿಟ್ |= 1 << mark_ಬಿಟ್;

		/*
		 * Compare blocks or inodes in the first byte.
		 * If there is any marked bit, this function returns 0.
		 */
		ಆದರೆ (ಮೊದಲ_ಬಿಟ್ & ADDR[ಆರಂಭ_byte])
			ಹಿಂತಿರುಗಿ 0;
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (len <= 8 - ಆರಂಭ_ಬಿಟ್)
			ಹಿಂತಿರುಗಿ 1;

		ಆರಂಭ_byte++;
		len_ಬಿಟ್ = (len - mark_ಎಣಿಕೆ) % 8;
		len_byte = (len - mark_ಎಣಿಕೆ) >> 3;
	}

	/*
	 * The compared start block number or start inode number is
	 * the first bit in a byte.
	 */
	ಆದರೆ (len_ಬಿಟ್ != 0) {
		/*
		 * The compared end block number or end inode number is
		 * not the last bit in a byte.
		 */
		ಗಾಗಿ (mark_ಬಿಟ್ = len_ಬಿಟ್ - 1; mark_ಬಿಟ್ >= 0; mark_ಬಿಟ್--)
			ಕೊನೆಯ_ಬಿಟ್ |= 1 << mark_ಬಿಟ್;

		/*
		 * Compare blocks or inodes in the last byte.
		 * If there is any marked bit, this function returns 0.
		 */
		ಆದರೆ (ಕೊನೆಯ_ಬಿಟ್ & ADDR[ಆರಂಭ_byte + len_byte])
			ಹಿಂತಿರುಗಿ 0;
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (len_byte == 0)
			ಹಿಂತಿರುಗಿ 1;
	}

	/* Check whether all bytes are 0 */
	ಹಿಂತಿರುಗಿ ext2fs_mem_is_zero(ADDR + ಆರಂಭ_byte, len_byte);
}

errcode_t ext2fs_ಕಂಡುಹಿಡಿ_ಮೊದಲ_zero_generic_bitmap(ext2fs_generic_bitmap gen_bitmap,
						__u32 ಆರಂಭ, __u32 ಕೊನೆ,
						__u32 *ಹೊರಹೋಗು)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	blk_t b;

	ಆದರೆ (ಆರಂಭ < bitmap->ಆರಂಭ || ಕೊನೆ > bitmap->ಕೊನೆ || ಆರಂಭ > ಕೊನೆ) {
		ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_TEST_ERROR, ಆರಂಭ);
		ಹಿಂತಿರುಗಿ EINVAL;
	}

	ಆಗ (ಆರಂಭ <= ಕೊನೆ) {
		b = ext2fs_test_ಬಿಟ್(ಆರಂಭ - bitmap->ಆರಂಭ, bitmap->bitmap);
		ಆದರೆ (!b) {
			*ಹೊರಹೋಗು = ಆರಂಭ;
			ಹಿಂತಿರುಗಿ 0;
		}
		ಆರಂಭ++;
	}

	ಹಿಂತಿರುಗಿ ENOENT;
}

errcode_t ext2fs_ಕಂಡುಹಿಡಿ_ಮೊದಲ_ಗಣ_generic_bitmap(ext2fs_generic_bitmap gen_bitmap,
					       __u32 ಆರಂಭ, __u32 ಕೊನೆ,
					       __u32 *ಹೊರಹೋಗು)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	blk_t b;

	ಆದರೆ (ಆರಂಭ < bitmap->ಆರಂಭ || ಕೊನೆ > bitmap->ಕೊನೆ || ಆರಂಭ > ಕೊನೆ) {
		ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_TEST_ERROR, ಆರಂಭ);
		ಹಿಂತಿರುಗಿ EINVAL;
	}

	ಆಗ (ಆರಂಭ <= ಕೊನೆ) {
		b = ext2fs_test_ಬಿಟ್(ಆರಂಭ - bitmap->ಆರಂಭ, bitmap->bitmap);
		ಆದರೆ (b) {
			*ಹೊರಹೋಗು = ಆರಂಭ;
			ಹಿಂತಿರುಗಿ 0;
		}
		ಆರಂಭ++;
	}

	ಹಿಂತಿರುಗಿ ENOENT;
}

ಪೂರ್ಣಾಂಕ ext2fs_test_ಖಂಡ_bitmap_range(ext2fs_ಖಂಡ_bitmap gen_bitmap,
				   blk_t ಖಂಡ, ಪೂರ್ಣಾಂಕ ಅಂಕೆ)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	EXT2_CHECK_MAGIC(bitmap, EXT2_ET_MAGIC_ಖಂಡ_BITMAP);
	ಆದರೆ ((ಖಂಡ < bitmap->ಆರಂಭ) || (ಖಂಡ > bitmap->real_ಕೊನೆ) ||
	    (ಖಂಡ+ಅಂಕೆ-1 > bitmap->real_ಕೊನೆ)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_ಖಂಡ_TEST,
				   ಖಂಡ, bitmap->description);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಹಿಂತಿರುಗಿ ext2fs_test_clear_generic_bitmap_range((ext2fs_generic_bitmap)
						      bitmap, ಖಂಡ, ಅಂಕೆ);
}

ಪೂರ್ಣಾಂಕ ext2fs_test_inode_bitmap_range(ext2fs_inode_bitmap gen_bitmap,
				   ext2_ino_t inode, ಪೂರ್ಣಾಂಕ ಅಂಕೆ)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	EXT2_CHECK_MAGIC(bitmap, EXT2_ET_MAGIC_INODE_BITMAP);
	ಆದರೆ ((inode < bitmap->ಆರಂಭ) || (inode > bitmap->real_ಕೊನೆ) ||
	    (inode+ಅಂಕೆ-1 > bitmap->real_ಕೊನೆ)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_INODE_TEST,
				   inode, bitmap->description);
		ಹಿಂತಿರುಗಿ 0;
	}
	ಹಿಂತಿರುಗಿ ext2fs_test_clear_generic_bitmap_range((ext2fs_generic_bitmap)
						      bitmap, inode, ಅಂಕೆ);
}

ಶೂನ್ಯ ext2fs_mark_ಖಂಡ_bitmap_range(ext2fs_ಖಂಡ_bitmap gen_bitmap,
				    blk_t ಖಂಡ, ಪೂರ್ಣಾಂಕ ಅಂಕೆ)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	ಪೂರ್ಣಾಂಕ	i;

	ಆದರೆ ((ಖಂಡ < bitmap->ಆರಂಭ) || (ಖಂಡ > bitmap->ಕೊನೆ) ||
	    (ಖಂಡ+ಅಂಕೆ-1 > bitmap->ಕೊನೆ)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_ಖಂಡ_MARK, ಖಂಡ,
				   bitmap->description);
		ಹಿಂತಿರುಗಿ;
	}
	ಗಾಗಿ (i=0; i < ಅಂಕೆ; i++)
		ext2fs_fast_ಗಣ_ಬಿಟ್(ಖಂಡ + i - bitmap->ಆರಂಭ, bitmap->bitmap);
}

ಶೂನ್ಯ ext2fs_unmark_ಖಂಡ_bitmap_range(ext2fs_ಖಂಡ_bitmap gen_bitmap,
				      blk_t ಖಂಡ, ಪೂರ್ಣಾಂಕ ಅಂಕೆ)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	ಪೂರ್ಣಾಂಕ	i;

	ಆದರೆ ((ಖಂಡ < bitmap->ಆರಂಭ) || (ಖಂಡ > bitmap->ಕೊನೆ) ||
	    (ಖಂಡ+ಅಂಕೆ-1 > bitmap->ಕೊನೆ)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_ಖಂಡ_UNMARK, ಖಂಡ,
				   bitmap->description);
		ಹಿಂತಿರುಗಿ;
	}
	ಗಾಗಿ (i=0; i < ಅಂಕೆ; i++)
		ext2fs_fast_clear_ಬಿಟ್(ಖಂಡ + i - bitmap->ಆರಂಭ,
				      bitmap->bitmap);
}

