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
#အကယ်၍ HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>
#include <time.h>
#အကယ်၍ HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#အကယ်၍ HAVE_SYS_TYPES_H
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
			    ကိန်းပြည့် code, unsigned long arg)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

#ifndef OMIT_COM_ERR
	အကယ်၍ (bitmap->description)
		com_err(0, bitmap->base_error_code+code,
			"#%lu for %s", arg, bitmap->description);
	မဟုတ်ပါက
		com_err(0, bitmap->base_error_code + code, "#%lu", arg);
#endif
}

static errcode_t check_magic(ext2fs_generic_bitmap bitmap)
{
	အကယ်၍ (!bitmap || !((bitmap->magic == EXT2_ET_MAGIC_GENERIC_BITMAP) ||
			 (bitmap->magic == EXT2_ET_MAGIC_INODE_BITMAP) ||
			 (bitmap->magic == EXT2_ET_MAGIC_BLOCK_BITMAP)))
		ပြန်ပေး EXT2_ET_MAGIC_GENERIC_BITMAP;
	ပြန်ပေး 0;
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
	အကယ်၍ (retval)
		ပြန်ပေး retval;

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
	အကယ်၍ (descr) {
		retval = ext2fs_get_mem(strlen(descr)+1, &bitmap->description);
		အကယ်၍ (retval) {
			ext2fs_free_mem(&bitmap);
			ပြန်ပေး retval;
		}
		strcpy(bitmap->description, descr);
	} မဟုတ်ပါက
		bitmap->description = 0;

	size = (size_t) (((bitmap->real_end - bitmap->start) / 8) + 1);
	/* Round up to allow for the BT x86 instruction */
	size = (size + 7) & ~3;
	retval = ext2fs_get_mem(size, &bitmap->bitmap);
	အကယ်၍ (retval) {
		ext2fs_free_mem(&bitmap->description);
		ext2fs_free_mem(&bitmap);
		ပြန်ပေး retval;
	}

	အကယ်၍ (init_map)
		memcpy(bitmap->bitmap, init_map, size);
	မဟုတ်ပါက
		memset(bitmap->bitmap, 0, size);
	*ret = (ext2fs_generic_bitmap) bitmap;
	ပြန်ပေး 0;
}

errcode_t ext2fs_allocate_generic_bitmap(__u32 start,
					 __u32 end,
					 __u32 real_end,
					 const var_na *descr,
					 ext2fs_generic_bitmap *ret)
{
	ပြန်ပေး ext2fs_make_generic_bitmap(EXT2_ET_MAGIC_GENERIC_BITMAP, 0,
					  start, end, real_end, descr, 0, ret);
}

errcode_t ext2fs_copy_generic_bitmap(ext2fs_generic_bitmap gen_src,
				     ext2fs_generic_bitmap *dest)
{
	ext2fs_generic_bitmap_32 src = (ext2fs_generic_bitmap_32) gen_src;

	ပြန်ပေး (ext2fs_make_generic_bitmap(src->magic, src->fs,
					   src->start, src->end,
					   src->real_end,
					   src->description, src->bitmap,
					   dest));
}

void ext2fs_free_generic_bitmap(ext2fs_inode_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	အကယ်၍ (check_magic(gen_bitmap))
		ပြန်ပေး;

	bitmap->magic = 0;
	အကယ်၍ (bitmap->description) {
		ext2fs_free_mem(&bitmap->description);
		bitmap->description = 0;
	}
	အကယ်၍ (bitmap->bitmap) {
		ext2fs_free_mem(&bitmap->bitmap);
		bitmap->bitmap = 0;
	}
	ext2fs_free_mem(&bitmap);
}

ကိန်းပြည့် ext2fs_test_generic_bitmap(ext2fs_generic_bitmap bitmap,
					blk_t bitno)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	အကယ်၍ (!EXT2FS_IS_32_BITMAP(bitmap)) {
		အကယ်၍ (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			ပြန်ပေး ext2fs_test_generic_bmap(bitmap, bitno);
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"test_bitmap(%lu)", (unsigned long) bitno);
#endif
		ပြန်ပေး 0;
	}

	အကယ်၍ ((bitno < bitmap32->start) || (bitno > bitmap32->end)) {
		ext2fs_warn_bitmap2(bitmap, EXT2FS_TEST_ERROR, bitno);
		ပြန်ပေး 0;
	}
	ပြန်ပေး ext2fs_test_bit(bitno - bitmap32->start, bitmap32->bitmap);
}

ကိန်းပြည့် ext2fs_mark_generic_bitmap(ext2fs_generic_bitmap bitmap,
					 __u32 bitno)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	အကယ်၍ (!EXT2FS_IS_32_BITMAP(bitmap)) {
		အကယ်၍ (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			ပြန်ပေး ext2fs_mark_generic_bmap(bitmap, bitno);
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"mark_bitmap(%lu)", (unsigned long) bitno);
#endif
		ပြန်ပေး 0;
	}

	အကယ်၍ ((bitno < bitmap32->start) || (bitno > bitmap32->end)) {
		ext2fs_warn_bitmap2(bitmap, EXT2FS_MARK_ERROR, bitno);
		ပြန်ပေး 0;
	}
	ပြန်ပေး ext2fs_set_bit(bitno - bitmap32->start, bitmap32->bitmap);
}

ကိန်းပြည့် ext2fs_unmark_generic_bitmap(ext2fs_generic_bitmap bitmap,
					   blk_t bitno)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	အကယ်၍ (!EXT2FS_IS_32_BITMAP(bitmap)) {
		အကယ်၍ (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			ပြန်ပေး ext2fs_unmark_generic_bmap(bitmap, bitno);
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"mark_bitmap(%lu)", (unsigned long) bitno);
#endif
		ပြန်ပေး 0;
	}

	အကယ်၍ ((bitno < bitmap32->start) || (bitno > bitmap32->end)) {
		ext2fs_warn_bitmap2(bitmap, EXT2FS_UNMARK_ERROR, bitno);
		ပြန်ပေး 0;
	}
	ပြန်ပေး ext2fs_clear_bit(bitno - bitmap32->start, bitmap32->bitmap);
}

__u32 ext2fs_get_generic_bitmap_start(ext2fs_generic_bitmap bitmap)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	အကယ်၍ (!EXT2FS_IS_32_BITMAP(bitmap)) {
		အကယ်၍ (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			ပြန်ပေး ext2fs_get_generic_bmap_start(bitmap);
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"get_bitmap_start");
#endif
		ပြန်ပေး 0;
	}

	ပြန်ပေး bitmap32->start;
}

__u32 ext2fs_get_generic_bitmap_end(ext2fs_generic_bitmap bitmap)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	အကယ်၍ (!EXT2FS_IS_32_BITMAP(bitmap)) {
		အကယ်၍ (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			ပြန်ပေး ext2fs_get_generic_bmap_end(bitmap);
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"get_bitmap_end");
#endif
		ပြန်ပေး 0;
	}
	ပြန်ပေး bitmap32->end;
}

void ext2fs_clear_generic_bitmap(ext2fs_generic_bitmap bitmap)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	အကယ်၍ (!EXT2FS_IS_32_BITMAP(bitmap)) {
		အကယ်၍ (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			ext2fs_clear_generic_bmap(bitmap);
			ပြန်ပေး;
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"clear_generic_bitmap");
#endif
		ပြန်ပေး;
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

	အကယ်၍ (end > bitmap->real_end)
		ပြန်ပေး neq;
	အကယ်၍ (oend)
		*oend = bitmap->end;
	bitmap->end = end;
	ပြန်ပေး 0;
}

errcode_t ext2fs_resize_generic_bitmap(errcode_t magic,
				       __u32 new_end, __u32 new_real_end,
				       ext2fs_generic_bitmap gen_bmap)
{
	ext2fs_generic_bitmap_32 bmap = (ext2fs_generic_bitmap_32) gen_bmap;
	errcode_t	retval;
	size_t		size, new_size;
	__u32		bitno;

	အကယ်၍ (!bmap || (bmap->magic != magic))
		ပြန်ပေး magic;

	/*
	 * If we're expanding the bitmap, make sure all of the new
	 * parts of the bitmap are zero.
	 */
	အကယ်၍ (new_end > bmap->end) {
		bitno = bmap->real_end;
		အကယ်၍ (bitno > new_end)
			bitno = new_end;
		အတွက် (; bitno > bmap->end; bitno--)
			ext2fs_clear_bit(bitno - bmap->start, bmap->bitmap);
	}
	အကယ်၍ (new_real_end == bmap->real_end) {
		bmap->end = new_end;
		ပြန်ပေး 0;
	}

	size = ((bmap->real_end - bmap->start) / 8) + 1;
	new_size = ((new_real_end - bmap->start) / 8) + 1;

	အကယ်၍ (size != new_size) {
		retval = ext2fs_resize_mem(size, new_size, &bmap->bitmap);
		အကယ်၍ (retval)
			ပြန်ပေး retval;
	}
	အကယ်၍ (new_size > size)
		memset(bmap->bitmap + size, 0, new_size - size);

	bmap->end = new_end;
	bmap->real_end = new_real_end;
	ပြန်ပေး 0;
}

errcode_t ext2fs_compare_generic_bitmap(errcode_t magic, errcode_t neq,
					ext2fs_generic_bitmap gen_bm1,
					ext2fs_generic_bitmap gen_bm2)
{
	ext2fs_generic_bitmap_32 bm1 = (ext2fs_generic_bitmap_32) gen_bm1;
	ext2fs_generic_bitmap_32 bm2 = (ext2fs_generic_bitmap_32) gen_bm2;
	blk_t	i;

	အကယ်၍ (!bm1 || bm1->magic != magic)
		ပြန်ပေး magic;
	အကယ်၍ (!bm2 || bm2->magic != magic)
		ပြန်ပေး magic;

	အကယ်၍ ((bm1->start != bm2->start) ||
	    (bm1->end != bm2->end) ||
	    (memcmp(bm1->bitmap, bm2->bitmap,
		    (size_t) (bm1->end - bm1->start)/8)))
		ပြန်ပေး neq;

	အတွက် (i = bm1->end - ((bm1->end - bm1->start) % 8); i <= bm1->end; i++)
		အကယ်၍ (ext2fs_fast_test_block_bitmap(gen_bm1, i) !=
		    ext2fs_fast_test_block_bitmap(gen_bm2, i))
			ပြန်ပေး neq;

	ပြန်ပေး 0;
}

void ext2fs_set_generic_bitmap_padding(ext2fs_generic_bitmap gen_map)
{
	ext2fs_generic_bitmap_32 map = (ext2fs_generic_bitmap_32) gen_map;
	__u32	i, j;

	/* Protect loop from wrap-around if map->real_end is maxed */
	အတွက် (i=map->end+1, j = i - map->start;
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

	အကယ်၍ (!bmap || (bmap->magic != magic))
		ပြန်ပေး magic;

	အကယ်၍ ((start < bmap->start) || (start+num-1 > bmap->real_end))
		ပြန်ပေး EXT2_ET_INVALID_ARGUMENT;

	memcpy(out, bmap->bitmap + ((start - bmap->start) >> 3), (num+7) >> 3);
	ပြန်ပေး 0;
}

errcode_t ext2fs_set_generic_bitmap_range(ext2fs_generic_bitmap gen_bmap,
					  errcode_t magic,
					  __u32 start, __u32 num,
					  void *in)
{
	ext2fs_generic_bitmap_32 bmap = (ext2fs_generic_bitmap_32) gen_bmap;

	အကယ်၍ (!bmap || (bmap->magic != magic))
		ပြန်ပေး magic;

	အကယ်၍ ((start < bmap->start) || (start+num-1 > bmap->real_end))
		ပြန်ပေး EXT2_ET_INVALID_ARGUMENT;

	memcpy(bmap->bitmap + ((start - bmap->start) >> 3), in, (num+7) >> 3);
	ပြန်ပေး 0;
}

/*
 * Compare @mem to zero buffer by 256 bytes.
 * Return 1 if @mem is zeroed memory, otherwise return 0.
 */
ကိန်းပြည့် ext2fs_mem_is_zero(const var_na *mem, size_t len)
{
	static const var_na zero_buf[256];

	နေစဉ် (len >= parimaa_na(zero_buf)) {
		အကယ်၍ (memcmp(mem, zero_buf, parimaa_na(zero_buf)))
			ပြန်ပေး 0;
		len -= parimaa_na(zero_buf);
		mem += parimaa_na(zero_buf);
	}
	/* Deal with leftover bytes. */
	အကယ်၍ (len)
		ပြန်ပေး !memcmp(mem, zero_buf, len);
	ပြန်ပေး 1;
}

/*
 * Return true if all of the bits in a specified range are clear
 */
static ကိန်းပြည့် ext2fs_test_clear_generic_bitmap_range(ext2fs_generic_bitmap gen_bitmap,
						  unsigned ကိန်းပြည့် start,
						  unsigned ကိန်းပြည့် len)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	size_t start_byte, len_byte = len >> 3;
	unsigned ကိန်းပြည့် start_bit, len_bit = len % 8;
	ကိန်းပြည့် first_bit = 0;
	ကိန်းပြည့် last_bit  = 0;
	ကိန်းပြည့် mark_count = 0;
	ကိန်းပြည့် mark_bit = 0;
	ကိန်းပြည့် i;
	const var_na *ADDR = bitmap->bitmap;

	start -= bitmap->start;
	start_byte = start >> 3;
	start_bit = start % 8;

	အကယ်၍ (start_bit != 0) {
		/*
		 * The compared start block number or start inode number
		 * is not the first bit in a byte.
		 */
		mark_count = 8 - start_bit;
		အကယ်၍ (len < 8 - start_bit) {
			mark_count = (ကိန်းပြည့်)len;
			mark_bit = len + start_bit - 1;
		} မဟုတ်ပါက
			mark_bit = 7;

		အတွက် (i = mark_count; i > 0; i--, mark_bit--)
			first_bit |= 1 << mark_bit;

		/*
		 * Compare blocks or inodes in the first byte.
		 * If there is any marked bit, this function returns 0.
		 */
		အကယ်၍ (first_bit & ADDR[start_byte])
			ပြန်ပေး 0;
		မဟုတ်ပါက အကယ်၍ (len <= 8 - start_bit)
			ပြန်ပေး 1;

		start_byte++;
		len_bit = (len - mark_count) % 8;
		len_byte = (len - mark_count) >> 3;
	}

	/*
	 * The compared start block number or start inode number is
	 * the first bit in a byte.
	 */
	အကယ်၍ (len_bit != 0) {
		/*
		 * The compared end block number or end inode number is
		 * not the last bit in a byte.
		 */
		အတွက် (mark_bit = len_bit - 1; mark_bit >= 0; mark_bit--)
			last_bit |= 1 << mark_bit;

		/*
		 * Compare blocks or inodes in the last byte.
		 * If there is any marked bit, this function returns 0.
		 */
		အကယ်၍ (last_bit & ADDR[start_byte + len_byte])
			ပြန်ပေး 0;
		မဟုတ်ပါက အကယ်၍ (len_byte == 0)
			ပြန်ပေး 1;
	}

	/* Check whether all bytes are 0 */
	ပြန်ပေး ext2fs_mem_is_zero(ADDR + start_byte, len_byte);
}

errcode_t ext2fs_find_first_zero_generic_bitmap(ext2fs_generic_bitmap gen_bitmap,
						__u32 start, __u32 end,
						__u32 *out)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	blk_t b;

	အကယ်၍ (start < bitmap->start || end > bitmap->end || start > end) {
		ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_TEST_ERROR, start);
		ပြန်ပေး EINVAL;
	}

	နေစဉ် (start <= end) {
		b = ext2fs_test_bit(start - bitmap->start, bitmap->bitmap);
		အကယ်၍ (!b) {
			*out = start;
			ပြန်ပေး 0;
		}
		start++;
	}

	ပြန်ပေး ENOENT;
}

errcode_t ext2fs_find_first_set_generic_bitmap(ext2fs_generic_bitmap gen_bitmap,
					       __u32 start, __u32 end,
					       __u32 *out)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	blk_t b;

	အကယ်၍ (start < bitmap->start || end > bitmap->end || start > end) {
		ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_TEST_ERROR, start);
		ပြန်ပေး EINVAL;
	}

	နေစဉ် (start <= end) {
		b = ext2fs_test_bit(start - bitmap->start, bitmap->bitmap);
		အကယ်၍ (b) {
			*out = start;
			ပြန်ပေး 0;
		}
		start++;
	}

	ပြန်ပေး ENOENT;
}

ကိန်းပြည့် ext2fs_test_block_bitmap_range(ext2fs_block_bitmap gen_bitmap,
				   blk_t block, ကိန်းပြည့် num)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	EXT2_CHECK_MAGIC(bitmap, EXT2_ET_MAGIC_BLOCK_BITMAP);
	အကယ်၍ ((block < bitmap->start) || (block > bitmap->real_end) ||
	    (block+num-1 > bitmap->real_end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_TEST,
				   block, bitmap->description);
		ပြန်ပေး 0;
	}
	ပြန်ပေး ext2fs_test_clear_generic_bitmap_range((ext2fs_generic_bitmap)
						      bitmap, block, num);
}

ကိန်းပြည့် ext2fs_test_inode_bitmap_range(ext2fs_inode_bitmap gen_bitmap,
				   ext2_ino_t inode, ကိန်းပြည့် num)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	EXT2_CHECK_MAGIC(bitmap, EXT2_ET_MAGIC_INODE_BITMAP);
	အကယ်၍ ((inode < bitmap->start) || (inode > bitmap->real_end) ||
	    (inode+num-1 > bitmap->real_end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_INODE_TEST,
				   inode, bitmap->description);
		ပြန်ပေး 0;
	}
	ပြန်ပေး ext2fs_test_clear_generic_bitmap_range((ext2fs_generic_bitmap)
						      bitmap, inode, num);
}

void ext2fs_mark_block_bitmap_range(ext2fs_block_bitmap gen_bitmap,
				    blk_t block, ကိန်းပြည့် num)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	ကိန်းပြည့်	i;

	အကယ်၍ ((block < bitmap->start) || (block > bitmap->end) ||
	    (block+num-1 > bitmap->end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_MARK, block,
				   bitmap->description);
		ပြန်ပေး;
	}
	အတွက် (i=0; i < num; i++)
		ext2fs_fast_set_bit(block + i - bitmap->start, bitmap->bitmap);
}

void ext2fs_unmark_block_bitmap_range(ext2fs_block_bitmap gen_bitmap,
				      blk_t block, ကိန်းပြည့် num)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	ကိန်းပြည့်	i;

	အကယ်၍ ((block < bitmap->start) || (block > bitmap->end) ||
	    (block+num-1 > bitmap->end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_UNMARK, block,
				   bitmap->description);
		ပြန်ပေး;
	}
	အတွက် (i=0; i < num; i++)
		ext2fs_fast_clear_bit(block + i - bitmap->start,
				      bitmap->bitmap);
}

