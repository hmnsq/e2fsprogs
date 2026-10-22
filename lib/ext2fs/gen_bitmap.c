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
#nếu HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>
#include <time.h>
#nếu HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#nếu HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include "ext2_fs.h"
#include "ext2fsP.h"

struct ext2fs_struct_generic_bitmap_32 {
	errcode_t	magic;
	ext2_filsys 	fs;
	__u32		start, end;
	__u32		real_end;
	char	*	description;
	char	*	bitmap;
	errcode_t	base_error_code;
	__u32		reserved[7];
};

typedef struct ext2fs_struct_generic_bitmap_32 *ext2fs_generic_bitmap_32;

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
			    số_nguyên code, unsigned long arg)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

#ifndef OMIT_COM_ERR
	nếu (bitmap->description)
		com_err(0, bitmap->base_error_code+code,
			"#%lu for %s", arg, bitmap->description);
	không_thì
		com_err(0, bitmap->base_error_code + code, "#%lu", arg);
#endif
}

static errcode_t check_magic(ext2fs_generic_bitmap bitmap)
{
	nếu (!bitmap || !((bitmap->magic == EXT2_ET_MAGIC_GENERIC_BITMAP) ||
			 (bitmap->magic == EXT2_ET_MAGIC_INODE_BITMAP) ||
			 (bitmap->magic == EXT2_ET_MAGIC_BLOCK_BITMAP)))
		trả_về EXT2_ET_MAGIC_GENERIC_BITMAP;
	trả_về 0;
}

errcode_t ext2fs_make_generic_bitmap(errcode_t magic, ext2_filsys fs,
				     __u32 start, __u32 end, __u32 real_end,
				     const char *descr, char *init_map,
				     ext2fs_generic_bitmap *ret)
{
	ext2fs_generic_bitmap_32 bitmap;
	errcode_t		retval;
	size_t			size;

	retval = ext2fs_get_mem(sizeof(struct ext2fs_struct_generic_bitmap_32),
				&bitmap);
	nếu (retval)
		trả_về retval;

	bitmap->magic = magic;
	bitmap->fs = fs;
	bitmap->start = start;
	bitmap->end = end;
	bitmap->real_end = real_end;
	switch (magic) {
	case EXT2_ET_MAGIC_INODE_BITMAP:
		bitmap->base_error_code = EXT2_ET_BAD_INODE_MARK;
		break;
	case EXT2_ET_MAGIC_BLOCK_BITMAP:
		bitmap->base_error_code = EXT2_ET_BAD_BLOCK_MARK;
		break;
	default:
		bitmap->base_error_code = EXT2_ET_BAD_GENERIC_MARK;
	}
	nếu (descr) {
		retval = ext2fs_get_mem(strlen(descr)+1, &bitmap->description);
		nếu (retval) {
			ext2fs_free_mem(&bitmap);
			trả_về retval;
		}
		strcpy(bitmap->description, descr);
	} không_thì
		bitmap->description = 0;

	size = (size_t) (((bitmap->real_end - bitmap->start) / 8) + 1);
	/* Round up to allow for the BT x86 instruction */
	size = (size + 7) & ~3;
	retval = ext2fs_get_mem(size, &bitmap->bitmap);
	nếu (retval) {
		ext2fs_free_mem(&bitmap->description);
		ext2fs_free_mem(&bitmap);
		trả_về retval;
	}

	nếu (init_map)
		memcpy(bitmap->bitmap, init_map, size);
	không_thì
		memset(bitmap->bitmap, 0, size);
	*ret = (ext2fs_generic_bitmap) bitmap;
	trả_về 0;
}

errcode_t ext2fs_allocate_generic_bitmap(__u32 start,
					 __u32 end,
					 __u32 real_end,
					 const char *descr,
					 ext2fs_generic_bitmap *ret)
{
	trả_về ext2fs_make_generic_bitmap(EXT2_ET_MAGIC_GENERIC_BITMAP, 0,
					  start, end, real_end, descr, 0, ret);
}

errcode_t ext2fs_copy_generic_bitmap(ext2fs_generic_bitmap gen_src,
				     ext2fs_generic_bitmap *dest)
{
	ext2fs_generic_bitmap_32 src = (ext2fs_generic_bitmap_32) gen_src;

	trả_về (ext2fs_make_generic_bitmap(src->magic, src->fs,
					   src->start, src->end,
					   src->real_end,
					   src->description, src->bitmap,
					   dest));
}

void ext2fs_free_generic_bitmap(ext2fs_inode_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	nếu (check_magic(gen_bitmap))
		trả_về;

	bitmap->magic = 0;
	nếu (bitmap->description) {
		ext2fs_free_mem(&bitmap->description);
		bitmap->description = 0;
	}
	nếu (bitmap->bitmap) {
		ext2fs_free_mem(&bitmap->bitmap);
		bitmap->bitmap = 0;
	}
	ext2fs_free_mem(&bitmap);
}

số_nguyên ext2fs_test_generic_bitmap(ext2fs_generic_bitmap bitmap,
					blk_t bitno)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	nếu (!EXT2FS_IS_32_BITMAP(bitmap)) {
		nếu (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			trả_về ext2fs_test_generic_bmap(bitmap, bitno);
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"test_bitmap(%lu)", (unsigned long) bitno);
#endif
		trả_về 0;
	}

	nếu ((bitno < bitmap32->start) || (bitno > bitmap32->end)) {
		ext2fs_warn_bitmap2(bitmap, EXT2FS_TEST_ERROR, bitno);
		trả_về 0;
	}
	trả_về ext2fs_test_bit(bitno - bitmap32->start, bitmap32->bitmap);
}

số_nguyên ext2fs_mark_generic_bitmap(ext2fs_generic_bitmap bitmap,
					 __u32 bitno)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	nếu (!EXT2FS_IS_32_BITMAP(bitmap)) {
		nếu (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			trả_về ext2fs_mark_generic_bmap(bitmap, bitno);
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"mark_bitmap(%lu)", (unsigned long) bitno);
#endif
		trả_về 0;
	}

	nếu ((bitno < bitmap32->start) || (bitno > bitmap32->end)) {
		ext2fs_warn_bitmap2(bitmap, EXT2FS_MARK_ERROR, bitno);
		trả_về 0;
	}
	trả_về ext2fs_set_bit(bitno - bitmap32->start, bitmap32->bitmap);
}

số_nguyên ext2fs_unmark_generic_bitmap(ext2fs_generic_bitmap bitmap,
					   blk_t bitno)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	nếu (!EXT2FS_IS_32_BITMAP(bitmap)) {
		nếu (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			trả_về ext2fs_unmark_generic_bmap(bitmap, bitno);
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"mark_bitmap(%lu)", (unsigned long) bitno);
#endif
		trả_về 0;
	}

	nếu ((bitno < bitmap32->start) || (bitno > bitmap32->end)) {
		ext2fs_warn_bitmap2(bitmap, EXT2FS_UNMARK_ERROR, bitno);
		trả_về 0;
	}
	trả_về ext2fs_clear_bit(bitno - bitmap32->start, bitmap32->bitmap);
}

__u32 ext2fs_get_generic_bitmap_start(ext2fs_generic_bitmap bitmap)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	nếu (!EXT2FS_IS_32_BITMAP(bitmap)) {
		nếu (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			trả_về ext2fs_get_generic_bmap_start(bitmap);
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"get_bitmap_start");
#endif
		trả_về 0;
	}

	trả_về bitmap32->start;
}

__u32 ext2fs_get_generic_bitmap_end(ext2fs_generic_bitmap bitmap)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	nếu (!EXT2FS_IS_32_BITMAP(bitmap)) {
		nếu (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			trả_về ext2fs_get_generic_bmap_end(bitmap);
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"get_bitmap_end");
#endif
		trả_về 0;
	}
	trả_về bitmap32->end;
}

void ext2fs_clear_generic_bitmap(ext2fs_generic_bitmap bitmap)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	nếu (!EXT2FS_IS_32_BITMAP(bitmap)) {
		nếu (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			ext2fs_clear_generic_bmap(bitmap);
			trả_về;
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"clear_generic_bitmap");
#endif
		trả_về;
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

	nếu (end > bitmap->real_end)
		trả_về neq;
	nếu (oend)
		*oend = bitmap->end;
	bitmap->end = end;
	trả_về 0;
}

errcode_t ext2fs_resize_generic_bitmap(errcode_t magic,
				       __u32 new_end, __u32 new_real_end,
				       ext2fs_generic_bitmap gen_bmap)
{
	ext2fs_generic_bitmap_32 bmap = (ext2fs_generic_bitmap_32) gen_bmap;
	errcode_t	retval;
	size_t		size, new_size;
	__u32		bitno;

	nếu (!bmap || (bmap->magic != magic))
		trả_về magic;

	/*
	 * If we're expanding the bitmap, make sure all of the new
	 * parts of the bitmap are zero.
	 */
	nếu (new_end > bmap->end) {
		bitno = bmap->real_end;
		nếu (bitno > new_end)
			bitno = new_end;
		cho (; bitno > bmap->end; bitno--)
			ext2fs_clear_bit(bitno - bmap->start, bmap->bitmap);
	}
	nếu (new_real_end == bmap->real_end) {
		bmap->end = new_end;
		trả_về 0;
	}

	size = ((bmap->real_end - bmap->start) / 8) + 1;
	new_size = ((new_real_end - bmap->start) / 8) + 1;

	nếu (size != new_size) {
		retval = ext2fs_resize_mem(size, new_size, &bmap->bitmap);
		nếu (retval)
			trả_về retval;
	}
	nếu (new_size > size)
		memset(bmap->bitmap + size, 0, new_size - size);

	bmap->end = new_end;
	bmap->real_end = new_real_end;
	trả_về 0;
}

errcode_t ext2fs_compare_generic_bitmap(errcode_t magic, errcode_t neq,
					ext2fs_generic_bitmap gen_bm1,
					ext2fs_generic_bitmap gen_bm2)
{
	ext2fs_generic_bitmap_32 bm1 = (ext2fs_generic_bitmap_32) gen_bm1;
	ext2fs_generic_bitmap_32 bm2 = (ext2fs_generic_bitmap_32) gen_bm2;
	blk_t	i;

	nếu (!bm1 || bm1->magic != magic)
		trả_về magic;
	nếu (!bm2 || bm2->magic != magic)
		trả_về magic;

	nếu ((bm1->start != bm2->start) ||
	    (bm1->end != bm2->end) ||
	    (memcmp(bm1->bitmap, bm2->bitmap,
		    (size_t) (bm1->end - bm1->start)/8)))
		trả_về neq;

	cho (i = bm1->end - ((bm1->end - bm1->start) % 8); i <= bm1->end; i++)
		nếu (ext2fs_fast_test_block_bitmap(gen_bm1, i) !=
		    ext2fs_fast_test_block_bitmap(gen_bm2, i))
			trả_về neq;

	trả_về 0;
}

void ext2fs_set_generic_bitmap_padding(ext2fs_generic_bitmap gen_map)
{
	ext2fs_generic_bitmap_32 map = (ext2fs_generic_bitmap_32) gen_map;
	__u32	i, j;

	/* Protect loop from wrap-around if map->real_end is maxed */
	cho (i=map->end+1, j = i - map->start;
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

	nếu (!bmap || (bmap->magic != magic))
		trả_về magic;

	nếu ((start < bmap->start) || (start+num-1 > bmap->real_end))
		trả_về EXT2_ET_INVALID_ARGUMENT;

	memcpy(out, bmap->bitmap + ((start - bmap->start) >> 3), (num+7) >> 3);
	trả_về 0;
}

errcode_t ext2fs_set_generic_bitmap_range(ext2fs_generic_bitmap gen_bmap,
					  errcode_t magic,
					  __u32 start, __u32 num,
					  void *in)
{
	ext2fs_generic_bitmap_32 bmap = (ext2fs_generic_bitmap_32) gen_bmap;

	nếu (!bmap || (bmap->magic != magic))
		trả_về magic;

	nếu ((start < bmap->start) || (start+num-1 > bmap->real_end))
		trả_về EXT2_ET_INVALID_ARGUMENT;

	memcpy(bmap->bitmap + ((start - bmap->start) >> 3), in, (num+7) >> 3);
	trả_về 0;
}

/*
 * Compare @mem to zero buffer by 256 bytes.
 * Return 1 if @mem is zeroed memory, otherwise return 0.
 */
số_nguyên ext2fs_mem_is_zero(const char *mem, size_t len)
{
	static const char zero_buf[256];

	trong_khi (len >= sizeof(zero_buf)) {
		nếu (memcmp(mem, zero_buf, sizeof(zero_buf)))
			trả_về 0;
		len -= sizeof(zero_buf);
		mem += sizeof(zero_buf);
	}
	/* Deal with leftover bytes. */
	nếu (len)
		trả_về !memcmp(mem, zero_buf, len);
	trả_về 1;
}

/*
 * Return true if all of the bits in a specified range are clear
 */
static số_nguyên ext2fs_test_clear_generic_bitmap_range(ext2fs_generic_bitmap gen_bitmap,
						  unsigned số_nguyên start,
						  unsigned số_nguyên len)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	size_t start_byte, len_byte = len >> 3;
	unsigned số_nguyên start_bit, len_bit = len % 8;
	số_nguyên first_bit = 0;
	số_nguyên last_bit  = 0;
	số_nguyên mark_count = 0;
	số_nguyên mark_bit = 0;
	số_nguyên i;
	const char *ADDR = bitmap->bitmap;

	start -= bitmap->start;
	start_byte = start >> 3;
	start_bit = start % 8;

	nếu (start_bit != 0) {
		/*
		 * The compared start block number or start inode number
		 * is not the first bit in a byte.
		 */
		mark_count = 8 - start_bit;
		nếu (len < 8 - start_bit) {
			mark_count = (số_nguyên)len;
			mark_bit = len + start_bit - 1;
		} không_thì
			mark_bit = 7;

		cho (i = mark_count; i > 0; i--, mark_bit--)
			first_bit |= 1 << mark_bit;

		/*
		 * Compare blocks or inodes in the first byte.
		 * If there is any marked bit, this function returns 0.
		 */
		nếu (first_bit & ADDR[start_byte])
			trả_về 0;
		không_thì nếu (len <= 8 - start_bit)
			trả_về 1;

		start_byte++;
		len_bit = (len - mark_count) % 8;
		len_byte = (len - mark_count) >> 3;
	}

	/*
	 * The compared start block number or start inode number is
	 * the first bit in a byte.
	 */
	nếu (len_bit != 0) {
		/*
		 * The compared end block number or end inode number is
		 * not the last bit in a byte.
		 */
		cho (mark_bit = len_bit - 1; mark_bit >= 0; mark_bit--)
			last_bit |= 1 << mark_bit;

		/*
		 * Compare blocks or inodes in the last byte.
		 * If there is any marked bit, this function returns 0.
		 */
		nếu (last_bit & ADDR[start_byte + len_byte])
			trả_về 0;
		không_thì nếu (len_byte == 0)
			trả_về 1;
	}

	/* Check whether all bytes are 0 */
	trả_về ext2fs_mem_is_zero(ADDR + start_byte, len_byte);
}

errcode_t ext2fs_find_first_zero_generic_bitmap(ext2fs_generic_bitmap gen_bitmap,
						__u32 start, __u32 end,
						__u32 *out)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	blk_t b;

	nếu (start < bitmap->start || end > bitmap->end || start > end) {
		ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_TEST_ERROR, start);
		trả_về EINVAL;
	}

	trong_khi (start <= end) {
		b = ext2fs_test_bit(start - bitmap->start, bitmap->bitmap);
		nếu (!b) {
			*out = start;
			trả_về 0;
		}
		start++;
	}

	trả_về ENOENT;
}

errcode_t ext2fs_find_first_set_generic_bitmap(ext2fs_generic_bitmap gen_bitmap,
					       __u32 start, __u32 end,
					       __u32 *out)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	blk_t b;

	nếu (start < bitmap->start || end > bitmap->end || start > end) {
		ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_TEST_ERROR, start);
		trả_về EINVAL;
	}

	trong_khi (start <= end) {
		b = ext2fs_test_bit(start - bitmap->start, bitmap->bitmap);
		nếu (b) {
			*out = start;
			trả_về 0;
		}
		start++;
	}

	trả_về ENOENT;
}

số_nguyên ext2fs_test_block_bitmap_range(ext2fs_block_bitmap gen_bitmap,
				   blk_t block, số_nguyên num)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	EXT2_CHECK_MAGIC(bitmap, EXT2_ET_MAGIC_BLOCK_BITMAP);
	nếu ((block < bitmap->start) || (block > bitmap->real_end) ||
	    (block+num-1 > bitmap->real_end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_TEST,
				   block, bitmap->description);
		trả_về 0;
	}
	trả_về ext2fs_test_clear_generic_bitmap_range((ext2fs_generic_bitmap)
						      bitmap, block, num);
}

số_nguyên ext2fs_test_inode_bitmap_range(ext2fs_inode_bitmap gen_bitmap,
				   ext2_ino_t inode, số_nguyên num)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	EXT2_CHECK_MAGIC(bitmap, EXT2_ET_MAGIC_INODE_BITMAP);
	nếu ((inode < bitmap->start) || (inode > bitmap->real_end) ||
	    (inode+num-1 > bitmap->real_end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_INODE_TEST,
				   inode, bitmap->description);
		trả_về 0;
	}
	trả_về ext2fs_test_clear_generic_bitmap_range((ext2fs_generic_bitmap)
						      bitmap, inode, num);
}

void ext2fs_mark_block_bitmap_range(ext2fs_block_bitmap gen_bitmap,
				    blk_t block, số_nguyên num)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	số_nguyên	i;

	nếu ((block < bitmap->start) || (block > bitmap->end) ||
	    (block+num-1 > bitmap->end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_MARK, block,
				   bitmap->description);
		trả_về;
	}
	cho (i=0; i < num; i++)
		ext2fs_fast_set_bit(block + i - bitmap->start, bitmap->bitmap);
}

void ext2fs_unmark_block_bitmap_range(ext2fs_block_bitmap gen_bitmap,
				      blk_t block, số_nguyên num)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	số_nguyên	i;

	nếu ((block < bitmap->start) || (block > bitmap->end) ||
	    (block+num-1 > bitmap->end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_UNMARK, block,
				   bitmap->description);
		trả_về;
	}
	cho (i=0; i < num; i++)
		ext2fs_fast_clear_bit(block + i - bitmap->start,
				      bitmap->bitmap);
}

