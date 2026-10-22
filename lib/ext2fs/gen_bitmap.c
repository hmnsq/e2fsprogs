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


#அந்தர்பாவ "config.h"
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <string.h>
#எனில் HAVE_UNISTD_H
#அந்தர்பாவ <unistd.h>
#endif
#அந்தர்பாவ <fcntl.h>
#அந்தர்பாவ <time.h>
#எனில் HAVE_SYS_STAT_H
#அந்தர்பாவ <sys/stat.h>
#endif
#எனில் HAVE_SYS_TYPES_H
#அந்தர்பாவ <sys/types.h>
#endif

#அந்தர்பாவ "ext2_fs.h"
#அந்தர்பாவ "ext2fsP.h"

கட்டமைப்பு ext2fs_struct_generic_bitmap_32 {
	errcode_t	magic;
	ext2_filsys 	fs;
	__u32		start, end;
	__u32		real_end;
	எழுத்து	*	description;
	எழுத்து	*	bitmap;
	errcode_t	base_error_code;
	__u32		reserved[7];
};

வகைவரை கட்டமைப்பு ext2fs_struct_generic_bitmap_32 *ext2fs_generic_bitmap_32;

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
வெறுமை ext2fs_warn_bitmap2(ext2fs_generic_bitmap gen_bitmap,
			    முழு code, unsigned long arg)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

#ifndef OMIT_COM_ERR
	எனில் (bitmap->description)
		com_err(0, bitmap->base_error_code+code,
			"#%lu for %s", arg, bitmap->description);
	இல்லையெனில்
		com_err(0, bitmap->base_error_code + code, "#%lu", arg);
#endif
}

நிலையான errcode_t check_magic(ext2fs_generic_bitmap bitmap)
{
	எனில் (!bitmap || !((bitmap->magic == EXT2_ET_MAGIC_GENERIC_BITMAP) ||
			 (bitmap->magic == EXT2_ET_MAGIC_INODE_BITMAP) ||
			 (bitmap->magic == EXT2_ET_MAGIC_BLOCK_BITMAP)))
		திரும்பு EXT2_ET_MAGIC_GENERIC_BITMAP;
	திரும்பு 0;
}

errcode_t ext2fs_make_generic_bitmap(errcode_t magic, ext2_filsys fs,
				     __u32 start, __u32 end, __u32 real_end,
				     மாறிலி எழுத்து *descr, எழுத்து *init_map,
				     ext2fs_generic_bitmap *ret)
{
	ext2fs_generic_bitmap_32 bitmap;
	errcode_t		retval;
	size_t			size;

	retval = ext2fs_get_mem(அளவு(கட்டமைப்பு ext2fs_struct_generic_bitmap_32),
				&bitmap);
	எனில் (retval)
		திரும்பு retval;

	bitmap->magic = magic;
	bitmap->fs = fs;
	bitmap->start = start;
	bitmap->end = end;
	bitmap->real_end = real_end;
	தேர்வு (magic) {
	நிலை EXT2_ET_MAGIC_INODE_BITMAP:
		bitmap->base_error_code = EXT2_ET_BAD_INODE_MARK;
		நிறுத்து;
	நிலை EXT2_ET_MAGIC_BLOCK_BITMAP:
		bitmap->base_error_code = EXT2_ET_BAD_BLOCK_MARK;
		நிறுத்து;
	இயல்பு:
		bitmap->base_error_code = EXT2_ET_BAD_GENERIC_MARK;
	}
	எனில் (descr) {
		retval = ext2fs_get_mem(strlen(descr)+1, &bitmap->description);
		எனில் (retval) {
			ext2fs_free_mem(&bitmap);
			திரும்பு retval;
		}
		strcpy(bitmap->description, descr);
	} இல்லையெனில்
		bitmap->description = 0;

	size = (size_t) (((bitmap->real_end - bitmap->start) / 8) + 1);
	/* Round up to allow for the BT x86 instruction */
	size = (size + 7) & ~3;
	retval = ext2fs_get_mem(size, &bitmap->bitmap);
	எனில் (retval) {
		ext2fs_free_mem(&bitmap->description);
		ext2fs_free_mem(&bitmap);
		திரும்பு retval;
	}

	எனில் (init_map)
		memcpy(bitmap->bitmap, init_map, size);
	இல்லையெனில்
		memset(bitmap->bitmap, 0, size);
	*ret = (ext2fs_generic_bitmap) bitmap;
	திரும்பு 0;
}

errcode_t ext2fs_allocate_generic_bitmap(__u32 start,
					 __u32 end,
					 __u32 real_end,
					 மாறிலி எழுத்து *descr,
					 ext2fs_generic_bitmap *ret)
{
	திரும்பு ext2fs_make_generic_bitmap(EXT2_ET_MAGIC_GENERIC_BITMAP, 0,
					  start, end, real_end, descr, 0, ret);
}

errcode_t ext2fs_copy_generic_bitmap(ext2fs_generic_bitmap gen_src,
				     ext2fs_generic_bitmap *dest)
{
	ext2fs_generic_bitmap_32 src = (ext2fs_generic_bitmap_32) gen_src;

	திரும்பு (ext2fs_make_generic_bitmap(src->magic, src->fs,
					   src->start, src->end,
					   src->real_end,
					   src->description, src->bitmap,
					   dest));
}

வெறுமை ext2fs_free_generic_bitmap(ext2fs_inode_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	எனில் (check_magic(gen_bitmap))
		திரும்பு;

	bitmap->magic = 0;
	எனில் (bitmap->description) {
		ext2fs_free_mem(&bitmap->description);
		bitmap->description = 0;
	}
	எனில் (bitmap->bitmap) {
		ext2fs_free_mem(&bitmap->bitmap);
		bitmap->bitmap = 0;
	}
	ext2fs_free_mem(&bitmap);
}

முழு ext2fs_test_generic_bitmap(ext2fs_generic_bitmap bitmap,
					blk_t bitno)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	எனில் (!EXT2FS_IS_32_BITMAP(bitmap)) {
		எனில் (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			திரும்பு ext2fs_test_generic_bmap(bitmap, bitno);
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"test_bitmap(%lu)", (unsigned long) bitno);
#endif
		திரும்பு 0;
	}

	எனில் ((bitno < bitmap32->start) || (bitno > bitmap32->end)) {
		ext2fs_warn_bitmap2(bitmap, EXT2FS_TEST_ERROR, bitno);
		திரும்பு 0;
	}
	திரும்பு ext2fs_test_bit(bitno - bitmap32->start, bitmap32->bitmap);
}

முழு ext2fs_mark_generic_bitmap(ext2fs_generic_bitmap bitmap,
					 __u32 bitno)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	எனில் (!EXT2FS_IS_32_BITMAP(bitmap)) {
		எனில் (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			திரும்பு ext2fs_mark_generic_bmap(bitmap, bitno);
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"mark_bitmap(%lu)", (unsigned long) bitno);
#endif
		திரும்பு 0;
	}

	எனில் ((bitno < bitmap32->start) || (bitno > bitmap32->end)) {
		ext2fs_warn_bitmap2(bitmap, EXT2FS_MARK_ERROR, bitno);
		திரும்பு 0;
	}
	திரும்பு ext2fs_set_bit(bitno - bitmap32->start, bitmap32->bitmap);
}

முழு ext2fs_unmark_generic_bitmap(ext2fs_generic_bitmap bitmap,
					   blk_t bitno)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	எனில் (!EXT2FS_IS_32_BITMAP(bitmap)) {
		எனில் (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			திரும்பு ext2fs_unmark_generic_bmap(bitmap, bitno);
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"mark_bitmap(%lu)", (unsigned long) bitno);
#endif
		திரும்பு 0;
	}

	எனில் ((bitno < bitmap32->start) || (bitno > bitmap32->end)) {
		ext2fs_warn_bitmap2(bitmap, EXT2FS_UNMARK_ERROR, bitno);
		திரும்பு 0;
	}
	திரும்பு ext2fs_clear_bit(bitno - bitmap32->start, bitmap32->bitmap);
}

__u32 ext2fs_get_generic_bitmap_start(ext2fs_generic_bitmap bitmap)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	எனில் (!EXT2FS_IS_32_BITMAP(bitmap)) {
		எனில் (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			திரும்பு ext2fs_get_generic_bmap_start(bitmap);
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"get_bitmap_start");
#endif
		திரும்பு 0;
	}

	திரும்பு bitmap32->start;
}

__u32 ext2fs_get_generic_bitmap_end(ext2fs_generic_bitmap bitmap)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	எனில் (!EXT2FS_IS_32_BITMAP(bitmap)) {
		எனில் (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			திரும்பு ext2fs_get_generic_bmap_end(bitmap);
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"get_bitmap_end");
#endif
		திரும்பு 0;
	}
	திரும்பு bitmap32->end;
}

வெறுமை ext2fs_clear_generic_bitmap(ext2fs_generic_bitmap bitmap)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	எனில் (!EXT2FS_IS_32_BITMAP(bitmap)) {
		எனில் (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			ext2fs_clear_generic_bmap(bitmap);
			திரும்பு;
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"clear_generic_bitmap");
#endif
		திரும்பு;
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

	எனில் (end > bitmap->real_end)
		திரும்பு neq;
	எனில் (oend)
		*oend = bitmap->end;
	bitmap->end = end;
	திரும்பு 0;
}

errcode_t ext2fs_resize_generic_bitmap(errcode_t magic,
				       __u32 new_end, __u32 new_real_end,
				       ext2fs_generic_bitmap gen_bmap)
{
	ext2fs_generic_bitmap_32 bmap = (ext2fs_generic_bitmap_32) gen_bmap;
	errcode_t	retval;
	size_t		size, new_size;
	__u32		bitno;

	எனில் (!bmap || (bmap->magic != magic))
		திரும்பு magic;

	/*
	 * If we're expanding the bitmap, make sure all of the new
	 * parts of the bitmap are zero.
	 */
	எனில் (new_end > bmap->end) {
		bitno = bmap->real_end;
		எனில் (bitno > new_end)
			bitno = new_end;
		க்கு (; bitno > bmap->end; bitno--)
			ext2fs_clear_bit(bitno - bmap->start, bmap->bitmap);
	}
	எனில் (new_real_end == bmap->real_end) {
		bmap->end = new_end;
		திரும்பு 0;
	}

	size = ((bmap->real_end - bmap->start) / 8) + 1;
	new_size = ((new_real_end - bmap->start) / 8) + 1;

	எனில் (size != new_size) {
		retval = ext2fs_resize_mem(size, new_size, &bmap->bitmap);
		எனில் (retval)
			திரும்பு retval;
	}
	எனில் (new_size > size)
		memset(bmap->bitmap + size, 0, new_size - size);

	bmap->end = new_end;
	bmap->real_end = new_real_end;
	திரும்பு 0;
}

errcode_t ext2fs_compare_generic_bitmap(errcode_t magic, errcode_t neq,
					ext2fs_generic_bitmap gen_bm1,
					ext2fs_generic_bitmap gen_bm2)
{
	ext2fs_generic_bitmap_32 bm1 = (ext2fs_generic_bitmap_32) gen_bm1;
	ext2fs_generic_bitmap_32 bm2 = (ext2fs_generic_bitmap_32) gen_bm2;
	blk_t	i;

	எனில் (!bm1 || bm1->magic != magic)
		திரும்பு magic;
	எனில் (!bm2 || bm2->magic != magic)
		திரும்பு magic;

	எனில் ((bm1->start != bm2->start) ||
	    (bm1->end != bm2->end) ||
	    (memcmp(bm1->bitmap, bm2->bitmap,
		    (size_t) (bm1->end - bm1->start)/8)))
		திரும்பு neq;

	க்கு (i = bm1->end - ((bm1->end - bm1->start) % 8); i <= bm1->end; i++)
		எனில் (ext2fs_fast_test_block_bitmap(gen_bm1, i) !=
		    ext2fs_fast_test_block_bitmap(gen_bm2, i))
			திரும்பு neq;

	திரும்பு 0;
}

வெறுமை ext2fs_set_generic_bitmap_padding(ext2fs_generic_bitmap gen_map)
{
	ext2fs_generic_bitmap_32 map = (ext2fs_generic_bitmap_32) gen_map;
	__u32	i, j;

	/* Protect loop from wrap-around if map->real_end is maxed */
	க்கு (i=map->end+1, j = i - map->start;
	     i <= map->real_end && i > map->end;
	     i++, j++)
		ext2fs_set_bit(j, map->bitmap);
}

errcode_t ext2fs_get_generic_bitmap_range(ext2fs_generic_bitmap gen_bmap,
					  errcode_t magic,
					  __u32 start, __u32 இலக்கம்,
					  வெறுமை *out)
{
	ext2fs_generic_bitmap_32 bmap = (ext2fs_generic_bitmap_32) gen_bmap;

	எனில் (!bmap || (bmap->magic != magic))
		திரும்பு magic;

	எனில் ((start < bmap->start) || (start+இலக்கம்-1 > bmap->real_end))
		திரும்பு EXT2_ET_INVALID_ARGUMENT;

	memcpy(out, bmap->bitmap + ((start - bmap->start) >> 3), (இலக்கம்+7) >> 3);
	திரும்பு 0;
}

errcode_t ext2fs_set_generic_bitmap_range(ext2fs_generic_bitmap gen_bmap,
					  errcode_t magic,
					  __u32 start, __u32 இலக்கம்,
					  வெறுமை *in)
{
	ext2fs_generic_bitmap_32 bmap = (ext2fs_generic_bitmap_32) gen_bmap;

	எனில் (!bmap || (bmap->magic != magic))
		திரும்பு magic;

	எனில் ((start < bmap->start) || (start+இலக்கம்-1 > bmap->real_end))
		திரும்பு EXT2_ET_INVALID_ARGUMENT;

	memcpy(bmap->bitmap + ((start - bmap->start) >> 3), in, (இலக்கம்+7) >> 3);
	திரும்பு 0;
}

/*
 * Compare @mem to zero buffer by 256 bytes.
 * Return 1 if @mem is zeroed memory, otherwise return 0.
 */
முழு ext2fs_mem_is_zero(மாறிலி எழுத்து *mem, size_t len)
{
	நிலையான மாறிலி எழுத்து zero_buf[256];

	போது (len >= அளவு(zero_buf)) {
		எனில் (memcmp(mem, zero_buf, அளவு(zero_buf)))
			திரும்பு 0;
		len -= அளவு(zero_buf);
		mem += அளவு(zero_buf);
	}
	/* Deal with leftover bytes. */
	எனில் (len)
		திரும்பு !memcmp(mem, zero_buf, len);
	திரும்பு 1;
}

/*
 * Return true if all of the bits in a specified range are clear
 */
நிலையான முழு ext2fs_test_clear_generic_bitmap_range(ext2fs_generic_bitmap gen_bitmap,
						  unsigned முழு start,
						  unsigned முழு len)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	size_t start_byte, len_byte = len >> 3;
	unsigned முழு start_bit, len_bit = len % 8;
	முழு first_bit = 0;
	முழு last_bit  = 0;
	முழு mark_மொத்தம் = 0;
	முழு mark_bit = 0;
	முழு i;
	மாறிலி எழுத்து *ADDR = bitmap->bitmap;

	start -= bitmap->start;
	start_byte = start >> 3;
	start_bit = start % 8;

	எனில் (start_bit != 0) {
		/*
		 * The compared start block number or start inode number
		 * is not the first bit in a byte.
		 */
		mark_மொத்தம் = 8 - start_bit;
		எனில் (len < 8 - start_bit) {
			mark_மொத்தம் = (முழு)len;
			mark_bit = len + start_bit - 1;
		} இல்லையெனில்
			mark_bit = 7;

		க்கு (i = mark_மொத்தம்; i > 0; i--, mark_bit--)
			first_bit |= 1 << mark_bit;

		/*
		 * Compare blocks or inodes in the first byte.
		 * If there is any marked bit, this function returns 0.
		 */
		எனில் (first_bit & ADDR[start_byte])
			திரும்பு 0;
		இல்லையெனில் எனில் (len <= 8 - start_bit)
			திரும்பு 1;

		start_byte++;
		len_bit = (len - mark_மொத்தம்) % 8;
		len_byte = (len - mark_மொத்தம்) >> 3;
	}

	/*
	 * The compared start block number or start inode number is
	 * the first bit in a byte.
	 */
	எனில் (len_bit != 0) {
		/*
		 * The compared end block number or end inode number is
		 * not the last bit in a byte.
		 */
		க்கு (mark_bit = len_bit - 1; mark_bit >= 0; mark_bit--)
			last_bit |= 1 << mark_bit;

		/*
		 * Compare blocks or inodes in the last byte.
		 * If there is any marked bit, this function returns 0.
		 */
		எனில் (last_bit & ADDR[start_byte + len_byte])
			திரும்பு 0;
		இல்லையெனில் எனில் (len_byte == 0)
			திரும்பு 1;
	}

	/* Check whether all bytes are 0 */
	திரும்பு ext2fs_mem_is_zero(ADDR + start_byte, len_byte);
}

errcode_t ext2fs_find_first_zero_generic_bitmap(ext2fs_generic_bitmap gen_bitmap,
						__u32 start, __u32 end,
						__u32 *out)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	blk_t b;

	எனில் (start < bitmap->start || end > bitmap->end || start > end) {
		ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_TEST_ERROR, start);
		திரும்பு EINVAL;
	}

	போது (start <= end) {
		b = ext2fs_test_bit(start - bitmap->start, bitmap->bitmap);
		எனில் (!b) {
			*out = start;
			திரும்பு 0;
		}
		start++;
	}

	திரும்பு ENOENT;
}

errcode_t ext2fs_find_first_set_generic_bitmap(ext2fs_generic_bitmap gen_bitmap,
					       __u32 start, __u32 end,
					       __u32 *out)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	blk_t b;

	எனில் (start < bitmap->start || end > bitmap->end || start > end) {
		ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_TEST_ERROR, start);
		திரும்பு EINVAL;
	}

	போது (start <= end) {
		b = ext2fs_test_bit(start - bitmap->start, bitmap->bitmap);
		எனில் (b) {
			*out = start;
			திரும்பு 0;
		}
		start++;
	}

	திரும்பு ENOENT;
}

முழு ext2fs_test_block_bitmap_range(ext2fs_block_bitmap gen_bitmap,
				   blk_t block, முழு இலக்கம்)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	EXT2_CHECK_MAGIC(bitmap, EXT2_ET_MAGIC_BLOCK_BITMAP);
	எனில் ((block < bitmap->start) || (block > bitmap->real_end) ||
	    (block+இலக்கம்-1 > bitmap->real_end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_TEST,
				   block, bitmap->description);
		திரும்பு 0;
	}
	திரும்பு ext2fs_test_clear_generic_bitmap_range((ext2fs_generic_bitmap)
						      bitmap, block, இலக்கம்);
}

முழு ext2fs_test_inode_bitmap_range(ext2fs_inode_bitmap gen_bitmap,
				   ext2_ino_t inode, முழு இலக்கம்)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	EXT2_CHECK_MAGIC(bitmap, EXT2_ET_MAGIC_INODE_BITMAP);
	எனில் ((inode < bitmap->start) || (inode > bitmap->real_end) ||
	    (inode+இலக்கம்-1 > bitmap->real_end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_INODE_TEST,
				   inode, bitmap->description);
		திரும்பு 0;
	}
	திரும்பு ext2fs_test_clear_generic_bitmap_range((ext2fs_generic_bitmap)
						      bitmap, inode, இலக்கம்);
}

வெறுமை ext2fs_mark_block_bitmap_range(ext2fs_block_bitmap gen_bitmap,
				    blk_t block, முழு இலக்கம்)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	முழு	i;

	எனில் ((block < bitmap->start) || (block > bitmap->end) ||
	    (block+இலக்கம்-1 > bitmap->end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_MARK, block,
				   bitmap->description);
		திரும்பு;
	}
	க்கு (i=0; i < இலக்கம்; i++)
		ext2fs_fast_set_bit(block + i - bitmap->start, bitmap->bitmap);
}

வெறுமை ext2fs_unmark_block_bitmap_range(ext2fs_block_bitmap gen_bitmap,
				      blk_t block, முழு இலக்கம்)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	முழு	i;

	எனில் ((block < bitmap->start) || (block > bitmap->end) ||
	    (block+இலக்கம்-1 > bitmap->end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_UNMARK, block,
				   bitmap->description);
		திரும்பு;
	}
	க்கு (i=0; i < இலக்கம்; i++)
		ext2fs_fast_clear_bit(block + i - bitmap->start,
				      bitmap->bitmap);
}

