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


#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>
#सचे HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <time.h>
#सचे HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#endif
#सचे HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#endif

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fsP.h"

संरचना ext2fs_struct_generic_bitmap_32 {
	errcode_t	magic;
	ext2_filsys 	fs;
	__u32		start, end;
	__u32		real_end;
	वर्ण	*	description;
	वर्ण	*	bitmap;
	errcode_t	base_error_code;
	__u32		reserved[7];
};

प्रकारनिर्देश संरचना ext2fs_struct_generic_bitmap_32 *ext2fs_generic_bitmap_32;

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
शून्य ext2fs_warn_bitmap2(ext2fs_generic_bitmap gen_bitmap,
			    पूर्णाङ्क code, unsigned long arg)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

#ifndef OMIT_COM_ERR
	सचे (bitmap->description)
		com_err(0, bitmap->base_error_code+code,
			"#%lu for %s", arg, bitmap->description);
	अञ्ञथा
		com_err(0, bitmap->base_error_code + code, "#%lu", arg);
#endif
}

स्थिर errcode_t check_magic(ext2fs_generic_bitmap bitmap)
{
	सचे (!bitmap || !((bitmap->magic == EXT2_ET_MAGIC_GENERIC_BITMAP) ||
			 (bitmap->magic == EXT2_ET_MAGIC_INODE_BITMAP) ||
			 (bitmap->magic == EXT2_ET_MAGIC_BLOCK_BITMAP)))
		पटिनिवत्त EXT2_ET_MAGIC_GENERIC_BITMAP;
	पटिनिवत्त 0;
}

errcode_t ext2fs_make_generic_bitmap(errcode_t magic, ext2_filsys fs,
				     __u32 start, __u32 end, __u32 real_end,
				     ध्रुव वर्ण *descr, वर्ण *init_map,
				     ext2fs_generic_bitmap *ret)
{
	ext2fs_generic_bitmap_32 bitmap;
	errcode_t		retval;
	size_t			size;

	retval = ext2fs_get_mem(परिमाण(संरचना ext2fs_struct_generic_bitmap_32),
				&bitmap);
	सचे (retval)
		पटिनिवत्त retval;

	bitmap->magic = magic;
	bitmap->fs = fs;
	bitmap->start = start;
	bitmap->end = end;
	bitmap->real_end = real_end;
	विकल्प (magic) {
	पक्ष EXT2_ET_MAGIC_INODE_BITMAP:
		bitmap->base_error_code = EXT2_ET_BAD_INODE_MARK;
		भङ्ग;
	पक्ष EXT2_ET_MAGIC_BLOCK_BITMAP:
		bitmap->base_error_code = EXT2_ET_BAD_BLOCK_MARK;
		भङ्ग;
	स्वभाव:
		bitmap->base_error_code = EXT2_ET_BAD_GENERIC_MARK;
	}
	सचे (descr) {
		retval = ext2fs_get_mem(strlen(descr)+1, &bitmap->description);
		सचे (retval) {
			ext2fs_free_mem(&bitmap);
			पटिनिवत्त retval;
		}
		strcpy(bitmap->description, descr);
	} अञ्ञथा
		bitmap->description = 0;

	size = (size_t) (((bitmap->real_end - bitmap->start) / 8) + 1);
	/* Round up to allow for the BT x86 instruction */
	size = (size + 7) & ~3;
	retval = ext2fs_get_mem(size, &bitmap->bitmap);
	सचे (retval) {
		ext2fs_free_mem(&bitmap->description);
		ext2fs_free_mem(&bitmap);
		पटिनिवत्त retval;
	}

	सचे (init_map)
		memcpy(bitmap->bitmap, init_map, size);
	अञ्ञथा
		memset(bitmap->bitmap, 0, size);
	*ret = (ext2fs_generic_bitmap) bitmap;
	पटिनिवत्त 0;
}

errcode_t ext2fs_allocate_generic_bitmap(__u32 start,
					 __u32 end,
					 __u32 real_end,
					 ध्रुव वर्ण *descr,
					 ext2fs_generic_bitmap *ret)
{
	पटिनिवत्त ext2fs_make_generic_bitmap(EXT2_ET_MAGIC_GENERIC_BITMAP, 0,
					  start, end, real_end, descr, 0, ret);
}

errcode_t ext2fs_copy_generic_bitmap(ext2fs_generic_bitmap gen_src,
				     ext2fs_generic_bitmap *dest)
{
	ext2fs_generic_bitmap_32 src = (ext2fs_generic_bitmap_32) gen_src;

	पटिनिवत्त (ext2fs_make_generic_bitmap(src->magic, src->fs,
					   src->start, src->end,
					   src->real_end,
					   src->description, src->bitmap,
					   dest));
}

शून्य ext2fs_free_generic_bitmap(ext2fs_inode_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	सचे (check_magic(gen_bitmap))
		पटिनिवत्त;

	bitmap->magic = 0;
	सचे (bitmap->description) {
		ext2fs_free_mem(&bitmap->description);
		bitmap->description = 0;
	}
	सचे (bitmap->bitmap) {
		ext2fs_free_mem(&bitmap->bitmap);
		bitmap->bitmap = 0;
	}
	ext2fs_free_mem(&bitmap);
}

पूर्णाङ्क ext2fs_test_generic_bitmap(ext2fs_generic_bitmap bitmap,
					blk_t bitno)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	सचे (!EXT2FS_IS_32_BITMAP(bitmap)) {
		सचे (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			पटिनिवत्त ext2fs_test_generic_bmap(bitmap, bitno);
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"test_bitmap(%lu)", (unsigned long) bitno);
#endif
		पटिनिवत्त 0;
	}

	सचे ((bitno < bitmap32->start) || (bitno > bitmap32->end)) {
		ext2fs_warn_bitmap2(bitmap, EXT2FS_TEST_ERROR, bitno);
		पटिनिवत्त 0;
	}
	पटिनिवत्त ext2fs_test_bit(bitno - bitmap32->start, bitmap32->bitmap);
}

पूर्णाङ्क ext2fs_mark_generic_bitmap(ext2fs_generic_bitmap bitmap,
					 __u32 bitno)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	सचे (!EXT2FS_IS_32_BITMAP(bitmap)) {
		सचे (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			पटिनिवत्त ext2fs_mark_generic_bmap(bitmap, bitno);
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"mark_bitmap(%lu)", (unsigned long) bitno);
#endif
		पटिनिवत्त 0;
	}

	सचे ((bitno < bitmap32->start) || (bitno > bitmap32->end)) {
		ext2fs_warn_bitmap2(bitmap, EXT2FS_MARK_ERROR, bitno);
		पटिनिवत्त 0;
	}
	पटिनिवत्त ext2fs_set_bit(bitno - bitmap32->start, bitmap32->bitmap);
}

पूर्णाङ्क ext2fs_unmark_generic_bitmap(ext2fs_generic_bitmap bitmap,
					   blk_t bitno)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	सचे (!EXT2FS_IS_32_BITMAP(bitmap)) {
		सचे (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			पटिनिवत्त ext2fs_unmark_generic_bmap(bitmap, bitno);
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"mark_bitmap(%lu)", (unsigned long) bitno);
#endif
		पटिनिवत्त 0;
	}

	सचे ((bitno < bitmap32->start) || (bitno > bitmap32->end)) {
		ext2fs_warn_bitmap2(bitmap, EXT2FS_UNMARK_ERROR, bitno);
		पटिनिवत्त 0;
	}
	पटिनिवत्त ext2fs_clear_bit(bitno - bitmap32->start, bitmap32->bitmap);
}

__u32 ext2fs_get_generic_bitmap_start(ext2fs_generic_bitmap bitmap)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	सचे (!EXT2FS_IS_32_BITMAP(bitmap)) {
		सचे (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			पटिनिवत्त ext2fs_get_generic_bmap_start(bitmap);
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"get_bitmap_start");
#endif
		पटिनिवत्त 0;
	}

	पटिनिवत्त bitmap32->start;
}

__u32 ext2fs_get_generic_bitmap_end(ext2fs_generic_bitmap bitmap)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	सचे (!EXT2FS_IS_32_BITMAP(bitmap)) {
		सचे (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			पटिनिवत्त ext2fs_get_generic_bmap_end(bitmap);
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"get_bitmap_end");
#endif
		पटिनिवत्त 0;
	}
	पटिनिवत्त bitmap32->end;
}

शून्य ext2fs_clear_generic_bitmap(ext2fs_generic_bitmap bitmap)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	सचे (!EXT2FS_IS_32_BITMAP(bitmap)) {
		सचे (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			ext2fs_clear_generic_bmap(bitmap);
			पटिनिवत्त;
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"clear_generic_bitmap");
#endif
		पटिनिवत्त;
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

	सचे (end > bitmap->real_end)
		पटिनिवत्त neq;
	सचे (oend)
		*oend = bitmap->end;
	bitmap->end = end;
	पटिनिवत्त 0;
}

errcode_t ext2fs_resize_generic_bitmap(errcode_t magic,
				       __u32 new_end, __u32 new_real_end,
				       ext2fs_generic_bitmap gen_bmap)
{
	ext2fs_generic_bitmap_32 bmap = (ext2fs_generic_bitmap_32) gen_bmap;
	errcode_t	retval;
	size_t		size, new_size;
	__u32		bitno;

	सचे (!bmap || (bmap->magic != magic))
		पटिनिवत्त magic;

	/*
	 * If we're expanding the bitmap, make sure all of the new
	 * parts of the bitmap are zero.
	 */
	सचे (new_end > bmap->end) {
		bitno = bmap->real_end;
		सचे (bitno > new_end)
			bitno = new_end;
		अत्थाय (; bitno > bmap->end; bitno--)
			ext2fs_clear_bit(bitno - bmap->start, bmap->bitmap);
	}
	सचे (new_real_end == bmap->real_end) {
		bmap->end = new_end;
		पटिनिवत्त 0;
	}

	size = ((bmap->real_end - bmap->start) / 8) + 1;
	new_size = ((new_real_end - bmap->start) / 8) + 1;

	सचे (size != new_size) {
		retval = ext2fs_resize_mem(size, new_size, &bmap->bitmap);
		सचे (retval)
			पटिनिवत्त retval;
	}
	सचे (new_size > size)
		memset(bmap->bitmap + size, 0, new_size - size);

	bmap->end = new_end;
	bmap->real_end = new_real_end;
	पटिनिवत्त 0;
}

errcode_t ext2fs_compare_generic_bitmap(errcode_t magic, errcode_t neq,
					ext2fs_generic_bitmap gen_bm1,
					ext2fs_generic_bitmap gen_bm2)
{
	ext2fs_generic_bitmap_32 bm1 = (ext2fs_generic_bitmap_32) gen_bm1;
	ext2fs_generic_bitmap_32 bm2 = (ext2fs_generic_bitmap_32) gen_bm2;
	blk_t	i;

	सचे (!bm1 || bm1->magic != magic)
		पटिनिवत्त magic;
	सचे (!bm2 || bm2->magic != magic)
		पटिनिवत्त magic;

	सचे ((bm1->start != bm2->start) ||
	    (bm1->end != bm2->end) ||
	    (memcmp(bm1->bitmap, bm2->bitmap,
		    (size_t) (bm1->end - bm1->start)/8)))
		पटिनिवत्त neq;

	अत्थाय (i = bm1->end - ((bm1->end - bm1->start) % 8); i <= bm1->end; i++)
		सचे (ext2fs_fast_test_block_bitmap(gen_bm1, i) !=
		    ext2fs_fast_test_block_bitmap(gen_bm2, i))
			पटिनिवत्त neq;

	पटिनिवत्त 0;
}

शून्य ext2fs_set_generic_bitmap_padding(ext2fs_generic_bitmap gen_map)
{
	ext2fs_generic_bitmap_32 map = (ext2fs_generic_bitmap_32) gen_map;
	__u32	i, j;

	/* Protect loop from wrap-around if map->real_end is maxed */
	अत्थाय (i=map->end+1, j = i - map->start;
	     i <= map->real_end && i > map->end;
	     i++, j++)
		ext2fs_set_bit(j, map->bitmap);
}

errcode_t ext2fs_get_generic_bitmap_range(ext2fs_generic_bitmap gen_bmap,
					  errcode_t magic,
					  __u32 start, __u32 अङ्कः,
					  शून्य *out)
{
	ext2fs_generic_bitmap_32 bmap = (ext2fs_generic_bitmap_32) gen_bmap;

	सचे (!bmap || (bmap->magic != magic))
		पटिनिवत्त magic;

	सचे ((start < bmap->start) || (start+अङ्कः-1 > bmap->real_end))
		पटिनिवत्त EXT2_ET_INVALID_ARGUMENT;

	memcpy(out, bmap->bitmap + ((start - bmap->start) >> 3), (अङ्कः+7) >> 3);
	पटिनिवत्त 0;
}

errcode_t ext2fs_set_generic_bitmap_range(ext2fs_generic_bitmap gen_bmap,
					  errcode_t magic,
					  __u32 start, __u32 अङ्कः,
					  शून्य *in)
{
	ext2fs_generic_bitmap_32 bmap = (ext2fs_generic_bitmap_32) gen_bmap;

	सचे (!bmap || (bmap->magic != magic))
		पटिनिवत्त magic;

	सचे ((start < bmap->start) || (start+अङ्कः-1 > bmap->real_end))
		पटिनिवत्त EXT2_ET_INVALID_ARGUMENT;

	memcpy(bmap->bitmap + ((start - bmap->start) >> 3), in, (अङ्कः+7) >> 3);
	पटिनिवत्त 0;
}

/*
 * Compare @mem to zero buffer by 256 bytes.
 * Return 1 if @mem is zeroed memory, otherwise return 0.
 */
पूर्णाङ्क ext2fs_mem_is_zero(ध्रुव वर्ण *mem, size_t len)
{
	स्थिर ध्रुव वर्ण zero_buf[256];

	याव (len >= परिमाण(zero_buf)) {
		सचे (memcmp(mem, zero_buf, परिमाण(zero_buf)))
			पटिनिवत्त 0;
		len -= परिमाण(zero_buf);
		mem += परिमाण(zero_buf);
	}
	/* Deal with leftover bytes. */
	सचे (len)
		पटिनिवत्त !memcmp(mem, zero_buf, len);
	पटिनिवत्त 1;
}

/*
 * Return true if all of the bits in a specified range are clear
 */
स्थिर पूर्णाङ्क ext2fs_test_clear_generic_bitmap_range(ext2fs_generic_bitmap gen_bitmap,
						  unsigned पूर्णाङ्क start,
						  unsigned पूर्णाङ्क len)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	size_t start_byte, len_byte = len >> 3;
	unsigned पूर्णाङ्क start_bit, len_bit = len % 8;
	पूर्णाङ्क first_bit = 0;
	पूर्णाङ्क last_bit  = 0;
	पूर्णाङ्क mark_गणनम् = 0;
	पूर्णाङ्क mark_bit = 0;
	पूर्णाङ्क i;
	ध्रुव वर्ण *ADDR = bitmap->bitmap;

	start -= bitmap->start;
	start_byte = start >> 3;
	start_bit = start % 8;

	सचे (start_bit != 0) {
		/*
		 * The compared start block number or start inode number
		 * is not the first bit in a byte.
		 */
		mark_गणनम् = 8 - start_bit;
		सचे (len < 8 - start_bit) {
			mark_गणनम् = (पूर्णाङ्क)len;
			mark_bit = len + start_bit - 1;
		} अञ्ञथा
			mark_bit = 7;

		अत्थाय (i = mark_गणनम्; i > 0; i--, mark_bit--)
			first_bit |= 1 << mark_bit;

		/*
		 * Compare blocks or inodes in the first byte.
		 * If there is any marked bit, this function returns 0.
		 */
		सचे (first_bit & ADDR[start_byte])
			पटिनिवत्त 0;
		अञ्ञथा सचे (len <= 8 - start_bit)
			पटिनिवत्त 1;

		start_byte++;
		len_bit = (len - mark_गणनम्) % 8;
		len_byte = (len - mark_गणनम्) >> 3;
	}

	/*
	 * The compared start block number or start inode number is
	 * the first bit in a byte.
	 */
	सचे (len_bit != 0) {
		/*
		 * The compared end block number or end inode number is
		 * not the last bit in a byte.
		 */
		अत्थाय (mark_bit = len_bit - 1; mark_bit >= 0; mark_bit--)
			last_bit |= 1 << mark_bit;

		/*
		 * Compare blocks or inodes in the last byte.
		 * If there is any marked bit, this function returns 0.
		 */
		सचे (last_bit & ADDR[start_byte + len_byte])
			पटिनिवत्त 0;
		अञ्ञथा सचे (len_byte == 0)
			पटिनिवत्त 1;
	}

	/* Check whether all bytes are 0 */
	पटिनिवत्त ext2fs_mem_is_zero(ADDR + start_byte, len_byte);
}

errcode_t ext2fs_find_first_zero_generic_bitmap(ext2fs_generic_bitmap gen_bitmap,
						__u32 start, __u32 end,
						__u32 *out)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	blk_t b;

	सचे (start < bitmap->start || end > bitmap->end || start > end) {
		ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_TEST_ERROR, start);
		पटिनिवत्त EINVAL;
	}

	याव (start <= end) {
		b = ext2fs_test_bit(start - bitmap->start, bitmap->bitmap);
		सचे (!b) {
			*out = start;
			पटिनिवत्त 0;
		}
		start++;
	}

	पटिनिवत्त ENOENT;
}

errcode_t ext2fs_find_first_set_generic_bitmap(ext2fs_generic_bitmap gen_bitmap,
					       __u32 start, __u32 end,
					       __u32 *out)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	blk_t b;

	सचे (start < bitmap->start || end > bitmap->end || start > end) {
		ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_TEST_ERROR, start);
		पटिनिवत्त EINVAL;
	}

	याव (start <= end) {
		b = ext2fs_test_bit(start - bitmap->start, bitmap->bitmap);
		सचे (b) {
			*out = start;
			पटिनिवत्त 0;
		}
		start++;
	}

	पटिनिवत्त ENOENT;
}

पूर्णाङ्क ext2fs_test_block_bitmap_range(ext2fs_block_bitmap gen_bitmap,
				   blk_t block, पूर्णाङ्क अङ्कः)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	EXT2_CHECK_MAGIC(bitmap, EXT2_ET_MAGIC_BLOCK_BITMAP);
	सचे ((block < bitmap->start) || (block > bitmap->real_end) ||
	    (block+अङ्कः-1 > bitmap->real_end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_TEST,
				   block, bitmap->description);
		पटिनिवत्त 0;
	}
	पटिनिवत्त ext2fs_test_clear_generic_bitmap_range((ext2fs_generic_bitmap)
						      bitmap, block, अङ्कः);
}

पूर्णाङ्क ext2fs_test_inode_bitmap_range(ext2fs_inode_bitmap gen_bitmap,
				   ext2_ino_t inode, पूर्णाङ्क अङ्कः)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	EXT2_CHECK_MAGIC(bitmap, EXT2_ET_MAGIC_INODE_BITMAP);
	सचे ((inode < bitmap->start) || (inode > bitmap->real_end) ||
	    (inode+अङ्कः-1 > bitmap->real_end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_INODE_TEST,
				   inode, bitmap->description);
		पटिनिवत्त 0;
	}
	पटिनिवत्त ext2fs_test_clear_generic_bitmap_range((ext2fs_generic_bitmap)
						      bitmap, inode, अङ्कः);
}

शून्य ext2fs_mark_block_bitmap_range(ext2fs_block_bitmap gen_bitmap,
				    blk_t block, पूर्णाङ्क अङ्कः)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	पूर्णाङ्क	i;

	सचे ((block < bitmap->start) || (block > bitmap->end) ||
	    (block+अङ्कः-1 > bitmap->end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_MARK, block,
				   bitmap->description);
		पटिनिवत्त;
	}
	अत्थाय (i=0; i < अङ्कः; i++)
		ext2fs_fast_set_bit(block + i - bitmap->start, bitmap->bitmap);
}

शून्य ext2fs_unmark_block_bitmap_range(ext2fs_block_bitmap gen_bitmap,
				      blk_t block, पूर्णाङ्क अङ्कः)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	पूर्णाङ्क	i;

	सचे ((block < bitmap->start) || (block > bitmap->end) ||
	    (block+अङ्कः-1 > bitmap->end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_UNMARK, block,
				   bitmap->description);
		पटिनिवत्त;
	}
	अत्थाय (i=0; i < अङ्कः; i++)
		ext2fs_fast_clear_bit(block + i - bitmap->start,
				      bitmap->bitmap);
}

