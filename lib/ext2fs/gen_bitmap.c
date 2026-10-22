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
#جے_انت
#ضمن <fcntl.h>
#ضمن <time.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#جے_انت
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#جے_انت

#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"

بنية ext2fs_struct_generic_bitmap_32 {
	errcode_t	magic;
	ext2_filsys 	fs;
	__u32		شروع, سرا;
	__u32		real_سرا;
	حرف	*	description;
	حرف	*	bitmap;
	errcode_t	ادھار_error_کوڈ;
	__u32		reserved[7];
};

تعريفنوع بنية ext2fs_struct_generic_bitmap_32 *ext2fs_generic_bitmap_32;

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
فراغ ext2fs_warn_bitmap2(ext2fs_generic_bitmap gen_bitmap,
			    صحيح کوڈ, بنانشانی لما arg)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

#جے_بنا_تعریف OMIT_COM_ERR
	إذا (bitmap->description)
		com_err(0, bitmap->ادھار_error_کوڈ+کوڈ,
			"#%lu for %s", arg, bitmap->description);
	وإلا
		com_err(0, bitmap->ادھار_error_کوڈ + کوڈ, "#%lu", arg);
#جے_انت
}

ساكن errcode_t check_magic(ext2fs_generic_bitmap bitmap)
{
	إذا (!bitmap || !((bitmap->magic == EXT2_ET_MAGIC_GENERIC_BITMAP) ||
			 (bitmap->magic == EXT2_ET_MAGIC_INODE_BITMAP) ||
			 (bitmap->magic == EXT2_ET_MAGIC_بلاک_BITMAP)))
		أرجع EXT2_ET_MAGIC_GENERIC_BITMAP;
	أرجع 0;
}

errcode_t ext2fs_make_generic_bitmap(errcode_t magic, ext2_filsys fs,
				     __u32 شروع, __u32 سرا, __u32 real_سرا,
				     ثابت حرف *descr, حرف *init_map,
				     ext2fs_generic_bitmap *ret)
{
	ext2fs_generic_bitmap_32 bitmap;
	errcode_t		retval;
	size_t			ناپ;

	retval = ext2fs_get_mem(حجم(بنية ext2fs_struct_generic_bitmap_32),
				&bitmap);
	إذا (retval)
		أرجع retval;

	bitmap->magic = magic;
	bitmap->fs = fs;
	bitmap->شروع = شروع;
	bitmap->سرا = سرا;
	bitmap->real_سرا = real_سرا;
	بدل (magic) {
	حالة EXT2_ET_MAGIC_INODE_BITMAP:
		bitmap->ادھار_error_کوڈ = EXT2_ET_BAD_INODE_MARK;
		اكسر;
	حالة EXT2_ET_MAGIC_بلاک_BITMAP:
		bitmap->ادھار_error_کوڈ = EXT2_ET_BAD_بلاک_MARK;
		اكسر;
	افتراضي:
		bitmap->ادھار_error_کوڈ = EXT2_ET_BAD_GENERIC_MARK;
	}
	إذا (descr) {
		retval = ext2fs_get_mem(لڑی_لمبائی(descr)+1, &bitmap->description);
		إذا (retval) {
			ext2fs_free_mem(&bitmap);
			أرجع retval;
		}
		strcpy(bitmap->description, descr);
	} وإلا
		bitmap->description = 0;

	ناپ = (size_t) (((bitmap->real_سرا - bitmap->شروع) / 8) + 1);
	/* Round up to allow for the BT x86 instruction */
	ناپ = (ناپ + 7) & ~3;
	retval = ext2fs_get_mem(ناپ, &bitmap->bitmap);
	إذا (retval) {
		ext2fs_free_mem(&bitmap->description);
		ext2fs_free_mem(&bitmap);
		أرجع retval;
	}

	إذا (init_map)
		حافظہ_نقل(bitmap->bitmap, init_map, ناپ);
	وإلا
		memset(bitmap->bitmap, 0, ناپ);
	*ret = (ext2fs_generic_bitmap) bitmap;
	أرجع 0;
}

errcode_t ext2fs_allocate_generic_bitmap(__u32 شروع,
					 __u32 سرا,
					 __u32 real_سرا,
					 ثابت حرف *descr,
					 ext2fs_generic_bitmap *ret)
{
	أرجع ext2fs_make_generic_bitmap(EXT2_ET_MAGIC_GENERIC_BITMAP, 0,
					  شروع, سرا, real_سرا, descr, 0, ret);
}

errcode_t ext2fs_copy_generic_bitmap(ext2fs_generic_bitmap gen_src,
				     ext2fs_generic_bitmap *dest)
{
	ext2fs_generic_bitmap_32 src = (ext2fs_generic_bitmap_32) gen_src;

	أرجع (ext2fs_make_generic_bitmap(src->magic, src->fs,
					   src->شروع, src->سرا,
					   src->real_سرا,
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
#جے_بنا_تعریف OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"test_bitmap(%lu)", (بنانشانی لما) bitno);
#جے_انت
		أرجع 0;
	}

	إذا ((bitno < bitmap32->شروع) || (bitno > bitmap32->سرا)) {
		ext2fs_warn_bitmap2(bitmap, EXT2FS_TEST_ERROR, bitno);
		أرجع 0;
	}
	أرجع ext2fs_test_ثنائی_ہندسہ(bitno - bitmap32->شروع, bitmap32->bitmap);
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
#جے_بنا_تعریف OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"mark_bitmap(%lu)", (بنانشانی لما) bitno);
#جے_انت
		أرجع 0;
	}

	إذا ((bitno < bitmap32->شروع) || (bitno > bitmap32->سرا)) {
		ext2fs_warn_bitmap2(bitmap, EXT2FS_MARK_ERROR, bitno);
		أرجع 0;
	}
	أرجع ext2fs_سیٹ_ثنائی_ہندسہ(bitno - bitmap32->شروع, bitmap32->bitmap);
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
#جے_بنا_تعریف OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"mark_bitmap(%lu)", (بنانشانی لما) bitno);
#جے_انت
		أرجع 0;
	}

	إذا ((bitno < bitmap32->شروع) || (bitno > bitmap32->سرا)) {
		ext2fs_warn_bitmap2(bitmap, EXT2FS_UNMARK_ERROR, bitno);
		أرجع 0;
	}
	أرجع ext2fs_clear_ثنائی_ہندسہ(bitno - bitmap32->شروع, bitmap32->bitmap);
}

__u32 ext2fs_get_generic_bitmap_شروع(ext2fs_generic_bitmap bitmap)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	إذا (!EXT2FS_IS_32_BITMAP(bitmap)) {
		إذا (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			أرجع ext2fs_get_generic_bmap_شروع(bitmap);
		}
#جے_بنا_تعریف OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"get_bitmap_start");
#جے_انت
		أرجع 0;
	}

	أرجع bitmap32->شروع;
}

__u32 ext2fs_get_generic_bitmap_سرا(ext2fs_generic_bitmap bitmap)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	إذا (!EXT2FS_IS_32_BITMAP(bitmap)) {
		إذا (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			أرجع ext2fs_get_generic_bmap_سرا(bitmap);
		}
#جے_بنا_تعریف OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"get_bitmap_end");
#جے_انت
		أرجع 0;
	}
	أرجع bitmap32->سرا;
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
#جے_بنا_تعریف OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"clear_generic_bitmap");
#جے_انت
		أرجع;
	}

	memset(bitmap32->bitmap, 0,
	       (size_t) (((bitmap32->real_سرا - bitmap32->شروع) / 8) + 1));
}

errcode_t ext2fs_fudge_generic_bitmap_سرا(ext2fs_inode_bitmap gen_bitmap,
					  errcode_t magic, errcode_t neq,
					  ext2_ino_t سرا, ext2_ino_t *oend)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	EXT2_CHECK_MAGIC(bitmap, magic);

	إذا (سرا > bitmap->real_سرا)
		أرجع neq;
	إذا (oend)
		*oend = bitmap->سرا;
	bitmap->سرا = سرا;
	أرجع 0;
}

errcode_t ext2fs_resize_generic_bitmap(errcode_t magic,
				       __u32 new_سرا, __u32 new_real_سرا,
				       ext2fs_generic_bitmap gen_bmap)
{
	ext2fs_generic_bitmap_32 bmap = (ext2fs_generic_bitmap_32) gen_bmap;
	errcode_t	retval;
	size_t		ناپ, new_ناپ;
	__u32		bitno;

	إذا (!bmap || (bmap->magic != magic))
		أرجع magic;

	/*
	 * If we're expanding the bitmap, make sure all of the new
	 * parts of the bitmap are zero.
	 */
	إذا (new_سرا > bmap->سرا) {
		bitno = bmap->real_سرا;
		إذا (bitno > new_سرا)
			bitno = new_سرا;
		لكل (; bitno > bmap->سرا; bitno--)
			ext2fs_clear_ثنائی_ہندسہ(bitno - bmap->شروع, bmap->bitmap);
	}
	إذا (new_real_سرا == bmap->real_سرا) {
		bmap->سرا = new_سرا;
		أرجع 0;
	}

	ناپ = ((bmap->real_سرا - bmap->شروع) / 8) + 1;
	new_ناپ = ((new_real_سرا - bmap->شروع) / 8) + 1;

	إذا (ناپ != new_ناپ) {
		retval = ext2fs_resize_mem(ناپ, new_ناپ, &bmap->bitmap);
		إذا (retval)
			أرجع retval;
	}
	إذا (new_ناپ > ناپ)
		memset(bmap->bitmap + ناپ, 0, new_ناپ - ناپ);

	bmap->سرا = new_سرا;
	bmap->real_سرا = new_real_سرا;
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
	    (bm1->سرا != bm2->سرا) ||
	    (memcmp(bm1->bitmap, bm2->bitmap,
		    (size_t) (bm1->سرا - bm1->شروع)/8)))
		أرجع neq;

	لكل (i = bm1->سرا - ((bm1->سرا - bm1->شروع) % 8); i <= bm1->سرا; i++)
		إذا (ext2fs_fast_test_بلاک_bitmap(gen_bm1, i) !=
		    ext2fs_fast_test_بلاک_bitmap(gen_bm2, i))
			أرجع neq;

	أرجع 0;
}

فراغ ext2fs_سیٹ_generic_bitmap_padding(ext2fs_generic_bitmap gen_map)
{
	ext2fs_generic_bitmap_32 map = (ext2fs_generic_bitmap_32) gen_map;
	__u32	i, j;

	/* Protect loop from wrap-around if map->real_end is maxed */
	لكل (i=map->سرا+1, j = i - map->شروع;
	     i <= map->real_سرا && i > map->سرا;
	     i++, j++)
		ext2fs_سیٹ_ثنائی_ہندسہ(j, map->bitmap);
}

errcode_t ext2fs_get_generic_bitmap_range(ext2fs_generic_bitmap gen_bmap,
					  errcode_t magic,
					  __u32 شروع, __u32 عدد,
					  فراغ *باہرلا)
{
	ext2fs_generic_bitmap_32 bmap = (ext2fs_generic_bitmap_32) gen_bmap;

	إذا (!bmap || (bmap->magic != magic))
		أرجع magic;

	إذا ((شروع < bmap->شروع) || (شروع+عدد-1 > bmap->real_سرا))
		أرجع EXT2_ET_INVALID_دلیل;

	حافظہ_نقل(باہرلا, bmap->bitmap + ((شروع - bmap->شروع) >> 3), (عدد+7) >> 3);
	أرجع 0;
}

errcode_t ext2fs_سیٹ_generic_bitmap_range(ext2fs_generic_bitmap gen_bmap,
					  errcode_t magic,
					  __u32 شروع, __u32 عدد,
					  فراغ *وچ)
{
	ext2fs_generic_bitmap_32 bmap = (ext2fs_generic_bitmap_32) gen_bmap;

	إذا (!bmap || (bmap->magic != magic))
		أرجع magic;

	إذا ((شروع < bmap->شروع) || (شروع+عدد-1 > bmap->real_سرا))
		أرجع EXT2_ET_INVALID_دلیل;

	حافظہ_نقل(bmap->bitmap + ((شروع - bmap->شروع) >> 3), وچ, (عدد+7) >> 3);
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
						  بنانشانی صحيح شروع,
						  بنانشانی صحيح len)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	size_t شروع_byte, len_byte = len >> 3;
	بنانشانی صحيح شروع_ثنائی_ہندسہ, len_ثنائی_ہندسہ = len % 8;
	صحيح پہلا_ثنائی_ہندسہ = 0;
	صحيح آخری_ثنائی_ہندسہ  = 0;
	صحيح mark_گنتی = 0;
	صحيح mark_ثنائی_ہندسہ = 0;
	صحيح i;
	ثابت حرف *ADDR = bitmap->bitmap;

	شروع -= bitmap->شروع;
	شروع_byte = شروع >> 3;
	شروع_ثنائی_ہندسہ = شروع % 8;

	إذا (شروع_ثنائی_ہندسہ != 0) {
		/*
		 * The compared start block number or start inode number
		 * is not the first bit in a byte.
		 */
		mark_گنتی = 8 - شروع_ثنائی_ہندسہ;
		إذا (len < 8 - شروع_ثنائی_ہندسہ) {
			mark_گنتی = (صحيح)len;
			mark_ثنائی_ہندسہ = len + شروع_ثنائی_ہندسہ - 1;
		} وإلا
			mark_ثنائی_ہندسہ = 7;

		لكل (i = mark_گنتی; i > 0; i--, mark_ثنائی_ہندسہ--)
			پہلا_ثنائی_ہندسہ |= 1 << mark_ثنائی_ہندسہ;

		/*
		 * Compare blocks or inodes in the first byte.
		 * If there is any marked bit, this function returns 0.
		 */
		إذا (پہلا_ثنائی_ہندسہ & ADDR[شروع_byte])
			أرجع 0;
		وإلا إذا (len <= 8 - شروع_ثنائی_ہندسہ)
			أرجع 1;

		شروع_byte++;
		len_ثنائی_ہندسہ = (len - mark_گنتی) % 8;
		len_byte = (len - mark_گنتی) >> 3;
	}

	/*
	 * The compared start block number or start inode number is
	 * the first bit in a byte.
	 */
	إذا (len_ثنائی_ہندسہ != 0) {
		/*
		 * The compared end block number or end inode number is
		 * not the last bit in a byte.
		 */
		لكل (mark_ثنائی_ہندسہ = len_ثنائی_ہندسہ - 1; mark_ثنائی_ہندسہ >= 0; mark_ثنائی_ہندسہ--)
			آخری_ثنائی_ہندسہ |= 1 << mark_ثنائی_ہندسہ;

		/*
		 * Compare blocks or inodes in the last byte.
		 * If there is any marked bit, this function returns 0.
		 */
		إذا (آخری_ثنائی_ہندسہ & ADDR[شروع_byte + len_byte])
			أرجع 0;
		وإلا إذا (len_byte == 0)
			أرجع 1;
	}

	/* Check whether all bytes are 0 */
	أرجع ext2fs_mem_is_zero(ADDR + شروع_byte, len_byte);
}

errcode_t ext2fs_لبھو_پہلا_zero_generic_bitmap(ext2fs_generic_bitmap gen_bitmap,
						__u32 شروع, __u32 سرا,
						__u32 *باہرلا)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	blk_t b;

	إذا (شروع < bitmap->شروع || سرا > bitmap->سرا || شروع > سرا) {
		ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_TEST_ERROR, شروع);
		أرجع EINVAL;
	}

	طالما (شروع <= سرا) {
		b = ext2fs_test_ثنائی_ہندسہ(شروع - bitmap->شروع, bitmap->bitmap);
		إذا (!b) {
			*باہرلا = شروع;
			أرجع 0;
		}
		شروع++;
	}

	أرجع ENOENT;
}

errcode_t ext2fs_لبھو_پہلا_سیٹ_generic_bitmap(ext2fs_generic_bitmap gen_bitmap,
					       __u32 شروع, __u32 سرا,
					       __u32 *باہرلا)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	blk_t b;

	إذا (شروع < bitmap->شروع || سرا > bitmap->سرا || شروع > سرا) {
		ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_TEST_ERROR, شروع);
		أرجع EINVAL;
	}

	طالما (شروع <= سرا) {
		b = ext2fs_test_ثنائی_ہندسہ(شروع - bitmap->شروع, bitmap->bitmap);
		إذا (b) {
			*باہرلا = شروع;
			أرجع 0;
		}
		شروع++;
	}

	أرجع ENOENT;
}

صحيح ext2fs_test_بلاک_bitmap_range(ext2fs_بلاک_bitmap gen_bitmap,
				   blk_t بلاک, صحيح عدد)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	EXT2_CHECK_MAGIC(bitmap, EXT2_ET_MAGIC_بلاک_BITMAP);
	إذا ((بلاک < bitmap->شروع) || (بلاک > bitmap->real_سرا) ||
	    (بلاک+عدد-1 > bitmap->real_سرا)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_بلاک_TEST,
				   بلاک, bitmap->description);
		أرجع 0;
	}
	أرجع ext2fs_test_clear_generic_bitmap_range((ext2fs_generic_bitmap)
						      bitmap, بلاک, عدد);
}

صحيح ext2fs_test_inode_bitmap_range(ext2fs_inode_bitmap gen_bitmap,
				   ext2_ino_t inode, صحيح عدد)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	EXT2_CHECK_MAGIC(bitmap, EXT2_ET_MAGIC_INODE_BITMAP);
	إذا ((inode < bitmap->شروع) || (inode > bitmap->real_سرا) ||
	    (inode+عدد-1 > bitmap->real_سرا)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_INODE_TEST,
				   inode, bitmap->description);
		أرجع 0;
	}
	أرجع ext2fs_test_clear_generic_bitmap_range((ext2fs_generic_bitmap)
						      bitmap, inode, عدد);
}

فراغ ext2fs_mark_بلاک_bitmap_range(ext2fs_بلاک_bitmap gen_bitmap,
				    blk_t بلاک, صحيح عدد)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	صحيح	i;

	إذا ((بلاک < bitmap->شروع) || (بلاک > bitmap->سرا) ||
	    (بلاک+عدد-1 > bitmap->سرا)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_بلاک_MARK, بلاک,
				   bitmap->description);
		أرجع;
	}
	لكل (i=0; i < عدد; i++)
		ext2fs_fast_سیٹ_ثنائی_ہندسہ(بلاک + i - bitmap->شروع, bitmap->bitmap);
}

فراغ ext2fs_unmark_بلاک_bitmap_range(ext2fs_بلاک_bitmap gen_bitmap,
				      blk_t بلاک, صحيح عدد)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	صحيح	i;

	إذا ((بلاک < bitmap->شروع) || (بلاک > bitmap->سرا) ||
	    (بلاک+عدد-1 > bitmap->سرا)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_بلاک_UNMARK, بلاک,
				   bitmap->description);
		أرجع;
	}
	لكل (i=0; i < عدد; i++)
		ext2fs_fast_clear_ثنائی_ہندسہ(بلاک + i - bitmap->شروع,
				      bitmap->bitmap);
}

