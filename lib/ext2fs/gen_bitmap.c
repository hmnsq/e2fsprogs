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
#ಯದಿ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#endif
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <time.h>
#ಯದಿ HAVE_SYS_STAT_H
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#endif
#ಯದಿ HAVE_SYS_TYPES_H
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#endif

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fsP.h"

ಸಂರಚನಾ ext2fs_struct_generic_bitmap_32 {
	errcode_t	magic;
	ext2_filsys 	fs;
	__u32		start, end;
	__u32		real_end;
	ವರ್ಣ	*	description;
	ವರ್ಣ	*	bitmap;
	errcode_t	base_error_code;
	__u32		reserved[7];
};

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ ext2fs_struct_generic_bitmap_32 *ext2fs_generic_bitmap_32;

#define EXT2FS_IS_32_BITMAP(bmap) \
	(((bmap)->magic == EXT2_ET_MAGIC_GENERIC_BITMAP) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_BLOCK_BITMAP) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_INODE_BITMAP))

#define EXT2FS_IS_64_BITMAP(bmap) \
	(((bmap)->magic == EXT2_ET_MAGIC_GENERIC_BITMAP64) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_BLOCK_BITMAP64) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_INODE_BITMAP64))

/*
 * Used by previously inlined function, so we have to export this and
 * not change the function signature
 */
ಶೂನ್ಯ ext2fs_warn_bitmap2(ext2fs_generic_bitmap gen_bitmap,
			    ಪೂರ್ಣಾಙ್ಕ code, unsigned long arg)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

#ifndef OMIT_COM_ERR
	ಯದಿ (bitmap->description)
		com_err(0, bitmap->base_error_code+code,
			"#%lu for %s", arg, bitmap->description);
	ಅನ್ಯಥಾ
		com_err(0, bitmap->base_error_code + code, "#%lu", arg);
#endif
}

ಸ್ಥಿರ errcode_t check_magic(ext2fs_generic_bitmap bitmap)
{
	ಯದಿ (!bitmap || !((bitmap->magic == EXT2_ET_MAGIC_GENERIC_BITMAP) ||
			 (bitmap->magic == EXT2_ET_MAGIC_INODE_BITMAP) ||
			 (bitmap->magic == EXT2_ET_MAGIC_BLOCK_BITMAP)))
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_MAGIC_GENERIC_BITMAP;
	ಪ್ರತ್ಯಾಗಮ 0;
}

errcode_t ext2fs_make_generic_bitmap(errcode_t magic, ext2_filsys fs,
				     __u32 start, __u32 end, __u32 real_end,
				     ಧ್ರುವ ವರ್ಣ *descr, ವರ್ಣ *init_map,
				     ext2fs_generic_bitmap *ret)
{
	ext2fs_generic_bitmap_32 bitmap;
	errcode_t		retval;
	size_t			size;

	retval = ext2fs_get_mem(ಪರಿಮಾಣ(ಸಂರಚನಾ ext2fs_struct_generic_bitmap_32),
				&bitmap);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;

	bitmap->magic = magic;
	bitmap->fs = fs;
	bitmap->start = start;
	bitmap->end = end;
	bitmap->real_end = real_end;
	ವಿಕಲ್ಪ (magic) {
	ಪಕ್ಷ EXT2_ET_MAGIC_INODE_BITMAP:
		bitmap->base_error_code = EXT2_ET_BAD_INODE_MARK;
		ಭಙ್ಗ;
	ಪಕ್ಷ EXT2_ET_MAGIC_BLOCK_BITMAP:
		bitmap->base_error_code = EXT2_ET_BAD_BLOCK_MARK;
		ಭಙ್ಗ;
	ಸ್ವಭಾವ:
		bitmap->base_error_code = EXT2_ET_BAD_GENERIC_MARK;
	}
	ಯದಿ (descr) {
		retval = ext2fs_get_mem(strlen(descr)+1, &bitmap->description);
		ಯದಿ (retval) {
			ext2fs_free_mem(&bitmap);
			ಪ್ರತ್ಯಾಗಮ retval;
		}
		strcpy(bitmap->description, descr);
	} ಅನ್ಯಥಾ
		bitmap->description = 0;

	size = (size_t) (((bitmap->real_end - bitmap->start) / 8) + 1);
	/* Round up to allow for the BT x86 instruction */
	size = (size + 7) & ~3;
	retval = ext2fs_get_mem(size, &bitmap->bitmap);
	ಯದಿ (retval) {
		ext2fs_free_mem(&bitmap->description);
		ext2fs_free_mem(&bitmap);
		ಪ್ರತ್ಯಾಗಮ retval;
	}

	ಯದಿ (init_map)
		memcpy(bitmap->bitmap, init_map, size);
	ಅನ್ಯಥಾ
		memset(bitmap->bitmap, 0, size);
	*ret = (ext2fs_generic_bitmap) bitmap;
	ಪ್ರತ್ಯಾಗಮ 0;
}

errcode_t ext2fs_allocate_generic_bitmap(__u32 start,
					 __u32 end,
					 __u32 real_end,
					 ಧ್ರುವ ವರ್ಣ *descr,
					 ext2fs_generic_bitmap *ret)
{
	ಪ್ರತ್ಯಾಗಮ ext2fs_make_generic_bitmap(EXT2_ET_MAGIC_GENERIC_BITMAP, 0,
					  start, end, real_end, descr, 0, ret);
}

errcode_t ext2fs_copy_generic_bitmap(ext2fs_generic_bitmap gen_src,
				     ext2fs_generic_bitmap *dest)
{
	ext2fs_generic_bitmap_32 src = (ext2fs_generic_bitmap_32) gen_src;

	ಪ್ರತ್ಯಾಗಮ (ext2fs_make_generic_bitmap(src->magic, src->fs,
					   src->start, src->end,
					   src->real_end,
					   src->description, src->bitmap,
					   dest));
}

ಶೂನ್ಯ ext2fs_free_generic_bitmap(ext2fs_inode_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	ಯದಿ (check_magic(gen_bitmap))
		ಪ್ರತ್ಯಾಗಮ;

	bitmap->magic = 0;
	ಯದಿ (bitmap->description) {
		ext2fs_free_mem(&bitmap->description);
		bitmap->description = 0;
	}
	ಯದಿ (bitmap->bitmap) {
		ext2fs_free_mem(&bitmap->bitmap);
		bitmap->bitmap = 0;
	}
	ext2fs_free_mem(&bitmap);
}

ಪೂರ್ಣಾಙ್ಕ ext2fs_test_generic_bitmap(ext2fs_generic_bitmap bitmap,
					blk_t bitno)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	ಯದಿ (!EXT2FS_IS_32_BITMAP(bitmap)) {
		ಯದಿ (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			ಪ್ರತ್ಯಾಗಮ ext2fs_test_generic_bmap(bitmap, bitno);
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"test_bitmap(%lu)", (unsigned long) bitno);
#endif
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ ((bitno < bitmap32->start) || (bitno > bitmap32->end)) {
		ext2fs_warn_bitmap2(bitmap, EXT2FS_TEST_ERROR, bitno);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಪ್ರತ್ಯಾಗಮ ext2fs_test_bit(bitno - bitmap32->start, bitmap32->bitmap);
}

ಪೂರ್ಣಾಙ್ಕ ext2fs_mark_generic_bitmap(ext2fs_generic_bitmap bitmap,
					 __u32 bitno)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	ಯದಿ (!EXT2FS_IS_32_BITMAP(bitmap)) {
		ಯದಿ (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			ಪ್ರತ್ಯಾಗಮ ext2fs_mark_generic_bmap(bitmap, bitno);
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"mark_bitmap(%lu)", (unsigned long) bitno);
#endif
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ ((bitno < bitmap32->start) || (bitno > bitmap32->end)) {
		ext2fs_warn_bitmap2(bitmap, EXT2FS_MARK_ERROR, bitno);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಪ್ರತ್ಯಾಗಮ ext2fs_set_bit(bitno - bitmap32->start, bitmap32->bitmap);
}

ಪೂರ್ಣಾಙ್ಕ ext2fs_unmark_generic_bitmap(ext2fs_generic_bitmap bitmap,
					   blk_t bitno)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	ಯದಿ (!EXT2FS_IS_32_BITMAP(bitmap)) {
		ಯದಿ (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			ಪ್ರತ್ಯಾಗಮ ext2fs_unmark_generic_bmap(bitmap, bitno);
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"mark_bitmap(%lu)", (unsigned long) bitno);
#endif
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಯದಿ ((bitno < bitmap32->start) || (bitno > bitmap32->end)) {
		ext2fs_warn_bitmap2(bitmap, EXT2FS_UNMARK_ERROR, bitno);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಪ್ರತ್ಯಾಗಮ ext2fs_clear_bit(bitno - bitmap32->start, bitmap32->bitmap);
}

__u32 ext2fs_get_generic_bitmap_start(ext2fs_generic_bitmap bitmap)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	ಯದಿ (!EXT2FS_IS_32_BITMAP(bitmap)) {
		ಯದಿ (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			ಪ್ರತ್ಯಾಗಮ ext2fs_get_generic_bmap_start(bitmap);
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"get_bitmap_start");
#endif
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	ಪ್ರತ್ಯಾಗಮ bitmap32->start;
}

__u32 ext2fs_get_generic_bitmap_end(ext2fs_generic_bitmap bitmap)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	ಯದಿ (!EXT2FS_IS_32_BITMAP(bitmap)) {
		ಯದಿ (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			ಪ್ರತ್ಯಾಗಮ ext2fs_get_generic_bmap_end(bitmap);
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"get_bitmap_end");
#endif
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಪ್ರತ್ಯಾಗಮ bitmap32->end;
}

ಶೂನ್ಯ ext2fs_clear_generic_bitmap(ext2fs_generic_bitmap bitmap)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	ಯದಿ (!EXT2FS_IS_32_BITMAP(bitmap)) {
		ಯದಿ (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			ext2fs_clear_generic_bmap(bitmap);
			ಪ್ರತ್ಯಾಗಮ;
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"clear_generic_bitmap");
#endif
		ಪ್ರತ್ಯಾಗಮ;
	}

	memset(bitmap32->bitmap, 0,
	       (size_t) (((bitmap32->real_end - bitmap32->start) / 8) + 1));
}

errcode_t ext2fs_fudge_generic_bitmap_end(ext2fs_inode_bitmap gen_bitmap,
					  errcode_t magic, errcode_t neq,
					  ext2_ino_t end, ext2_ino_t *oend)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	EXT2_CHECK_MAGIC(bitmap, magic);

	ಯದಿ (end > bitmap->real_end)
		ಪ್ರತ್ಯಾಗಮ neq;
	ಯದಿ (oend)
		*oend = bitmap->end;
	bitmap->end = end;
	ಪ್ರತ್ಯಾಗಮ 0;
}

errcode_t ext2fs_resize_generic_bitmap(errcode_t magic,
				       __u32 new_end, __u32 new_real_end,
				       ext2fs_generic_bitmap gen_bmap)
{
	ext2fs_generic_bitmap_32 bmap = (ext2fs_generic_bitmap_32) gen_bmap;
	errcode_t	retval;
	size_t		size, new_size;
	__u32		bitno;

	ಯದಿ (!bmap || (bmap->magic != magic))
		ಪ್ರತ್ಯಾಗಮ magic;

	/*
	 * If we're expanding the bitmap, make sure all of the new
	 * parts of the bitmap are zero.
	 */
	ಯದಿ (new_end > bmap->end) {
		bitno = bmap->real_end;
		ಯದಿ (bitno > new_end)
			bitno = new_end;
		ಪ್ರತಿ (; bitno > bmap->end; bitno--)
			ext2fs_clear_bit(bitno - bmap->start, bmap->bitmap);
	}
	ಯದಿ (new_real_end == bmap->real_end) {
		bmap->end = new_end;
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	size = ((bmap->real_end - bmap->start) / 8) + 1;
	new_size = ((new_real_end - bmap->start) / 8) + 1;

	ಯದಿ (size != new_size) {
		retval = ext2fs_resize_mem(size, new_size, &bmap->bitmap);
		ಯದಿ (retval)
			ಪ್ರತ್ಯಾಗಮ retval;
	}
	ಯದಿ (new_size > size)
		memset(bmap->bitmap + size, 0, new_size - size);

	bmap->end = new_end;
	bmap->real_end = new_real_end;
	ಪ್ರತ್ಯಾಗಮ 0;
}

errcode_t ext2fs_compare_generic_bitmap(errcode_t magic, errcode_t neq,
					ext2fs_generic_bitmap gen_bm1,
					ext2fs_generic_bitmap gen_bm2)
{
	ext2fs_generic_bitmap_32 bm1 = (ext2fs_generic_bitmap_32) gen_bm1;
	ext2fs_generic_bitmap_32 bm2 = (ext2fs_generic_bitmap_32) gen_bm2;
	blk_t	i;

	ಯದಿ (!bm1 || bm1->magic != magic)
		ಪ್ರತ್ಯಾಗಮ magic;
	ಯದಿ (!bm2 || bm2->magic != magic)
		ಪ್ರತ್ಯಾಗಮ magic;

	ಯದಿ ((bm1->start != bm2->start) ||
	    (bm1->end != bm2->end) ||
	    (memcmp(bm1->bitmap, bm2->bitmap,
		    (size_t) (bm1->end - bm1->start)/8)))
		ಪ್ರತ್ಯಾಗಮ neq;

	ಪ್ರತಿ (i = bm1->end - ((bm1->end - bm1->start) % 8); i <= bm1->end; i++)
		ಯದಿ (ext2fs_fast_test_block_bitmap(gen_bm1, i) !=
		    ext2fs_fast_test_block_bitmap(gen_bm2, i))
			ಪ್ರತ್ಯಾಗಮ neq;

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಶೂನ್ಯ ext2fs_set_generic_bitmap_padding(ext2fs_generic_bitmap gen_map)
{
	ext2fs_generic_bitmap_32 map = (ext2fs_generic_bitmap_32) gen_map;
	__u32	i, j;

	/* Protect loop from wrap-around if map->real_end is maxed */
	ಪ್ರತಿ (i=map->end+1, j = i - map->start;
	     i <= map->real_end && i > map->end;
	     i++, j++)
		ext2fs_set_bit(j, map->bitmap);
}

errcode_t ext2fs_get_generic_bitmap_range(ext2fs_generic_bitmap gen_bmap,
					  errcode_t magic,
					  __u32 start, __u32 ಅಂಕೆ,
					  ಶೂನ್ಯ *out)
{
	ext2fs_generic_bitmap_32 bmap = (ext2fs_generic_bitmap_32) gen_bmap;

	ಯದಿ (!bmap || (bmap->magic != magic))
		ಪ್ರತ್ಯಾಗಮ magic;

	ಯದಿ ((start < bmap->start) || (start+ಅಂಕೆ-1 > bmap->real_end))
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_INVALID_ARGUMENT;

	memcpy(out, bmap->bitmap + ((start - bmap->start) >> 3), (ಅಂಕೆ+7) >> 3);
	ಪ್ರತ್ಯಾಗಮ 0;
}

errcode_t ext2fs_set_generic_bitmap_range(ext2fs_generic_bitmap gen_bmap,
					  errcode_t magic,
					  __u32 start, __u32 ಅಂಕೆ,
					  ಶೂನ್ಯ *in)
{
	ext2fs_generic_bitmap_32 bmap = (ext2fs_generic_bitmap_32) gen_bmap;

	ಯದಿ (!bmap || (bmap->magic != magic))
		ಪ್ರತ್ಯಾಗಮ magic;

	ಯದಿ ((start < bmap->start) || (start+ಅಂಕೆ-1 > bmap->real_end))
		ಪ್ರತ್ಯಾಗಮ EXT2_ET_INVALID_ARGUMENT;

	memcpy(bmap->bitmap + ((start - bmap->start) >> 3), in, (ಅಂಕೆ+7) >> 3);
	ಪ್ರತ್ಯಾಗಮ 0;
}

/*
 * Compare @mem to zero buffer by 256 bytes.
 * Return 1 if @mem is zeroed memory, otherwise return 0.
 */
ಪೂರ್ಣಾಙ್ಕ ext2fs_mem_is_zero(ಧ್ರುವ ವರ್ಣ *mem, size_t len)
{
	ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ zero_buf[256];

	ಯಾವತ್ (len >= ಪರಿಮಾಣ(zero_buf)) {
		ಯದಿ (memcmp(mem, zero_buf, ಪರಿಮಾಣ(zero_buf)))
			ಪ್ರತ್ಯಾಗಮ 0;
		len -= ಪರಿಮಾಣ(zero_buf);
		mem += ಪರಿಮಾಣ(zero_buf);
	}
	/* Deal with leftover bytes. */
	ಯದಿ (len)
		ಪ್ರತ್ಯಾಗಮ !memcmp(mem, zero_buf, len);
	ಪ್ರತ್ಯಾಗಮ 1;
}

/*
 * Return true if all of the bits in a specified range are clear
 */
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ ext2fs_test_clear_generic_bitmap_range(ext2fs_generic_bitmap gen_bitmap,
						  unsigned ಪೂರ್ಣಾಙ್ಕ start,
						  unsigned ಪೂರ್ಣಾಙ್ಕ len)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	size_t start_byte, len_byte = len >> 3;
	unsigned ಪೂರ್ಣಾಙ್ಕ start_bit, len_bit = len % 8;
	ಪೂರ್ಣಾಙ್ಕ first_bit = 0;
	ಪೂರ್ಣಾಙ್ಕ last_bit  = 0;
	ಪೂರ್ಣಾಙ್ಕ mark_ಎಣಿಕೆ = 0;
	ಪೂರ್ಣಾಙ್ಕ mark_bit = 0;
	ಪೂರ್ಣಾಙ್ಕ i;
	ಧ್ರುವ ವರ್ಣ *ADDR = bitmap->bitmap;

	start -= bitmap->start;
	start_byte = start >> 3;
	start_bit = start % 8;

	ಯದಿ (start_bit != 0) {
		/*
		 * The compared start block number or start inode number
		 * is not the first bit in a byte.
		 */
		mark_ಎಣಿಕೆ = 8 - start_bit;
		ಯದಿ (len < 8 - start_bit) {
			mark_ಎಣಿಕೆ = (ಪೂರ್ಣಾಙ್ಕ)len;
			mark_bit = len + start_bit - 1;
		} ಅನ್ಯಥಾ
			mark_bit = 7;

		ಪ್ರತಿ (i = mark_ಎಣಿಕೆ; i > 0; i--, mark_bit--)
			first_bit |= 1 << mark_bit;

		/*
		 * Compare blocks or inodes in the first byte.
		 * If there is any marked bit, this function returns 0.
		 */
		ಯದಿ (first_bit & ADDR[start_byte])
			ಪ್ರತ್ಯಾಗಮ 0;
		ಅನ್ಯಥಾ ಯದಿ (len <= 8 - start_bit)
			ಪ್ರತ್ಯಾಗಮ 1;

		start_byte++;
		len_bit = (len - mark_ಎಣಿಕೆ) % 8;
		len_byte = (len - mark_ಎಣಿಕೆ) >> 3;
	}

	/*
	 * The compared start block number or start inode number is
	 * the first bit in a byte.
	 */
	ಯದಿ (len_bit != 0) {
		/*
		 * The compared end block number or end inode number is
		 * not the last bit in a byte.
		 */
		ಪ್ರತಿ (mark_bit = len_bit - 1; mark_bit >= 0; mark_bit--)
			last_bit |= 1 << mark_bit;

		/*
		 * Compare blocks or inodes in the last byte.
		 * If there is any marked bit, this function returns 0.
		 */
		ಯದಿ (last_bit & ADDR[start_byte + len_byte])
			ಪ್ರತ್ಯಾಗಮ 0;
		ಅನ್ಯಥಾ ಯದಿ (len_byte == 0)
			ಪ್ರತ್ಯಾಗಮ 1;
	}

	/* Check whether all bytes are 0 */
	ಪ್ರತ್ಯಾಗಮ ext2fs_mem_is_zero(ADDR + start_byte, len_byte);
}

errcode_t ext2fs_find_first_zero_generic_bitmap(ext2fs_generic_bitmap gen_bitmap,
						__u32 start, __u32 end,
						__u32 *out)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	blk_t b;

	ಯದಿ (start < bitmap->start || end > bitmap->end || start > end) {
		ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_TEST_ERROR, start);
		ಪ್ರತ್ಯಾಗಮ EINVAL;
	}

	ಯಾವತ್ (start <= end) {
		b = ext2fs_test_bit(start - bitmap->start, bitmap->bitmap);
		ಯದಿ (!b) {
			*out = start;
			ಪ್ರತ್ಯಾಗಮ 0;
		}
		start++;
	}

	ಪ್ರತ್ಯಾಗಮ ENOENT;
}

errcode_t ext2fs_find_first_set_generic_bitmap(ext2fs_generic_bitmap gen_bitmap,
					       __u32 start, __u32 end,
					       __u32 *out)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	blk_t b;

	ಯದಿ (start < bitmap->start || end > bitmap->end || start > end) {
		ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_TEST_ERROR, start);
		ಪ್ರತ್ಯಾಗಮ EINVAL;
	}

	ಯಾವತ್ (start <= end) {
		b = ext2fs_test_bit(start - bitmap->start, bitmap->bitmap);
		ಯದಿ (b) {
			*out = start;
			ಪ್ರತ್ಯಾಗಮ 0;
		}
		start++;
	}

	ಪ್ರತ್ಯಾಗಮ ENOENT;
}

ಪೂರ್ಣಾಙ್ಕ ext2fs_test_block_bitmap_range(ext2fs_block_bitmap gen_bitmap,
				   blk_t block, ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	EXT2_CHECK_MAGIC(bitmap, EXT2_ET_MAGIC_BLOCK_BITMAP);
	ಯದಿ ((block < bitmap->start) || (block > bitmap->real_end) ||
	    (block+ಅಂಕೆ-1 > bitmap->real_end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_TEST,
				   block, bitmap->description);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಪ್ರತ್ಯಾಗಮ ext2fs_test_clear_generic_bitmap_range((ext2fs_generic_bitmap)
						      bitmap, block, ಅಂಕೆ);
}

ಪೂರ್ಣಾಙ್ಕ ext2fs_test_inode_bitmap_range(ext2fs_inode_bitmap gen_bitmap,
				   ext2_ino_t inode, ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	EXT2_CHECK_MAGIC(bitmap, EXT2_ET_MAGIC_INODE_BITMAP);
	ಯದಿ ((inode < bitmap->start) || (inode > bitmap->real_end) ||
	    (inode+ಅಂಕೆ-1 > bitmap->real_end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_INODE_TEST,
				   inode, bitmap->description);
		ಪ್ರತ್ಯಾಗಮ 0;
	}
	ಪ್ರತ್ಯಾಗಮ ext2fs_test_clear_generic_bitmap_range((ext2fs_generic_bitmap)
						      bitmap, inode, ಅಂಕೆ);
}

ಶೂನ್ಯ ext2fs_mark_block_bitmap_range(ext2fs_block_bitmap gen_bitmap,
				    blk_t block, ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	ಪೂರ್ಣಾಙ್ಕ	i;

	ಯದಿ ((block < bitmap->start) || (block > bitmap->end) ||
	    (block+ಅಂಕೆ-1 > bitmap->end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_MARK, block,
				   bitmap->description);
		ಪ್ರತ್ಯಾಗಮ;
	}
	ಪ್ರತಿ (i=0; i < ಅಂಕೆ; i++)
		ext2fs_fast_set_bit(block + i - bitmap->start, bitmap->bitmap);
}

ಶೂನ್ಯ ext2fs_unmark_block_bitmap_range(ext2fs_block_bitmap gen_bitmap,
				      blk_t block, ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	ಪೂರ್ಣಾಙ್ಕ	i;

	ಯದಿ ((block < bitmap->start) || (block > bitmap->end) ||
	    (block+ಅಂಕೆ-1 > bitmap->end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_UNMARK, block,
				   bitmap->description);
		ಪ್ರತ್ಯಾಗಮ;
	}
	ಪ್ರತಿ (i=0; i < ಅಂಕೆ; i++)
		ext2fs_fast_clear_bit(block + i - bitmap->start,
				      bitmap->bitmap);
}

