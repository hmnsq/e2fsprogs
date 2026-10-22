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
#ضمن "bmap64.h"

/*
 * Private data for bit array implementation of bitmap ops.
 * Currently, this is just a pointer to our big flat hunk of memory,
 * exactly equivalent to the old-skool char * bitmap member.
 */

بنية ext2fs_ba_ذاتي_struct {
	حرف *bitarray;
};

تعريفنوع بنية ext2fs_ba_ذاتي_struct *ext2fs_ba_ذاتي;

ساكن errcode_t ba_alloc_ذاتي_معلومات (ext2fs_generic_bitmap_64 bitmap)
{
	ext2fs_ba_ذاتي bp;
	errcode_t	retval;
	size_t		سائيز;

	/*
	 * Since we only have the one pointer, we could just shove our
	 * private data in the void *private field itself, but then
	 * we'd have to do a fair bit of rewriting if we ever added a
	 * field.  I'm agnostic.
	 */
	retval = ext2fs_get_mem(حجم (ext2fs_ba_ذاتي), &bp);
	إذا (retval)
		أرجع retval;

	سائيز = (size_t) (((bitmap->real_پڇاڙي - bitmap->شروعات) / 8) + 1);

	retval = ext2fs_get_mem(سائيز, &bp->bitarray);
	إذا (retval) {
		ext2fs_free_mem(&bp);
		bp = 0;
		أرجع retval;
	}
	bitmap->خاص = (فراغ *) bp;
	أرجع 0;
}

ساكن errcode_t ba_new_bmap(ext2_filsys fs EXT2FS_ATTR((اڻ_استعمال_ٿيل)),
			     ext2fs_generic_bitmap_64 bitmap)
{
	ext2fs_ba_ذاتي bp;
	errcode_t	retval;
	size_t		سائيز;

	retval = ba_alloc_ذاتي_معلومات (bitmap);
	إذا (retval)
		أرجع retval;

	bp = (ext2fs_ba_ذاتي) bitmap->خاص;
	سائيز = (size_t) (((bitmap->real_پڇاڙي - bitmap->شروعات) / 8) + 1);
	memset(bp->bitarray, 0, سائيز);

	أرجع 0;
}

ساكن فراغ ba_free_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	ext2fs_ba_ذاتي bp = (ext2fs_ba_ذاتي) bitmap->خاص;

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
	ext2fs_ba_ذاتي src_bp = (ext2fs_ba_ذاتي) src->خاص;
	ext2fs_ba_ذاتي dest_bp;
	errcode_t retval;
	size_t سائيز;

	retval = ba_alloc_ذاتي_معلومات (dest);
	إذا (retval)
		أرجع retval;

	dest_bp = (ext2fs_ba_ذاتي) dest->خاص;

	سائيز = (size_t) (((src->real_پڇاڙي - src->شروعات) / 8) + 1);
	يادگيري_نقل (dest_bp->bitarray, src_bp->bitarray, سائيز);

	أرجع 0;
}

ساكن errcode_t ba_resize_bmap(ext2fs_generic_bitmap_64 bmap,
				__u64 new_پڇاڙي, __u64 new_real_پڇاڙي)
{
	ext2fs_ba_ذاتي bp = (ext2fs_ba_ذاتي) bmap->خاص;
	errcode_t	retval;
	size_t		سائيز, new_سائيز;
	__u64		bitno;

	/*
	 * If we're expanding the bitmap, make sure all of the new
	 * parts of the bitmap are zero.
	 */
	إذا (new_پڇاڙي > bmap->پڇاڙي) {
		bitno = bmap->real_پڇاڙي;
		إذا (bitno > new_پڇاڙي)
			bitno = new_پڇاڙي;
		لكل (; bitno > bmap->پڇاڙي; bitno--)
			ext2fs_clear_bit64(bitno - bmap->شروعات, bp->bitarray);
	}
	إذا (new_real_پڇاڙي == bmap->real_پڇاڙي) {
		bmap->پڇاڙي = new_پڇاڙي;
		أرجع 0;
	}

	سائيز = ((bmap->real_پڇاڙي - bmap->شروعات) / 8) + 1;
	new_سائيز = ((new_real_پڇاڙي - bmap->شروعات) / 8) + 1;

	إذا (سائيز != new_سائيز) {
		retval = ext2fs_resize_mem(سائيز, new_سائيز, &bp->bitarray);
		إذا (retval)
			أرجع retval;
	}
	إذا (new_سائيز > سائيز)
		memset(bp->bitarray + سائيز, 0, new_سائيز - سائيز);

	bmap->پڇاڙي = new_پڇاڙي;
	bmap->real_پڇاڙي = new_real_پڇاڙي;
	أرجع 0;

}

ساكن صحيح ba_mark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ext2fs_ba_ذاتي bp = (ext2fs_ba_ذاتي) bitmap->خاص;
	blk64_t bitno = (blk64_t) arg;

	أرجع ext2fs_سيٽ_bit64(bitno - bitmap->شروعات, bp->bitarray);
}

ساكن صحيح ba_unmark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ext2fs_ba_ذاتي bp = (ext2fs_ba_ذاتي) bitmap->خاص;
	blk64_t bitno = (blk64_t) arg;

	أرجع ext2fs_clear_bit64(bitno - bitmap->شروعات, bp->bitarray);
}

ساكن صحيح ba_test_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ext2fs_ba_ذاتي bp = (ext2fs_ba_ذاتي) bitmap->خاص;
	blk64_t bitno = (blk64_t) arg;

	أرجع ext2fs_test_bit64(bitno - bitmap->شروعات, bp->bitarray);
}

ساكن فراغ ba_mark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				بنانشان صحيح انگ)
{
	ext2fs_ba_ذاتي bp = (ext2fs_ba_ذاتي) bitmap->خاص;
	blk64_t bitno = (blk64_t) arg;
	بنانشان صحيح i;

	لكل (i = 0; i < انگ; i++)
		ext2fs_fast_سيٽ_bit64(bitno + i - bitmap->شروعات, bp->bitarray);
}

ساكن فراغ ba_unmark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				  بنانشان صحيح انگ)
{
	ext2fs_ba_ذاتي bp = (ext2fs_ba_ذاتي) bitmap->خاص;
	blk64_t bitno = (blk64_t) arg;
	بنانشان صحيح i;

	لكل (i = 0; i < انگ; i++)
		ext2fs_fast_clear_bit64(bitno + i - bitmap->شروعات, bp->bitarray);
}

ساكن صحيح ba_test_clear_bmap_extent(ext2fs_generic_bitmap_64 bitmap,
				     __u64 شروعات, بنانشان صحيح len)
{
	ext2fs_ba_ذاتي bp = (ext2fs_ba_ذاتي) bitmap->خاص;
	__u64 شروعات_byte, len_byte = len >> 3;
	بنانشان صحيح شروعات_ٻٽو_انگ, len_ٻٽو_انگ = len % 8;
	بنانشان صحيح پهريون_ٻٽو_انگ = 0;
	بنانشان صحيح آخري_ٻٽو_انگ  = 0;
	صحيح mark_ڳڻپ = 0;
	صحيح mark_ٻٽو_انگ = 0;
	صحيح i;
	ثابت حرف *ADDR;

	ADDR = bp->bitarray;
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


ساكن errcode_t ba_سيٽ_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 شروعات, size_t انگ, فراغ *اندريون)
{
	ext2fs_ba_ذاتي bp = (ext2fs_ba_ذاتي) bitmap->خاص;

	يادگيري_نقل (bp->bitarray + (شروعات >> 3), اندريون, (انگ + 7) >> 3);

	أرجع 0;
}

ساكن errcode_t ba_get_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 شروعات, size_t انگ, فراغ *ٻاهريون)
{
	ext2fs_ba_ذاتي bp = (ext2fs_ba_ذاتي) bitmap->خاص;

	يادگيري_نقل (ٻاهريون, bp->bitarray + (شروعات >> 3), (انگ + 7) >> 3);

	أرجع 0;
}

ساكن فراغ ba_clear_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	ext2fs_ba_ذاتي bp = (ext2fs_ba_ذاتي) bitmap->خاص;

	memset(bp->bitarray, 0,
	       (size_t) (((bitmap->real_پڇاڙي - bitmap->شروعات) / 8) + 1));
}

#جيڪڏهن_وصف_ٿيل ENABLE_BMAP_STATS
ساكن فراغ ba_print_stats(ext2fs_generic_bitmap_64 bitmap)
{
	fprintf(stderr, "%16llu Bytes used by bitarray\n", (بنانشان ڊگهو ڊگهو)
		((bitmap->real_پڇاڙي - bitmap->شروعات) >> 3) + 1 +
		حجم(بنية ext2fs_ba_ذاتي_struct));
}
#وإلا
ساكن فراغ ba_print_stats(ext2fs_generic_bitmap_64 bitmap EXT2FS_ATTR((اڻ_استعمال_ٿيل)))
{
}
#جيڪڏهن_پورو

/* Find the first zero bit between start and end, inclusive. */
ساكن errcode_t ba_ڳوليو_پهريون_zero(ext2fs_generic_bitmap_64 bitmap,
				    __u64 شروعات, __u64 پڇاڙي, __u64 *ٻاهريون)
{
	ext2fs_ba_ذاتي bp = (ext2fs_ba_ذاتي)bitmap->خاص;
	بنانشان ڊگهو bitpos = شروعات - bitmap->شروعات;
	بنانشان ڊگهو ڳڻپ = پڇاڙي - شروعات + 1;
	صحيح byte_مليو = 0; /* whether a != 0xff byte has been found */
	ثابت بنانشان حرف *pos;
	بنانشان ڊگهو max_loop_ڳڻپ, i;

	/* scan bits until we hit a byte boundary */
	طالما ((bitpos & 0x7) != 0 && ڳڻپ > 0) {
		إذا (!ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*ٻاهريون = bitpos + bitmap->شروعات;
			أرجع 0;
		}
		bitpos++;
		ڳڻپ--;
	}

	إذا (!ڳڻپ)
		أرجع ENOENT;

	pos = ((بنانشان حرف *)bp->bitarray) + (bitpos >> 3);
	/* scan bytes until 8-byte (64-bit) aligned */
	طالما (ڳڻپ >= 8 && (((uintptr_t)pos) & 0x07)) {
		إذا (*pos != 0xff) {
			byte_مليو = 1;
			اكسر;
		}
		pos++;
		ڳڻپ -= 8;
		bitpos += 8;
	}

	إذا (!byte_مليو) {
		max_loop_ڳڻپ = ڳڻپ >> 6; /* 8-byte blocks */
		i = max_loop_ڳڻپ;
		طالما (i) {
			إذا (*((ثابت __u64 *)pos) != ((__u64)-1))
				اكسر;
			pos += 8;
			i--;
		}
		ڳڻپ -= 64 * (max_loop_ڳڻپ - i);
		bitpos += 64 * (max_loop_ڳڻپ - i);

		max_loop_ڳڻپ = ڳڻپ >> 3;
		i = max_loop_ڳڻپ;
		طالما (i) {
			إذا (*pos != 0xff) {
				byte_مليو = 1;
				اكسر;
			}
			pos++;
			i--;
		}
		ڳڻپ -= 8 * (max_loop_ڳڻپ - i);
		bitpos += 8 * (max_loop_ڳڻپ - i);
	}

	/* Here either count < 8 or byte_found == 1. */
	طالما (ڳڻپ-- > 0) {
		إذا (!ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*ٻاهريون = bitpos + bitmap->شروعات;
			أرجع 0;
		}
		bitpos++;
	}

	أرجع ENOENT;
}

/* Find the first one bit between start and end, inclusive. */
ساكن errcode_t ba_ڳوليو_پهريون_سيٽ(ext2fs_generic_bitmap_64 bitmap,
				    __u64 شروعات, __u64 پڇاڙي, __u64 *ٻاهريون)
{
	ext2fs_ba_ذاتي bp = (ext2fs_ba_ذاتي)bitmap->خاص;
	بنانشان ڊگهو bitpos = شروعات - bitmap->شروعات;
	بنانشان ڊگهو ڳڻپ = پڇاڙي - شروعات + 1;
	صحيح byte_مليو = 0; /* whether a != 0xff byte has been found */
	ثابت بنانشان حرف *pos;
	بنانشان ڊگهو max_loop_ڳڻپ, i;

	/* scan bits until we hit a byte boundary */
	طالما ((bitpos & 0x7) != 0 && ڳڻپ > 0) {
		إذا (ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*ٻاهريون = bitpos + bitmap->شروعات;
			أرجع 0;
		}
		bitpos++;
		ڳڻپ--;
	}

	إذا (!ڳڻپ)
		أرجع ENOENT;

	pos = ((بنانشان حرف *)bp->bitarray) + (bitpos >> 3);
	/* scan bytes until 8-byte (64-bit) aligned */
	طالما (ڳڻپ >= 8 && (((uintptr_t)pos) & 0x07)) {
		إذا (*pos != 0) {
			byte_مليو = 1;
			اكسر;
		}
		pos++;
		ڳڻپ -= 8;
		bitpos += 8;
	}

	إذا (!byte_مليو) {
		max_loop_ڳڻپ = ڳڻپ >> 6; /* 8-byte blocks */
		i = max_loop_ڳڻپ;
		طالما (i) {
			إذا (*((ثابت __u64 *)pos) != 0)
				اكسر;
			pos += 8;
			i--;
		}
		ڳڻپ -= 64 * (max_loop_ڳڻپ - i);
		bitpos += 64 * (max_loop_ڳڻپ - i);

		max_loop_ڳڻپ = ڳڻپ >> 3;
		i = max_loop_ڳڻپ;
		طالما (i) {
			إذا (*pos != 0) {
				byte_مليو = 1;
				اكسر;
			}
			pos++;
			i--;
		}
		ڳڻپ -= 8 * (max_loop_ڳڻپ - i);
		bitpos += 8 * (max_loop_ڳڻپ - i);
	}

	/* Here either count < 8 or byte_found == 1. */
	طالما (ڳڻپ-- > 0) {
		إذا (ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*ٻاهريون = bitpos + bitmap->شروعات;
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
	.سيٽ_bmap_range = ba_سيٽ_bmap_range,
	.get_bmap_range = ba_get_bmap_range,
	.clear_bmap = ba_clear_bmap,
	.print_stats = ba_print_stats,
	.ڳوليو_پهريون_zero = ba_ڳوليو_پهريون_zero,
	.ڳوليو_پهريون_سيٽ = ba_ڳوليو_پهريون_سيٽ
};
