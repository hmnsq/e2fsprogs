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


#شامل "config.h"
#شامل <stdio.h>
#شامل <string.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#اگر_ختم
#شامل <fcntl.h>
#شامل <time.h>
#اگر HAVE_SYS_STAT_H
#شامل <sys/stat.h>
#اگر_ختم
#اگر HAVE_SYS_TYPES_H
#شامل <sys/types.h>
#اگر_ختم

#شامل "ext2_fs.h"
#شامل "ext2fsP.h"

ساخت ext2fs_struct_generic_bitmap_32 {
	errcode_t	magic;
	ext2_filsys 	fs;
	__u32		آغاز, انتہا;
	__u32		real_انتہا;
	حرف	*	description;
	حرف	*	bitmap;
	errcode_t	بنیاد_error_رمز;
	__u32		reserved[7];
};

قسمتعریف ساخت ext2fs_struct_generic_bitmap_32 *ext2fs_generic_bitmap_32;

#تعریف_کرو EXT2FS_IS_32_BITMAP(bmap) \
	(((bmap)->magic == EXT2_ET_MAGIC_GENERIC_BITMAP) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_بلاک_BITMAP) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_INODE_BITMAP))

#تعریف_کرو EXT2FS_IS_64_BITMAP(bmap) \
	(((bmap)->magic == EXT2_ET_MAGIC_GENERIC_BITMAP64) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_بلاک_BITMAP64) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_INODE_BITMAP64))

/*
 * Used by previously inlined function, so we have to export this and
 * not change the function signature
 */
خالی ext2fs_warn_bitmap2(ext2fs_generic_bitmap gen_bitmap,
			    صحیح رمز, غیرعلامتی طویل arg)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

#اگر_غیر_تعریف_شدہ OMIT_COM_ERR
	اگر (bitmap->description)
		com_err(0, bitmap->بنیاد_error_رمز+رمز,
			"#%lu for %s", arg, bitmap->description);
	ورنہ
		com_err(0, bitmap->بنیاد_error_رمز + رمز, "#%lu", arg);
#اگر_ختم
}

ساکن errcode_t check_magic(ext2fs_generic_bitmap bitmap)
{
	اگر (!bitmap || !((bitmap->magic == EXT2_ET_MAGIC_GENERIC_BITMAP) ||
			 (bitmap->magic == EXT2_ET_MAGIC_INODE_BITMAP) ||
			 (bitmap->magic == EXT2_ET_MAGIC_بلاک_BITMAP)))
		واپس EXT2_ET_MAGIC_GENERIC_BITMAP;
	واپس 0;
}

errcode_t ext2fs_make_generic_bitmap(errcode_t magic, ext2_filsys fs,
				     __u32 آغاز, __u32 انتہا, __u32 real_انتہا,
				     ثابت حرف *descr, حرف *init_map,
				     ext2fs_generic_bitmap *ret)
{
	ext2fs_generic_bitmap_32 bitmap;
	errcode_t		retval;
	size_t			سائز;

	retval = ext2fs_get_mem(حجم(ساخت ext2fs_struct_generic_bitmap_32),
				&bitmap);
	اگر (retval)
		واپس retval;

	bitmap->magic = magic;
	bitmap->fs = fs;
	bitmap->آغاز = آغاز;
	bitmap->انتہا = انتہا;
	bitmap->real_انتہا = real_انتہا;
	انتخاب (magic) {
	حالت EXT2_ET_MAGIC_INODE_BITMAP:
		bitmap->بنیاد_error_رمز = EXT2_ET_BAD_INODE_MARK;
		توڑو;
	حالت EXT2_ET_MAGIC_بلاک_BITMAP:
		bitmap->بنیاد_error_رمز = EXT2_ET_BAD_بلاک_MARK;
		توڑو;
	طےشدہ:
		bitmap->بنیاد_error_رمز = EXT2_ET_BAD_GENERIC_MARK;
	}
	اگر (descr) {
		retval = ext2fs_get_mem(لڑی_لمبائی(descr)+1, &bitmap->description);
		اگر (retval) {
			ext2fs_free_mem(&bitmap);
			واپس retval;
		}
		strcpy(bitmap->description, descr);
	} ورنہ
		bitmap->description = 0;

	سائز = (size_t) (((bitmap->real_انتہا - bitmap->آغاز) / 8) + 1);
	/* Round up to allow for the BT x86 instruction */
	سائز = (سائز + 7) & ~3;
	retval = ext2fs_get_mem(سائز, &bitmap->bitmap);
	اگر (retval) {
		ext2fs_free_mem(&bitmap->description);
		ext2fs_free_mem(&bitmap);
		واپس retval;
	}

	اگر (init_map)
		حافظہ_نقل(bitmap->bitmap, init_map, سائز);
	ورنہ
		memset(bitmap->bitmap, 0, سائز);
	*ret = (ext2fs_generic_bitmap) bitmap;
	واپس 0;
}

errcode_t ext2fs_allocate_generic_bitmap(__u32 آغاز,
					 __u32 انتہا,
					 __u32 real_انتہا,
					 ثابت حرف *descr,
					 ext2fs_generic_bitmap *ret)
{
	واپس ext2fs_make_generic_bitmap(EXT2_ET_MAGIC_GENERIC_BITMAP, 0,
					  آغاز, انتہا, real_انتہا, descr, 0, ret);
}

errcode_t ext2fs_copy_generic_bitmap(ext2fs_generic_bitmap gen_src,
				     ext2fs_generic_bitmap *dest)
{
	ext2fs_generic_bitmap_32 src = (ext2fs_generic_bitmap_32) gen_src;

	واپس (ext2fs_make_generic_bitmap(src->magic, src->fs,
					   src->آغاز, src->انتہا,
					   src->real_انتہا,
					   src->description, src->bitmap,
					   dest));
}

خالی ext2fs_free_generic_bitmap(ext2fs_inode_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	اگر (check_magic(gen_bitmap))
		واپس;

	bitmap->magic = 0;
	اگر (bitmap->description) {
		ext2fs_free_mem(&bitmap->description);
		bitmap->description = 0;
	}
	اگر (bitmap->bitmap) {
		ext2fs_free_mem(&bitmap->bitmap);
		bitmap->bitmap = 0;
	}
	ext2fs_free_mem(&bitmap);
}

صحیح ext2fs_test_generic_bitmap(ext2fs_generic_bitmap bitmap,
					blk_t bitno)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	اگر (!EXT2FS_IS_32_BITMAP(bitmap)) {
		اگر (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			واپس ext2fs_test_generic_bmap(bitmap, bitno);
		}
#اگر_غیر_تعریف_شدہ OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"test_bitmap(%lu)", (غیرعلامتی طویل) bitno);
#اگر_ختم
		واپس 0;
	}

	اگر ((bitno < bitmap32->آغاز) || (bitno > bitmap32->انتہا)) {
		ext2fs_warn_bitmap2(bitmap, EXT2FS_TEST_ERROR, bitno);
		واپس 0;
	}
	واپس ext2fs_test_ثنائی_ہندسہ(bitno - bitmap32->آغاز, bitmap32->bitmap);
}

صحیح ext2fs_mark_generic_bitmap(ext2fs_generic_bitmap bitmap,
					 __u32 bitno)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	اگر (!EXT2FS_IS_32_BITMAP(bitmap)) {
		اگر (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			واپس ext2fs_mark_generic_bmap(bitmap, bitno);
		}
#اگر_غیر_تعریف_شدہ OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"mark_bitmap(%lu)", (غیرعلامتی طویل) bitno);
#اگر_ختم
		واپس 0;
	}

	اگر ((bitno < bitmap32->آغاز) || (bitno > bitmap32->انتہا)) {
		ext2fs_warn_bitmap2(bitmap, EXT2FS_MARK_ERROR, bitno);
		واپس 0;
	}
	واپس ext2fs_سیٹ_ثنائی_ہندسہ(bitno - bitmap32->آغاز, bitmap32->bitmap);
}

صحیح ext2fs_unmark_generic_bitmap(ext2fs_generic_bitmap bitmap,
					   blk_t bitno)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	اگر (!EXT2FS_IS_32_BITMAP(bitmap)) {
		اگر (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			واپس ext2fs_unmark_generic_bmap(bitmap, bitno);
		}
#اگر_غیر_تعریف_شدہ OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"mark_bitmap(%lu)", (غیرعلامتی طویل) bitno);
#اگر_ختم
		واپس 0;
	}

	اگر ((bitno < bitmap32->آغاز) || (bitno > bitmap32->انتہا)) {
		ext2fs_warn_bitmap2(bitmap, EXT2FS_UNMARK_ERROR, bitno);
		واپس 0;
	}
	واپس ext2fs_clear_ثنائی_ہندسہ(bitno - bitmap32->آغاز, bitmap32->bitmap);
}

__u32 ext2fs_get_generic_bitmap_آغاز(ext2fs_generic_bitmap bitmap)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	اگر (!EXT2FS_IS_32_BITMAP(bitmap)) {
		اگر (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			واپس ext2fs_get_generic_bmap_آغاز(bitmap);
		}
#اگر_غیر_تعریف_شدہ OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"get_bitmap_start");
#اگر_ختم
		واپس 0;
	}

	واپس bitmap32->آغاز;
}

__u32 ext2fs_get_generic_bitmap_انتہا(ext2fs_generic_bitmap bitmap)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	اگر (!EXT2FS_IS_32_BITMAP(bitmap)) {
		اگر (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			واپس ext2fs_get_generic_bmap_انتہا(bitmap);
		}
#اگر_غیر_تعریف_شدہ OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"get_bitmap_end");
#اگر_ختم
		واپس 0;
	}
	واپس bitmap32->انتہا;
}

خالی ext2fs_clear_generic_bitmap(ext2fs_generic_bitmap bitmap)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	اگر (!EXT2FS_IS_32_BITMAP(bitmap)) {
		اگر (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			ext2fs_clear_generic_bmap(bitmap);
			واپس;
		}
#اگر_غیر_تعریف_شدہ OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"clear_generic_bitmap");
#اگر_ختم
		واپس;
	}

	memset(bitmap32->bitmap, 0,
	       (size_t) (((bitmap32->real_انتہا - bitmap32->آغاز) / 8) + 1));
}

errcode_t ext2fs_fudge_generic_bitmap_انتہا(ext2fs_inode_bitmap gen_bitmap,
					  errcode_t magic, errcode_t neq,
					  ext2_ino_t انتہا, ext2_ino_t *oend)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	EXT2_CHECK_MAGIC(bitmap, magic);

	اگر (انتہا > bitmap->real_انتہا)
		واپس neq;
	اگر (oend)
		*oend = bitmap->انتہا;
	bitmap->انتہا = انتہا;
	واپس 0;
}

errcode_t ext2fs_resize_generic_bitmap(errcode_t magic,
				       __u32 new_انتہا, __u32 new_real_انتہا,
				       ext2fs_generic_bitmap gen_bmap)
{
	ext2fs_generic_bitmap_32 bmap = (ext2fs_generic_bitmap_32) gen_bmap;
	errcode_t	retval;
	size_t		سائز, new_سائز;
	__u32		bitno;

	اگر (!bmap || (bmap->magic != magic))
		واپس magic;

	/*
	 * If we're expanding the bitmap, make sure all of the new
	 * parts of the bitmap are zero.
	 */
	اگر (new_انتہا > bmap->انتہا) {
		bitno = bmap->real_انتہا;
		اگر (bitno > new_انتہا)
			bitno = new_انتہا;
		برائے (; bitno > bmap->انتہا; bitno--)
			ext2fs_clear_ثنائی_ہندسہ(bitno - bmap->آغاز, bmap->bitmap);
	}
	اگر (new_real_انتہا == bmap->real_انتہا) {
		bmap->انتہا = new_انتہا;
		واپس 0;
	}

	سائز = ((bmap->real_انتہا - bmap->آغاز) / 8) + 1;
	new_سائز = ((new_real_انتہا - bmap->آغاز) / 8) + 1;

	اگر (سائز != new_سائز) {
		retval = ext2fs_resize_mem(سائز, new_سائز, &bmap->bitmap);
		اگر (retval)
			واپس retval;
	}
	اگر (new_سائز > سائز)
		memset(bmap->bitmap + سائز, 0, new_سائز - سائز);

	bmap->انتہا = new_انتہا;
	bmap->real_انتہا = new_real_انتہا;
	واپس 0;
}

errcode_t ext2fs_compare_generic_bitmap(errcode_t magic, errcode_t neq,
					ext2fs_generic_bitmap gen_bm1,
					ext2fs_generic_bitmap gen_bm2)
{
	ext2fs_generic_bitmap_32 bm1 = (ext2fs_generic_bitmap_32) gen_bm1;
	ext2fs_generic_bitmap_32 bm2 = (ext2fs_generic_bitmap_32) gen_bm2;
	blk_t	i;

	اگر (!bm1 || bm1->magic != magic)
		واپس magic;
	اگر (!bm2 || bm2->magic != magic)
		واپس magic;

	اگر ((bm1->آغاز != bm2->آغاز) ||
	    (bm1->انتہا != bm2->انتہا) ||
	    (memcmp(bm1->bitmap, bm2->bitmap,
		    (size_t) (bm1->انتہا - bm1->آغاز)/8)))
		واپس neq;

	برائے (i = bm1->انتہا - ((bm1->انتہا - bm1->آغاز) % 8); i <= bm1->انتہا; i++)
		اگر (ext2fs_fast_test_بلاک_bitmap(gen_bm1, i) !=
		    ext2fs_fast_test_بلاک_bitmap(gen_bm2, i))
			واپس neq;

	واپس 0;
}

خالی ext2fs_سیٹ_generic_bitmap_padding(ext2fs_generic_bitmap gen_map)
{
	ext2fs_generic_bitmap_32 map = (ext2fs_generic_bitmap_32) gen_map;
	__u32	i, j;

	/* Protect loop from wrap-around if map->real_end is maxed */
	برائے (i=map->انتہا+1, j = i - map->آغاز;
	     i <= map->real_انتہا && i > map->انتہا;
	     i++, j++)
		ext2fs_سیٹ_ثنائی_ہندسہ(j, map->bitmap);
}

errcode_t ext2fs_get_generic_bitmap_range(ext2fs_generic_bitmap gen_bmap,
					  errcode_t magic,
					  __u32 آغاز, __u32 عدد,
					  خالی *برآمد)
{
	ext2fs_generic_bitmap_32 bmap = (ext2fs_generic_bitmap_32) gen_bmap;

	اگر (!bmap || (bmap->magic != magic))
		واپس magic;

	اگر ((آغاز < bmap->آغاز) || (آغاز+عدد-1 > bmap->real_انتہا))
		واپس EXT2_ET_INVALID_دلیل;

	حافظہ_نقل(برآمد, bmap->bitmap + ((آغاز - bmap->آغاز) >> 3), (عدد+7) >> 3);
	واپس 0;
}

errcode_t ext2fs_سیٹ_generic_bitmap_range(ext2fs_generic_bitmap gen_bmap,
					  errcode_t magic,
					  __u32 آغاز, __u32 عدد,
					  خالی *اندر)
{
	ext2fs_generic_bitmap_32 bmap = (ext2fs_generic_bitmap_32) gen_bmap;

	اگر (!bmap || (bmap->magic != magic))
		واپس magic;

	اگر ((آغاز < bmap->آغاز) || (آغاز+عدد-1 > bmap->real_انتہا))
		واپس EXT2_ET_INVALID_دلیل;

	حافظہ_نقل(bmap->bitmap + ((آغاز - bmap->آغاز) >> 3), اندر, (عدد+7) >> 3);
	واپس 0;
}

/*
 * Compare @mem to zero buffer by 256 bytes.
 * Return 1 if @mem is zeroed memory, otherwise return 0.
 */
صحیح ext2fs_mem_is_zero(ثابت حرف *mem, size_t len)
{
	ساکن ثابت حرف zero_buf[256];

	جب (len >= حجم(zero_buf)) {
		اگر (memcmp(mem, zero_buf, حجم(zero_buf)))
			واپس 0;
		len -= حجم(zero_buf);
		mem += حجم(zero_buf);
	}
	/* Deal with leftover bytes. */
	اگر (len)
		واپس !memcmp(mem, zero_buf, len);
	واپس 1;
}

/*
 * Return true if all of the bits in a specified range are clear
 */
ساکن صحیح ext2fs_test_clear_generic_bitmap_range(ext2fs_generic_bitmap gen_bitmap,
						  غیرعلامتی صحیح آغاز,
						  غیرعلامتی صحیح len)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	size_t آغاز_byte, len_byte = len >> 3;
	غیرعلامتی صحیح آغاز_ثنائی_ہندسہ, len_ثنائی_ہندسہ = len % 8;
	صحیح پہلا_ثنائی_ہندسہ = 0;
	صحیح آخری_ثنائی_ہندسہ  = 0;
	صحیح mark_گنتی = 0;
	صحیح mark_ثنائی_ہندسہ = 0;
	صحیح i;
	ثابت حرف *ADDR = bitmap->bitmap;

	آغاز -= bitmap->آغاز;
	آغاز_byte = آغاز >> 3;
	آغاز_ثنائی_ہندسہ = آغاز % 8;

	اگر (آغاز_ثنائی_ہندسہ != 0) {
		/*
		 * The compared start block number or start inode number
		 * is not the first bit in a byte.
		 */
		mark_گنتی = 8 - آغاز_ثنائی_ہندسہ;
		اگر (len < 8 - آغاز_ثنائی_ہندسہ) {
			mark_گنتی = (صحیح)len;
			mark_ثنائی_ہندسہ = len + آغاز_ثنائی_ہندسہ - 1;
		} ورنہ
			mark_ثنائی_ہندسہ = 7;

		برائے (i = mark_گنتی; i > 0; i--, mark_ثنائی_ہندسہ--)
			پہلا_ثنائی_ہندسہ |= 1 << mark_ثنائی_ہندسہ;

		/*
		 * Compare blocks or inodes in the first byte.
		 * If there is any marked bit, this function returns 0.
		 */
		اگر (پہلا_ثنائی_ہندسہ & ADDR[آغاز_byte])
			واپس 0;
		ورنہ اگر (len <= 8 - آغاز_ثنائی_ہندسہ)
			واپس 1;

		آغاز_byte++;
		len_ثنائی_ہندسہ = (len - mark_گنتی) % 8;
		len_byte = (len - mark_گنتی) >> 3;
	}

	/*
	 * The compared start block number or start inode number is
	 * the first bit in a byte.
	 */
	اگر (len_ثنائی_ہندسہ != 0) {
		/*
		 * The compared end block number or end inode number is
		 * not the last bit in a byte.
		 */
		برائے (mark_ثنائی_ہندسہ = len_ثنائی_ہندسہ - 1; mark_ثنائی_ہندسہ >= 0; mark_ثنائی_ہندسہ--)
			آخری_ثنائی_ہندسہ |= 1 << mark_ثنائی_ہندسہ;

		/*
		 * Compare blocks or inodes in the last byte.
		 * If there is any marked bit, this function returns 0.
		 */
		اگر (آخری_ثنائی_ہندسہ & ADDR[آغاز_byte + len_byte])
			واپس 0;
		ورنہ اگر (len_byte == 0)
			واپس 1;
	}

	/* Check whether all bytes are 0 */
	واپس ext2fs_mem_is_zero(ADDR + آغاز_byte, len_byte);
}

errcode_t ext2fs_ڈھونڈو_پہلا_zero_generic_bitmap(ext2fs_generic_bitmap gen_bitmap,
						__u32 آغاز, __u32 انتہا,
						__u32 *برآمد)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	blk_t b;

	اگر (آغاز < bitmap->آغاز || انتہا > bitmap->انتہا || آغاز > انتہا) {
		ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_TEST_ERROR, آغاز);
		واپس EINVAL;
	}

	جب (آغاز <= انتہا) {
		b = ext2fs_test_ثنائی_ہندسہ(آغاز - bitmap->آغاز, bitmap->bitmap);
		اگر (!b) {
			*برآمد = آغاز;
			واپس 0;
		}
		آغاز++;
	}

	واپس ENOENT;
}

errcode_t ext2fs_ڈھونڈو_پہلا_سیٹ_generic_bitmap(ext2fs_generic_bitmap gen_bitmap,
					       __u32 آغاز, __u32 انتہا,
					       __u32 *برآمد)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	blk_t b;

	اگر (آغاز < bitmap->آغاز || انتہا > bitmap->انتہا || آغاز > انتہا) {
		ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_TEST_ERROR, آغاز);
		واپس EINVAL;
	}

	جب (آغاز <= انتہا) {
		b = ext2fs_test_ثنائی_ہندسہ(آغاز - bitmap->آغاز, bitmap->bitmap);
		اگر (b) {
			*برآمد = آغاز;
			واپس 0;
		}
		آغاز++;
	}

	واپس ENOENT;
}

صحیح ext2fs_test_بلاک_bitmap_range(ext2fs_بلاک_bitmap gen_bitmap,
				   blk_t بلاک, صحیح عدد)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	EXT2_CHECK_MAGIC(bitmap, EXT2_ET_MAGIC_بلاک_BITMAP);
	اگر ((بلاک < bitmap->آغاز) || (بلاک > bitmap->real_انتہا) ||
	    (بلاک+عدد-1 > bitmap->real_انتہا)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_بلاک_TEST,
				   بلاک, bitmap->description);
		واپس 0;
	}
	واپس ext2fs_test_clear_generic_bitmap_range((ext2fs_generic_bitmap)
						      bitmap, بلاک, عدد);
}

صحیح ext2fs_test_inode_bitmap_range(ext2fs_inode_bitmap gen_bitmap,
				   ext2_ino_t inode, صحیح عدد)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	EXT2_CHECK_MAGIC(bitmap, EXT2_ET_MAGIC_INODE_BITMAP);
	اگر ((inode < bitmap->آغاز) || (inode > bitmap->real_انتہا) ||
	    (inode+عدد-1 > bitmap->real_انتہا)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_INODE_TEST,
				   inode, bitmap->description);
		واپس 0;
	}
	واپس ext2fs_test_clear_generic_bitmap_range((ext2fs_generic_bitmap)
						      bitmap, inode, عدد);
}

خالی ext2fs_mark_بلاک_bitmap_range(ext2fs_بلاک_bitmap gen_bitmap,
				    blk_t بلاک, صحیح عدد)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	صحیح	i;

	اگر ((بلاک < bitmap->آغاز) || (بلاک > bitmap->انتہا) ||
	    (بلاک+عدد-1 > bitmap->انتہا)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_بلاک_MARK, بلاک,
				   bitmap->description);
		واپس;
	}
	برائے (i=0; i < عدد; i++)
		ext2fs_fast_سیٹ_ثنائی_ہندسہ(بلاک + i - bitmap->آغاز, bitmap->bitmap);
}

خالی ext2fs_unmark_بلاک_bitmap_range(ext2fs_بلاک_bitmap gen_bitmap,
				      blk_t بلاک, صحیح عدد)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	صحیح	i;

	اگر ((بلاک < bitmap->آغاز) || (بلاک > bitmap->انتہا) ||
	    (بلاک+عدد-1 > bitmap->انتہا)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_بلاک_UNMARK, بلاک,
				   bitmap->description);
		واپس;
	}
	برائے (i=0; i < عدد; i++)
		ext2fs_fast_clear_ثنائی_ہندسہ(بلاک + i - bitmap->آغاز,
				      bitmap->bitmap);
}

