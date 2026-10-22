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
#ضمن "bmap64.h"

/*
 * Private data for bit array implementation of bitmap ops.
 * Currently, this is just a pointer to our big flat hunk of memory,
 * exactly equivalent to the old-skool char * bitmap member.
 */

بنية ext2fs_ba_نجی_struct {
	حرف *bitarray;
};

تعريفنوع بنية ext2fs_ba_نجی_struct *ext2fs_ba_نجی;

ساكن errcode_t ba_alloc_نجی_انکڑے (ext2fs_generic_bitmap_64 bitmap)
{
	ext2fs_ba_نجی bp;
	errcode_t	retval;
	size_t		ناپ;

	/*
	 * Since we only have the one pointer, we could just shove our
	 * private data in the void *private field itself, but then
	 * we'd have to do a fair bit of rewriting if we ever added a
	 * field.  I'm agnostic.
	 */
	retval = ext2fs_get_mem(حجم (ext2fs_ba_نجی), &bp);
	إذا (retval)
		أرجع retval;

	ناپ = (size_t) (((bitmap->real_سرا - bitmap->شروع) / 8) + 1);

	retval = ext2fs_get_mem(ناپ, &bp->bitarray);
	إذا (retval) {
		ext2fs_free_mem(&bp);
		bp = 0;
		أرجع retval;
	}
	bitmap->خاص = (فراغ *) bp;
	أرجع 0;
}

ساكن errcode_t ba_new_bmap(ext2_filsys fs EXT2FS_ATTR((بن_ورتیا)),
			     ext2fs_generic_bitmap_64 bitmap)
{
	ext2fs_ba_نجی bp;
	errcode_t	retval;
	size_t		ناپ;

	retval = ba_alloc_نجی_انکڑے (bitmap);
	إذا (retval)
		أرجع retval;

	bp = (ext2fs_ba_نجی) bitmap->خاص;
	ناپ = (size_t) (((bitmap->real_سرا - bitmap->شروع) / 8) + 1);
	memset(bp->bitarray, 0, ناپ);

	أرجع 0;
}

ساكن فراغ ba_free_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	ext2fs_ba_نجی bp = (ext2fs_ba_نجی) bitmap->خاص;

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
	ext2fs_ba_نجی src_bp = (ext2fs_ba_نجی) src->خاص;
	ext2fs_ba_نجی dest_bp;
	errcode_t retval;
	size_t ناپ;

	retval = ba_alloc_نجی_انکڑے (dest);
	إذا (retval)
		أرجع retval;

	dest_bp = (ext2fs_ba_نجی) dest->خاص;

	ناپ = (size_t) (((src->real_سرا - src->شروع) / 8) + 1);
	حافظہ_نقل (dest_bp->bitarray, src_bp->bitarray, ناپ);

	أرجع 0;
}

ساكن errcode_t ba_resize_bmap(ext2fs_generic_bitmap_64 bmap,
				__u64 new_سرا, __u64 new_real_سرا)
{
	ext2fs_ba_نجی bp = (ext2fs_ba_نجی) bmap->خاص;
	errcode_t	retval;
	size_t		ناپ, new_ناپ;
	__u64		bitno;

	/*
	 * If we're expanding the bitmap, make sure all of the new
	 * parts of the bitmap are zero.
	 */
	إذا (new_سرا > bmap->سرا) {
		bitno = bmap->real_سرا;
		إذا (bitno > new_سرا)
			bitno = new_سرا;
		لكل (; bitno > bmap->سرا; bitno--)
			ext2fs_clear_bit64(bitno - bmap->شروع, bp->bitarray);
	}
	إذا (new_real_سرا == bmap->real_سرا) {
		bmap->سرا = new_سرا;
		أرجع 0;
	}

	ناپ = ((bmap->real_سرا - bmap->شروع) / 8) + 1;
	new_ناپ = ((new_real_سرا - bmap->شروع) / 8) + 1;

	إذا (ناپ != new_ناپ) {
		retval = ext2fs_resize_mem(ناپ, new_ناپ, &bp->bitarray);
		إذا (retval)
			أرجع retval;
	}
	إذا (new_ناپ > ناپ)
		memset(bp->bitarray + ناپ, 0, new_ناپ - ناپ);

	bmap->سرا = new_سرا;
	bmap->real_سرا = new_real_سرا;
	أرجع 0;

}

ساكن صحيح ba_mark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ext2fs_ba_نجی bp = (ext2fs_ba_نجی) bitmap->خاص;
	blk64_t bitno = (blk64_t) arg;

	أرجع ext2fs_سیٹ_bit64(bitno - bitmap->شروع, bp->bitarray);
}

ساكن صحيح ba_unmark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ext2fs_ba_نجی bp = (ext2fs_ba_نجی) bitmap->خاص;
	blk64_t bitno = (blk64_t) arg;

	أرجع ext2fs_clear_bit64(bitno - bitmap->شروع, bp->bitarray);
}

ساكن صحيح ba_test_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ext2fs_ba_نجی bp = (ext2fs_ba_نجی) bitmap->خاص;
	blk64_t bitno = (blk64_t) arg;

	أرجع ext2fs_test_bit64(bitno - bitmap->شروع, bp->bitarray);
}

ساكن فراغ ba_mark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				بنانشانی صحيح عدد)
{
	ext2fs_ba_نجی bp = (ext2fs_ba_نجی) bitmap->خاص;
	blk64_t bitno = (blk64_t) arg;
	بنانشانی صحيح i;

	لكل (i = 0; i < عدد; i++)
		ext2fs_fast_سیٹ_bit64(bitno + i - bitmap->شروع, bp->bitarray);
}

ساكن فراغ ba_unmark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				  بنانشانی صحيح عدد)
{
	ext2fs_ba_نجی bp = (ext2fs_ba_نجی) bitmap->خاص;
	blk64_t bitno = (blk64_t) arg;
	بنانشانی صحيح i;

	لكل (i = 0; i < عدد; i++)
		ext2fs_fast_clear_bit64(bitno + i - bitmap->شروع, bp->bitarray);
}

ساكن صحيح ba_test_clear_bmap_extent(ext2fs_generic_bitmap_64 bitmap,
				     __u64 شروع, بنانشانی صحيح len)
{
	ext2fs_ba_نجی bp = (ext2fs_ba_نجی) bitmap->خاص;
	__u64 شروع_byte, len_byte = len >> 3;
	بنانشانی صحيح شروع_ثنائی_ہندسہ, len_ثنائی_ہندسہ = len % 8;
	بنانشانی صحيح پہلا_ثنائی_ہندسہ = 0;
	بنانشانی صحيح آخری_ثنائی_ہندسہ  = 0;
	صحيح mark_گنتی = 0;
	صحيح mark_ثنائی_ہندسہ = 0;
	صحيح i;
	ثابت حرف *ADDR;

	ADDR = bp->bitarray;
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


ساكن errcode_t ba_سیٹ_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 شروع, size_t عدد, فراغ *وچ)
{
	ext2fs_ba_نجی bp = (ext2fs_ba_نجی) bitmap->خاص;

	حافظہ_نقل (bp->bitarray + (شروع >> 3), وچ, (عدد + 7) >> 3);

	أرجع 0;
}

ساكن errcode_t ba_get_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 شروع, size_t عدد, فراغ *باہرلا)
{
	ext2fs_ba_نجی bp = (ext2fs_ba_نجی) bitmap->خاص;

	حافظہ_نقل (باہرلا, bp->bitarray + (شروع >> 3), (عدد + 7) >> 3);

	أرجع 0;
}

ساكن فراغ ba_clear_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	ext2fs_ba_نجی bp = (ext2fs_ba_نجی) bitmap->خاص;

	memset(bp->bitarray, 0,
	       (size_t) (((bitmap->real_سرا - bitmap->شروع) / 8) + 1));
}

#جے_تعریف_شدہ ENABLE_BMAP_STATS
ساكن فراغ ba_print_stats(ext2fs_generic_bitmap_64 bitmap)
{
	fprintf(stderr, "%16llu Bytes used by bitarray\n", (بنانشانی لما لما)
		((bitmap->real_سرا - bitmap->شروع) >> 3) + 1 +
		حجم(بنية ext2fs_ba_نجی_struct));
}
#وإلا
ساكن فراغ ba_print_stats(ext2fs_generic_bitmap_64 bitmap EXT2FS_ATTR((بن_ورتیا)))
{
}
#جے_انت

/* Find the first zero bit between start and end, inclusive. */
ساكن errcode_t ba_لبھو_پہلا_zero(ext2fs_generic_bitmap_64 bitmap,
				    __u64 شروع, __u64 سرا, __u64 *باہرلا)
{
	ext2fs_ba_نجی bp = (ext2fs_ba_نجی)bitmap->خاص;
	بنانشانی لما bitpos = شروع - bitmap->شروع;
	بنانشانی لما گنتی = سرا - شروع + 1;
	صحيح byte_لبھیا = 0; /* whether a != 0xff byte has been found */
	ثابت بنانشانی حرف *pos;
	بنانشانی لما max_loop_گنتی, i;

	/* scan bits until we hit a byte boundary */
	طالما ((bitpos & 0x7) != 0 && گنتی > 0) {
		إذا (!ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*باہرلا = bitpos + bitmap->شروع;
			أرجع 0;
		}
		bitpos++;
		گنتی--;
	}

	إذا (!گنتی)
		أرجع ENOENT;

	pos = ((بنانشانی حرف *)bp->bitarray) + (bitpos >> 3);
	/* scan bytes until 8-byte (64-bit) aligned */
	طالما (گنتی >= 8 && (((uintptr_t)pos) & 0x07)) {
		إذا (*pos != 0xff) {
			byte_لبھیا = 1;
			اكسر;
		}
		pos++;
		گنتی -= 8;
		bitpos += 8;
	}

	إذا (!byte_لبھیا) {
		max_loop_گنتی = گنتی >> 6; /* 8-byte blocks */
		i = max_loop_گنتی;
		طالما (i) {
			إذا (*((ثابت __u64 *)pos) != ((__u64)-1))
				اكسر;
			pos += 8;
			i--;
		}
		گنتی -= 64 * (max_loop_گنتی - i);
		bitpos += 64 * (max_loop_گنتی - i);

		max_loop_گنتی = گنتی >> 3;
		i = max_loop_گنتی;
		طالما (i) {
			إذا (*pos != 0xff) {
				byte_لبھیا = 1;
				اكسر;
			}
			pos++;
			i--;
		}
		گنتی -= 8 * (max_loop_گنتی - i);
		bitpos += 8 * (max_loop_گنتی - i);
	}

	/* Here either count < 8 or byte_found == 1. */
	طالما (گنتی-- > 0) {
		إذا (!ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*باہرلا = bitpos + bitmap->شروع;
			أرجع 0;
		}
		bitpos++;
	}

	أرجع ENOENT;
}

/* Find the first one bit between start and end, inclusive. */
ساكن errcode_t ba_لبھو_پہلا_سیٹ(ext2fs_generic_bitmap_64 bitmap,
				    __u64 شروع, __u64 سرا, __u64 *باہرلا)
{
	ext2fs_ba_نجی bp = (ext2fs_ba_نجی)bitmap->خاص;
	بنانشانی لما bitpos = شروع - bitmap->شروع;
	بنانشانی لما گنتی = سرا - شروع + 1;
	صحيح byte_لبھیا = 0; /* whether a != 0xff byte has been found */
	ثابت بنانشانی حرف *pos;
	بنانشانی لما max_loop_گنتی, i;

	/* scan bits until we hit a byte boundary */
	طالما ((bitpos & 0x7) != 0 && گنتی > 0) {
		إذا (ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*باہرلا = bitpos + bitmap->شروع;
			أرجع 0;
		}
		bitpos++;
		گنتی--;
	}

	إذا (!گنتی)
		أرجع ENOENT;

	pos = ((بنانشانی حرف *)bp->bitarray) + (bitpos >> 3);
	/* scan bytes until 8-byte (64-bit) aligned */
	طالما (گنتی >= 8 && (((uintptr_t)pos) & 0x07)) {
		إذا (*pos != 0) {
			byte_لبھیا = 1;
			اكسر;
		}
		pos++;
		گنتی -= 8;
		bitpos += 8;
	}

	إذا (!byte_لبھیا) {
		max_loop_گنتی = گنتی >> 6; /* 8-byte blocks */
		i = max_loop_گنتی;
		طالما (i) {
			إذا (*((ثابت __u64 *)pos) != 0)
				اكسر;
			pos += 8;
			i--;
		}
		گنتی -= 64 * (max_loop_گنتی - i);
		bitpos += 64 * (max_loop_گنتی - i);

		max_loop_گنتی = گنتی >> 3;
		i = max_loop_گنتی;
		طالما (i) {
			إذا (*pos != 0) {
				byte_لبھیا = 1;
				اكسر;
			}
			pos++;
			i--;
		}
		گنتی -= 8 * (max_loop_گنتی - i);
		bitpos += 8 * (max_loop_گنتی - i);
	}

	/* Here either count < 8 or byte_found == 1. */
	طالما (گنتی-- > 0) {
		إذا (ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*باہرلا = bitpos + bitmap->شروع;
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
	.سیٹ_bmap_range = ba_سیٹ_bmap_range,
	.get_bmap_range = ba_get_bmap_range,
	.clear_bmap = ba_clear_bmap,
	.print_stats = ba_print_stats,
	.لبھو_پہلا_zero = ba_لبھو_پہلا_zero,
	.لبھو_پہلا_سیٹ = ba_لبھو_پہلا_سیٹ
};
