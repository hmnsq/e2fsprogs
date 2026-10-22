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

#समावेश "config.h"
#समावेश <stdio.h>
#समावेश <string.h>
#अगर HAVE_UNISTD_H
#समावेश <unistd.h>
#पूर्ण_अगर
#समावेश <fcntl.h>
#समावेश <time.h>
#अगर HAVE_SYS_STAT_H
#समावेश <sys/stat.h>
#पूर्ण_अगर
#अगर HAVE_SYS_TYPES_H
#समावेश <sys/types.h>
#पूर्ण_अगर

#समावेश "ext2_fs.h"
#समावेश "ext2fsP.h"
#समावेश "bmap64.h"

/*
 * Private data for bit array implementation of bitmap ops.
 * Currently, this is just a pointer to our big flat hunk of memory,
 * exactly equivalent to the old-skool char * bitmap member.
 */

काष्ठा ext2fs_ba_private_struct {
	अक्षर *bitarray;
};

प्रकार काष्ठा ext2fs_ba_private_struct *ext2fs_ba_private;

अटल errcode_t ba_alloc_private_तथ्य (ext2fs_generic_bitmap_64 bitmap)
{
	ext2fs_ba_private bp;
	errcode_t	retval;
	माप_प्रकार		आकार;

	/*
	 * Since we only have the one pointer, we could just shove our
	 * private data in the void *private field itself, but then
	 * we'd have to do a fair bit of rewriting if we ever added a
	 * field.  I'm agnostic.
	 */
	retval = ext2fs_get_mem(माप (ext2fs_ba_private), &bp);
	अगर (retval)
		वापस retval;

	आकार = (माप_प्रकार) (((bitmap->real_end - bitmap->start) / 8) + 1);

	retval = ext2fs_get_mem(आकार, &bp->bitarray);
	अगर (retval) {
		ext2fs_free_mem(&bp);
		bp = 0;
		वापस retval;
	}
	bitmap->निजी = (व्योम *) bp;
	वापस 0;
}

अटल errcode_t ba_new_bmap(ext2_filsys fs EXT2FS_ATTR((unused)),
			     ext2fs_generic_bitmap_64 bitmap)
{
	ext2fs_ba_private bp;
	errcode_t	retval;
	माप_प्रकार		आकार;

	retval = ba_alloc_private_तथ्य (bitmap);
	अगर (retval)
		वापस retval;

	bp = (ext2fs_ba_private) bitmap->निजी;
	आकार = (माप_प्रकार) (((bitmap->real_end - bitmap->start) / 8) + 1);
	स_रखो(bp->bitarray, 0, आकार);

	वापस 0;
}

अटल व्योम ba_free_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->निजी;

	अगर (!bp)
		वापस;

	अगर (bp->bitarray) {
		ext2fs_free_mem (&bp->bitarray);
		bp->bitarray = 0;
	}
	ext2fs_free_mem (&bp);
	bp = 0;
}

अटल errcode_t ba_copy_bmap(ext2fs_generic_bitmap_64 src,
			      ext2fs_generic_bitmap_64 dest)
{
	ext2fs_ba_private src_bp = (ext2fs_ba_private) src->निजी;
	ext2fs_ba_private dest_bp;
	errcode_t retval;
	माप_प्रकार आकार;

	retval = ba_alloc_private_तथ्य (dest);
	अगर (retval)
		वापस retval;

	dest_bp = (ext2fs_ba_private) dest->निजी;

	आकार = (माप_प्रकार) (((src->real_end - src->start) / 8) + 1);
	स_नकल (dest_bp->bitarray, src_bp->bitarray, आकार);

	वापस 0;
}

अटल errcode_t ba_resize_bmap(ext2fs_generic_bitmap_64 bmap,
				__u64 new_end, __u64 new_real_end)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bmap->निजी;
	errcode_t	retval;
	माप_प्रकार		आकार, new_आकार;
	__u64		bitno;

	/*
	 * If we're expanding the bitmap, make sure all of the new
	 * parts of the bitmap are zero.
	 */
	अगर (new_end > bmap->end) {
		bitno = bmap->real_end;
		अगर (bitno > new_end)
			bitno = new_end;
		क्रम (; bitno > bmap->end; bitno--)
			ext2fs_clear_bit64(bitno - bmap->start, bp->bitarray);
	}
	अगर (new_real_end == bmap->real_end) {
		bmap->end = new_end;
		वापस 0;
	}

	आकार = ((bmap->real_end - bmap->start) / 8) + 1;
	new_आकार = ((new_real_end - bmap->start) / 8) + 1;

	अगर (आकार != new_आकार) {
		retval = ext2fs_resize_mem(आकार, new_आकार, &bp->bitarray);
		अगर (retval)
			वापस retval;
	}
	अगर (new_आकार > आकार)
		स_रखो(bp->bitarray + आकार, 0, new_आकार - आकार);

	bmap->end = new_end;
	bmap->real_end = new_real_end;
	वापस 0;

}

अटल पूर्णांक ba_mark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->निजी;
	blk64_t bitno = (blk64_t) arg;

	वापस ext2fs_set_bit64(bitno - bitmap->start, bp->bitarray);
}

अटल पूर्णांक ba_unmark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->निजी;
	blk64_t bitno = (blk64_t) arg;

	वापस ext2fs_clear_bit64(bitno - bitmap->start, bp->bitarray);
}

अटल पूर्णांक ba_test_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->निजी;
	blk64_t bitno = (blk64_t) arg;

	वापस ext2fs_test_bit64(bitno - bitmap->start, bp->bitarray);
}

अटल व्योम ba_mark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				अचिन्हित पूर्णांक अंक)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->निजी;
	blk64_t bitno = (blk64_t) arg;
	अचिन्हित पूर्णांक i;

	क्रम (i = 0; i < अंक; i++)
		ext2fs_fast_set_bit64(bitno + i - bitmap->start, bp->bitarray);
}

अटल व्योम ba_unmark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				  अचिन्हित पूर्णांक अंक)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->निजी;
	blk64_t bitno = (blk64_t) arg;
	अचिन्हित पूर्णांक i;

	क्रम (i = 0; i < अंक; i++)
		ext2fs_fast_clear_bit64(bitno + i - bitmap->start, bp->bitarray);
}

अटल पूर्णांक ba_test_clear_bmap_extent(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, अचिन्हित पूर्णांक len)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->निजी;
	__u64 start_byte, len_byte = len >> 3;
	अचिन्हित पूर्णांक start_bit, len_bit = len % 8;
	अचिन्हित पूर्णांक first_bit = 0;
	अचिन्हित पूर्णांक last_bit  = 0;
	पूर्णांक mark_गिनती = 0;
	पूर्णांक mark_bit = 0;
	पूर्णांक i;
	स्थिर अक्षर *ADDR;

	ADDR = bp->bitarray;
	start -= bitmap->start;
	start_byte = start >> 3;
	start_bit = start % 8;

	अगर (start_bit != 0) {
		/*
		 * The compared start block number or start inode number
		 * is not the first bit in a byte.
		 */
		mark_गिनती = 8 - start_bit;
		अगर (len < 8 - start_bit) {
			mark_गिनती = (पूर्णांक)len;
			mark_bit = len + start_bit - 1;
		} अन्यथा
			mark_bit = 7;

		क्रम (i = mark_गिनती; i > 0; i--, mark_bit--)
			first_bit |= 1 << mark_bit;

		/*
		 * Compare blocks or inodes in the first byte.
		 * If there is any marked bit, this function returns 0.
		 */
		अगर (first_bit & ADDR[start_byte])
			वापस 0;
		अन्यथा अगर (len <= 8 - start_bit)
			वापस 1;

		start_byte++;
		len_bit = (len - mark_गिनती) % 8;
		len_byte = (len - mark_गिनती) >> 3;
	}

	/*
	 * The compared start block number or start inode number is
	 * the first bit in a byte.
	 */
	अगर (len_bit != 0) {
		/*
		 * The compared end block number or end inode number is
		 * not the last bit in a byte.
		 */
		क्रम (mark_bit = len_bit - 1; mark_bit >= 0; mark_bit--)
			last_bit |= 1 << mark_bit;

		/*
		 * Compare blocks or inodes in the last byte.
		 * If there is any marked bit, this function returns 0.
		 */
		अगर (last_bit & ADDR[start_byte + len_byte])
			वापस 0;
		अन्यथा अगर (len_byte == 0)
			वापस 1;
	}

	/* Check whether all bytes are 0 */
	वापस ext2fs_mem_is_zero(ADDR + start_byte, len_byte);
}


अटल errcode_t ba_set_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, माप_प्रकार अंक, व्योम *in)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->निजी;

	स_नकल (bp->bitarray + (start >> 3), in, (अंक + 7) >> 3);

	वापस 0;
}

अटल errcode_t ba_get_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 start, माप_प्रकार अंक, व्योम *out)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->निजी;

	स_नकल (out, bp->bitarray + (start >> 3), (अंक + 7) >> 3);

	वापस 0;
}

अटल व्योम ba_clear_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	ext2fs_ba_private bp = (ext2fs_ba_private) bitmap->निजी;

	स_रखो(bp->bitarray, 0,
	       (माप_प्रकार) (((bitmap->real_end - bitmap->start) / 8) + 1));
}

#अगर_घोषित ENABLE_BMAP_STATS
अटल व्योम ba_print_stats(ext2fs_generic_bitmap_64 bitmap)
{
	ख_लिखो(मानक_त्रुटि, "%16llu Bytes used by bitarray\n", (अचिन्हित दीर्घ दीर्घ)
		((bitmap->real_end - bitmap->start) >> 3) + 1 +
		माप(काष्ठा ext2fs_ba_private_struct));
}
#अन्यथा
अटल व्योम ba_print_stats(ext2fs_generic_bitmap_64 bitmap EXT2FS_ATTR((unused)))
{
}
#पूर्ण_अगर

/* Find the first zero bit between start and end, inclusive. */
अटल errcode_t ba_find_first_zero(ext2fs_generic_bitmap_64 bitmap,
				    __u64 start, __u64 end, __u64 *out)
{
	ext2fs_ba_private bp = (ext2fs_ba_private)bitmap->निजी;
	अचिन्हित दीर्घ bitpos = start - bitmap->start;
	अचिन्हित दीर्घ गिनती = end - start + 1;
	पूर्णांक byte_found = 0; /* whether a != 0xff byte has been found */
	स्थिर अचिन्हित अक्षर *pos;
	अचिन्हित दीर्घ max_loop_गिनती, i;

	/* scan bits until we hit a byte boundary */
	जबतक ((bitpos & 0x7) != 0 && गिनती > 0) {
		अगर (!ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*out = bitpos + bitmap->start;
			वापस 0;
		}
		bitpos++;
		गिनती--;
	}

	अगर (!गिनती)
		वापस ENOENT;

	pos = ((अचिन्हित अक्षर *)bp->bitarray) + (bitpos >> 3);
	/* scan bytes until 8-byte (64-bit) aligned */
	जबतक (गिनती >= 8 && (((uintptr_t)pos) & 0x07)) {
		अगर (*pos != 0xff) {
			byte_found = 1;
			अवरोध;
		}
		pos++;
		गिनती -= 8;
		bitpos += 8;
	}

	अगर (!byte_found) {
		max_loop_गिनती = गिनती >> 6; /* 8-byte blocks */
		i = max_loop_गिनती;
		जबतक (i) {
			अगर (*((स्थिर __u64 *)pos) != ((__u64)-1))
				अवरोध;
			pos += 8;
			i--;
		}
		गिनती -= 64 * (max_loop_गिनती - i);
		bitpos += 64 * (max_loop_गिनती - i);

		max_loop_गिनती = गिनती >> 3;
		i = max_loop_गिनती;
		जबतक (i) {
			अगर (*pos != 0xff) {
				byte_found = 1;
				अवरोध;
			}
			pos++;
			i--;
		}
		गिनती -= 8 * (max_loop_गिनती - i);
		bitpos += 8 * (max_loop_गिनती - i);
	}

	/* Here either count < 8 or byte_found == 1. */
	जबतक (गिनती-- > 0) {
		अगर (!ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*out = bitpos + bitmap->start;
			वापस 0;
		}
		bitpos++;
	}

	वापस ENOENT;
}

/* Find the first one bit between start and end, inclusive. */
अटल errcode_t ba_find_first_set(ext2fs_generic_bitmap_64 bitmap,
				    __u64 start, __u64 end, __u64 *out)
{
	ext2fs_ba_private bp = (ext2fs_ba_private)bitmap->निजी;
	अचिन्हित दीर्घ bitpos = start - bitmap->start;
	अचिन्हित दीर्घ गिनती = end - start + 1;
	पूर्णांक byte_found = 0; /* whether a != 0xff byte has been found */
	स्थिर अचिन्हित अक्षर *pos;
	अचिन्हित दीर्घ max_loop_गिनती, i;

	/* scan bits until we hit a byte boundary */
	जबतक ((bitpos & 0x7) != 0 && गिनती > 0) {
		अगर (ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*out = bitpos + bitmap->start;
			वापस 0;
		}
		bitpos++;
		गिनती--;
	}

	अगर (!गिनती)
		वापस ENOENT;

	pos = ((अचिन्हित अक्षर *)bp->bitarray) + (bitpos >> 3);
	/* scan bytes until 8-byte (64-bit) aligned */
	जबतक (गिनती >= 8 && (((uintptr_t)pos) & 0x07)) {
		अगर (*pos != 0) {
			byte_found = 1;
			अवरोध;
		}
		pos++;
		गिनती -= 8;
		bitpos += 8;
	}

	अगर (!byte_found) {
		max_loop_गिनती = गिनती >> 6; /* 8-byte blocks */
		i = max_loop_गिनती;
		जबतक (i) {
			अगर (*((स्थिर __u64 *)pos) != 0)
				अवरोध;
			pos += 8;
			i--;
		}
		गिनती -= 64 * (max_loop_गिनती - i);
		bitpos += 64 * (max_loop_गिनती - i);

		max_loop_गिनती = गिनती >> 3;
		i = max_loop_गिनती;
		जबतक (i) {
			अगर (*pos != 0) {
				byte_found = 1;
				अवरोध;
			}
			pos++;
			i--;
		}
		गिनती -= 8 * (max_loop_गिनती - i);
		bitpos += 8 * (max_loop_गिनती - i);
	}

	/* Here either count < 8 or byte_found == 1. */
	जबतक (गिनती-- > 0) {
		अगर (ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*out = bitpos + bitmap->start;
			वापस 0;
		}
		bitpos++;
	}

	वापस ENOENT;
}

काष्ठा ext2_bitmap_ops ext2fs_blkmap64_bitarray = {
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
