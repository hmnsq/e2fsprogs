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

#અન્તર્ભાવ "config.h"
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <string.h>
#જો HAVE_UNISTD_H
#અન્તર્ભાવ <unistd.h>
#endif
#અન્તર્ભાવ <fcntl.h>
#અન્તર્ભાવ <time.h>
#જો HAVE_SYS_STAT_H
#અન્તર્ભાવ <sys/stat.h>
#endif
#જો HAVE_SYS_TYPES_H
#અન્તર્ભાવ <sys/types.h>
#endif

#અન્તર્ભાવ "ext2_fs.h"
#અન્તર્ભાવ "ext2fsP.h"
#અન્તર્ભાવ "bmap64.h"

/*
 * Private data for bit array implementation of bitmap ops.
 * Currently, this is just a pointer to our big flat hunk of memory,
 * exactly equivalent to the old-skool char * bitmap member.
 */

સંરચના ext2fs_ba_private_struct {
	અક્ષર *bitarray;
};

પ્રકારવ્યાખ્યા સંરચના ext2fs_ba_private_struct *ext2fs_ba_private;

સ્થિર errcode_t ba_alloc_private_data (ext2fs_generic_bitmap_64 bitmap)
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
	retval = ext2fs_get_mem(કદ (ext2fs_ba_private), &bp);
	જો (retval)
		પાછા retval;

	size = (size_t) (((bitmap->real_end - bitmap->start) / 8) + 1);

	retval = ext2fs_get_mem(size, &bp->bitarray);
	જો (retval) {
		ext2fs_free_mem(&bp);
		bp = 0;
		પાછા retval;
	}
	bitmap->ખાનગી = (શૂન્ય *) bp;
	પાછા 0;
}

સ્થિર errcode_t ba_new_bmap(ext2_filsys fs EXT2FS_ATTR((unused)),
			     ext2fs_generic_bitmap_64 bitmap)
{
	ext2fs_ba_private bp;
	errcode_t	retval;
	size_t		size;

	retval = ba_alloc_private_data (bitmap);
	જો (retval)
		પાછા retval;

	bp = (ext2fs_ba_private) bitmap->ખાનગી;
	size = (size_t) (((bitmap->real_end - bitmap->start) / 8) + 1);
	memset(bp->bitarray, 0, size);

	પાછા 0;
}

સ્થિર શૂન્ય ba_free_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->ખાનગી;

	જો (!bp)
		પાછા;

	જો (bp->bitarray) {
		ext2fs_free_mem (&bp->bitarray);
		bp->bitarray = 0;
	}
	ext2fs_free_mem (&bp);
	bp = 0;
}

સ્થિર errcode_t ba_copy_bmap(ext2fs_generic_bitmap_64 src,
			      ext2fs_generic_bitmap_64 dest)
{
	ext2fs_ba_private src_bp = (ext2fs_ba_private) src->ખાનગી;
	ext2fs_ba_private dest_bp;
	errcode_t retval;
	size_t size;

	retval = ba_alloc_private_data (dest);
	જો (retval)
		પાછા retval;

	dest_bp = (ext2fs_ba_private) dest->ખાનગી;

	size = (size_t) (((src->real_end - src->start) / 8) + 1);
	memcpy (dest_bp->bitarray, src_bp->bitarray, size);

	પાછા 0;
}

સ્થિર errcode_t ba_resize_bmap(ext2fs_generic_bitmap_64 bmap,
				__u64 new_end, __u64 new_real_end)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bmap->ખાનગી;
	errcode_t	retval;
	size_t		size, new_size;
	__u64		bitno;

	/*
	 * If we're expanding the bitmap, make sure all of the new
	 * parts of the bitmap are zero.
	 */
	જો (new_end > bmap->end) {
		bitno = bmap->real_end;
		જો (bitno > new_end)
			bitno = new_end;
		માટે (; bitno > bmap->end; bitno--)
			ext2fs_clear_bit64(bitno - bmap->start, bp->bitarray);
	}
	જો (new_real_end == bmap->real_end) {
		bmap->end = new_end;
		પાછા 0;
	}

	size = ((bmap->real_end - bmap->start) / 8) + 1;
	new_size = ((new_real_end - bmap->start) / 8) + 1;

	જો (size != new_size) {
		retval = ext2fs_resize_mem(size, new_size, &bp->bitarray);
		જો (retval)
			પાછા retval;
	}
	જો (new_size > size)
		memset(bp->bitarray + size, 0, new_size - size);

	bmap->end = new_end;
	bmap->real_end = new_real_end;
	પાછા 0;

}

સ્થિર પૂર્ણાંક ba_mark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->ખાનગી;
	blk64_t bitno = (blk64_t) arg;

	પાછા ext2fs_set_bit64(bitno - bitmap->start, bp->bitarray);
}

સ્થિર પૂર્ણાંક ba_unmark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->ખાનગી;
	blk64_t bitno = (blk64_t) arg;

	પાછા ext2fs_clear_bit64(bitno - bitmap->start, bp->bitarray);
}

સ્થિર પૂર્ણાંક ba_test_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->ખાનગી;
	blk64_t bitno = (blk64_t) arg;

	પાછા ext2fs_test_bit64(bitno - bitmap->start, bp->bitarray);
}

સ્થિર શૂન્ય ba_mark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				unsigned પૂર્ણાંક અંક)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->ખાનગી;
	blk64_t bitno = (blk64_t) arg;
	unsigned પૂર્ણાંક i;

	માટે (i = 0; i < અંક; i++)
		ext2fs_fast_set_bit64(bitno + i - bitmap->start, bp->bitarray);
}

સ્થિર શૂન્ય ba_unmark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				  unsigned પૂર્ણાંક અંક)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->ખાનગી;
	blk64_t bitno = (blk64_t) arg;
	unsigned પૂર્ણાંક i;

	માટે (i = 0; i < અંક; i++)
		ext2fs_fast_clear_bit64(bitno + i - bitmap->start, bp->bitarray);
}

સ્થિર પૂર્ણાંક ba_test_clear_bmap_extent(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, unsigned પૂર્ણાંક len)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->ખાનગી;
	__u64 start_byte, len_byte = len >> 3;
	unsigned પૂર્ણાંક start_bit, len_bit = len % 8;
	unsigned પૂર્ણાંક first_bit = 0;
	unsigned પૂર્ણાંક last_bit  = 0;
	પૂર્ણાંક mark_ગણતરી = 0;
	પૂર્ણાંક mark_bit = 0;
	પૂર્ણાંક i;
	અચલ અક્ષર *ADDR;

	ADDR = bp->bitarray;
	start -= bitmap->start;
	start_byte = start >> 3;
	start_bit = start % 8;

	જો (start_bit != 0) {
		/*
		 * The compared start block number or start inode number
		 * is not the first bit in a byte.
		 */
		mark_ગણતરી = 8 - start_bit;
		જો (len < 8 - start_bit) {
			mark_ગણતરી = (પૂર્ણાંક)len;
			mark_bit = len + start_bit - 1;
		} નહીંતર
			mark_bit = 7;

		માટે (i = mark_ગણતરી; i > 0; i--, mark_bit--)
			first_bit |= 1 << mark_bit;

		/*
		 * Compare blocks or inodes in the first byte.
		 * If there is any marked bit, this function returns 0.
		 */
		જો (first_bit & ADDR[start_byte])
			પાછા 0;
		નહીંતર જો (len <= 8 - start_bit)
			પાછા 1;

		start_byte++;
		len_bit = (len - mark_ગણતરી) % 8;
		len_byte = (len - mark_ગણતરી) >> 3;
	}

	/*
	 * The compared start block number or start inode number is
	 * the first bit in a byte.
	 */
	જો (len_bit != 0) {
		/*
		 * The compared end block number or end inode number is
		 * not the last bit in a byte.
		 */
		માટે (mark_bit = len_bit - 1; mark_bit >= 0; mark_bit--)
			last_bit |= 1 << mark_bit;

		/*
		 * Compare blocks or inodes in the last byte.
		 * If there is any marked bit, this function returns 0.
		 */
		જો (last_bit & ADDR[start_byte + len_byte])
			પાછા 0;
		નહીંતર જો (len_byte == 0)
			પાછા 1;
	}

	/* Check whether all bytes are 0 */
	પાછા ext2fs_mem_is_zero(ADDR + start_byte, len_byte);
}


સ્થિર errcode_t ba_set_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, size_t અંક, શૂન્ય *in)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->ખાનગી;

	memcpy (bp->bitarray + (start >> 3), in, (અંક + 7) >> 3);

	પાછા 0;
}

સ્થિર errcode_t ba_get_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, size_t અંક, શૂન્ય *out)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->ખાનગી;

	memcpy (out, bp->bitarray + (start >> 3), (અંક + 7) >> 3);

	પાછા 0;
}

સ્થિર શૂન્ય ba_clear_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->ખાનગી;

	memset(bp->bitarray, 0,
	       (size_t) (((bitmap->real_end - bitmap->start) / 8) + 1));
}

#ifdef ENABLE_BMAP_STATS
સ્થિર શૂન્ય ba_print_stats(ext2fs_generic_bitmap_64 bitmap)
{
	fprintf(stderr, "%16llu Bytes used by bitarray\n", (unsigned long long)
		((bitmap->real_end - bitmap->start) >> 3) + 1 +
		કદ(સંરચના ext2fs_ba_private_struct));
}
#નહીંતર
સ્થિર શૂન્ય ba_print_stats(ext2fs_generic_bitmap_64 bitmap EXT2FS_ATTR((unused)))
{
}
#endif

/* Find the first zero bit between start and end, inclusive. */
સ્થિર errcode_t ba_find_first_zero(ext2fs_generic_bitmap_64 bitmap,
				    __u64 start, __u64 end, __u64 *out)
{
	ext2fs_ba_private bp = (ext2fs_ba_private)bitmap->ખાનગી;
	unsigned long bitpos = start - bitmap->start;
	unsigned long ગણતરી = end - start + 1;
	પૂર્ણાંક byte_found = 0; /* whether a != 0xff byte has been found */
	અચલ unsigned અક્ષર *pos;
	unsigned long max_loop_ગણતરી, i;

	/* scan bits until we hit a byte boundary */
	જ્યારે ((bitpos & 0x7) != 0 && ગણતરી > 0) {
		જો (!ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*out = bitpos + bitmap->start;
			પાછા 0;
		}
		bitpos++;
		ગણતરી--;
	}

	જો (!ગણતરી)
		પાછા ENOENT;

	pos = ((unsigned અક્ષર *)bp->bitarray) + (bitpos >> 3);
	/* scan bytes until 8-byte (64-bit) aligned */
	જ્યારે (ગણતરી >= 8 && (((uintptr_t)pos) & 0x07)) {
		જો (*pos != 0xff) {
			byte_found = 1;
			તોડો;
		}
		pos++;
		ગણતરી -= 8;
		bitpos += 8;
	}

	જો (!byte_found) {
		max_loop_ગણતરી = ગણતરી >> 6; /* 8-byte blocks */
		i = max_loop_ગણતરી;
		જ્યારે (i) {
			જો (*((અચલ __u64 *)pos) != ((__u64)-1))
				તોડો;
			pos += 8;
			i--;
		}
		ગણતરી -= 64 * (max_loop_ગણતરી - i);
		bitpos += 64 * (max_loop_ગણતરી - i);

		max_loop_ગણતરી = ગણતરી >> 3;
		i = max_loop_ગણતરી;
		જ્યારે (i) {
			જો (*pos != 0xff) {
				byte_found = 1;
				તોડો;
			}
			pos++;
			i--;
		}
		ગણતરી -= 8 * (max_loop_ગણતરી - i);
		bitpos += 8 * (max_loop_ગણતરી - i);
	}

	/* Here either count < 8 or byte_found == 1. */
	જ્યારે (ગણતરી-- > 0) {
		જો (!ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*out = bitpos + bitmap->start;
			પાછા 0;
		}
		bitpos++;
	}

	પાછા ENOENT;
}

/* Find the first one bit between start and end, inclusive. */
સ્થિર errcode_t ba_find_first_set(ext2fs_generic_bitmap_64 bitmap,
				    __u64 start, __u64 end, __u64 *out)
{
	ext2fs_ba_private bp = (ext2fs_ba_private)bitmap->ખાનગી;
	unsigned long bitpos = start - bitmap->start;
	unsigned long ગણતરી = end - start + 1;
	પૂર્ણાંક byte_found = 0; /* whether a != 0xff byte has been found */
	અચલ unsigned અક્ષર *pos;
	unsigned long max_loop_ગણતરી, i;

	/* scan bits until we hit a byte boundary */
	જ્યારે ((bitpos & 0x7) != 0 && ગણતરી > 0) {
		જો (ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*out = bitpos + bitmap->start;
			પાછા 0;
		}
		bitpos++;
		ગણતરી--;
	}

	જો (!ગણતરી)
		પાછા ENOENT;

	pos = ((unsigned અક્ષર *)bp->bitarray) + (bitpos >> 3);
	/* scan bytes until 8-byte (64-bit) aligned */
	જ્યારે (ગણતરી >= 8 && (((uintptr_t)pos) & 0x07)) {
		જો (*pos != 0) {
			byte_found = 1;
			તોડો;
		}
		pos++;
		ગણતરી -= 8;
		bitpos += 8;
	}

	જો (!byte_found) {
		max_loop_ગણતરી = ગણતરી >> 6; /* 8-byte blocks */
		i = max_loop_ગણતરી;
		જ્યારે (i) {
			જો (*((અચલ __u64 *)pos) != 0)
				તોડો;
			pos += 8;
			i--;
		}
		ગણતરી -= 64 * (max_loop_ગણતરી - i);
		bitpos += 64 * (max_loop_ગણતરી - i);

		max_loop_ગણતરી = ગણતરી >> 3;
		i = max_loop_ગણતરી;
		જ્યારે (i) {
			જો (*pos != 0) {
				byte_found = 1;
				તોડો;
			}
			pos++;
			i--;
		}
		ગણતરી -= 8 * (max_loop_ગણતરી - i);
		bitpos += 8 * (max_loop_ગણતરી - i);
	}

	/* Here either count < 8 or byte_found == 1. */
	જ્યારે (ગણતરી-- > 0) {
		જો (ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*out = bitpos + bitmap->start;
			પાછા 0;
		}
		bitpos++;
	}

	પાછા ENOENT;
}

સંરચના ext2_bitmap_ops ext2fs_blkmap64_bitarray = {
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
