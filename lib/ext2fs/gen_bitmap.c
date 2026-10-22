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


#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਜੇ HAVE_UNISTD_H
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <fcntl.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <time.h>
#ਜੇ HAVE_SYS_STAT_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_SYS_TYPES_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fsP.h"

ਬਣਤਰ ext2fs_struct_generic_bitmap_32 {
	errcode_t	magic;
	ext2_filsys 	fs;
	__u32		ਸ਼ੁਰੂ, ਸਿਰਾ;
	__u32		real_ਸਿਰਾ;
	ਅੱਖਰ	*	description;
	ਅੱਖਰ	*	bitmap;
	errcode_t	ਅਧਾਰ_error_ਕੋਡ;
	__u32		reserved[7];
};

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ext2fs_struct_generic_bitmap_32 *ext2fs_generic_bitmap_32;

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2FS_IS_32_BITMAP(bmap) \
	(((bmap)->magic == EXT2_ET_MAGIC_GENERIC_BITMAP) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_ਖੰਡ_BITMAP) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_INODE_BITMAP))

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ EXT2FS_IS_64_BITMAP(bmap) \
	(((bmap)->magic == EXT2_ET_MAGIC_GENERIC_BITMAP64) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_ਖੰਡ_BITMAP64) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_INODE_BITMAP64))

/*
 * Used by previously inlined function, so we have to export this and
 * not change the function signature
 */
ਖਾਲੀ ext2fs_warn_bitmap2(ext2fs_generic_bitmap gen_bitmap,
			    ਪੂਰਨ ਕੋਡ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ arg)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OMIT_COM_ERR
	ਜੇ (bitmap->description)
		com_err(0, bitmap->ਅਧਾਰ_error_ਕੋਡ+ਕੋਡ,
			"#%lu for %s", arg, bitmap->description);
	ਨਹੀਂਤਾਂ
		com_err(0, bitmap->ਅਧਾਰ_error_ਕੋਡ + ਕੋਡ, "#%lu", arg);
#ਜੇ_ਅੰਤ
}

ਸਥਿਰ errcode_t check_magic(ext2fs_generic_bitmap bitmap)
{
	ਜੇ (!bitmap || !((bitmap->magic == EXT2_ET_MAGIC_GENERIC_BITMAP) ||
			 (bitmap->magic == EXT2_ET_MAGIC_INODE_BITMAP) ||
			 (bitmap->magic == EXT2_ET_MAGIC_ਖੰਡ_BITMAP)))
		ਵਾਪਸ EXT2_ET_MAGIC_GENERIC_BITMAP;
	ਵਾਪਸ 0;
}

errcode_t ext2fs_make_generic_bitmap(errcode_t magic, ext2_filsys fs,
				     __u32 ਸ਼ੁਰੂ, __u32 ਸਿਰਾ, __u32 real_ਸਿਰਾ,
				     ਅਚਲ ਅੱਖਰ *descr, ਅੱਖਰ *init_map,
				     ext2fs_generic_bitmap *ret)
{
	ext2fs_generic_bitmap_32 bitmap;
	errcode_t		retval;
	size_t			ਮਾਪ;

	retval = ext2fs_get_mem(ਆਕਾਰ(ਬਣਤਰ ext2fs_struct_generic_bitmap_32),
				&bitmap);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	bitmap->magic = magic;
	bitmap->fs = fs;
	bitmap->ਸ਼ੁਰੂ = ਸ਼ੁਰੂ;
	bitmap->ਸਿਰਾ = ਸਿਰਾ;
	bitmap->real_ਸਿਰਾ = real_ਸਿਰਾ;
	ਚੋਣ (magic) {
	ਹਾਲਤ EXT2_ET_MAGIC_INODE_BITMAP:
		bitmap->ਅਧਾਰ_error_ਕੋਡ = EXT2_ET_BAD_INODE_MARK;
		ਰੋਕੋ;
	ਹਾਲਤ EXT2_ET_MAGIC_ਖੰਡ_BITMAP:
		bitmap->ਅਧਾਰ_error_ਕੋਡ = EXT2_ET_BAD_ਖੰਡ_MARK;
		ਰੋਕੋ;
	ਮੂਲ:
		bitmap->ਅਧਾਰ_error_ਕੋਡ = EXT2_ET_BAD_GENERIC_MARK;
	}
	ਜੇ (descr) {
		retval = ext2fs_get_mem(ਲੜੀ_ਲੰਬਾਈ(descr)+1, &bitmap->description);
		ਜੇ (retval) {
			ext2fs_free_mem(&bitmap);
			ਵਾਪਸ retval;
		}
		strcpy(bitmap->description, descr);
	} ਨਹੀਂਤਾਂ
		bitmap->description = 0;

	ਮਾਪ = (size_t) (((bitmap->real_ਸਿਰਾ - bitmap->ਸ਼ੁਰੂ) / 8) + 1);
	/* Round up to allow for the BT x86 instruction */
	ਮਾਪ = (ਮਾਪ + 7) & ~3;
	retval = ext2fs_get_mem(ਮਾਪ, &bitmap->bitmap);
	ਜੇ (retval) {
		ext2fs_free_mem(&bitmap->description);
		ext2fs_free_mem(&bitmap);
		ਵਾਪਸ retval;
	}

	ਜੇ (init_map)
		ਸਮ੍ਰਿਤੀ_ਨਕਲ(bitmap->bitmap, init_map, ਮਾਪ);
	ਨਹੀਂਤਾਂ
		memset(bitmap->bitmap, 0, ਮਾਪ);
	*ret = (ext2fs_generic_bitmap) bitmap;
	ਵਾਪਸ 0;
}

errcode_t ext2fs_allocate_generic_bitmap(__u32 ਸ਼ੁਰੂ,
					 __u32 ਸਿਰਾ,
					 __u32 real_ਸਿਰਾ,
					 ਅਚਲ ਅੱਖਰ *descr,
					 ext2fs_generic_bitmap *ret)
{
	ਵਾਪਸ ext2fs_make_generic_bitmap(EXT2_ET_MAGIC_GENERIC_BITMAP, 0,
					  ਸ਼ੁਰੂ, ਸਿਰਾ, real_ਸਿਰਾ, descr, 0, ret);
}

errcode_t ext2fs_copy_generic_bitmap(ext2fs_generic_bitmap gen_src,
				     ext2fs_generic_bitmap *dest)
{
	ext2fs_generic_bitmap_32 src = (ext2fs_generic_bitmap_32) gen_src;

	ਵਾਪਸ (ext2fs_make_generic_bitmap(src->magic, src->fs,
					   src->ਸ਼ੁਰੂ, src->ਸਿਰਾ,
					   src->real_ਸਿਰਾ,
					   src->description, src->bitmap,
					   dest));
}

ਖਾਲੀ ext2fs_free_generic_bitmap(ext2fs_inode_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	ਜੇ (check_magic(gen_bitmap))
		ਵਾਪਸ;

	bitmap->magic = 0;
	ਜੇ (bitmap->description) {
		ext2fs_free_mem(&bitmap->description);
		bitmap->description = 0;
	}
	ਜੇ (bitmap->bitmap) {
		ext2fs_free_mem(&bitmap->bitmap);
		bitmap->bitmap = 0;
	}
	ext2fs_free_mem(&bitmap);
}

ਪੂਰਨ ext2fs_test_generic_bitmap(ext2fs_generic_bitmap bitmap,
					blk_t bitno)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	ਜੇ (!EXT2FS_IS_32_BITMAP(bitmap)) {
		ਜੇ (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			ਵਾਪਸ ext2fs_test_generic_bmap(bitmap, bitno);
		}
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"test_bitmap(%lu)", (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ) bitno);
#ਜੇ_ਅੰਤ
		ਵਾਪਸ 0;
	}

	ਜੇ ((bitno < bitmap32->ਸ਼ੁਰੂ) || (bitno > bitmap32->ਸਿਰਾ)) {
		ext2fs_warn_bitmap2(bitmap, EXT2FS_TEST_ERROR, bitno);
		ਵਾਪਸ 0;
	}
	ਵਾਪਸ ext2fs_test_ਦੋਆਧਾਰੀ_ਅੰਕ(bitno - bitmap32->ਸ਼ੁਰੂ, bitmap32->bitmap);
}

ਪੂਰਨ ext2fs_mark_generic_bitmap(ext2fs_generic_bitmap bitmap,
					 __u32 bitno)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	ਜੇ (!EXT2FS_IS_32_BITMAP(bitmap)) {
		ਜੇ (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			ਵਾਪਸ ext2fs_mark_generic_bmap(bitmap, bitno);
		}
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"mark_bitmap(%lu)", (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ) bitno);
#ਜੇ_ਅੰਤ
		ਵਾਪਸ 0;
	}

	ਜੇ ((bitno < bitmap32->ਸ਼ੁਰੂ) || (bitno > bitmap32->ਸਿਰਾ)) {
		ext2fs_warn_bitmap2(bitmap, EXT2FS_MARK_ERROR, bitno);
		ਵਾਪਸ 0;
	}
	ਵਾਪਸ ext2fs_ਸੈੱਟ_ਦੋਆਧਾਰੀ_ਅੰਕ(bitno - bitmap32->ਸ਼ੁਰੂ, bitmap32->bitmap);
}

ਪੂਰਨ ext2fs_unmark_generic_bitmap(ext2fs_generic_bitmap bitmap,
					   blk_t bitno)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	ਜੇ (!EXT2FS_IS_32_BITMAP(bitmap)) {
		ਜੇ (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			ਵਾਪਸ ext2fs_unmark_generic_bmap(bitmap, bitno);
		}
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"mark_bitmap(%lu)", (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ) bitno);
#ਜੇ_ਅੰਤ
		ਵਾਪਸ 0;
	}

	ਜੇ ((bitno < bitmap32->ਸ਼ੁਰੂ) || (bitno > bitmap32->ਸਿਰਾ)) {
		ext2fs_warn_bitmap2(bitmap, EXT2FS_UNMARK_ERROR, bitno);
		ਵਾਪਸ 0;
	}
	ਵਾਪਸ ext2fs_clear_ਦੋਆਧਾਰੀ_ਅੰਕ(bitno - bitmap32->ਸ਼ੁਰੂ, bitmap32->bitmap);
}

__u32 ext2fs_get_generic_bitmap_ਸ਼ੁਰੂ(ext2fs_generic_bitmap bitmap)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	ਜੇ (!EXT2FS_IS_32_BITMAP(bitmap)) {
		ਜੇ (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			ਵਾਪਸ ext2fs_get_generic_bmap_ਸ਼ੁਰੂ(bitmap);
		}
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"get_bitmap_start");
#ਜੇ_ਅੰਤ
		ਵਾਪਸ 0;
	}

	ਵਾਪਸ bitmap32->ਸ਼ੁਰੂ;
}

__u32 ext2fs_get_generic_bitmap_ਸਿਰਾ(ext2fs_generic_bitmap bitmap)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	ਜੇ (!EXT2FS_IS_32_BITMAP(bitmap)) {
		ਜੇ (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			ਵਾਪਸ ext2fs_get_generic_bmap_ਸਿਰਾ(bitmap);
		}
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"get_bitmap_end");
#ਜੇ_ਅੰਤ
		ਵਾਪਸ 0;
	}
	ਵਾਪਸ bitmap32->ਸਿਰਾ;
}

ਖਾਲੀ ext2fs_clear_generic_bitmap(ext2fs_generic_bitmap bitmap)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	ਜੇ (!EXT2FS_IS_32_BITMAP(bitmap)) {
		ਜੇ (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			ext2fs_clear_generic_bmap(bitmap);
			ਵਾਪਸ;
		}
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"clear_generic_bitmap");
#ਜੇ_ਅੰਤ
		ਵਾਪਸ;
	}

	memset(bitmap32->bitmap, 0,
	       (size_t) (((bitmap32->real_ਸਿਰਾ - bitmap32->ਸ਼ੁਰੂ) / 8) + 1));
}

errcode_t ext2fs_fudge_generic_bitmap_ਸਿਰਾ(ext2fs_inode_bitmap gen_bitmap,
					  errcode_t magic, errcode_t neq,
					  ext2_ino_t ਸਿਰਾ, ext2_ino_t *oend)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	EXT2_CHECK_MAGIC(bitmap, magic);

	ਜੇ (ਸਿਰਾ > bitmap->real_ਸਿਰਾ)
		ਵਾਪਸ neq;
	ਜੇ (oend)
		*oend = bitmap->ਸਿਰਾ;
	bitmap->ਸਿਰਾ = ਸਿਰਾ;
	ਵਾਪਸ 0;
}

errcode_t ext2fs_resize_generic_bitmap(errcode_t magic,
				       __u32 new_ਸਿਰਾ, __u32 new_real_ਸਿਰਾ,
				       ext2fs_generic_bitmap gen_bmap)
{
	ext2fs_generic_bitmap_32 bmap = (ext2fs_generic_bitmap_32) gen_bmap;
	errcode_t	retval;
	size_t		ਮਾਪ, new_ਮਾਪ;
	__u32		bitno;

	ਜੇ (!bmap || (bmap->magic != magic))
		ਵਾਪਸ magic;

	/*
	 * If we're expanding the bitmap, make sure all of the new
	 * parts of the bitmap are zero.
	 */
	ਜੇ (new_ਸਿਰਾ > bmap->ਸਿਰਾ) {
		bitno = bmap->real_ਸਿਰਾ;
		ਜੇ (bitno > new_ਸਿਰਾ)
			bitno = new_ਸਿਰਾ;
		ਲਈ (; bitno > bmap->ਸਿਰਾ; bitno--)
			ext2fs_clear_ਦੋਆਧਾਰੀ_ਅੰਕ(bitno - bmap->ਸ਼ੁਰੂ, bmap->bitmap);
	}
	ਜੇ (new_real_ਸਿਰਾ == bmap->real_ਸਿਰਾ) {
		bmap->ਸਿਰਾ = new_ਸਿਰਾ;
		ਵਾਪਸ 0;
	}

	ਮਾਪ = ((bmap->real_ਸਿਰਾ - bmap->ਸ਼ੁਰੂ) / 8) + 1;
	new_ਮਾਪ = ((new_real_ਸਿਰਾ - bmap->ਸ਼ੁਰੂ) / 8) + 1;

	ਜੇ (ਮਾਪ != new_ਮਾਪ) {
		retval = ext2fs_resize_mem(ਮਾਪ, new_ਮਾਪ, &bmap->bitmap);
		ਜੇ (retval)
			ਵਾਪਸ retval;
	}
	ਜੇ (new_ਮਾਪ > ਮਾਪ)
		memset(bmap->bitmap + ਮਾਪ, 0, new_ਮਾਪ - ਮਾਪ);

	bmap->ਸਿਰਾ = new_ਸਿਰਾ;
	bmap->real_ਸਿਰਾ = new_real_ਸਿਰਾ;
	ਵਾਪਸ 0;
}

errcode_t ext2fs_compare_generic_bitmap(errcode_t magic, errcode_t neq,
					ext2fs_generic_bitmap gen_bm1,
					ext2fs_generic_bitmap gen_bm2)
{
	ext2fs_generic_bitmap_32 bm1 = (ext2fs_generic_bitmap_32) gen_bm1;
	ext2fs_generic_bitmap_32 bm2 = (ext2fs_generic_bitmap_32) gen_bm2;
	blk_t	i;

	ਜੇ (!bm1 || bm1->magic != magic)
		ਵਾਪਸ magic;
	ਜੇ (!bm2 || bm2->magic != magic)
		ਵਾਪਸ magic;

	ਜੇ ((bm1->ਸ਼ੁਰੂ != bm2->ਸ਼ੁਰੂ) ||
	    (bm1->ਸਿਰਾ != bm2->ਸਿਰਾ) ||
	    (memcmp(bm1->bitmap, bm2->bitmap,
		    (size_t) (bm1->ਸਿਰਾ - bm1->ਸ਼ੁਰੂ)/8)))
		ਵਾਪਸ neq;

	ਲਈ (i = bm1->ਸਿਰਾ - ((bm1->ਸਿਰਾ - bm1->ਸ਼ੁਰੂ) % 8); i <= bm1->ਸਿਰਾ; i++)
		ਜੇ (ext2fs_fast_test_ਖੰਡ_bitmap(gen_bm1, i) !=
		    ext2fs_fast_test_ਖੰਡ_bitmap(gen_bm2, i))
			ਵਾਪਸ neq;

	ਵਾਪਸ 0;
}

ਖਾਲੀ ext2fs_ਸੈੱਟ_generic_bitmap_padding(ext2fs_generic_bitmap gen_map)
{
	ext2fs_generic_bitmap_32 map = (ext2fs_generic_bitmap_32) gen_map;
	__u32	i, j;

	/* Protect loop from wrap-around if map->real_end is maxed */
	ਲਈ (i=map->ਸਿਰਾ+1, j = i - map->ਸ਼ੁਰੂ;
	     i <= map->real_ਸਿਰਾ && i > map->ਸਿਰਾ;
	     i++, j++)
		ext2fs_ਸੈੱਟ_ਦੋਆਧਾਰੀ_ਅੰਕ(j, map->bitmap);
}

errcode_t ext2fs_get_generic_bitmap_range(ext2fs_generic_bitmap gen_bmap,
					  errcode_t magic,
					  __u32 ਸ਼ੁਰੂ, __u32 ਅੰਕ,
					  ਖਾਲੀ *ਬਾਹਰਲਾ)
{
	ext2fs_generic_bitmap_32 bmap = (ext2fs_generic_bitmap_32) gen_bmap;

	ਜੇ (!bmap || (bmap->magic != magic))
		ਵਾਪਸ magic;

	ਜੇ ((ਸ਼ੁਰੂ < bmap->ਸ਼ੁਰੂ) || (ਸ਼ੁਰੂ+ਅੰਕ-1 > bmap->real_ਸਿਰਾ))
		ਵਾਪਸ EXT2_ET_INVALID_ਦਲੀਲ;

	ਸਮ੍ਰਿਤੀ_ਨਕਲ(ਬਾਹਰਲਾ, bmap->bitmap + ((ਸ਼ੁਰੂ - bmap->ਸ਼ੁਰੂ) >> 3), (ਅੰਕ+7) >> 3);
	ਵਾਪਸ 0;
}

errcode_t ext2fs_ਸੈੱਟ_generic_bitmap_range(ext2fs_generic_bitmap gen_bmap,
					  errcode_t magic,
					  __u32 ਸ਼ੁਰੂ, __u32 ਅੰਕ,
					  ਖਾਲੀ *ਵਿੱਚ)
{
	ext2fs_generic_bitmap_32 bmap = (ext2fs_generic_bitmap_32) gen_bmap;

	ਜੇ (!bmap || (bmap->magic != magic))
		ਵਾਪਸ magic;

	ਜੇ ((ਸ਼ੁਰੂ < bmap->ਸ਼ੁਰੂ) || (ਸ਼ੁਰੂ+ਅੰਕ-1 > bmap->real_ਸਿਰਾ))
		ਵਾਪਸ EXT2_ET_INVALID_ਦਲੀਲ;

	ਸਮ੍ਰਿਤੀ_ਨਕਲ(bmap->bitmap + ((ਸ਼ੁਰੂ - bmap->ਸ਼ੁਰੂ) >> 3), ਵਿੱਚ, (ਅੰਕ+7) >> 3);
	ਵਾਪਸ 0;
}

/*
 * Compare @mem to zero buffer by 256 bytes.
 * Return 1 if @mem is zeroed memory, otherwise return 0.
 */
ਪੂਰਨ ext2fs_mem_is_zero(ਅਚਲ ਅੱਖਰ *mem, size_t len)
{
	ਸਥਿਰ ਅਚਲ ਅੱਖਰ zero_buf[256];

	ਜਦੋਂ (len >= ਆਕਾਰ(zero_buf)) {
		ਜੇ (memcmp(mem, zero_buf, ਆਕਾਰ(zero_buf)))
			ਵਾਪਸ 0;
		len -= ਆਕਾਰ(zero_buf);
		mem += ਆਕਾਰ(zero_buf);
	}
	/* Deal with leftover bytes. */
	ਜੇ (len)
		ਵਾਪਸ !memcmp(mem, zero_buf, len);
	ਵਾਪਸ 1;
}

/*
 * Return true if all of the bits in a specified range are clear
 */
ਸਥਿਰ ਪੂਰਨ ext2fs_test_clear_generic_bitmap_range(ext2fs_generic_bitmap gen_bitmap,
						  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ,
						  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ len)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	size_t ਸ਼ੁਰੂ_byte, len_byte = len >> 3;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ_ਦੋਆਧਾਰੀ_ਅੰਕ, len_ਦੋਆਧਾਰੀ_ਅੰਕ = len % 8;
	ਪੂਰਨ ਪਹਿਲਾ_ਦੋਆਧਾਰੀ_ਅੰਕ = 0;
	ਪੂਰਨ ਆਖਰੀ_ਦੋਆਧਾਰੀ_ਅੰਕ  = 0;
	ਪੂਰਨ mark_ਗਿਣਤੀ = 0;
	ਪੂਰਨ mark_ਦੋਆਧਾਰੀ_ਅੰਕ = 0;
	ਪੂਰਨ i;
	ਅਚਲ ਅੱਖਰ *ADDR = bitmap->bitmap;

	ਸ਼ੁਰੂ -= bitmap->ਸ਼ੁਰੂ;
	ਸ਼ੁਰੂ_byte = ਸ਼ੁਰੂ >> 3;
	ਸ਼ੁਰੂ_ਦੋਆਧਾਰੀ_ਅੰਕ = ਸ਼ੁਰੂ % 8;

	ਜੇ (ਸ਼ੁਰੂ_ਦੋਆਧਾਰੀ_ਅੰਕ != 0) {
		/*
		 * The compared start block number or start inode number
		 * is not the first bit in a byte.
		 */
		mark_ਗਿਣਤੀ = 8 - ਸ਼ੁਰੂ_ਦੋਆਧਾਰੀ_ਅੰਕ;
		ਜੇ (len < 8 - ਸ਼ੁਰੂ_ਦੋਆਧਾਰੀ_ਅੰਕ) {
			mark_ਗਿਣਤੀ = (ਪੂਰਨ)len;
			mark_ਦੋਆਧਾਰੀ_ਅੰਕ = len + ਸ਼ੁਰੂ_ਦੋਆਧਾਰੀ_ਅੰਕ - 1;
		} ਨਹੀਂਤਾਂ
			mark_ਦੋਆਧਾਰੀ_ਅੰਕ = 7;

		ਲਈ (i = mark_ਗਿਣਤੀ; i > 0; i--, mark_ਦੋਆਧਾਰੀ_ਅੰਕ--)
			ਪਹਿਲਾ_ਦੋਆਧਾਰੀ_ਅੰਕ |= 1 << mark_ਦੋਆਧਾਰੀ_ਅੰਕ;

		/*
		 * Compare blocks or inodes in the first byte.
		 * If there is any marked bit, this function returns 0.
		 */
		ਜੇ (ਪਹਿਲਾ_ਦੋਆਧਾਰੀ_ਅੰਕ & ADDR[ਸ਼ੁਰੂ_byte])
			ਵਾਪਸ 0;
		ਨਹੀਂਤਾਂ ਜੇ (len <= 8 - ਸ਼ੁਰੂ_ਦੋਆਧਾਰੀ_ਅੰਕ)
			ਵਾਪਸ 1;

		ਸ਼ੁਰੂ_byte++;
		len_ਦੋਆਧਾਰੀ_ਅੰਕ = (len - mark_ਗਿਣਤੀ) % 8;
		len_byte = (len - mark_ਗਿਣਤੀ) >> 3;
	}

	/*
	 * The compared start block number or start inode number is
	 * the first bit in a byte.
	 */
	ਜੇ (len_ਦੋਆਧਾਰੀ_ਅੰਕ != 0) {
		/*
		 * The compared end block number or end inode number is
		 * not the last bit in a byte.
		 */
		ਲਈ (mark_ਦੋਆਧਾਰੀ_ਅੰਕ = len_ਦੋਆਧਾਰੀ_ਅੰਕ - 1; mark_ਦੋਆਧਾਰੀ_ਅੰਕ >= 0; mark_ਦੋਆਧਾਰੀ_ਅੰਕ--)
			ਆਖਰੀ_ਦੋਆਧਾਰੀ_ਅੰਕ |= 1 << mark_ਦੋਆਧਾਰੀ_ਅੰਕ;

		/*
		 * Compare blocks or inodes in the last byte.
		 * If there is any marked bit, this function returns 0.
		 */
		ਜੇ (ਆਖਰੀ_ਦੋਆਧਾਰੀ_ਅੰਕ & ADDR[ਸ਼ੁਰੂ_byte + len_byte])
			ਵਾਪਸ 0;
		ਨਹੀਂਤਾਂ ਜੇ (len_byte == 0)
			ਵਾਪਸ 1;
	}

	/* Check whether all bytes are 0 */
	ਵਾਪਸ ext2fs_mem_is_zero(ADDR + ਸ਼ੁਰੂ_byte, len_byte);
}

errcode_t ext2fs_ਲੱਭੋ_ਪਹਿਲਾ_zero_generic_bitmap(ext2fs_generic_bitmap gen_bitmap,
						__u32 ਸ਼ੁਰੂ, __u32 ਸਿਰਾ,
						__u32 *ਬਾਹਰਲਾ)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	blk_t b;

	ਜੇ (ਸ਼ੁਰੂ < bitmap->ਸ਼ੁਰੂ || ਸਿਰਾ > bitmap->ਸਿਰਾ || ਸ਼ੁਰੂ > ਸਿਰਾ) {
		ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_TEST_ERROR, ਸ਼ੁਰੂ);
		ਵਾਪਸ EINVAL;
	}

	ਜਦੋਂ (ਸ਼ੁਰੂ <= ਸਿਰਾ) {
		b = ext2fs_test_ਦੋਆਧਾਰੀ_ਅੰਕ(ਸ਼ੁਰੂ - bitmap->ਸ਼ੁਰੂ, bitmap->bitmap);
		ਜੇ (!b) {
			*ਬਾਹਰਲਾ = ਸ਼ੁਰੂ;
			ਵਾਪਸ 0;
		}
		ਸ਼ੁਰੂ++;
	}

	ਵਾਪਸ ENOENT;
}

errcode_t ext2fs_ਲੱਭੋ_ਪਹਿਲਾ_ਸੈੱਟ_generic_bitmap(ext2fs_generic_bitmap gen_bitmap,
					       __u32 ਸ਼ੁਰੂ, __u32 ਸਿਰਾ,
					       __u32 *ਬਾਹਰਲਾ)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	blk_t b;

	ਜੇ (ਸ਼ੁਰੂ < bitmap->ਸ਼ੁਰੂ || ਸਿਰਾ > bitmap->ਸਿਰਾ || ਸ਼ੁਰੂ > ਸਿਰਾ) {
		ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_TEST_ERROR, ਸ਼ੁਰੂ);
		ਵਾਪਸ EINVAL;
	}

	ਜਦੋਂ (ਸ਼ੁਰੂ <= ਸਿਰਾ) {
		b = ext2fs_test_ਦੋਆਧਾਰੀ_ਅੰਕ(ਸ਼ੁਰੂ - bitmap->ਸ਼ੁਰੂ, bitmap->bitmap);
		ਜੇ (b) {
			*ਬਾਹਰਲਾ = ਸ਼ੁਰੂ;
			ਵਾਪਸ 0;
		}
		ਸ਼ੁਰੂ++;
	}

	ਵਾਪਸ ENOENT;
}

ਪੂਰਨ ext2fs_test_ਖੰਡ_bitmap_range(ext2fs_ਖੰਡ_bitmap gen_bitmap,
				   blk_t ਖੰਡ, ਪੂਰਨ ਅੰਕ)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	EXT2_CHECK_MAGIC(bitmap, EXT2_ET_MAGIC_ਖੰਡ_BITMAP);
	ਜੇ ((ਖੰਡ < bitmap->ਸ਼ੁਰੂ) || (ਖੰਡ > bitmap->real_ਸਿਰਾ) ||
	    (ਖੰਡ+ਅੰਕ-1 > bitmap->real_ਸਿਰਾ)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_ਖੰਡ_TEST,
				   ਖੰਡ, bitmap->description);
		ਵਾਪਸ 0;
	}
	ਵਾਪਸ ext2fs_test_clear_generic_bitmap_range((ext2fs_generic_bitmap)
						      bitmap, ਖੰਡ, ਅੰਕ);
}

ਪੂਰਨ ext2fs_test_inode_bitmap_range(ext2fs_inode_bitmap gen_bitmap,
				   ext2_ino_t inode, ਪੂਰਨ ਅੰਕ)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	EXT2_CHECK_MAGIC(bitmap, EXT2_ET_MAGIC_INODE_BITMAP);
	ਜੇ ((inode < bitmap->ਸ਼ੁਰੂ) || (inode > bitmap->real_ਸਿਰਾ) ||
	    (inode+ਅੰਕ-1 > bitmap->real_ਸਿਰਾ)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_INODE_TEST,
				   inode, bitmap->description);
		ਵਾਪਸ 0;
	}
	ਵਾਪਸ ext2fs_test_clear_generic_bitmap_range((ext2fs_generic_bitmap)
						      bitmap, inode, ਅੰਕ);
}

ਖਾਲੀ ext2fs_mark_ਖੰਡ_bitmap_range(ext2fs_ਖੰਡ_bitmap gen_bitmap,
				    blk_t ਖੰਡ, ਪੂਰਨ ਅੰਕ)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	ਪੂਰਨ	i;

	ਜੇ ((ਖੰਡ < bitmap->ਸ਼ੁਰੂ) || (ਖੰਡ > bitmap->ਸਿਰਾ) ||
	    (ਖੰਡ+ਅੰਕ-1 > bitmap->ਸਿਰਾ)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_ਖੰਡ_MARK, ਖੰਡ,
				   bitmap->description);
		ਵਾਪਸ;
	}
	ਲਈ (i=0; i < ਅੰਕ; i++)
		ext2fs_fast_ਸੈੱਟ_ਦੋਆਧਾਰੀ_ਅੰਕ(ਖੰਡ + i - bitmap->ਸ਼ੁਰੂ, bitmap->bitmap);
}

ਖਾਲੀ ext2fs_unmark_ਖੰਡ_bitmap_range(ext2fs_ਖੰਡ_bitmap gen_bitmap,
				      blk_t ਖੰਡ, ਪੂਰਨ ਅੰਕ)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	ਪੂਰਨ	i;

	ਜੇ ((ਖੰਡ < bitmap->ਸ਼ੁਰੂ) || (ਖੰਡ > bitmap->ਸਿਰਾ) ||
	    (ਖੰਡ+ਅੰਕ-1 > bitmap->ਸਿਰਾ)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_ਖੰਡ_UNMARK, ਖੰਡ,
				   bitmap->description);
		ਵਾਪਸ;
	}
	ਲਈ (i=0; i < ਅੰਕ; i++)
		ext2fs_fast_clear_ਦੋਆਧਾਰੀ_ਅੰਕ(ਖੰਡ + i - bitmap->ਸ਼ੁਰੂ,
				      bitmap->bitmap);
}

