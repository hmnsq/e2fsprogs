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

#include "config.h"
#include <stdio.h>
#include <string.h>
#якщо HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>
#include <time.h>
#якщо HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#якщо HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include "ext2_fs.h"
#include "ext2fsP.h"
#include "bmap64.h"

/*
 * Private data for bit array implementation of bitmap ops.
 * Currently, this is just a pointer to our big flat hunk of memory,
 * exactly equivalent to the old-skool char * bitmap member.
 */

struct ext2fs_ba_private_struct {
	char *bitarray;
};

typedef struct ext2fs_ba_private_struct *ext2fs_ba_private;

static errcode_t ba_alloc_private_data (ext2fs_generic_bitmap_64 bitmap)
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
	retval = ext2fs_get_mem(sizeof (ext2fs_ba_private), &bp);
	якщо (retval)
		повернути retval;

	size = (size_t) (((bitmap->real_end - bitmap->start) / 8) + 1);

	retval = ext2fs_get_mem(size, &bp->bitarray);
	якщо (retval) {
		ext2fs_free_mem(&bp);
		bp = 0;
		повернути retval;
	}
	bitmap->private = (void *) bp;
	повернути 0;
}

static errcode_t ba_new_bmap(ext2_filsys fs EXT2FS_ATTR((unused)),
			     ext2fs_generic_bitmap_64 bitmap)
{
	ext2fs_ba_private bp;
	errcode_t	retval;
	size_t		size;

	retval = ba_alloc_private_data (bitmap);
	якщо (retval)
		повернути retval;

	bp = (ext2fs_ba_private) bitmap->private;
	size = (size_t) (((bitmap->real_end - bitmap->start) / 8) + 1);
	memset(bp->bitarray, 0, size);

	повернути 0;
}

static void ba_free_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->private;

	якщо (!bp)
		повернути;

	якщо (bp->bitarray) {
		ext2fs_free_mem (&bp->bitarray);
		bp->bitarray = 0;
	}
	ext2fs_free_mem (&bp);
	bp = 0;
}

static errcode_t ba_copy_bmap(ext2fs_generic_bitmap_64 src,
			      ext2fs_generic_bitmap_64 dest)
{
	ext2fs_ba_private src_bp = (ext2fs_ba_private) src->private;
	ext2fs_ba_private dest_bp;
	errcode_t retval;
	size_t size;

	retval = ba_alloc_private_data (dest);
	якщо (retval)
		повернути retval;

	dest_bp = (ext2fs_ba_private) dest->private;

	size = (size_t) (((src->real_end - src->start) / 8) + 1);
	memcpy (dest_bp->bitarray, src_bp->bitarray, size);

	повернути 0;
}

static errcode_t ba_resize_bmap(ext2fs_generic_bitmap_64 bmap,
				__u64 new_end, __u64 new_real_end)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bmap->private;
	errcode_t	retval;
	size_t		size, new_size;
	__u64		bitno;

	/*
	 * If we're expanding the bitmap, make sure all of the new
	 * parts of the bitmap are zero.
	 */
	якщо (new_end > bmap->end) {
		bitno = bmap->real_end;
		якщо (bitno > new_end)
			bitno = new_end;
		для (; bitno > bmap->end; bitno--)
			ext2fs_clear_bit64(bitno - bmap->start, bp->bitarray);
	}
	якщо (new_real_end == bmap->real_end) {
		bmap->end = new_end;
		повернути 0;
	}

	size = ((bmap->real_end - bmap->start) / 8) + 1;
	new_size = ((new_real_end - bmap->start) / 8) + 1;

	якщо (size != new_size) {
		retval = ext2fs_resize_mem(size, new_size, &bp->bitarray);
		якщо (retval)
			повернути retval;
	}
	якщо (new_size > size)
		memset(bp->bitarray + size, 0, new_size - size);

	bmap->end = new_end;
	bmap->real_end = new_real_end;
	повернути 0;

}

static ціле ba_mark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->private;
	blk64_t bitno = (blk64_t) arg;

	повернути ext2fs_set_bit64(bitno - bitmap->start, bp->bitarray);
}

static ціле ba_unmark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->private;
	blk64_t bitno = (blk64_t) arg;

	повернути ext2fs_clear_bit64(bitno - bitmap->start, bp->bitarray);
}

static ціле ba_test_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->private;
	blk64_t bitno = (blk64_t) arg;

	повернути ext2fs_test_bit64(bitno - bitmap->start, bp->bitarray);
}

static void ba_mark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				unsigned ціле num)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->private;
	blk64_t bitno = (blk64_t) arg;
	unsigned ціле i;

	для (i = 0; i < num; i++)
		ext2fs_fast_set_bit64(bitno + i - bitmap->start, bp->bitarray);
}

static void ba_unmark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				  unsigned ціле num)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->private;
	blk64_t bitno = (blk64_t) arg;
	unsigned ціле i;

	для (i = 0; i < num; i++)
		ext2fs_fast_clear_bit64(bitno + i - bitmap->start, bp->bitarray);
}

static ціле ba_test_clear_bmap_extent(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, unsigned ціле len)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->private;
	__u64 start_byte, len_byte = len >> 3;
	unsigned ціле start_bit, len_bit = len % 8;
	unsigned ціле first_bit = 0;
	unsigned ціле last_bit  = 0;
	ціле mark_count = 0;
	ціле mark_bit = 0;
	ціле i;
	const char *ADDR;

	ADDR = bp->bitarray;
	start -= bitmap->start;
	start_byte = start >> 3;
	start_bit = start % 8;

	якщо (start_bit != 0) {
		/*
		 * The compared start block number or start inode number
		 * is not the first bit in a byte.
		 */
		mark_count = 8 - start_bit;
		якщо (len < 8 - start_bit) {
			mark_count = (ціле)len;
			mark_bit = len + start_bit - 1;
		} інакше
			mark_bit = 7;

		для (i = mark_count; i > 0; i--, mark_bit--)
			first_bit |= 1 << mark_bit;

		/*
		 * Compare blocks or inodes in the first byte.
		 * If there is any marked bit, this function returns 0.
		 */
		якщо (first_bit & ADDR[start_byte])
			повернути 0;
		інакше якщо (len <= 8 - start_bit)
			повернути 1;

		start_byte++;
		len_bit = (len - mark_count) % 8;
		len_byte = (len - mark_count) >> 3;
	}

	/*
	 * The compared start block number or start inode number is
	 * the first bit in a byte.
	 */
	якщо (len_bit != 0) {
		/*
		 * The compared end block number or end inode number is
		 * not the last bit in a byte.
		 */
		для (mark_bit = len_bit - 1; mark_bit >= 0; mark_bit--)
			last_bit |= 1 << mark_bit;

		/*
		 * Compare blocks or inodes in the last byte.
		 * If there is any marked bit, this function returns 0.
		 */
		якщо (last_bit & ADDR[start_byte + len_byte])
			повернути 0;
		інакше якщо (len_byte == 0)
			повернути 1;
	}

	/* Check whether all bytes are 0 */
	повернути ext2fs_mem_is_zero(ADDR + start_byte, len_byte);
}


static errcode_t ba_set_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, size_t num, void *in)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->private;

	memcpy (bp->bitarray + (start >> 3), in, (num + 7) >> 3);

	повернути 0;
}

static errcode_t ba_get_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, size_t num, void *out)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->private;

	memcpy (out, bp->bitarray + (start >> 3), (num + 7) >> 3);

	повернути 0;
}

static void ba_clear_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->private;

	memset(bp->bitarray, 0,
	       (size_t) (((bitmap->real_end - bitmap->start) / 8) + 1));
}

#ifdef ENABLE_BMAP_STATS
static void ba_print_stats(ext2fs_generic_bitmap_64 bitmap)
{
	fprintf(stderr, "%16llu Bytes used by bitarray\n", (unsigned long long)
		((bitmap->real_end - bitmap->start) >> 3) + 1 +
		sizeof(struct ext2fs_ba_private_struct));
}
#інакше
static void ba_print_stats(ext2fs_generic_bitmap_64 bitmap EXT2FS_ATTR((unused)))
{
}
#endif

/* Find the first zero bit between start and end, inclusive. */
static errcode_t ba_find_first_zero(ext2fs_generic_bitmap_64 bitmap,
				    __u64 start, __u64 end, __u64 *out)
{
	ext2fs_ba_private bp = (ext2fs_ba_private)bitmap->private;
	unsigned long bitpos = start - bitmap->start;
	unsigned long count = end - start + 1;
	ціле byte_found = 0; /* whether a != 0xff byte has been found */
	const unsigned char *pos;
	unsigned long max_loop_count, i;

	/* scan bits until we hit a byte boundary */
	поки ((bitpos & 0x7) != 0 && count > 0) {
		якщо (!ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*out = bitpos + bitmap->start;
			повернути 0;
		}
		bitpos++;
		count--;
	}

	якщо (!count)
		повернути ENOENT;

	pos = ((unsigned char *)bp->bitarray) + (bitpos >> 3);
	/* scan bytes until 8-byte (64-bit) aligned */
	поки (count >= 8 && (((uintptr_t)pos) & 0x07)) {
		якщо (*pos != 0xff) {
			byte_found = 1;
			break;
		}
		pos++;
		count -= 8;
		bitpos += 8;
	}

	якщо (!byte_found) {
		max_loop_count = count >> 6; /* 8-byte blocks */
		i = max_loop_count;
		поки (i) {
			якщо (*((const __u64 *)pos) != ((__u64)-1))
				break;
			pos += 8;
			i--;
		}
		count -= 64 * (max_loop_count - i);
		bitpos += 64 * (max_loop_count - i);

		max_loop_count = count >> 3;
		i = max_loop_count;
		поки (i) {
			якщо (*pos != 0xff) {
				byte_found = 1;
				break;
			}
			pos++;
			i--;
		}
		count -= 8 * (max_loop_count - i);
		bitpos += 8 * (max_loop_count - i);
	}

	/* Here either count < 8 or byte_found == 1. */
	поки (count-- > 0) {
		якщо (!ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*out = bitpos + bitmap->start;
			повернути 0;
		}
		bitpos++;
	}

	повернути ENOENT;
}

/* Find the first one bit between start and end, inclusive. */
static errcode_t ba_find_first_set(ext2fs_generic_bitmap_64 bitmap,
				    __u64 start, __u64 end, __u64 *out)
{
	ext2fs_ba_private bp = (ext2fs_ba_private)bitmap->private;
	unsigned long bitpos = start - bitmap->start;
	unsigned long count = end - start + 1;
	ціле byte_found = 0; /* whether a != 0xff byte has been found */
	const unsigned char *pos;
	unsigned long max_loop_count, i;

	/* scan bits until we hit a byte boundary */
	поки ((bitpos & 0x7) != 0 && count > 0) {
		якщо (ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*out = bitpos + bitmap->start;
			повернути 0;
		}
		bitpos++;
		count--;
	}

	якщо (!count)
		повернути ENOENT;

	pos = ((unsigned char *)bp->bitarray) + (bitpos >> 3);
	/* scan bytes until 8-byte (64-bit) aligned */
	поки (count >= 8 && (((uintptr_t)pos) & 0x07)) {
		якщо (*pos != 0) {
			byte_found = 1;
			break;
		}
		pos++;
		count -= 8;
		bitpos += 8;
	}

	якщо (!byte_found) {
		max_loop_count = count >> 6; /* 8-byte blocks */
		i = max_loop_count;
		поки (i) {
			якщо (*((const __u64 *)pos) != 0)
				break;
			pos += 8;
			i--;
		}
		count -= 64 * (max_loop_count - i);
		bitpos += 64 * (max_loop_count - i);

		max_loop_count = count >> 3;
		i = max_loop_count;
		поки (i) {
			якщо (*pos != 0) {
				byte_found = 1;
				break;
			}
			pos++;
			i--;
		}
		count -= 8 * (max_loop_count - i);
		bitpos += 8 * (max_loop_count - i);
	}

	/* Here either count < 8 or byte_found == 1. */
	поки (count-- > 0) {
		якщо (ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*out = bitpos + bitmap->start;
			повернути 0;
		}
		bitpos++;
	}

	повернути ENOENT;
}

struct ext2_bitmap_ops ext2fs_blkmap64_bitarray = {
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
