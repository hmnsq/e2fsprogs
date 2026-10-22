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
#جيڪڏهن_پورو
#ضمن <fcntl.h>
#ضمن <time.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#جيڪڏهن_پورو
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#جيڪڏهن_پورو

#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"

بنية ext2fs_struct_generic_bitmap_32 {
	errcode_t	magic;
	ext2_filsys 	fs;
	__u32		شروعات, پڇاڙي;
	__u32		real_پڇاڙي;
	حرف	*	description;
	حرف	*	bitmap;
	errcode_t	بنياد_error_ڪوڊ;
	__u32		reserved[7];
};

تعريفنوع بنية ext2fs_struct_generic_bitmap_32 *ext2fs_generic_bitmap_32;

#وصف_ڪريو EXT2FS_IS_32_BITMAP(bmap) \
	(((bmap)->magic == EXT2_ET_MAGIC_GENERIC_BITMAP) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_بلاڪ_BITMAP) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_INODE_BITMAP))

#وصف_ڪريو EXT2FS_IS_64_BITMAP(bmap) \
	(((bmap)->magic == EXT2_ET_MAGIC_GENERIC_BITMAP64) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_بلاڪ_BITMAP64) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_INODE_BITMAP64))

/*
 * Used by previously inlined function, so we have to export this and
 * not change the function signature
 */
فراغ ext2fs_warn_bitmap2(ext2fs_generic_bitmap gen_bitmap,
			    صحيح ڪوڊ, بنانشان ڊگهو arg)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

#جيڪڏهن_وصف_نه_ٿيل OMIT_COM_ERR
	إذا (bitmap->description)
		com_err(0, bitmap->بنياد_error_ڪوڊ+ڪوڊ,
			"#%lu for %s", arg, bitmap->description);
	وإلا
		com_err(0, bitmap->بنياد_error_ڪوڊ + ڪوڊ, "#%lu", arg);
#جيڪڏهن_پورو
}

ساكن errcode_t check_magic(ext2fs_generic_bitmap bitmap)
{
	إذا (!bitmap || !((bitmap->magic == EXT2_ET_MAGIC_GENERIC_BITMAP) ||
			 (bitmap->magic == EXT2_ET_MAGIC_INODE_BITMAP) ||
			 (bitmap->magic == EXT2_ET_MAGIC_بلاڪ_BITMAP)))
		أرجع EXT2_ET_MAGIC_GENERIC_BITMAP;
	أرجع 0;
}

errcode_t ext2fs_make_generic_bitmap(errcode_t magic, ext2_filsys fs,
				     __u32 شروعات, __u32 پڇاڙي, __u32 real_پڇاڙي,
				     ثابت حرف *descr, حرف *init_map,
				     ext2fs_generic_bitmap *ret)
{
	ext2fs_generic_bitmap_32 bitmap;
	errcode_t		retval;
	size_t			سائيز;

	retval = ext2fs_get_mem(حجم(بنية ext2fs_struct_generic_bitmap_32),
				&bitmap);
	إذا (retval)
		أرجع retval;

	bitmap->magic = magic;
	bitmap->fs = fs;
	bitmap->شروعات = شروعات;
	bitmap->پڇاڙي = پڇاڙي;
	bitmap->real_پڇاڙي = real_پڇاڙي;
	بدل (magic) {
	حالة EXT2_ET_MAGIC_INODE_BITMAP:
		bitmap->بنياد_error_ڪوڊ = EXT2_ET_BAD_INODE_MARK;
		اكسر;
	حالة EXT2_ET_MAGIC_بلاڪ_BITMAP:
		bitmap->بنياد_error_ڪوڊ = EXT2_ET_BAD_بلاڪ_MARK;
		اكسر;
	افتراضي:
		bitmap->بنياد_error_ڪوڊ = EXT2_ET_BAD_GENERIC_MARK;
	}
	إذا (descr) {
		retval = ext2fs_get_mem(لڙي_ڊيگهه(descr)+1, &bitmap->description);
		إذا (retval) {
			ext2fs_free_mem(&bitmap);
			أرجع retval;
		}
		strcpy(bitmap->description, descr);
	} وإلا
		bitmap->description = 0;

	سائيز = (size_t) (((bitmap->real_پڇاڙي - bitmap->شروعات) / 8) + 1);
	/* Round up to allow for the BT x86 instruction */
	سائيز = (سائيز + 7) & ~3;
	retval = ext2fs_get_mem(سائيز, &bitmap->bitmap);
	إذا (retval) {
		ext2fs_free_mem(&bitmap->description);
		ext2fs_free_mem(&bitmap);
		أرجع retval;
	}

	إذا (init_map)
		يادگيري_نقل(bitmap->bitmap, init_map, سائيز);
	وإلا
		memset(bitmap->bitmap, 0, سائيز);
	*ret = (ext2fs_generic_bitmap) bitmap;
	أرجع 0;
}

errcode_t ext2fs_allocate_generic_bitmap(__u32 شروعات,
					 __u32 پڇاڙي,
					 __u32 real_پڇاڙي,
					 ثابت حرف *descr,
					 ext2fs_generic_bitmap *ret)
{
	أرجع ext2fs_make_generic_bitmap(EXT2_ET_MAGIC_GENERIC_BITMAP, 0,
					  شروعات, پڇاڙي, real_پڇاڙي, descr, 0, ret);
}

errcode_t ext2fs_copy_generic_bitmap(ext2fs_generic_bitmap gen_src,
				     ext2fs_generic_bitmap *dest)
{
	ext2fs_generic_bitmap_32 src = (ext2fs_generic_bitmap_32) gen_src;

	أرجع (ext2fs_make_generic_bitmap(src->magic, src->fs,
					   src->شروعات, src->پڇاڙي,
					   src->real_پڇاڙي,
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
#جيڪڏهن_وصف_نه_ٿيل OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"test_bitmap(%lu)", (بنانشان ڊگهو) bitno);
#جيڪڏهن_پورو
		أرجع 0;
	}

	إذا ((bitno < bitmap32->شروعات) || (bitno > bitmap32->پڇاڙي)) {
		ext2fs_warn_bitmap2(bitmap, EXT2FS_TEST_ERROR, bitno);
		أرجع 0;
	}
	أرجع ext2fs_test_ٻٽو_انگ(bitno - bitmap32->شروعات, bitmap32->bitmap);
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
#جيڪڏهن_وصف_نه_ٿيل OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"mark_bitmap(%lu)", (بنانشان ڊگهو) bitno);
#جيڪڏهن_پورو
		أرجع 0;
	}

	إذا ((bitno < bitmap32->شروعات) || (bitno > bitmap32->پڇاڙي)) {
		ext2fs_warn_bitmap2(bitmap, EXT2FS_MARK_ERROR, bitno);
		أرجع 0;
	}
	أرجع ext2fs_سيٽ_ٻٽو_انگ(bitno - bitmap32->شروعات, bitmap32->bitmap);
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
#جيڪڏهن_وصف_نه_ٿيل OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"mark_bitmap(%lu)", (بنانشان ڊگهو) bitno);
#جيڪڏهن_پورو
		أرجع 0;
	}

	إذا ((bitno < bitmap32->شروعات) || (bitno > bitmap32->پڇاڙي)) {
		ext2fs_warn_bitmap2(bitmap, EXT2FS_UNMARK_ERROR, bitno);
		أرجع 0;
	}
	أرجع ext2fs_clear_ٻٽو_انگ(bitno - bitmap32->شروعات, bitmap32->bitmap);
}

__u32 ext2fs_get_generic_bitmap_شروعات(ext2fs_generic_bitmap bitmap)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	إذا (!EXT2FS_IS_32_BITMAP(bitmap)) {
		إذا (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			أرجع ext2fs_get_generic_bmap_شروعات(bitmap);
		}
#جيڪڏهن_وصف_نه_ٿيل OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"get_bitmap_start");
#جيڪڏهن_پورو
		أرجع 0;
	}

	أرجع bitmap32->شروعات;
}

__u32 ext2fs_get_generic_bitmap_پڇاڙي(ext2fs_generic_bitmap bitmap)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	إذا (!EXT2FS_IS_32_BITMAP(bitmap)) {
		إذا (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			أرجع ext2fs_get_generic_bmap_پڇاڙي(bitmap);
		}
#جيڪڏهن_وصف_نه_ٿيل OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"get_bitmap_end");
#جيڪڏهن_پورو
		أرجع 0;
	}
	أرجع bitmap32->پڇاڙي;
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
#جيڪڏهن_وصف_نه_ٿيل OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"clear_generic_bitmap");
#جيڪڏهن_پورو
		أرجع;
	}

	memset(bitmap32->bitmap, 0,
	       (size_t) (((bitmap32->real_پڇاڙي - bitmap32->شروعات) / 8) + 1));
}

errcode_t ext2fs_fudge_generic_bitmap_پڇاڙي(ext2fs_inode_bitmap gen_bitmap,
					  errcode_t magic, errcode_t neq,
					  ext2_ino_t پڇاڙي, ext2_ino_t *oend)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	EXT2_CHECK_MAGIC(bitmap, magic);

	إذا (پڇاڙي > bitmap->real_پڇاڙي)
		أرجع neq;
	إذا (oend)
		*oend = bitmap->پڇاڙي;
	bitmap->پڇاڙي = پڇاڙي;
	أرجع 0;
}

errcode_t ext2fs_resize_generic_bitmap(errcode_t magic,
				       __u32 new_پڇاڙي, __u32 new_real_پڇاڙي,
				       ext2fs_generic_bitmap gen_bmap)
{
	ext2fs_generic_bitmap_32 bmap = (ext2fs_generic_bitmap_32) gen_bmap;
	errcode_t	retval;
	size_t		سائيز, new_سائيز;
	__u32		bitno;

	إذا (!bmap || (bmap->magic != magic))
		أرجع magic;

	/*
	 * If we're expanding the bitmap, make sure all of the new
	 * parts of the bitmap are zero.
	 */
	إذا (new_پڇاڙي > bmap->پڇاڙي) {
		bitno = bmap->real_پڇاڙي;
		إذا (bitno > new_پڇاڙي)
			bitno = new_پڇاڙي;
		لكل (; bitno > bmap->پڇاڙي; bitno--)
			ext2fs_clear_ٻٽو_انگ(bitno - bmap->شروعات, bmap->bitmap);
	}
	إذا (new_real_پڇاڙي == bmap->real_پڇاڙي) {
		bmap->پڇاڙي = new_پڇاڙي;
		أرجع 0;
	}

	سائيز = ((bmap->real_پڇاڙي - bmap->شروعات) / 8) + 1;
	new_سائيز = ((new_real_پڇاڙي - bmap->شروعات) / 8) + 1;

	إذا (سائيز != new_سائيز) {
		retval = ext2fs_resize_mem(سائيز, new_سائيز, &bmap->bitmap);
		إذا (retval)
			أرجع retval;
	}
	إذا (new_سائيز > سائيز)
		memset(bmap->bitmap + سائيز, 0, new_سائيز - سائيز);

	bmap->پڇاڙي = new_پڇاڙي;
	bmap->real_پڇاڙي = new_real_پڇاڙي;
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

	إذا ((bm1->شروعات != bm2->شروعات) ||
	    (bm1->پڇاڙي != bm2->پڇاڙي) ||
	    (memcmp(bm1->bitmap, bm2->bitmap,
		    (size_t) (bm1->پڇاڙي - bm1->شروعات)/8)))
		أرجع neq;

	لكل (i = bm1->پڇاڙي - ((bm1->پڇاڙي - bm1->شروعات) % 8); i <= bm1->پڇاڙي; i++)
		إذا (ext2fs_fast_test_بلاڪ_bitmap(gen_bm1, i) !=
		    ext2fs_fast_test_بلاڪ_bitmap(gen_bm2, i))
			أرجع neq;

	أرجع 0;
}

فراغ ext2fs_سيٽ_generic_bitmap_padding(ext2fs_generic_bitmap gen_map)
{
	ext2fs_generic_bitmap_32 map = (ext2fs_generic_bitmap_32) gen_map;
	__u32	i, j;

	/* Protect loop from wrap-around if map->real_end is maxed */
	لكل (i=map->پڇاڙي+1, j = i - map->شروعات;
	     i <= map->real_پڇاڙي && i > map->پڇاڙي;
	     i++, j++)
		ext2fs_سيٽ_ٻٽو_انگ(j, map->bitmap);
}

errcode_t ext2fs_get_generic_bitmap_range(ext2fs_generic_bitmap gen_bmap,
					  errcode_t magic,
					  __u32 شروعات, __u32 انگ,
					  فراغ *ٻاهريون)
{
	ext2fs_generic_bitmap_32 bmap = (ext2fs_generic_bitmap_32) gen_bmap;

	إذا (!bmap || (bmap->magic != magic))
		أرجع magic;

	إذا ((شروعات < bmap->شروعات) || (شروعات+انگ-1 > bmap->real_پڇاڙي))
		أرجع EXT2_ET_INVALID_دليل;

	يادگيري_نقل(ٻاهريون, bmap->bitmap + ((شروعات - bmap->شروعات) >> 3), (انگ+7) >> 3);
	أرجع 0;
}

errcode_t ext2fs_سيٽ_generic_bitmap_range(ext2fs_generic_bitmap gen_bmap,
					  errcode_t magic,
					  __u32 شروعات, __u32 انگ,
					  فراغ *اندريون)
{
	ext2fs_generic_bitmap_32 bmap = (ext2fs_generic_bitmap_32) gen_bmap;

	إذا (!bmap || (bmap->magic != magic))
		أرجع magic;

	إذا ((شروعات < bmap->شروعات) || (شروعات+انگ-1 > bmap->real_پڇاڙي))
		أرجع EXT2_ET_INVALID_دليل;

	يادگيري_نقل(bmap->bitmap + ((شروعات - bmap->شروعات) >> 3), اندريون, (انگ+7) >> 3);
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
						  بنانشان صحيح شروعات,
						  بنانشان صحيح len)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	size_t شروعات_byte, len_byte = len >> 3;
	بنانشان صحيح شروعات_ٻٽو_انگ, len_ٻٽو_انگ = len % 8;
	صحيح پهريون_ٻٽو_انگ = 0;
	صحيح آخري_ٻٽو_انگ  = 0;
	صحيح mark_ڳڻپ = 0;
	صحيح mark_ٻٽو_انگ = 0;
	صحيح i;
	ثابت حرف *ADDR = bitmap->bitmap;

	شروعات -= bitmap->شروعات;
	شروعات_byte = شروعات >> 3;
	شروعات_ٻٽو_انگ = شروعات % 8;

	إذا (شروعات_ٻٽو_انگ != 0) {
		/*
		 * The compared start block number or start inode number
		 * is not the first bit in a byte.
		 */
		mark_ڳڻپ = 8 - شروعات_ٻٽو_انگ;
		إذا (len < 8 - شروعات_ٻٽو_انگ) {
			mark_ڳڻپ = (صحيح)len;
			mark_ٻٽو_انگ = len + شروعات_ٻٽو_انگ - 1;
		} وإلا
			mark_ٻٽو_انگ = 7;

		لكل (i = mark_ڳڻپ; i > 0; i--, mark_ٻٽو_انگ--)
			پهريون_ٻٽو_انگ |= 1 << mark_ٻٽو_انگ;

		/*
		 * Compare blocks or inodes in the first byte.
		 * If there is any marked bit, this function returns 0.
		 */
		إذا (پهريون_ٻٽو_انگ & ADDR[شروعات_byte])
			أرجع 0;
		وإلا إذا (len <= 8 - شروعات_ٻٽو_انگ)
			أرجع 1;

		شروعات_byte++;
		len_ٻٽو_انگ = (len - mark_ڳڻپ) % 8;
		len_byte = (len - mark_ڳڻپ) >> 3;
	}

	/*
	 * The compared start block number or start inode number is
	 * the first bit in a byte.
	 */
	إذا (len_ٻٽو_انگ != 0) {
		/*
		 * The compared end block number or end inode number is
		 * not the last bit in a byte.
		 */
		لكل (mark_ٻٽو_انگ = len_ٻٽو_انگ - 1; mark_ٻٽو_انگ >= 0; mark_ٻٽو_انگ--)
			آخري_ٻٽو_انگ |= 1 << mark_ٻٽو_انگ;

		/*
		 * Compare blocks or inodes in the last byte.
		 * If there is any marked bit, this function returns 0.
		 */
		إذا (آخري_ٻٽو_انگ & ADDR[شروعات_byte + len_byte])
			أرجع 0;
		وإلا إذا (len_byte == 0)
			أرجع 1;
	}

	/* Check whether all bytes are 0 */
	أرجع ext2fs_mem_is_zero(ADDR + شروعات_byte, len_byte);
}

errcode_t ext2fs_ڳوليو_پهريون_zero_generic_bitmap(ext2fs_generic_bitmap gen_bitmap,
						__u32 شروعات, __u32 پڇاڙي,
						__u32 *ٻاهريون)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	blk_t b;

	إذا (شروعات < bitmap->شروعات || پڇاڙي > bitmap->پڇاڙي || شروعات > پڇاڙي) {
		ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_TEST_ERROR, شروعات);
		أرجع EINVAL;
	}

	طالما (شروعات <= پڇاڙي) {
		b = ext2fs_test_ٻٽو_انگ(شروعات - bitmap->شروعات, bitmap->bitmap);
		إذا (!b) {
			*ٻاهريون = شروعات;
			أرجع 0;
		}
		شروعات++;
	}

	أرجع ENOENT;
}

errcode_t ext2fs_ڳوليو_پهريون_سيٽ_generic_bitmap(ext2fs_generic_bitmap gen_bitmap,
					       __u32 شروعات, __u32 پڇاڙي,
					       __u32 *ٻاهريون)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	blk_t b;

	إذا (شروعات < bitmap->شروعات || پڇاڙي > bitmap->پڇاڙي || شروعات > پڇاڙي) {
		ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_TEST_ERROR, شروعات);
		أرجع EINVAL;
	}

	طالما (شروعات <= پڇاڙي) {
		b = ext2fs_test_ٻٽو_انگ(شروعات - bitmap->شروعات, bitmap->bitmap);
		إذا (b) {
			*ٻاهريون = شروعات;
			أرجع 0;
		}
		شروعات++;
	}

	أرجع ENOENT;
}

صحيح ext2fs_test_بلاڪ_bitmap_range(ext2fs_بلاڪ_bitmap gen_bitmap,
				   blk_t بلاڪ, صحيح انگ)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	EXT2_CHECK_MAGIC(bitmap, EXT2_ET_MAGIC_بلاڪ_BITMAP);
	إذا ((بلاڪ < bitmap->شروعات) || (بلاڪ > bitmap->real_پڇاڙي) ||
	    (بلاڪ+انگ-1 > bitmap->real_پڇاڙي)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_بلاڪ_TEST,
				   بلاڪ, bitmap->description);
		أرجع 0;
	}
	أرجع ext2fs_test_clear_generic_bitmap_range((ext2fs_generic_bitmap)
						      bitmap, بلاڪ, انگ);
}

صحيح ext2fs_test_inode_bitmap_range(ext2fs_inode_bitmap gen_bitmap,
				   ext2_ino_t inode, صحيح انگ)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	EXT2_CHECK_MAGIC(bitmap, EXT2_ET_MAGIC_INODE_BITMAP);
	إذا ((inode < bitmap->شروعات) || (inode > bitmap->real_پڇاڙي) ||
	    (inode+انگ-1 > bitmap->real_پڇاڙي)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_INODE_TEST,
				   inode, bitmap->description);
		أرجع 0;
	}
	أرجع ext2fs_test_clear_generic_bitmap_range((ext2fs_generic_bitmap)
						      bitmap, inode, انگ);
}

فراغ ext2fs_mark_بلاڪ_bitmap_range(ext2fs_بلاڪ_bitmap gen_bitmap,
				    blk_t بلاڪ, صحيح انگ)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	صحيح	i;

	إذا ((بلاڪ < bitmap->شروعات) || (بلاڪ > bitmap->پڇاڙي) ||
	    (بلاڪ+انگ-1 > bitmap->پڇاڙي)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_بلاڪ_MARK, بلاڪ,
				   bitmap->description);
		أرجع;
	}
	لكل (i=0; i < انگ; i++)
		ext2fs_fast_سيٽ_ٻٽو_انگ(بلاڪ + i - bitmap->شروعات, bitmap->bitmap);
}

فراغ ext2fs_unmark_بلاڪ_bitmap_range(ext2fs_بلاڪ_bitmap gen_bitmap,
				      blk_t بلاڪ, صحيح انگ)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	صحيح	i;

	إذا ((بلاڪ < bitmap->شروعات) || (بلاڪ > bitmap->پڇاڙي) ||
	    (بلاڪ+انگ-1 > bitmap->پڇاڙي)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_بلاڪ_UNMARK, بلاڪ,
				   bitmap->description);
		أرجع;
	}
	لكل (i=0; i < انگ; i++)
		ext2fs_fast_clear_ٻٽو_انگ(بلاڪ + i - bitmap->شروعات,
				      bitmap->bitmap);
}

