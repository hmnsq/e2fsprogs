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

#அந்தர்பாவ "config.h"
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <string.h>
#எனில் HAVE_UNISTD_H
#அந்தர்பாவ <unistd.h>
#endif
#அந்தர்பாவ <fcntl.h>
#அந்தர்பாவ <time.h>
#எனில் HAVE_SYS_STAT_H
#அந்தர்பாவ <sys/stat.h>
#endif
#எனில் HAVE_SYS_TYPES_H
#அந்தர்பாவ <sys/types.h>
#endif

#அந்தர்பாவ "ext2_fs.h"
#அந்தர்பாவ "ext2fsP.h"
#அந்தர்பாவ "bmap64.h"

/*
 * Private data for bit array implementation of bitmap ops.
 * Currently, this is just a pointer to our big flat hunk of memory,
 * exactly equivalent to the old-skool char * bitmap member.
 */

கட்டமைப்பு ext2fs_ba_private_struct {
	எழுத்து *bitarray;
};

வகைவரை கட்டமைப்பு ext2fs_ba_private_struct *ext2fs_ba_private;

நிலையான errcode_t ba_alloc_private_data (ext2fs_generic_bitmap_64 bitmap)
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
	retval = ext2fs_get_mem(அளவு (ext2fs_ba_private), &bp);
	எனில் (retval)
		திரும்பு retval;

	size = (size_t) (((bitmap->real_end - bitmap->start) / 8) + 1);

	retval = ext2fs_get_mem(size, &bp->bitarray);
	எனில் (retval) {
		ext2fs_free_mem(&bp);
		bp = 0;
		திரும்பு retval;
	}
	bitmap->தனிப்பட்ட = (வெறுமை *) bp;
	திரும்பு 0;
}

நிலையான errcode_t ba_new_bmap(ext2_filsys fs EXT2FS_ATTR((unused)),
			     ext2fs_generic_bitmap_64 bitmap)
{
	ext2fs_ba_private bp;
	errcode_t	retval;
	size_t		size;

	retval = ba_alloc_private_data (bitmap);
	எனில் (retval)
		திரும்பு retval;

	bp = (ext2fs_ba_private) bitmap->தனிப்பட்ட;
	size = (size_t) (((bitmap->real_end - bitmap->start) / 8) + 1);
	memset(bp->bitarray, 0, size);

	திரும்பு 0;
}

நிலையான வெறுமை ba_free_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->தனிப்பட்ட;

	எனில் (!bp)
		திரும்பு;

	எனில் (bp->bitarray) {
		ext2fs_free_mem (&bp->bitarray);
		bp->bitarray = 0;
	}
	ext2fs_free_mem (&bp);
	bp = 0;
}

நிலையான errcode_t ba_copy_bmap(ext2fs_generic_bitmap_64 src,
			      ext2fs_generic_bitmap_64 dest)
{
	ext2fs_ba_private src_bp = (ext2fs_ba_private) src->தனிப்பட்ட;
	ext2fs_ba_private dest_bp;
	errcode_t retval;
	size_t size;

	retval = ba_alloc_private_data (dest);
	எனில் (retval)
		திரும்பு retval;

	dest_bp = (ext2fs_ba_private) dest->தனிப்பட்ட;

	size = (size_t) (((src->real_end - src->start) / 8) + 1);
	memcpy (dest_bp->bitarray, src_bp->bitarray, size);

	திரும்பு 0;
}

நிலையான errcode_t ba_resize_bmap(ext2fs_generic_bitmap_64 bmap,
				__u64 new_end, __u64 new_real_end)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bmap->தனிப்பட்ட;
	errcode_t	retval;
	size_t		size, new_size;
	__u64		bitno;

	/*
	 * If we're expanding the bitmap, make sure all of the new
	 * parts of the bitmap are zero.
	 */
	எனில் (new_end > bmap->end) {
		bitno = bmap->real_end;
		எனில் (bitno > new_end)
			bitno = new_end;
		க்கு (; bitno > bmap->end; bitno--)
			ext2fs_clear_bit64(bitno - bmap->start, bp->bitarray);
	}
	எனில் (new_real_end == bmap->real_end) {
		bmap->end = new_end;
		திரும்பு 0;
	}

	size = ((bmap->real_end - bmap->start) / 8) + 1;
	new_size = ((new_real_end - bmap->start) / 8) + 1;

	எனில் (size != new_size) {
		retval = ext2fs_resize_mem(size, new_size, &bp->bitarray);
		எனில் (retval)
			திரும்பு retval;
	}
	எனில் (new_size > size)
		memset(bp->bitarray + size, 0, new_size - size);

	bmap->end = new_end;
	bmap->real_end = new_real_end;
	திரும்பு 0;

}

நிலையான முழு ba_mark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->தனிப்பட்ட;
	blk64_t bitno = (blk64_t) arg;

	திரும்பு ext2fs_set_bit64(bitno - bitmap->start, bp->bitarray);
}

நிலையான முழு ba_unmark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->தனிப்பட்ட;
	blk64_t bitno = (blk64_t) arg;

	திரும்பு ext2fs_clear_bit64(bitno - bitmap->start, bp->bitarray);
}

நிலையான முழு ba_test_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->தனிப்பட்ட;
	blk64_t bitno = (blk64_t) arg;

	திரும்பு ext2fs_test_bit64(bitno - bitmap->start, bp->bitarray);
}

நிலையான வெறுமை ba_mark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				unsigned முழு இலக்கம்)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->தனிப்பட்ட;
	blk64_t bitno = (blk64_t) arg;
	unsigned முழு i;

	க்கு (i = 0; i < இலக்கம்; i++)
		ext2fs_fast_set_bit64(bitno + i - bitmap->start, bp->bitarray);
}

நிலையான வெறுமை ba_unmark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				  unsigned முழு இலக்கம்)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->தனிப்பட்ட;
	blk64_t bitno = (blk64_t) arg;
	unsigned முழு i;

	க்கு (i = 0; i < இலக்கம்; i++)
		ext2fs_fast_clear_bit64(bitno + i - bitmap->start, bp->bitarray);
}

நிலையான முழு ba_test_clear_bmap_extent(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, unsigned முழு len)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->தனிப்பட்ட;
	__u64 start_byte, len_byte = len >> 3;
	unsigned முழு start_bit, len_bit = len % 8;
	unsigned முழு first_bit = 0;
	unsigned முழு last_bit  = 0;
	முழு mark_மொத்தம் = 0;
	முழு mark_bit = 0;
	முழு i;
	மாறிலி எழுத்து *ADDR;

	ADDR = bp->bitarray;
	start -= bitmap->start;
	start_byte = start >> 3;
	start_bit = start % 8;

	எனில் (start_bit != 0) {
		/*
		 * The compared start block number or start inode number
		 * is not the first bit in a byte.
		 */
		mark_மொத்தம் = 8 - start_bit;
		எனில் (len < 8 - start_bit) {
			mark_மொத்தம் = (முழு)len;
			mark_bit = len + start_bit - 1;
		} இல்லையெனில்
			mark_bit = 7;

		க்கு (i = mark_மொத்தம்; i > 0; i--, mark_bit--)
			first_bit |= 1 << mark_bit;

		/*
		 * Compare blocks or inodes in the first byte.
		 * If there is any marked bit, this function returns 0.
		 */
		எனில் (first_bit & ADDR[start_byte])
			திரும்பு 0;
		இல்லையெனில் எனில் (len <= 8 - start_bit)
			திரும்பு 1;

		start_byte++;
		len_bit = (len - mark_மொத்தம்) % 8;
		len_byte = (len - mark_மொத்தம்) >> 3;
	}

	/*
	 * The compared start block number or start inode number is
	 * the first bit in a byte.
	 */
	எனில் (len_bit != 0) {
		/*
		 * The compared end block number or end inode number is
		 * not the last bit in a byte.
		 */
		க்கு (mark_bit = len_bit - 1; mark_bit >= 0; mark_bit--)
			last_bit |= 1 << mark_bit;

		/*
		 * Compare blocks or inodes in the last byte.
		 * If there is any marked bit, this function returns 0.
		 */
		எனில் (last_bit & ADDR[start_byte + len_byte])
			திரும்பு 0;
		இல்லையெனில் எனில் (len_byte == 0)
			திரும்பு 1;
	}

	/* Check whether all bytes are 0 */
	திரும்பு ext2fs_mem_is_zero(ADDR + start_byte, len_byte);
}


நிலையான errcode_t ba_set_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, size_t இலக்கம், வெறுமை *in)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->தனிப்பட்ட;

	memcpy (bp->bitarray + (start >> 3), in, (இலக்கம் + 7) >> 3);

	திரும்பு 0;
}

நிலையான errcode_t ba_get_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, size_t இலக்கம், வெறுமை *out)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->தனிப்பட்ட;

	memcpy (out, bp->bitarray + (start >> 3), (இலக்கம் + 7) >> 3);

	திரும்பு 0;
}

நிலையான வெறுமை ba_clear_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->தனிப்பட்ட;

	memset(bp->bitarray, 0,
	       (size_t) (((bitmap->real_end - bitmap->start) / 8) + 1));
}

#ifdef ENABLE_BMAP_STATS
நிலையான வெறுமை ba_print_stats(ext2fs_generic_bitmap_64 bitmap)
{
	fprintf(stderr, "%16llu Bytes used by bitarray\n", (unsigned long long)
		((bitmap->real_end - bitmap->start) >> 3) + 1 +
		அளவு(கட்டமைப்பு ext2fs_ba_private_struct));
}
#இல்லையெனில்
நிலையான வெறுமை ba_print_stats(ext2fs_generic_bitmap_64 bitmap EXT2FS_ATTR((unused)))
{
}
#endif

/* Find the first zero bit between start and end, inclusive. */
நிலையான errcode_t ba_find_first_zero(ext2fs_generic_bitmap_64 bitmap,
				    __u64 start, __u64 end, __u64 *out)
{
	ext2fs_ba_private bp = (ext2fs_ba_private)bitmap->தனிப்பட்ட;
	unsigned long bitpos = start - bitmap->start;
	unsigned long மொத்தம் = end - start + 1;
	முழு byte_found = 0; /* whether a != 0xff byte has been found */
	மாறிலி unsigned எழுத்து *pos;
	unsigned long max_loop_மொத்தம், i;

	/* scan bits until we hit a byte boundary */
	போது ((bitpos & 0x7) != 0 && மொத்தம் > 0) {
		எனில் (!ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*out = bitpos + bitmap->start;
			திரும்பு 0;
		}
		bitpos++;
		மொத்தம்--;
	}

	எனில் (!மொத்தம்)
		திரும்பு ENOENT;

	pos = ((unsigned எழுத்து *)bp->bitarray) + (bitpos >> 3);
	/* scan bytes until 8-byte (64-bit) aligned */
	போது (மொத்தம் >= 8 && (((uintptr_t)pos) & 0x07)) {
		எனில் (*pos != 0xff) {
			byte_found = 1;
			நிறுத்து;
		}
		pos++;
		மொத்தம் -= 8;
		bitpos += 8;
	}

	எனில் (!byte_found) {
		max_loop_மொத்தம் = மொத்தம் >> 6; /* 8-byte blocks */
		i = max_loop_மொத்தம்;
		போது (i) {
			எனில் (*((மாறிலி __u64 *)pos) != ((__u64)-1))
				நிறுத்து;
			pos += 8;
			i--;
		}
		மொத்தம் -= 64 * (max_loop_மொத்தம் - i);
		bitpos += 64 * (max_loop_மொத்தம் - i);

		max_loop_மொத்தம் = மொத்தம் >> 3;
		i = max_loop_மொத்தம்;
		போது (i) {
			எனில் (*pos != 0xff) {
				byte_found = 1;
				நிறுத்து;
			}
			pos++;
			i--;
		}
		மொத்தம் -= 8 * (max_loop_மொத்தம் - i);
		bitpos += 8 * (max_loop_மொத்தம் - i);
	}

	/* Here either count < 8 or byte_found == 1. */
	போது (மொத்தம்-- > 0) {
		எனில் (!ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*out = bitpos + bitmap->start;
			திரும்பு 0;
		}
		bitpos++;
	}

	திரும்பு ENOENT;
}

/* Find the first one bit between start and end, inclusive. */
நிலையான errcode_t ba_find_first_set(ext2fs_generic_bitmap_64 bitmap,
				    __u64 start, __u64 end, __u64 *out)
{
	ext2fs_ba_private bp = (ext2fs_ba_private)bitmap->தனிப்பட்ட;
	unsigned long bitpos = start - bitmap->start;
	unsigned long மொத்தம் = end - start + 1;
	முழு byte_found = 0; /* whether a != 0xff byte has been found */
	மாறிலி unsigned எழுத்து *pos;
	unsigned long max_loop_மொத்தம், i;

	/* scan bits until we hit a byte boundary */
	போது ((bitpos & 0x7) != 0 && மொத்தம் > 0) {
		எனில் (ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*out = bitpos + bitmap->start;
			திரும்பு 0;
		}
		bitpos++;
		மொத்தம்--;
	}

	எனில் (!மொத்தம்)
		திரும்பு ENOENT;

	pos = ((unsigned எழுத்து *)bp->bitarray) + (bitpos >> 3);
	/* scan bytes until 8-byte (64-bit) aligned */
	போது (மொத்தம் >= 8 && (((uintptr_t)pos) & 0x07)) {
		எனில் (*pos != 0) {
			byte_found = 1;
			நிறுத்து;
		}
		pos++;
		மொத்தம் -= 8;
		bitpos += 8;
	}

	எனில் (!byte_found) {
		max_loop_மொத்தம் = மொத்தம் >> 6; /* 8-byte blocks */
		i = max_loop_மொத்தம்;
		போது (i) {
			எனில் (*((மாறிலி __u64 *)pos) != 0)
				நிறுத்து;
			pos += 8;
			i--;
		}
		மொத்தம் -= 64 * (max_loop_மொத்தம் - i);
		bitpos += 64 * (max_loop_மொத்தம் - i);

		max_loop_மொத்தம் = மொத்தம் >> 3;
		i = max_loop_மொத்தம்;
		போது (i) {
			எனில் (*pos != 0) {
				byte_found = 1;
				நிறுத்து;
			}
			pos++;
			i--;
		}
		மொத்தம் -= 8 * (max_loop_மொத்தம் - i);
		bitpos += 8 * (max_loop_மொத்தம் - i);
	}

	/* Here either count < 8 or byte_found == 1. */
	போது (மொத்தம்-- > 0) {
		எனில் (ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*out = bitpos + bitmap->start;
			திரும்பு 0;
		}
		bitpos++;
	}

	திரும்பு ENOENT;
}

கட்டமைப்பு ext2_bitmap_ops ext2fs_blkmap64_bitarray = {
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
