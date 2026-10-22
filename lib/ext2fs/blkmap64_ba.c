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

#अन्तर्भाव "config.h"
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <string.h>
#जर HAVE_UNISTD_H
#अन्तर्भाव <unistd.h>
#endif
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <time.h>
#जर HAVE_SYS_STAT_H
#अन्तर्भाव <sys/stat.h>
#endif
#जर HAVE_SYS_TYPES_H
#अन्तर्भाव <sys/types.h>
#endif

#अन्तर्भाव "ext2_fs.h"
#अन्तर्भाव "ext2fsP.h"
#अन्तर्भाव "bmap64.h"

/*
 * Private data for bit array implementation of bitmap ops.
 * Currently, this is just a pointer to our big flat hunk of memory,
 * exactly equivalent to the old-skool char * bitmap member.
 */

संरचना ext2fs_ba_private_struct {
	अक्षर *bitarray;
};

प्रकारव्याख्या संरचना ext2fs_ba_private_struct *ext2fs_ba_private;

स्थिर errcode_t ba_alloc_private_data (ext2fs_generic_bitmap_64 bitmap)
{
	ext2fs_ba_private bp;
	errcode_t	retval;
	माप_प्रकार		size;

	/*
	 * Since we only have the one pointer, we could just shove our
	 * private data in the void *private field itself, but then
	 * we'd have to do a fair bit of rewriting if we ever added a
	 * field.  I'm agnostic.
	 */
	retval = ext2fs_get_mem(आकार (ext2fs_ba_private), &bp);
	जर (retval)
		परत retval;

	size = (माप_प्रकार) (((bitmap->real_end - bitmap->start) / 8) + 1);

	retval = ext2fs_get_mem(size, &bp->bitarray);
	जर (retval) {
		ext2fs_free_mem(&bp);
		bp = 0;
		परत retval;
	}
	bitmap->खाजगी = (शून्य *) bp;
	परत 0;
}

स्थिर errcode_t ba_new_bmap(ext2_filsys fs EXT2FS_ATTR((unused)),
			     ext2fs_generic_bitmap_64 bitmap)
{
	ext2fs_ba_private bp;
	errcode_t	retval;
	माप_प्रकार		size;

	retval = ba_alloc_private_data (bitmap);
	जर (retval)
		परत retval;

	bp = (ext2fs_ba_private) bitmap->खाजगी;
	size = (माप_प्रकार) (((bitmap->real_end - bitmap->start) / 8) + 1);
	स_रखो(bp->bitarray, 0, size);

	परत 0;
}

स्थिर शून्य ba_free_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->खाजगी;

	जर (!bp)
		परत;

	जर (bp->bitarray) {
		ext2fs_free_mem (&bp->bitarray);
		bp->bitarray = 0;
	}
	ext2fs_free_mem (&bp);
	bp = 0;
}

स्थिर errcode_t ba_copy_bmap(ext2fs_generic_bitmap_64 src,
			      ext2fs_generic_bitmap_64 dest)
{
	ext2fs_ba_private src_bp = (ext2fs_ba_private) src->खाजगी;
	ext2fs_ba_private dest_bp;
	errcode_t retval;
	माप_प्रकार size;

	retval = ba_alloc_private_data (dest);
	जर (retval)
		परत retval;

	dest_bp = (ext2fs_ba_private) dest->खाजगी;

	size = (माप_प्रकार) (((src->real_end - src->start) / 8) + 1);
	स_नकल (dest_bp->bitarray, src_bp->bitarray, size);

	परत 0;
}

स्थिर errcode_t ba_resize_bmap(ext2fs_generic_bitmap_64 bmap,
				__u64 new_end, __u64 new_real_end)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bmap->खाजगी;
	errcode_t	retval;
	माप_प्रकार		size, new_size;
	__u64		bitno;

	/*
	 * If we're expanding the bitmap, make sure all of the new
	 * parts of the bitmap are zero.
	 */
	जर (new_end > bmap->end) {
		bitno = bmap->real_end;
		जर (bitno > new_end)
			bitno = new_end;
		साठी (; bitno > bmap->end; bitno--)
			ext2fs_clear_bit64(bitno - bmap->start, bp->bitarray);
	}
	जर (new_real_end == bmap->real_end) {
		bmap->end = new_end;
		परत 0;
	}

	size = ((bmap->real_end - bmap->start) / 8) + 1;
	new_size = ((new_real_end - bmap->start) / 8) + 1;

	जर (size != new_size) {
		retval = ext2fs_resize_mem(size, new_size, &bp->bitarray);
		जर (retval)
			परत retval;
	}
	जर (new_size > size)
		स_रखो(bp->bitarray + size, 0, new_size - size);

	bmap->end = new_end;
	bmap->real_end = new_real_end;
	परत 0;

}

स्थिर पूर्णांक ba_mark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->खाजगी;
	blk64_t bitno = (blk64_t) arg;

	परत ext2fs_set_bit64(bitno - bitmap->start, bp->bitarray);
}

स्थिर पूर्णांक ba_unmark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->खाजगी;
	blk64_t bitno = (blk64_t) arg;

	परत ext2fs_clear_bit64(bitno - bitmap->start, bp->bitarray);
}

स्थिर पूर्णांक ba_test_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->खाजगी;
	blk64_t bitno = (blk64_t) arg;

	परत ext2fs_test_bit64(bitno - bitmap->start, bp->bitarray);
}

स्थिर शून्य ba_mark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				अचिन्हित पूर्णांक अंक)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->खाजगी;
	blk64_t bitno = (blk64_t) arg;
	अचिन्हित पूर्णांक i;

	साठी (i = 0; i < अंक; i++)
		ext2fs_fast_set_bit64(bitno + i - bitmap->start, bp->bitarray);
}

स्थिर शून्य ba_unmark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				  अचिन्हित पूर्णांक अंक)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->खाजगी;
	blk64_t bitno = (blk64_t) arg;
	अचिन्हित पूर्णांक i;

	साठी (i = 0; i < अंक; i++)
		ext2fs_fast_clear_bit64(bitno + i - bitmap->start, bp->bitarray);
}

स्थिर पूर्णांक ba_test_clear_bmap_extent(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, अचिन्हित पूर्णांक len)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->खाजगी;
	__u64 start_byte, len_byte = len >> 3;
	अचिन्हित पूर्णांक start_bit, len_bit = len % 8;
	अचिन्हित पूर्णांक first_bit = 0;
	अचिन्हित पूर्णांक last_bit  = 0;
	पूर्णांक mark_मोजणी = 0;
	पूर्णांक mark_bit = 0;
	पूर्णांक i;
	स्थिरांक अक्षर *ADDR;

	ADDR = bp->bitarray;
	start -= bitmap->start;
	start_byte = start >> 3;
	start_bit = start % 8;

	जर (start_bit != 0) {
		/*
		 * The compared start block number or start inode number
		 * is not the first bit in a byte.
		 */
		mark_मोजणी = 8 - start_bit;
		जर (len < 8 - start_bit) {
			mark_मोजणी = (पूर्णांक)len;
			mark_bit = len + start_bit - 1;
		} अन्यथा
			mark_bit = 7;

		साठी (i = mark_मोजणी; i > 0; i--, mark_bit--)
			first_bit |= 1 << mark_bit;

		/*
		 * Compare blocks or inodes in the first byte.
		 * If there is any marked bit, this function returns 0.
		 */
		जर (first_bit & ADDR[start_byte])
			परत 0;
		अन्यथा जर (len <= 8 - start_bit)
			परत 1;

		start_byte++;
		len_bit = (len - mark_मोजणी) % 8;
		len_byte = (len - mark_मोजणी) >> 3;
	}

	/*
	 * The compared start block number or start inode number is
	 * the first bit in a byte.
	 */
	जर (len_bit != 0) {
		/*
		 * The compared end block number or end inode number is
		 * not the last bit in a byte.
		 */
		साठी (mark_bit = len_bit - 1; mark_bit >= 0; mark_bit--)
			last_bit |= 1 << mark_bit;

		/*
		 * Compare blocks or inodes in the last byte.
		 * If there is any marked bit, this function returns 0.
		 */
		जर (last_bit & ADDR[start_byte + len_byte])
			परत 0;
		अन्यथा जर (len_byte == 0)
			परत 1;
	}

	/* Check whether all bytes are 0 */
	परत ext2fs_mem_is_zero(ADDR + start_byte, len_byte);
}


स्थिर errcode_t ba_set_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, माप_प्रकार अंक, शून्य *in)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->खाजगी;

	स_नकल (bp->bitarray + (start >> 3), in, (अंक + 7) >> 3);

	परत 0;
}

स्थिर errcode_t ba_get_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, माप_प्रकार अंक, शून्य *out)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->खाजगी;

	स_नकल (out, bp->bitarray + (start >> 3), (अंक + 7) >> 3);

	परत 0;
}

स्थिर शून्य ba_clear_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->खाजगी;

	स_रखो(bp->bitarray, 0,
	       (माप_प्रकार) (((bitmap->real_end - bitmap->start) / 8) + 1));
}

#ifdef ENABLE_BMAP_STATS
स्थिर शून्य ba_print_stats(ext2fs_generic_bitmap_64 bitmap)
{
	ख_लिखो(मानक_त्रुटि, "%16llu Bytes used by bitarray\n", (अचिन्हित दीर्घ दीर्घ)
		((bitmap->real_end - bitmap->start) >> 3) + 1 +
		आकार(संरचना ext2fs_ba_private_struct));
}
#अन्यथा
स्थिर शून्य ba_print_stats(ext2fs_generic_bitmap_64 bitmap EXT2FS_ATTR((unused)))
{
}
#endif

/* Find the first zero bit between start and end, inclusive. */
स्थिर errcode_t ba_find_first_zero(ext2fs_generic_bitmap_64 bitmap,
				    __u64 start, __u64 end, __u64 *out)
{
	ext2fs_ba_private bp = (ext2fs_ba_private)bitmap->खाजगी;
	अचिन्हित दीर्घ bitpos = start - bitmap->start;
	अचिन्हित दीर्घ मोजणी = end - start + 1;
	पूर्णांक byte_found = 0; /* whether a != 0xff byte has been found */
	स्थिरांक अचिन्हित अक्षर *pos;
	अचिन्हित दीर्घ max_loop_मोजणी, i;

	/* scan bits until we hit a byte boundary */
	पर्यंत ((bitpos & 0x7) != 0 && मोजणी > 0) {
		जर (!ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*out = bitpos + bitmap->start;
			परत 0;
		}
		bitpos++;
		मोजणी--;
	}

	जर (!मोजणी)
		परत ENOENT;

	pos = ((अचिन्हित अक्षर *)bp->bitarray) + (bitpos >> 3);
	/* scan bytes until 8-byte (64-bit) aligned */
	पर्यंत (मोजणी >= 8 && (((uintptr_t)pos) & 0x07)) {
		जर (*pos != 0xff) {
			byte_found = 1;
			थांबा;
		}
		pos++;
		मोजणी -= 8;
		bitpos += 8;
	}

	जर (!byte_found) {
		max_loop_मोजणी = मोजणी >> 6; /* 8-byte blocks */
		i = max_loop_मोजणी;
		पर्यंत (i) {
			जर (*((स्थिरांक __u64 *)pos) != ((__u64)-1))
				थांबा;
			pos += 8;
			i--;
		}
		मोजणी -= 64 * (max_loop_मोजणी - i);
		bitpos += 64 * (max_loop_मोजणी - i);

		max_loop_मोजणी = मोजणी >> 3;
		i = max_loop_मोजणी;
		पर्यंत (i) {
			जर (*pos != 0xff) {
				byte_found = 1;
				थांबा;
			}
			pos++;
			i--;
		}
		मोजणी -= 8 * (max_loop_मोजणी - i);
		bitpos += 8 * (max_loop_मोजणी - i);
	}

	/* Here either count < 8 or byte_found == 1. */
	पर्यंत (मोजणी-- > 0) {
		जर (!ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*out = bitpos + bitmap->start;
			परत 0;
		}
		bitpos++;
	}

	परत ENOENT;
}

/* Find the first one bit between start and end, inclusive. */
स्थिर errcode_t ba_find_first_set(ext2fs_generic_bitmap_64 bitmap,
				    __u64 start, __u64 end, __u64 *out)
{
	ext2fs_ba_private bp = (ext2fs_ba_private)bitmap->खाजगी;
	अचिन्हित दीर्घ bitpos = start - bitmap->start;
	अचिन्हित दीर्घ मोजणी = end - start + 1;
	पूर्णांक byte_found = 0; /* whether a != 0xff byte has been found */
	स्थिरांक अचिन्हित अक्षर *pos;
	अचिन्हित दीर्घ max_loop_मोजणी, i;

	/* scan bits until we hit a byte boundary */
	पर्यंत ((bitpos & 0x7) != 0 && मोजणी > 0) {
		जर (ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*out = bitpos + bitmap->start;
			परत 0;
		}
		bitpos++;
		मोजणी--;
	}

	जर (!मोजणी)
		परत ENOENT;

	pos = ((अचिन्हित अक्षर *)bp->bitarray) + (bitpos >> 3);
	/* scan bytes until 8-byte (64-bit) aligned */
	पर्यंत (मोजणी >= 8 && (((uintptr_t)pos) & 0x07)) {
		जर (*pos != 0) {
			byte_found = 1;
			थांबा;
		}
		pos++;
		मोजणी -= 8;
		bitpos += 8;
	}

	जर (!byte_found) {
		max_loop_मोजणी = मोजणी >> 6; /* 8-byte blocks */
		i = max_loop_मोजणी;
		पर्यंत (i) {
			जर (*((स्थिरांक __u64 *)pos) != 0)
				थांबा;
			pos += 8;
			i--;
		}
		मोजणी -= 64 * (max_loop_मोजणी - i);
		bitpos += 64 * (max_loop_मोजणी - i);

		max_loop_मोजणी = मोजणी >> 3;
		i = max_loop_मोजणी;
		पर्यंत (i) {
			जर (*pos != 0) {
				byte_found = 1;
				थांबा;
			}
			pos++;
			i--;
		}
		मोजणी -= 8 * (max_loop_मोजणी - i);
		bitpos += 8 * (max_loop_मोजणी - i);
	}

	/* Here either count < 8 or byte_found == 1. */
	पर्यंत (मोजणी-- > 0) {
		जर (ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*out = bitpos + bitmap->start;
			परत 0;
		}
		bitpos++;
	}

	परत ENOENT;
}

संरचना ext2_bitmap_ops ext2fs_blkmap64_bitarray = {
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
