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
#ضمن "bmap64.h"

/*
 * Private data for bit array implementation of bitmap ops.
 * Currently, this is just a pointer to our big flat hunk of memory,
 * exactly equivalent to the old-skool char * bitmap member.
 */

بنية ext2fs_ba_شخصي_struct {
	حرف *bitarray;
};

تعريفنوع بنية ext2fs_ba_شخصي_struct *ext2fs_ba_شخصي;

ساكن errcode_t ba_alloc_شخصي_معلومات (ext2fs_generic_bitmap_64 bitmap)
{
	ext2fs_ba_شخصي bp;
	errcode_t	retval;
	size_t		اندازه;

	/*
	 * Since we only have the one pointer, we could just shove our
	 * private data in the void *private field itself, but then
	 * we'd have to do a fair bit of rewriting if we ever added a
	 * field.  I'm agnostic.
	 */
	retval = ext2fs_get_mem(حجم (ext2fs_ba_شخصي), &bp);
	إذا (retval)
		أرجع retval;

	اندازه = (size_t) (((bitmap->real_پای_ټکی - bitmap->پیل) / 8) + 1);

	retval = ext2fs_get_mem(اندازه, &bp->bitarray);
	إذا (retval) {
		ext2fs_free_mem(&bp);
		bp = 0;
		أرجع retval;
	}
	bitmap->خاص = (فراغ *) bp;
	أرجع 0;
}

ساكن errcode_t ba_new_bmap(ext2_filsys fs EXT2FS_ATTR((ناکارول_شوی)),
			     ext2fs_generic_bitmap_64 bitmap)
{
	ext2fs_ba_شخصي bp;
	errcode_t	retval;
	size_t		اندازه;

	retval = ba_alloc_شخصي_معلومات (bitmap);
	إذا (retval)
		أرجع retval;

	bp = (ext2fs_ba_شخصي) bitmap->خاص;
	اندازه = (size_t) (((bitmap->real_پای_ټکی - bitmap->پیل) / 8) + 1);
	memset(bp->bitarray, 0, اندازه);

	أرجع 0;
}

ساكن فراغ ba_free_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	ext2fs_ba_شخصي bp = (ext2fs_ba_شخصي) bitmap->خاص;

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
	ext2fs_ba_شخصي src_bp = (ext2fs_ba_شخصي) src->خاص;
	ext2fs_ba_شخصي dest_bp;
	errcode_t retval;
	size_t اندازه;

	retval = ba_alloc_شخصي_معلومات (dest);
	إذا (retval)
		أرجع retval;

	dest_bp = (ext2fs_ba_شخصي) dest->خاص;

	اندازه = (size_t) (((src->real_پای_ټکی - src->پیل) / 8) + 1);
	حافظه_نقل (dest_bp->bitarray, src_bp->bitarray, اندازه);

	أرجع 0;
}

ساكن errcode_t ba_resize_bmap(ext2fs_generic_bitmap_64 bmap,
				__u64 new_پای_ټکی, __u64 new_real_پای_ټکی)
{
	ext2fs_ba_شخصي bp = (ext2fs_ba_شخصي) bmap->خاص;
	errcode_t	retval;
	size_t		اندازه, new_اندازه;
	__u64		bitno;

	/*
	 * If we're expanding the bitmap, make sure all of the new
	 * parts of the bitmap are zero.
	 */
	إذا (new_پای_ټکی > bmap->پای_ټکی) {
		bitno = bmap->real_پای_ټکی;
		إذا (bitno > new_پای_ټکی)
			bitno = new_پای_ټکی;
		لكل (; bitno > bmap->پای_ټکی; bitno--)
			ext2fs_clear_bit64(bitno - bmap->پیل, bp->bitarray);
	}
	إذا (new_real_پای_ټکی == bmap->real_پای_ټکی) {
		bmap->پای_ټکی = new_پای_ټکی;
		أرجع 0;
	}

	اندازه = ((bmap->real_پای_ټکی - bmap->پیل) / 8) + 1;
	new_اندازه = ((new_real_پای_ټکی - bmap->پیل) / 8) + 1;

	إذا (اندازه != new_اندازه) {
		retval = ext2fs_resize_mem(اندازه, new_اندازه, &bp->bitarray);
		إذا (retval)
			أرجع retval;
	}
	إذا (new_اندازه > اندازه)
		memset(bp->bitarray + اندازه, 0, new_اندازه - اندازه);

	bmap->پای_ټکی = new_پای_ټکی;
	bmap->real_پای_ټکی = new_real_پای_ټکی;
	أرجع 0;

}

ساكن صحيح ba_mark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ext2fs_ba_شخصي bp = (ext2fs_ba_شخصي) bitmap->خاص;
	blk64_t bitno = (blk64_t) arg;

	أرجع ext2fs_سېټ_bit64(bitno - bitmap->پیل, bp->bitarray);
}

ساكن صحيح ba_unmark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ext2fs_ba_شخصي bp = (ext2fs_ba_شخصي) bitmap->خاص;
	blk64_t bitno = (blk64_t) arg;

	أرجع ext2fs_clear_bit64(bitno - bitmap->پیل, bp->bitarray);
}

ساكن صحيح ba_test_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ext2fs_ba_شخصي bp = (ext2fs_ba_شخصي) bitmap->خاص;
	blk64_t bitno = (blk64_t) arg;

	أرجع ext2fs_test_bit64(bitno - bitmap->پیل, bp->bitarray);
}

ساكن فراغ ba_mark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				بینښه صحيح عدد)
{
	ext2fs_ba_شخصي bp = (ext2fs_ba_شخصي) bitmap->خاص;
	blk64_t bitno = (blk64_t) arg;
	بینښه صحيح i;

	لكل (i = 0; i < عدد; i++)
		ext2fs_fast_سېټ_bit64(bitno + i - bitmap->پیل, bp->bitarray);
}

ساكن فراغ ba_unmark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				  بینښه صحيح عدد)
{
	ext2fs_ba_شخصي bp = (ext2fs_ba_شخصي) bitmap->خاص;
	blk64_t bitno = (blk64_t) arg;
	بینښه صحيح i;

	لكل (i = 0; i < عدد; i++)
		ext2fs_fast_clear_bit64(bitno + i - bitmap->پیل, bp->bitarray);
}

ساكن صحيح ba_test_clear_bmap_extent(ext2fs_generic_bitmap_64 bitmap,
				     __u64 پیل, بینښه صحيح len)
{
	ext2fs_ba_شخصي bp = (ext2fs_ba_شخصي) bitmap->خاص;
	__u64 پیل_byte, len_byte = len >> 3;
	بینښه صحيح پیل_بټ, len_بټ = len % 8;
	بینښه صحيح لومړی_بټ = 0;
	بینښه صحيح وروستی_بټ  = 0;
	صحيح mark_شمېر = 0;
	صحيح mark_بټ = 0;
	صحيح i;
	ثابت حرف *ADDR;

	ADDR = bp->bitarray;
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


ساكن errcode_t ba_سېټ_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 پیل, size_t عدد, فراغ *دننه)
{
	ext2fs_ba_شخصي bp = (ext2fs_ba_شخصي) bitmap->خاص;

	حافظه_نقل (bp->bitarray + (پیل >> 3), دننه, (عدد + 7) >> 3);

	أرجع 0;
}

ساكن errcode_t ba_get_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 پیل, size_t عدد, فراغ *بهر)
{
	ext2fs_ba_شخصي bp = (ext2fs_ba_شخصي) bitmap->خاص;

	حافظه_نقل (بهر, bp->bitarray + (پیل >> 3), (عدد + 7) >> 3);

	أرجع 0;
}

ساكن فراغ ba_clear_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	ext2fs_ba_شخصي bp = (ext2fs_ba_شخصي) bitmap->خاص;

	memset(bp->bitarray, 0,
	       (size_t) (((bitmap->real_پای_ټکی - bitmap->پیل) / 8) + 1));
}

#که_تعریف_شوی ENABLE_BMAP_STATS
ساكن فراغ ba_print_stats(ext2fs_generic_bitmap_64 bitmap)
{
	fprintf(stderr, "%16llu Bytes used by bitarray\n", (بینښه اوږد اوږد)
		((bitmap->real_پای_ټکی - bitmap->پیل) >> 3) + 1 +
		حجم(بنية ext2fs_ba_شخصي_struct));
}
#وإلا
ساكن فراغ ba_print_stats(ext2fs_generic_bitmap_64 bitmap EXT2FS_ATTR((ناکارول_شوی)))
{
}
#د_که_پای

/* Find the first zero bit between start and end, inclusive. */
ساكن errcode_t ba_پیدا_کړه_لومړی_zero(ext2fs_generic_bitmap_64 bitmap,
				    __u64 پیل, __u64 پای_ټکی, __u64 *بهر)
{
	ext2fs_ba_شخصي bp = (ext2fs_ba_شخصي)bitmap->خاص;
	بینښه اوږد bitpos = پیل - bitmap->پیل;
	بینښه اوږد شمېر = پای_ټکی - پیل + 1;
	صحيح byte_وموندل = 0; /* whether a != 0xff byte has been found */
	ثابت بینښه حرف *pos;
	بینښه اوږد max_loop_شمېر, i;

	/* scan bits until we hit a byte boundary */
	طالما ((bitpos & 0x7) != 0 && شمېر > 0) {
		إذا (!ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*بهر = bitpos + bitmap->پیل;
			أرجع 0;
		}
		bitpos++;
		شمېر--;
	}

	إذا (!شمېر)
		أرجع ENOENT;

	pos = ((بینښه حرف *)bp->bitarray) + (bitpos >> 3);
	/* scan bytes until 8-byte (64-bit) aligned */
	طالما (شمېر >= 8 && (((uintptr_t)pos) & 0x07)) {
		إذا (*pos != 0xff) {
			byte_وموندل = 1;
			اكسر;
		}
		pos++;
		شمېر -= 8;
		bitpos += 8;
	}

	إذا (!byte_وموندل) {
		max_loop_شمېر = شمېر >> 6; /* 8-byte blocks */
		i = max_loop_شمېر;
		طالما (i) {
			إذا (*((ثابت __u64 *)pos) != ((__u64)-1))
				اكسر;
			pos += 8;
			i--;
		}
		شمېر -= 64 * (max_loop_شمېر - i);
		bitpos += 64 * (max_loop_شمېر - i);

		max_loop_شمېر = شمېر >> 3;
		i = max_loop_شمېر;
		طالما (i) {
			إذا (*pos != 0xff) {
				byte_وموندل = 1;
				اكسر;
			}
			pos++;
			i--;
		}
		شمېر -= 8 * (max_loop_شمېر - i);
		bitpos += 8 * (max_loop_شمېر - i);
	}

	/* Here either count < 8 or byte_found == 1. */
	طالما (شمېر-- > 0) {
		إذا (!ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*بهر = bitpos + bitmap->پیل;
			أرجع 0;
		}
		bitpos++;
	}

	أرجع ENOENT;
}

/* Find the first one bit between start and end, inclusive. */
ساكن errcode_t ba_پیدا_کړه_لومړی_سېټ(ext2fs_generic_bitmap_64 bitmap,
				    __u64 پیل, __u64 پای_ټکی, __u64 *بهر)
{
	ext2fs_ba_شخصي bp = (ext2fs_ba_شخصي)bitmap->خاص;
	بینښه اوږد bitpos = پیل - bitmap->پیل;
	بینښه اوږد شمېر = پای_ټکی - پیل + 1;
	صحيح byte_وموندل = 0; /* whether a != 0xff byte has been found */
	ثابت بینښه حرف *pos;
	بینښه اوږد max_loop_شمېر, i;

	/* scan bits until we hit a byte boundary */
	طالما ((bitpos & 0x7) != 0 && شمېر > 0) {
		إذا (ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*بهر = bitpos + bitmap->پیل;
			أرجع 0;
		}
		bitpos++;
		شمېر--;
	}

	إذا (!شمېر)
		أرجع ENOENT;

	pos = ((بینښه حرف *)bp->bitarray) + (bitpos >> 3);
	/* scan bytes until 8-byte (64-bit) aligned */
	طالما (شمېر >= 8 && (((uintptr_t)pos) & 0x07)) {
		إذا (*pos != 0) {
			byte_وموندل = 1;
			اكسر;
		}
		pos++;
		شمېر -= 8;
		bitpos += 8;
	}

	إذا (!byte_وموندل) {
		max_loop_شمېر = شمېر >> 6; /* 8-byte blocks */
		i = max_loop_شمېر;
		طالما (i) {
			إذا (*((ثابت __u64 *)pos) != 0)
				اكسر;
			pos += 8;
			i--;
		}
		شمېر -= 64 * (max_loop_شمېر - i);
		bitpos += 64 * (max_loop_شمېر - i);

		max_loop_شمېر = شمېر >> 3;
		i = max_loop_شمېر;
		طالما (i) {
			إذا (*pos != 0) {
				byte_وموندل = 1;
				اكسر;
			}
			pos++;
			i--;
		}
		شمېر -= 8 * (max_loop_شمېر - i);
		bitpos += 8 * (max_loop_شمېر - i);
	}

	/* Here either count < 8 or byte_found == 1. */
	طالما (شمېر-- > 0) {
		إذا (ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*بهر = bitpos + bitmap->پیل;
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
	.سېټ_bmap_range = ba_سېټ_bmap_range,
	.get_bmap_range = ba_get_bmap_range,
	.clear_bmap = ba_clear_bmap,
	.print_stats = ba_print_stats,
	.پیدا_کړه_لومړی_zero = ba_پیدا_کړه_لومړی_zero,
	.پیدا_کړه_لومړی_سېټ = ba_پیدا_کړه_لومړی_سېټ
};
