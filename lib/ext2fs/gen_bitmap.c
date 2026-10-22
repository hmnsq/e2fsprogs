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
#यदि HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#यदि_अन्तः
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <time.h>
#यदि HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#यदि_अन्तः
#यदि HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#यदि_अन्तः

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fsP.h"

संरचना ext2fs_struct_generic_bitmap_32 {
	errcode_t	magic;
	ext2_filsys 	fs;
	__u32		start, अन्तः;
	__u32		real_अन्तः;
	वर्ण	*	description;
	वर्ण	*	bitmap;
	errcode_t	आधारः_error_संहिता;
	__u32		reserved[7];
};

प्रकारनिर्देश संरचना ext2fs_struct_generic_bitmap_32 *ext2fs_generic_bitmap_32;

#परिभाषय EXT2FS_IS_32_BITMAP(bmap) \
	(((bmap)->magic == EXT2_ET_MAGIC_GENERIC_BITMAP) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_खण्डम्_BITMAP) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_INODE_BITMAP))

#परिभाषय EXT2FS_IS_64_BITMAP(bmap) \
	(((bmap)->magic == EXT2_ET_MAGIC_GENERIC_BITMAP64) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_खण्डम्_BITMAP64) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_INODE_BITMAP64))

/*
 * Used by previously inlined function, so we have to export this and
 * not change the function signature
 */
शून्य ext2fs_warn_bitmap2(ext2fs_generic_bitmap gen_bitmap,
			    पूर्णाङ्क संहिता, अचिह्नित दीर्घ arg)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

#यदि_अपरिभाषितम् OMIT_COM_ERR
	यदि (bitmap->description)
		com_err(0, bitmap->आधारः_error_संहिता+संहिता,
			"#%lu for %s", arg, bitmap->description);
	अन्यथा
		com_err(0, bitmap->आधारः_error_संहिता + संहिता, "#%lu", arg);
#यदि_अन्तः
}

स्थिर errcode_t check_magic(ext2fs_generic_bitmap bitmap)
{
	यदि (!bitmap || !((bitmap->magic == EXT2_ET_MAGIC_GENERIC_BITMAP) ||
			 (bitmap->magic == EXT2_ET_MAGIC_INODE_BITMAP) ||
			 (bitmap->magic == EXT2_ET_MAGIC_खण्डम्_BITMAP)))
		प्रत्यागम EXT2_ET_MAGIC_GENERIC_BITMAP;
	प्रत्यागम 0;
}

errcode_t ext2fs_make_generic_bitmap(errcode_t magic, ext2_filsys fs,
				     __u32 start, __u32 अन्तः, __u32 real_अन्तः,
				     ध्रुव वर्ण *descr, वर्ण *init_map,
				     ext2fs_generic_bitmap *ret)
{
	ext2fs_generic_bitmap_32 bitmap;
	errcode_t		retval;
	size_t			आकार;

	retval = ext2fs_get_mem(परिमाण(संरचना ext2fs_struct_generic_bitmap_32),
				&bitmap);
	यदि (retval)
		प्रत्यागम retval;

	bitmap->magic = magic;
	bitmap->fs = fs;
	bitmap->start = start;
	bitmap->अन्तः = अन्तः;
	bitmap->real_अन्तः = real_अन्तः;
	विकल्प (magic) {
	पक्ष EXT2_ET_MAGIC_INODE_BITMAP:
		bitmap->आधारः_error_संहिता = EXT2_ET_BAD_INODE_MARK;
		भङ्ग;
	पक्ष EXT2_ET_MAGIC_खण्डम्_BITMAP:
		bitmap->आधारः_error_संहिता = EXT2_ET_BAD_खण्डम्_MARK;
		भङ्ग;
	स्वभाव:
		bitmap->आधारः_error_संहिता = EXT2_ET_BAD_GENERIC_MARK;
	}
	यदि (descr) {
		retval = ext2fs_get_mem(वाक्यदैर्घ्यम्(descr)+1, &bitmap->description);
		यदि (retval) {
			ext2fs_free_mem(&bitmap);
			प्रत्यागम retval;
		}
		वाक्यं_प्रतिलिख(bitmap->description, descr);
	} अन्यथा
		bitmap->description = 0;

	आकार = (size_t) (((bitmap->real_अन्तः - bitmap->start) / 8) + 1);
	/* Round up to allow for the BT x86 instruction */
	आकार = (आकार + 7) & ~3;
	retval = ext2fs_get_mem(आकार, &bitmap->bitmap);
	यदि (retval) {
		ext2fs_free_mem(&bitmap->description);
		ext2fs_free_mem(&bitmap);
		प्रत्यागम retval;
	}

	यदि (init_map)
		स्मृतिं_प्रतिलिख(bitmap->bitmap, init_map, आकार);
	अन्यथा
		स्मृतिं_पूरय(bitmap->bitmap, 0, आकार);
	*ret = (ext2fs_generic_bitmap) bitmap;
	प्रत्यागम 0;
}

errcode_t ext2fs_allocate_generic_bitmap(__u32 start,
					 __u32 अन्तः,
					 __u32 real_अन्तः,
					 ध्रुव वर्ण *descr,
					 ext2fs_generic_bitmap *ret)
{
	प्रत्यागम ext2fs_make_generic_bitmap(EXT2_ET_MAGIC_GENERIC_BITMAP, 0,
					  start, अन्तः, real_अन्तः, descr, 0, ret);
}

errcode_t ext2fs_copy_generic_bitmap(ext2fs_generic_bitmap gen_src,
				     ext2fs_generic_bitmap *dest)
{
	ext2fs_generic_bitmap_32 src = (ext2fs_generic_bitmap_32) gen_src;

	प्रत्यागम (ext2fs_make_generic_bitmap(src->magic, src->fs,
					   src->start, src->अन्तः,
					   src->real_अन्तः,
					   src->description, src->bitmap,
					   dest));
}

शून्य ext2fs_free_generic_bitmap(ext2fs_inode_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	यदि (check_magic(gen_bitmap))
		प्रत्यागम;

	bitmap->magic = 0;
	यदि (bitmap->description) {
		ext2fs_free_mem(&bitmap->description);
		bitmap->description = 0;
	}
	यदि (bitmap->bitmap) {
		ext2fs_free_mem(&bitmap->bitmap);
		bitmap->bitmap = 0;
	}
	ext2fs_free_mem(&bitmap);
}

पूर्णाङ्क ext2fs_test_generic_bitmap(ext2fs_generic_bitmap bitmap,
					blk_t bitno)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	यदि (!EXT2FS_IS_32_BITMAP(bitmap)) {
		यदि (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			प्रत्यागम ext2fs_test_generic_bmap(bitmap, bitno);
		}
#यदि_अपरिभाषितम् OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"test_bitmap(%lu)", (अचिह्नित दीर्घ) bitno);
#यदि_अन्तः
		प्रत्यागम 0;
	}

	यदि ((bitno < bitmap32->start) || (bitno > bitmap32->अन्तः)) {
		ext2fs_warn_bitmap2(bitmap, EXT2FS_TEST_ERROR, bitno);
		प्रत्यागम 0;
	}
	प्रत्यागम ext2fs_test_bit(bitno - bitmap32->start, bitmap32->bitmap);
}

पूर्णाङ्क ext2fs_mark_generic_bitmap(ext2fs_generic_bitmap bitmap,
					 __u32 bitno)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	यदि (!EXT2FS_IS_32_BITMAP(bitmap)) {
		यदि (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			प्रत्यागम ext2fs_mark_generic_bmap(bitmap, bitno);
		}
#यदि_अपरिभाषितम् OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"mark_bitmap(%lu)", (अचिह्नित दीर्घ) bitno);
#यदि_अन्तः
		प्रत्यागम 0;
	}

	यदि ((bitno < bitmap32->start) || (bitno > bitmap32->अन्तः)) {
		ext2fs_warn_bitmap2(bitmap, EXT2FS_MARK_ERROR, bitno);
		प्रत्यागम 0;
	}
	प्रत्यागम ext2fs_समुच्चयः_bit(bitno - bitmap32->start, bitmap32->bitmap);
}

पूर्णाङ्क ext2fs_unmark_generic_bitmap(ext2fs_generic_bitmap bitmap,
					   blk_t bitno)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	यदि (!EXT2FS_IS_32_BITMAP(bitmap)) {
		यदि (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			प्रत्यागम ext2fs_unmark_generic_bmap(bitmap, bitno);
		}
#यदि_अपरिभाषितम् OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"mark_bitmap(%lu)", (अचिह्नित दीर्घ) bitno);
#यदि_अन्तः
		प्रत्यागम 0;
	}

	यदि ((bitno < bitmap32->start) || (bitno > bitmap32->अन्तः)) {
		ext2fs_warn_bitmap2(bitmap, EXT2FS_UNMARK_ERROR, bitno);
		प्रत्यागम 0;
	}
	प्रत्यागम ext2fs_clear_bit(bitno - bitmap32->start, bitmap32->bitmap);
}

__u32 ext2fs_get_generic_bitmap_start(ext2fs_generic_bitmap bitmap)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	यदि (!EXT2FS_IS_32_BITMAP(bitmap)) {
		यदि (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			प्रत्यागम ext2fs_get_generic_bmap_start(bitmap);
		}
#यदि_अपरिभाषितम् OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"get_bitmap_start");
#यदि_अन्तः
		प्रत्यागम 0;
	}

	प्रत्यागम bitmap32->start;
}

__u32 ext2fs_get_generic_bitmap_अन्तः(ext2fs_generic_bitmap bitmap)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	यदि (!EXT2FS_IS_32_BITMAP(bitmap)) {
		यदि (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			प्रत्यागम ext2fs_get_generic_bmap_अन्तः(bitmap);
		}
#यदि_अपरिभाषितम् OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"get_bitmap_end");
#यदि_अन्तः
		प्रत्यागम 0;
	}
	प्रत्यागम bitmap32->अन्तः;
}

शून्य ext2fs_clear_generic_bitmap(ext2fs_generic_bitmap bitmap)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	यदि (!EXT2FS_IS_32_BITMAP(bitmap)) {
		यदि (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			ext2fs_clear_generic_bmap(bitmap);
			प्रत्यागम;
		}
#यदि_अपरिभाषितम् OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"clear_generic_bitmap");
#यदि_अन्तः
		प्रत्यागम;
	}

	स्मृतिं_पूरय(bitmap32->bitmap, 0,
	       (size_t) (((bitmap32->real_अन्तः - bitmap32->start) / 8) + 1));
}

errcode_t ext2fs_fudge_generic_bitmap_अन्तः(ext2fs_inode_bitmap gen_bitmap,
					  errcode_t magic, errcode_t neq,
					  ext2_ino_t अन्तः, ext2_ino_t *oend)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	EXT2_CHECK_MAGIC(bitmap, magic);

	यदि (अन्तः > bitmap->real_अन्तः)
		प्रत्यागम neq;
	यदि (oend)
		*oend = bitmap->अन्तः;
	bitmap->अन्तः = अन्तः;
	प्रत्यागम 0;
}

errcode_t ext2fs_resize_generic_bitmap(errcode_t magic,
				       __u32 new_अन्तः, __u32 new_real_अन्तः,
				       ext2fs_generic_bitmap gen_bmap)
{
	ext2fs_generic_bitmap_32 bmap = (ext2fs_generic_bitmap_32) gen_bmap;
	errcode_t	retval;
	size_t		आकार, new_आकार;
	__u32		bitno;

	यदि (!bmap || (bmap->magic != magic))
		प्रत्यागम magic;

	/*
	 * If we're expanding the bitmap, make sure all of the new
	 * parts of the bitmap are zero.
	 */
	यदि (new_अन्तः > bmap->अन्तः) {
		bitno = bmap->real_अन्तः;
		यदि (bitno > new_अन्तः)
			bitno = new_अन्तः;
		प्रति (; bitno > bmap->अन्तः; bitno--)
			ext2fs_clear_bit(bitno - bmap->start, bmap->bitmap);
	}
	यदि (new_real_अन्तः == bmap->real_अन्तः) {
		bmap->अन्तः = new_अन्तः;
		प्रत्यागम 0;
	}

	आकार = ((bmap->real_अन्तः - bmap->start) / 8) + 1;
	new_आकार = ((new_real_अन्तः - bmap->start) / 8) + 1;

	यदि (आकार != new_आकार) {
		retval = ext2fs_resize_mem(आकार, new_आकार, &bmap->bitmap);
		यदि (retval)
			प्रत्यागम retval;
	}
	यदि (new_आकार > आकार)
		स्मृतिं_पूरय(bmap->bitmap + आकार, 0, new_आकार - आकार);

	bmap->अन्तः = new_अन्तः;
	bmap->real_अन्तः = new_real_अन्तः;
	प्रत्यागम 0;
}

errcode_t ext2fs_compare_generic_bitmap(errcode_t magic, errcode_t neq,
					ext2fs_generic_bitmap gen_bm1,
					ext2fs_generic_bitmap gen_bm2)
{
	ext2fs_generic_bitmap_32 bm1 = (ext2fs_generic_bitmap_32) gen_bm1;
	ext2fs_generic_bitmap_32 bm2 = (ext2fs_generic_bitmap_32) gen_bm2;
	blk_t	i;

	यदि (!bm1 || bm1->magic != magic)
		प्रत्यागम magic;
	यदि (!bm2 || bm2->magic != magic)
		प्रत्यागम magic;

	यदि ((bm1->start != bm2->start) ||
	    (bm1->अन्तः != bm2->अन्तः) ||
	    (memcmp(bm1->bitmap, bm2->bitmap,
		    (size_t) (bm1->अन्तः - bm1->start)/8)))
		प्रत्यागम neq;

	प्रति (i = bm1->अन्तः - ((bm1->अन्तः - bm1->start) % 8); i <= bm1->अन्तः; i++)
		यदि (ext2fs_fast_test_खण्डम्_bitmap(gen_bm1, i) !=
		    ext2fs_fast_test_खण्डम्_bitmap(gen_bm2, i))
			प्रत्यागम neq;

	प्रत्यागम 0;
}

शून्य ext2fs_समुच्चयः_generic_bitmap_padding(ext2fs_generic_bitmap gen_map)
{
	ext2fs_generic_bitmap_32 map = (ext2fs_generic_bitmap_32) gen_map;
	__u32	i, j;

	/* Protect loop from wrap-around if map->real_end is maxed */
	प्रति (i=map->अन्तः+1, j = i - map->start;
	     i <= map->real_अन्तः && i > map->अन्तः;
	     i++, j++)
		ext2fs_समुच्चयः_bit(j, map->bitmap);
}

errcode_t ext2fs_get_generic_bitmap_range(ext2fs_generic_bitmap gen_bmap,
					  errcode_t magic,
					  __u32 start, __u32 अङ्कः,
					  शून्य *बहिर्गमनम्)
{
	ext2fs_generic_bitmap_32 bmap = (ext2fs_generic_bitmap_32) gen_bmap;

	यदि (!bmap || (bmap->magic != magic))
		प्रत्यागम magic;

	यदि ((start < bmap->start) || (start+अङ्कः-1 > bmap->real_अन्तः))
		प्रत्यागम EXT2_ET_INVALID_तर्कः;

	स्मृतिं_प्रतिलिख(बहिर्गमनम्, bmap->bitmap + ((start - bmap->start) >> 3), (अङ्कः+7) >> 3);
	प्रत्यागम 0;
}

errcode_t ext2fs_समुच्चयः_generic_bitmap_range(ext2fs_generic_bitmap gen_bmap,
					  errcode_t magic,
					  __u32 start, __u32 अङ्कः,
					  शून्य *अन्तर्)
{
	ext2fs_generic_bitmap_32 bmap = (ext2fs_generic_bitmap_32) gen_bmap;

	यदि (!bmap || (bmap->magic != magic))
		प्रत्यागम magic;

	यदि ((start < bmap->start) || (start+अङ्कः-1 > bmap->real_अन्तः))
		प्रत्यागम EXT2_ET_INVALID_तर्कः;

	स्मृतिं_प्रतिलिख(bmap->bitmap + ((start - bmap->start) >> 3), अन्तर्, (अङ्कः+7) >> 3);
	प्रत्यागम 0;
}

/*
 * Compare @mem to zero buffer by 256 bytes.
 * Return 1 if @mem is zeroed memory, otherwise return 0.
 */
पूर्णाङ्क ext2fs_mem_is_zero(ध्रुव वर्ण *mem, size_t len)
{
	स्थिर ध्रुव वर्ण zero_buf[256];

	यावत् (len >= परिमाण(zero_buf)) {
		यदि (memcmp(mem, zero_buf, परिमाण(zero_buf)))
			प्रत्यागम 0;
		len -= परिमाण(zero_buf);
		mem += परिमाण(zero_buf);
	}
	/* Deal with leftover bytes. */
	यदि (len)
		प्रत्यागम !memcmp(mem, zero_buf, len);
	प्रत्यागम 1;
}

/*
 * Return true if all of the bits in a specified range are clear
 */
स्थिर पूर्णाङ्क ext2fs_test_clear_generic_bitmap_range(ext2fs_generic_bitmap gen_bitmap,
						  अचिह्नित पूर्णाङ्क start,
						  अचिह्नित पूर्णाङ्क len)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	size_t start_byte, len_byte = len >> 3;
	अचिह्नित पूर्णाङ्क start_bit, len_bit = len % 8;
	पूर्णाङ्क first_bit = 0;
	पूर्णाङ्क चरमः_bit  = 0;
	पूर्णाङ्क mark_गणनम् = 0;
	पूर्णाङ्क mark_bit = 0;
	पूर्णाङ्क i;
	ध्रुव वर्ण *ADDR = bitmap->bitmap;

	start -= bitmap->start;
	start_byte = start >> 3;
	start_bit = start % 8;

	यदि (start_bit != 0) {
		/*
		 * The compared start block number or start inode number
		 * is not the first bit in a byte.
		 */
		mark_गणनम् = 8 - start_bit;
		यदि (len < 8 - start_bit) {
			mark_गणनम् = (पूर्णाङ्क)len;
			mark_bit = len + start_bit - 1;
		} अन्यथा
			mark_bit = 7;

		प्रति (i = mark_गणनम्; i > 0; i--, mark_bit--)
			first_bit |= 1 << mark_bit;

		/*
		 * Compare blocks or inodes in the first byte.
		 * If there is any marked bit, this function returns 0.
		 */
		यदि (first_bit & ADDR[start_byte])
			प्रत्यागम 0;
		अन्यथा यदि (len <= 8 - start_bit)
			प्रत्यागम 1;

		start_byte++;
		len_bit = (len - mark_गणनम्) % 8;
		len_byte = (len - mark_गणनम्) >> 3;
	}

	/*
	 * The compared start block number or start inode number is
	 * the first bit in a byte.
	 */
	यदि (len_bit != 0) {
		/*
		 * The compared end block number or end inode number is
		 * not the last bit in a byte.
		 */
		प्रति (mark_bit = len_bit - 1; mark_bit >= 0; mark_bit--)
			चरमः_bit |= 1 << mark_bit;

		/*
		 * Compare blocks or inodes in the last byte.
		 * If there is any marked bit, this function returns 0.
		 */
		यदि (चरमः_bit & ADDR[start_byte + len_byte])
			प्रत्यागम 0;
		अन्यथा यदि (len_byte == 0)
			प्रत्यागम 1;
	}

	/* Check whether all bytes are 0 */
	प्रत्यागम ext2fs_mem_is_zero(ADDR + start_byte, len_byte);
}

errcode_t ext2fs_find_first_zero_generic_bitmap(ext2fs_generic_bitmap gen_bitmap,
						__u32 start, __u32 अन्तः,
						__u32 *बहिर्गमनम्)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	blk_t b;

	यदि (start < bitmap->start || अन्तः > bitmap->अन्तः || start > अन्तः) {
		ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_TEST_ERROR, start);
		प्रत्यागम EINVAL;
	}

	यावत् (start <= अन्तः) {
		b = ext2fs_test_bit(start - bitmap->start, bitmap->bitmap);
		यदि (!b) {
			*बहिर्गमनम् = start;
			प्रत्यागम 0;
		}
		start++;
	}

	प्रत्यागम ENOENT;
}

errcode_t ext2fs_find_first_समुच्चयः_generic_bitmap(ext2fs_generic_bitmap gen_bitmap,
					       __u32 start, __u32 अन्तः,
					       __u32 *बहिर्गमनम्)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	blk_t b;

	यदि (start < bitmap->start || अन्तः > bitmap->अन्तः || start > अन्तः) {
		ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_TEST_ERROR, start);
		प्रत्यागम EINVAL;
	}

	यावत् (start <= अन्तः) {
		b = ext2fs_test_bit(start - bitmap->start, bitmap->bitmap);
		यदि (b) {
			*बहिर्गमनम् = start;
			प्रत्यागम 0;
		}
		start++;
	}

	प्रत्यागम ENOENT;
}

पूर्णाङ्क ext2fs_test_खण्डम्_bitmap_range(ext2fs_खण्डम्_bitmap gen_bitmap,
				   blk_t खण्डम्, पूर्णाङ्क अङ्कः)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	EXT2_CHECK_MAGIC(bitmap, EXT2_ET_MAGIC_खण्डम्_BITMAP);
	यदि ((खण्डम् < bitmap->start) || (खण्डम् > bitmap->real_अन्तः) ||
	    (खण्डम्+अङ्कः-1 > bitmap->real_अन्तः)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_खण्डम्_TEST,
				   खण्डम्, bitmap->description);
		प्रत्यागम 0;
	}
	प्रत्यागम ext2fs_test_clear_generic_bitmap_range((ext2fs_generic_bitmap)
						      bitmap, खण्डम्, अङ्कः);
}

पूर्णाङ्क ext2fs_test_inode_bitmap_range(ext2fs_inode_bitmap gen_bitmap,
				   ext2_ino_t inode, पूर्णाङ्क अङ्कः)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	EXT2_CHECK_MAGIC(bitmap, EXT2_ET_MAGIC_INODE_BITMAP);
	यदि ((inode < bitmap->start) || (inode > bitmap->real_अन्तः) ||
	    (inode+अङ्कः-1 > bitmap->real_अन्तः)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_INODE_TEST,
				   inode, bitmap->description);
		प्रत्यागम 0;
	}
	प्रत्यागम ext2fs_test_clear_generic_bitmap_range((ext2fs_generic_bitmap)
						      bitmap, inode, अङ्कः);
}

शून्य ext2fs_mark_खण्डम्_bitmap_range(ext2fs_खण्डम्_bitmap gen_bitmap,
				    blk_t खण्डम्, पूर्णाङ्क अङ्कः)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	पूर्णाङ्क	i;

	यदि ((खण्डम् < bitmap->start) || (खण्डम् > bitmap->अन्तः) ||
	    (खण्डम्+अङ्कः-1 > bitmap->अन्तः)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_खण्डम्_MARK, खण्डम्,
				   bitmap->description);
		प्रत्यागम;
	}
	प्रति (i=0; i < अङ्कः; i++)
		ext2fs_fast_समुच्चयः_bit(खण्डम् + i - bitmap->start, bitmap->bitmap);
}

शून्य ext2fs_unmark_खण्डम्_bitmap_range(ext2fs_खण्डम्_bitmap gen_bitmap,
				      blk_t खण्डम्, पूर्णाङ्क अङ्कः)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	पूर्णाङ्क	i;

	यदि ((खण्डम् < bitmap->start) || (खण्डम् > bitmap->अन्तः) ||
	    (खण्डम्+अङ्कः-1 > bitmap->अन्तः)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_खण्डम्_UNMARK, खण्डम्,
				   bitmap->description);
		प्रत्यागम;
	}
	प्रति (i=0; i < अङ्कः; i++)
		ext2fs_fast_clear_bit(खण्डम् + i - bitmap->start,
				      bitmap->bitmap);
}

