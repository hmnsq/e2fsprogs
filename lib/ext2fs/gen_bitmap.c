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


#অন্তর্ভাৱ "config.h"
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <string.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাৱ <unistd.h>
#endif
#অন্তর্ভাৱ <fcntl.h>
#অন্তর্ভাৱ <time.h>
#যদি HAVE_SYS_STAT_H
#অন্তর্ভাৱ <sys/stat.h>
#endif
#যদি HAVE_SYS_TYPES_H
#অন্তর্ভাৱ <sys/types.h>
#endif

#অন্তর্ভাৱ "ext2_fs.h"
#অন্তর্ভাৱ "ext2fsP.h"

সংরচনা ext2fs_struct_generic_bitmap_32 {
	errcode_t	magic;
	ext2_filsys 	fs;
	__u32		start, end;
	__u32		real_end;
	ৱর্ণ	*	description;
	ৱর্ণ	*	bitmap;
	errcode_t	base_error_code;
	__u32		reserved[7];
};

প্রকারনির্দেশ সংরচনা ext2fs_struct_generic_bitmap_32 *ext2fs_generic_bitmap_32;

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
			    পূর্ণাঙ্ক code, unsigned long arg)
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
		ঘূৰাই EXT2_ET_MAGIC_GENERIC_BITMAP;
	ঘূৰাই 0;
}

errcode_t ext2fs_make_generic_bitmap(errcode_t magic, ext2_filsys fs,
				     __u32 start, __u32 end, __u32 real_end,
				     ধ্রুৱ ৱর্ণ *descr, ৱর্ণ *init_map,
				     ext2fs_generic_bitmap *ret)
{
	ext2fs_generic_bitmap_32 bitmap;
	errcode_t		retval;
	size_t			size;

	retval = ext2fs_get_mem(পরিমাণ(সংরচনা ext2fs_struct_generic_bitmap_32),
				&bitmap);
	যদি (retval)
		ঘূৰাই retval;

	bitmap->magic = magic;
	bitmap->fs = fs;
	bitmap->start = start;
	bitmap->end = end;
	bitmap->real_end = real_end;
	ৱিকল্প (magic) {
	পক্ষ EXT2_ET_MAGIC_INODE_BITMAP:
		bitmap->base_error_code = EXT2_ET_BAD_INODE_MARK;
		ভঙ্গ;
	পক্ষ EXT2_ET_MAGIC_BLOCK_BITMAP:
		bitmap->base_error_code = EXT2_ET_BAD_BLOCK_MARK;
		ভঙ্গ;
	স্ৱভাৱ:
		bitmap->base_error_code = EXT2_ET_BAD_GENERIC_MARK;
	}
	যদি (descr) {
		retval = ext2fs_get_mem(strlen(descr)+1, &bitmap->description);
		যদি (retval) {
			ext2fs_free_mem(&bitmap);
			ঘূৰাই retval;
		}
		strcpy(bitmap->description, descr);
	} অন্যথা
		bitmap->description = 0;

	size = (size_t) (((bitmap->real_end - bitmap->start) / 8) + 1);
	/* Round up to allow for the BT x86 instruction */
	size = (size + 7) & ~3;
	retval = ext2fs_get_mem(size, &bitmap->bitmap);
	যদি (retval) {
		ext2fs_free_mem(&bitmap->description);
		ext2fs_free_mem(&bitmap);
		ঘূৰাই retval;
	}

	যদি (init_map)
		memcpy(bitmap->bitmap, init_map, size);
	অন্যথা
		memset(bitmap->bitmap, 0, size);
	*ret = (ext2fs_generic_bitmap) bitmap;
	ঘূৰাই 0;
}

errcode_t ext2fs_allocate_generic_bitmap(__u32 start,
					 __u32 end,
					 __u32 real_end,
					 ধ্রুৱ ৱর্ণ *descr,
					 ext2fs_generic_bitmap *ret)
{
	ঘূৰাই ext2fs_make_generic_bitmap(EXT2_ET_MAGIC_GENERIC_BITMAP, 0,
					  start, end, real_end, descr, 0, ret);
}

errcode_t ext2fs_copy_generic_bitmap(ext2fs_generic_bitmap gen_src,
				     ext2fs_generic_bitmap *dest)
{
	ext2fs_generic_bitmap_32 src = (ext2fs_generic_bitmap_32) gen_src;

	ঘূৰাই (ext2fs_make_generic_bitmap(src->magic, src->fs,
					   src->start, src->end,
					   src->real_end,
					   src->description, src->bitmap,
					   dest));
}

শূন্য ext2fs_free_generic_bitmap(ext2fs_inode_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	যদি (check_magic(gen_bitmap))
		ঘূৰাই;

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

পূর্ণাঙ্ক ext2fs_test_generic_bitmap(ext2fs_generic_bitmap bitmap,
					blk_t bitno)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	যদি (!EXT2FS_IS_32_BITMAP(bitmap)) {
		যদি (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			ঘূৰাই ext2fs_test_generic_bmap(bitmap, bitno);
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"test_bitmap(%lu)", (unsigned long) bitno);
#endif
		ঘূৰাই 0;
	}

	যদি ((bitno < bitmap32->start) || (bitno > bitmap32->end)) {
		ext2fs_warn_bitmap2(bitmap, EXT2FS_TEST_ERROR, bitno);
		ঘূৰাই 0;
	}
	ঘূৰাই ext2fs_test_bit(bitno - bitmap32->start, bitmap32->bitmap);
}

পূর্ণাঙ্ক ext2fs_mark_generic_bitmap(ext2fs_generic_bitmap bitmap,
					 __u32 bitno)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	যদি (!EXT2FS_IS_32_BITMAP(bitmap)) {
		যদি (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			ঘূৰাই ext2fs_mark_generic_bmap(bitmap, bitno);
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"mark_bitmap(%lu)", (unsigned long) bitno);
#endif
		ঘূৰাই 0;
	}

	যদি ((bitno < bitmap32->start) || (bitno > bitmap32->end)) {
		ext2fs_warn_bitmap2(bitmap, EXT2FS_MARK_ERROR, bitno);
		ঘূৰাই 0;
	}
	ঘূৰাই ext2fs_set_bit(bitno - bitmap32->start, bitmap32->bitmap);
}

পূর্ণাঙ্ক ext2fs_unmark_generic_bitmap(ext2fs_generic_bitmap bitmap,
					   blk_t bitno)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	যদি (!EXT2FS_IS_32_BITMAP(bitmap)) {
		যদি (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			ঘূৰাই ext2fs_unmark_generic_bmap(bitmap, bitno);
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"mark_bitmap(%lu)", (unsigned long) bitno);
#endif
		ঘূৰাই 0;
	}

	যদি ((bitno < bitmap32->start) || (bitno > bitmap32->end)) {
		ext2fs_warn_bitmap2(bitmap, EXT2FS_UNMARK_ERROR, bitno);
		ঘূৰাই 0;
	}
	ঘূৰাই ext2fs_clear_bit(bitno - bitmap32->start, bitmap32->bitmap);
}

__u32 ext2fs_get_generic_bitmap_start(ext2fs_generic_bitmap bitmap)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	যদি (!EXT2FS_IS_32_BITMAP(bitmap)) {
		যদি (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			ঘূৰাই ext2fs_get_generic_bmap_start(bitmap);
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"get_bitmap_start");
#endif
		ঘূৰাই 0;
	}

	ঘূৰাই bitmap32->start;
}

__u32 ext2fs_get_generic_bitmap_end(ext2fs_generic_bitmap bitmap)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	যদি (!EXT2FS_IS_32_BITMAP(bitmap)) {
		যদি (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			ঘূৰাই ext2fs_get_generic_bmap_end(bitmap);
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"get_bitmap_end");
#endif
		ঘূৰাই 0;
	}
	ঘূৰাই bitmap32->end;
}

শূন্য ext2fs_clear_generic_bitmap(ext2fs_generic_bitmap bitmap)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	যদি (!EXT2FS_IS_32_BITMAP(bitmap)) {
		যদি (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			ext2fs_clear_generic_bmap(bitmap);
			ঘূৰাই;
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"clear_generic_bitmap");
#endif
		ঘূৰাই;
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
		ঘূৰাই neq;
	যদি (oend)
		*oend = bitmap->end;
	bitmap->end = end;
	ঘূৰাই 0;
}

errcode_t ext2fs_resize_generic_bitmap(errcode_t magic,
				       __u32 new_end, __u32 new_real_end,
				       ext2fs_generic_bitmap gen_bmap)
{
	ext2fs_generic_bitmap_32 bmap = (ext2fs_generic_bitmap_32) gen_bmap;
	errcode_t	retval;
	size_t		size, new_size;
	__u32		bitno;

	যদি (!bmap || (bmap->magic != magic))
		ঘূৰাই magic;

	/*
	 * If we're expanding the bitmap, make sure all of the new
	 * parts of the bitmap are zero.
	 */
	যদি (new_end > bmap->end) {
		bitno = bmap->real_end;
		যদি (bitno > new_end)
			bitno = new_end;
		বাবে (; bitno > bmap->end; bitno--)
			ext2fs_clear_bit(bitno - bmap->start, bmap->bitmap);
	}
	যদি (new_real_end == bmap->real_end) {
		bmap->end = new_end;
		ঘূৰাই 0;
	}

	size = ((bmap->real_end - bmap->start) / 8) + 1;
	new_size = ((new_real_end - bmap->start) / 8) + 1;

	যদি (size != new_size) {
		retval = ext2fs_resize_mem(size, new_size, &bmap->bitmap);
		যদি (retval)
			ঘূৰাই retval;
	}
	যদি (new_size > size)
		memset(bmap->bitmap + size, 0, new_size - size);

	bmap->end = new_end;
	bmap->real_end = new_real_end;
	ঘূৰাই 0;
}

errcode_t ext2fs_compare_generic_bitmap(errcode_t magic, errcode_t neq,
					ext2fs_generic_bitmap gen_bm1,
					ext2fs_generic_bitmap gen_bm2)
{
	ext2fs_generic_bitmap_32 bm1 = (ext2fs_generic_bitmap_32) gen_bm1;
	ext2fs_generic_bitmap_32 bm2 = (ext2fs_generic_bitmap_32) gen_bm2;
	blk_t	i;

	যদি (!bm1 || bm1->magic != magic)
		ঘূৰাই magic;
	যদি (!bm2 || bm2->magic != magic)
		ঘূৰাই magic;

	যদি ((bm1->start != bm2->start) ||
	    (bm1->end != bm2->end) ||
	    (memcmp(bm1->bitmap, bm2->bitmap,
		    (size_t) (bm1->end - bm1->start)/8)))
		ঘূৰাই neq;

	বাবে (i = bm1->end - ((bm1->end - bm1->start) % 8); i <= bm1->end; i++)
		যদি (ext2fs_fast_test_block_bitmap(gen_bm1, i) !=
		    ext2fs_fast_test_block_bitmap(gen_bm2, i))
			ঘূৰাই neq;

	ঘূৰাই 0;
}

শূন্য ext2fs_set_generic_bitmap_padding(ext2fs_generic_bitmap gen_map)
{
	ext2fs_generic_bitmap_32 map = (ext2fs_generic_bitmap_32) gen_map;
	__u32	i, j;

	/* Protect loop from wrap-around if map->real_end is maxed */
	বাবে (i=map->end+1, j = i - map->start;
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
		ঘূৰাই magic;

	যদি ((start < bmap->start) || (start+অঙ্ক-1 > bmap->real_end))
		ঘূৰাই EXT2_ET_INVALID_ARGUMENT;

	memcpy(out, bmap->bitmap + ((start - bmap->start) >> 3), (অঙ্ক+7) >> 3);
	ঘূৰাই 0;
}

errcode_t ext2fs_set_generic_bitmap_range(ext2fs_generic_bitmap gen_bmap,
					  errcode_t magic,
					  __u32 start, __u32 অঙ্ক,
					  শূন্য *in)
{
	ext2fs_generic_bitmap_32 bmap = (ext2fs_generic_bitmap_32) gen_bmap;

	যদি (!bmap || (bmap->magic != magic))
		ঘূৰাই magic;

	যদি ((start < bmap->start) || (start+অঙ্ক-1 > bmap->real_end))
		ঘূৰাই EXT2_ET_INVALID_ARGUMENT;

	memcpy(bmap->bitmap + ((start - bmap->start) >> 3), in, (অঙ্ক+7) >> 3);
	ঘূৰাই 0;
}

/*
 * Compare @mem to zero buffer by 256 bytes.
 * Return 1 if @mem is zeroed memory, otherwise return 0.
 */
পূর্ণাঙ্ক ext2fs_mem_is_zero(ধ্রুৱ ৱর্ণ *mem, size_t len)
{
	স্থির ধ্রুৱ ৱর্ণ zero_buf[256];

	যেতিয়ালৈ (len >= পরিমাণ(zero_buf)) {
		যদি (memcmp(mem, zero_buf, পরিমাণ(zero_buf)))
			ঘূৰাই 0;
		len -= পরিমাণ(zero_buf);
		mem += পরিমাণ(zero_buf);
	}
	/* Deal with leftover bytes. */
	যদি (len)
		ঘূৰাই !memcmp(mem, zero_buf, len);
	ঘূৰাই 1;
}

/*
 * Return true if all of the bits in a specified range are clear
 */
স্থির পূর্ণাঙ্ক ext2fs_test_clear_generic_bitmap_range(ext2fs_generic_bitmap gen_bitmap,
						  unsigned পূর্ণাঙ্ক start,
						  unsigned পূর্ণাঙ্ক len)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	size_t start_byte, len_byte = len >> 3;
	unsigned পূর্ণাঙ্ক start_bit, len_bit = len % 8;
	পূর্ণাঙ্ক first_bit = 0;
	পূর্ণাঙ্ক last_bit  = 0;
	পূর্ণাঙ্ক mark_গুনতি = 0;
	পূর্ণাঙ্ক mark_bit = 0;
	পূর্ণাঙ্ক i;
	ধ্রুৱ ৱর্ণ *ADDR = bitmap->bitmap;

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
			mark_গুনতি = (পূর্ণাঙ্ক)len;
			mark_bit = len + start_bit - 1;
		} অন্যথা
			mark_bit = 7;

		বাবে (i = mark_গুনতি; i > 0; i--, mark_bit--)
			first_bit |= 1 << mark_bit;

		/*
		 * Compare blocks or inodes in the first byte.
		 * If there is any marked bit, this function returns 0.
		 */
		যদি (first_bit & ADDR[start_byte])
			ঘূৰাই 0;
		অন্যথা যদি (len <= 8 - start_bit)
			ঘূৰাই 1;

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
		বাবে (mark_bit = len_bit - 1; mark_bit >= 0; mark_bit--)
			last_bit |= 1 << mark_bit;

		/*
		 * Compare blocks or inodes in the last byte.
		 * If there is any marked bit, this function returns 0.
		 */
		যদি (last_bit & ADDR[start_byte + len_byte])
			ঘূৰাই 0;
		অন্যথা যদি (len_byte == 0)
			ঘূৰাই 1;
	}

	/* Check whether all bytes are 0 */
	ঘূৰাই ext2fs_mem_is_zero(ADDR + start_byte, len_byte);
}

errcode_t ext2fs_find_first_zero_generic_bitmap(ext2fs_generic_bitmap gen_bitmap,
						__u32 start, __u32 end,
						__u32 *out)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	blk_t b;

	যদি (start < bitmap->start || end > bitmap->end || start > end) {
		ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_TEST_ERROR, start);
		ঘূৰাই EINVAL;
	}

	যেতিয়ালৈ (start <= end) {
		b = ext2fs_test_bit(start - bitmap->start, bitmap->bitmap);
		যদি (!b) {
			*out = start;
			ঘূৰাই 0;
		}
		start++;
	}

	ঘূৰাই ENOENT;
}

errcode_t ext2fs_find_first_set_generic_bitmap(ext2fs_generic_bitmap gen_bitmap,
					       __u32 start, __u32 end,
					       __u32 *out)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	blk_t b;

	যদি (start < bitmap->start || end > bitmap->end || start > end) {
		ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_TEST_ERROR, start);
		ঘূৰাই EINVAL;
	}

	যেতিয়ালৈ (start <= end) {
		b = ext2fs_test_bit(start - bitmap->start, bitmap->bitmap);
		যদি (b) {
			*out = start;
			ঘূৰাই 0;
		}
		start++;
	}

	ঘূৰাই ENOENT;
}

পূর্ণাঙ্ক ext2fs_test_block_bitmap_range(ext2fs_block_bitmap gen_bitmap,
				   blk_t block, পূর্ণাঙ্ক অঙ্ক)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	EXT2_CHECK_MAGIC(bitmap, EXT2_ET_MAGIC_BLOCK_BITMAP);
	যদি ((block < bitmap->start) || (block > bitmap->real_end) ||
	    (block+অঙ্ক-1 > bitmap->real_end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_TEST,
				   block, bitmap->description);
		ঘূৰাই 0;
	}
	ঘূৰাই ext2fs_test_clear_generic_bitmap_range((ext2fs_generic_bitmap)
						      bitmap, block, অঙ্ক);
}

পূর্ণাঙ্ক ext2fs_test_inode_bitmap_range(ext2fs_inode_bitmap gen_bitmap,
				   ext2_ino_t inode, পূর্ণাঙ্ক অঙ্ক)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	EXT2_CHECK_MAGIC(bitmap, EXT2_ET_MAGIC_INODE_BITMAP);
	যদি ((inode < bitmap->start) || (inode > bitmap->real_end) ||
	    (inode+অঙ্ক-1 > bitmap->real_end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_INODE_TEST,
				   inode, bitmap->description);
		ঘূৰাই 0;
	}
	ঘূৰাই ext2fs_test_clear_generic_bitmap_range((ext2fs_generic_bitmap)
						      bitmap, inode, অঙ্ক);
}

শূন্য ext2fs_mark_block_bitmap_range(ext2fs_block_bitmap gen_bitmap,
				    blk_t block, পূর্ণাঙ্ক অঙ্ক)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	পূর্ণাঙ্ক	i;

	যদি ((block < bitmap->start) || (block > bitmap->end) ||
	    (block+অঙ্ক-1 > bitmap->end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_MARK, block,
				   bitmap->description);
		ঘূৰাই;
	}
	বাবে (i=0; i < অঙ্ক; i++)
		ext2fs_fast_set_bit(block + i - bitmap->start, bitmap->bitmap);
}

শূন্য ext2fs_unmark_block_bitmap_range(ext2fs_block_bitmap gen_bitmap,
				      blk_t block, পূর্ণাঙ্ক অঙ্ক)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	পূর্ণাঙ্ক	i;

	যদি ((block < bitmap->start) || (block > bitmap->end) ||
	    (block+অঙ্ক-1 > bitmap->end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_UNMARK, block,
				   bitmap->description);
		ঘূৰাই;
	}
	বাবে (i=0; i < অঙ্ক; i++)
		ext2fs_fast_clear_bit(block + i - bitmap->start,
				      bitmap->bitmap);
}

