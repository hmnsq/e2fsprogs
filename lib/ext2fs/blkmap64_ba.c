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

#അന്തര്ഭാവ "config.h"
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <string.h>
#എങ്കിൽ HAVE_UNISTD_H
#അന്തര്ഭാവ <unistd.h>
#endif
#അന്തര്ഭാവ <fcntl.h>
#അന്തര്ഭാവ <time.h>
#എങ്കിൽ HAVE_SYS_STAT_H
#അന്തര്ഭാവ <sys/stat.h>
#endif
#എങ്കിൽ HAVE_SYS_TYPES_H
#അന്തര്ഭാവ <sys/types.h>
#endif

#അന്തര്ഭാവ "ext2_fs.h"
#അന്തര്ഭാവ "ext2fsP.h"
#അന്തര്ഭാവ "bmap64.h"

/*
 * Private data for bit array implementation of bitmap ops.
 * Currently, this is just a pointer to our big flat hunk of memory,
 * exactly equivalent to the old-skool char * bitmap member.
 */

ഘടന ext2fs_ba_private_struct {
	അക്ഷരം *bitarray;
};

തരനിർവചനം ഘടന ext2fs_ba_private_struct *ext2fs_ba_private;

സ്ഥിതം errcode_t ba_alloc_private_data (ext2fs_generic_bitmap_64 bitmap)
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
	retval = ext2fs_get_mem(വലുപ്പം (ext2fs_ba_private), &bp);
	എങ്കിൽ (retval)
		തിരികെ retval;

	size = (size_t) (((bitmap->real_end - bitmap->start) / 8) + 1);

	retval = ext2fs_get_mem(size, &bp->bitarray);
	എങ്കിൽ (retval) {
		ext2fs_free_mem(&bp);
		bp = 0;
		തിരികെ retval;
	}
	bitmap->സ്വകാര്യ = (ശൂന്യം *) bp;
	തിരികെ 0;
}

സ്ഥിതം errcode_t ba_new_bmap(ext2_filsys fs EXT2FS_ATTR((unused)),
			     ext2fs_generic_bitmap_64 bitmap)
{
	ext2fs_ba_private bp;
	errcode_t	retval;
	size_t		size;

	retval = ba_alloc_private_data (bitmap);
	എങ്കിൽ (retval)
		തിരികെ retval;

	bp = (ext2fs_ba_private) bitmap->സ്വകാര്യ;
	size = (size_t) (((bitmap->real_end - bitmap->start) / 8) + 1);
	memset(bp->bitarray, 0, size);

	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം ba_free_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->സ്വകാര്യ;

	എങ്കിൽ (!bp)
		തിരികെ;

	എങ്കിൽ (bp->bitarray) {
		ext2fs_free_mem (&bp->bitarray);
		bp->bitarray = 0;
	}
	ext2fs_free_mem (&bp);
	bp = 0;
}

സ്ഥിതം errcode_t ba_copy_bmap(ext2fs_generic_bitmap_64 src,
			      ext2fs_generic_bitmap_64 dest)
{
	ext2fs_ba_private src_bp = (ext2fs_ba_private) src->സ്വകാര്യ;
	ext2fs_ba_private dest_bp;
	errcode_t retval;
	size_t size;

	retval = ba_alloc_private_data (dest);
	എങ്കിൽ (retval)
		തിരികെ retval;

	dest_bp = (ext2fs_ba_private) dest->സ്വകാര്യ;

	size = (size_t) (((src->real_end - src->start) / 8) + 1);
	memcpy (dest_bp->bitarray, src_bp->bitarray, size);

	തിരികെ 0;
}

സ്ഥിതം errcode_t ba_resize_bmap(ext2fs_generic_bitmap_64 bmap,
				__u64 new_end, __u64 new_real_end)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bmap->സ്വകാര്യ;
	errcode_t	retval;
	size_t		size, new_size;
	__u64		bitno;

	/*
	 * If we're expanding the bitmap, make sure all of the new
	 * parts of the bitmap are zero.
	 */
	എങ്കിൽ (new_end > bmap->end) {
		bitno = bmap->real_end;
		എങ്കിൽ (bitno > new_end)
			bitno = new_end;
		വേണ്ടി (; bitno > bmap->end; bitno--)
			ext2fs_clear_bit64(bitno - bmap->start, bp->bitarray);
	}
	എങ്കിൽ (new_real_end == bmap->real_end) {
		bmap->end = new_end;
		തിരികെ 0;
	}

	size = ((bmap->real_end - bmap->start) / 8) + 1;
	new_size = ((new_real_end - bmap->start) / 8) + 1;

	എങ്കിൽ (size != new_size) {
		retval = ext2fs_resize_mem(size, new_size, &bp->bitarray);
		എങ്കിൽ (retval)
			തിരികെ retval;
	}
	എങ്കിൽ (new_size > size)
		memset(bp->bitarray + size, 0, new_size - size);

	bmap->end = new_end;
	bmap->real_end = new_real_end;
	തിരികെ 0;

}

സ്ഥിതം പൂർണ്ണസംഖ്യ ba_mark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->സ്വകാര്യ;
	blk64_t bitno = (blk64_t) arg;

	തിരികെ ext2fs_set_bit64(bitno - bitmap->start, bp->bitarray);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ ba_unmark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->സ്വകാര്യ;
	blk64_t bitno = (blk64_t) arg;

	തിരികെ ext2fs_clear_bit64(bitno - bitmap->start, bp->bitarray);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ ba_test_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->സ്വകാര്യ;
	blk64_t bitno = (blk64_t) arg;

	തിരികെ ext2fs_test_bit64(bitno - bitmap->start, bp->bitarray);
}

സ്ഥിതം ശൂന്യം ba_mark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				unsigned പൂർണ്ണസംഖ്യ അക്കം)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->സ്വകാര്യ;
	blk64_t bitno = (blk64_t) arg;
	unsigned പൂർണ്ണസംഖ്യ i;

	വേണ്ടി (i = 0; i < അക്കം; i++)
		ext2fs_fast_set_bit64(bitno + i - bitmap->start, bp->bitarray);
}

സ്ഥിതം ശൂന്യം ba_unmark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				  unsigned പൂർണ്ണസംഖ്യ അക്കം)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->സ്വകാര്യ;
	blk64_t bitno = (blk64_t) arg;
	unsigned പൂർണ്ണസംഖ്യ i;

	വേണ്ടി (i = 0; i < അക്കം; i++)
		ext2fs_fast_clear_bit64(bitno + i - bitmap->start, bp->bitarray);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ ba_test_clear_bmap_extent(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, unsigned പൂർണ്ണസംഖ്യ len)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->സ്വകാര്യ;
	__u64 start_byte, len_byte = len >> 3;
	unsigned പൂർണ്ണസംഖ്യ start_bit, len_bit = len % 8;
	unsigned പൂർണ്ണസംഖ്യ first_bit = 0;
	unsigned പൂർണ്ണസംഖ്യ last_bit  = 0;
	പൂർണ്ണസംഖ്യ mark_എണ്ണം = 0;
	പൂർണ്ണസംഖ്യ mark_bit = 0;
	പൂർണ്ണസംഖ്യ i;
	സ്ഥിരം അക്ഷരം *ADDR;

	ADDR = bp->bitarray;
	start -= bitmap->start;
	start_byte = start >> 3;
	start_bit = start % 8;

	എങ്കിൽ (start_bit != 0) {
		/*
		 * The compared start block number or start inode number
		 * is not the first bit in a byte.
		 */
		mark_എണ്ണം = 8 - start_bit;
		എങ്കിൽ (len < 8 - start_bit) {
			mark_എണ്ണം = (പൂർണ്ണസംഖ്യ)len;
			mark_bit = len + start_bit - 1;
		} അല്ലെങ്കിൽ
			mark_bit = 7;

		വേണ്ടി (i = mark_എണ്ണം; i > 0; i--, mark_bit--)
			first_bit |= 1 << mark_bit;

		/*
		 * Compare blocks or inodes in the first byte.
		 * If there is any marked bit, this function returns 0.
		 */
		എങ്കിൽ (first_bit & ADDR[start_byte])
			തിരികെ 0;
		അല്ലെങ്കിൽ എങ്കിൽ (len <= 8 - start_bit)
			തിരികെ 1;

		start_byte++;
		len_bit = (len - mark_എണ്ണം) % 8;
		len_byte = (len - mark_എണ്ണം) >> 3;
	}

	/*
	 * The compared start block number or start inode number is
	 * the first bit in a byte.
	 */
	എങ്കിൽ (len_bit != 0) {
		/*
		 * The compared end block number or end inode number is
		 * not the last bit in a byte.
		 */
		വേണ്ടി (mark_bit = len_bit - 1; mark_bit >= 0; mark_bit--)
			last_bit |= 1 << mark_bit;

		/*
		 * Compare blocks or inodes in the last byte.
		 * If there is any marked bit, this function returns 0.
		 */
		എങ്കിൽ (last_bit & ADDR[start_byte + len_byte])
			തിരികെ 0;
		അല്ലെങ്കിൽ എങ്കിൽ (len_byte == 0)
			തിരികെ 1;
	}

	/* Check whether all bytes are 0 */
	തിരികെ ext2fs_mem_is_zero(ADDR + start_byte, len_byte);
}


സ്ഥിതം errcode_t ba_set_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, size_t അക്കം, ശൂന്യം *in)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->സ്വകാര്യ;

	memcpy (bp->bitarray + (start >> 3), in, (അക്കം + 7) >> 3);

	തിരികെ 0;
}

സ്ഥിതം errcode_t ba_get_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, size_t അക്കം, ശൂന്യം *out)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->സ്വകാര്യ;

	memcpy (out, bp->bitarray + (start >> 3), (അക്കം + 7) >> 3);

	തിരികെ 0;
}

സ്ഥിതം ശൂന്യം ba_clear_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->സ്വകാര്യ;

	memset(bp->bitarray, 0,
	       (size_t) (((bitmap->real_end - bitmap->start) / 8) + 1));
}

#ifdef ENABLE_BMAP_STATS
സ്ഥിതം ശൂന്യം ba_print_stats(ext2fs_generic_bitmap_64 bitmap)
{
	fprintf(stderr, "%16llu Bytes used by bitarray\n", (unsigned long long)
		((bitmap->real_end - bitmap->start) >> 3) + 1 +
		വലുപ്പം(ഘടന ext2fs_ba_private_struct));
}
#അല്ലെങ്കിൽ
സ്ഥിതം ശൂന്യം ba_print_stats(ext2fs_generic_bitmap_64 bitmap EXT2FS_ATTR((unused)))
{
}
#endif

/* Find the first zero bit between start and end, inclusive. */
സ്ഥിതം errcode_t ba_find_first_zero(ext2fs_generic_bitmap_64 bitmap,
				    __u64 start, __u64 end, __u64 *out)
{
	ext2fs_ba_private bp = (ext2fs_ba_private)bitmap->സ്വകാര്യ;
	unsigned long bitpos = start - bitmap->start;
	unsigned long എണ്ണം = end - start + 1;
	പൂർണ്ണസംഖ്യ byte_found = 0; /* whether a != 0xff byte has been found */
	സ്ഥിരം unsigned അക്ഷരം *pos;
	unsigned long max_loop_എണ്ണം, i;

	/* scan bits until we hit a byte boundary */
	ആകുമ്പോൾ ((bitpos & 0x7) != 0 && എണ്ണം > 0) {
		എങ്കിൽ (!ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*out = bitpos + bitmap->start;
			തിരികെ 0;
		}
		bitpos++;
		എണ്ണം--;
	}

	എങ്കിൽ (!എണ്ണം)
		തിരികെ ENOENT;

	pos = ((unsigned അക്ഷരം *)bp->bitarray) + (bitpos >> 3);
	/* scan bytes until 8-byte (64-bit) aligned */
	ആകുമ്പോൾ (എണ്ണം >= 8 && (((uintptr_t)pos) & 0x07)) {
		എങ്കിൽ (*pos != 0xff) {
			byte_found = 1;
			നിർത്തുക;
		}
		pos++;
		എണ്ണം -= 8;
		bitpos += 8;
	}

	എങ്കിൽ (!byte_found) {
		max_loop_എണ്ണം = എണ്ണം >> 6; /* 8-byte blocks */
		i = max_loop_എണ്ണം;
		ആകുമ്പോൾ (i) {
			എങ്കിൽ (*((സ്ഥിരം __u64 *)pos) != ((__u64)-1))
				നിർത്തുക;
			pos += 8;
			i--;
		}
		എണ്ണം -= 64 * (max_loop_എണ്ണം - i);
		bitpos += 64 * (max_loop_എണ്ണം - i);

		max_loop_എണ്ണം = എണ്ണം >> 3;
		i = max_loop_എണ്ണം;
		ആകുമ്പോൾ (i) {
			എങ്കിൽ (*pos != 0xff) {
				byte_found = 1;
				നിർത്തുക;
			}
			pos++;
			i--;
		}
		എണ്ണം -= 8 * (max_loop_എണ്ണം - i);
		bitpos += 8 * (max_loop_എണ്ണം - i);
	}

	/* Here either count < 8 or byte_found == 1. */
	ആകുമ്പോൾ (എണ്ണം-- > 0) {
		എങ്കിൽ (!ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*out = bitpos + bitmap->start;
			തിരികെ 0;
		}
		bitpos++;
	}

	തിരികെ ENOENT;
}

/* Find the first one bit between start and end, inclusive. */
സ്ഥിതം errcode_t ba_find_first_set(ext2fs_generic_bitmap_64 bitmap,
				    __u64 start, __u64 end, __u64 *out)
{
	ext2fs_ba_private bp = (ext2fs_ba_private)bitmap->സ്വകാര്യ;
	unsigned long bitpos = start - bitmap->start;
	unsigned long എണ്ണം = end - start + 1;
	പൂർണ്ണസംഖ്യ byte_found = 0; /* whether a != 0xff byte has been found */
	സ്ഥിരം unsigned അക്ഷരം *pos;
	unsigned long max_loop_എണ്ണം, i;

	/* scan bits until we hit a byte boundary */
	ആകുമ്പോൾ ((bitpos & 0x7) != 0 && എണ്ണം > 0) {
		എങ്കിൽ (ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*out = bitpos + bitmap->start;
			തിരികെ 0;
		}
		bitpos++;
		എണ്ണം--;
	}

	എങ്കിൽ (!എണ്ണം)
		തിരികെ ENOENT;

	pos = ((unsigned അക്ഷരം *)bp->bitarray) + (bitpos >> 3);
	/* scan bytes until 8-byte (64-bit) aligned */
	ആകുമ്പോൾ (എണ്ണം >= 8 && (((uintptr_t)pos) & 0x07)) {
		എങ്കിൽ (*pos != 0) {
			byte_found = 1;
			നിർത്തുക;
		}
		pos++;
		എണ്ണം -= 8;
		bitpos += 8;
	}

	എങ്കിൽ (!byte_found) {
		max_loop_എണ്ണം = എണ്ണം >> 6; /* 8-byte blocks */
		i = max_loop_എണ്ണം;
		ആകുമ്പോൾ (i) {
			എങ്കിൽ (*((സ്ഥിരം __u64 *)pos) != 0)
				നിർത്തുക;
			pos += 8;
			i--;
		}
		എണ്ണം -= 64 * (max_loop_എണ്ണം - i);
		bitpos += 64 * (max_loop_എണ്ണം - i);

		max_loop_എണ്ണം = എണ്ണം >> 3;
		i = max_loop_എണ്ണം;
		ആകുമ്പോൾ (i) {
			എങ്കിൽ (*pos != 0) {
				byte_found = 1;
				നിർത്തുക;
			}
			pos++;
			i--;
		}
		എണ്ണം -= 8 * (max_loop_എണ്ണം - i);
		bitpos += 8 * (max_loop_എണ്ണം - i);
	}

	/* Here either count < 8 or byte_found == 1. */
	ആകുമ്പോൾ (എണ്ണം-- > 0) {
		എങ്കിൽ (ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*out = bitpos + bitmap->start;
			തിരികെ 0;
		}
		bitpos++;
	}

	തിരികെ ENOENT;
}

ഘടന ext2_bitmap_ops ext2fs_blkmap64_bitarray = {
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
