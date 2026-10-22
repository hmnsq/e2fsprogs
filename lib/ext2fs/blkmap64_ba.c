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

#شامل "config.h"
#شامل <stdio.h>
#شامل <string.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#endif
#شامل <fcntl.h>
#شامل <time.h>
#اگر HAVE_SYS_STAT_H
#شامل <sys/stat.h>
#endif
#اگر HAVE_SYS_TYPES_H
#شامل <sys/types.h>
#endif

#شامل "ext2_fs.h"
#شامل "ext2fsP.h"
#شامل "bmap64.h"

/*
 * Private data for bit array implementation of bitmap ops.
 * Currently, this is just a pointer to our big flat hunk of memory,
 * exactly equivalent to the old-skool char * bitmap member.
 */

ساختار ext2fs_ba_private_struct {
	نویسه *bitarray;
};

تعریفنوع ساختار ext2fs_ba_private_struct *ext2fs_ba_private;

ایستا errcode_t ba_alloc_private_data (ext2fs_generic_bitmap_64 bitmap)
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
	retval = ext2fs_get_mem(اندازه (ext2fs_ba_private), &bp);
	اگر (retval)
		بازگشت retval;

	size = (size_t) (((bitmap->real_end - bitmap->start) / 8) + 1);

	retval = ext2fs_get_mem(size, &bp->bitarray);
	اگر (retval) {
		ext2fs_free_mem(&bp);
		bp = 0;
		بازگشت retval;
	}
	bitmap->خصوصی = (تهی *) bp;
	بازگشت 0;
}

ایستا errcode_t ba_new_bmap(ext2_filsys fs EXT2FS_ATTR((unused)),
			     ext2fs_generic_bitmap_64 bitmap)
{
	ext2fs_ba_private bp;
	errcode_t	retval;
	size_t		size;

	retval = ba_alloc_private_data (bitmap);
	اگر (retval)
		بازگشت retval;

	bp = (ext2fs_ba_private) bitmap->خصوصی;
	size = (size_t) (((bitmap->real_end - bitmap->start) / 8) + 1);
	memset(bp->bitarray, 0, size);

	بازگشت 0;
}

ایستا تهی ba_free_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->خصوصی;

	اگر (!bp)
		بازگشت;

	اگر (bp->bitarray) {
		ext2fs_free_mem (&bp->bitarray);
		bp->bitarray = 0;
	}
	ext2fs_free_mem (&bp);
	bp = 0;
}

ایستا errcode_t ba_copy_bmap(ext2fs_generic_bitmap_64 src,
			      ext2fs_generic_bitmap_64 dest)
{
	ext2fs_ba_private src_bp = (ext2fs_ba_private) src->خصوصی;
	ext2fs_ba_private dest_bp;
	errcode_t retval;
	size_t size;

	retval = ba_alloc_private_data (dest);
	اگر (retval)
		بازگشت retval;

	dest_bp = (ext2fs_ba_private) dest->خصوصی;

	size = (size_t) (((src->real_end - src->start) / 8) + 1);
	memcpy (dest_bp->bitarray, src_bp->bitarray, size);

	بازگشت 0;
}

ایستا errcode_t ba_resize_bmap(ext2fs_generic_bitmap_64 bmap,
				__u64 new_end, __u64 new_real_end)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bmap->خصوصی;
	errcode_t	retval;
	size_t		size, new_size;
	__u64		bitno;

	/*
	 * If we're expanding the bitmap, make sure all of the new
	 * parts of the bitmap are zero.
	 */
	اگر (new_end > bmap->end) {
		bitno = bmap->real_end;
		اگر (bitno > new_end)
			bitno = new_end;
		برای (; bitno > bmap->end; bitno--)
			ext2fs_clear_bit64(bitno - bmap->start, bp->bitarray);
	}
	اگر (new_real_end == bmap->real_end) {
		bmap->end = new_end;
		بازگشت 0;
	}

	size = ((bmap->real_end - bmap->start) / 8) + 1;
	new_size = ((new_real_end - bmap->start) / 8) + 1;

	اگر (size != new_size) {
		retval = ext2fs_resize_mem(size, new_size, &bp->bitarray);
		اگر (retval)
			بازگشت retval;
	}
	اگر (new_size > size)
		memset(bp->bitarray + size, 0, new_size - size);

	bmap->end = new_end;
	bmap->real_end = new_real_end;
	بازگشت 0;

}

ایستا صحیح ba_mark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->خصوصی;
	blk64_t bitno = (blk64_t) arg;

	بازگشت ext2fs_set_bit64(bitno - bitmap->start, bp->bitarray);
}

ایستا صحیح ba_unmark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->خصوصی;
	blk64_t bitno = (blk64_t) arg;

	بازگشت ext2fs_clear_bit64(bitno - bitmap->start, bp->bitarray);
}

ایستا صحیح ba_test_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->خصوصی;
	blk64_t bitno = (blk64_t) arg;

	بازگشت ext2fs_test_bit64(bitno - bitmap->start, bp->bitarray);
}

ایستا تهی ba_mark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				unsigned صحیح رقم)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->خصوصی;
	blk64_t bitno = (blk64_t) arg;
	unsigned صحیح i;

	برای (i = 0; i < رقم; i++)
		ext2fs_fast_set_bit64(bitno + i - bitmap->start, bp->bitarray);
}

ایستا تهی ba_unmark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				  unsigned صحیح رقم)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->خصوصی;
	blk64_t bitno = (blk64_t) arg;
	unsigned صحیح i;

	برای (i = 0; i < رقم; i++)
		ext2fs_fast_clear_bit64(bitno + i - bitmap->start, bp->bitarray);
}

ایستا صحیح ba_test_clear_bmap_extent(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, unsigned صحیح len)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->خصوصی;
	__u64 start_byte, len_byte = len >> 3;
	unsigned صحیح start_bit, len_bit = len % 8;
	unsigned صحیح first_bit = 0;
	unsigned صحیح last_bit  = 0;
	صحیح mark_شمار = 0;
	صحیح mark_bit = 0;
	صحیح i;
	ثابت نویسه *ADDR;

	ADDR = bp->bitarray;
	start -= bitmap->start;
	start_byte = start >> 3;
	start_bit = start % 8;

	اگر (start_bit != 0) {
		/*
		 * The compared start block number or start inode number
		 * is not the first bit in a byte.
		 */
		mark_شمار = 8 - start_bit;
		اگر (len < 8 - start_bit) {
			mark_شمار = (صحیح)len;
			mark_bit = len + start_bit - 1;
		} وگرنه
			mark_bit = 7;

		برای (i = mark_شمار; i > 0; i--, mark_bit--)
			first_bit |= 1 << mark_bit;

		/*
		 * Compare blocks or inodes in the first byte.
		 * If there is any marked bit, this function returns 0.
		 */
		اگر (first_bit & ADDR[start_byte])
			بازگشت 0;
		وگرنه اگر (len <= 8 - start_bit)
			بازگشت 1;

		start_byte++;
		len_bit = (len - mark_شمار) % 8;
		len_byte = (len - mark_شمار) >> 3;
	}

	/*
	 * The compared start block number or start inode number is
	 * the first bit in a byte.
	 */
	اگر (len_bit != 0) {
		/*
		 * The compared end block number or end inode number is
		 * not the last bit in a byte.
		 */
		برای (mark_bit = len_bit - 1; mark_bit >= 0; mark_bit--)
			last_bit |= 1 << mark_bit;

		/*
		 * Compare blocks or inodes in the last byte.
		 * If there is any marked bit, this function returns 0.
		 */
		اگر (last_bit & ADDR[start_byte + len_byte])
			بازگشت 0;
		وگرنه اگر (len_byte == 0)
			بازگشت 1;
	}

	/* Check whether all bytes are 0 */
	بازگشت ext2fs_mem_is_zero(ADDR + start_byte, len_byte);
}


ایستا errcode_t ba_set_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, size_t رقم, تهی *in)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->خصوصی;

	memcpy (bp->bitarray + (start >> 3), in, (رقم + 7) >> 3);

	بازگشت 0;
}

ایستا errcode_t ba_get_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, size_t رقم, تهی *out)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->خصوصی;

	memcpy (out, bp->bitarray + (start >> 3), (رقم + 7) >> 3);

	بازگشت 0;
}

ایستا تهی ba_clear_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->خصوصی;

	memset(bp->bitarray, 0,
	       (size_t) (((bitmap->real_end - bitmap->start) / 8) + 1));
}

#ifdef ENABLE_BMAP_STATS
ایستا تهی ba_print_stats(ext2fs_generic_bitmap_64 bitmap)
{
	fprintf(stderr, "%16llu Bytes used by bitarray\n", (unsigned long long)
		((bitmap->real_end - bitmap->start) >> 3) + 1 +
		اندازه(ساختار ext2fs_ba_private_struct));
}
#وگرنه
ایستا تهی ba_print_stats(ext2fs_generic_bitmap_64 bitmap EXT2FS_ATTR((unused)))
{
}
#endif

/* Find the first zero bit between start and end, inclusive. */
ایستا errcode_t ba_find_first_zero(ext2fs_generic_bitmap_64 bitmap,
				    __u64 start, __u64 end, __u64 *out)
{
	ext2fs_ba_private bp = (ext2fs_ba_private)bitmap->خصوصی;
	unsigned long bitpos = start - bitmap->start;
	unsigned long شمار = end - start + 1;
	صحیح byte_found = 0; /* whether a != 0xff byte has been found */
	ثابت unsigned نویسه *pos;
	unsigned long max_loop_شمار, i;

	/* scan bits until we hit a byte boundary */
	وقتی ((bitpos & 0x7) != 0 && شمار > 0) {
		اگر (!ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*out = bitpos + bitmap->start;
			بازگشت 0;
		}
		bitpos++;
		شمار--;
	}

	اگر (!شمار)
		بازگشت ENOENT;

	pos = ((unsigned نویسه *)bp->bitarray) + (bitpos >> 3);
	/* scan bytes until 8-byte (64-bit) aligned */
	وقتی (شمار >= 8 && (((uintptr_t)pos) & 0x07)) {
		اگر (*pos != 0xff) {
			byte_found = 1;
			بشکن;
		}
		pos++;
		شمار -= 8;
		bitpos += 8;
	}

	اگر (!byte_found) {
		max_loop_شمار = شمار >> 6; /* 8-byte blocks */
		i = max_loop_شمار;
		وقتی (i) {
			اگر (*((ثابت __u64 *)pos) != ((__u64)-1))
				بشکن;
			pos += 8;
			i--;
		}
		شمار -= 64 * (max_loop_شمار - i);
		bitpos += 64 * (max_loop_شمار - i);

		max_loop_شمار = شمار >> 3;
		i = max_loop_شمار;
		وقتی (i) {
			اگر (*pos != 0xff) {
				byte_found = 1;
				بشکن;
			}
			pos++;
			i--;
		}
		شمار -= 8 * (max_loop_شمار - i);
		bitpos += 8 * (max_loop_شمار - i);
	}

	/* Here either count < 8 or byte_found == 1. */
	وقتی (شمار-- > 0) {
		اگر (!ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*out = bitpos + bitmap->start;
			بازگشت 0;
		}
		bitpos++;
	}

	بازگشت ENOENT;
}

/* Find the first one bit between start and end, inclusive. */
ایستا errcode_t ba_find_first_set(ext2fs_generic_bitmap_64 bitmap,
				    __u64 start, __u64 end, __u64 *out)
{
	ext2fs_ba_private bp = (ext2fs_ba_private)bitmap->خصوصی;
	unsigned long bitpos = start - bitmap->start;
	unsigned long شمار = end - start + 1;
	صحیح byte_found = 0; /* whether a != 0xff byte has been found */
	ثابت unsigned نویسه *pos;
	unsigned long max_loop_شمار, i;

	/* scan bits until we hit a byte boundary */
	وقتی ((bitpos & 0x7) != 0 && شمار > 0) {
		اگر (ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*out = bitpos + bitmap->start;
			بازگشت 0;
		}
		bitpos++;
		شمار--;
	}

	اگر (!شمار)
		بازگشت ENOENT;

	pos = ((unsigned نویسه *)bp->bitarray) + (bitpos >> 3);
	/* scan bytes until 8-byte (64-bit) aligned */
	وقتی (شمار >= 8 && (((uintptr_t)pos) & 0x07)) {
		اگر (*pos != 0) {
			byte_found = 1;
			بشکن;
		}
		pos++;
		شمار -= 8;
		bitpos += 8;
	}

	اگر (!byte_found) {
		max_loop_شمار = شمار >> 6; /* 8-byte blocks */
		i = max_loop_شمار;
		وقتی (i) {
			اگر (*((ثابت __u64 *)pos) != 0)
				بشکن;
			pos += 8;
			i--;
		}
		شمار -= 64 * (max_loop_شمار - i);
		bitpos += 64 * (max_loop_شمار - i);

		max_loop_شمار = شمار >> 3;
		i = max_loop_شمار;
		وقتی (i) {
			اگر (*pos != 0) {
				byte_found = 1;
				بشکن;
			}
			pos++;
			i--;
		}
		شمار -= 8 * (max_loop_شمار - i);
		bitpos += 8 * (max_loop_شمار - i);
	}

	/* Here either count < 8 or byte_found == 1. */
	وقتی (شمار-- > 0) {
		اگر (ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*out = bitpos + bitmap->start;
			بازگشت 0;
		}
		bitpos++;
	}

	بازگشت ENOENT;
}

ساختار ext2_bitmap_ops ext2fs_blkmap64_bitarray = {
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
