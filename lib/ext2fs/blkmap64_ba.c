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

#ಅನ್ತರ್ಭಾವ "config.h"
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಯದಿ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#endif
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <time.h>
#ಯದಿ HAVE_SYS_STAT_H
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#endif
#ಯದಿ HAVE_SYS_TYPES_H
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#endif

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fsP.h"
#ಅನ್ತರ್ಭಾವ "bmap64.h"

/*
 * Private data for bit array implementation of bitmap ops.
 * Currently, this is just a pointer to our big flat hunk of memory,
 * exactly equivalent to the old-skool char * bitmap member.
 */

ಸಂರಚನಾ ext2fs_ba_private_struct {
	ವರ್ಣ *bitarray;
};

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ ext2fs_ba_private_struct *ext2fs_ba_private;

ಸ್ಥಿರ errcode_t ba_alloc_private_data (ext2fs_generic_bitmap_64 bitmap)
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
	retval = ext2fs_get_mem(ಪರಿಮಾಣ (ext2fs_ba_private), &bp);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;

	size = (size_t) (((bitmap->real_end - bitmap->start) / 8) + 1);

	retval = ext2fs_get_mem(size, &bp->bitarray);
	ಯದಿ (retval) {
		ext2fs_free_mem(&bp);
		bp = 0;
		ಪ್ರತ್ಯಾಗಮ retval;
	}
	bitmap->ಗುಪ್ತ = (ಶೂನ್ಯ *) bp;
	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ errcode_t ba_new_bmap(ext2_filsys fs EXT2FS_ATTR((unused)),
			     ext2fs_generic_bitmap_64 bitmap)
{
	ext2fs_ba_private bp;
	errcode_t	retval;
	size_t		size;

	retval = ba_alloc_private_data (bitmap);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;

	bp = (ext2fs_ba_private) bitmap->ಗುಪ್ತ;
	size = (size_t) (((bitmap->real_end - bitmap->start) / 8) + 1);
	memset(bp->bitarray, 0, size);

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ ba_free_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->ಗುಪ್ತ;

	ಯದಿ (!bp)
		ಪ್ರತ್ಯಾಗಮ;

	ಯದಿ (bp->bitarray) {
		ext2fs_free_mem (&bp->bitarray);
		bp->bitarray = 0;
	}
	ext2fs_free_mem (&bp);
	bp = 0;
}

ಸ್ಥಿರ errcode_t ba_copy_bmap(ext2fs_generic_bitmap_64 src,
			      ext2fs_generic_bitmap_64 dest)
{
	ext2fs_ba_private src_bp = (ext2fs_ba_private) src->ಗುಪ್ತ;
	ext2fs_ba_private dest_bp;
	errcode_t retval;
	size_t size;

	retval = ba_alloc_private_data (dest);
	ಯದಿ (retval)
		ಪ್ರತ್ಯಾಗಮ retval;

	dest_bp = (ext2fs_ba_private) dest->ಗುಪ್ತ;

	size = (size_t) (((src->real_end - src->start) / 8) + 1);
	memcpy (dest_bp->bitarray, src_bp->bitarray, size);

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ errcode_t ba_resize_bmap(ext2fs_generic_bitmap_64 bmap,
				__u64 new_end, __u64 new_real_end)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bmap->ಗುಪ್ತ;
	errcode_t	retval;
	size_t		size, new_size;
	__u64		bitno;

	/*
	 * If we're expanding the bitmap, make sure all of the new
	 * parts of the bitmap are zero.
	 */
	ಯದಿ (new_end > bmap->end) {
		bitno = bmap->real_end;
		ಯದಿ (bitno > new_end)
			bitno = new_end;
		ಪ್ರತಿ (; bitno > bmap->end; bitno--)
			ext2fs_clear_bit64(bitno - bmap->start, bp->bitarray);
	}
	ಯದಿ (new_real_end == bmap->real_end) {
		bmap->end = new_end;
		ಪ್ರತ್ಯಾಗಮ 0;
	}

	size = ((bmap->real_end - bmap->start) / 8) + 1;
	new_size = ((new_real_end - bmap->start) / 8) + 1;

	ಯದಿ (size != new_size) {
		retval = ext2fs_resize_mem(size, new_size, &bp->bitarray);
		ಯದಿ (retval)
			ಪ್ರತ್ಯಾಗಮ retval;
	}
	ಯದಿ (new_size > size)
		memset(bp->bitarray + size, 0, new_size - size);

	bmap->end = new_end;
	bmap->real_end = new_real_end;
	ಪ್ರತ್ಯಾಗಮ 0;

}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ ba_mark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->ಗುಪ್ತ;
	blk64_t bitno = (blk64_t) arg;

	ಪ್ರತ್ಯಾಗಮ ext2fs_set_bit64(bitno - bitmap->start, bp->bitarray);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ ba_unmark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->ಗುಪ್ತ;
	blk64_t bitno = (blk64_t) arg;

	ಪ್ರತ್ಯಾಗಮ ext2fs_clear_bit64(bitno - bitmap->start, bp->bitarray);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ ba_test_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->ಗುಪ್ತ;
	blk64_t bitno = (blk64_t) arg;

	ಪ್ರತ್ಯಾಗಮ ext2fs_test_bit64(bitno - bitmap->start, bp->bitarray);
}

ಸ್ಥಿರ ಶೂನ್ಯ ba_mark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->ಗುಪ್ತ;
	blk64_t bitno = (blk64_t) arg;
	unsigned ಪೂರ್ಣಾಙ್ಕ i;

	ಪ್ರತಿ (i = 0; i < ಅಂಕೆ; i++)
		ext2fs_fast_set_bit64(bitno + i - bitmap->start, bp->bitarray);
}

ಸ್ಥಿರ ಶೂನ್ಯ ba_unmark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				  unsigned ಪೂರ್ಣಾಙ್ಕ ಅಂಕೆ)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->ಗುಪ್ತ;
	blk64_t bitno = (blk64_t) arg;
	unsigned ಪೂರ್ಣಾಙ್ಕ i;

	ಪ್ರತಿ (i = 0; i < ಅಂಕೆ; i++)
		ext2fs_fast_clear_bit64(bitno + i - bitmap->start, bp->bitarray);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ ba_test_clear_bmap_extent(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, unsigned ಪೂರ್ಣಾಙ್ಕ len)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->ಗುಪ್ತ;
	__u64 start_byte, len_byte = len >> 3;
	unsigned ಪೂರ್ಣಾಙ್ಕ start_bit, len_bit = len % 8;
	unsigned ಪೂರ್ಣಾಙ್ಕ first_bit = 0;
	unsigned ಪೂರ್ಣಾಙ್ಕ last_bit  = 0;
	ಪೂರ್ಣಾಙ್ಕ mark_ಎಣಿಕೆ = 0;
	ಪೂರ್ಣಾಙ್ಕ mark_bit = 0;
	ಪೂರ್ಣಾಙ್ಕ i;
	ಧ್ರುವ ವರ್ಣ *ADDR;

	ADDR = bp->bitarray;
	start -= bitmap->start;
	start_byte = start >> 3;
	start_bit = start % 8;

	ಯದಿ (start_bit != 0) {
		/*
		 * The compared start block number or start inode number
		 * is not the first bit in a byte.
		 */
		mark_ಎಣಿಕೆ = 8 - start_bit;
		ಯದಿ (len < 8 - start_bit) {
			mark_ಎಣಿಕೆ = (ಪೂರ್ಣಾಙ್ಕ)len;
			mark_bit = len + start_bit - 1;
		} ಅನ್ಯಥಾ
			mark_bit = 7;

		ಪ್ರತಿ (i = mark_ಎಣಿಕೆ; i > 0; i--, mark_bit--)
			first_bit |= 1 << mark_bit;

		/*
		 * Compare blocks or inodes in the first byte.
		 * If there is any marked bit, this function returns 0.
		 */
		ಯದಿ (first_bit & ADDR[start_byte])
			ಪ್ರತ್ಯಾಗಮ 0;
		ಅನ್ಯಥಾ ಯದಿ (len <= 8 - start_bit)
			ಪ್ರತ್ಯಾಗಮ 1;

		start_byte++;
		len_bit = (len - mark_ಎಣಿಕೆ) % 8;
		len_byte = (len - mark_ಎಣಿಕೆ) >> 3;
	}

	/*
	 * The compared start block number or start inode number is
	 * the first bit in a byte.
	 */
	ಯದಿ (len_bit != 0) {
		/*
		 * The compared end block number or end inode number is
		 * not the last bit in a byte.
		 */
		ಪ್ರತಿ (mark_bit = len_bit - 1; mark_bit >= 0; mark_bit--)
			last_bit |= 1 << mark_bit;

		/*
		 * Compare blocks or inodes in the last byte.
		 * If there is any marked bit, this function returns 0.
		 */
		ಯದಿ (last_bit & ADDR[start_byte + len_byte])
			ಪ್ರತ್ಯಾಗಮ 0;
		ಅನ್ಯಥಾ ಯದಿ (len_byte == 0)
			ಪ್ರತ್ಯಾಗಮ 1;
	}

	/* Check whether all bytes are 0 */
	ಪ್ರತ್ಯಾಗಮ ext2fs_mem_is_zero(ADDR + start_byte, len_byte);
}


ಸ್ಥಿರ errcode_t ba_set_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, size_t ಅಂಕೆ, ಶೂನ್ಯ *in)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->ಗುಪ್ತ;

	memcpy (bp->bitarray + (start >> 3), in, (ಅಂಕೆ + 7) >> 3);

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ errcode_t ba_get_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, size_t ಅಂಕೆ, ಶೂನ್ಯ *out)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->ಗುಪ್ತ;

	memcpy (out, bp->bitarray + (start >> 3), (ಅಂಕೆ + 7) >> 3);

	ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ ba_clear_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->ಗುಪ್ತ;

	memset(bp->bitarray, 0,
	       (size_t) (((bitmap->real_end - bitmap->start) / 8) + 1));
}

#ifdef ENABLE_BMAP_STATS
ಸ್ಥಿರ ಶೂನ್ಯ ba_print_stats(ext2fs_generic_bitmap_64 bitmap)
{
	fprintf(stderr, "%16llu Bytes used by bitarray\n", (unsigned long long)
		((bitmap->real_end - bitmap->start) >> 3) + 1 +
		ಪರಿಮಾಣ(ಸಂರಚನಾ ext2fs_ba_private_struct));
}
#ಅನ್ಯಥಾ
ಸ್ಥಿರ ಶೂನ್ಯ ba_print_stats(ext2fs_generic_bitmap_64 bitmap EXT2FS_ATTR((unused)))
{
}
#endif

/* Find the first zero bit between start and end, inclusive. */
ಸ್ಥಿರ errcode_t ba_find_first_zero(ext2fs_generic_bitmap_64 bitmap,
				    __u64 start, __u64 end, __u64 *out)
{
	ext2fs_ba_private bp = (ext2fs_ba_private)bitmap->ಗುಪ್ತ;
	unsigned long bitpos = start - bitmap->start;
	unsigned long ಎಣಿಕೆ = end - start + 1;
	ಪೂರ್ಣಾಙ್ಕ byte_found = 0; /* whether a != 0xff byte has been found */
	ಧ್ರುವ unsigned ವರ್ಣ *pos;
	unsigned long max_loop_ಎಣಿಕೆ, i;

	/* scan bits until we hit a byte boundary */
	ಯಾವತ್ ((bitpos & 0x7) != 0 && ಎಣಿಕೆ > 0) {
		ಯದಿ (!ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*out = bitpos + bitmap->start;
			ಪ್ರತ್ಯಾಗಮ 0;
		}
		bitpos++;
		ಎಣಿಕೆ--;
	}

	ಯದಿ (!ಎಣಿಕೆ)
		ಪ್ರತ್ಯಾಗಮ ENOENT;

	pos = ((unsigned ವರ್ಣ *)bp->bitarray) + (bitpos >> 3);
	/* scan bytes until 8-byte (64-bit) aligned */
	ಯಾವತ್ (ಎಣಿಕೆ >= 8 && (((uintptr_t)pos) & 0x07)) {
		ಯದಿ (*pos != 0xff) {
			byte_found = 1;
			ಭಙ್ಗ;
		}
		pos++;
		ಎಣಿಕೆ -= 8;
		bitpos += 8;
	}

	ಯದಿ (!byte_found) {
		max_loop_ಎಣಿಕೆ = ಎಣಿಕೆ >> 6; /* 8-byte blocks */
		i = max_loop_ಎಣಿಕೆ;
		ಯಾವತ್ (i) {
			ಯದಿ (*((ಧ್ರುವ __u64 *)pos) != ((__u64)-1))
				ಭಙ್ಗ;
			pos += 8;
			i--;
		}
		ಎಣಿಕೆ -= 64 * (max_loop_ಎಣಿಕೆ - i);
		bitpos += 64 * (max_loop_ಎಣಿಕೆ - i);

		max_loop_ಎಣಿಕೆ = ಎಣಿಕೆ >> 3;
		i = max_loop_ಎಣಿಕೆ;
		ಯಾವತ್ (i) {
			ಯದಿ (*pos != 0xff) {
				byte_found = 1;
				ಭಙ್ಗ;
			}
			pos++;
			i--;
		}
		ಎಣಿಕೆ -= 8 * (max_loop_ಎಣಿಕೆ - i);
		bitpos += 8 * (max_loop_ಎಣಿಕೆ - i);
	}

	/* Here either count < 8 or byte_found == 1. */
	ಯಾವತ್ (ಎಣಿಕೆ-- > 0) {
		ಯದಿ (!ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*out = bitpos + bitmap->start;
			ಪ್ರತ್ಯಾಗಮ 0;
		}
		bitpos++;
	}

	ಪ್ರತ್ಯಾಗಮ ENOENT;
}

/* Find the first one bit between start and end, inclusive. */
ಸ್ಥಿರ errcode_t ba_find_first_set(ext2fs_generic_bitmap_64 bitmap,
				    __u64 start, __u64 end, __u64 *out)
{
	ext2fs_ba_private bp = (ext2fs_ba_private)bitmap->ಗುಪ್ತ;
	unsigned long bitpos = start - bitmap->start;
	unsigned long ಎಣಿಕೆ = end - start + 1;
	ಪೂರ್ಣಾಙ್ಕ byte_found = 0; /* whether a != 0xff byte has been found */
	ಧ್ರುವ unsigned ವರ್ಣ *pos;
	unsigned long max_loop_ಎಣಿಕೆ, i;

	/* scan bits until we hit a byte boundary */
	ಯಾವತ್ ((bitpos & 0x7) != 0 && ಎಣಿಕೆ > 0) {
		ಯದಿ (ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*out = bitpos + bitmap->start;
			ಪ್ರತ್ಯಾಗಮ 0;
		}
		bitpos++;
		ಎಣಿಕೆ--;
	}

	ಯದಿ (!ಎಣಿಕೆ)
		ಪ್ರತ್ಯಾಗಮ ENOENT;

	pos = ((unsigned ವರ್ಣ *)bp->bitarray) + (bitpos >> 3);
	/* scan bytes until 8-byte (64-bit) aligned */
	ಯಾವತ್ (ಎಣಿಕೆ >= 8 && (((uintptr_t)pos) & 0x07)) {
		ಯದಿ (*pos != 0) {
			byte_found = 1;
			ಭಙ್ಗ;
		}
		pos++;
		ಎಣಿಕೆ -= 8;
		bitpos += 8;
	}

	ಯದಿ (!byte_found) {
		max_loop_ಎಣಿಕೆ = ಎಣಿಕೆ >> 6; /* 8-byte blocks */
		i = max_loop_ಎಣಿಕೆ;
		ಯಾವತ್ (i) {
			ಯದಿ (*((ಧ್ರುವ __u64 *)pos) != 0)
				ಭಙ್ಗ;
			pos += 8;
			i--;
		}
		ಎಣಿಕೆ -= 64 * (max_loop_ಎಣಿಕೆ - i);
		bitpos += 64 * (max_loop_ಎಣಿಕೆ - i);

		max_loop_ಎಣಿಕೆ = ಎಣಿಕೆ >> 3;
		i = max_loop_ಎಣಿಕೆ;
		ಯಾವತ್ (i) {
			ಯದಿ (*pos != 0) {
				byte_found = 1;
				ಭಙ್ಗ;
			}
			pos++;
			i--;
		}
		ಎಣಿಕೆ -= 8 * (max_loop_ಎಣಿಕೆ - i);
		bitpos += 8 * (max_loop_ಎಣಿಕೆ - i);
	}

	/* Here either count < 8 or byte_found == 1. */
	ಯಾವತ್ (ಎಣಿಕೆ-- > 0) {
		ಯದಿ (ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*out = bitpos + bitmap->start;
			ಪ್ರತ್ಯಾಗಮ 0;
		}
		bitpos++;
	}

	ಪ್ರತ್ಯಾಗಮ ENOENT;
}

ಸಂರಚನಾ ext2_bitmap_ops ext2fs_blkmap64_bitarray = {
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
