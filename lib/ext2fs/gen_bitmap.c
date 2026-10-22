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
#إذا HAVE_UNISTD_H
#ضمن <unistd.h>
#پایان_اگر
#ضمن <fcntl.h>
#ضمن <time.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#پایان_اگر
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#پایان_اگر

#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"

بنية ext2fs_struct_generic_bitmap_32 {
	errcode_t	magic;
	ext2_filsys 	fs;
	__u32		شروع, انتها;
	__u32		real_انتها;
	حرف	*	description;
	حرف	*	bitmap;
	errcode_t	پایه_error_کد;
	__u32		reserved[7];
};

تعريفنوع بنية ext2fs_struct_generic_bitmap_32 *ext2fs_generic_bitmap_32;

#تعریف_کن EXT2FS_IS_32_BITMAP(bmap) \
	(((bmap)->magic == EXT2_ET_MAGIC_GENERIC_BITMAP) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_بلوک_BITMAP) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_INODE_BITMAP))

#تعریف_کن EXT2FS_IS_64_BITMAP(bmap) \
	(((bmap)->magic == EXT2_ET_MAGIC_GENERIC_BITMAP64) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_بلوک_BITMAP64) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_INODE_BITMAP64))

/*
 * Used by previously inlined function, so we have to export this and
 * not change the function signature
 */
فراغ ext2fs_warn_bitmap2(ext2fs_generic_bitmap gen_bitmap,
			    صحيح کد, بیعلامت دراز arg)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

#اگر_تعریف_نشده OMIT_COM_ERR
	إذا (bitmap->description)
		com_err(0, bitmap->پایه_error_کد+کد,
			"#%lu for %s", arg, bitmap->description);
	وإلا
		com_err(0, bitmap->پایه_error_کد + کد, "#%lu", arg);
#پایان_اگر
}

ساكن errcode_t check_magic(ext2fs_generic_bitmap bitmap)
{
	إذا (!bitmap || !((bitmap->magic == EXT2_ET_MAGIC_GENERIC_BITMAP) ||
			 (bitmap->magic == EXT2_ET_MAGIC_INODE_BITMAP) ||
			 (bitmap->magic == EXT2_ET_MAGIC_بلوک_BITMAP)))
		أرجع EXT2_ET_MAGIC_GENERIC_BITMAP;
	أرجع 0;
}

errcode_t ext2fs_make_generic_bitmap(errcode_t magic, ext2_filsys fs,
				     __u32 شروع, __u32 انتها, __u32 real_انتها,
				     ثابت حرف *descr, حرف *init_map,
				     ext2fs_generic_bitmap *ret)
{
	ext2fs_generic_bitmap_32 bitmap;
	errcode_t		retval;
	size_t			اندازه;

	retval = ext2fs_get_mem(حجم(بنية ext2fs_struct_generic_bitmap_32),
				&bitmap);
	إذا (retval)
		أرجع retval;

	bitmap->magic = magic;
	bitmap->fs = fs;
	bitmap->شروع = شروع;
	bitmap->انتها = انتها;
	bitmap->real_انتها = real_انتها;
	بدل (magic) {
	حالة EXT2_ET_MAGIC_INODE_BITMAP:
		bitmap->پایه_error_کد = EXT2_ET_BAD_INODE_MARK;
		اكسر;
	حالة EXT2_ET_MAGIC_بلوک_BITMAP:
		bitmap->پایه_error_کد = EXT2_ET_BAD_بلوک_MARK;
		اكسر;
	افتراضي:
		bitmap->پایه_error_کد = EXT2_ET_BAD_GENERIC_MARK;
	}
	إذا (descr) {
		retval = ext2fs_get_mem(طول_رشته(descr)+1, &bitmap->description);
		إذا (retval) {
			ext2fs_free_mem(&bitmap);
			أرجع retval;
		}
		strcpy(bitmap->description, descr);
	} وإلا
		bitmap->description = 0;

	اندازه = (size_t) (((bitmap->real_انتها - bitmap->شروع) / 8) + 1);
	/* Round up to allow for the BT x86 instruction */
	اندازه = (اندازه + 7) & ~3;
	retval = ext2fs_get_mem(اندازه, &bitmap->bitmap);
	إذا (retval) {
		ext2fs_free_mem(&bitmap->description);
		ext2fs_free_mem(&bitmap);
		أرجع retval;
	}

	إذا (init_map)
		کپی_حافظه(bitmap->bitmap, init_map, اندازه);
	وإلا
		memset(bitmap->bitmap, 0, اندازه);
	*ret = (ext2fs_generic_bitmap) bitmap;
	أرجع 0;
}

errcode_t ext2fs_allocate_generic_bitmap(__u32 شروع,
					 __u32 انتها,
					 __u32 real_انتها,
					 ثابت حرف *descr,
					 ext2fs_generic_bitmap *ret)
{
	أرجع ext2fs_make_generic_bitmap(EXT2_ET_MAGIC_GENERIC_BITMAP, 0,
					  شروع, انتها, real_انتها, descr, 0, ret);
}

errcode_t ext2fs_copy_generic_bitmap(ext2fs_generic_bitmap gen_src,
				     ext2fs_generic_bitmap *dest)
{
	ext2fs_generic_bitmap_32 src = (ext2fs_generic_bitmap_32) gen_src;

	أرجع (ext2fs_make_generic_bitmap(src->magic, src->fs,
					   src->شروع, src->انتها,
					   src->real_انتها,
					   src->description, src->bitmap,
					   dest));
}

فراغ ext2fs_free_generic_bitmap(ext2fs_inode_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	إذا (check_magic(gen_bitmap))
		أرجع;

	bitmap->magic = 0;
	إذا (bitmap->description) {
		ext2fs_free_mem(&bitmap->description);
		bitmap->description = 0;
	}
	إذا (bitmap->bitmap) {
		ext2fs_free_mem(&bitmap->bitmap);
		bitmap->bitmap = 0;
	}
	ext2fs_free_mem(&bitmap);
}

صحيح ext2fs_test_generic_bitmap(ext2fs_generic_bitmap bitmap,
					blk_t bitno)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	إذا (!EXT2FS_IS_32_BITMAP(bitmap)) {
		إذا (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			أرجع ext2fs_test_generic_bmap(bitmap, bitno);
		}
#اگر_تعریف_نشده OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"test_bitmap(%lu)", (بیعلامت دراز) bitno);
#پایان_اگر
		أرجع 0;
	}

	إذا ((bitno < bitmap32->شروع) || (bitno > bitmap32->انتها)) {
		ext2fs_warn_bitmap2(bitmap, EXT2FS_TEST_ERROR, bitno);
		أرجع 0;
	}
	أرجع ext2fs_test_بیت(bitno - bitmap32->شروع, bitmap32->bitmap);
}

صحيح ext2fs_mark_generic_bitmap(ext2fs_generic_bitmap bitmap,
					 __u32 bitno)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	إذا (!EXT2FS_IS_32_BITMAP(bitmap)) {
		إذا (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			أرجع ext2fs_mark_generic_bmap(bitmap, bitno);
		}
#اگر_تعریف_نشده OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"mark_bitmap(%lu)", (بیعلامت دراز) bitno);
#پایان_اگر
		أرجع 0;
	}

	إذا ((bitno < bitmap32->شروع) || (bitno > bitmap32->انتها)) {
		ext2fs_warn_bitmap2(bitmap, EXT2FS_MARK_ERROR, bitno);
		أرجع 0;
	}
	أرجع ext2fs_مجموعه_بیت(bitno - bitmap32->شروع, bitmap32->bitmap);
}

صحيح ext2fs_unmark_generic_bitmap(ext2fs_generic_bitmap bitmap,
					   blk_t bitno)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	إذا (!EXT2FS_IS_32_BITMAP(bitmap)) {
		إذا (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			أرجع ext2fs_unmark_generic_bmap(bitmap, bitno);
		}
#اگر_تعریف_نشده OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"mark_bitmap(%lu)", (بیعلامت دراز) bitno);
#پایان_اگر
		أرجع 0;
	}

	إذا ((bitno < bitmap32->شروع) || (bitno > bitmap32->انتها)) {
		ext2fs_warn_bitmap2(bitmap, EXT2FS_UNMARK_ERROR, bitno);
		أرجع 0;
	}
	أرجع ext2fs_clear_بیت(bitno - bitmap32->شروع, bitmap32->bitmap);
}

__u32 ext2fs_get_generic_bitmap_شروع(ext2fs_generic_bitmap bitmap)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	إذا (!EXT2FS_IS_32_BITMAP(bitmap)) {
		إذا (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			أرجع ext2fs_get_generic_bmap_شروع(bitmap);
		}
#اگر_تعریف_نشده OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"get_bitmap_start");
#پایان_اگر
		أرجع 0;
	}

	أرجع bitmap32->شروع;
}

__u32 ext2fs_get_generic_bitmap_انتها(ext2fs_generic_bitmap bitmap)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	إذا (!EXT2FS_IS_32_BITMAP(bitmap)) {
		إذا (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			أرجع ext2fs_get_generic_bmap_انتها(bitmap);
		}
#اگر_تعریف_نشده OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"get_bitmap_end");
#پایان_اگر
		أرجع 0;
	}
	أرجع bitmap32->انتها;
}

فراغ ext2fs_clear_generic_bitmap(ext2fs_generic_bitmap bitmap)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	إذا (!EXT2FS_IS_32_BITMAP(bitmap)) {
		إذا (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			ext2fs_clear_generic_bmap(bitmap);
			أرجع;
		}
#اگر_تعریف_نشده OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"clear_generic_bitmap");
#پایان_اگر
		أرجع;
	}

	memset(bitmap32->bitmap, 0,
	       (size_t) (((bitmap32->real_انتها - bitmap32->شروع) / 8) + 1));
}

errcode_t ext2fs_fudge_generic_bitmap_انتها(ext2fs_inode_bitmap gen_bitmap,
					  errcode_t magic, errcode_t neq,
					  ext2_ino_t انتها, ext2_ino_t *oend)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	EXT2_CHECK_MAGIC(bitmap, magic);

	إذا (انتها > bitmap->real_انتها)
		أرجع neq;
	إذا (oend)
		*oend = bitmap->انتها;
	bitmap->انتها = انتها;
	أرجع 0;
}

errcode_t ext2fs_resize_generic_bitmap(errcode_t magic,
				       __u32 new_انتها, __u32 new_real_انتها,
				       ext2fs_generic_bitmap gen_bmap)
{
	ext2fs_generic_bitmap_32 bmap = (ext2fs_generic_bitmap_32) gen_bmap;
	errcode_t	retval;
	size_t		اندازه, new_اندازه;
	__u32		bitno;

	إذا (!bmap || (bmap->magic != magic))
		أرجع magic;

	/*
	 * If we're expanding the bitmap, make sure all of the new
	 * parts of the bitmap are zero.
	 */
	إذا (new_انتها > bmap->انتها) {
		bitno = bmap->real_انتها;
		إذا (bitno > new_انتها)
			bitno = new_انتها;
		لكل (; bitno > bmap->انتها; bitno--)
			ext2fs_clear_بیت(bitno - bmap->شروع, bmap->bitmap);
	}
	إذا (new_real_انتها == bmap->real_انتها) {
		bmap->انتها = new_انتها;
		أرجع 0;
	}

	اندازه = ((bmap->real_انتها - bmap->شروع) / 8) + 1;
	new_اندازه = ((new_real_انتها - bmap->شروع) / 8) + 1;

	إذا (اندازه != new_اندازه) {
		retval = ext2fs_resize_mem(اندازه, new_اندازه, &bmap->bitmap);
		إذا (retval)
			أرجع retval;
	}
	إذا (new_اندازه > اندازه)
		memset(bmap->bitmap + اندازه, 0, new_اندازه - اندازه);

	bmap->انتها = new_انتها;
	bmap->real_انتها = new_real_انتها;
	أرجع 0;
}

errcode_t ext2fs_compare_generic_bitmap(errcode_t magic, errcode_t neq,
					ext2fs_generic_bitmap gen_bm1,
					ext2fs_generic_bitmap gen_bm2)
{
	ext2fs_generic_bitmap_32 bm1 = (ext2fs_generic_bitmap_32) gen_bm1;
	ext2fs_generic_bitmap_32 bm2 = (ext2fs_generic_bitmap_32) gen_bm2;
	blk_t	i;

	إذا (!bm1 || bm1->magic != magic)
		أرجع magic;
	إذا (!bm2 || bm2->magic != magic)
		أرجع magic;

	إذا ((bm1->شروع != bm2->شروع) ||
	    (bm1->انتها != bm2->انتها) ||
	    (memcmp(bm1->bitmap, bm2->bitmap,
		    (size_t) (bm1->انتها - bm1->شروع)/8)))
		أرجع neq;

	لكل (i = bm1->انتها - ((bm1->انتها - bm1->شروع) % 8); i <= bm1->انتها; i++)
		إذا (ext2fs_fast_test_بلوک_bitmap(gen_bm1, i) !=
		    ext2fs_fast_test_بلوک_bitmap(gen_bm2, i))
			أرجع neq;

	أرجع 0;
}

فراغ ext2fs_مجموعه_generic_bitmap_padding(ext2fs_generic_bitmap gen_map)
{
	ext2fs_generic_bitmap_32 map = (ext2fs_generic_bitmap_32) gen_map;
	__u32	i, j;

	/* Protect loop from wrap-around if map->real_end is maxed */
	لكل (i=map->انتها+1, j = i - map->شروع;
	     i <= map->real_انتها && i > map->انتها;
	     i++, j++)
		ext2fs_مجموعه_بیت(j, map->bitmap);
}

errcode_t ext2fs_get_generic_bitmap_range(ext2fs_generic_bitmap gen_bmap,
					  errcode_t magic,
					  __u32 شروع, __u32 رقم,
					  فراغ *بیرون)
{
	ext2fs_generic_bitmap_32 bmap = (ext2fs_generic_bitmap_32) gen_bmap;

	إذا (!bmap || (bmap->magic != magic))
		أرجع magic;

	إذا ((شروع < bmap->شروع) || (شروع+رقم-1 > bmap->real_انتها))
		أرجع EXT2_ET_INVALID_آرگومان;

	کپی_حافظه(بیرون, bmap->bitmap + ((شروع - bmap->شروع) >> 3), (رقم+7) >> 3);
	أرجع 0;
}

errcode_t ext2fs_مجموعه_generic_bitmap_range(ext2fs_generic_bitmap gen_bmap,
					  errcode_t magic,
					  __u32 شروع, __u32 رقم,
					  فراغ *درون)
{
	ext2fs_generic_bitmap_32 bmap = (ext2fs_generic_bitmap_32) gen_bmap;

	إذا (!bmap || (bmap->magic != magic))
		أرجع magic;

	إذا ((شروع < bmap->شروع) || (شروع+رقم-1 > bmap->real_انتها))
		أرجع EXT2_ET_INVALID_آرگومان;

	کپی_حافظه(bmap->bitmap + ((شروع - bmap->شروع) >> 3), درون, (رقم+7) >> 3);
	أرجع 0;
}

/*
 * Compare @mem to zero buffer by 256 bytes.
 * Return 1 if @mem is zeroed memory, otherwise return 0.
 */
صحيح ext2fs_mem_is_zero(ثابت حرف *mem, size_t len)
{
	ساكن ثابت حرف zero_buf[256];

	طالما (len >= حجم(zero_buf)) {
		إذا (memcmp(mem, zero_buf, حجم(zero_buf)))
			أرجع 0;
		len -= حجم(zero_buf);
		mem += حجم(zero_buf);
	}
	/* Deal with leftover bytes. */
	إذا (len)
		أرجع !memcmp(mem, zero_buf, len);
	أرجع 1;
}

/*
 * Return true if all of the bits in a specified range are clear
 */
ساكن صحيح ext2fs_test_clear_generic_bitmap_range(ext2fs_generic_bitmap gen_bitmap,
						  بیعلامت صحيح شروع,
						  بیعلامت صحيح len)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	size_t شروع_byte, len_byte = len >> 3;
	بیعلامت صحيح شروع_بیت, len_بیت = len % 8;
	صحيح اولی_بیت = 0;
	صحيح آخرین_بیت  = 0;
	صحيح mark_شمار = 0;
	صحيح mark_بیت = 0;
	صحيح i;
	ثابت حرف *ADDR = bitmap->bitmap;

	شروع -= bitmap->شروع;
	شروع_byte = شروع >> 3;
	شروع_بیت = شروع % 8;

	إذا (شروع_بیت != 0) {
		/*
		 * The compared start block number or start inode number
		 * is not the first bit in a byte.
		 */
		mark_شمار = 8 - شروع_بیت;
		إذا (len < 8 - شروع_بیت) {
			mark_شمار = (صحيح)len;
			mark_بیت = len + شروع_بیت - 1;
		} وإلا
			mark_بیت = 7;

		لكل (i = mark_شمار; i > 0; i--, mark_بیت--)
			اولی_بیت |= 1 << mark_بیت;

		/*
		 * Compare blocks or inodes in the first byte.
		 * If there is any marked bit, this function returns 0.
		 */
		إذا (اولی_بیت & ADDR[شروع_byte])
			أرجع 0;
		وإلا إذا (len <= 8 - شروع_بیت)
			أرجع 1;

		شروع_byte++;
		len_بیت = (len - mark_شمار) % 8;
		len_byte = (len - mark_شمار) >> 3;
	}

	/*
	 * The compared start block number or start inode number is
	 * the first bit in a byte.
	 */
	إذا (len_بیت != 0) {
		/*
		 * The compared end block number or end inode number is
		 * not the last bit in a byte.
		 */
		لكل (mark_بیت = len_بیت - 1; mark_بیت >= 0; mark_بیت--)
			آخرین_بیت |= 1 << mark_بیت;

		/*
		 * Compare blocks or inodes in the last byte.
		 * If there is any marked bit, this function returns 0.
		 */
		إذا (آخرین_بیت & ADDR[شروع_byte + len_byte])
			أرجع 0;
		وإلا إذا (len_byte == 0)
			أرجع 1;
	}

	/* Check whether all bytes are 0 */
	أرجع ext2fs_mem_is_zero(ADDR + شروع_byte, len_byte);
}

errcode_t ext2fs_پیدا_کن_اولی_zero_generic_bitmap(ext2fs_generic_bitmap gen_bitmap,
						__u32 شروع, __u32 انتها,
						__u32 *بیرون)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	blk_t b;

	إذا (شروع < bitmap->شروع || انتها > bitmap->انتها || شروع > انتها) {
		ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_TEST_ERROR, شروع);
		أرجع EINVAL;
	}

	طالما (شروع <= انتها) {
		b = ext2fs_test_بیت(شروع - bitmap->شروع, bitmap->bitmap);
		إذا (!b) {
			*بیرون = شروع;
			أرجع 0;
		}
		شروع++;
	}

	أرجع ENOENT;
}

errcode_t ext2fs_پیدا_کن_اولی_مجموعه_generic_bitmap(ext2fs_generic_bitmap gen_bitmap,
					       __u32 شروع, __u32 انتها,
					       __u32 *بیرون)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	blk_t b;

	إذا (شروع < bitmap->شروع || انتها > bitmap->انتها || شروع > انتها) {
		ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_TEST_ERROR, شروع);
		أرجع EINVAL;
	}

	طالما (شروع <= انتها) {
		b = ext2fs_test_بیت(شروع - bitmap->شروع, bitmap->bitmap);
		إذا (b) {
			*بیرون = شروع;
			أرجع 0;
		}
		شروع++;
	}

	أرجع ENOENT;
}

صحيح ext2fs_test_بلوک_bitmap_range(ext2fs_بلوک_bitmap gen_bitmap,
				   blk_t بلوک, صحيح رقم)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	EXT2_CHECK_MAGIC(bitmap, EXT2_ET_MAGIC_بلوک_BITMAP);
	إذا ((بلوک < bitmap->شروع) || (بلوک > bitmap->real_انتها) ||
	    (بلوک+رقم-1 > bitmap->real_انتها)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_بلوک_TEST,
				   بلوک, bitmap->description);
		أرجع 0;
	}
	أرجع ext2fs_test_clear_generic_bitmap_range((ext2fs_generic_bitmap)
						      bitmap, بلوک, رقم);
}

صحيح ext2fs_test_inode_bitmap_range(ext2fs_inode_bitmap gen_bitmap,
				   ext2_ino_t inode, صحيح رقم)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	EXT2_CHECK_MAGIC(bitmap, EXT2_ET_MAGIC_INODE_BITMAP);
	إذا ((inode < bitmap->شروع) || (inode > bitmap->real_انتها) ||
	    (inode+رقم-1 > bitmap->real_انتها)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_INODE_TEST,
				   inode, bitmap->description);
		أرجع 0;
	}
	أرجع ext2fs_test_clear_generic_bitmap_range((ext2fs_generic_bitmap)
						      bitmap, inode, رقم);
}

فراغ ext2fs_mark_بلوک_bitmap_range(ext2fs_بلوک_bitmap gen_bitmap,
				    blk_t بلوک, صحيح رقم)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	صحيح	i;

	إذا ((بلوک < bitmap->شروع) || (بلوک > bitmap->انتها) ||
	    (بلوک+رقم-1 > bitmap->انتها)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_بلوک_MARK, بلوک,
				   bitmap->description);
		أرجع;
	}
	لكل (i=0; i < رقم; i++)
		ext2fs_fast_مجموعه_بیت(بلوک + i - bitmap->شروع, bitmap->bitmap);
}

فراغ ext2fs_unmark_بلوک_bitmap_range(ext2fs_بلوک_bitmap gen_bitmap,
				      blk_t بلوک, صحيح رقم)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	صحيح	i;

	إذا ((بلوک < bitmap->شروع) || (بلوک > bitmap->انتها) ||
	    (بلوک+رقم-1 > bitmap->انتها)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_بلوک_UNMARK, بلوک,
				   bitmap->description);
		أرجع;
	}
	لكل (i=0; i < رقم; i++)
		ext2fs_fast_clear_بیت(بلوک + i - bitmap->شروع,
				      bitmap->bitmap);
}

