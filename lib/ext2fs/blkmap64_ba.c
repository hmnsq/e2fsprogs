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

#অন্তর্ভাব "config.h"
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <string.h>
#যদি HAVE_UNISTD_H
#অন্তর্ভাব <unistd.h>
#endif
#অন্তর্ভাব <fcntl.h>
#অন্তর্ভাব <time.h>
#যদি HAVE_SYS_STAT_H
#অন্তর্ভাব <sys/stat.h>
#endif
#যদি HAVE_SYS_TYPES_H
#অন্তর্ভাব <sys/types.h>
#endif

#অন্তর্ভাব "ext2_fs.h"
#অন্তর্ভাব "ext2fsP.h"
#অন্তর্ভাব "bmap64.h"

/*
 * Private data for bit array implementation of bitmap ops.
 * Currently, this is just a pointer to our big flat hunk of memory,
 * exactly equivalent to the old-skool char * bitmap member.
 */

সংরচনা ext2fs_ba_private_struct {
	বর্ণ *bitarray;
};

প্রকারনির্দেশ সংরচনা ext2fs_ba_private_struct *ext2fs_ba_private;

স্থির errcode_t ba_alloc_private_data (ext2fs_generic_bitmap_64 bitmap)
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
	retval = ext2fs_get_mem(পরিমাণ (ext2fs_ba_private), &bp);
	যদি (retval)
		প্রত্যাগম retval;

	size = (size_t) (((bitmap->real_end - bitmap->start) / 8) + 1);

	retval = ext2fs_get_mem(size, &bp->bitarray);
	যদি (retval) {
		ext2fs_free_mem(&bp);
		bp = 0;
		প্রত্যাগম retval;
	}
	bitmap->গুপ্ত = (শূন্য *) bp;
	প্রত্যাগম 0;
}

স্থির errcode_t ba_new_bmap(ext2_filsys fs EXT2FS_ATTR((unused)),
			     ext2fs_generic_bitmap_64 bitmap)
{
	ext2fs_ba_private bp;
	errcode_t	retval;
	size_t		size;

	retval = ba_alloc_private_data (bitmap);
	যদি (retval)
		প্রত্যাগম retval;

	bp = (ext2fs_ba_private) bitmap->গুপ্ত;
	size = (size_t) (((bitmap->real_end - bitmap->start) / 8) + 1);
	memset(bp->bitarray, 0, size);

	প্রত্যাগম 0;
}

স্থির শূন্য ba_free_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->গুপ্ত;

	যদি (!bp)
		প্রত্যাগম;

	যদি (bp->bitarray) {
		ext2fs_free_mem (&bp->bitarray);
		bp->bitarray = 0;
	}
	ext2fs_free_mem (&bp);
	bp = 0;
}

স্থির errcode_t ba_copy_bmap(ext2fs_generic_bitmap_64 src,
			      ext2fs_generic_bitmap_64 dest)
{
	ext2fs_ba_private src_bp = (ext2fs_ba_private) src->গুপ্ত;
	ext2fs_ba_private dest_bp;
	errcode_t retval;
	size_t size;

	retval = ba_alloc_private_data (dest);
	যদি (retval)
		প্রত্যাগম retval;

	dest_bp = (ext2fs_ba_private) dest->গুপ্ত;

	size = (size_t) (((src->real_end - src->start) / 8) + 1);
	memcpy (dest_bp->bitarray, src_bp->bitarray, size);

	প্রত্যাগম 0;
}

স্থির errcode_t ba_resize_bmap(ext2fs_generic_bitmap_64 bmap,
				__u64 new_end, __u64 new_real_end)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bmap->গুপ্ত;
	errcode_t	retval;
	size_t		size, new_size;
	__u64		bitno;

	/*
	 * If we're expanding the bitmap, make sure all of the new
	 * parts of the bitmap are zero.
	 */
	যদি (new_end > bmap->end) {
		bitno = bmap->real_end;
		যদি (bitno > new_end)
			bitno = new_end;
		প্রতি (; bitno > bmap->end; bitno--)
			ext2fs_clear_bit64(bitno - bmap->start, bp->bitarray);
	}
	যদি (new_real_end == bmap->real_end) {
		bmap->end = new_end;
		প্রত্যাগম 0;
	}

	size = ((bmap->real_end - bmap->start) / 8) + 1;
	new_size = ((new_real_end - bmap->start) / 8) + 1;

	যদি (size != new_size) {
		retval = ext2fs_resize_mem(size, new_size, &bp->bitarray);
		যদি (retval)
			প্রত্যাগম retval;
	}
	যদি (new_size > size)
		memset(bp->bitarray + size, 0, new_size - size);

	bmap->end = new_end;
	bmap->real_end = new_real_end;
	প্রত্যাগম 0;

}

স্থির পূর্ণাঙ্ক ba_mark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->গুপ্ত;
	blk64_t bitno = (blk64_t) arg;

	প্রত্যাগম ext2fs_set_bit64(bitno - bitmap->start, bp->bitarray);
}

স্থির পূর্ণাঙ্ক ba_unmark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->গুপ্ত;
	blk64_t bitno = (blk64_t) arg;

	প্রত্যাগম ext2fs_clear_bit64(bitno - bitmap->start, bp->bitarray);
}

স্থির পূর্ণাঙ্ক ba_test_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->গুপ্ত;
	blk64_t bitno = (blk64_t) arg;

	প্রত্যাগম ext2fs_test_bit64(bitno - bitmap->start, bp->bitarray);
}

স্থির শূন্য ba_mark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				unsigned পূর্ণাঙ্ক অঙ্ক)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->গুপ্ত;
	blk64_t bitno = (blk64_t) arg;
	unsigned পূর্ণাঙ্ক i;

	প্রতি (i = 0; i < অঙ্ক; i++)
		ext2fs_fast_set_bit64(bitno + i - bitmap->start, bp->bitarray);
}

স্থির শূন্য ba_unmark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				  unsigned পূর্ণাঙ্ক অঙ্ক)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->গুপ্ত;
	blk64_t bitno = (blk64_t) arg;
	unsigned পূর্ণাঙ্ক i;

	প্রতি (i = 0; i < অঙ্ক; i++)
		ext2fs_fast_clear_bit64(bitno + i - bitmap->start, bp->bitarray);
}

স্থির পূর্ণাঙ্ক ba_test_clear_bmap_extent(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, unsigned পূর্ণাঙ্ক len)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->গুপ্ত;
	__u64 start_byte, len_byte = len >> 3;
	unsigned পূর্ণাঙ্ক start_bit, len_bit = len % 8;
	unsigned পূর্ণাঙ্ক first_bit = 0;
	unsigned পূর্ণাঙ্ক last_bit  = 0;
	পূর্ণাঙ্ক mark_গুনতি = 0;
	পূর্ণাঙ্ক mark_bit = 0;
	পূর্ণাঙ্ক i;
	ধ্রুব বর্ণ *ADDR;

	ADDR = bp->bitarray;
	start -= bitmap->start;
	start_byte = start >> 3;
	start_bit = start % 8;

	যদি (start_bit != 0) {
		/*
		 * The compared start block number or start inode number
		 * is not the first bit in a byte.
		 */
		mark_গুনতি = 8 - start_bit;
		যদি (len < 8 - start_bit) {
			mark_গুনতি = (পূর্ণাঙ্ক)len;
			mark_bit = len + start_bit - 1;
		} অন্যথা
			mark_bit = 7;

		প্রতি (i = mark_গুনতি; i > 0; i--, mark_bit--)
			first_bit |= 1 << mark_bit;

		/*
		 * Compare blocks or inodes in the first byte.
		 * If there is any marked bit, this function returns 0.
		 */
		যদি (first_bit & ADDR[start_byte])
			প্রত্যাগম 0;
		অন্যথা যদি (len <= 8 - start_bit)
			প্রত্যাগম 1;

		start_byte++;
		len_bit = (len - mark_গুনতি) % 8;
		len_byte = (len - mark_গুনতি) >> 3;
	}

	/*
	 * The compared start block number or start inode number is
	 * the first bit in a byte.
	 */
	যদি (len_bit != 0) {
		/*
		 * The compared end block number or end inode number is
		 * not the last bit in a byte.
		 */
		প্রতি (mark_bit = len_bit - 1; mark_bit >= 0; mark_bit--)
			last_bit |= 1 << mark_bit;

		/*
		 * Compare blocks or inodes in the last byte.
		 * If there is any marked bit, this function returns 0.
		 */
		যদি (last_bit & ADDR[start_byte + len_byte])
			প্রত্যাগম 0;
		অন্যথা যদি (len_byte == 0)
			প্রত্যাগম 1;
	}

	/* Check whether all bytes are 0 */
	প্রত্যাগম ext2fs_mem_is_zero(ADDR + start_byte, len_byte);
}


স্থির errcode_t ba_set_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, size_t অঙ্ক, শূন্য *in)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->গুপ্ত;

	memcpy (bp->bitarray + (start >> 3), in, (অঙ্ক + 7) >> 3);

	প্রত্যাগম 0;
}

স্থির errcode_t ba_get_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, size_t অঙ্ক, শূন্য *out)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->গুপ্ত;

	memcpy (out, bp->bitarray + (start >> 3), (অঙ্ক + 7) >> 3);

	প্রত্যাগম 0;
}

স্থির শূন্য ba_clear_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->গুপ্ত;

	memset(bp->bitarray, 0,
	       (size_t) (((bitmap->real_end - bitmap->start) / 8) + 1));
}

#ifdef ENABLE_BMAP_STATS
স্থির শূন্য ba_print_stats(ext2fs_generic_bitmap_64 bitmap)
{
	fprintf(stderr, "%16llu Bytes used by bitarray\n", (unsigned long long)
		((bitmap->real_end - bitmap->start) >> 3) + 1 +
		পরিমাণ(সংরচনা ext2fs_ba_private_struct));
}
#অন্যথা
স্থির শূন্য ba_print_stats(ext2fs_generic_bitmap_64 bitmap EXT2FS_ATTR((unused)))
{
}
#endif

/* Find the first zero bit between start and end, inclusive. */
স্থির errcode_t ba_find_first_zero(ext2fs_generic_bitmap_64 bitmap,
				    __u64 start, __u64 end, __u64 *out)
{
	ext2fs_ba_private bp = (ext2fs_ba_private)bitmap->গুপ্ত;
	unsigned long bitpos = start - bitmap->start;
	unsigned long গুনতি = end - start + 1;
	পূর্ণাঙ্ক byte_found = 0; /* whether a != 0xff byte has been found */
	ধ্রুব unsigned বর্ণ *pos;
	unsigned long max_loop_গুনতি, i;

	/* scan bits until we hit a byte boundary */
	যাবত্ ((bitpos & 0x7) != 0 && গুনতি > 0) {
		যদি (!ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*out = bitpos + bitmap->start;
			প্রত্যাগম 0;
		}
		bitpos++;
		গুনতি--;
	}

	যদি (!গুনতি)
		প্রত্যাগম ENOENT;

	pos = ((unsigned বর্ণ *)bp->bitarray) + (bitpos >> 3);
	/* scan bytes until 8-byte (64-bit) aligned */
	যাবত্ (গুনতি >= 8 && (((uintptr_t)pos) & 0x07)) {
		যদি (*pos != 0xff) {
			byte_found = 1;
			ভঙ্গ;
		}
		pos++;
		গুনতি -= 8;
		bitpos += 8;
	}

	যদি (!byte_found) {
		max_loop_গুনতি = গুনতি >> 6; /* 8-byte blocks */
		i = max_loop_গুনতি;
		যাবত্ (i) {
			যদি (*((ধ্রুব __u64 *)pos) != ((__u64)-1))
				ভঙ্গ;
			pos += 8;
			i--;
		}
		গুনতি -= 64 * (max_loop_গুনতি - i);
		bitpos += 64 * (max_loop_গুনতি - i);

		max_loop_গুনতি = গুনতি >> 3;
		i = max_loop_গুনতি;
		যাবত্ (i) {
			যদি (*pos != 0xff) {
				byte_found = 1;
				ভঙ্গ;
			}
			pos++;
			i--;
		}
		গুনতি -= 8 * (max_loop_গুনতি - i);
		bitpos += 8 * (max_loop_গুনতি - i);
	}

	/* Here either count < 8 or byte_found == 1. */
	যাবত্ (গুনতি-- > 0) {
		যদি (!ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*out = bitpos + bitmap->start;
			প্রত্যাগম 0;
		}
		bitpos++;
	}

	প্রত্যাগম ENOENT;
}

/* Find the first one bit between start and end, inclusive. */
স্থির errcode_t ba_find_first_set(ext2fs_generic_bitmap_64 bitmap,
				    __u64 start, __u64 end, __u64 *out)
{
	ext2fs_ba_private bp = (ext2fs_ba_private)bitmap->গুপ্ত;
	unsigned long bitpos = start - bitmap->start;
	unsigned long গুনতি = end - start + 1;
	পূর্ণাঙ্ক byte_found = 0; /* whether a != 0xff byte has been found */
	ধ্রুব unsigned বর্ণ *pos;
	unsigned long max_loop_গুনতি, i;

	/* scan bits until we hit a byte boundary */
	যাবত্ ((bitpos & 0x7) != 0 && গুনতি > 0) {
		যদি (ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*out = bitpos + bitmap->start;
			প্রত্যাগম 0;
		}
		bitpos++;
		গুনতি--;
	}

	যদি (!গুনতি)
		প্রত্যাগম ENOENT;

	pos = ((unsigned বর্ণ *)bp->bitarray) + (bitpos >> 3);
	/* scan bytes until 8-byte (64-bit) aligned */
	যাবত্ (গুনতি >= 8 && (((uintptr_t)pos) & 0x07)) {
		যদি (*pos != 0) {
			byte_found = 1;
			ভঙ্গ;
		}
		pos++;
		গুনতি -= 8;
		bitpos += 8;
	}

	যদি (!byte_found) {
		max_loop_গুনতি = গুনতি >> 6; /* 8-byte blocks */
		i = max_loop_গুনতি;
		যাবত্ (i) {
			যদি (*((ধ্রুব __u64 *)pos) != 0)
				ভঙ্গ;
			pos += 8;
			i--;
		}
		গুনতি -= 64 * (max_loop_গুনতি - i);
		bitpos += 64 * (max_loop_গুনতি - i);

		max_loop_গুনতি = গুনতি >> 3;
		i = max_loop_গুনতি;
		যাবত্ (i) {
			যদি (*pos != 0) {
				byte_found = 1;
				ভঙ্গ;
			}
			pos++;
			i--;
		}
		গুনতি -= 8 * (max_loop_গুনতি - i);
		bitpos += 8 * (max_loop_গুনতি - i);
	}

	/* Here either count < 8 or byte_found == 1. */
	যাবত্ (গুনতি-- > 0) {
		যদি (ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*out = bitpos + bitmap->start;
			প্রত্যাগম 0;
		}
		bitpos++;
	}

	প্রত্যাগম ENOENT;
}

সংরচনা ext2_bitmap_ops ext2fs_blkmap64_bitarray = {
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
