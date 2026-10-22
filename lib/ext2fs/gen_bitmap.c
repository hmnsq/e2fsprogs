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
#जर HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <time.h>
#जर HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#endif
#जर HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#endif

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fsP.h"

संरचना ext2fs_struct_generic_bitmap_32 {
	errcode_t	magic;
	ext2_filsys 	fs;
	__u32		start, end;
	__u32		real_end;
	अक्षर	*	description;
	अक्षर	*	bitmap;
	errcode_t	base_error_code;
	__u32		reserved[7];
};

प्रकारव्याख्या संरचना ext2fs_struct_generic_bitmap_32 *ext2fs_generic_bitmap_32;

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
			    पूर्णांक code, अचिन्हित दीर्घ arg)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

#ifndef OMIT_COM_ERR
	जर (bitmap->description)
		com_err(0, bitmap->base_error_code+code,
			"#%lu for %s", arg, bitmap->description);
	अन्यथा
		com_err(0, bitmap->base_error_code + code, "#%lu", arg);
#endif
}

स्थिर errcode_t check_magic(ext2fs_generic_bitmap bitmap)
{
	जर (!bitmap || !((bitmap->magic == EXT2_ET_MAGIC_GENERIC_BITMAP) ||
			 (bitmap->magic == EXT2_ET_MAGIC_INODE_BITMAP) ||
			 (bitmap->magic == EXT2_ET_MAGIC_BLOCK_BITMAP)))
		परत EXT2_ET_MAGIC_GENERIC_BITMAP;
	परत 0;
}

errcode_t ext2fs_make_generic_bitmap(errcode_t magic, ext2_filsys fs,
				     __u32 start, __u32 end, __u32 real_end,
				     स्थिरांक अक्षर *descr, अक्षर *init_map,
				     ext2fs_generic_bitmap *ret)
{
	ext2fs_generic_bitmap_32 bitmap;
	errcode_t		retval;
	माप_प्रकार			size;

	retval = ext2fs_get_mem(आकार(संरचना ext2fs_struct_generic_bitmap_32),
				&bitmap);
	जर (retval)
		परत retval;

	bitmap->magic = magic;
	bitmap->fs = fs;
	bitmap->start = start;
	bitmap->end = end;
	bitmap->real_end = real_end;
	निवड (magic) {
	स्थिती EXT2_ET_MAGIC_INODE_BITMAP:
		bitmap->base_error_code = EXT2_ET_BAD_INODE_MARK;
		थांबा;
	स्थिती EXT2_ET_MAGIC_BLOCK_BITMAP:
		bitmap->base_error_code = EXT2_ET_BAD_BLOCK_MARK;
		थांबा;
	पूर्वनिर्धारित:
		bitmap->base_error_code = EXT2_ET_BAD_GENERIC_MARK;
	}
	जर (descr) {
		retval = ext2fs_get_mem(म_माप(descr)+1, &bitmap->description);
		जर (retval) {
			ext2fs_free_mem(&bitmap);
			परत retval;
		}
		म_नकल(bitmap->description, descr);
	} अन्यथा
		bitmap->description = 0;

	size = (माप_प्रकार) (((bitmap->real_end - bitmap->start) / 8) + 1);
	/* Round up to allow for the BT x86 instruction */
	size = (size + 7) & ~3;
	retval = ext2fs_get_mem(size, &bitmap->bitmap);
	जर (retval) {
		ext2fs_free_mem(&bitmap->description);
		ext2fs_free_mem(&bitmap);
		परत retval;
	}

	जर (init_map)
		स_नकल(bitmap->bitmap, init_map, size);
	अन्यथा
		स_रखो(bitmap->bitmap, 0, size);
	*ret = (ext2fs_generic_bitmap) bitmap;
	परत 0;
}

errcode_t ext2fs_allocate_generic_bitmap(__u32 start,
					 __u32 end,
					 __u32 real_end,
					 स्थिरांक अक्षर *descr,
					 ext2fs_generic_bitmap *ret)
{
	परत ext2fs_make_generic_bitmap(EXT2_ET_MAGIC_GENERIC_BITMAP, 0,
					  start, end, real_end, descr, 0, ret);
}

errcode_t ext2fs_copy_generic_bitmap(ext2fs_generic_bitmap gen_src,
				     ext2fs_generic_bitmap *dest)
{
	ext2fs_generic_bitmap_32 src = (ext2fs_generic_bitmap_32) gen_src;

	परत (ext2fs_make_generic_bitmap(src->magic, src->fs,
					   src->start, src->end,
					   src->real_end,
					   src->description, src->bitmap,
					   dest));
}

शून्य ext2fs_free_generic_bitmap(ext2fs_inode_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	जर (check_magic(gen_bitmap))
		परत;

	bitmap->magic = 0;
	जर (bitmap->description) {
		ext2fs_free_mem(&bitmap->description);
		bitmap->description = 0;
	}
	जर (bitmap->bitmap) {
		ext2fs_free_mem(&bitmap->bitmap);
		bitmap->bitmap = 0;
	}
	ext2fs_free_mem(&bitmap);
}

पूर्णांक ext2fs_test_generic_bitmap(ext2fs_generic_bitmap bitmap,
					blk_t bitno)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	जर (!EXT2FS_IS_32_BITMAP(bitmap)) {
		जर (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			परत ext2fs_test_generic_bmap(bitmap, bitno);
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"test_bitmap(%lu)", (अचिन्हित दीर्घ) bitno);
#endif
		परत 0;
	}

	जर ((bitno < bitmap32->start) || (bitno > bitmap32->end)) {
		ext2fs_warn_bitmap2(bitmap, EXT2FS_TEST_ERROR, bitno);
		परत 0;
	}
	परत ext2fs_test_bit(bitno - bitmap32->start, bitmap32->bitmap);
}

पूर्णांक ext2fs_mark_generic_bitmap(ext2fs_generic_bitmap bitmap,
					 __u32 bitno)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	जर (!EXT2FS_IS_32_BITMAP(bitmap)) {
		जर (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			परत ext2fs_mark_generic_bmap(bitmap, bitno);
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"mark_bitmap(%lu)", (अचिन्हित दीर्घ) bitno);
#endif
		परत 0;
	}

	जर ((bitno < bitmap32->start) || (bitno > bitmap32->end)) {
		ext2fs_warn_bitmap2(bitmap, EXT2FS_MARK_ERROR, bitno);
		परत 0;
	}
	परत ext2fs_set_bit(bitno - bitmap32->start, bitmap32->bitmap);
}

पूर्णांक ext2fs_unmark_generic_bitmap(ext2fs_generic_bitmap bitmap,
					   blk_t bitno)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	जर (!EXT2FS_IS_32_BITMAP(bitmap)) {
		जर (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			परत ext2fs_unmark_generic_bmap(bitmap, bitno);
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"mark_bitmap(%lu)", (अचिन्हित दीर्घ) bitno);
#endif
		परत 0;
	}

	जर ((bitno < bitmap32->start) || (bitno > bitmap32->end)) {
		ext2fs_warn_bitmap2(bitmap, EXT2FS_UNMARK_ERROR, bitno);
		परत 0;
	}
	परत ext2fs_clear_bit(bitno - bitmap32->start, bitmap32->bitmap);
}

__u32 ext2fs_get_generic_bitmap_start(ext2fs_generic_bitmap bitmap)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	जर (!EXT2FS_IS_32_BITMAP(bitmap)) {
		जर (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			परत ext2fs_get_generic_bmap_start(bitmap);
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"get_bitmap_start");
#endif
		परत 0;
	}

	परत bitmap32->start;
}

__u32 ext2fs_get_generic_bitmap_end(ext2fs_generic_bitmap bitmap)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	जर (!EXT2FS_IS_32_BITMAP(bitmap)) {
		जर (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			परत ext2fs_get_generic_bmap_end(bitmap);
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"get_bitmap_end");
#endif
		परत 0;
	}
	परत bitmap32->end;
}

शून्य ext2fs_clear_generic_bitmap(ext2fs_generic_bitmap bitmap)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	जर (!EXT2FS_IS_32_BITMAP(bitmap)) {
		जर (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			ext2fs_clear_generic_bmap(bitmap);
			परत;
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"clear_generic_bitmap");
#endif
		परत;
	}

	स_रखो(bitmap32->bitmap, 0,
	       (माप_प्रकार) (((bitmap32->real_end - bitmap32->start) / 8) + 1));
}

errcode_t ext2fs_fudge_generic_bitmap_end(ext2fs_inode_bitmap gen_bitmap,
					  errcode_t magic, errcode_t neq,
					  ext2_ino_t end, ext2_ino_t *oend)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	EXT2_CHECK_MAGIC(bitmap, magic);

	जर (end > bitmap->real_end)
		परत neq;
	जर (oend)
		*oend = bitmap->end;
	bitmap->end = end;
	परत 0;
}

errcode_t ext2fs_resize_generic_bitmap(errcode_t magic,
				       __u32 new_end, __u32 new_real_end,
				       ext2fs_generic_bitmap gen_bmap)
{
	ext2fs_generic_bitmap_32 bmap = (ext2fs_generic_bitmap_32) gen_bmap;
	errcode_t	retval;
	माप_प्रकार		size, new_size;
	__u32		bitno;

	जर (!bmap || (bmap->magic != magic))
		परत magic;

	/*
	 * If we're expanding the bitmap, make sure all of the new
	 * parts of the bitmap are zero.
	 */
	जर (new_end > bmap->end) {
		bitno = bmap->real_end;
		जर (bitno > new_end)
			bitno = new_end;
		साठी (; bitno > bmap->end; bitno--)
			ext2fs_clear_bit(bitno - bmap->start, bmap->bitmap);
	}
	जर (new_real_end == bmap->real_end) {
		bmap->end = new_end;
		परत 0;
	}

	size = ((bmap->real_end - bmap->start) / 8) + 1;
	new_size = ((new_real_end - bmap->start) / 8) + 1;

	जर (size != new_size) {
		retval = ext2fs_resize_mem(size, new_size, &bmap->bitmap);
		जर (retval)
			परत retval;
	}
	जर (new_size > size)
		स_रखो(bmap->bitmap + size, 0, new_size - size);

	bmap->end = new_end;
	bmap->real_end = new_real_end;
	परत 0;
}

errcode_t ext2fs_compare_generic_bitmap(errcode_t magic, errcode_t neq,
					ext2fs_generic_bitmap gen_bm1,
					ext2fs_generic_bitmap gen_bm2)
{
	ext2fs_generic_bitmap_32 bm1 = (ext2fs_generic_bitmap_32) gen_bm1;
	ext2fs_generic_bitmap_32 bm2 = (ext2fs_generic_bitmap_32) gen_bm2;
	blk_t	i;

	जर (!bm1 || bm1->magic != magic)
		परत magic;
	जर (!bm2 || bm2->magic != magic)
		परत magic;

	जर ((bm1->start != bm2->start) ||
	    (bm1->end != bm2->end) ||
	    (स_भेद(bm1->bitmap, bm2->bitmap,
		    (माप_प्रकार) (bm1->end - bm1->start)/8)))
		परत neq;

	साठी (i = bm1->end - ((bm1->end - bm1->start) % 8); i <= bm1->end; i++)
		जर (ext2fs_fast_test_block_bitmap(gen_bm1, i) !=
		    ext2fs_fast_test_block_bitmap(gen_bm2, i))
			परत neq;

	परत 0;
}

शून्य ext2fs_set_generic_bitmap_padding(ext2fs_generic_bitmap gen_map)
{
	ext2fs_generic_bitmap_32 map = (ext2fs_generic_bitmap_32) gen_map;
	__u32	i, j;

	/* Protect loop from wrap-around if map->real_end is maxed */
	साठी (i=map->end+1, j = i - map->start;
	     i <= map->real_end && i > map->end;
	     i++, j++)
		ext2fs_set_bit(j, map->bitmap);
}

errcode_t ext2fs_get_generic_bitmap_range(ext2fs_generic_bitmap gen_bmap,
					  errcode_t magic,
					  __u32 start, __u32 अंक,
					  शून्य *out)
{
	ext2fs_generic_bitmap_32 bmap = (ext2fs_generic_bitmap_32) gen_bmap;

	जर (!bmap || (bmap->magic != magic))
		परत magic;

	जर ((start < bmap->start) || (start+अंक-1 > bmap->real_end))
		परत EXT2_ET_INVALID_ARGUMENT;

	स_नकल(out, bmap->bitmap + ((start - bmap->start) >> 3), (अंक+7) >> 3);
	परत 0;
}

errcode_t ext2fs_set_generic_bitmap_range(ext2fs_generic_bitmap gen_bmap,
					  errcode_t magic,
					  __u32 start, __u32 अंक,
					  शून्य *in)
{
	ext2fs_generic_bitmap_32 bmap = (ext2fs_generic_bitmap_32) gen_bmap;

	जर (!bmap || (bmap->magic != magic))
		परत magic;

	जर ((start < bmap->start) || (start+अंक-1 > bmap->real_end))
		परत EXT2_ET_INVALID_ARGUMENT;

	स_नकल(bmap->bitmap + ((start - bmap->start) >> 3), in, (अंक+7) >> 3);
	परत 0;
}

/*
 * Compare @mem to zero buffer by 256 bytes.
 * Return 1 if @mem is zeroed memory, otherwise return 0.
 */
पूर्णांक ext2fs_mem_is_zero(स्थिरांक अक्षर *mem, माप_प्रकार len)
{
	स्थिर स्थिरांक अक्षर zero_buf[256];

	पर्यंत (len >= आकार(zero_buf)) {
		जर (स_भेद(mem, zero_buf, आकार(zero_buf)))
			परत 0;
		len -= आकार(zero_buf);
		mem += आकार(zero_buf);
	}
	/* Deal with leftover bytes. */
	जर (len)
		परत !स_भेद(mem, zero_buf, len);
	परत 1;
}

/*
 * Return true if all of the bits in a specified range are clear
 */
स्थिर पूर्णांक ext2fs_test_clear_generic_bitmap_range(ext2fs_generic_bitmap gen_bitmap,
						  अचिन्हित पूर्णांक start,
						  अचिन्हित पूर्णांक len)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	माप_प्रकार start_byte, len_byte = len >> 3;
	अचिन्हित पूर्णांक start_bit, len_bit = len % 8;
	पूर्णांक first_bit = 0;
	पूर्णांक last_bit  = 0;
	पूर्णांक mark_मोजणी = 0;
	पूर्णांक mark_bit = 0;
	पूर्णांक i;
	स्थिरांक अक्षर *ADDR = bitmap->bitmap;

	start -= bitmap->start;
	start_byte = start >> 3;
	start_bit = start % 8;

	जर (start_bit != 0) {
		/*
		 * The compared start block number or start inode number
		 * is not the first bit in a byte.
		 */
		mark_मोजणी = 8 - start_bit;
		जर (len < 8 - start_bit) {
			mark_मोजणी = (पूर्णांक)len;
			mark_bit = len + start_bit - 1;
		} अन्यथा
			mark_bit = 7;

		साठी (i = mark_मोजणी; i > 0; i--, mark_bit--)
			first_bit |= 1 << mark_bit;

		/*
		 * Compare blocks or inodes in the first byte.
		 * If there is any marked bit, this function returns 0.
		 */
		जर (first_bit & ADDR[start_byte])
			परत 0;
		अन्यथा जर (len <= 8 - start_bit)
			परत 1;

		start_byte++;
		len_bit = (len - mark_मोजणी) % 8;
		len_byte = (len - mark_मोजणी) >> 3;
	}

	/*
	 * The compared start block number or start inode number is
	 * the first bit in a byte.
	 */
	जर (len_bit != 0) {
		/*
		 * The compared end block number or end inode number is
		 * not the last bit in a byte.
		 */
		साठी (mark_bit = len_bit - 1; mark_bit >= 0; mark_bit--)
			last_bit |= 1 << mark_bit;

		/*
		 * Compare blocks or inodes in the last byte.
		 * If there is any marked bit, this function returns 0.
		 */
		जर (last_bit & ADDR[start_byte + len_byte])
			परत 0;
		अन्यथा जर (len_byte == 0)
			परत 1;
	}

	/* Check whether all bytes are 0 */
	परत ext2fs_mem_is_zero(ADDR + start_byte, len_byte);
}

errcode_t ext2fs_find_first_zero_generic_bitmap(ext2fs_generic_bitmap gen_bitmap,
						__u32 start, __u32 end,
						__u32 *out)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	blk_t b;

	जर (start < bitmap->start || end > bitmap->end || start > end) {
		ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_TEST_ERROR, start);
		परत EINVAL;
	}

	पर्यंत (start <= end) {
		b = ext2fs_test_bit(start - bitmap->start, bitmap->bitmap);
		जर (!b) {
			*out = start;
			परत 0;
		}
		start++;
	}

	परत ENOENT;
}

errcode_t ext2fs_find_first_set_generic_bitmap(ext2fs_generic_bitmap gen_bitmap,
					       __u32 start, __u32 end,
					       __u32 *out)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	blk_t b;

	जर (start < bitmap->start || end > bitmap->end || start > end) {
		ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_TEST_ERROR, start);
		परत EINVAL;
	}

	पर्यंत (start <= end) {
		b = ext2fs_test_bit(start - bitmap->start, bitmap->bitmap);
		जर (b) {
			*out = start;
			परत 0;
		}
		start++;
	}

	परत ENOENT;
}

पूर्णांक ext2fs_test_block_bitmap_range(ext2fs_block_bitmap gen_bitmap,
				   blk_t block, पूर्णांक अंक)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	EXT2_CHECK_MAGIC(bitmap, EXT2_ET_MAGIC_BLOCK_BITMAP);
	जर ((block < bitmap->start) || (block > bitmap->real_end) ||
	    (block+अंक-1 > bitmap->real_end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_TEST,
				   block, bitmap->description);
		परत 0;
	}
	परत ext2fs_test_clear_generic_bitmap_range((ext2fs_generic_bitmap)
						      bitmap, block, अंक);
}

पूर्णांक ext2fs_test_inode_bitmap_range(ext2fs_inode_bitmap gen_bitmap,
				   ext2_ino_t inode, पूर्णांक अंक)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	EXT2_CHECK_MAGIC(bitmap, EXT2_ET_MAGIC_INODE_BITMAP);
	जर ((inode < bitmap->start) || (inode > bitmap->real_end) ||
	    (inode+अंक-1 > bitmap->real_end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_INODE_TEST,
				   inode, bitmap->description);
		परत 0;
	}
	परत ext2fs_test_clear_generic_bitmap_range((ext2fs_generic_bitmap)
						      bitmap, inode, अंक);
}

शून्य ext2fs_mark_block_bitmap_range(ext2fs_block_bitmap gen_bitmap,
				    blk_t block, पूर्णांक अंक)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	पूर्णांक	i;

	जर ((block < bitmap->start) || (block > bitmap->end) ||
	    (block+अंक-1 > bitmap->end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_MARK, block,
				   bitmap->description);
		परत;
	}
	साठी (i=0; i < अंक; i++)
		ext2fs_fast_set_bit(block + i - bitmap->start, bitmap->bitmap);
}

शून्य ext2fs_unmark_block_bitmap_range(ext2fs_block_bitmap gen_bitmap,
				      blk_t block, पूर्णांक अंक)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	पूर्णांक	i;

	जर ((block < bitmap->start) || (block > bitmap->end) ||
	    (block+अंक-1 > bitmap->end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_UNMARK, block,
				   bitmap->description);
		परत;
	}
	साठी (i=0; i < अंक; i++)
		ext2fs_fast_clear_bit(block + i - bitmap->start,
				      bitmap->bitmap);
}

