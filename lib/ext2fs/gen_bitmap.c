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


#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଯଦି HAVE_UNISTD_H
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#endif
#ଅନ୍ତର୍ଭାଵ <fcntl.h>
#ଅନ୍ତର୍ଭାଵ <time.h>
#ଯଦି HAVE_SYS_STAT_H
#ଅନ୍ତର୍ଭାଵ <sys/stat.h>
#endif
#ଯଦି HAVE_SYS_TYPES_H
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#endif

#ଅନ୍ତର୍ଭାଵ "ext2_fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fsP.h"

ସଂରଚନା ext2fs_struct_generic_bitmap_32 {
	errcode_t	magic;
	ext2_filsys 	fs;
	__u32		start, end;
	__u32		real_end;
	ଅକ୍ଷର	*	description;
	ଅକ୍ଷର	*	bitmap;
	errcode_t	base_error_code;
	__u32		reserved[7];
};

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା ext2fs_struct_generic_bitmap_32 *ext2fs_generic_bitmap_32;

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
ଶୂନ୍ୟ ext2fs_warn_bitmap2(ext2fs_generic_bitmap gen_bitmap,
			    ପୂର୍ଣ୍ଣସଂଖ୍ୟା code, unsigned long arg)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

#ifndef OMIT_COM_ERR
	ଯଦି (bitmap->description)
		com_err(0, bitmap->base_error_code+code,
			"#%lu for %s", arg, bitmap->description);
	ଅନ୍ୟଥା
		com_err(0, bitmap->base_error_code + code, "#%lu", arg);
#endif
}

ସ୍ଥିର errcode_t check_magic(ext2fs_generic_bitmap bitmap)
{
	ଯଦି (!bitmap || !((bitmap->magic == EXT2_ET_MAGIC_GENERIC_BITMAP) ||
			 (bitmap->magic == EXT2_ET_MAGIC_INODE_BITMAP) ||
			 (bitmap->magic == EXT2_ET_MAGIC_BLOCK_BITMAP)))
		ଫେରନ୍ତୁ EXT2_ET_MAGIC_GENERIC_BITMAP;
	ଫେରନ୍ତୁ 0;
}

errcode_t ext2fs_make_generic_bitmap(errcode_t magic, ext2_filsys fs,
				     __u32 start, __u32 end, __u32 real_end,
				     ଅଚଳ ଅକ୍ଷର *descr, ଅକ୍ଷର *init_map,
				     ext2fs_generic_bitmap *ret)
{
	ext2fs_generic_bitmap_32 bitmap;
	errcode_t		retval;
	size_t			size;

	retval = ext2fs_get_mem(ଆକାର(ସଂରଚନା ext2fs_struct_generic_bitmap_32),
				&bitmap);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;

	bitmap->magic = magic;
	bitmap->fs = fs;
	bitmap->start = start;
	bitmap->end = end;
	bitmap->real_end = real_end;
	ବାଛ (magic) {
	ସ୍ଥିତି EXT2_ET_MAGIC_INODE_BITMAP:
		bitmap->base_error_code = EXT2_ET_BAD_INODE_MARK;
		ଭାଙ୍ଗ;
	ସ୍ଥିତି EXT2_ET_MAGIC_BLOCK_BITMAP:
		bitmap->base_error_code = EXT2_ET_BAD_BLOCK_MARK;
		ଭାଙ୍ଗ;
	ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
		bitmap->base_error_code = EXT2_ET_BAD_GENERIC_MARK;
	}
	ଯଦି (descr) {
		retval = ext2fs_get_mem(strlen(descr)+1, &bitmap->description);
		ଯଦି (retval) {
			ext2fs_free_mem(&bitmap);
			ଫେରନ୍ତୁ retval;
		}
		strcpy(bitmap->description, descr);
	} ଅନ୍ୟଥା
		bitmap->description = 0;

	size = (size_t) (((bitmap->real_end - bitmap->start) / 8) + 1);
	/* Round up to allow for the BT x86 instruction */
	size = (size + 7) & ~3;
	retval = ext2fs_get_mem(size, &bitmap->bitmap);
	ଯଦି (retval) {
		ext2fs_free_mem(&bitmap->description);
		ext2fs_free_mem(&bitmap);
		ଫେରନ୍ତୁ retval;
	}

	ଯଦି (init_map)
		memcpy(bitmap->bitmap, init_map, size);
	ଅନ୍ୟଥା
		memset(bitmap->bitmap, 0, size);
	*ret = (ext2fs_generic_bitmap) bitmap;
	ଫେରନ୍ତୁ 0;
}

errcode_t ext2fs_allocate_generic_bitmap(__u32 start,
					 __u32 end,
					 __u32 real_end,
					 ଅଚଳ ଅକ୍ଷର *descr,
					 ext2fs_generic_bitmap *ret)
{
	ଫେରନ୍ତୁ ext2fs_make_generic_bitmap(EXT2_ET_MAGIC_GENERIC_BITMAP, 0,
					  start, end, real_end, descr, 0, ret);
}

errcode_t ext2fs_copy_generic_bitmap(ext2fs_generic_bitmap gen_src,
				     ext2fs_generic_bitmap *dest)
{
	ext2fs_generic_bitmap_32 src = (ext2fs_generic_bitmap_32) gen_src;

	ଫେରନ୍ତୁ (ext2fs_make_generic_bitmap(src->magic, src->fs,
					   src->start, src->end,
					   src->real_end,
					   src->description, src->bitmap,
					   dest));
}

ଶୂନ୍ୟ ext2fs_free_generic_bitmap(ext2fs_inode_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	ଯଦି (check_magic(gen_bitmap))
		ଫେରନ୍ତୁ;

	bitmap->magic = 0;
	ଯଦି (bitmap->description) {
		ext2fs_free_mem(&bitmap->description);
		bitmap->description = 0;
	}
	ଯଦି (bitmap->bitmap) {
		ext2fs_free_mem(&bitmap->bitmap);
		bitmap->bitmap = 0;
	}
	ext2fs_free_mem(&bitmap);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_test_generic_bitmap(ext2fs_generic_bitmap bitmap,
					blk_t bitno)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	ଯଦି (!EXT2FS_IS_32_BITMAP(bitmap)) {
		ଯଦି (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			ଫେରନ୍ତୁ ext2fs_test_generic_bmap(bitmap, bitno);
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"test_bitmap(%lu)", (unsigned long) bitno);
#endif
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି ((bitno < bitmap32->start) || (bitno > bitmap32->end)) {
		ext2fs_warn_bitmap2(bitmap, EXT2FS_TEST_ERROR, bitno);
		ଫେରନ୍ତୁ 0;
	}
	ଫେରନ୍ତୁ ext2fs_test_bit(bitno - bitmap32->start, bitmap32->bitmap);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_mark_generic_bitmap(ext2fs_generic_bitmap bitmap,
					 __u32 bitno)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	ଯଦି (!EXT2FS_IS_32_BITMAP(bitmap)) {
		ଯଦି (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			ଫେରନ୍ତୁ ext2fs_mark_generic_bmap(bitmap, bitno);
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"mark_bitmap(%lu)", (unsigned long) bitno);
#endif
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି ((bitno < bitmap32->start) || (bitno > bitmap32->end)) {
		ext2fs_warn_bitmap2(bitmap, EXT2FS_MARK_ERROR, bitno);
		ଫେରନ୍ତୁ 0;
	}
	ଫେରନ୍ତୁ ext2fs_set_bit(bitno - bitmap32->start, bitmap32->bitmap);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_unmark_generic_bitmap(ext2fs_generic_bitmap bitmap,
					   blk_t bitno)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	ଯଦି (!EXT2FS_IS_32_BITMAP(bitmap)) {
		ଯଦି (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			ଫେରନ୍ତୁ ext2fs_unmark_generic_bmap(bitmap, bitno);
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"mark_bitmap(%lu)", (unsigned long) bitno);
#endif
		ଫେରନ୍ତୁ 0;
	}

	ଯଦି ((bitno < bitmap32->start) || (bitno > bitmap32->end)) {
		ext2fs_warn_bitmap2(bitmap, EXT2FS_UNMARK_ERROR, bitno);
		ଫେରନ୍ତୁ 0;
	}
	ଫେରନ୍ତୁ ext2fs_clear_bit(bitno - bitmap32->start, bitmap32->bitmap);
}

__u32 ext2fs_get_generic_bitmap_start(ext2fs_generic_bitmap bitmap)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	ଯଦି (!EXT2FS_IS_32_BITMAP(bitmap)) {
		ଯଦି (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			ଫେରନ୍ତୁ ext2fs_get_generic_bmap_start(bitmap);
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"get_bitmap_start");
#endif
		ଫେରନ୍ତୁ 0;
	}

	ଫେରନ୍ତୁ bitmap32->start;
}

__u32 ext2fs_get_generic_bitmap_end(ext2fs_generic_bitmap bitmap)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	ଯଦି (!EXT2FS_IS_32_BITMAP(bitmap)) {
		ଯଦି (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			ଫେରନ୍ତୁ ext2fs_get_generic_bmap_end(bitmap);
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"get_bitmap_end");
#endif
		ଫେରନ୍ତୁ 0;
	}
	ଫେରନ୍ତୁ bitmap32->end;
}

ଶୂନ୍ୟ ext2fs_clear_generic_bitmap(ext2fs_generic_bitmap bitmap)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	ଯଦି (!EXT2FS_IS_32_BITMAP(bitmap)) {
		ଯଦି (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			ext2fs_clear_generic_bmap(bitmap);
			ଫେରନ୍ତୁ;
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"clear_generic_bitmap");
#endif
		ଫେରନ୍ତୁ;
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

	ଯଦି (end > bitmap->real_end)
		ଫେରନ୍ତୁ neq;
	ଯଦି (oend)
		*oend = bitmap->end;
	bitmap->end = end;
	ଫେରନ୍ତୁ 0;
}

errcode_t ext2fs_resize_generic_bitmap(errcode_t magic,
				       __u32 new_end, __u32 new_real_end,
				       ext2fs_generic_bitmap gen_bmap)
{
	ext2fs_generic_bitmap_32 bmap = (ext2fs_generic_bitmap_32) gen_bmap;
	errcode_t	retval;
	size_t		size, new_size;
	__u32		bitno;

	ଯଦି (!bmap || (bmap->magic != magic))
		ଫେରନ୍ତୁ magic;

	/*
	 * If we're expanding the bitmap, make sure all of the new
	 * parts of the bitmap are zero.
	 */
	ଯଦି (new_end > bmap->end) {
		bitno = bmap->real_end;
		ଯଦି (bitno > new_end)
			bitno = new_end;
		ପାଇଁ (; bitno > bmap->end; bitno--)
			ext2fs_clear_bit(bitno - bmap->start, bmap->bitmap);
	}
	ଯଦି (new_real_end == bmap->real_end) {
		bmap->end = new_end;
		ଫେରନ୍ତୁ 0;
	}

	size = ((bmap->real_end - bmap->start) / 8) + 1;
	new_size = ((new_real_end - bmap->start) / 8) + 1;

	ଯଦି (size != new_size) {
		retval = ext2fs_resize_mem(size, new_size, &bmap->bitmap);
		ଯଦି (retval)
			ଫେରନ୍ତୁ retval;
	}
	ଯଦି (new_size > size)
		memset(bmap->bitmap + size, 0, new_size - size);

	bmap->end = new_end;
	bmap->real_end = new_real_end;
	ଫେରନ୍ତୁ 0;
}

errcode_t ext2fs_compare_generic_bitmap(errcode_t magic, errcode_t neq,
					ext2fs_generic_bitmap gen_bm1,
					ext2fs_generic_bitmap gen_bm2)
{
	ext2fs_generic_bitmap_32 bm1 = (ext2fs_generic_bitmap_32) gen_bm1;
	ext2fs_generic_bitmap_32 bm2 = (ext2fs_generic_bitmap_32) gen_bm2;
	blk_t	i;

	ଯଦି (!bm1 || bm1->magic != magic)
		ଫେରନ୍ତୁ magic;
	ଯଦି (!bm2 || bm2->magic != magic)
		ଫେରନ୍ତୁ magic;

	ଯଦି ((bm1->start != bm2->start) ||
	    (bm1->end != bm2->end) ||
	    (memcmp(bm1->bitmap, bm2->bitmap,
		    (size_t) (bm1->end - bm1->start)/8)))
		ଫେରନ୍ତୁ neq;

	ପାଇଁ (i = bm1->end - ((bm1->end - bm1->start) % 8); i <= bm1->end; i++)
		ଯଦି (ext2fs_fast_test_block_bitmap(gen_bm1, i) !=
		    ext2fs_fast_test_block_bitmap(gen_bm2, i))
			ଫେରନ୍ତୁ neq;

	ଫେରନ୍ତୁ 0;
}

ଶୂନ୍ୟ ext2fs_set_generic_bitmap_padding(ext2fs_generic_bitmap gen_map)
{
	ext2fs_generic_bitmap_32 map = (ext2fs_generic_bitmap_32) gen_map;
	__u32	i, j;

	/* Protect loop from wrap-around if map->real_end is maxed */
	ପାଇଁ (i=map->end+1, j = i - map->start;
	     i <= map->real_end && i > map->end;
	     i++, j++)
		ext2fs_set_bit(j, map->bitmap);
}

errcode_t ext2fs_get_generic_bitmap_range(ext2fs_generic_bitmap gen_bmap,
					  errcode_t magic,
					  __u32 start, __u32 ଅଙ୍କ,
					  ଶୂନ୍ୟ *out)
{
	ext2fs_generic_bitmap_32 bmap = (ext2fs_generic_bitmap_32) gen_bmap;

	ଯଦି (!bmap || (bmap->magic != magic))
		ଫେରନ୍ତୁ magic;

	ଯଦି ((start < bmap->start) || (start+ଅଙ୍କ-1 > bmap->real_end))
		ଫେରନ୍ତୁ EXT2_ET_INVALID_ARGUMENT;

	memcpy(out, bmap->bitmap + ((start - bmap->start) >> 3), (ଅଙ୍କ+7) >> 3);
	ଫେରନ୍ତୁ 0;
}

errcode_t ext2fs_set_generic_bitmap_range(ext2fs_generic_bitmap gen_bmap,
					  errcode_t magic,
					  __u32 start, __u32 ଅଙ୍କ,
					  ଶୂନ୍ୟ *in)
{
	ext2fs_generic_bitmap_32 bmap = (ext2fs_generic_bitmap_32) gen_bmap;

	ଯଦି (!bmap || (bmap->magic != magic))
		ଫେରନ୍ତୁ magic;

	ଯଦି ((start < bmap->start) || (start+ଅଙ୍କ-1 > bmap->real_end))
		ଫେରନ୍ତୁ EXT2_ET_INVALID_ARGUMENT;

	memcpy(bmap->bitmap + ((start - bmap->start) >> 3), in, (ଅଙ୍କ+7) >> 3);
	ଫେରନ୍ତୁ 0;
}

/*
 * Compare @mem to zero buffer by 256 bytes.
 * Return 1 if @mem is zeroed memory, otherwise return 0.
 */
ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_mem_is_zero(ଅଚଳ ଅକ୍ଷର *mem, size_t len)
{
	ସ୍ଥିର ଅଚଳ ଅକ୍ଷର zero_buf[256];

	ଯେତେବେଳେ (len >= ଆକାର(zero_buf)) {
		ଯଦି (memcmp(mem, zero_buf, ଆକାର(zero_buf)))
			ଫେରନ୍ତୁ 0;
		len -= ଆକାର(zero_buf);
		mem += ଆକାର(zero_buf);
	}
	/* Deal with leftover bytes. */
	ଯଦି (len)
		ଫେରନ୍ତୁ !memcmp(mem, zero_buf, len);
	ଫେରନ୍ତୁ 1;
}

/*
 * Return true if all of the bits in a specified range are clear
 */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_test_clear_generic_bitmap_range(ext2fs_generic_bitmap gen_bitmap,
						  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start,
						  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା len)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	size_t start_byte, len_byte = len >> 3;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start_bit, len_bit = len % 8;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା first_bit = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା last_bit  = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା mark_ଗଣନା = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା mark_bit = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	ଅଚଳ ଅକ୍ଷର *ADDR = bitmap->bitmap;

	start -= bitmap->start;
	start_byte = start >> 3;
	start_bit = start % 8;

	ଯଦି (start_bit != 0) {
		/*
		 * The compared start block number or start inode number
		 * is not the first bit in a byte.
		 */
		mark_ଗଣନା = 8 - start_bit;
		ଯଦି (len < 8 - start_bit) {
			mark_ଗଣନା = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)len;
			mark_bit = len + start_bit - 1;
		} ଅନ୍ୟଥା
			mark_bit = 7;

		ପାଇଁ (i = mark_ଗଣନା; i > 0; i--, mark_bit--)
			first_bit |= 1 << mark_bit;

		/*
		 * Compare blocks or inodes in the first byte.
		 * If there is any marked bit, this function returns 0.
		 */
		ଯଦି (first_bit & ADDR[start_byte])
			ଫେରନ୍ତୁ 0;
		ଅନ୍ୟଥା ଯଦି (len <= 8 - start_bit)
			ଫେରନ୍ତୁ 1;

		start_byte++;
		len_bit = (len - mark_ଗଣନା) % 8;
		len_byte = (len - mark_ଗଣନା) >> 3;
	}

	/*
	 * The compared start block number or start inode number is
	 * the first bit in a byte.
	 */
	ଯଦି (len_bit != 0) {
		/*
		 * The compared end block number or end inode number is
		 * not the last bit in a byte.
		 */
		ପାଇଁ (mark_bit = len_bit - 1; mark_bit >= 0; mark_bit--)
			last_bit |= 1 << mark_bit;

		/*
		 * Compare blocks or inodes in the last byte.
		 * If there is any marked bit, this function returns 0.
		 */
		ଯଦି (last_bit & ADDR[start_byte + len_byte])
			ଫେରନ୍ତୁ 0;
		ଅନ୍ୟଥା ଯଦି (len_byte == 0)
			ଫେରନ୍ତୁ 1;
	}

	/* Check whether all bytes are 0 */
	ଫେରନ୍ତୁ ext2fs_mem_is_zero(ADDR + start_byte, len_byte);
}

errcode_t ext2fs_find_first_zero_generic_bitmap(ext2fs_generic_bitmap gen_bitmap,
						__u32 start, __u32 end,
						__u32 *out)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	blk_t b;

	ଯଦି (start < bitmap->start || end > bitmap->end || start > end) {
		ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_TEST_ERROR, start);
		ଫେରନ୍ତୁ EINVAL;
	}

	ଯେତେବେଳେ (start <= end) {
		b = ext2fs_test_bit(start - bitmap->start, bitmap->bitmap);
		ଯଦି (!b) {
			*out = start;
			ଫେରନ୍ତୁ 0;
		}
		start++;
	}

	ଫେରନ୍ତୁ ENOENT;
}

errcode_t ext2fs_find_first_set_generic_bitmap(ext2fs_generic_bitmap gen_bitmap,
					       __u32 start, __u32 end,
					       __u32 *out)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	blk_t b;

	ଯଦି (start < bitmap->start || end > bitmap->end || start > end) {
		ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_TEST_ERROR, start);
		ଫେରନ୍ତୁ EINVAL;
	}

	ଯେତେବେଳେ (start <= end) {
		b = ext2fs_test_bit(start - bitmap->start, bitmap->bitmap);
		ଯଦି (b) {
			*out = start;
			ଫେରନ୍ତୁ 0;
		}
		start++;
	}

	ଫେରନ୍ତୁ ENOENT;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_test_block_bitmap_range(ext2fs_block_bitmap gen_bitmap,
				   blk_t block, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	EXT2_CHECK_MAGIC(bitmap, EXT2_ET_MAGIC_BLOCK_BITMAP);
	ଯଦି ((block < bitmap->start) || (block > bitmap->real_end) ||
	    (block+ଅଙ୍କ-1 > bitmap->real_end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_TEST,
				   block, bitmap->description);
		ଫେରନ୍ତୁ 0;
	}
	ଫେରନ୍ତୁ ext2fs_test_clear_generic_bitmap_range((ext2fs_generic_bitmap)
						      bitmap, block, ଅଙ୍କ);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା ext2fs_test_inode_bitmap_range(ext2fs_inode_bitmap gen_bitmap,
				   ext2_ino_t inode, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	EXT2_CHECK_MAGIC(bitmap, EXT2_ET_MAGIC_INODE_BITMAP);
	ଯଦି ((inode < bitmap->start) || (inode > bitmap->real_end) ||
	    (inode+ଅଙ୍କ-1 > bitmap->real_end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_INODE_TEST,
				   inode, bitmap->description);
		ଫେରନ୍ତୁ 0;
	}
	ଫେରନ୍ତୁ ext2fs_test_clear_generic_bitmap_range((ext2fs_generic_bitmap)
						      bitmap, inode, ଅଙ୍କ);
}

ଶୂନ୍ୟ ext2fs_mark_block_bitmap_range(ext2fs_block_bitmap gen_bitmap,
				    blk_t block, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା	i;

	ଯଦି ((block < bitmap->start) || (block > bitmap->end) ||
	    (block+ଅଙ୍କ-1 > bitmap->end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_MARK, block,
				   bitmap->description);
		ଫେରନ୍ତୁ;
	}
	ପାଇଁ (i=0; i < ଅଙ୍କ; i++)
		ext2fs_fast_set_bit(block + i - bitmap->start, bitmap->bitmap);
}

ଶୂନ୍ୟ ext2fs_unmark_block_bitmap_range(ext2fs_block_bitmap gen_bitmap,
				      blk_t block, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା	i;

	ଯଦି ((block < bitmap->start) || (block > bitmap->end) ||
	    (block+ଅଙ୍କ-1 > bitmap->end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_UNMARK, block,
				   bitmap->description);
		ଫେରନ୍ତୁ;
	}
	ପାଇଁ (i=0; i < ଅଙ୍କ; i++)
		ext2fs_fast_clear_bit(block + i - bitmap->start,
				      bitmap->bitmap);
}

