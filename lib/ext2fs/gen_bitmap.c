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


#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#ܐܢ HAVE_UNISTD_H
#ضمن <unistd.h>
#endif
#ضمن <fcntl.h>
#ضمن <time.h>
#ܐܢ HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#endif
#ܐܢ HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#endif

#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"

بنية ext2fs_struct_generic_bitmap_32 {
	errcode_t	magic;
	ext2_filsys 	fs;
	__u32		start, end;
	__u32		real_end;
	حرف	*	description;
	حرف	*	bitmap;
	errcode_t	base_error_code;
	__u32		reserved[7];
};

تعريفنوع بنية ext2fs_struct_generic_bitmap_32 *ext2fs_generic_bitmap_32;

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
فراغ ext2fs_warn_bitmap2(ext2fs_generic_bitmap gen_bitmap,
			    صحيح code, unsigned long arg)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

#ifndef OMIT_COM_ERR
	ܐܢ (bitmap->description)
		com_err(0, bitmap->base_error_code+code,
			"#%lu for %s", arg, bitmap->description);
	ܐܚܪܢܐ
		com_err(0, bitmap->base_error_code + code, "#%lu", arg);
#endif
}

ساكن errcode_t check_magic(ext2fs_generic_bitmap bitmap)
{
	ܐܢ (!bitmap || !((bitmap->magic == EXT2_ET_MAGIC_GENERIC_BITMAP) ||
			 (bitmap->magic == EXT2_ET_MAGIC_INODE_BITMAP) ||
			 (bitmap->magic == EXT2_ET_MAGIC_BLOCK_BITMAP)))
		ܦܢܐ EXT2_ET_MAGIC_GENERIC_BITMAP;
	ܦܢܐ 0;
}

errcode_t ext2fs_make_generic_bitmap(errcode_t magic, ext2_filsys fs,
				     __u32 start, __u32 end, __u32 real_end,
				     ثابت حرف *descr, حرف *init_map,
				     ext2fs_generic_bitmap *ret)
{
	ext2fs_generic_bitmap_32 bitmap;
	errcode_t		retval;
	size_t			size;

	retval = ext2fs_get_mem(حجم(بنية ext2fs_struct_generic_bitmap_32),
				&bitmap);
	ܐܢ (retval)
		ܦܢܐ retval;

	bitmap->magic = magic;
	bitmap->fs = fs;
	bitmap->start = start;
	bitmap->end = end;
	bitmap->real_end = real_end;
	بدل (magic) {
	حالة EXT2_ET_MAGIC_INODE_BITMAP:
		bitmap->base_error_code = EXT2_ET_BAD_INODE_MARK;
		اكسر;
	حالة EXT2_ET_MAGIC_BLOCK_BITMAP:
		bitmap->base_error_code = EXT2_ET_BAD_BLOCK_MARK;
		اكسر;
	افتراضي:
		bitmap->base_error_code = EXT2_ET_BAD_GENERIC_MARK;
	}
	ܐܢ (descr) {
		retval = ext2fs_get_mem(strlen(descr)+1, &bitmap->description);
		ܐܢ (retval) {
			ext2fs_free_mem(&bitmap);
			ܦܢܐ retval;
		}
		strcpy(bitmap->description, descr);
	} ܐܚܪܢܐ
		bitmap->description = 0;

	size = (size_t) (((bitmap->real_end - bitmap->start) / 8) + 1);
	/* Round up to allow for the BT x86 instruction */
	size = (size + 7) & ~3;
	retval = ext2fs_get_mem(size, &bitmap->bitmap);
	ܐܢ (retval) {
		ext2fs_free_mem(&bitmap->description);
		ext2fs_free_mem(&bitmap);
		ܦܢܐ retval;
	}

	ܐܢ (init_map)
		memcpy(bitmap->bitmap, init_map, size);
	ܐܚܪܢܐ
		memset(bitmap->bitmap, 0, size);
	*ret = (ext2fs_generic_bitmap) bitmap;
	ܦܢܐ 0;
}

errcode_t ext2fs_allocate_generic_bitmap(__u32 start,
					 __u32 end,
					 __u32 real_end,
					 ثابت حرف *descr,
					 ext2fs_generic_bitmap *ret)
{
	ܦܢܐ ext2fs_make_generic_bitmap(EXT2_ET_MAGIC_GENERIC_BITMAP, 0,
					  start, end, real_end, descr, 0, ret);
}

errcode_t ext2fs_copy_generic_bitmap(ext2fs_generic_bitmap gen_src,
				     ext2fs_generic_bitmap *dest)
{
	ext2fs_generic_bitmap_32 src = (ext2fs_generic_bitmap_32) gen_src;

	ܦܢܐ (ext2fs_make_generic_bitmap(src->magic, src->fs,
					   src->start, src->end,
					   src->real_end,
					   src->description, src->bitmap,
					   dest));
}

فراغ ext2fs_free_generic_bitmap(ext2fs_inode_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	ܐܢ (check_magic(gen_bitmap))
		ܦܢܐ;

	bitmap->magic = 0;
	ܐܢ (bitmap->description) {
		ext2fs_free_mem(&bitmap->description);
		bitmap->description = 0;
	}
	ܐܢ (bitmap->bitmap) {
		ext2fs_free_mem(&bitmap->bitmap);
		bitmap->bitmap = 0;
	}
	ext2fs_free_mem(&bitmap);
}

صحيح ext2fs_test_generic_bitmap(ext2fs_generic_bitmap bitmap,
					blk_t bitno)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	ܐܢ (!EXT2FS_IS_32_BITMAP(bitmap)) {
		ܐܢ (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			ܦܢܐ ext2fs_test_generic_bmap(bitmap, bitno);
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"test_bitmap(%lu)", (unsigned long) bitno);
#endif
		ܦܢܐ 0;
	}

	ܐܢ ((bitno < bitmap32->start) || (bitno > bitmap32->end)) {
		ext2fs_warn_bitmap2(bitmap, EXT2FS_TEST_ERROR, bitno);
		ܦܢܐ 0;
	}
	ܦܢܐ ext2fs_test_bit(bitno - bitmap32->start, bitmap32->bitmap);
}

صحيح ext2fs_mark_generic_bitmap(ext2fs_generic_bitmap bitmap,
					 __u32 bitno)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	ܐܢ (!EXT2FS_IS_32_BITMAP(bitmap)) {
		ܐܢ (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			ܦܢܐ ext2fs_mark_generic_bmap(bitmap, bitno);
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"mark_bitmap(%lu)", (unsigned long) bitno);
#endif
		ܦܢܐ 0;
	}

	ܐܢ ((bitno < bitmap32->start) || (bitno > bitmap32->end)) {
		ext2fs_warn_bitmap2(bitmap, EXT2FS_MARK_ERROR, bitno);
		ܦܢܐ 0;
	}
	ܦܢܐ ext2fs_set_bit(bitno - bitmap32->start, bitmap32->bitmap);
}

صحيح ext2fs_unmark_generic_bitmap(ext2fs_generic_bitmap bitmap,
					   blk_t bitno)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	ܐܢ (!EXT2FS_IS_32_BITMAP(bitmap)) {
		ܐܢ (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			ܦܢܐ ext2fs_unmark_generic_bmap(bitmap, bitno);
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"mark_bitmap(%lu)", (unsigned long) bitno);
#endif
		ܦܢܐ 0;
	}

	ܐܢ ((bitno < bitmap32->start) || (bitno > bitmap32->end)) {
		ext2fs_warn_bitmap2(bitmap, EXT2FS_UNMARK_ERROR, bitno);
		ܦܢܐ 0;
	}
	ܦܢܐ ext2fs_clear_bit(bitno - bitmap32->start, bitmap32->bitmap);
}

__u32 ext2fs_get_generic_bitmap_start(ext2fs_generic_bitmap bitmap)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	ܐܢ (!EXT2FS_IS_32_BITMAP(bitmap)) {
		ܐܢ (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			ܦܢܐ ext2fs_get_generic_bmap_start(bitmap);
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"get_bitmap_start");
#endif
		ܦܢܐ 0;
	}

	ܦܢܐ bitmap32->start;
}

__u32 ext2fs_get_generic_bitmap_end(ext2fs_generic_bitmap bitmap)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	ܐܢ (!EXT2FS_IS_32_BITMAP(bitmap)) {
		ܐܢ (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			ܦܢܐ ext2fs_get_generic_bmap_end(bitmap);
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"get_bitmap_end");
#endif
		ܦܢܐ 0;
	}
	ܦܢܐ bitmap32->end;
}

فراغ ext2fs_clear_generic_bitmap(ext2fs_generic_bitmap bitmap)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	ܐܢ (!EXT2FS_IS_32_BITMAP(bitmap)) {
		ܐܢ (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			ext2fs_clear_generic_bmap(bitmap);
			ܦܢܐ;
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"clear_generic_bitmap");
#endif
		ܦܢܐ;
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

	ܐܢ (end > bitmap->real_end)
		ܦܢܐ neq;
	ܐܢ (oend)
		*oend = bitmap->end;
	bitmap->end = end;
	ܦܢܐ 0;
}

errcode_t ext2fs_resize_generic_bitmap(errcode_t magic,
				       __u32 new_end, __u32 new_real_end,
				       ext2fs_generic_bitmap gen_bmap)
{
	ext2fs_generic_bitmap_32 bmap = (ext2fs_generic_bitmap_32) gen_bmap;
	errcode_t	retval;
	size_t		size, new_size;
	__u32		bitno;

	ܐܢ (!bmap || (bmap->magic != magic))
		ܦܢܐ magic;

	/*
	 * If we're expanding the bitmap, make sure all of the new
	 * parts of the bitmap are zero.
	 */
	ܐܢ (new_end > bmap->end) {
		bitno = bmap->real_end;
		ܐܢ (bitno > new_end)
			bitno = new_end;
		لكل (; bitno > bmap->end; bitno--)
			ext2fs_clear_bit(bitno - bmap->start, bmap->bitmap);
	}
	ܐܢ (new_real_end == bmap->real_end) {
		bmap->end = new_end;
		ܦܢܐ 0;
	}

	size = ((bmap->real_end - bmap->start) / 8) + 1;
	new_size = ((new_real_end - bmap->start) / 8) + 1;

	ܐܢ (size != new_size) {
		retval = ext2fs_resize_mem(size, new_size, &bmap->bitmap);
		ܐܢ (retval)
			ܦܢܐ retval;
	}
	ܐܢ (new_size > size)
		memset(bmap->bitmap + size, 0, new_size - size);

	bmap->end = new_end;
	bmap->real_end = new_real_end;
	ܦܢܐ 0;
}

errcode_t ext2fs_compare_generic_bitmap(errcode_t magic, errcode_t neq,
					ext2fs_generic_bitmap gen_bm1,
					ext2fs_generic_bitmap gen_bm2)
{
	ext2fs_generic_bitmap_32 bm1 = (ext2fs_generic_bitmap_32) gen_bm1;
	ext2fs_generic_bitmap_32 bm2 = (ext2fs_generic_bitmap_32) gen_bm2;
	blk_t	i;

	ܐܢ (!bm1 || bm1->magic != magic)
		ܦܢܐ magic;
	ܐܢ (!bm2 || bm2->magic != magic)
		ܦܢܐ magic;

	ܐܢ ((bm1->start != bm2->start) ||
	    (bm1->end != bm2->end) ||
	    (memcmp(bm1->bitmap, bm2->bitmap,
		    (size_t) (bm1->end - bm1->start)/8)))
		ܦܢܐ neq;

	لكل (i = bm1->end - ((bm1->end - bm1->start) % 8); i <= bm1->end; i++)
		ܐܢ (ext2fs_fast_test_block_bitmap(gen_bm1, i) !=
		    ext2fs_fast_test_block_bitmap(gen_bm2, i))
			ܦܢܐ neq;

	ܦܢܐ 0;
}

فراغ ext2fs_set_generic_bitmap_padding(ext2fs_generic_bitmap gen_map)
{
	ext2fs_generic_bitmap_32 map = (ext2fs_generic_bitmap_32) gen_map;
	__u32	i, j;

	/* Protect loop from wrap-around if map->real_end is maxed */
	لكل (i=map->end+1, j = i - map->start;
	     i <= map->real_end && i > map->end;
	     i++, j++)
		ext2fs_set_bit(j, map->bitmap);
}

errcode_t ext2fs_get_generic_bitmap_range(ext2fs_generic_bitmap gen_bmap,
					  errcode_t magic,
					  __u32 start, __u32 num,
					  فراغ *out)
{
	ext2fs_generic_bitmap_32 bmap = (ext2fs_generic_bitmap_32) gen_bmap;

	ܐܢ (!bmap || (bmap->magic != magic))
		ܦܢܐ magic;

	ܐܢ ((start < bmap->start) || (start+num-1 > bmap->real_end))
		ܦܢܐ EXT2_ET_INVALID_ARGUMENT;

	memcpy(out, bmap->bitmap + ((start - bmap->start) >> 3), (num+7) >> 3);
	ܦܢܐ 0;
}

errcode_t ext2fs_set_generic_bitmap_range(ext2fs_generic_bitmap gen_bmap,
					  errcode_t magic,
					  __u32 start, __u32 num,
					  فراغ *in)
{
	ext2fs_generic_bitmap_32 bmap = (ext2fs_generic_bitmap_32) gen_bmap;

	ܐܢ (!bmap || (bmap->magic != magic))
		ܦܢܐ magic;

	ܐܢ ((start < bmap->start) || (start+num-1 > bmap->real_end))
		ܦܢܐ EXT2_ET_INVALID_ARGUMENT;

	memcpy(bmap->bitmap + ((start - bmap->start) >> 3), in, (num+7) >> 3);
	ܦܢܐ 0;
}

/*
 * Compare @mem to zero buffer by 256 bytes.
 * Return 1 if @mem is zeroed memory, otherwise return 0.
 */
صحيح ext2fs_mem_is_zero(ثابت حرف *mem, size_t len)
{
	ساكن ثابت حرف zero_buf[256];

	ܟܡܐ (len >= حجم(zero_buf)) {
		ܐܢ (memcmp(mem, zero_buf, حجم(zero_buf)))
			ܦܢܐ 0;
		len -= حجم(zero_buf);
		mem += حجم(zero_buf);
	}
	/* Deal with leftover bytes. */
	ܐܢ (len)
		ܦܢܐ !memcmp(mem, zero_buf, len);
	ܦܢܐ 1;
}

/*
 * Return true if all of the bits in a specified range are clear
 */
ساكن صحيح ext2fs_test_clear_generic_bitmap_range(ext2fs_generic_bitmap gen_bitmap,
						  unsigned صحيح start,
						  unsigned صحيح len)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	size_t start_byte, len_byte = len >> 3;
	unsigned صحيح start_bit, len_bit = len % 8;
	صحيح first_bit = 0;
	صحيح last_bit  = 0;
	صحيح mark_count = 0;
	صحيح mark_bit = 0;
	صحيح i;
	ثابت حرف *ADDR = bitmap->bitmap;

	start -= bitmap->start;
	start_byte = start >> 3;
	start_bit = start % 8;

	ܐܢ (start_bit != 0) {
		/*
		 * The compared start block number or start inode number
		 * is not the first bit in a byte.
		 */
		mark_count = 8 - start_bit;
		ܐܢ (len < 8 - start_bit) {
			mark_count = (صحيح)len;
			mark_bit = len + start_bit - 1;
		} ܐܚܪܢܐ
			mark_bit = 7;

		لكل (i = mark_count; i > 0; i--, mark_bit--)
			first_bit |= 1 << mark_bit;

		/*
		 * Compare blocks or inodes in the first byte.
		 * If there is any marked bit, this function returns 0.
		 */
		ܐܢ (first_bit & ADDR[start_byte])
			ܦܢܐ 0;
		ܐܚܪܢܐ ܐܢ (len <= 8 - start_bit)
			ܦܢܐ 1;

		start_byte++;
		len_bit = (len - mark_count) % 8;
		len_byte = (len - mark_count) >> 3;
	}

	/*
	 * The compared start block number or start inode number is
	 * the first bit in a byte.
	 */
	ܐܢ (len_bit != 0) {
		/*
		 * The compared end block number or end inode number is
		 * not the last bit in a byte.
		 */
		لكل (mark_bit = len_bit - 1; mark_bit >= 0; mark_bit--)
			last_bit |= 1 << mark_bit;

		/*
		 * Compare blocks or inodes in the last byte.
		 * If there is any marked bit, this function returns 0.
		 */
		ܐܢ (last_bit & ADDR[start_byte + len_byte])
			ܦܢܐ 0;
		ܐܚܪܢܐ ܐܢ (len_byte == 0)
			ܦܢܐ 1;
	}

	/* Check whether all bytes are 0 */
	ܦܢܐ ext2fs_mem_is_zero(ADDR + start_byte, len_byte);
}

errcode_t ext2fs_find_first_zero_generic_bitmap(ext2fs_generic_bitmap gen_bitmap,
						__u32 start, __u32 end,
						__u32 *out)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	blk_t b;

	ܐܢ (start < bitmap->start || end > bitmap->end || start > end) {
		ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_TEST_ERROR, start);
		ܦܢܐ EINVAL;
	}

	ܟܡܐ (start <= end) {
		b = ext2fs_test_bit(start - bitmap->start, bitmap->bitmap);
		ܐܢ (!b) {
			*out = start;
			ܦܢܐ 0;
		}
		start++;
	}

	ܦܢܐ ENOENT;
}

errcode_t ext2fs_find_first_set_generic_bitmap(ext2fs_generic_bitmap gen_bitmap,
					       __u32 start, __u32 end,
					       __u32 *out)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	blk_t b;

	ܐܢ (start < bitmap->start || end > bitmap->end || start > end) {
		ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_TEST_ERROR, start);
		ܦܢܐ EINVAL;
	}

	ܟܡܐ (start <= end) {
		b = ext2fs_test_bit(start - bitmap->start, bitmap->bitmap);
		ܐܢ (b) {
			*out = start;
			ܦܢܐ 0;
		}
		start++;
	}

	ܦܢܐ ENOENT;
}

صحيح ext2fs_test_block_bitmap_range(ext2fs_block_bitmap gen_bitmap,
				   blk_t block, صحيح num)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	EXT2_CHECK_MAGIC(bitmap, EXT2_ET_MAGIC_BLOCK_BITMAP);
	ܐܢ ((block < bitmap->start) || (block > bitmap->real_end) ||
	    (block+num-1 > bitmap->real_end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_TEST,
				   block, bitmap->description);
		ܦܢܐ 0;
	}
	ܦܢܐ ext2fs_test_clear_generic_bitmap_range((ext2fs_generic_bitmap)
						      bitmap, block, num);
}

صحيح ext2fs_test_inode_bitmap_range(ext2fs_inode_bitmap gen_bitmap,
				   ext2_ino_t inode, صحيح num)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	EXT2_CHECK_MAGIC(bitmap, EXT2_ET_MAGIC_INODE_BITMAP);
	ܐܢ ((inode < bitmap->start) || (inode > bitmap->real_end) ||
	    (inode+num-1 > bitmap->real_end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_INODE_TEST,
				   inode, bitmap->description);
		ܦܢܐ 0;
	}
	ܦܢܐ ext2fs_test_clear_generic_bitmap_range((ext2fs_generic_bitmap)
						      bitmap, inode, num);
}

فراغ ext2fs_mark_block_bitmap_range(ext2fs_block_bitmap gen_bitmap,
				    blk_t block, صحيح num)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	صحيح	i;

	ܐܢ ((block < bitmap->start) || (block > bitmap->end) ||
	    (block+num-1 > bitmap->end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_MARK, block,
				   bitmap->description);
		ܦܢܐ;
	}
	لكل (i=0; i < num; i++)
		ext2fs_fast_set_bit(block + i - bitmap->start, bitmap->bitmap);
}

فراغ ext2fs_unmark_block_bitmap_range(ext2fs_block_bitmap gen_bitmap,
				      blk_t block, صحيح num)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	صحيح	i;

	ܐܢ ((block < bitmap->start) || (block > bitmap->end) ||
	    (block+num-1 > bitmap->end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_UNMARK, block,
				   bitmap->description);
		ܦܢܐ;
	}
	لكل (i=0; i < num; i++)
		ext2fs_fast_clear_bit(block + i - bitmap->start,
				      bitmap->bitmap);
}

