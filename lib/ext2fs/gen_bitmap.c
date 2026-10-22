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


#অন্তর্ভাব "config.h"
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <string.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাব <unistd.h>
#endif
#অন্তর্ভাব <fcntl.h>
#অন্তর্ভাব <time.h>
#যদি HAVE_SYS_STAT_H
#অন্তর্ভাব <sys/stat.h>
#endif
#যদি HAVE_SYS_TYPES_H
#অন্তর্ভাব <sys/types.h>
#endif

#অন্তর্ভাব "ext2_fs.h"
#অন্তর্ভাব "ext2fsP.h"

কাঠামো ext2fs_struct_generic_bitmap_32 {
	errcode_t	magic;
	ext2_filsys 	fs;
	__u32		start, end;
	__u32		real_end;
	অক্ষর	*	description;
	অক্ষর	*	bitmap;
	errcode_t	base_error_code;
	__u32		reserved[7];
};

প্রকারলেখ কাঠামো ext2fs_struct_generic_bitmap_32 *ext2fs_generic_bitmap_32;

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
শূন্য ext2fs_warn_bitmap2(ext2fs_generic_bitmap gen_bitmap,
			    পূর্ণসংখ্যা code, unsigned long arg)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

#ifndef OMIT_COM_ERR
	যদি (bitmap->description)
		com_err(0, bitmap->base_error_code+code,
			"#%lu for %s", arg, bitmap->description);
	অন্যথা
		com_err(0, bitmap->base_error_code + code, "#%lu", arg);
#endif
}

স্থির errcode_t check_magic(ext2fs_generic_bitmap bitmap)
{
	যদি (!bitmap || !((bitmap->magic == EXT2_ET_MAGIC_GENERIC_BITMAP) ||
			 (bitmap->magic == EXT2_ET_MAGIC_INODE_BITMAP) ||
			 (bitmap->magic == EXT2_ET_MAGIC_BLOCK_BITMAP)))
		ফেরত EXT2_ET_MAGIC_GENERIC_BITMAP;
	ফেরত 0;
}

errcode_t ext2fs_make_generic_bitmap(errcode_t magic, ext2_filsys fs,
				     __u32 start, __u32 end, __u32 real_end,
				     ধ্রুবক অক্ষর *descr, অক্ষর *init_map,
				     ext2fs_generic_bitmap *ret)
{
	ext2fs_generic_bitmap_32 bitmap;
	errcode_t		retval;
	size_t			আকার_2;

	retval = ext2fs_get_mem(আকার(কাঠামো ext2fs_struct_generic_bitmap_32),
				&bitmap);
	যদি (retval)
		ফেরত retval;

	bitmap->magic = magic;
	bitmap->fs = fs;
	bitmap->start = start;
	bitmap->end = end;
	bitmap->real_end = real_end;
	নির্বাচন (magic) {
	ক্ষেত্র EXT2_ET_MAGIC_INODE_BITMAP:
		bitmap->base_error_code = EXT2_ET_BAD_INODE_MARK;
		বিরতি;
	ক্ষেত্র EXT2_ET_MAGIC_BLOCK_BITMAP:
		bitmap->base_error_code = EXT2_ET_BAD_BLOCK_MARK;
		বিরতি;
	পূর্বনির্ধারিত:
		bitmap->base_error_code = EXT2_ET_BAD_GENERIC_MARK;
	}
	যদি (descr) {
		retval = ext2fs_get_mem(strlen(descr)+1, &bitmap->description);
		যদি (retval) {
			ext2fs_free_mem(&bitmap);
			ফেরত retval;
		}
		strcpy(bitmap->description, descr);
	} অন্যথা
		bitmap->description = 0;

	আকার_2 = (size_t) (((bitmap->real_end - bitmap->start) / 8) + 1);
	/* Round up to allow for the BT x86 instruction */
	আকার_2 = (আকার_2 + 7) & ~3;
	retval = ext2fs_get_mem(আকার_2, &bitmap->bitmap);
	যদি (retval) {
		ext2fs_free_mem(&bitmap->description);
		ext2fs_free_mem(&bitmap);
		ফেরত retval;
	}

	যদি (init_map)
		memcpy(bitmap->bitmap, init_map, আকার_2);
	অন্যথা
		memset(bitmap->bitmap, 0, আকার_2);
	*ret = (ext2fs_generic_bitmap) bitmap;
	ফেরত 0;
}

errcode_t ext2fs_allocate_generic_bitmap(__u32 start,
					 __u32 end,
					 __u32 real_end,
					 ধ্রুবক অক্ষর *descr,
					 ext2fs_generic_bitmap *ret)
{
	ফেরত ext2fs_make_generic_bitmap(EXT2_ET_MAGIC_GENERIC_BITMAP, 0,
					  start, end, real_end, descr, 0, ret);
}

errcode_t ext2fs_copy_generic_bitmap(ext2fs_generic_bitmap gen_src,
				     ext2fs_generic_bitmap *dest)
{
	ext2fs_generic_bitmap_32 src = (ext2fs_generic_bitmap_32) gen_src;

	ফেরত (ext2fs_make_generic_bitmap(src->magic, src->fs,
					   src->start, src->end,
					   src->real_end,
					   src->description, src->bitmap,
					   dest));
}

শূন্য ext2fs_free_generic_bitmap(ext2fs_inode_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	যদি (check_magic(gen_bitmap))
		ফেরত;

	bitmap->magic = 0;
	যদি (bitmap->description) {
		ext2fs_free_mem(&bitmap->description);
		bitmap->description = 0;
	}
	যদি (bitmap->bitmap) {
		ext2fs_free_mem(&bitmap->bitmap);
		bitmap->bitmap = 0;
	}
	ext2fs_free_mem(&bitmap);
}

পূর্ণসংখ্যা ext2fs_test_generic_bitmap(ext2fs_generic_bitmap bitmap,
					blk_t bitno)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	যদি (!EXT2FS_IS_32_BITMAP(bitmap)) {
		যদি (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			ফেরত ext2fs_test_generic_bmap(bitmap, bitno);
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"test_bitmap(%lu)", (unsigned long) bitno);
#endif
		ফেরত 0;
	}

	যদি ((bitno < bitmap32->start) || (bitno > bitmap32->end)) {
		ext2fs_warn_bitmap2(bitmap, EXT2FS_TEST_ERROR, bitno);
		ফেরত 0;
	}
	ফেরত ext2fs_test_bit(bitno - bitmap32->start, bitmap32->bitmap);
}

পূর্ণসংখ্যা ext2fs_mark_generic_bitmap(ext2fs_generic_bitmap bitmap,
					 __u32 bitno)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	যদি (!EXT2FS_IS_32_BITMAP(bitmap)) {
		যদি (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			ফেরত ext2fs_mark_generic_bmap(bitmap, bitno);
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"mark_bitmap(%lu)", (unsigned long) bitno);
#endif
		ফেরত 0;
	}

	যদি ((bitno < bitmap32->start) || (bitno > bitmap32->end)) {
		ext2fs_warn_bitmap2(bitmap, EXT2FS_MARK_ERROR, bitno);
		ফেরত 0;
	}
	ফেরত ext2fs_set_bit(bitno - bitmap32->start, bitmap32->bitmap);
}

পূর্ণসংখ্যা ext2fs_unmark_generic_bitmap(ext2fs_generic_bitmap bitmap,
					   blk_t bitno)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	যদি (!EXT2FS_IS_32_BITMAP(bitmap)) {
		যদি (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			ফেরত ext2fs_unmark_generic_bmap(bitmap, bitno);
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"mark_bitmap(%lu)", (unsigned long) bitno);
#endif
		ফেরত 0;
	}

	যদি ((bitno < bitmap32->start) || (bitno > bitmap32->end)) {
		ext2fs_warn_bitmap2(bitmap, EXT2FS_UNMARK_ERROR, bitno);
		ফেরত 0;
	}
	ফেরত ext2fs_clear_bit(bitno - bitmap32->start, bitmap32->bitmap);
}

__u32 ext2fs_get_generic_bitmap_start(ext2fs_generic_bitmap bitmap)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	যদি (!EXT2FS_IS_32_BITMAP(bitmap)) {
		যদি (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			ফেরত ext2fs_get_generic_bmap_start(bitmap);
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"get_bitmap_start");
#endif
		ফেরত 0;
	}

	ফেরত bitmap32->start;
}

__u32 ext2fs_get_generic_bitmap_end(ext2fs_generic_bitmap bitmap)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	যদি (!EXT2FS_IS_32_BITMAP(bitmap)) {
		যদি (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			ফেরত ext2fs_get_generic_bmap_end(bitmap);
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"get_bitmap_end");
#endif
		ফেরত 0;
	}
	ফেরত bitmap32->end;
}

শূন্য ext2fs_clear_generic_bitmap(ext2fs_generic_bitmap bitmap)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	যদি (!EXT2FS_IS_32_BITMAP(bitmap)) {
		যদি (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			ext2fs_clear_generic_bmap(bitmap);
			ফেরত;
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"clear_generic_bitmap");
#endif
		ফেরত;
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

	যদি (end > bitmap->real_end)
		ফেরত neq;
	যদি (oend)
		*oend = bitmap->end;
	bitmap->end = end;
	ফেরত 0;
}

errcode_t ext2fs_resize_generic_bitmap(errcode_t magic,
				       __u32 new_end, __u32 new_real_end,
				       ext2fs_generic_bitmap gen_bmap)
{
	ext2fs_generic_bitmap_32 bmap = (ext2fs_generic_bitmap_32) gen_bmap;
	errcode_t	retval;
	size_t		আকার_2, new_আকার;
	__u32		bitno;

	যদি (!bmap || (bmap->magic != magic))
		ফেরত magic;

	/*
	 * If we're expanding the bitmap, make sure all of the new
	 * parts of the bitmap are zero.
	 */
	যদি (new_end > bmap->end) {
		bitno = bmap->real_end;
		যদি (bitno > new_end)
			bitno = new_end;
		জন্য (; bitno > bmap->end; bitno--)
			ext2fs_clear_bit(bitno - bmap->start, bmap->bitmap);
	}
	যদি (new_real_end == bmap->real_end) {
		bmap->end = new_end;
		ফেরত 0;
	}

	আকার_2 = ((bmap->real_end - bmap->start) / 8) + 1;
	new_আকার = ((new_real_end - bmap->start) / 8) + 1;

	যদি (আকার_2 != new_আকার) {
		retval = ext2fs_resize_mem(আকার_2, new_আকার, &bmap->bitmap);
		যদি (retval)
			ফেরত retval;
	}
	যদি (new_আকার > আকার_2)
		memset(bmap->bitmap + আকার_2, 0, new_আকার - আকার_2);

	bmap->end = new_end;
	bmap->real_end = new_real_end;
	ফেরত 0;
}

errcode_t ext2fs_compare_generic_bitmap(errcode_t magic, errcode_t neq,
					ext2fs_generic_bitmap gen_bm1,
					ext2fs_generic_bitmap gen_bm2)
{
	ext2fs_generic_bitmap_32 bm1 = (ext2fs_generic_bitmap_32) gen_bm1;
	ext2fs_generic_bitmap_32 bm2 = (ext2fs_generic_bitmap_32) gen_bm2;
	blk_t	i;

	যদি (!bm1 || bm1->magic != magic)
		ফেরত magic;
	যদি (!bm2 || bm2->magic != magic)
		ফেরত magic;

	যদি ((bm1->start != bm2->start) ||
	    (bm1->end != bm2->end) ||
	    (memcmp(bm1->bitmap, bm2->bitmap,
		    (size_t) (bm1->end - bm1->start)/8)))
		ফেরত neq;

	জন্য (i = bm1->end - ((bm1->end - bm1->start) % 8); i <= bm1->end; i++)
		যদি (ext2fs_fast_test_block_bitmap(gen_bm1, i) !=
		    ext2fs_fast_test_block_bitmap(gen_bm2, i))
			ফেরত neq;

	ফেরত 0;
}

শূন্য ext2fs_set_generic_bitmap_padding(ext2fs_generic_bitmap gen_map)
{
	ext2fs_generic_bitmap_32 map = (ext2fs_generic_bitmap_32) gen_map;
	__u32	i, j;

	/* Protect loop from wrap-around if map->real_end is maxed */
	জন্য (i=map->end+1, j = i - map->start;
	     i <= map->real_end && i > map->end;
	     i++, j++)
		ext2fs_set_bit(j, map->bitmap);
}

errcode_t ext2fs_get_generic_bitmap_range(ext2fs_generic_bitmap gen_bmap,
					  errcode_t magic,
					  __u32 start, __u32 অঙ্ক,
					  শূন্য *out)
{
	ext2fs_generic_bitmap_32 bmap = (ext2fs_generic_bitmap_32) gen_bmap;

	যদি (!bmap || (bmap->magic != magic))
		ফেরত magic;

	যদি ((start < bmap->start) || (start+অঙ্ক-1 > bmap->real_end))
		ফেরত EXT2_ET_INVALID_ARGUMENT;

	memcpy(out, bmap->bitmap + ((start - bmap->start) >> 3), (অঙ্ক+7) >> 3);
	ফেরত 0;
}

errcode_t ext2fs_set_generic_bitmap_range(ext2fs_generic_bitmap gen_bmap,
					  errcode_t magic,
					  __u32 start, __u32 অঙ্ক,
					  শূন্য *in)
{
	ext2fs_generic_bitmap_32 bmap = (ext2fs_generic_bitmap_32) gen_bmap;

	যদি (!bmap || (bmap->magic != magic))
		ফেরত magic;

	যদি ((start < bmap->start) || (start+অঙ্ক-1 > bmap->real_end))
		ফেরত EXT2_ET_INVALID_ARGUMENT;

	memcpy(bmap->bitmap + ((start - bmap->start) >> 3), in, (অঙ্ক+7) >> 3);
	ফেরত 0;
}

/*
 * Compare @mem to zero buffer by 256 bytes.
 * Return 1 if @mem is zeroed memory, otherwise return 0.
 */
পূর্ণসংখ্যা ext2fs_mem_is_zero(ধ্রুবক অক্ষর *mem, size_t len)
{
	স্থির ধ্রুবক অক্ষর zero_buf[256];

	যখন (len >= আকার(zero_buf)) {
		যদি (memcmp(mem, zero_buf, আকার(zero_buf)))
			ফেরত 0;
		len -= আকার(zero_buf);
		mem += আকার(zero_buf);
	}
	/* Deal with leftover bytes. */
	যদি (len)
		ফেরত !memcmp(mem, zero_buf, len);
	ফেরত 1;
}

/*
 * Return true if all of the bits in a specified range are clear
 */
স্থির পূর্ণসংখ্যা ext2fs_test_clear_generic_bitmap_range(ext2fs_generic_bitmap gen_bitmap,
						  unsigned পূর্ণসংখ্যা start,
						  unsigned পূর্ণসংখ্যা len)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	size_t start_byte, len_byte = len >> 3;
	unsigned পূর্ণসংখ্যা start_bit, len_bit = len % 8;
	পূর্ণসংখ্যা first_bit = 0;
	পূর্ণসংখ্যা last_bit  = 0;
	পূর্ণসংখ্যা mark_গুনতি = 0;
	পূর্ণসংখ্যা mark_bit = 0;
	পূর্ণসংখ্যা i;
	ধ্রুবক অক্ষর *ADDR = bitmap->bitmap;

	start -= bitmap->start;
	start_byte = start >> 3;
	start_bit = start % 8;

	যদি (start_bit != 0) {
		/*
		 * The compared start block number or start inode number
		 * is not the first bit in a byte.
		 */
		mark_গুনতি = 8 - start_bit;
		যদি (len < 8 - start_bit) {
			mark_গুনতি = (পূর্ণসংখ্যা)len;
			mark_bit = len + start_bit - 1;
		} অন্যথা
			mark_bit = 7;

		জন্য (i = mark_গুনতি; i > 0; i--, mark_bit--)
			first_bit |= 1 << mark_bit;

		/*
		 * Compare blocks or inodes in the first byte.
		 * If there is any marked bit, this function returns 0.
		 */
		যদি (first_bit & ADDR[start_byte])
			ফেরত 0;
		অন্যথা যদি (len <= 8 - start_bit)
			ফেরত 1;

		start_byte++;
		len_bit = (len - mark_গুনতি) % 8;
		len_byte = (len - mark_গুনতি) >> 3;
	}

	/*
	 * The compared start block number or start inode number is
	 * the first bit in a byte.
	 */
	যদি (len_bit != 0) {
		/*
		 * The compared end block number or end inode number is
		 * not the last bit in a byte.
		 */
		জন্য (mark_bit = len_bit - 1; mark_bit >= 0; mark_bit--)
			last_bit |= 1 << mark_bit;

		/*
		 * Compare blocks or inodes in the last byte.
		 * If there is any marked bit, this function returns 0.
		 */
		যদি (last_bit & ADDR[start_byte + len_byte])
			ফেরত 0;
		অন্যথা যদি (len_byte == 0)
			ফেরত 1;
	}

	/* Check whether all bytes are 0 */
	ফেরত ext2fs_mem_is_zero(ADDR + start_byte, len_byte);
}

errcode_t ext2fs_find_first_zero_generic_bitmap(ext2fs_generic_bitmap gen_bitmap,
						__u32 start, __u32 end,
						__u32 *out)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	blk_t b;

	যদি (start < bitmap->start || end > bitmap->end || start > end) {
		ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_TEST_ERROR, start);
		ফেরত EINVAL;
	}

	যখন (start <= end) {
		b = ext2fs_test_bit(start - bitmap->start, bitmap->bitmap);
		যদি (!b) {
			*out = start;
			ফেরত 0;
		}
		start++;
	}

	ফেরত ENOENT;
}

errcode_t ext2fs_find_first_set_generic_bitmap(ext2fs_generic_bitmap gen_bitmap,
					       __u32 start, __u32 end,
					       __u32 *out)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	blk_t b;

	যদি (start < bitmap->start || end > bitmap->end || start > end) {
		ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_TEST_ERROR, start);
		ফেরত EINVAL;
	}

	যখন (start <= end) {
		b = ext2fs_test_bit(start - bitmap->start, bitmap->bitmap);
		যদি (b) {
			*out = start;
			ফেরত 0;
		}
		start++;
	}

	ফেরত ENOENT;
}

পূর্ণসংখ্যা ext2fs_test_block_bitmap_range(ext2fs_block_bitmap gen_bitmap,
				   blk_t block, পূর্ণসংখ্যা অঙ্ক)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	EXT2_CHECK_MAGIC(bitmap, EXT2_ET_MAGIC_BLOCK_BITMAP);
	যদি ((block < bitmap->start) || (block > bitmap->real_end) ||
	    (block+অঙ্ক-1 > bitmap->real_end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_TEST,
				   block, bitmap->description);
		ফেরত 0;
	}
	ফেরত ext2fs_test_clear_generic_bitmap_range((ext2fs_generic_bitmap)
						      bitmap, block, অঙ্ক);
}

পূর্ণসংখ্যা ext2fs_test_inode_bitmap_range(ext2fs_inode_bitmap gen_bitmap,
				   ext2_ino_t inode, পূর্ণসংখ্যা অঙ্ক)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	EXT2_CHECK_MAGIC(bitmap, EXT2_ET_MAGIC_INODE_BITMAP);
	যদি ((inode < bitmap->start) || (inode > bitmap->real_end) ||
	    (inode+অঙ্ক-1 > bitmap->real_end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_INODE_TEST,
				   inode, bitmap->description);
		ফেরত 0;
	}
	ফেরত ext2fs_test_clear_generic_bitmap_range((ext2fs_generic_bitmap)
						      bitmap, inode, অঙ্ক);
}

শূন্য ext2fs_mark_block_bitmap_range(ext2fs_block_bitmap gen_bitmap,
				    blk_t block, পূর্ণসংখ্যা অঙ্ক)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	পূর্ণসংখ্যা	i;

	যদি ((block < bitmap->start) || (block > bitmap->end) ||
	    (block+অঙ্ক-1 > bitmap->end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_MARK, block,
				   bitmap->description);
		ফেরত;
	}
	জন্য (i=0; i < অঙ্ক; i++)
		ext2fs_fast_set_bit(block + i - bitmap->start, bitmap->bitmap);
}

শূন্য ext2fs_unmark_block_bitmap_range(ext2fs_block_bitmap gen_bitmap,
				      blk_t block, পূর্ণসংখ্যা অঙ্ক)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	পূর্ণসংখ্যা	i;

	যদি ((block < bitmap->start) || (block > bitmap->end) ||
	    (block+অঙ্ক-1 > bitmap->end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_UNMARK, block,
				   bitmap->description);
		ফেরত;
	}
	জন্য (i=0; i < অঙ্ক; i++)
		ext2fs_fast_clear_bit(block + i - bitmap->start,
				      bitmap->bitmap);
}

