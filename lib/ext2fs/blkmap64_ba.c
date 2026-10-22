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

#ਅਨ੍ਤਰ੍ਭਾਵ "config.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਜੇ HAVE_UNISTD_H
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਜੇ_ਅੰਤ
#ਅਨ੍ਤਰ੍ਭਾਵ <fcntl.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <time.h>
#ਜੇ HAVE_SYS_STAT_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/stat.h>
#ਜੇ_ਅੰਤ
#ਜੇ HAVE_SYS_TYPES_H
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/types.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "ext2_fs.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "ext2fsP.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "bmap64.h"

/*
 * Private data for bit array implementation of bitmap ops.
 * Currently, this is just a pointer to our big flat hunk of memory,
 * exactly equivalent to the old-skool char * bitmap member.
 */

ਬਣਤਰ ext2fs_ba_ਨਿੱਜੀ_struct {
	ਅੱਖਰ *bitarray;
};

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ ext2fs_ba_ਨਿੱਜੀ_struct *ext2fs_ba_ਨਿੱਜੀ;

ਸਥਿਰ errcode_t ba_alloc_ਨਿੱਜੀ_ਅੰਕੜੇ (ext2fs_generic_bitmap_64 bitmap)
{
	ext2fs_ba_ਨਿੱਜੀ bp;
	errcode_t	retval;
	size_t		ਮਾਪ;

	/*
	 * Since we only have the one pointer, we could just shove our
	 * private data in the void *private field itself, but then
	 * we'd have to do a fair bit of rewriting if we ever added a
	 * field.  I'm agnostic.
	 */
	retval = ext2fs_get_mem(ਆਕਾਰ (ext2fs_ba_ਨਿੱਜੀ), &bp);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	ਮਾਪ = (size_t) (((bitmap->real_ਸਿਰਾ - bitmap->ਸ਼ੁਰੂ) / 8) + 1);

	retval = ext2fs_get_mem(ਮਾਪ, &bp->bitarray);
	ਜੇ (retval) {
		ext2fs_free_mem(&bp);
		bp = 0;
		ਵਾਪਸ retval;
	}
	bitmap->ਨਿਜੀ = (ਖਾਲੀ *) bp;
	ਵਾਪਸ 0;
}

ਸਥਿਰ errcode_t ba_new_bmap(ext2_filsys fs EXT2FS_ATTR((ਅਣਵਰਤਿਆ)),
			     ext2fs_generic_bitmap_64 bitmap)
{
	ext2fs_ba_ਨਿੱਜੀ bp;
	errcode_t	retval;
	size_t		ਮਾਪ;

	retval = ba_alloc_ਨਿੱਜੀ_ਅੰਕੜੇ (bitmap);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	bp = (ext2fs_ba_ਨਿੱਜੀ) bitmap->ਨਿਜੀ;
	ਮਾਪ = (size_t) (((bitmap->real_ਸਿਰਾ - bitmap->ਸ਼ੁਰੂ) / 8) + 1);
	memset(bp->bitarray, 0, ਮਾਪ);

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ ba_free_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	ext2fs_ba_ਨਿੱਜੀ bp = (ext2fs_ba_ਨਿੱਜੀ) bitmap->ਨਿਜੀ;

	ਜੇ (!bp)
		ਵਾਪਸ;

	ਜੇ (bp->bitarray) {
		ext2fs_free_mem (&bp->bitarray);
		bp->bitarray = 0;
	}
	ext2fs_free_mem (&bp);
	bp = 0;
}

ਸਥਿਰ errcode_t ba_copy_bmap(ext2fs_generic_bitmap_64 src,
			      ext2fs_generic_bitmap_64 dest)
{
	ext2fs_ba_ਨਿੱਜੀ src_bp = (ext2fs_ba_ਨਿੱਜੀ) src->ਨਿਜੀ;
	ext2fs_ba_ਨਿੱਜੀ dest_bp;
	errcode_t retval;
	size_t ਮਾਪ;

	retval = ba_alloc_ਨਿੱਜੀ_ਅੰਕੜੇ (dest);
	ਜੇ (retval)
		ਵਾਪਸ retval;

	dest_bp = (ext2fs_ba_ਨਿੱਜੀ) dest->ਨਿਜੀ;

	ਮਾਪ = (size_t) (((src->real_ਸਿਰਾ - src->ਸ਼ੁਰੂ) / 8) + 1);
	ਸਮ੍ਰਿਤੀ_ਨਕਲ (dest_bp->bitarray, src_bp->bitarray, ਮਾਪ);

	ਵਾਪਸ 0;
}

ਸਥਿਰ errcode_t ba_resize_bmap(ext2fs_generic_bitmap_64 bmap,
				__u64 new_ਸਿਰਾ, __u64 new_real_ਸਿਰਾ)
{
	ext2fs_ba_ਨਿੱਜੀ bp = (ext2fs_ba_ਨਿੱਜੀ) bmap->ਨਿਜੀ;
	errcode_t	retval;
	size_t		ਮਾਪ, new_ਮਾਪ;
	__u64		bitno;

	/*
	 * If we're expanding the bitmap, make sure all of the new
	 * parts of the bitmap are zero.
	 */
	ਜੇ (new_ਸਿਰਾ > bmap->ਸਿਰਾ) {
		bitno = bmap->real_ਸਿਰਾ;
		ਜੇ (bitno > new_ਸਿਰਾ)
			bitno = new_ਸਿਰਾ;
		ਲਈ (; bitno > bmap->ਸਿਰਾ; bitno--)
			ext2fs_clear_bit64(bitno - bmap->ਸ਼ੁਰੂ, bp->bitarray);
	}
	ਜੇ (new_real_ਸਿਰਾ == bmap->real_ਸਿਰਾ) {
		bmap->ਸਿਰਾ = new_ਸਿਰਾ;
		ਵਾਪਸ 0;
	}

	ਮਾਪ = ((bmap->real_ਸਿਰਾ - bmap->ਸ਼ੁਰੂ) / 8) + 1;
	new_ਮਾਪ = ((new_real_ਸਿਰਾ - bmap->ਸ਼ੁਰੂ) / 8) + 1;

	ਜੇ (ਮਾਪ != new_ਮਾਪ) {
		retval = ext2fs_resize_mem(ਮਾਪ, new_ਮਾਪ, &bp->bitarray);
		ਜੇ (retval)
			ਵਾਪਸ retval;
	}
	ਜੇ (new_ਮਾਪ > ਮਾਪ)
		memset(bp->bitarray + ਮਾਪ, 0, new_ਮਾਪ - ਮਾਪ);

	bmap->ਸਿਰਾ = new_ਸਿਰਾ;
	bmap->real_ਸਿਰਾ = new_real_ਸਿਰਾ;
	ਵਾਪਸ 0;

}

ਸਥਿਰ ਪੂਰਨ ba_mark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ext2fs_ba_ਨਿੱਜੀ bp = (ext2fs_ba_ਨਿੱਜੀ) bitmap->ਨਿਜੀ;
	blk64_t bitno = (blk64_t) arg;

	ਵਾਪਸ ext2fs_ਸੈੱਟ_bit64(bitno - bitmap->ਸ਼ੁਰੂ, bp->bitarray);
}

ਸਥਿਰ ਪੂਰਨ ba_unmark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ext2fs_ba_ਨਿੱਜੀ bp = (ext2fs_ba_ਨਿੱਜੀ) bitmap->ਨਿਜੀ;
	blk64_t bitno = (blk64_t) arg;

	ਵਾਪਸ ext2fs_clear_bit64(bitno - bitmap->ਸ਼ੁਰੂ, bp->bitarray);
}

ਸਥਿਰ ਪੂਰਨ ba_test_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ext2fs_ba_ਨਿੱਜੀ bp = (ext2fs_ba_ਨਿੱਜੀ) bitmap->ਨਿਜੀ;
	blk64_t bitno = (blk64_t) arg;

	ਵਾਪਸ ext2fs_test_bit64(bitno - bitmap->ਸ਼ੁਰੂ, bp->bitarray);
}

ਸਥਿਰ ਖਾਲੀ ba_mark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ)
{
	ext2fs_ba_ਨਿੱਜੀ bp = (ext2fs_ba_ਨਿੱਜੀ) bitmap->ਨਿਜੀ;
	blk64_t bitno = (blk64_t) arg;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i;

	ਲਈ (i = 0; i < ਅੰਕ; i++)
		ext2fs_fast_ਸੈੱਟ_bit64(bitno + i - bitmap->ਸ਼ੁਰੂ, bp->bitarray);
}

ਸਥਿਰ ਖਾਲੀ ba_unmark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਅੰਕ)
{
	ext2fs_ba_ਨਿੱਜੀ bp = (ext2fs_ba_ਨਿੱਜੀ) bitmap->ਨਿਜੀ;
	blk64_t bitno = (blk64_t) arg;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ i;

	ਲਈ (i = 0; i < ਅੰਕ; i++)
		ext2fs_fast_clear_bit64(bitno + i - bitmap->ਸ਼ੁਰੂ, bp->bitarray);
}

ਸਥਿਰ ਪੂਰਨ ba_test_clear_bmap_extent(ext2fs_generic_bitmap_64 bitmap,
				     __u64 ਸ਼ੁਰੂ, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ len)
{
	ext2fs_ba_ਨਿੱਜੀ bp = (ext2fs_ba_ਨਿੱਜੀ) bitmap->ਨਿਜੀ;
	__u64 ਸ਼ੁਰੂ_byte, len_byte = len >> 3;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਸ਼ੁਰੂ_ਦੋਆਧਾਰੀ_ਅੰਕ, len_ਦੋਆਧਾਰੀ_ਅੰਕ = len % 8;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਪਹਿਲਾ_ਦੋਆਧਾਰੀ_ਅੰਕ = 0;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ ਆਖਰੀ_ਦੋਆਧਾਰੀ_ਅੰਕ  = 0;
	ਪੂਰਨ mark_ਗਿਣਤੀ = 0;
	ਪੂਰਨ mark_ਦੋਆਧਾਰੀ_ਅੰਕ = 0;
	ਪੂਰਨ i;
	ਅਚਲ ਅੱਖਰ *ADDR;

	ADDR = bp->bitarray;
	ਸ਼ੁਰੂ -= bitmap->ਸ਼ੁਰੂ;
	ਸ਼ੁਰੂ_byte = ਸ਼ੁਰੂ >> 3;
	ਸ਼ੁਰੂ_ਦੋਆਧਾਰੀ_ਅੰਕ = ਸ਼ੁਰੂ % 8;

	ਜੇ (ਸ਼ੁਰੂ_ਦੋਆਧਾਰੀ_ਅੰਕ != 0) {
		/*
		 * The compared start block number or start inode number
		 * is not the first bit in a byte.
		 */
		mark_ਗਿਣਤੀ = 8 - ਸ਼ੁਰੂ_ਦੋਆਧਾਰੀ_ਅੰਕ;
		ਜੇ (len < 8 - ਸ਼ੁਰੂ_ਦੋਆਧਾਰੀ_ਅੰਕ) {
			mark_ਗਿਣਤੀ = (ਪੂਰਨ)len;
			mark_ਦੋਆਧਾਰੀ_ਅੰਕ = len + ਸ਼ੁਰੂ_ਦੋਆਧਾਰੀ_ਅੰਕ - 1;
		} ਨਹੀਂਤਾਂ
			mark_ਦੋਆਧਾਰੀ_ਅੰਕ = 7;

		ਲਈ (i = mark_ਗਿਣਤੀ; i > 0; i--, mark_ਦੋਆਧਾਰੀ_ਅੰਕ--)
			ਪਹਿਲਾ_ਦੋਆਧਾਰੀ_ਅੰਕ |= 1 << mark_ਦੋਆਧਾਰੀ_ਅੰਕ;

		/*
		 * Compare blocks or inodes in the first byte.
		 * If there is any marked bit, this function returns 0.
		 */
		ਜੇ (ਪਹਿਲਾ_ਦੋਆਧਾਰੀ_ਅੰਕ & ADDR[ਸ਼ੁਰੂ_byte])
			ਵਾਪਸ 0;
		ਨਹੀਂਤਾਂ ਜੇ (len <= 8 - ਸ਼ੁਰੂ_ਦੋਆਧਾਰੀ_ਅੰਕ)
			ਵਾਪਸ 1;

		ਸ਼ੁਰੂ_byte++;
		len_ਦੋਆਧਾਰੀ_ਅੰਕ = (len - mark_ਗਿਣਤੀ) % 8;
		len_byte = (len - mark_ਗਿਣਤੀ) >> 3;
	}

	/*
	 * The compared start block number or start inode number is
	 * the first bit in a byte.
	 */
	ਜੇ (len_ਦੋਆਧਾਰੀ_ਅੰਕ != 0) {
		/*
		 * The compared end block number or end inode number is
		 * not the last bit in a byte.
		 */
		ਲਈ (mark_ਦੋਆਧਾਰੀ_ਅੰਕ = len_ਦੋਆਧਾਰੀ_ਅੰਕ - 1; mark_ਦੋਆਧਾਰੀ_ਅੰਕ >= 0; mark_ਦੋਆਧਾਰੀ_ਅੰਕ--)
			ਆਖਰੀ_ਦੋਆਧਾਰੀ_ਅੰਕ |= 1 << mark_ਦੋਆਧਾਰੀ_ਅੰਕ;

		/*
		 * Compare blocks or inodes in the last byte.
		 * If there is any marked bit, this function returns 0.
		 */
		ਜੇ (ਆਖਰੀ_ਦੋਆਧਾਰੀ_ਅੰਕ & ADDR[ਸ਼ੁਰੂ_byte + len_byte])
			ਵਾਪਸ 0;
		ਨਹੀਂਤਾਂ ਜੇ (len_byte == 0)
			ਵਾਪਸ 1;
	}

	/* Check whether all bytes are 0 */
	ਵਾਪਸ ext2fs_mem_is_zero(ADDR + ਸ਼ੁਰੂ_byte, len_byte);
}


ਸਥਿਰ errcode_t ba_ਸੈੱਟ_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 ਸ਼ੁਰੂ, size_t ਅੰਕ, ਖਾਲੀ *ਵਿੱਚ)
{
	ext2fs_ba_ਨਿੱਜੀ bp = (ext2fs_ba_ਨਿੱਜੀ) bitmap->ਨਿਜੀ;

	ਸਮ੍ਰਿਤੀ_ਨਕਲ (bp->bitarray + (ਸ਼ੁਰੂ >> 3), ਵਿੱਚ, (ਅੰਕ + 7) >> 3);

	ਵਾਪਸ 0;
}

ਸਥਿਰ errcode_t ba_get_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 ਸ਼ੁਰੂ, size_t ਅੰਕ, ਖਾਲੀ *ਬਾਹਰਲਾ)
{
	ext2fs_ba_ਨਿੱਜੀ bp = (ext2fs_ba_ਨਿੱਜੀ) bitmap->ਨਿਜੀ;

	ਸਮ੍ਰਿਤੀ_ਨਕਲ (ਬਾਹਰਲਾ, bp->bitarray + (ਸ਼ੁਰੂ >> 3), (ਅੰਕ + 7) >> 3);

	ਵਾਪਸ 0;
}

ਸਥਿਰ ਖਾਲੀ ba_clear_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	ext2fs_ba_ਨਿੱਜੀ bp = (ext2fs_ba_ਨਿੱਜੀ) bitmap->ਨਿਜੀ;

	memset(bp->bitarray, 0,
	       (size_t) (((bitmap->real_ਸਿਰਾ - bitmap->ਸ਼ੁਰੂ) / 8) + 1));
}

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ ENABLE_BMAP_STATS
ਸਥਿਰ ਖਾਲੀ ba_print_stats(ext2fs_generic_bitmap_64 bitmap)
{
	fprintf(stderr, "%16llu Bytes used by bitarray\n", (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਲੰਮਾ)
		((bitmap->real_ਸਿਰਾ - bitmap->ਸ਼ੁਰੂ) >> 3) + 1 +
		ਆਕਾਰ(ਬਣਤਰ ext2fs_ba_ਨਿੱਜੀ_struct));
}
#ਨਹੀਂਤਾਂ
ਸਥਿਰ ਖਾਲੀ ba_print_stats(ext2fs_generic_bitmap_64 bitmap EXT2FS_ATTR((ਅਣਵਰਤਿਆ)))
{
}
#ਜੇ_ਅੰਤ

/* Find the first zero bit between start and end, inclusive. */
ਸਥਿਰ errcode_t ba_ਲੱਭੋ_ਪਹਿਲਾ_zero(ext2fs_generic_bitmap_64 bitmap,
				    __u64 ਸ਼ੁਰੂ, __u64 ਸਿਰਾ, __u64 *ਬਾਹਰਲਾ)
{
	ext2fs_ba_ਨਿੱਜੀ bp = (ext2fs_ba_ਨਿੱਜੀ)bitmap->ਨਿਜੀ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ bitpos = ਸ਼ੁਰੂ - bitmap->ਸ਼ੁਰੂ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਗਿਣਤੀ_2 = ਸਿਰਾ - ਸ਼ੁਰੂ + 1;
	ਪੂਰਨ byte_ਮਿਲਿਆ = 0; /* whether a != 0xff byte has been found */
	ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *pos;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ max_loop_ਗਿਣਤੀ, i;

	/* scan bits until we hit a byte boundary */
	ਜਦੋਂ ((bitpos & 0x7) != 0 && ਗਿਣਤੀ_2 > 0) {
		ਜੇ (!ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*ਬਾਹਰਲਾ = bitpos + bitmap->ਸ਼ੁਰੂ;
			ਵਾਪਸ 0;
		}
		bitpos++;
		ਗਿਣਤੀ_2--;
	}

	ਜੇ (!ਗਿਣਤੀ_2)
		ਵਾਪਸ ENOENT;

	pos = ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *)bp->bitarray) + (bitpos >> 3);
	/* scan bytes until 8-byte (64-bit) aligned */
	ਜਦੋਂ (ਗਿਣਤੀ_2 >= 8 && (((uintptr_t)pos) & 0x07)) {
		ਜੇ (*pos != 0xff) {
			byte_ਮਿਲਿਆ = 1;
			ਰੋਕੋ;
		}
		pos++;
		ਗਿਣਤੀ_2 -= 8;
		bitpos += 8;
	}

	ਜੇ (!byte_ਮਿਲਿਆ) {
		max_loop_ਗਿਣਤੀ = ਗਿਣਤੀ_2 >> 6; /* 8-byte blocks */
		i = max_loop_ਗਿਣਤੀ;
		ਜਦੋਂ (i) {
			ਜੇ (*((ਅਚਲ __u64 *)pos) != ((__u64)-1))
				ਰੋਕੋ;
			pos += 8;
			i--;
		}
		ਗਿਣਤੀ_2 -= 64 * (max_loop_ਗਿਣਤੀ - i);
		bitpos += 64 * (max_loop_ਗਿਣਤੀ - i);

		max_loop_ਗਿਣਤੀ = ਗਿਣਤੀ_2 >> 3;
		i = max_loop_ਗਿਣਤੀ;
		ਜਦੋਂ (i) {
			ਜੇ (*pos != 0xff) {
				byte_ਮਿਲਿਆ = 1;
				ਰੋਕੋ;
			}
			pos++;
			i--;
		}
		ਗਿਣਤੀ_2 -= 8 * (max_loop_ਗਿਣਤੀ - i);
		bitpos += 8 * (max_loop_ਗਿਣਤੀ - i);
	}

	/* Here either count < 8 or byte_found == 1. */
	ਜਦੋਂ (ਗਿਣਤੀ_2-- > 0) {
		ਜੇ (!ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*ਬਾਹਰਲਾ = bitpos + bitmap->ਸ਼ੁਰੂ;
			ਵਾਪਸ 0;
		}
		bitpos++;
	}

	ਵਾਪਸ ENOENT;
}

/* Find the first one bit between start and end, inclusive. */
ਸਥਿਰ errcode_t ba_ਲੱਭੋ_ਪਹਿਲਾ_ਸੈੱਟ(ext2fs_generic_bitmap_64 bitmap,
				    __u64 ਸ਼ੁਰੂ, __u64 ਸਿਰਾ, __u64 *ਬਾਹਰਲਾ)
{
	ext2fs_ba_ਨਿੱਜੀ bp = (ext2fs_ba_ਨਿੱਜੀ)bitmap->ਨਿਜੀ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ bitpos = ਸ਼ੁਰੂ - bitmap->ਸ਼ੁਰੂ;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ ਗਿਣਤੀ_2 = ਸਿਰਾ - ਸ਼ੁਰੂ + 1;
	ਪੂਰਨ byte_ਮਿਲਿਆ = 0; /* whether a != 0xff byte has been found */
	ਅਚਲ ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *pos;
	ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ max_loop_ਗਿਣਤੀ, i;

	/* scan bits until we hit a byte boundary */
	ਜਦੋਂ ((bitpos & 0x7) != 0 && ਗਿਣਤੀ_2 > 0) {
		ਜੇ (ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*ਬਾਹਰਲਾ = bitpos + bitmap->ਸ਼ੁਰੂ;
			ਵਾਪਸ 0;
		}
		bitpos++;
		ਗਿਣਤੀ_2--;
	}

	ਜੇ (!ਗਿਣਤੀ_2)
		ਵਾਪਸ ENOENT;

	pos = ((ਬਿਨਾਨਿਸ਼ਾਨੀ ਅੱਖਰ *)bp->bitarray) + (bitpos >> 3);
	/* scan bytes until 8-byte (64-bit) aligned */
	ਜਦੋਂ (ਗਿਣਤੀ_2 >= 8 && (((uintptr_t)pos) & 0x07)) {
		ਜੇ (*pos != 0) {
			byte_ਮਿਲਿਆ = 1;
			ਰੋਕੋ;
		}
		pos++;
		ਗਿਣਤੀ_2 -= 8;
		bitpos += 8;
	}

	ਜੇ (!byte_ਮਿਲਿਆ) {
		max_loop_ਗਿਣਤੀ = ਗਿਣਤੀ_2 >> 6; /* 8-byte blocks */
		i = max_loop_ਗਿਣਤੀ;
		ਜਦੋਂ (i) {
			ਜੇ (*((ਅਚਲ __u64 *)pos) != 0)
				ਰੋਕੋ;
			pos += 8;
			i--;
		}
		ਗਿਣਤੀ_2 -= 64 * (max_loop_ਗਿਣਤੀ - i);
		bitpos += 64 * (max_loop_ਗਿਣਤੀ - i);

		max_loop_ਗਿਣਤੀ = ਗਿਣਤੀ_2 >> 3;
		i = max_loop_ਗਿਣਤੀ;
		ਜਦੋਂ (i) {
			ਜੇ (*pos != 0) {
				byte_ਮਿਲਿਆ = 1;
				ਰੋਕੋ;
			}
			pos++;
			i--;
		}
		ਗਿਣਤੀ_2 -= 8 * (max_loop_ਗਿਣਤੀ - i);
		bitpos += 8 * (max_loop_ਗਿਣਤੀ - i);
	}

	/* Here either count < 8 or byte_found == 1. */
	ਜਦੋਂ (ਗਿਣਤੀ_2-- > 0) {
		ਜੇ (ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*ਬਾਹਰਲਾ = bitpos + bitmap->ਸ਼ੁਰੂ;
			ਵਾਪਸ 0;
		}
		bitpos++;
	}

	ਵਾਪਸ ENOENT;
}

ਬਣਤਰ ext2_bitmap_ops ext2fs_blkmap64_bitarray = {
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
	.ਸੈੱਟ_bmap_range = ba_ਸੈੱਟ_bmap_range,
	.get_bmap_range = ba_get_bmap_range,
	.clear_bmap = ba_clear_bmap,
	.print_stats = ba_print_stats,
	.ਲੱਭੋ_ਪਹਿਲਾ_zero = ba_ਲੱਭੋ_ਪਹਿਲਾ_zero,
	.ਲੱਭੋ_ਪਹਿਲਾ_ਸੈੱਟ = ba_ਲੱਭੋ_ਪਹਿਲਾ_ਸੈੱਟ
};
