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

#ଅନ୍ତର୍ଭାଵ "config.h"
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଯଦି HAVE_UNISTD_H
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#endif
#ଅନ୍ତର୍ଭାଵ <fcntl.h>
#ଅନ୍ତର୍ଭାଵ <time.h>
#ଯଦି HAVE_SYS_STAT_H
#ଅନ୍ତର୍ଭାଵ <sys/stat.h>
#endif
#ଯଦି HAVE_SYS_TYPES_H
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#endif

#ଅନ୍ତର୍ଭାଵ "ext2_fs.h"
#ଅନ୍ତର୍ଭାଵ "ext2fsP.h"
#ଅନ୍ତର୍ଭାଵ "bmap64.h"

/*
 * Private data for bit array implementation of bitmap ops.
 * Currently, this is just a pointer to our big flat hunk of memory,
 * exactly equivalent to the old-skool char * bitmap member.
 */

ସଂରଚନା ext2fs_ba_private_struct {
	ଅକ୍ଷର *bitarray;
};

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା ext2fs_ba_private_struct *ext2fs_ba_private;

ସ୍ଥିର errcode_t ba_alloc_private_data (ext2fs_generic_bitmap_64 bitmap)
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
	retval = ext2fs_get_mem(ଆକାର (ext2fs_ba_private), &bp);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;

	size = (size_t) (((bitmap->real_end - bitmap->start) / 8) + 1);

	retval = ext2fs_get_mem(size, &bp->bitarray);
	ଯଦି (retval) {
		ext2fs_free_mem(&bp);
		bp = 0;
		ଫେରନ୍ତୁ retval;
	}
	bitmap->ବ୍ୟକ୍ତିଗତ = (ଶୂନ୍ୟ *) bp;
	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର errcode_t ba_new_bmap(ext2_filsys fs EXT2FS_ATTR((unused)),
			     ext2fs_generic_bitmap_64 bitmap)
{
	ext2fs_ba_private bp;
	errcode_t	retval;
	size_t		size;

	retval = ba_alloc_private_data (bitmap);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;

	bp = (ext2fs_ba_private) bitmap->ବ୍ୟକ୍ତିଗତ;
	size = (size_t) (((bitmap->real_end - bitmap->start) / 8) + 1);
	memset(bp->bitarray, 0, size);

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ ba_free_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->ବ୍ୟକ୍ତିଗତ;

	ଯଦି (!bp)
		ଫେରନ୍ତୁ;

	ଯଦି (bp->bitarray) {
		ext2fs_free_mem (&bp->bitarray);
		bp->bitarray = 0;
	}
	ext2fs_free_mem (&bp);
	bp = 0;
}

ସ୍ଥିର errcode_t ba_copy_bmap(ext2fs_generic_bitmap_64 src,
			      ext2fs_generic_bitmap_64 dest)
{
	ext2fs_ba_private src_bp = (ext2fs_ba_private) src->ବ୍ୟକ୍ତିଗତ;
	ext2fs_ba_private dest_bp;
	errcode_t retval;
	size_t size;

	retval = ba_alloc_private_data (dest);
	ଯଦି (retval)
		ଫେରନ୍ତୁ retval;

	dest_bp = (ext2fs_ba_private) dest->ବ୍ୟକ୍ତିଗତ;

	size = (size_t) (((src->real_end - src->start) / 8) + 1);
	memcpy (dest_bp->bitarray, src_bp->bitarray, size);

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର errcode_t ba_resize_bmap(ext2fs_generic_bitmap_64 bmap,
				__u64 new_end, __u64 new_real_end)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bmap->ବ୍ୟକ୍ତିଗତ;
	errcode_t	retval;
	size_t		size, new_size;
	__u64		bitno;

	/*
	 * If we're expanding the bitmap, make sure all of the new
	 * parts of the bitmap are zero.
	 */
	ଯଦି (new_end > bmap->end) {
		bitno = bmap->real_end;
		ଯଦି (bitno > new_end)
			bitno = new_end;
		ପାଇଁ (; bitno > bmap->end; bitno--)
			ext2fs_clear_bit64(bitno - bmap->start, bp->bitarray);
	}
	ଯଦି (new_real_end == bmap->real_end) {
		bmap->end = new_end;
		ଫେରନ୍ତୁ 0;
	}

	size = ((bmap->real_end - bmap->start) / 8) + 1;
	new_size = ((new_real_end - bmap->start) / 8) + 1;

	ଯଦି (size != new_size) {
		retval = ext2fs_resize_mem(size, new_size, &bp->bitarray);
		ଯଦି (retval)
			ଫେରନ୍ତୁ retval;
	}
	ଯଦି (new_size > size)
		memset(bp->bitarray + size, 0, new_size - size);

	bmap->end = new_end;
	bmap->real_end = new_real_end;
	ଫେରନ୍ତୁ 0;

}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା ba_mark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->ବ୍ୟକ୍ତିଗତ;
	blk64_t bitno = (blk64_t) arg;

	ଫେରନ୍ତୁ ext2fs_set_bit64(bitno - bitmap->start, bp->bitarray);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା ba_unmark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->ବ୍ୟକ୍ତିଗତ;
	blk64_t bitno = (blk64_t) arg;

	ଫେରନ୍ତୁ ext2fs_clear_bit64(bitno - bitmap->start, bp->bitarray);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା ba_test_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->ବ୍ୟକ୍ତିଗତ;
	blk64_t bitno = (blk64_t) arg;

	ଫେରନ୍ତୁ ext2fs_test_bit64(bitno - bitmap->start, bp->bitarray);
}

ସ୍ଥିର ଶୂନ୍ୟ ba_mark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->ବ୍ୟକ୍ତିଗତ;
	blk64_t bitno = (blk64_t) arg;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	ପାଇଁ (i = 0; i < ଅଙ୍କ; i++)
		ext2fs_fast_set_bit64(bitno + i - bitmap->start, bp->bitarray);
}

ସ୍ଥିର ଶୂନ୍ୟ ba_unmark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଅଙ୍କ)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->ବ୍ୟକ୍ତିଗତ;
	blk64_t bitno = (blk64_t) arg;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;

	ପାଇଁ (i = 0; i < ଅଙ୍କ; i++)
		ext2fs_fast_clear_bit64(bitno + i - bitmap->start, bp->bitarray);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା ba_test_clear_bmap_extent(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା len)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->ବ୍ୟକ୍ତିଗତ;
	__u64 start_byte, len_byte = len >> 3;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା start_bit, len_bit = len % 8;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା first_bit = 0;
	unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା last_bit  = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା mark_ଗଣନା = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା mark_bit = 0;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
	ଅଚଳ ଅକ୍ଷର *ADDR;

	ADDR = bp->bitarray;
	start -= bitmap->start;
	start_byte = start >> 3;
	start_bit = start % 8;

	ଯଦି (start_bit != 0) {
		/*
		 * The compared start block number or start inode number
		 * is not the first bit in a byte.
		 */
		mark_ଗଣନା = 8 - start_bit;
		ଯଦି (len < 8 - start_bit) {
			mark_ଗଣନା = (ପୂର୍ଣ୍ଣସଂଖ୍ୟା)len;
			mark_bit = len + start_bit - 1;
		} ଅନ୍ୟଥା
			mark_bit = 7;

		ପାଇଁ (i = mark_ଗଣନା; i > 0; i--, mark_bit--)
			first_bit |= 1 << mark_bit;

		/*
		 * Compare blocks or inodes in the first byte.
		 * If there is any marked bit, this function returns 0.
		 */
		ଯଦି (first_bit & ADDR[start_byte])
			ଫେରନ୍ତୁ 0;
		ଅନ୍ୟଥା ଯଦି (len <= 8 - start_bit)
			ଫେରନ୍ତୁ 1;

		start_byte++;
		len_bit = (len - mark_ଗଣନା) % 8;
		len_byte = (len - mark_ଗଣନା) >> 3;
	}

	/*
	 * The compared start block number or start inode number is
	 * the first bit in a byte.
	 */
	ଯଦି (len_bit != 0) {
		/*
		 * The compared end block number or end inode number is
		 * not the last bit in a byte.
		 */
		ପାଇଁ (mark_bit = len_bit - 1; mark_bit >= 0; mark_bit--)
			last_bit |= 1 << mark_bit;

		/*
		 * Compare blocks or inodes in the last byte.
		 * If there is any marked bit, this function returns 0.
		 */
		ଯଦି (last_bit & ADDR[start_byte + len_byte])
			ଫେରନ୍ତୁ 0;
		ଅନ୍ୟଥା ଯଦି (len_byte == 0)
			ଫେରନ୍ତୁ 1;
	}

	/* Check whether all bytes are 0 */
	ଫେରନ୍ତୁ ext2fs_mem_is_zero(ADDR + start_byte, len_byte);
}


ସ୍ଥିର errcode_t ba_set_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, size_t ଅଙ୍କ, ଶୂନ୍ୟ *in)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->ବ୍ୟକ୍ତିଗତ;

	memcpy (bp->bitarray + (start >> 3), in, (ଅଙ୍କ + 7) >> 3);

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର errcode_t ba_get_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, size_t ଅଙ୍କ, ଶୂନ୍ୟ *out)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->ବ୍ୟକ୍ତିଗତ;

	memcpy (out, bp->bitarray + (start >> 3), (ଅଙ୍କ + 7) >> 3);

	ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଶୂନ୍ୟ ba_clear_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->ବ୍ୟକ୍ତିଗତ;

	memset(bp->bitarray, 0,
	       (size_t) (((bitmap->real_end - bitmap->start) / 8) + 1));
}

#ifdef ENABLE_BMAP_STATS
ସ୍ଥିର ଶୂନ୍ୟ ba_print_stats(ext2fs_generic_bitmap_64 bitmap)
{
	fprintf(stderr, "%16llu Bytes used by bitarray\n", (unsigned long long)
		((bitmap->real_end - bitmap->start) >> 3) + 1 +
		ଆକାର(ସଂରଚନା ext2fs_ba_private_struct));
}
#ଅନ୍ୟଥା
ସ୍ଥିର ଶୂନ୍ୟ ba_print_stats(ext2fs_generic_bitmap_64 bitmap EXT2FS_ATTR((unused)))
{
}
#endif

/* Find the first zero bit between start and end, inclusive. */
ସ୍ଥିର errcode_t ba_find_first_zero(ext2fs_generic_bitmap_64 bitmap,
				    __u64 start, __u64 end, __u64 *out)
{
	ext2fs_ba_private bp = (ext2fs_ba_private)bitmap->ବ୍ୟକ୍ତିଗତ;
	unsigned long bitpos = start - bitmap->start;
	unsigned long ଗଣନା_2 = end - start + 1;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା byte_found = 0; /* whether a != 0xff byte has been found */
	ଅଚଳ unsigned ଅକ୍ଷର *pos;
	unsigned long max_loop_ଗଣନା, i;

	/* scan bits until we hit a byte boundary */
	ଯେତେବେଳେ ((bitpos & 0x7) != 0 && ଗଣନା_2 > 0) {
		ଯଦି (!ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*out = bitpos + bitmap->start;
			ଫେରନ୍ତୁ 0;
		}
		bitpos++;
		ଗଣନା_2--;
	}

	ଯଦି (!ଗଣନା_2)
		ଫେରନ୍ତୁ ENOENT;

	pos = ((unsigned ଅକ୍ଷର *)bp->bitarray) + (bitpos >> 3);
	/* scan bytes until 8-byte (64-bit) aligned */
	ଯେତେବେଳେ (ଗଣନା_2 >= 8 && (((uintptr_t)pos) & 0x07)) {
		ଯଦି (*pos != 0xff) {
			byte_found = 1;
			ଭାଙ୍ଗ;
		}
		pos++;
		ଗଣନା_2 -= 8;
		bitpos += 8;
	}

	ଯଦି (!byte_found) {
		max_loop_ଗଣନା = ଗଣନା_2 >> 6; /* 8-byte blocks */
		i = max_loop_ଗଣନା;
		ଯେତେବେଳେ (i) {
			ଯଦି (*((ଅଚଳ __u64 *)pos) != ((__u64)-1))
				ଭାଙ୍ଗ;
			pos += 8;
			i--;
		}
		ଗଣନା_2 -= 64 * (max_loop_ଗଣନା - i);
		bitpos += 64 * (max_loop_ଗଣନା - i);

		max_loop_ଗଣନା = ଗଣନା_2 >> 3;
		i = max_loop_ଗଣନା;
		ଯେତେବେଳେ (i) {
			ଯଦି (*pos != 0xff) {
				byte_found = 1;
				ଭାଙ୍ଗ;
			}
			pos++;
			i--;
		}
		ଗଣନା_2 -= 8 * (max_loop_ଗଣନା - i);
		bitpos += 8 * (max_loop_ଗଣନା - i);
	}

	/* Here either count < 8 or byte_found == 1. */
	ଯେତେବେଳେ (ଗଣନା_2-- > 0) {
		ଯଦି (!ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*out = bitpos + bitmap->start;
			ଫେରନ୍ତୁ 0;
		}
		bitpos++;
	}

	ଫେରନ୍ତୁ ENOENT;
}

/* Find the first one bit between start and end, inclusive. */
ସ୍ଥିର errcode_t ba_find_first_set(ext2fs_generic_bitmap_64 bitmap,
				    __u64 start, __u64 end, __u64 *out)
{
	ext2fs_ba_private bp = (ext2fs_ba_private)bitmap->ବ୍ୟକ୍ତିଗତ;
	unsigned long bitpos = start - bitmap->start;
	unsigned long ଗଣନା_2 = end - start + 1;
	ପୂର୍ଣ୍ଣସଂଖ୍ୟା byte_found = 0; /* whether a != 0xff byte has been found */
	ଅଚଳ unsigned ଅକ୍ଷର *pos;
	unsigned long max_loop_ଗଣନା, i;

	/* scan bits until we hit a byte boundary */
	ଯେତେବେଳେ ((bitpos & 0x7) != 0 && ଗଣନା_2 > 0) {
		ଯଦି (ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*out = bitpos + bitmap->start;
			ଫେରନ୍ତୁ 0;
		}
		bitpos++;
		ଗଣନା_2--;
	}

	ଯଦି (!ଗଣନା_2)
		ଫେରନ୍ତୁ ENOENT;

	pos = ((unsigned ଅକ୍ଷର *)bp->bitarray) + (bitpos >> 3);
	/* scan bytes until 8-byte (64-bit) aligned */
	ଯେତେବେଳେ (ଗଣନା_2 >= 8 && (((uintptr_t)pos) & 0x07)) {
		ଯଦି (*pos != 0) {
			byte_found = 1;
			ଭାଙ୍ଗ;
		}
		pos++;
		ଗଣନା_2 -= 8;
		bitpos += 8;
	}

	ଯଦି (!byte_found) {
		max_loop_ଗଣନା = ଗଣନା_2 >> 6; /* 8-byte blocks */
		i = max_loop_ଗଣନା;
		ଯେତେବେଳେ (i) {
			ଯଦି (*((ଅଚଳ __u64 *)pos) != 0)
				ଭାଙ୍ଗ;
			pos += 8;
			i--;
		}
		ଗଣନା_2 -= 64 * (max_loop_ଗଣନା - i);
		bitpos += 64 * (max_loop_ଗଣନା - i);

		max_loop_ଗଣନା = ଗଣନା_2 >> 3;
		i = max_loop_ଗଣନା;
		ଯେତେବେଳେ (i) {
			ଯଦି (*pos != 0) {
				byte_found = 1;
				ଭାଙ୍ଗ;
			}
			pos++;
			i--;
		}
		ଗଣନା_2 -= 8 * (max_loop_ଗଣନା - i);
		bitpos += 8 * (max_loop_ଗଣନା - i);
	}

	/* Here either count < 8 or byte_found == 1. */
	ଯେତେବେଳେ (ଗଣନା_2-- > 0) {
		ଯଦି (ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*out = bitpos + bitmap->start;
			ଫେରନ୍ତୁ 0;
		}
		bitpos++;
	}

	ଫେରନ୍ତୁ ENOENT;
}

ସଂରଚନା ext2_bitmap_ops ext2fs_blkmap64_bitarray = {
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
