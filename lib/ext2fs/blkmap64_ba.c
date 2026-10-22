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
#ಆದರೆ HAVE_UNISTD_H
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <time.h>
#ಆದರೆ HAVE_SYS_STAT_H
#ಅನ್ತರ್ಭಾವ <sys/stat.h>
#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ HAVE_SYS_TYPES_H
#ಅನ್ತರ್ಭಾವ <sys/types.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "ext2_fs.h"
#ಅನ್ತರ್ಭಾವ "ext2fsP.h"
#ಅನ್ತರ್ಭಾವ "bmap64.h"

/*
 * Private data for bit array implementation of bitmap ops.
 * Currently, this is just a pointer to our big flat hunk of memory,
 * exactly equivalent to the old-skool char * bitmap member.
 */

ರಚನೆ ext2fs_ba_ಖಾಸಗಿ_struct {
	ಅಕ್ಷರ *bitarray;
};

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ ext2fs_ba_ಖಾಸಗಿ_struct *ext2fs_ba_ಖಾಸಗಿ;

ಸ್ಥಿರ errcode_t ba_alloc_ಖಾಸಗಿ_ದತ್ತಾಂಶ (ext2fs_generic_bitmap_64 bitmap)
{
	ext2fs_ba_ಖಾಸಗಿ bp;
	errcode_t	retval;
	size_t		ಗಾತ್ರ_2;

	/*
	 * Since we only have the one pointer, we could just shove our
	 * private data in the void *private field itself, but then
	 * we'd have to do a fair bit of rewriting if we ever added a
	 * field.  I'm agnostic.
	 */
	retval = ext2fs_get_mem(ಗಾತ್ರ (ext2fs_ba_ಖಾಸಗಿ), &bp);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	ಗಾತ್ರ_2 = (size_t) (((bitmap->real_ಕೊನೆ - bitmap->ಆರಂಭ) / 8) + 1);

	retval = ext2fs_get_mem(ಗಾತ್ರ_2, &bp->bitarray);
	ಆದರೆ (retval) {
		ext2fs_free_mem(&bp);
		bp = 0;
		ಹಿಂತಿರುಗಿ retval;
	}
	bitmap->ಖಾಸಗಿ = (ಶೂನ್ಯ *) bp;
	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ errcode_t ba_new_bmap(ext2_filsys fs EXT2FS_ATTR((ಬಳಸದ)),
			     ext2fs_generic_bitmap_64 bitmap)
{
	ext2fs_ba_ಖಾಸಗಿ bp;
	errcode_t	retval;
	size_t		ಗಾತ್ರ_2;

	retval = ba_alloc_ಖಾಸಗಿ_ದತ್ತಾಂಶ (bitmap);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	bp = (ext2fs_ba_ಖಾಸಗಿ) bitmap->ಖಾಸಗಿ;
	ಗಾತ್ರ_2 = (size_t) (((bitmap->real_ಕೊನೆ - bitmap->ಆರಂಭ) / 8) + 1);
	memset(bp->bitarray, 0, ಗಾತ್ರ_2);

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ ba_free_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	ext2fs_ba_ಖಾಸಗಿ bp = (ext2fs_ba_ಖಾಸಗಿ) bitmap->ಖಾಸಗಿ;

	ಆದರೆ (!bp)
		ಹಿಂತಿರುಗಿ;

	ಆದರೆ (bp->bitarray) {
		ext2fs_free_mem (&bp->bitarray);
		bp->bitarray = 0;
	}
	ext2fs_free_mem (&bp);
	bp = 0;
}

ಸ್ಥಿರ errcode_t ba_copy_bmap(ext2fs_generic_bitmap_64 src,
			      ext2fs_generic_bitmap_64 dest)
{
	ext2fs_ba_ಖಾಸಗಿ src_bp = (ext2fs_ba_ಖಾಸಗಿ) src->ಖಾಸಗಿ;
	ext2fs_ba_ಖಾಸಗಿ dest_bp;
	errcode_t retval;
	size_t ಗಾತ್ರ_2;

	retval = ba_alloc_ಖಾಸಗಿ_ದತ್ತಾಂಶ (dest);
	ಆದರೆ (retval)
		ಹಿಂತಿರುಗಿ retval;

	dest_bp = (ext2fs_ba_ಖಾಸಗಿ) dest->ಖಾಸಗಿ;

	ಗಾತ್ರ_2 = (size_t) (((src->real_ಕೊನೆ - src->ಆರಂಭ) / 8) + 1);
	ಸ್ಮರಣೆ_ನಕಲು (dest_bp->bitarray, src_bp->bitarray, ಗಾತ್ರ_2);

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ errcode_t ba_resize_bmap(ext2fs_generic_bitmap_64 bmap,
				__u64 new_ಕೊನೆ, __u64 new_real_ಕೊನೆ)
{
	ext2fs_ba_ಖಾಸಗಿ bp = (ext2fs_ba_ಖಾಸಗಿ) bmap->ಖಾಸಗಿ;
	errcode_t	retval;
	size_t		ಗಾತ್ರ_2, new_ಗಾತ್ರ;
	__u64		bitno;

	/*
	 * If we're expanding the bitmap, make sure all of the new
	 * parts of the bitmap are zero.
	 */
	ಆದರೆ (new_ಕೊನೆ > bmap->ಕೊನೆ) {
		bitno = bmap->real_ಕೊನೆ;
		ಆದರೆ (bitno > new_ಕೊನೆ)
			bitno = new_ಕೊನೆ;
		ಗಾಗಿ (; bitno > bmap->ಕೊನೆ; bitno--)
			ext2fs_clear_bit64(bitno - bmap->ಆರಂಭ, bp->bitarray);
	}
	ಆದರೆ (new_real_ಕೊನೆ == bmap->real_ಕೊನೆ) {
		bmap->ಕೊನೆ = new_ಕೊನೆ;
		ಹಿಂತಿರುಗಿ 0;
	}

	ಗಾತ್ರ_2 = ((bmap->real_ಕೊನೆ - bmap->ಆರಂಭ) / 8) + 1;
	new_ಗಾತ್ರ = ((new_real_ಕೊನೆ - bmap->ಆರಂಭ) / 8) + 1;

	ಆದರೆ (ಗಾತ್ರ_2 != new_ಗಾತ್ರ) {
		retval = ext2fs_resize_mem(ಗಾತ್ರ_2, new_ಗಾತ್ರ, &bp->bitarray);
		ಆದರೆ (retval)
			ಹಿಂತಿರುಗಿ retval;
	}
	ಆದರೆ (new_ಗಾತ್ರ > ಗಾತ್ರ_2)
		memset(bp->bitarray + ಗಾತ್ರ_2, 0, new_ಗಾತ್ರ - ಗಾತ್ರ_2);

	bmap->ಕೊನೆ = new_ಕೊನೆ;
	bmap->real_ಕೊನೆ = new_real_ಕೊನೆ;
	ಹಿಂತಿರುಗಿ 0;

}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ba_mark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ext2fs_ba_ಖಾಸಗಿ bp = (ext2fs_ba_ಖಾಸಗಿ) bitmap->ಖಾಸಗಿ;
	blk64_t bitno = (blk64_t) arg;

	ಹಿಂತಿರುಗಿ ext2fs_ಗಣ_bit64(bitno - bitmap->ಆರಂಭ, bp->bitarray);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ba_unmark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ext2fs_ba_ಖಾಸಗಿ bp = (ext2fs_ba_ಖಾಸಗಿ) bitmap->ಖಾಸಗಿ;
	blk64_t bitno = (blk64_t) arg;

	ಹಿಂತಿರುಗಿ ext2fs_clear_bit64(bitno - bitmap->ಆರಂಭ, bp->bitarray);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ba_test_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ext2fs_ba_ಖಾಸಗಿ bp = (ext2fs_ba_ಖಾಸಗಿ) bitmap->ಖಾಸಗಿ;
	blk64_t bitno = (blk64_t) arg;

	ಹಿಂತಿರುಗಿ ext2fs_test_bit64(bitno - bitmap->ಆರಂಭ, bp->bitarray);
}

ಸ್ಥಿರ ಶೂನ್ಯ ba_mark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಅಂಕೆ)
{
	ext2fs_ba_ಖಾಸಗಿ bp = (ext2fs_ba_ಖಾಸಗಿ) bitmap->ಖಾಸಗಿ;
	blk64_t bitno = (blk64_t) arg;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ i;

	ಗಾಗಿ (i = 0; i < ಅಂಕೆ; i++)
		ext2fs_fast_ಗಣ_bit64(bitno + i - bitmap->ಆರಂಭ, bp->bitarray);
}

ಸ್ಥಿರ ಶೂನ್ಯ ba_unmark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				  ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಅಂಕೆ)
{
	ext2fs_ba_ಖಾಸಗಿ bp = (ext2fs_ba_ಖಾಸಗಿ) bitmap->ಖಾಸಗಿ;
	blk64_t bitno = (blk64_t) arg;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ i;

	ಗಾಗಿ (i = 0; i < ಅಂಕೆ; i++)
		ext2fs_fast_clear_bit64(bitno + i - bitmap->ಆರಂಭ, bp->bitarray);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ ba_test_clear_bmap_extent(ext2fs_generic_bitmap_64 bitmap,
				     __u64 ಆರಂಭ, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ len)
{
	ext2fs_ba_ಖಾಸಗಿ bp = (ext2fs_ba_ಖಾಸಗಿ) bitmap->ಖಾಸಗಿ;
	__u64 ಆರಂಭ_byte, len_byte = len >> 3;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಆರಂಭ_ಬಿಟ್, len_ಬಿಟ್ = len % 8;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಮೊದಲ_ಬಿಟ್ = 0;
	ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ ಕೊನೆಯ_ಬಿಟ್  = 0;
	ಪೂರ್ಣಾಂಕ mark_ಎಣಿಕೆ = 0;
	ಪೂರ್ಣಾಂಕ mark_ಬಿಟ್ = 0;
	ಪೂರ್ಣಾಂಕ i;
	ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ADDR;

	ADDR = bp->bitarray;
	ಆರಂಭ -= bitmap->ಆರಂಭ;
	ಆರಂಭ_byte = ಆರಂಭ >> 3;
	ಆರಂಭ_ಬಿಟ್ = ಆರಂಭ % 8;

	ಆದರೆ (ಆರಂಭ_ಬಿಟ್ != 0) {
		/*
		 * The compared start block number or start inode number
		 * is not the first bit in a byte.
		 */
		mark_ಎಣಿಕೆ = 8 - ಆರಂಭ_ಬಿಟ್;
		ಆದರೆ (len < 8 - ಆರಂಭ_ಬಿಟ್) {
			mark_ಎಣಿಕೆ = (ಪೂರ್ಣಾಂಕ)len;
			mark_ಬಿಟ್ = len + ಆರಂಭ_ಬಿಟ್ - 1;
		} ಇಲ್ಲದಿದ್ದರೆ
			mark_ಬಿಟ್ = 7;

		ಗಾಗಿ (i = mark_ಎಣಿಕೆ; i > 0; i--, mark_ಬಿಟ್--)
			ಮೊದಲ_ಬಿಟ್ |= 1 << mark_ಬಿಟ್;

		/*
		 * Compare blocks or inodes in the first byte.
		 * If there is any marked bit, this function returns 0.
		 */
		ಆದರೆ (ಮೊದಲ_ಬಿಟ್ & ADDR[ಆರಂಭ_byte])
			ಹಿಂತಿರುಗಿ 0;
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (len <= 8 - ಆರಂಭ_ಬಿಟ್)
			ಹಿಂತಿರುಗಿ 1;

		ಆರಂಭ_byte++;
		len_ಬಿಟ್ = (len - mark_ಎಣಿಕೆ) % 8;
		len_byte = (len - mark_ಎಣಿಕೆ) >> 3;
	}

	/*
	 * The compared start block number or start inode number is
	 * the first bit in a byte.
	 */
	ಆದರೆ (len_ಬಿಟ್ != 0) {
		/*
		 * The compared end block number or end inode number is
		 * not the last bit in a byte.
		 */
		ಗಾಗಿ (mark_ಬಿಟ್ = len_ಬಿಟ್ - 1; mark_ಬಿಟ್ >= 0; mark_ಬಿಟ್--)
			ಕೊನೆಯ_ಬಿಟ್ |= 1 << mark_ಬಿಟ್;

		/*
		 * Compare blocks or inodes in the last byte.
		 * If there is any marked bit, this function returns 0.
		 */
		ಆದರೆ (ಕೊನೆಯ_ಬಿಟ್ & ADDR[ಆರಂಭ_byte + len_byte])
			ಹಿಂತಿರುಗಿ 0;
		ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (len_byte == 0)
			ಹಿಂತಿರುಗಿ 1;
	}

	/* Check whether all bytes are 0 */
	ಹಿಂತಿರುಗಿ ext2fs_mem_is_zero(ADDR + ಆರಂಭ_byte, len_byte);
}


ಸ್ಥಿರ errcode_t ba_ಗಣ_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 ಆರಂಭ, size_t ಅಂಕೆ, ಶೂನ್ಯ *ಒಳ)
{
	ext2fs_ba_ಖಾಸಗಿ bp = (ext2fs_ba_ಖಾಸಗಿ) bitmap->ಖಾಸಗಿ;

	ಸ್ಮರಣೆ_ನಕಲು (bp->bitarray + (ಆರಂಭ >> 3), ಒಳ, (ಅಂಕೆ + 7) >> 3);

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ errcode_t ba_get_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 ಆರಂಭ, size_t ಅಂಕೆ, ಶೂನ್ಯ *ಹೊರಹೋಗು)
{
	ext2fs_ba_ಖಾಸಗಿ bp = (ext2fs_ba_ಖಾಸಗಿ) bitmap->ಖಾಸಗಿ;

	ಸ್ಮರಣೆ_ನಕಲು (ಹೊರಹೋಗು, bp->bitarray + (ಆರಂಭ >> 3), (ಅಂಕೆ + 7) >> 3);

	ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಶೂನ್ಯ ba_clear_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	ext2fs_ba_ಖಾಸಗಿ bp = (ext2fs_ba_ಖಾಸಗಿ) bitmap->ಖಾಸಗಿ;

	memset(bp->bitarray, 0,
	       (size_t) (((bitmap->real_ಕೊನೆ - bitmap->ಆರಂಭ) / 8) + 1));
}

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ ENABLE_BMAP_STATS
ಸ್ಥಿರ ಶೂನ್ಯ ba_print_stats(ext2fs_generic_bitmap_64 bitmap)
{
	fprintf(stderr, "%16llu Bytes used by bitarray\n", (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ದೀರ್ಘ)
		((bitmap->real_ಕೊನೆ - bitmap->ಆರಂಭ) >> 3) + 1 +
		ಗಾತ್ರ(ರಚನೆ ext2fs_ba_ಖಾಸಗಿ_struct));
}
#ಇಲ್ಲದಿದ್ದರೆ
ಸ್ಥಿರ ಶೂನ್ಯ ba_print_stats(ext2fs_generic_bitmap_64 bitmap EXT2FS_ATTR((ಬಳಸದ)))
{
}
#ಆದರೆ_ಅಂತ್ಯ

/* Find the first zero bit between start and end, inclusive. */
ಸ್ಥಿರ errcode_t ba_ಕಂಡುಹಿಡಿ_ಮೊದಲ_zero(ext2fs_generic_bitmap_64 bitmap,
				    __u64 ಆರಂಭ, __u64 ಕೊನೆ, __u64 *ಹೊರಹೋಗು)
{
	ext2fs_ba_ಖಾಸಗಿ bp = (ext2fs_ba_ಖಾಸಗಿ)bitmap->ಖಾಸಗಿ;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ bitpos = ಆರಂಭ - bitmap->ಆರಂಭ;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಎಣಿಕೆ_2 = ಕೊನೆ - ಆರಂಭ + 1;
	ಪೂರ್ಣಾಂಕ byte_ಸಿಕ್ಕಿತು = 0; /* whether a != 0xff byte has been found */
	ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *pos;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ max_loop_ಎಣಿಕೆ, i;

	/* scan bits until we hit a byte boundary */
	ಆಗ ((bitpos & 0x7) != 0 && ಎಣಿಕೆ_2 > 0) {
		ಆದರೆ (!ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*ಹೊರಹೋಗು = bitpos + bitmap->ಆರಂಭ;
			ಹಿಂತಿರುಗಿ 0;
		}
		bitpos++;
		ಎಣಿಕೆ_2--;
	}

	ಆದರೆ (!ಎಣಿಕೆ_2)
		ಹಿಂತಿರುಗಿ ENOENT;

	pos = ((ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *)bp->bitarray) + (bitpos >> 3);
	/* scan bytes until 8-byte (64-bit) aligned */
	ಆಗ (ಎಣಿಕೆ_2 >= 8 && (((uintptr_t)pos) & 0x07)) {
		ಆದರೆ (*pos != 0xff) {
			byte_ಸಿಕ್ಕಿತು = 1;
			ನಿಲ್ಲಿಸು;
		}
		pos++;
		ಎಣಿಕೆ_2 -= 8;
		bitpos += 8;
	}

	ಆದರೆ (!byte_ಸಿಕ್ಕಿತು) {
		max_loop_ಎಣಿಕೆ = ಎಣಿಕೆ_2 >> 6; /* 8-byte blocks */
		i = max_loop_ಎಣಿಕೆ;
		ಆಗ (i) {
			ಆದರೆ (*((ಸ್ಥಿರಾಂಕ __u64 *)pos) != ((__u64)-1))
				ನಿಲ್ಲಿಸು;
			pos += 8;
			i--;
		}
		ಎಣಿಕೆ_2 -= 64 * (max_loop_ಎಣಿಕೆ - i);
		bitpos += 64 * (max_loop_ಎಣಿಕೆ - i);

		max_loop_ಎಣಿಕೆ = ಎಣಿಕೆ_2 >> 3;
		i = max_loop_ಎಣಿಕೆ;
		ಆಗ (i) {
			ಆದರೆ (*pos != 0xff) {
				byte_ಸಿಕ್ಕಿತು = 1;
				ನಿಲ್ಲಿಸು;
			}
			pos++;
			i--;
		}
		ಎಣಿಕೆ_2 -= 8 * (max_loop_ಎಣಿಕೆ - i);
		bitpos += 8 * (max_loop_ಎಣಿಕೆ - i);
	}

	/* Here either count < 8 or byte_found == 1. */
	ಆಗ (ಎಣಿಕೆ_2-- > 0) {
		ಆದರೆ (!ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*ಹೊರಹೋಗು = bitpos + bitmap->ಆರಂಭ;
			ಹಿಂತಿರುಗಿ 0;
		}
		bitpos++;
	}

	ಹಿಂತಿರುಗಿ ENOENT;
}

/* Find the first one bit between start and end, inclusive. */
ಸ್ಥಿರ errcode_t ba_ಕಂಡುಹಿಡಿ_ಮೊದಲ_ಗಣ(ext2fs_generic_bitmap_64 bitmap,
				    __u64 ಆರಂಭ, __u64 ಕೊನೆ, __u64 *ಹೊರಹೋಗು)
{
	ext2fs_ba_ಖಾಸಗಿ bp = (ext2fs_ba_ಖಾಸಗಿ)bitmap->ಖಾಸಗಿ;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ bitpos = ಆರಂಭ - bitmap->ಆರಂಭ;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ ಎಣಿಕೆ_2 = ಕೊನೆ - ಆರಂಭ + 1;
	ಪೂರ್ಣಾಂಕ byte_ಸಿಕ್ಕಿತು = 0; /* whether a != 0xff byte has been found */
	ಸ್ಥಿರಾಂಕ ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *pos;
	ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ max_loop_ಎಣಿಕೆ, i;

	/* scan bits until we hit a byte boundary */
	ಆಗ ((bitpos & 0x7) != 0 && ಎಣಿಕೆ_2 > 0) {
		ಆದರೆ (ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*ಹೊರಹೋಗು = bitpos + bitmap->ಆರಂಭ;
			ಹಿಂತಿರುಗಿ 0;
		}
		bitpos++;
		ಎಣಿಕೆ_2--;
	}

	ಆದರೆ (!ಎಣಿಕೆ_2)
		ಹಿಂತಿರುಗಿ ENOENT;

	pos = ((ಚಿಹ್ನೆರಹಿತ ಅಕ್ಷರ *)bp->bitarray) + (bitpos >> 3);
	/* scan bytes until 8-byte (64-bit) aligned */
	ಆಗ (ಎಣಿಕೆ_2 >= 8 && (((uintptr_t)pos) & 0x07)) {
		ಆದರೆ (*pos != 0) {
			byte_ಸಿಕ್ಕಿತು = 1;
			ನಿಲ್ಲಿಸು;
		}
		pos++;
		ಎಣಿಕೆ_2 -= 8;
		bitpos += 8;
	}

	ಆದರೆ (!byte_ಸಿಕ್ಕಿತು) {
		max_loop_ಎಣಿಕೆ = ಎಣಿಕೆ_2 >> 6; /* 8-byte blocks */
		i = max_loop_ಎಣಿಕೆ;
		ಆಗ (i) {
			ಆದರೆ (*((ಸ್ಥಿರಾಂಕ __u64 *)pos) != 0)
				ನಿಲ್ಲಿಸು;
			pos += 8;
			i--;
		}
		ಎಣಿಕೆ_2 -= 64 * (max_loop_ಎಣಿಕೆ - i);
		bitpos += 64 * (max_loop_ಎಣಿಕೆ - i);

		max_loop_ಎಣಿಕೆ = ಎಣಿಕೆ_2 >> 3;
		i = max_loop_ಎಣಿಕೆ;
		ಆಗ (i) {
			ಆದರೆ (*pos != 0) {
				byte_ಸಿಕ್ಕಿತು = 1;
				ನಿಲ್ಲಿಸು;
			}
			pos++;
			i--;
		}
		ಎಣಿಕೆ_2 -= 8 * (max_loop_ಎಣಿಕೆ - i);
		bitpos += 8 * (max_loop_ಎಣಿಕೆ - i);
	}

	/* Here either count < 8 or byte_found == 1. */
	ಆಗ (ಎಣಿಕೆ_2-- > 0) {
		ಆದರೆ (ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*ಹೊರಹೋಗು = bitpos + bitmap->ಆರಂಭ;
			ಹಿಂತಿರುಗಿ 0;
		}
		bitpos++;
	}

	ಹಿಂತಿರುಗಿ ENOENT;
}

ರಚನೆ ext2_bitmap_ops ext2fs_blkmap64_bitarray = {
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
	.ಗಣ_bmap_range = ba_ಗಣ_bmap_range,
	.get_bmap_range = ba_get_bmap_range,
	.clear_bmap = ba_clear_bmap,
	.print_stats = ba_print_stats,
	.ಕಂಡುಹಿಡಿ_ಮೊದಲ_zero = ba_ಕಂಡುಹಿಡಿ_ಮೊದಲ_zero,
	.ಕಂಡುಹಿಡಿ_ಮೊದಲ_ಗಣ = ba_ಕಂಡುಹಿಡಿ_ಮೊದಲ_ಗಣ
};
