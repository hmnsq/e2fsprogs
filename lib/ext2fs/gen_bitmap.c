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


#समावेश "config.h"
#समावेश <stdio.h>
#समावेश <string.h>
#अगर HAVE_UNISTD_H
#समावेश <unistd.h>
#पूर्ण_अगर
#समावेश <fcntl.h>
#समावेश <time.h>
#अगर HAVE_SYS_STAT_H
#समावेश <sys/stat.h>
#पूर्ण_अगर
#अगर HAVE_SYS_TYPES_H
#समावेश <sys/types.h>
#पूर्ण_अगर

#समावेश "ext2_fs.h"
#समावेश "ext2fsP.h"

काष्ठा ext2fs_struct_generic_bitmap_32 {
	errcode_t	magic;
	ext2_filsys 	fs;
	__u32		start, end;
	__u32		real_end;
	अक्षर	*	description;
	अक्षर	*	bitmap;
	errcode_t	base_error_code;
	__u32		reserved[7];
};

प्रकार काष्ठा ext2fs_struct_generic_bitmap_32 *ext2fs_generic_bitmap_32;

#घोषणा EXT2FS_IS_32_BITMAP(bmap) \
	(((bmap)->magic == EXT2_ET_MAGIC_GENERIC_BITMAP) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_BLOCK_BITMAP) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_INODE_BITMAP))

#घोषणा EXT2FS_IS_64_BITMAP(bmap) \
	(((bmap)->magic == EXT2_ET_MAGIC_GENERIC_BITMAP64) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_BLOCK_BITMAP64) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_INODE_BITMAP64))

/*
 * Used by previously inlined function, so we have to export this and
 * not change the function signature
 */
व्योम ext2fs_warn_bitmap2(ext2fs_generic_bitmap gen_bitmap,
			    पूर्णांक code, अचिन्हित दीर्घ arg)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

#अगर_अघोषित OMIT_COM_ERR
	अगर (bitmap->description)
		com_err(0, bitmap->base_error_code+code,
			"#%lu for %s", arg, bitmap->description);
	अन्यथा
		com_err(0, bitmap->base_error_code + code, "#%lu", arg);
#पूर्ण_अगर
}

अटल errcode_t check_magic(ext2fs_generic_bitmap bitmap)
{
	अगर (!bitmap || !((bitmap->magic == EXT2_ET_MAGIC_GENERIC_BITMAP) ||
			 (bitmap->magic == EXT2_ET_MAGIC_INODE_BITMAP) ||
			 (bitmap->magic == EXT2_ET_MAGIC_BLOCK_BITMAP)))
		वापस EXT2_ET_MAGIC_GENERIC_BITMAP;
	वापस 0;
}

errcode_t ext2fs_make_generic_bitmap(errcode_t magic, ext2_filsys fs,
				     __u32 start, __u32 end, __u32 real_end,
				     स्थिर अक्षर *descr, अक्षर *init_map,
				     ext2fs_generic_bitmap *ret)
{
	ext2fs_generic_bitmap_32 bitmap;
	errcode_t		retval;
	माप_प्रकार			आकार;

	retval = ext2fs_get_mem(माप(काष्ठा ext2fs_struct_generic_bitmap_32),
				&bitmap);
	अगर (retval)
		वापस retval;

	bitmap->magic = magic;
	bitmap->fs = fs;
	bitmap->start = start;
	bitmap->end = end;
	bitmap->real_end = real_end;
	चयन (magic) {
	हाल EXT2_ET_MAGIC_INODE_BITMAP:
		bitmap->base_error_code = EXT2_ET_BAD_INODE_MARK;
		अवरोध;
	हाल EXT2_ET_MAGIC_BLOCK_BITMAP:
		bitmap->base_error_code = EXT2_ET_BAD_BLOCK_MARK;
		अवरोध;
	शेष:
		bitmap->base_error_code = EXT2_ET_BAD_GENERIC_MARK;
	}
	अगर (descr) {
		retval = ext2fs_get_mem(म_माप(descr)+1, &bitmap->description);
		अगर (retval) {
			ext2fs_free_mem(&bitmap);
			वापस retval;
		}
		म_नकल(bitmap->description, descr);
	} अन्यथा
		bitmap->description = 0;

	आकार = (माप_प्रकार) (((bitmap->real_end - bitmap->start) / 8) + 1);
	/* Round up to allow for the BT x86 instruction */
	आकार = (आकार + 7) & ~3;
	retval = ext2fs_get_mem(आकार, &bitmap->bitmap);
	अगर (retval) {
		ext2fs_free_mem(&bitmap->description);
		ext2fs_free_mem(&bitmap);
		वापस retval;
	}

	अगर (init_map)
		स_नकल(bitmap->bitmap, init_map, आकार);
	अन्यथा
		स_रखो(bitmap->bitmap, 0, आकार);
	*ret = (ext2fs_generic_bitmap) bitmap;
	वापस 0;
}

errcode_t ext2fs_allocate_generic_bitmap(__u32 start,
					 __u32 end,
					 __u32 real_end,
					 स्थिर अक्षर *descr,
					 ext2fs_generic_bitmap *ret)
{
	वापस ext2fs_make_generic_bitmap(EXT2_ET_MAGIC_GENERIC_BITMAP, 0,
					  start, end, real_end, descr, 0, ret);
}

errcode_t ext2fs_copy_generic_bitmap(ext2fs_generic_bitmap gen_src,
				     ext2fs_generic_bitmap *dest)
{
	ext2fs_generic_bitmap_32 src = (ext2fs_generic_bitmap_32) gen_src;

	वापस (ext2fs_make_generic_bitmap(src->magic, src->fs,
					   src->start, src->end,
					   src->real_end,
					   src->description, src->bitmap,
					   dest));
}

व्योम ext2fs_free_generic_bitmap(ext2fs_inode_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	अगर (check_magic(gen_bitmap))
		वापस;

	bitmap->magic = 0;
	अगर (bitmap->description) {
		ext2fs_free_mem(&bitmap->description);
		bitmap->description = 0;
	}
	अगर (bitmap->bitmap) {
		ext2fs_free_mem(&bitmap->bitmap);
		bitmap->bitmap = 0;
	}
	ext2fs_free_mem(&bitmap);
}

पूर्णांक ext2fs_test_generic_bitmap(ext2fs_generic_bitmap bitmap,
					blk_t bitno)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	अगर (!EXT2FS_IS_32_BITMAP(bitmap)) {
		अगर (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			वापस ext2fs_test_generic_bmap(bitmap, bitno);
		}
#अगर_अघोषित OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"test_bitmap(%lu)", (अचिन्हित दीर्घ) bitno);
#पूर्ण_अगर
		वापस 0;
	}

	अगर ((bitno < bitmap32->start) || (bitno > bitmap32->end)) {
		ext2fs_warn_bitmap2(bitmap, EXT2FS_TEST_ERROR, bitno);
		वापस 0;
	}
	वापस ext2fs_test_bit(bitno - bitmap32->start, bitmap32->bitmap);
}

पूर्णांक ext2fs_mark_generic_bitmap(ext2fs_generic_bitmap bitmap,
					 __u32 bitno)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	अगर (!EXT2FS_IS_32_BITMAP(bitmap)) {
		अगर (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			वापस ext2fs_mark_generic_bmap(bitmap, bitno);
		}
#अगर_अघोषित OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"mark_bitmap(%lu)", (अचिन्हित दीर्घ) bitno);
#पूर्ण_अगर
		वापस 0;
	}

	अगर ((bitno < bitmap32->start) || (bitno > bitmap32->end)) {
		ext2fs_warn_bitmap2(bitmap, EXT2FS_MARK_ERROR, bitno);
		वापस 0;
	}
	वापस ext2fs_set_bit(bitno - bitmap32->start, bitmap32->bitmap);
}

पूर्णांक ext2fs_unmark_generic_bitmap(ext2fs_generic_bitmap bitmap,
					   blk_t bitno)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	अगर (!EXT2FS_IS_32_BITMAP(bitmap)) {
		अगर (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			वापस ext2fs_unmark_generic_bmap(bitmap, bitno);
		}
#अगर_अघोषित OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"mark_bitmap(%lu)", (अचिन्हित दीर्घ) bitno);
#पूर्ण_अगर
		वापस 0;
	}

	अगर ((bitno < bitmap32->start) || (bitno > bitmap32->end)) {
		ext2fs_warn_bitmap2(bitmap, EXT2FS_UNMARK_ERROR, bitno);
		वापस 0;
	}
	वापस ext2fs_clear_bit(bitno - bitmap32->start, bitmap32->bitmap);
}

__u32 ext2fs_get_generic_bitmap_start(ext2fs_generic_bitmap bitmap)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	अगर (!EXT2FS_IS_32_BITMAP(bitmap)) {
		अगर (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			वापस ext2fs_get_generic_bmap_start(bitmap);
		}
#अगर_अघोषित OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"get_bitmap_start");
#पूर्ण_अगर
		वापस 0;
	}

	वापस bitmap32->start;
}

__u32 ext2fs_get_generic_bitmap_end(ext2fs_generic_bitmap bitmap)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	अगर (!EXT2FS_IS_32_BITMAP(bitmap)) {
		अगर (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			वापस ext2fs_get_generic_bmap_end(bitmap);
		}
#अगर_अघोषित OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"get_bitmap_end");
#पूर्ण_अगर
		वापस 0;
	}
	वापस bitmap32->end;
}

व्योम ext2fs_clear_generic_bitmap(ext2fs_generic_bitmap bitmap)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	अगर (!EXT2FS_IS_32_BITMAP(bitmap)) {
		अगर (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			ext2fs_clear_generic_bmap(bitmap);
			वापस;
		}
#अगर_अघोषित OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"clear_generic_bitmap");
#पूर्ण_अगर
		वापस;
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

	अगर (end > bitmap->real_end)
		वापस neq;
	अगर (oend)
		*oend = bitmap->end;
	bitmap->end = end;
	वापस 0;
}

errcode_t ext2fs_resize_generic_bitmap(errcode_t magic,
				       __u32 new_end, __u32 new_real_end,
				       ext2fs_generic_bitmap gen_bmap)
{
	ext2fs_generic_bitmap_32 bmap = (ext2fs_generic_bitmap_32) gen_bmap;
	errcode_t	retval;
	माप_प्रकार		आकार, new_आकार;
	__u32		bitno;

	अगर (!bmap || (bmap->magic != magic))
		वापस magic;

	/*
	 * If we're expanding the bitmap, make sure all of the new
	 * parts of the bitmap are zero.
	 */
	अगर (new_end > bmap->end) {
		bitno = bmap->real_end;
		अगर (bitno > new_end)
			bitno = new_end;
		क्रम (; bitno > bmap->end; bitno--)
			ext2fs_clear_bit(bitno - bmap->start, bmap->bitmap);
	}
	अगर (new_real_end == bmap->real_end) {
		bmap->end = new_end;
		वापस 0;
	}

	आकार = ((bmap->real_end - bmap->start) / 8) + 1;
	new_आकार = ((new_real_end - bmap->start) / 8) + 1;

	अगर (आकार != new_आकार) {
		retval = ext2fs_resize_mem(आकार, new_आकार, &bmap->bitmap);
		अगर (retval)
			वापस retval;
	}
	अगर (new_आकार > आकार)
		स_रखो(bmap->bitmap + आकार, 0, new_आकार - आकार);

	bmap->end = new_end;
	bmap->real_end = new_real_end;
	वापस 0;
}

errcode_t ext2fs_compare_generic_bitmap(errcode_t magic, errcode_t neq,
					ext2fs_generic_bitmap gen_bm1,
					ext2fs_generic_bitmap gen_bm2)
{
	ext2fs_generic_bitmap_32 bm1 = (ext2fs_generic_bitmap_32) gen_bm1;
	ext2fs_generic_bitmap_32 bm2 = (ext2fs_generic_bitmap_32) gen_bm2;
	blk_t	i;

	अगर (!bm1 || bm1->magic != magic)
		वापस magic;
	अगर (!bm2 || bm2->magic != magic)
		वापस magic;

	अगर ((bm1->start != bm2->start) ||
	    (bm1->end != bm2->end) ||
	    (स_भेद(bm1->bitmap, bm2->bitmap,
		    (माप_प्रकार) (bm1->end - bm1->start)/8)))
		वापस neq;

	क्रम (i = bm1->end - ((bm1->end - bm1->start) % 8); i <= bm1->end; i++)
		अगर (ext2fs_fast_test_block_bitmap(gen_bm1, i) !=
		    ext2fs_fast_test_block_bitmap(gen_bm2, i))
			वापस neq;

	वापस 0;
}

व्योम ext2fs_set_generic_bitmap_padding(ext2fs_generic_bitmap gen_map)
{
	ext2fs_generic_bitmap_32 map = (ext2fs_generic_bitmap_32) gen_map;
	__u32	i, j;

	/* Protect loop from wrap-around if map->real_end is maxed */
	क्रम (i=map->end+1, j = i - map->start;
	     i <= map->real_end && i > map->end;
	     i++, j++)
		ext2fs_set_bit(j, map->bitmap);
}

errcode_t ext2fs_get_generic_bitmap_range(ext2fs_generic_bitmap gen_bmap,
					  errcode_t magic,
					  __u32 start, __u32 अंक,
					  व्योम *out)
{
	ext2fs_generic_bitmap_32 bmap = (ext2fs_generic_bitmap_32) gen_bmap;

	अगर (!bmap || (bmap->magic != magic))
		वापस magic;

	अगर ((start < bmap->start) || (start+अंक-1 > bmap->real_end))
		वापस EXT2_ET_INVALID_ARGUMENT;

	स_नकल(out, bmap->bitmap + ((start - bmap->start) >> 3), (अंक+7) >> 3);
	वापस 0;
}

errcode_t ext2fs_set_generic_bitmap_range(ext2fs_generic_bitmap gen_bmap,
					  errcode_t magic,
					  __u32 start, __u32 अंक,
					  व्योम *in)
{
	ext2fs_generic_bitmap_32 bmap = (ext2fs_generic_bitmap_32) gen_bmap;

	अगर (!bmap || (bmap->magic != magic))
		वापस magic;

	अगर ((start < bmap->start) || (start+अंक-1 > bmap->real_end))
		वापस EXT2_ET_INVALID_ARGUMENT;

	स_नकल(bmap->bitmap + ((start - bmap->start) >> 3), in, (अंक+7) >> 3);
	वापस 0;
}

/*
 * Compare @mem to zero buffer by 256 bytes.
 * Return 1 if @mem is zeroed memory, otherwise return 0.
 */
पूर्णांक ext2fs_mem_is_zero(स्थिर अक्षर *mem, माप_प्रकार len)
{
	अटल स्थिर अक्षर zero_buf[256];

	जबतक (len >= माप(zero_buf)) {
		अगर (स_भेद(mem, zero_buf, माप(zero_buf)))
			वापस 0;
		len -= माप(zero_buf);
		mem += माप(zero_buf);
	}
	/* Deal with leftover bytes. */
	अगर (len)
		वापस !स_भेद(mem, zero_buf, len);
	वापस 1;
}

/*
 * Return true if all of the bits in a specified range are clear
 */
अटल पूर्णांक ext2fs_test_clear_generic_bitmap_range(ext2fs_generic_bitmap gen_bitmap,
						  अचिन्हित पूर्णांक start,
						  अचिन्हित पूर्णांक len)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	माप_प्रकार start_byte, len_byte = len >> 3;
	अचिन्हित पूर्णांक start_bit, len_bit = len % 8;
	पूर्णांक first_bit = 0;
	पूर्णांक last_bit  = 0;
	पूर्णांक mark_गिनती = 0;
	पूर्णांक mark_bit = 0;
	पूर्णांक i;
	स्थिर अक्षर *ADDR = bitmap->bitmap;

	start -= bitmap->start;
	start_byte = start >> 3;
	start_bit = start % 8;

	अगर (start_bit != 0) {
		/*
		 * The compared start block number or start inode number
		 * is not the first bit in a byte.
		 */
		mark_गिनती = 8 - start_bit;
		अगर (len < 8 - start_bit) {
			mark_गिनती = (पूर्णांक)len;
			mark_bit = len + start_bit - 1;
		} अन्यथा
			mark_bit = 7;

		क्रम (i = mark_गिनती; i > 0; i--, mark_bit--)
			first_bit |= 1 << mark_bit;

		/*
		 * Compare blocks or inodes in the first byte.
		 * If there is any marked bit, this function returns 0.
		 */
		अगर (first_bit & ADDR[start_byte])
			वापस 0;
		अन्यथा अगर (len <= 8 - start_bit)
			वापस 1;

		start_byte++;
		len_bit = (len - mark_गिनती) % 8;
		len_byte = (len - mark_गिनती) >> 3;
	}

	/*
	 * The compared start block number or start inode number is
	 * the first bit in a byte.
	 */
	अगर (len_bit != 0) {
		/*
		 * The compared end block number or end inode number is
		 * not the last bit in a byte.
		 */
		क्रम (mark_bit = len_bit - 1; mark_bit >= 0; mark_bit--)
			last_bit |= 1 << mark_bit;

		/*
		 * Compare blocks or inodes in the last byte.
		 * If there is any marked bit, this function returns 0.
		 */
		अगर (last_bit & ADDR[start_byte + len_byte])
			वापस 0;
		अन्यथा अगर (len_byte == 0)
			वापस 1;
	}

	/* Check whether all bytes are 0 */
	वापस ext2fs_mem_is_zero(ADDR + start_byte, len_byte);
}

errcode_t ext2fs_find_first_zero_generic_bitmap(ext2fs_generic_bitmap gen_bitmap,
						__u32 start, __u32 end,
						__u32 *out)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	blk_t b;

	अगर (start < bitmap->start || end > bitmap->end || start > end) {
		ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_TEST_ERROR, start);
		वापस EINVAL;
	}

	जबतक (start <= end) {
		b = ext2fs_test_bit(start - bitmap->start, bitmap->bitmap);
		अगर (!b) {
			*out = start;
			वापस 0;
		}
		start++;
	}

	वापस ENOENT;
}

errcode_t ext2fs_find_first_set_generic_bitmap(ext2fs_generic_bitmap gen_bitmap,
					       __u32 start, __u32 end,
					       __u32 *out)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	blk_t b;

	अगर (start < bitmap->start || end > bitmap->end || start > end) {
		ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_TEST_ERROR, start);
		वापस EINVAL;
	}

	जबतक (start <= end) {
		b = ext2fs_test_bit(start - bitmap->start, bitmap->bitmap);
		अगर (b) {
			*out = start;
			वापस 0;
		}
		start++;
	}

	वापस ENOENT;
}

पूर्णांक ext2fs_test_block_bitmap_range(ext2fs_block_bitmap gen_bitmap,
				   blk_t block, पूर्णांक अंक)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	EXT2_CHECK_MAGIC(bitmap, EXT2_ET_MAGIC_BLOCK_BITMAP);
	अगर ((block < bitmap->start) || (block > bitmap->real_end) ||
	    (block+अंक-1 > bitmap->real_end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_TEST,
				   block, bitmap->description);
		वापस 0;
	}
	वापस ext2fs_test_clear_generic_bitmap_range((ext2fs_generic_bitmap)
						      bitmap, block, अंक);
}

पूर्णांक ext2fs_test_inode_bitmap_range(ext2fs_inode_bitmap gen_bitmap,
				   ext2_ino_t inode, पूर्णांक अंक)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	EXT2_CHECK_MAGIC(bitmap, EXT2_ET_MAGIC_INODE_BITMAP);
	अगर ((inode < bitmap->start) || (inode > bitmap->real_end) ||
	    (inode+अंक-1 > bitmap->real_end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_INODE_TEST,
				   inode, bitmap->description);
		वापस 0;
	}
	वापस ext2fs_test_clear_generic_bitmap_range((ext2fs_generic_bitmap)
						      bitmap, inode, अंक);
}

व्योम ext2fs_mark_block_bitmap_range(ext2fs_block_bitmap gen_bitmap,
				    blk_t block, पूर्णांक अंक)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	पूर्णांक	i;

	अगर ((block < bitmap->start) || (block > bitmap->end) ||
	    (block+अंक-1 > bitmap->end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_MARK, block,
				   bitmap->description);
		वापस;
	}
	क्रम (i=0; i < अंक; i++)
		ext2fs_fast_set_bit(block + i - bitmap->start, bitmap->bitmap);
}

व्योम ext2fs_unmark_block_bitmap_range(ext2fs_block_bitmap gen_bitmap,
				      blk_t block, पूर्णांक अंक)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	पूर्णांक	i;

	अगर ((block < bitmap->start) || (block > bitmap->end) ||
	    (block+अंक-1 > bitmap->end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_UNMARK, block,
				   bitmap->description);
		वापस;
	}
	क्रम (i=0; i < अंक; i++)
		ext2fs_fast_clear_bit(block + i - bitmap->start,
				      bitmap->bitmap);
}

