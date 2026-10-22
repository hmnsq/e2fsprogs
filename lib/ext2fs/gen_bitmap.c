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
#د_که_پای
#ضمن <fcntl.h>
#ضمن <time.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#د_که_پای
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#د_که_پای

#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"

بنية ext2fs_struct_generic_bitmap_32 {
	errcode_t	magic;
	ext2_filsys 	fs;
	__u32		پیل, پای_ټکی;
	__u32		real_پای_ټکی;
	حرف	*	description;
	حرف	*	bitmap;
	errcode_t	بنسټ_error_کوډ;
	__u32		reserved[7];
};

تعريفنوع بنية ext2fs_struct_generic_bitmap_32 *ext2fs_generic_bitmap_32;

#تعریف_کړه EXT2FS_IS_32_BITMAP(bmap) \
	(((bmap)->magic == EXT2_ET_MAGIC_GENERIC_BITMAP) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_بلاک_BITMAP) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_INODE_BITMAP))

#تعریف_کړه EXT2FS_IS_64_BITMAP(bmap) \
	(((bmap)->magic == EXT2_ET_MAGIC_GENERIC_BITMAP64) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_بلاک_BITMAP64) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_INODE_BITMAP64))

/*
 * Used by previously inlined function, so we have to export this and
 * not change the function signature
 */
فراغ ext2fs_warn_bitmap2(ext2fs_generic_bitmap gen_bitmap,
			    صحيح کوډ, بینښه اوږد arg)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

#که_نه_تعریف_شوی OMIT_COM_ERR
	إذا (bitmap->description)
		com_err(0, bitmap->بنسټ_error_کوډ+کوډ,
			"#%lu for %s", arg, bitmap->description);
	وإلا
		com_err(0, bitmap->بنسټ_error_کوډ + کوډ, "#%lu", arg);
#د_که_پای
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
				     __u32 پیل, __u32 پای_ټکی, __u32 real_پای_ټکی,
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
	bitmap->پیل = پیل;
	bitmap->پای_ټکی = پای_ټکی;
	bitmap->real_پای_ټکی = real_پای_ټکی;
	بدل (magic) {
	حالة EXT2_ET_MAGIC_INODE_BITMAP:
		bitmap->بنسټ_error_کوډ = EXT2_ET_BAD_INODE_MARK;
		اكسر;
	حالة EXT2_ET_MAGIC_بلاک_BITMAP:
		bitmap->بنسټ_error_کوډ = EXT2_ET_BAD_بلاک_MARK;
		اكسر;
	افتراضي:
		bitmap->بنسټ_error_کوډ = EXT2_ET_BAD_GENERIC_MARK;
	}
	إذا (descr) {
		retval = ext2fs_get_mem(مزي_اوږدوالی(descr)+1, &bitmap->description);
		إذا (retval) {
			ext2fs_free_mem(&bitmap);
			أرجع retval;
		}
		strcpy(bitmap->description, descr);
	} وإلا
		bitmap->description = 0;

	اندازه = (size_t) (((bitmap->real_پای_ټکی - bitmap->پیل) / 8) + 1);
	/* Round up to allow for the BT x86 instruction */
	اندازه = (اندازه + 7) & ~3;
	retval = ext2fs_get_mem(اندازه, &bitmap->bitmap);
	إذا (retval) {
		ext2fs_free_mem(&bitmap->description);
		ext2fs_free_mem(&bitmap);
		أرجع retval;
	}

	إذا (init_map)
		حافظه_نقل(bitmap->bitmap, init_map, اندازه);
	وإلا
		memset(bitmap->bitmap, 0, اندازه);
	*ret = (ext2fs_generic_bitmap) bitmap;
	أرجع 0;
}

errcode_t ext2fs_allocate_generic_bitmap(__u32 پیل,
					 __u32 پای_ټکی,
					 __u32 real_پای_ټکی,
					 ثابت حرف *descr,
					 ext2fs_generic_bitmap *ret)
{
	أرجع ext2fs_make_generic_bitmap(EXT2_ET_MAGIC_GENERIC_BITMAP, 0,
					  پیل, پای_ټکی, real_پای_ټکی, descr, 0, ret);
}

errcode_t ext2fs_copy_generic_bitmap(ext2fs_generic_bitmap gen_src,
				     ext2fs_generic_bitmap *dest)
{
	ext2fs_generic_bitmap_32 src = (ext2fs_generic_bitmap_32) gen_src;

	أرجع (ext2fs_make_generic_bitmap(src->magic, src->fs,
					   src->پیل, src->پای_ټکی,
					   src->real_پای_ټکی,
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
#که_نه_تعریف_شوی OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"test_bitmap(%lu)", (بینښه اوږد) bitno);
#د_که_پای
		أرجع 0;
	}

	إذا ((bitno < bitmap32->پیل) || (bitno > bitmap32->پای_ټکی)) {
		ext2fs_warn_bitmap2(bitmap, EXT2FS_TEST_ERROR, bitno);
		أرجع 0;
	}
	أرجع ext2fs_test_بټ(bitno - bitmap32->پیل, bitmap32->bitmap);
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
#که_نه_تعریف_شوی OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"mark_bitmap(%lu)", (بینښه اوږد) bitno);
#د_که_پای
		أرجع 0;
	}

	إذا ((bitno < bitmap32->پیل) || (bitno > bitmap32->پای_ټکی)) {
		ext2fs_warn_bitmap2(bitmap, EXT2FS_MARK_ERROR, bitno);
		أرجع 0;
	}
	أرجع ext2fs_سېټ_بټ(bitno - bitmap32->پیل, bitmap32->bitmap);
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
#که_نه_تعریف_شوی OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"mark_bitmap(%lu)", (بینښه اوږد) bitno);
#د_که_پای
		أرجع 0;
	}

	إذا ((bitno < bitmap32->پیل) || (bitno > bitmap32->پای_ټکی)) {
		ext2fs_warn_bitmap2(bitmap, EXT2FS_UNMARK_ERROR, bitno);
		أرجع 0;
	}
	أرجع ext2fs_clear_بټ(bitno - bitmap32->پیل, bitmap32->bitmap);
}

__u32 ext2fs_get_generic_bitmap_پیل(ext2fs_generic_bitmap bitmap)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	إذا (!EXT2FS_IS_32_BITMAP(bitmap)) {
		إذا (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			أرجع ext2fs_get_generic_bmap_پیل(bitmap);
		}
#که_نه_تعریف_شوی OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"get_bitmap_start");
#د_که_پای
		أرجع 0;
	}

	أرجع bitmap32->پیل;
}

__u32 ext2fs_get_generic_bitmap_پای_ټکی(ext2fs_generic_bitmap bitmap)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	إذا (!EXT2FS_IS_32_BITMAP(bitmap)) {
		إذا (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			أرجع ext2fs_get_generic_bmap_پای_ټکی(bitmap);
		}
#که_نه_تعریف_شوی OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"get_bitmap_end");
#د_که_پای
		أرجع 0;
	}
	أرجع bitmap32->پای_ټکی;
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
#که_نه_تعریف_شوی OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"clear_generic_bitmap");
#د_که_پای
		أرجع;
	}

	memset(bitmap32->bitmap, 0,
	       (size_t) (((bitmap32->real_پای_ټکی - bitmap32->پیل) / 8) + 1));
}

errcode_t ext2fs_fudge_generic_bitmap_پای_ټکی(ext2fs_inode_bitmap gen_bitmap,
					  errcode_t magic, errcode_t neq,
					  ext2_ino_t پای_ټکی, ext2_ino_t *oend)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	EXT2_CHECK_MAGIC(bitmap, magic);

	إذا (پای_ټکی > bitmap->real_پای_ټکی)
		أرجع neq;
	إذا (oend)
		*oend = bitmap->پای_ټکی;
	bitmap->پای_ټکی = پای_ټکی;
	أرجع 0;
}

errcode_t ext2fs_resize_generic_bitmap(errcode_t magic,
				       __u32 new_پای_ټکی, __u32 new_real_پای_ټکی,
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
	إذا (new_پای_ټکی > bmap->پای_ټکی) {
		bitno = bmap->real_پای_ټکی;
		إذا (bitno > new_پای_ټکی)
			bitno = new_پای_ټکی;
		لكل (; bitno > bmap->پای_ټکی; bitno--)
			ext2fs_clear_بټ(bitno - bmap->پیل, bmap->bitmap);
	}
	إذا (new_real_پای_ټکی == bmap->real_پای_ټکی) {
		bmap->پای_ټکی = new_پای_ټکی;
		أرجع 0;
	}

	اندازه = ((bmap->real_پای_ټکی - bmap->پیل) / 8) + 1;
	new_اندازه = ((new_real_پای_ټکی - bmap->پیل) / 8) + 1;

	إذا (اندازه != new_اندازه) {
		retval = ext2fs_resize_mem(اندازه, new_اندازه, &bmap->bitmap);
		إذا (retval)
			أرجع retval;
	}
	إذا (new_اندازه > اندازه)
		memset(bmap->bitmap + اندازه, 0, new_اندازه - اندازه);

	bmap->پای_ټکی = new_پای_ټکی;
	bmap->real_پای_ټکی = new_real_پای_ټکی;
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

	إذا ((bm1->پیل != bm2->پیل) ||
	    (bm1->پای_ټکی != bm2->پای_ټکی) ||
	    (memcmp(bm1->bitmap, bm2->bitmap,
		    (size_t) (bm1->پای_ټکی - bm1->پیل)/8)))
		أرجع neq;

	لكل (i = bm1->پای_ټکی - ((bm1->پای_ټکی - bm1->پیل) % 8); i <= bm1->پای_ټکی; i++)
		إذا (ext2fs_fast_test_بلاک_bitmap(gen_bm1, i) !=
		    ext2fs_fast_test_بلاک_bitmap(gen_bm2, i))
			أرجع neq;

	أرجع 0;
}

فراغ ext2fs_سېټ_generic_bitmap_padding(ext2fs_generic_bitmap gen_map)
{
	ext2fs_generic_bitmap_32 map = (ext2fs_generic_bitmap_32) gen_map;
	__u32	i, j;

	/* Protect loop from wrap-around if map->real_end is maxed */
	لكل (i=map->پای_ټکی+1, j = i - map->پیل;
	     i <= map->real_پای_ټکی && i > map->پای_ټکی;
	     i++, j++)
		ext2fs_سېټ_بټ(j, map->bitmap);
}

errcode_t ext2fs_get_generic_bitmap_range(ext2fs_generic_bitmap gen_bmap,
					  errcode_t magic,
					  __u32 پیل, __u32 عدد,
					  فراغ *بهر)
{
	ext2fs_generic_bitmap_32 bmap = (ext2fs_generic_bitmap_32) gen_bmap;

	إذا (!bmap || (bmap->magic != magic))
		أرجع magic;

	إذا ((پیل < bmap->پیل) || (پیل+عدد-1 > bmap->real_پای_ټکی))
		أرجع EXT2_ET_INVALID_دلیل;

	حافظه_نقل(بهر, bmap->bitmap + ((پیل - bmap->پیل) >> 3), (عدد+7) >> 3);
	أرجع 0;
}

errcode_t ext2fs_سېټ_generic_bitmap_range(ext2fs_generic_bitmap gen_bmap,
					  errcode_t magic,
					  __u32 پیل, __u32 عدد,
					  فراغ *دننه)
{
	ext2fs_generic_bitmap_32 bmap = (ext2fs_generic_bitmap_32) gen_bmap;

	إذا (!bmap || (bmap->magic != magic))
		أرجع magic;

	إذا ((پیل < bmap->پیل) || (پیل+عدد-1 > bmap->real_پای_ټکی))
		أرجع EXT2_ET_INVALID_دلیل;

	حافظه_نقل(bmap->bitmap + ((پیل - bmap->پیل) >> 3), دننه, (عدد+7) >> 3);
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
						  بینښه صحيح پیل,
						  بینښه صحيح len)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	size_t پیل_byte, len_byte = len >> 3;
	بینښه صحيح پیل_بټ, len_بټ = len % 8;
	صحيح لومړی_بټ = 0;
	صحيح وروستی_بټ  = 0;
	صحيح mark_شمېر = 0;
	صحيح mark_بټ = 0;
	صحيح i;
	ثابت حرف *ADDR = bitmap->bitmap;

	پیل -= bitmap->پیل;
	پیل_byte = پیل >> 3;
	پیل_بټ = پیل % 8;

	إذا (پیل_بټ != 0) {
		/*
		 * The compared start block number or start inode number
		 * is not the first bit in a byte.
		 */
		mark_شمېر = 8 - پیل_بټ;
		إذا (len < 8 - پیل_بټ) {
			mark_شمېر = (صحيح)len;
			mark_بټ = len + پیل_بټ - 1;
		} وإلا
			mark_بټ = 7;

		لكل (i = mark_شمېر; i > 0; i--, mark_بټ--)
			لومړی_بټ |= 1 << mark_بټ;

		/*
		 * Compare blocks or inodes in the first byte.
		 * If there is any marked bit, this function returns 0.
		 */
		إذا (لومړی_بټ & ADDR[پیل_byte])
			أرجع 0;
		وإلا إذا (len <= 8 - پیل_بټ)
			أرجع 1;

		پیل_byte++;
		len_بټ = (len - mark_شمېر) % 8;
		len_byte = (len - mark_شمېر) >> 3;
	}

	/*
	 * The compared start block number or start inode number is
	 * the first bit in a byte.
	 */
	إذا (len_بټ != 0) {
		/*
		 * The compared end block number or end inode number is
		 * not the last bit in a byte.
		 */
		لكل (mark_بټ = len_بټ - 1; mark_بټ >= 0; mark_بټ--)
			وروستی_بټ |= 1 << mark_بټ;

		/*
		 * Compare blocks or inodes in the last byte.
		 * If there is any marked bit, this function returns 0.
		 */
		إذا (وروستی_بټ & ADDR[پیل_byte + len_byte])
			أرجع 0;
		وإلا إذا (len_byte == 0)
			أرجع 1;
	}

	/* Check whether all bytes are 0 */
	أرجع ext2fs_mem_is_zero(ADDR + پیل_byte, len_byte);
}

errcode_t ext2fs_پیدا_کړه_لومړی_zero_generic_bitmap(ext2fs_generic_bitmap gen_bitmap,
						__u32 پیل, __u32 پای_ټکی,
						__u32 *بهر)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	blk_t b;

	إذا (پیل < bitmap->پیل || پای_ټکی > bitmap->پای_ټکی || پیل > پای_ټکی) {
		ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_TEST_ERROR, پیل);
		أرجع EINVAL;
	}

	طالما (پیل <= پای_ټکی) {
		b = ext2fs_test_بټ(پیل - bitmap->پیل, bitmap->bitmap);
		إذا (!b) {
			*بهر = پیل;
			أرجع 0;
		}
		پیل++;
	}

	أرجع ENOENT;
}

errcode_t ext2fs_پیدا_کړه_لومړی_سېټ_generic_bitmap(ext2fs_generic_bitmap gen_bitmap,
					       __u32 پیل, __u32 پای_ټکی,
					       __u32 *بهر)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	blk_t b;

	إذا (پیل < bitmap->پیل || پای_ټکی > bitmap->پای_ټکی || پیل > پای_ټکی) {
		ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_TEST_ERROR, پیل);
		أرجع EINVAL;
	}

	طالما (پیل <= پای_ټکی) {
		b = ext2fs_test_بټ(پیل - bitmap->پیل, bitmap->bitmap);
		إذا (b) {
			*بهر = پیل;
			أرجع 0;
		}
		پیل++;
	}

	أرجع ENOENT;
}

صحيح ext2fs_test_بلاک_bitmap_range(ext2fs_بلاک_bitmap gen_bitmap,
				   blk_t بلاک, صحيح عدد)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	EXT2_CHECK_MAGIC(bitmap, EXT2_ET_MAGIC_بلاک_BITMAP);
	إذا ((بلاک < bitmap->پیل) || (بلاک > bitmap->real_پای_ټکی) ||
	    (بلاک+عدد-1 > bitmap->real_پای_ټکی)) {
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
	إذا ((inode < bitmap->پیل) || (inode > bitmap->real_پای_ټکی) ||
	    (inode+عدد-1 > bitmap->real_پای_ټکی)) {
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

	إذا ((بلاک < bitmap->پیل) || (بلاک > bitmap->پای_ټکی) ||
	    (بلاک+عدد-1 > bitmap->پای_ټکی)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_بلاک_MARK, بلاک,
				   bitmap->description);
		أرجع;
	}
	لكل (i=0; i < عدد; i++)
		ext2fs_fast_سېټ_بټ(بلاک + i - bitmap->پیل, bitmap->bitmap);
}

فراغ ext2fs_unmark_بلاک_bitmap_range(ext2fs_بلاک_bitmap gen_bitmap,
				      blk_t بلاک, صحيح عدد)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	صحيح	i;

	إذا ((بلاک < bitmap->پیل) || (بلاک > bitmap->پای_ټکی) ||
	    (بلاک+عدد-1 > bitmap->پای_ټکی)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_بلاک_UNMARK, بلاک,
				   bitmap->description);
		أرجع;
	}
	لكل (i=0; i < عدد; i++)
		ext2fs_fast_clear_بټ(بلاک + i - bitmap->پیل,
				      bitmap->bitmap);
}

