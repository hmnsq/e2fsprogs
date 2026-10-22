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


#include "config.h"
#include <stdio.h>
#include <string.h>
#ຖ້າ HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>
#include <time.h>
#ຖ້າ HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#ຖ້າ HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include "ext2_fs.h"
#include "ext2fsP.h"

sa_nracanaa ext2fs_struct_generic_bitmap_32 {
	errcode_t	magic;
	ext2_filsys 	fs;
	__u32		start, end;
	__u32		real_end;
	var_na	*	description;
	var_na	*	bitmap;
	errcode_t	base_error_code;
	__u32		reserved[7];
};

typedef sa_nracanaa ext2fs_struct_generic_bitmap_32 *ext2fs_generic_bitmap_32;

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
void ext2fs_warn_bitmap2(ext2fs_generic_bitmap gen_bitmap,
			    ຈຳນວນເຕັມ code, unsigned long arg)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

#ifndef OMIT_COM_ERR
	ຖ້າ (bitmap->description)
		com_err(0, bitmap->base_error_code+code,
			"#%lu for %s", arg, bitmap->description);
	ບໍ່ດັ່ງນັ້ນ
		com_err(0, bitmap->base_error_code + code, "#%lu", arg);
#endif
}

static errcode_t check_magic(ext2fs_generic_bitmap bitmap)
{
	ຖ້າ (!bitmap || !((bitmap->magic == EXT2_ET_MAGIC_GENERIC_BITMAP) ||
			 (bitmap->magic == EXT2_ET_MAGIC_INODE_BITMAP) ||
			 (bitmap->magic == EXT2_ET_MAGIC_BLOCK_BITMAP)))
		ສົ່ງຄືນ EXT2_ET_MAGIC_GENERIC_BITMAP;
	ສົ່ງຄືນ 0;
}

errcode_t ext2fs_make_generic_bitmap(errcode_t magic, ext2_filsys fs,
				     __u32 start, __u32 end, __u32 real_end,
				     const var_na *descr, var_na *init_map,
				     ext2fs_generic_bitmap *ret)
{
	ext2fs_generic_bitmap_32 bitmap;
	errcode_t		retval;
	size_t			size;

	retval = ext2fs_get_mem(parimaa_na(sa_nracanaa ext2fs_struct_generic_bitmap_32),
				&bitmap);
	ຖ້າ (retval)
		ສົ່ງຄືນ retval;

	bitmap->magic = magic;
	bitmap->fs = fs;
	bitmap->start = start;
	bitmap->end = end;
	bitmap->real_end = real_end;
	switch (magic) {
	pak_sa EXT2_ET_MAGIC_INODE_BITMAP:
		bitmap->base_error_code = EXT2_ET_BAD_INODE_MARK;
		bha_nga;
	pak_sa EXT2_ET_MAGIC_BLOCK_BITMAP:
		bitmap->base_error_code = EXT2_ET_BAD_BLOCK_MARK;
		bha_nga;
	default:
		bitmap->base_error_code = EXT2_ET_BAD_GENERIC_MARK;
	}
	ຖ້າ (descr) {
		retval = ext2fs_get_mem(strlen(descr)+1, &bitmap->description);
		ຖ້າ (retval) {
			ext2fs_free_mem(&bitmap);
			ສົ່ງຄືນ retval;
		}
		strcpy(bitmap->description, descr);
	} ບໍ່ດັ່ງນັ້ນ
		bitmap->description = 0;

	size = (size_t) (((bitmap->real_end - bitmap->start) / 8) + 1);
	/* Round up to allow for the BT x86 instruction */
	size = (size + 7) & ~3;
	retval = ext2fs_get_mem(size, &bitmap->bitmap);
	ຖ້າ (retval) {
		ext2fs_free_mem(&bitmap->description);
		ext2fs_free_mem(&bitmap);
		ສົ່ງຄືນ retval;
	}

	ຖ້າ (init_map)
		memcpy(bitmap->bitmap, init_map, size);
	ບໍ່ດັ່ງນັ້ນ
		memset(bitmap->bitmap, 0, size);
	*ret = (ext2fs_generic_bitmap) bitmap;
	ສົ່ງຄືນ 0;
}

errcode_t ext2fs_allocate_generic_bitmap(__u32 start,
					 __u32 end,
					 __u32 real_end,
					 const var_na *descr,
					 ext2fs_generic_bitmap *ret)
{
	ສົ່ງຄືນ ext2fs_make_generic_bitmap(EXT2_ET_MAGIC_GENERIC_BITMAP, 0,
					  start, end, real_end, descr, 0, ret);
}

errcode_t ext2fs_copy_generic_bitmap(ext2fs_generic_bitmap gen_src,
				     ext2fs_generic_bitmap *dest)
{
	ext2fs_generic_bitmap_32 src = (ext2fs_generic_bitmap_32) gen_src;

	ສົ່ງຄືນ (ext2fs_make_generic_bitmap(src->magic, src->fs,
					   src->start, src->end,
					   src->real_end,
					   src->description, src->bitmap,
					   dest));
}

void ext2fs_free_generic_bitmap(ext2fs_inode_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	ຖ້າ (check_magic(gen_bitmap))
		ສົ່ງຄືນ;

	bitmap->magic = 0;
	ຖ້າ (bitmap->description) {
		ext2fs_free_mem(&bitmap->description);
		bitmap->description = 0;
	}
	ຖ້າ (bitmap->bitmap) {
		ext2fs_free_mem(&bitmap->bitmap);
		bitmap->bitmap = 0;
	}
	ext2fs_free_mem(&bitmap);
}

ຈຳນວນເຕັມ ext2fs_test_generic_bitmap(ext2fs_generic_bitmap bitmap,
					blk_t bitno)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	ຖ້າ (!EXT2FS_IS_32_BITMAP(bitmap)) {
		ຖ້າ (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			ສົ່ງຄືນ ext2fs_test_generic_bmap(bitmap, bitno);
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"test_bitmap(%lu)", (unsigned long) bitno);
#endif
		ສົ່ງຄືນ 0;
	}

	ຖ້າ ((bitno < bitmap32->start) || (bitno > bitmap32->end)) {
		ext2fs_warn_bitmap2(bitmap, EXT2FS_TEST_ERROR, bitno);
		ສົ່ງຄືນ 0;
	}
	ສົ່ງຄືນ ext2fs_test_bit(bitno - bitmap32->start, bitmap32->bitmap);
}

ຈຳນວນເຕັມ ext2fs_mark_generic_bitmap(ext2fs_generic_bitmap bitmap,
					 __u32 bitno)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	ຖ້າ (!EXT2FS_IS_32_BITMAP(bitmap)) {
		ຖ້າ (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			ສົ່ງຄືນ ext2fs_mark_generic_bmap(bitmap, bitno);
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"mark_bitmap(%lu)", (unsigned long) bitno);
#endif
		ສົ່ງຄືນ 0;
	}

	ຖ້າ ((bitno < bitmap32->start) || (bitno > bitmap32->end)) {
		ext2fs_warn_bitmap2(bitmap, EXT2FS_MARK_ERROR, bitno);
		ສົ່ງຄືນ 0;
	}
	ສົ່ງຄືນ ext2fs_set_bit(bitno - bitmap32->start, bitmap32->bitmap);
}

ຈຳນວນເຕັມ ext2fs_unmark_generic_bitmap(ext2fs_generic_bitmap bitmap,
					   blk_t bitno)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	ຖ້າ (!EXT2FS_IS_32_BITMAP(bitmap)) {
		ຖ້າ (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			ສົ່ງຄືນ ext2fs_unmark_generic_bmap(bitmap, bitno);
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"mark_bitmap(%lu)", (unsigned long) bitno);
#endif
		ສົ່ງຄືນ 0;
	}

	ຖ້າ ((bitno < bitmap32->start) || (bitno > bitmap32->end)) {
		ext2fs_warn_bitmap2(bitmap, EXT2FS_UNMARK_ERROR, bitno);
		ສົ່ງຄືນ 0;
	}
	ສົ່ງຄືນ ext2fs_clear_bit(bitno - bitmap32->start, bitmap32->bitmap);
}

__u32 ext2fs_get_generic_bitmap_start(ext2fs_generic_bitmap bitmap)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	ຖ້າ (!EXT2FS_IS_32_BITMAP(bitmap)) {
		ຖ້າ (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			ສົ່ງຄືນ ext2fs_get_generic_bmap_start(bitmap);
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"get_bitmap_start");
#endif
		ສົ່ງຄືນ 0;
	}

	ສົ່ງຄືນ bitmap32->start;
}

__u32 ext2fs_get_generic_bitmap_end(ext2fs_generic_bitmap bitmap)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	ຖ້າ (!EXT2FS_IS_32_BITMAP(bitmap)) {
		ຖ້າ (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			ສົ່ງຄືນ ext2fs_get_generic_bmap_end(bitmap);
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"get_bitmap_end");
#endif
		ສົ່ງຄືນ 0;
	}
	ສົ່ງຄືນ bitmap32->end;
}

void ext2fs_clear_generic_bitmap(ext2fs_generic_bitmap bitmap)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	ຖ້າ (!EXT2FS_IS_32_BITMAP(bitmap)) {
		ຖ້າ (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			ext2fs_clear_generic_bmap(bitmap);
			ສົ່ງຄືນ;
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"clear_generic_bitmap");
#endif
		ສົ່ງຄືນ;
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

	ຖ້າ (end > bitmap->real_end)
		ສົ່ງຄືນ neq;
	ຖ້າ (oend)
		*oend = bitmap->end;
	bitmap->end = end;
	ສົ່ງຄືນ 0;
}

errcode_t ext2fs_resize_generic_bitmap(errcode_t magic,
				       __u32 new_end, __u32 new_real_end,
				       ext2fs_generic_bitmap gen_bmap)
{
	ext2fs_generic_bitmap_32 bmap = (ext2fs_generic_bitmap_32) gen_bmap;
	errcode_t	retval;
	size_t		size, new_size;
	__u32		bitno;

	ຖ້າ (!bmap || (bmap->magic != magic))
		ສົ່ງຄືນ magic;

	/*
	 * If we're expanding the bitmap, make sure all of the new
	 * parts of the bitmap are zero.
	 */
	ຖ້າ (new_end > bmap->end) {
		bitno = bmap->real_end;
		ຖ້າ (bitno > new_end)
			bitno = new_end;
		ສຳລັບ (; bitno > bmap->end; bitno--)
			ext2fs_clear_bit(bitno - bmap->start, bmap->bitmap);
	}
	ຖ້າ (new_real_end == bmap->real_end) {
		bmap->end = new_end;
		ສົ່ງຄືນ 0;
	}

	size = ((bmap->real_end - bmap->start) / 8) + 1;
	new_size = ((new_real_end - bmap->start) / 8) + 1;

	ຖ້າ (size != new_size) {
		retval = ext2fs_resize_mem(size, new_size, &bmap->bitmap);
		ຖ້າ (retval)
			ສົ່ງຄືນ retval;
	}
	ຖ້າ (new_size > size)
		memset(bmap->bitmap + size, 0, new_size - size);

	bmap->end = new_end;
	bmap->real_end = new_real_end;
	ສົ່ງຄືນ 0;
}

errcode_t ext2fs_compare_generic_bitmap(errcode_t magic, errcode_t neq,
					ext2fs_generic_bitmap gen_bm1,
					ext2fs_generic_bitmap gen_bm2)
{
	ext2fs_generic_bitmap_32 bm1 = (ext2fs_generic_bitmap_32) gen_bm1;
	ext2fs_generic_bitmap_32 bm2 = (ext2fs_generic_bitmap_32) gen_bm2;
	blk_t	i;

	ຖ້າ (!bm1 || bm1->magic != magic)
		ສົ່ງຄືນ magic;
	ຖ້າ (!bm2 || bm2->magic != magic)
		ສົ່ງຄືນ magic;

	ຖ້າ ((bm1->start != bm2->start) ||
	    (bm1->end != bm2->end) ||
	    (memcmp(bm1->bitmap, bm2->bitmap,
		    (size_t) (bm1->end - bm1->start)/8)))
		ສົ່ງຄືນ neq;

	ສຳລັບ (i = bm1->end - ((bm1->end - bm1->start) % 8); i <= bm1->end; i++)
		ຖ້າ (ext2fs_fast_test_block_bitmap(gen_bm1, i) !=
		    ext2fs_fast_test_block_bitmap(gen_bm2, i))
			ສົ່ງຄືນ neq;

	ສົ່ງຄືນ 0;
}

void ext2fs_set_generic_bitmap_padding(ext2fs_generic_bitmap gen_map)
{
	ext2fs_generic_bitmap_32 map = (ext2fs_generic_bitmap_32) gen_map;
	__u32	i, j;

	/* Protect loop from wrap-around if map->real_end is maxed */
	ສຳລັບ (i=map->end+1, j = i - map->start;
	     i <= map->real_end && i > map->end;
	     i++, j++)
		ext2fs_set_bit(j, map->bitmap);
}

errcode_t ext2fs_get_generic_bitmap_range(ext2fs_generic_bitmap gen_bmap,
					  errcode_t magic,
					  __u32 start, __u32 num,
					  void *out)
{
	ext2fs_generic_bitmap_32 bmap = (ext2fs_generic_bitmap_32) gen_bmap;

	ຖ້າ (!bmap || (bmap->magic != magic))
		ສົ່ງຄືນ magic;

	ຖ້າ ((start < bmap->start) || (start+num-1 > bmap->real_end))
		ສົ່ງຄືນ EXT2_ET_INVALID_ARGUMENT;

	memcpy(out, bmap->bitmap + ((start - bmap->start) >> 3), (num+7) >> 3);
	ສົ່ງຄືນ 0;
}

errcode_t ext2fs_set_generic_bitmap_range(ext2fs_generic_bitmap gen_bmap,
					  errcode_t magic,
					  __u32 start, __u32 num,
					  void *in)
{
	ext2fs_generic_bitmap_32 bmap = (ext2fs_generic_bitmap_32) gen_bmap;

	ຖ້າ (!bmap || (bmap->magic != magic))
		ສົ່ງຄືນ magic;

	ຖ້າ ((start < bmap->start) || (start+num-1 > bmap->real_end))
		ສົ່ງຄືນ EXT2_ET_INVALID_ARGUMENT;

	memcpy(bmap->bitmap + ((start - bmap->start) >> 3), in, (num+7) >> 3);
	ສົ່ງຄືນ 0;
}

/*
 * Compare @mem to zero buffer by 256 bytes.
 * Return 1 if @mem is zeroed memory, otherwise return 0.
 */
ຈຳນວນເຕັມ ext2fs_mem_is_zero(const var_na *mem, size_t len)
{
	static const var_na zero_buf[256];

	ໃນຂະນະທີ່ (len >= parimaa_na(zero_buf)) {
		ຖ້າ (memcmp(mem, zero_buf, parimaa_na(zero_buf)))
			ສົ່ງຄືນ 0;
		len -= parimaa_na(zero_buf);
		mem += parimaa_na(zero_buf);
	}
	/* Deal with leftover bytes. */
	ຖ້າ (len)
		ສົ່ງຄືນ !memcmp(mem, zero_buf, len);
	ສົ່ງຄືນ 1;
}

/*
 * Return true if all of the bits in a specified range are clear
 */
static ຈຳນວນເຕັມ ext2fs_test_clear_generic_bitmap_range(ext2fs_generic_bitmap gen_bitmap,
						  unsigned ຈຳນວນເຕັມ start,
						  unsigned ຈຳນວນເຕັມ len)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	size_t start_byte, len_byte = len >> 3;
	unsigned ຈຳນວນເຕັມ start_bit, len_bit = len % 8;
	ຈຳນວນເຕັມ first_bit = 0;
	ຈຳນວນເຕັມ last_bit  = 0;
	ຈຳນວນເຕັມ mark_count = 0;
	ຈຳນວນເຕັມ mark_bit = 0;
	ຈຳນວນເຕັມ i;
	const var_na *ADDR = bitmap->bitmap;

	start -= bitmap->start;
	start_byte = start >> 3;
	start_bit = start % 8;

	ຖ້າ (start_bit != 0) {
		/*
		 * The compared start block number or start inode number
		 * is not the first bit in a byte.
		 */
		mark_count = 8 - start_bit;
		ຖ້າ (len < 8 - start_bit) {
			mark_count = (ຈຳນວນເຕັມ)len;
			mark_bit = len + start_bit - 1;
		} ບໍ່ດັ່ງນັ້ນ
			mark_bit = 7;

		ສຳລັບ (i = mark_count; i > 0; i--, mark_bit--)
			first_bit |= 1 << mark_bit;

		/*
		 * Compare blocks or inodes in the first byte.
		 * If there is any marked bit, this function returns 0.
		 */
		ຖ້າ (first_bit & ADDR[start_byte])
			ສົ່ງຄືນ 0;
		ບໍ່ດັ່ງນັ້ນ ຖ້າ (len <= 8 - start_bit)
			ສົ່ງຄືນ 1;

		start_byte++;
		len_bit = (len - mark_count) % 8;
		len_byte = (len - mark_count) >> 3;
	}

	/*
	 * The compared start block number or start inode number is
	 * the first bit in a byte.
	 */
	ຖ້າ (len_bit != 0) {
		/*
		 * The compared end block number or end inode number is
		 * not the last bit in a byte.
		 */
		ສຳລັບ (mark_bit = len_bit - 1; mark_bit >= 0; mark_bit--)
			last_bit |= 1 << mark_bit;

		/*
		 * Compare blocks or inodes in the last byte.
		 * If there is any marked bit, this function returns 0.
		 */
		ຖ້າ (last_bit & ADDR[start_byte + len_byte])
			ສົ່ງຄືນ 0;
		ບໍ່ດັ່ງນັ້ນ ຖ້າ (len_byte == 0)
			ສົ່ງຄືນ 1;
	}

	/* Check whether all bytes are 0 */
	ສົ່ງຄືນ ext2fs_mem_is_zero(ADDR + start_byte, len_byte);
}

errcode_t ext2fs_find_first_zero_generic_bitmap(ext2fs_generic_bitmap gen_bitmap,
						__u32 start, __u32 end,
						__u32 *out)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	blk_t b;

	ຖ້າ (start < bitmap->start || end > bitmap->end || start > end) {
		ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_TEST_ERROR, start);
		ສົ່ງຄືນ EINVAL;
	}

	ໃນຂະນະທີ່ (start <= end) {
		b = ext2fs_test_bit(start - bitmap->start, bitmap->bitmap);
		ຖ້າ (!b) {
			*out = start;
			ສົ່ງຄືນ 0;
		}
		start++;
	}

	ສົ່ງຄືນ ENOENT;
}

errcode_t ext2fs_find_first_set_generic_bitmap(ext2fs_generic_bitmap gen_bitmap,
					       __u32 start, __u32 end,
					       __u32 *out)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	blk_t b;

	ຖ້າ (start < bitmap->start || end > bitmap->end || start > end) {
		ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_TEST_ERROR, start);
		ສົ່ງຄືນ EINVAL;
	}

	ໃນຂະນະທີ່ (start <= end) {
		b = ext2fs_test_bit(start - bitmap->start, bitmap->bitmap);
		ຖ້າ (b) {
			*out = start;
			ສົ່ງຄືນ 0;
		}
		start++;
	}

	ສົ່ງຄືນ ENOENT;
}

ຈຳນວນເຕັມ ext2fs_test_block_bitmap_range(ext2fs_block_bitmap gen_bitmap,
				   blk_t block, ຈຳນວນເຕັມ num)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	EXT2_CHECK_MAGIC(bitmap, EXT2_ET_MAGIC_BLOCK_BITMAP);
	ຖ້າ ((block < bitmap->start) || (block > bitmap->real_end) ||
	    (block+num-1 > bitmap->real_end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_TEST,
				   block, bitmap->description);
		ສົ່ງຄືນ 0;
	}
	ສົ່ງຄືນ ext2fs_test_clear_generic_bitmap_range((ext2fs_generic_bitmap)
						      bitmap, block, num);
}

ຈຳນວນເຕັມ ext2fs_test_inode_bitmap_range(ext2fs_inode_bitmap gen_bitmap,
				   ext2_ino_t inode, ຈຳນວນເຕັມ num)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	EXT2_CHECK_MAGIC(bitmap, EXT2_ET_MAGIC_INODE_BITMAP);
	ຖ້າ ((inode < bitmap->start) || (inode > bitmap->real_end) ||
	    (inode+num-1 > bitmap->real_end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_INODE_TEST,
				   inode, bitmap->description);
		ສົ່ງຄືນ 0;
	}
	ສົ່ງຄືນ ext2fs_test_clear_generic_bitmap_range((ext2fs_generic_bitmap)
						      bitmap, inode, num);
}

void ext2fs_mark_block_bitmap_range(ext2fs_block_bitmap gen_bitmap,
				    blk_t block, ຈຳນວນເຕັມ num)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	ຈຳນວນເຕັມ	i;

	ຖ້າ ((block < bitmap->start) || (block > bitmap->end) ||
	    (block+num-1 > bitmap->end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_MARK, block,
				   bitmap->description);
		ສົ່ງຄືນ;
	}
	ສຳລັບ (i=0; i < num; i++)
		ext2fs_fast_set_bit(block + i - bitmap->start, bitmap->bitmap);
}

void ext2fs_unmark_block_bitmap_range(ext2fs_block_bitmap gen_bitmap,
				      blk_t block, ຈຳນວນເຕັມ num)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	ຈຳນວນເຕັມ	i;

	ຖ້າ ((block < bitmap->start) || (block > bitmap->end) ||
	    (block+num-1 > bitmap->end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_UNMARK, block,
				   bitmap->description);
		ສົ່ງຄືນ;
	}
	ສຳລັບ (i=0; i < num; i++)
		ext2fs_fast_clear_bit(block + i - bitmap->start,
				      bitmap->bitmap);
}

