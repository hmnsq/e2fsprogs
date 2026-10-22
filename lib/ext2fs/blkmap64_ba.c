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
#endif
#ضمن <fcntl.h>
#ضمن <time.h>
#إذا HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#endif
#إذا HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#endif

#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"
#ضمن "bmap64.h"

/*
 * Private data for bit array implementation of bitmap ops.
 * Currently, this is just a pointer to our big flat hunk of memory,
 * exactly equivalent to the old-skool char * bitmap member.
 */

بنية ext2fs_ba_private_struct {
	حرف *bitarray;
};

تعريفنوع بنية ext2fs_ba_private_struct *ext2fs_ba_private;

ساكن errcode_t ba_alloc_private_data (ext2fs_generic_bitmap_64 bitmap)
{
	ext2fs_ba_private bp;
	errcode_t	retval;
	size_t		size;

	/*
	 * Since we only have the one pointer, we could just shove our
	 * private data in the void *private field itself, but then
	 * we'd have to do a fair bit of rewriting if we ever added a
	 * field.  I'm agnostic.
	 */
	retval = ext2fs_get_mem(حجم (ext2fs_ba_private), &bp);
	إذا (retval)
		أرجع retval;

	size = (size_t) (((bitmap->real_end - bitmap->start) / 8) + 1);

	retval = ext2fs_get_mem(size, &bp->bitarray);
	إذا (retval) {
		ext2fs_free_mem(&bp);
		bp = 0;
		أرجع retval;
	}
	bitmap->خاص = (فراغ *) bp;
	أرجع 0;
}

ساكن errcode_t ba_new_bmap(ext2_filsys fs EXT2FS_ATTR((unused)),
			     ext2fs_generic_bitmap_64 bitmap)
{
	ext2fs_ba_private bp;
	errcode_t	retval;
	size_t		size;

	retval = ba_alloc_private_data (bitmap);
	إذا (retval)
		أرجع retval;

	bp = (ext2fs_ba_private) bitmap->خاص;
	size = (size_t) (((bitmap->real_end - bitmap->start) / 8) + 1);
	memset(bp->bitarray, 0, size);

	أرجع 0;
}

ساكن فراغ ba_free_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->خاص;

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
	ext2fs_ba_private src_bp = (ext2fs_ba_private) src->خاص;
	ext2fs_ba_private dest_bp;
	errcode_t retval;
	size_t size;

	retval = ba_alloc_private_data (dest);
	إذا (retval)
		أرجع retval;

	dest_bp = (ext2fs_ba_private) dest->خاص;

	size = (size_t) (((src->real_end - src->start) / 8) + 1);
	memcpy (dest_bp->bitarray, src_bp->bitarray, size);

	أرجع 0;
}

ساكن errcode_t ba_resize_bmap(ext2fs_generic_bitmap_64 bmap,
				__u64 new_end, __u64 new_real_end)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bmap->خاص;
	errcode_t	retval;
	size_t		size, new_size;
	__u64		bitno;

	/*
	 * If we're expanding the bitmap, make sure all of the new
	 * parts of the bitmap are zero.
	 */
	إذا (new_end > bmap->end) {
		bitno = bmap->real_end;
		إذا (bitno > new_end)
			bitno = new_end;
		لكل (; bitno > bmap->end; bitno--)
			ext2fs_clear_bit64(bitno - bmap->start, bp->bitarray);
	}
	إذا (new_real_end == bmap->real_end) {
		bmap->end = new_end;
		أرجع 0;
	}

	size = ((bmap->real_end - bmap->start) / 8) + 1;
	new_size = ((new_real_end - bmap->start) / 8) + 1;

	إذا (size != new_size) {
		retval = ext2fs_resize_mem(size, new_size, &bp->bitarray);
		إذا (retval)
			أرجع retval;
	}
	إذا (new_size > size)
		memset(bp->bitarray + size, 0, new_size - size);

	bmap->end = new_end;
	bmap->real_end = new_real_end;
	أرجع 0;

}

ساكن صحيح ba_mark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->خاص;
	blk64_t bitno = (blk64_t) arg;

	أرجع ext2fs_set_bit64(bitno - bitmap->start, bp->bitarray);
}

ساكن صحيح ba_unmark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->خاص;
	blk64_t bitno = (blk64_t) arg;

	أرجع ext2fs_clear_bit64(bitno - bitmap->start, bp->bitarray);
}

ساكن صحيح ba_test_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->خاص;
	blk64_t bitno = (blk64_t) arg;

	أرجع ext2fs_test_bit64(bitno - bitmap->start, bp->bitarray);
}

ساكن فراغ ba_mark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				unsigned صحيح رەقەم)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->خاص;
	blk64_t bitno = (blk64_t) arg;
	unsigned صحيح i;

	لكل (i = 0; i < رەقەم; i++)
		ext2fs_fast_set_bit64(bitno + i - bitmap->start, bp->bitarray);
}

ساكن فراغ ba_unmark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				  unsigned صحيح رەقەم)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->خاص;
	blk64_t bitno = (blk64_t) arg;
	unsigned صحيح i;

	لكل (i = 0; i < رەقەم; i++)
		ext2fs_fast_clear_bit64(bitno + i - bitmap->start, bp->bitarray);
}

ساكن صحيح ba_test_clear_bmap_extent(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, unsigned صحيح len)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->خاص;
	__u64 start_byte, len_byte = len >> 3;
	unsigned صحيح start_bit, len_bit = len % 8;
	unsigned صحيح first_bit = 0;
	unsigned صحيح last_bit  = 0;
	صحيح mark_سانى = 0;
	صحيح mark_bit = 0;
	صحيح i;
	ثابت حرف *ADDR;

	ADDR = bp->bitarray;
	start -= bitmap->start;
	start_byte = start >> 3;
	start_bit = start % 8;

	إذا (start_bit != 0) {
		/*
		 * The compared start block number or start inode number
		 * is not the first bit in a byte.
		 */
		mark_سانى = 8 - start_bit;
		إذا (len < 8 - start_bit) {
			mark_سانى = (صحيح)len;
			mark_bit = len + start_bit - 1;
		} وإلا
			mark_bit = 7;

		لكل (i = mark_سانى; i > 0; i--, mark_bit--)
			first_bit |= 1 << mark_bit;

		/*
		 * Compare blocks or inodes in the first byte.
		 * If there is any marked bit, this function returns 0.
		 */
		إذا (first_bit & ADDR[start_byte])
			أرجع 0;
		وإلا إذا (len <= 8 - start_bit)
			أرجع 1;

		start_byte++;
		len_bit = (len - mark_سانى) % 8;
		len_byte = (len - mark_سانى) >> 3;
	}

	/*
	 * The compared start block number or start inode number is
	 * the first bit in a byte.
	 */
	إذا (len_bit != 0) {
		/*
		 * The compared end block number or end inode number is
		 * not the last bit in a byte.
		 */
		لكل (mark_bit = len_bit - 1; mark_bit >= 0; mark_bit--)
			last_bit |= 1 << mark_bit;

		/*
		 * Compare blocks or inodes in the last byte.
		 * If there is any marked bit, this function returns 0.
		 */
		إذا (last_bit & ADDR[start_byte + len_byte])
			أرجع 0;
		وإلا إذا (len_byte == 0)
			أرجع 1;
	}

	/* Check whether all bytes are 0 */
	أرجع ext2fs_mem_is_zero(ADDR + start_byte, len_byte);
}


ساكن errcode_t ba_set_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, size_t رەقەم, فراغ *in)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->خاص;

	memcpy (bp->bitarray + (start >> 3), in, (رەقەم + 7) >> 3);

	أرجع 0;
}

ساكن errcode_t ba_get_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, size_t رەقەم, فراغ *out)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->خاص;

	memcpy (out, bp->bitarray + (start >> 3), (رەقەم + 7) >> 3);

	أرجع 0;
}

ساكن فراغ ba_clear_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->خاص;

	memset(bp->bitarray, 0,
	       (size_t) (((bitmap->real_end - bitmap->start) / 8) + 1));
}

#ifdef ENABLE_BMAP_STATS
ساكن فراغ ba_print_stats(ext2fs_generic_bitmap_64 bitmap)
{
	fprintf(stderr, "%16llu Bytes used by bitarray\n", (unsigned long long)
		((bitmap->real_end - bitmap->start) >> 3) + 1 +
		حجم(بنية ext2fs_ba_private_struct));
}
#وإلا
ساكن فراغ ba_print_stats(ext2fs_generic_bitmap_64 bitmap EXT2FS_ATTR((unused)))
{
}
#endif

/* Find the first zero bit between start and end, inclusive. */
ساكن errcode_t ba_find_first_zero(ext2fs_generic_bitmap_64 bitmap,
				    __u64 start, __u64 end, __u64 *out)
{
	ext2fs_ba_private bp = (ext2fs_ba_private)bitmap->خاص;
	unsigned long bitpos = start - bitmap->start;
	unsigned long سانى = end - start + 1;
	صحيح byte_found = 0; /* whether a != 0xff byte has been found */
	ثابت unsigned حرف *pos;
	unsigned long max_loop_سانى, i;

	/* scan bits until we hit a byte boundary */
	طالما ((bitpos & 0x7) != 0 && سانى > 0) {
		إذا (!ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*out = bitpos + bitmap->start;
			أرجع 0;
		}
		bitpos++;
		سانى--;
	}

	إذا (!سانى)
		أرجع ENOENT;

	pos = ((unsigned حرف *)bp->bitarray) + (bitpos >> 3);
	/* scan bytes until 8-byte (64-bit) aligned */
	طالما (سانى >= 8 && (((uintptr_t)pos) & 0x07)) {
		إذا (*pos != 0xff) {
			byte_found = 1;
			اكسر;
		}
		pos++;
		سانى -= 8;
		bitpos += 8;
	}

	إذا (!byte_found) {
		max_loop_سانى = سانى >> 6; /* 8-byte blocks */
		i = max_loop_سانى;
		طالما (i) {
			إذا (*((ثابت __u64 *)pos) != ((__u64)-1))
				اكسر;
			pos += 8;
			i--;
		}
		سانى -= 64 * (max_loop_سانى - i);
		bitpos += 64 * (max_loop_سانى - i);

		max_loop_سانى = سانى >> 3;
		i = max_loop_سانى;
		طالما (i) {
			إذا (*pos != 0xff) {
				byte_found = 1;
				اكسر;
			}
			pos++;
			i--;
		}
		سانى -= 8 * (max_loop_سانى - i);
		bitpos += 8 * (max_loop_سانى - i);
	}

	/* Here either count < 8 or byte_found == 1. */
	طالما (سانى-- > 0) {
		إذا (!ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*out = bitpos + bitmap->start;
			أرجع 0;
		}
		bitpos++;
	}

	أرجع ENOENT;
}

/* Find the first one bit between start and end, inclusive. */
ساكن errcode_t ba_find_first_set(ext2fs_generic_bitmap_64 bitmap,
				    __u64 start, __u64 end, __u64 *out)
{
	ext2fs_ba_private bp = (ext2fs_ba_private)bitmap->خاص;
	unsigned long bitpos = start - bitmap->start;
	unsigned long سانى = end - start + 1;
	صحيح byte_found = 0; /* whether a != 0xff byte has been found */
	ثابت unsigned حرف *pos;
	unsigned long max_loop_سانى, i;

	/* scan bits until we hit a byte boundary */
	طالما ((bitpos & 0x7) != 0 && سانى > 0) {
		إذا (ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*out = bitpos + bitmap->start;
			أرجع 0;
		}
		bitpos++;
		سانى--;
	}

	إذا (!سانى)
		أرجع ENOENT;

	pos = ((unsigned حرف *)bp->bitarray) + (bitpos >> 3);
	/* scan bytes until 8-byte (64-bit) aligned */
	طالما (سانى >= 8 && (((uintptr_t)pos) & 0x07)) {
		إذا (*pos != 0) {
			byte_found = 1;
			اكسر;
		}
		pos++;
		سانى -= 8;
		bitpos += 8;
	}

	إذا (!byte_found) {
		max_loop_سانى = سانى >> 6; /* 8-byte blocks */
		i = max_loop_سانى;
		طالما (i) {
			إذا (*((ثابت __u64 *)pos) != 0)
				اكسر;
			pos += 8;
			i--;
		}
		سانى -= 64 * (max_loop_سانى - i);
		bitpos += 64 * (max_loop_سانى - i);

		max_loop_سانى = سانى >> 3;
		i = max_loop_سانى;
		طالما (i) {
			إذا (*pos != 0) {
				byte_found = 1;
				اكسر;
			}
			pos++;
			i--;
		}
		سانى -= 8 * (max_loop_سانى - i);
		bitpos += 8 * (max_loop_سانى - i);
	}

	/* Here either count < 8 or byte_found == 1. */
	طالما (سانى-- > 0) {
		إذا (ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*out = bitpos + bitmap->start;
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
	.set_bmap_range = ba_set_bmap_range,
	.get_bmap_range = ba_get_bmap_range,
	.clear_bmap = ba_clear_bmap,
	.print_stats = ba_print_stats,
	.find_first_zero = ba_find_first_zero,
	.find_first_set = ba_find_first_set
};
