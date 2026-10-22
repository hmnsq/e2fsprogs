/*
 * blkmap64_ba.c --- Simple bitarray implementation for bitmaps
 *
 * Copyright (C) 2008 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Public
 * License.
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
#ضمن "bmap64.h"

/*
 * Private data for bit array implementation of bitmap ops.
 * Currently, this is just a pointer to our big flat hunk of memory,
 * exactly equivalent to the old-skool char * bitmap member.
 */

بنية ext2fs_ba_خصوصی_struct {
	حرف *bitarray;
};

تعريفنوع بنية ext2fs_ba_خصوصی_struct *ext2fs_ba_خصوصی;

ساكن errcode_t ba_alloc_خصوصی_داده (ext2fs_generic_bitmap_64 bitmap)
{
	ext2fs_ba_خصوصی bp;
	errcode_t	retval;
	size_t		اندازه;

	/*
	 * Since we only have the one pointer, we could just shove our
	 * private data in the void *private field itself, but then
	 * we'd have to do a fair bit of rewriting if we ever added a
	 * field.  I'm agnostic.
	 */
	retval = ext2fs_get_mem(حجم (ext2fs_ba_خصوصی), &bp);
	إذا (retval)
		أرجع retval;

	اندازه = (size_t) (((bitmap->real_انتها - bitmap->شروع) / 8) + 1);

	retval = ext2fs_get_mem(اندازه, &bp->bitarray);
	إذا (retval) {
		ext2fs_free_mem(&bp);
		bp = 0;
		أرجع retval;
	}
	bitmap->خاص = (فراغ *) bp;
	أرجع 0;
}

ساكن errcode_t ba_new_bmap(ext2_filsys fs EXT2FS_ATTR((استفادهنشده)),
			     ext2fs_generic_bitmap_64 bitmap)
{
	ext2fs_ba_خصوصی bp;
	errcode_t	retval;
	size_t		اندازه;

	retval = ba_alloc_خصوصی_داده (bitmap);
	إذا (retval)
		أرجع retval;

	bp = (ext2fs_ba_خصوصی) bitmap->خاص;
	اندازه = (size_t) (((bitmap->real_انتها - bitmap->شروع) / 8) + 1);
	memset(bp->bitarray, 0, اندازه);

	أرجع 0;
}

ساكن فراغ ba_free_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	ext2fs_ba_خصوصی bp = (ext2fs_ba_خصوصی) bitmap->خاص;

	إذا (!bp)
		أرجع;

	إذا (bp->bitarray) {
		ext2fs_free_mem (&bp->bitarray);
		bp->bitarray = 0;
	}
	ext2fs_free_mem (&bp);
	bp = 0;
}

ساكن errcode_t ba_copy_bmap(ext2fs_generic_bitmap_64 src,
			      ext2fs_generic_bitmap_64 dest)
{
	ext2fs_ba_خصوصی src_bp = (ext2fs_ba_خصوصی) src->خاص;
	ext2fs_ba_خصوصی dest_bp;
	errcode_t retval;
	size_t اندازه;

	retval = ba_alloc_خصوصی_داده (dest);
	إذا (retval)
		أرجع retval;

	dest_bp = (ext2fs_ba_خصوصی) dest->خاص;

	اندازه = (size_t) (((src->real_انتها - src->شروع) / 8) + 1);
	کپی_حافظه (dest_bp->bitarray, src_bp->bitarray, اندازه);

	أرجع 0;
}

ساكن errcode_t ba_resize_bmap(ext2fs_generic_bitmap_64 bmap,
				__u64 new_انتها, __u64 new_real_انتها)
{
	ext2fs_ba_خصوصی bp = (ext2fs_ba_خصوصی) bmap->خاص;
	errcode_t	retval;
	size_t		اندازه, new_اندازه;
	__u64		bitno;

	/*
	 * If we're expanding the bitmap, make sure all of the new
	 * parts of the bitmap are zero.
	 */
	إذا (new_انتها > bmap->انتها) {
		bitno = bmap->real_انتها;
		إذا (bitno > new_انتها)
			bitno = new_انتها;
		لكل (; bitno > bmap->انتها; bitno--)
			ext2fs_clear_bit64(bitno - bmap->شروع, bp->bitarray);
	}
	إذا (new_real_انتها == bmap->real_انتها) {
		bmap->انتها = new_انتها;
		أرجع 0;
	}

	اندازه = ((bmap->real_انتها - bmap->شروع) / 8) + 1;
	new_اندازه = ((new_real_انتها - bmap->شروع) / 8) + 1;

	إذا (اندازه != new_اندازه) {
		retval = ext2fs_resize_mem(اندازه, new_اندازه, &bp->bitarray);
		إذا (retval)
			أرجع retval;
	}
	إذا (new_اندازه > اندازه)
		memset(bp->bitarray + اندازه, 0, new_اندازه - اندازه);

	bmap->انتها = new_انتها;
	bmap->real_انتها = new_real_انتها;
	أرجع 0;

}

ساكن صحيح ba_mark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ext2fs_ba_خصوصی bp = (ext2fs_ba_خصوصی) bitmap->خاص;
	blk64_t bitno = (blk64_t) arg;

	أرجع ext2fs_مجموعه_bit64(bitno - bitmap->شروع, bp->bitarray);
}

ساكن صحيح ba_unmark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ext2fs_ba_خصوصی bp = (ext2fs_ba_خصوصی) bitmap->خاص;
	blk64_t bitno = (blk64_t) arg;

	أرجع ext2fs_clear_bit64(bitno - bitmap->شروع, bp->bitarray);
}

ساكن صحيح ba_test_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ext2fs_ba_خصوصی bp = (ext2fs_ba_خصوصی) bitmap->خاص;
	blk64_t bitno = (blk64_t) arg;

	أرجع ext2fs_test_bit64(bitno - bitmap->شروع, bp->bitarray);
}

ساكن فراغ ba_mark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				بیعلامت صحيح رقم)
{
	ext2fs_ba_خصوصی bp = (ext2fs_ba_خصوصی) bitmap->خاص;
	blk64_t bitno = (blk64_t) arg;
	بیعلامت صحيح i;

	لكل (i = 0; i < رقم; i++)
		ext2fs_fast_مجموعه_bit64(bitno + i - bitmap->شروع, bp->bitarray);
}

ساكن فراغ ba_unmark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				  بیعلامت صحيح رقم)
{
	ext2fs_ba_خصوصی bp = (ext2fs_ba_خصوصی) bitmap->خاص;
	blk64_t bitno = (blk64_t) arg;
	بیعلامت صحيح i;

	لكل (i = 0; i < رقم; i++)
		ext2fs_fast_clear_bit64(bitno + i - bitmap->شروع, bp->bitarray);
}

ساكن صحيح ba_test_clear_bmap_extent(ext2fs_generic_bitmap_64 bitmap,
				     __u64 شروع, بیعلامت صحيح len)
{
	ext2fs_ba_خصوصی bp = (ext2fs_ba_خصوصی) bitmap->خاص;
	__u64 شروع_byte, len_byte = len >> 3;
	بیعلامت صحيح شروع_بیت, len_بیت = len % 8;
	بیعلامت صحيح اولی_بیت = 0;
	بیعلامت صحيح آخرین_بیت  = 0;
	صحيح mark_شمار = 0;
	صحيح mark_بیت = 0;
	صحيح i;
	ثابت حرف *ADDR;

	ADDR = bp->bitarray;
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


ساكن errcode_t ba_مجموعه_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 شروع, size_t رقم, فراغ *درون)
{
	ext2fs_ba_خصوصی bp = (ext2fs_ba_خصوصی) bitmap->خاص;

	کپی_حافظه (bp->bitarray + (شروع >> 3), درون, (رقم + 7) >> 3);

	أرجع 0;
}

ساكن errcode_t ba_get_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 شروع, size_t رقم, فراغ *بیرون)
{
	ext2fs_ba_خصوصی bp = (ext2fs_ba_خصوصی) bitmap->خاص;

	کپی_حافظه (بیرون, bp->bitarray + (شروع >> 3), (رقم + 7) >> 3);

	أرجع 0;
}

ساكن فراغ ba_clear_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	ext2fs_ba_خصوصی bp = (ext2fs_ba_خصوصی) bitmap->خاص;

	memset(bp->bitarray, 0,
	       (size_t) (((bitmap->real_انتها - bitmap->شروع) / 8) + 1));
}

#اگر_تعریف_شده ENABLE_BMAP_STATS
ساكن فراغ ba_print_stats(ext2fs_generic_bitmap_64 bitmap)
{
	fprintf(stderr, "%16llu Bytes used by bitarray\n", (بیعلامت دراز دراز)
		((bitmap->real_انتها - bitmap->شروع) >> 3) + 1 +
		حجم(بنية ext2fs_ba_خصوصی_struct));
}
#وإلا
ساكن فراغ ba_print_stats(ext2fs_generic_bitmap_64 bitmap EXT2FS_ATTR((استفادهنشده)))
{
}
#پایان_اگر

/* Find the first zero bit between start and end, inclusive. */
ساكن errcode_t ba_پیدا_کن_اولی_zero(ext2fs_generic_bitmap_64 bitmap,
				    __u64 شروع, __u64 انتها, __u64 *بیرون)
{
	ext2fs_ba_خصوصی bp = (ext2fs_ba_خصوصی)bitmap->خاص;
	بیعلامت دراز bitpos = شروع - bitmap->شروع;
	بیعلامت دراز شمار = انتها - شروع + 1;
	صحيح byte_یافت = 0; /* whether a != 0xff byte has been found */
	ثابت بیعلامت حرف *pos;
	بیعلامت دراز max_loop_شمار, i;

	/* scan bits until we hit a byte boundary */
	طالما ((bitpos & 0x7) != 0 && شمار > 0) {
		إذا (!ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*بیرون = bitpos + bitmap->شروع;
			أرجع 0;
		}
		bitpos++;
		شمار--;
	}

	إذا (!شمار)
		أرجع ENOENT;

	pos = ((بیعلامت حرف *)bp->bitarray) + (bitpos >> 3);
	/* scan bytes until 8-byte (64-bit) aligned */
	طالما (شمار >= 8 && (((uintptr_t)pos) & 0x07)) {
		إذا (*pos != 0xff) {
			byte_یافت = 1;
			اكسر;
		}
		pos++;
		شمار -= 8;
		bitpos += 8;
	}

	إذا (!byte_یافت) {
		max_loop_شمار = شمار >> 6; /* 8-byte blocks */
		i = max_loop_شمار;
		طالما (i) {
			إذا (*((ثابت __u64 *)pos) != ((__u64)-1))
				اكسر;
			pos += 8;
			i--;
		}
		شمار -= 64 * (max_loop_شمار - i);
		bitpos += 64 * (max_loop_شمار - i);

		max_loop_شمار = شمار >> 3;
		i = max_loop_شمار;
		طالما (i) {
			إذا (*pos != 0xff) {
				byte_یافت = 1;
				اكسر;
			}
			pos++;
			i--;
		}
		شمار -= 8 * (max_loop_شمار - i);
		bitpos += 8 * (max_loop_شمار - i);
	}

	/* Here either count < 8 or byte_found == 1. */
	طالما (شمار-- > 0) {
		إذا (!ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*بیرون = bitpos + bitmap->شروع;
			أرجع 0;
		}
		bitpos++;
	}

	أرجع ENOENT;
}

/* Find the first one bit between start and end, inclusive. */
ساكن errcode_t ba_پیدا_کن_اولی_مجموعه(ext2fs_generic_bitmap_64 bitmap,
				    __u64 شروع, __u64 انتها, __u64 *بیرون)
{
	ext2fs_ba_خصوصی bp = (ext2fs_ba_خصوصی)bitmap->خاص;
	بیعلامت دراز bitpos = شروع - bitmap->شروع;
	بیعلامت دراز شمار = انتها - شروع + 1;
	صحيح byte_یافت = 0; /* whether a != 0xff byte has been found */
	ثابت بیعلامت حرف *pos;
	بیعلامت دراز max_loop_شمار, i;

	/* scan bits until we hit a byte boundary */
	طالما ((bitpos & 0x7) != 0 && شمار > 0) {
		إذا (ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*بیرون = bitpos + bitmap->شروع;
			أرجع 0;
		}
		bitpos++;
		شمار--;
	}

	إذا (!شمار)
		أرجع ENOENT;

	pos = ((بیعلامت حرف *)bp->bitarray) + (bitpos >> 3);
	/* scan bytes until 8-byte (64-bit) aligned */
	طالما (شمار >= 8 && (((uintptr_t)pos) & 0x07)) {
		إذا (*pos != 0) {
			byte_یافت = 1;
			اكسر;
		}
		pos++;
		شمار -= 8;
		bitpos += 8;
	}

	إذا (!byte_یافت) {
		max_loop_شمار = شمار >> 6; /* 8-byte blocks */
		i = max_loop_شمار;
		طالما (i) {
			إذا (*((ثابت __u64 *)pos) != 0)
				اكسر;
			pos += 8;
			i--;
		}
		شمار -= 64 * (max_loop_شمار - i);
		bitpos += 64 * (max_loop_شمار - i);

		max_loop_شمار = شمار >> 3;
		i = max_loop_شمار;
		طالما (i) {
			إذا (*pos != 0) {
				byte_یافت = 1;
				اكسر;
			}
			pos++;
			i--;
		}
		شمار -= 8 * (max_loop_شمار - i);
		bitpos += 8 * (max_loop_شمار - i);
	}

	/* Here either count < 8 or byte_found == 1. */
	طالما (شمار-- > 0) {
		إذا (ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*بیرون = bitpos + bitmap->شروع;
			أرجع 0;
		}
		bitpos++;
	}

	أرجع ENOENT;
}

بنية ext2_bitmap_ops ext2fs_blkmap64_bitarray = {
	.type = EXT2FS_BMAP64_BITARRAY,
	.new_bmap = ba_new_bmap,
	.free_bmap = ba_free_bmap,
	.copy_bmap = ba_copy_bmap,
	.resize_bmap = ba_resize_bmap,
	.mark_bmap = ba_mark_bmap,
	.unmark_bmap = ba_unmark_bmap,
	.test_bmap = ba_test_bmap,
	.test_clear_bmap_extent = ba_test_clear_bmap_extent,
	.mark_bmap_extent = ba_mark_bmap_extent,
	.unmark_bmap_extent = ba_unmark_bmap_extent,
	.مجموعه_bmap_range = ba_مجموعه_bmap_range,
	.get_bmap_range = ba_get_bmap_range,
	.clear_bmap = ba_clear_bmap,
	.print_stats = ba_print_stats,
	.پیدا_کن_اولی_zero = ba_پیدا_کن_اولی_zero,
	.پیدا_کن_اولی_مجموعه = ba_پیدا_کن_اولی_مجموعه
};
