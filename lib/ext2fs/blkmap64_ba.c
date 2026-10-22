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

#అన్తర్భావ "config.h"
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <string.h>
#ఒకవేళ HAVE_UNISTD_H
#అన్తర్భావ <unistd.h>
#అయితే_ముగింపు
#అన్తర్భావ <fcntl.h>
#అన్తర్భావ <time.h>
#ఒకవేళ HAVE_SYS_STAT_H
#అన్తర్భావ <sys/stat.h>
#అయితే_ముగింపు
#ఒకవేళ HAVE_SYS_TYPES_H
#అన్తర్భావ <sys/types.h>
#అయితే_ముగింపు

#అన్తర్భావ "ext2_fs.h"
#అన్తర్భావ "ext2fsP.h"
#అన్తర్భావ "bmap64.h"

/*
 * Private data for bit array implementation of bitmap ops.
 * Currently, this is just a pointer to our big flat hunk of memory,
 * exactly equivalent to the old-skool char * bitmap member.
 */

నిర్మాణం ext2fs_ba_వ్యక్తిగత_struct {
	అక్షరం *bitarray;
};

రకనిర్వచనం నిర్మాణం ext2fs_ba_వ్యక్తిగత_struct *ext2fs_ba_వ్యక్తిగత;

స్థిర errcode_t ba_alloc_వ్యక్తిగత_దత్తాంశం (ext2fs_generic_bitmap_64 bitmap)
{
	ext2fs_ba_వ్యక్తిగత bp;
	errcode_t	retval;
	size_t		కొలత;

	/*
	 * Since we only have the one pointer, we could just shove our
	 * private data in the void *private field itself, but then
	 * we'd have to do a fair bit of rewriting if we ever added a
	 * field.  I'm agnostic.
	 */
	retval = ext2fs_get_mem(పరిమాణం (ext2fs_ba_వ్యక్తిగత), &bp);
	ఒకవేళ (retval)
		తిరిగి retval;

	కొలత = (size_t) (((bitmap->real_చివర - bitmap->ప్రారంభం) / 8) + 1);

	retval = ext2fs_get_mem(కొలత, &bp->bitarray);
	ఒకవేళ (retval) {
		ext2fs_free_mem(&bp);
		bp = 0;
		తిరిగి retval;
	}
	bitmap->అంతరంగ = (శూన్య *) bp;
	తిరిగి 0;
}

స్థిర errcode_t ba_new_bmap(ext2_filsys fs EXT2FS_ATTR((వాడని)),
			     ext2fs_generic_bitmap_64 bitmap)
{
	ext2fs_ba_వ్యక్తిగత bp;
	errcode_t	retval;
	size_t		కొలత;

	retval = ba_alloc_వ్యక్తిగత_దత్తాంశం (bitmap);
	ఒకవేళ (retval)
		తిరిగి retval;

	bp = (ext2fs_ba_వ్యక్తిగత) bitmap->అంతరంగ;
	కొలత = (size_t) (((bitmap->real_చివర - bitmap->ప్రారంభం) / 8) + 1);
	memset(bp->bitarray, 0, కొలత);

	తిరిగి 0;
}

స్థిర శూన్య ba_free_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	ext2fs_ba_వ్యక్తిగత bp = (ext2fs_ba_వ్యక్తిగత) bitmap->అంతరంగ;

	ఒకవేళ (!bp)
		తిరిగి;

	ఒకవేళ (bp->bitarray) {
		ext2fs_free_mem (&bp->bitarray);
		bp->bitarray = 0;
	}
	ext2fs_free_mem (&bp);
	bp = 0;
}

స్థిర errcode_t ba_copy_bmap(ext2fs_generic_bitmap_64 src,
			      ext2fs_generic_bitmap_64 dest)
{
	ext2fs_ba_వ్యక్తిగత src_bp = (ext2fs_ba_వ్యక్తిగత) src->అంతరంగ;
	ext2fs_ba_వ్యక్తిగత dest_bp;
	errcode_t retval;
	size_t కొలత;

	retval = ba_alloc_వ్యక్తిగత_దత్తాంశం (dest);
	ఒకవేళ (retval)
		తిరిగి retval;

	dest_bp = (ext2fs_ba_వ్యక్తిగత) dest->అంతరంగ;

	కొలత = (size_t) (((src->real_చివర - src->ప్రారంభం) / 8) + 1);
	జ్ఞాపకం_నకలు (dest_bp->bitarray, src_bp->bitarray, కొలత);

	తిరిగి 0;
}

స్థిర errcode_t ba_resize_bmap(ext2fs_generic_bitmap_64 bmap,
				__u64 new_చివర, __u64 new_real_చివర)
{
	ext2fs_ba_వ్యక్తిగత bp = (ext2fs_ba_వ్యక్తిగత) bmap->అంతరంగ;
	errcode_t	retval;
	size_t		కొలత, new_కొలత;
	__u64		bitno;

	/*
	 * If we're expanding the bitmap, make sure all of the new
	 * parts of the bitmap are zero.
	 */
	ఒకవేళ (new_చివర > bmap->చివర) {
		bitno = bmap->real_చివర;
		ఒకవేళ (bitno > new_చివర)
			bitno = new_చివర;
		కొరకు (; bitno > bmap->చివర; bitno--)
			ext2fs_clear_bit64(bitno - bmap->ప్రారంభం, bp->bitarray);
	}
	ఒకవేళ (new_real_చివర == bmap->real_చివర) {
		bmap->చివర = new_చివర;
		తిరిగి 0;
	}

	కొలత = ((bmap->real_చివర - bmap->ప్రారంభం) / 8) + 1;
	new_కొలత = ((new_real_చివర - bmap->ప్రారంభం) / 8) + 1;

	ఒకవేళ (కొలత != new_కొలత) {
		retval = ext2fs_resize_mem(కొలత, new_కొలత, &bp->bitarray);
		ఒకవేళ (retval)
			తిరిగి retval;
	}
	ఒకవేళ (new_కొలత > కొలత)
		memset(bp->bitarray + కొలత, 0, new_కొలత - కొలత);

	bmap->చివర = new_చివర;
	bmap->real_చివర = new_real_చివర;
	తిరిగి 0;

}

స్థిర పూర్ణాంకం ba_mark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ext2fs_ba_వ్యక్తిగత bp = (ext2fs_ba_వ్యక్తిగత) bitmap->అంతరంగ;
	blk64_t bitno = (blk64_t) arg;

	తిరిగి ext2fs_సమితి_bit64(bitno - bitmap->ప్రారంభం, bp->bitarray);
}

స్థిర పూర్ణాంకం ba_unmark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ext2fs_ba_వ్యక్తిగత bp = (ext2fs_ba_వ్యక్తిగత) bitmap->అంతరంగ;
	blk64_t bitno = (blk64_t) arg;

	తిరిగి ext2fs_clear_bit64(bitno - bitmap->ప్రారంభం, bp->bitarray);
}

స్థిర పూర్ణాంకం ba_test_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ext2fs_ba_వ్యక్తిగత bp = (ext2fs_ba_వ్యక్తిగత) bitmap->అంతరంగ;
	blk64_t bitno = (blk64_t) arg;

	తిరిగి ext2fs_test_bit64(bitno - bitmap->ప్రారంభం, bp->bitarray);
}

స్థిర శూన్య ba_mark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				గుర్తులేని పూర్ణాంకం అంకె)
{
	ext2fs_ba_వ్యక్తిగత bp = (ext2fs_ba_వ్యక్తిగత) bitmap->అంతరంగ;
	blk64_t bitno = (blk64_t) arg;
	గుర్తులేని పూర్ణాంకం i;

	కొరకు (i = 0; i < అంకె; i++)
		ext2fs_fast_సమితి_bit64(bitno + i - bitmap->ప్రారంభం, bp->bitarray);
}

స్థిర శూన్య ba_unmark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				  గుర్తులేని పూర్ణాంకం అంకె)
{
	ext2fs_ba_వ్యక్తిగత bp = (ext2fs_ba_వ్యక్తిగత) bitmap->అంతరంగ;
	blk64_t bitno = (blk64_t) arg;
	గుర్తులేని పూర్ణాంకం i;

	కొరకు (i = 0; i < అంకె; i++)
		ext2fs_fast_clear_bit64(bitno + i - bitmap->ప్రారంభం, bp->bitarray);
}

స్థిర పూర్ణాంకం ba_test_clear_bmap_extent(ext2fs_generic_bitmap_64 bitmap,
				     __u64 ప్రారంభం, గుర్తులేని పూర్ణాంకం len)
{
	ext2fs_ba_వ్యక్తిగత bp = (ext2fs_ba_వ్యక్తిగత) bitmap->అంతరంగ;
	__u64 ప్రారంభం_byte, len_byte = len >> 3;
	గుర్తులేని పూర్ణాంకం ప్రారంభం_బిట్టు, len_బిట్టు = len % 8;
	గుర్తులేని పూర్ణాంకం మొదటి_బిట్టు = 0;
	గుర్తులేని పూర్ణాంకం చివరి_బిట్టు  = 0;
	పూర్ణాంకం mark_లెక్క = 0;
	పూర్ణాంకం mark_బిట్టు = 0;
	పూర్ణాంకం i;
	స్థిరరాశి అక్షరం *ADDR;

	ADDR = bp->bitarray;
	ప్రారంభం -= bitmap->ప్రారంభం;
	ప్రారంభం_byte = ప్రారంభం >> 3;
	ప్రారంభం_బిట్టు = ప్రారంభం % 8;

	ఒకవేళ (ప్రారంభం_బిట్టు != 0) {
		/*
		 * The compared start block number or start inode number
		 * is not the first bit in a byte.
		 */
		mark_లెక్క = 8 - ప్రారంభం_బిట్టు;
		ఒకవేళ (len < 8 - ప్రారంభం_బిట్టు) {
			mark_లెక్క = (పూర్ణాంకం)len;
			mark_బిట్టు = len + ప్రారంభం_బిట్టు - 1;
		} లేకపోతే
			mark_బిట్టు = 7;

		కొరకు (i = mark_లెక్క; i > 0; i--, mark_బిట్టు--)
			మొదటి_బిట్టు |= 1 << mark_బిట్టు;

		/*
		 * Compare blocks or inodes in the first byte.
		 * If there is any marked bit, this function returns 0.
		 */
		ఒకవేళ (మొదటి_బిట్టు & ADDR[ప్రారంభం_byte])
			తిరిగి 0;
		లేకపోతే ఒకవేళ (len <= 8 - ప్రారంభం_బిట్టు)
			తిరిగి 1;

		ప్రారంభం_byte++;
		len_బిట్టు = (len - mark_లెక్క) % 8;
		len_byte = (len - mark_లెక్క) >> 3;
	}

	/*
	 * The compared start block number or start inode number is
	 * the first bit in a byte.
	 */
	ఒకవేళ (len_బిట్టు != 0) {
		/*
		 * The compared end block number or end inode number is
		 * not the last bit in a byte.
		 */
		కొరకు (mark_బిట్టు = len_బిట్టు - 1; mark_బిట్టు >= 0; mark_బిట్టు--)
			చివరి_బిట్టు |= 1 << mark_బిట్టు;

		/*
		 * Compare blocks or inodes in the last byte.
		 * If there is any marked bit, this function returns 0.
		 */
		ఒకవేళ (చివరి_బిట్టు & ADDR[ప్రారంభం_byte + len_byte])
			తిరిగి 0;
		లేకపోతే ఒకవేళ (len_byte == 0)
			తిరిగి 1;
	}

	/* Check whether all bytes are 0 */
	తిరిగి ext2fs_mem_is_zero(ADDR + ప్రారంభం_byte, len_byte);
}


స్థిర errcode_t ba_సమితి_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 ప్రారంభం, size_t అంకె, శూన్య *లోపల)
{
	ext2fs_ba_వ్యక్తిగత bp = (ext2fs_ba_వ్యక్తిగత) bitmap->అంతరంగ;

	జ్ఞాపకం_నకలు (bp->bitarray + (ప్రారంభం >> 3), లోపల, (అంకె + 7) >> 3);

	తిరిగి 0;
}

స్థిర errcode_t ba_get_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 ప్రారంభం, size_t అంకె, శూన్య *బహిర్గతం)
{
	ext2fs_ba_వ్యక్తిగత bp = (ext2fs_ba_వ్యక్తిగత) bitmap->అంతరంగ;

	జ్ఞాపకం_నకలు (బహిర్గతం, bp->bitarray + (ప్రారంభం >> 3), (అంకె + 7) >> 3);

	తిరిగి 0;
}

స్థిర శూన్య ba_clear_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	ext2fs_ba_వ్యక్తిగత bp = (ext2fs_ba_వ్యక్తిగత) bitmap->అంతరంగ;

	memset(bp->bitarray, 0,
	       (size_t) (((bitmap->real_చివర - bitmap->ప్రారంభం) / 8) + 1));
}

#నిర్వచితమైతే ENABLE_BMAP_STATS
స్థిర శూన్య ba_print_stats(ext2fs_generic_bitmap_64 bitmap)
{
	fprintf(stderr, "%16llu Bytes used by bitarray\n", (గుర్తులేని దీర్ఘ దీర్ఘ)
		((bitmap->real_చివర - bitmap->ప్రారంభం) >> 3) + 1 +
		పరిమాణం(నిర్మాణం ext2fs_ba_వ్యక్తిగత_struct));
}
#లేకపోతే
స్థిర శూన్య ba_print_stats(ext2fs_generic_bitmap_64 bitmap EXT2FS_ATTR((వాడని)))
{
}
#అయితే_ముగింపు

/* Find the first zero bit between start and end, inclusive. */
స్థిర errcode_t ba_కనుగొను_మొదటి_zero(ext2fs_generic_bitmap_64 bitmap,
				    __u64 ప్రారంభం, __u64 చివర, __u64 *బహిర్గతం)
{
	ext2fs_ba_వ్యక్తిగత bp = (ext2fs_ba_వ్యక్తిగత)bitmap->అంతరంగ;
	గుర్తులేని దీర్ఘ bitpos = ప్రారంభం - bitmap->ప్రారంభం;
	గుర్తులేని దీర్ఘ లెక్క = చివర - ప్రారంభం + 1;
	పూర్ణాంకం byte_దొరికింది = 0; /* whether a != 0xff byte has been found */
	స్థిరరాశి గుర్తులేని అక్షరం *pos;
	గుర్తులేని దీర్ఘ max_loop_లెక్క, i;

	/* scan bits until we hit a byte boundary */
	అయితే ((bitpos & 0x7) != 0 && లెక్క > 0) {
		ఒకవేళ (!ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*బహిర్గతం = bitpos + bitmap->ప్రారంభం;
			తిరిగి 0;
		}
		bitpos++;
		లెక్క--;
	}

	ఒకవేళ (!లెక్క)
		తిరిగి ENOENT;

	pos = ((గుర్తులేని అక్షరం *)bp->bitarray) + (bitpos >> 3);
	/* scan bytes until 8-byte (64-bit) aligned */
	అయితే (లెక్క >= 8 && (((uintptr_t)pos) & 0x07)) {
		ఒకవేళ (*pos != 0xff) {
			byte_దొరికింది = 1;
			ఆపు;
		}
		pos++;
		లెక్క -= 8;
		bitpos += 8;
	}

	ఒకవేళ (!byte_దొరికింది) {
		max_loop_లెక్క = లెక్క >> 6; /* 8-byte blocks */
		i = max_loop_లెక్క;
		అయితే (i) {
			ఒకవేళ (*((స్థిరరాశి __u64 *)pos) != ((__u64)-1))
				ఆపు;
			pos += 8;
			i--;
		}
		లెక్క -= 64 * (max_loop_లెక్క - i);
		bitpos += 64 * (max_loop_లెక్క - i);

		max_loop_లెక్క = లెక్క >> 3;
		i = max_loop_లెక్క;
		అయితే (i) {
			ఒకవేళ (*pos != 0xff) {
				byte_దొరికింది = 1;
				ఆపు;
			}
			pos++;
			i--;
		}
		లెక్క -= 8 * (max_loop_లెక్క - i);
		bitpos += 8 * (max_loop_లెక్క - i);
	}

	/* Here either count < 8 or byte_found == 1. */
	అయితే (లెక్క-- > 0) {
		ఒకవేళ (!ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*బహిర్గతం = bitpos + bitmap->ప్రారంభం;
			తిరిగి 0;
		}
		bitpos++;
	}

	తిరిగి ENOENT;
}

/* Find the first one bit between start and end, inclusive. */
స్థిర errcode_t ba_కనుగొను_మొదటి_సమితి(ext2fs_generic_bitmap_64 bitmap,
				    __u64 ప్రారంభం, __u64 చివర, __u64 *బహిర్గతం)
{
	ext2fs_ba_వ్యక్తిగత bp = (ext2fs_ba_వ్యక్తిగత)bitmap->అంతరంగ;
	గుర్తులేని దీర్ఘ bitpos = ప్రారంభం - bitmap->ప్రారంభం;
	గుర్తులేని దీర్ఘ లెక్క = చివర - ప్రారంభం + 1;
	పూర్ణాంకం byte_దొరికింది = 0; /* whether a != 0xff byte has been found */
	స్థిరరాశి గుర్తులేని అక్షరం *pos;
	గుర్తులేని దీర్ఘ max_loop_లెక్క, i;

	/* scan bits until we hit a byte boundary */
	అయితే ((bitpos & 0x7) != 0 && లెక్క > 0) {
		ఒకవేళ (ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*బహిర్గతం = bitpos + bitmap->ప్రారంభం;
			తిరిగి 0;
		}
		bitpos++;
		లెక్క--;
	}

	ఒకవేళ (!లెక్క)
		తిరిగి ENOENT;

	pos = ((గుర్తులేని అక్షరం *)bp->bitarray) + (bitpos >> 3);
	/* scan bytes until 8-byte (64-bit) aligned */
	అయితే (లెక్క >= 8 && (((uintptr_t)pos) & 0x07)) {
		ఒకవేళ (*pos != 0) {
			byte_దొరికింది = 1;
			ఆపు;
		}
		pos++;
		లెక్క -= 8;
		bitpos += 8;
	}

	ఒకవేళ (!byte_దొరికింది) {
		max_loop_లెక్క = లెక్క >> 6; /* 8-byte blocks */
		i = max_loop_లెక్క;
		అయితే (i) {
			ఒకవేళ (*((స్థిరరాశి __u64 *)pos) != 0)
				ఆపు;
			pos += 8;
			i--;
		}
		లెక్క -= 64 * (max_loop_లెక్క - i);
		bitpos += 64 * (max_loop_లెక్క - i);

		max_loop_లెక్క = లెక్క >> 3;
		i = max_loop_లెక్క;
		అయితే (i) {
			ఒకవేళ (*pos != 0) {
				byte_దొరికింది = 1;
				ఆపు;
			}
			pos++;
			i--;
		}
		లెక్క -= 8 * (max_loop_లెక్క - i);
		bitpos += 8 * (max_loop_లెక్క - i);
	}

	/* Here either count < 8 or byte_found == 1. */
	అయితే (లెక్క-- > 0) {
		ఒకవేళ (ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*బహిర్గతం = bitpos + bitmap->ప్రారంభం;
			తిరిగి 0;
		}
		bitpos++;
	}

	తిరిగి ENOENT;
}

నిర్మాణం ext2_bitmap_ops ext2fs_blkmap64_bitarray = {
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
	.సమితి_bmap_range = ba_సమితి_bmap_range,
	.get_bmap_range = ba_get_bmap_range,
	.clear_bmap = ba_clear_bmap,
	.print_stats = ba_print_stats,
	.కనుగొను_మొదటి_zero = ba_కనుగొను_మొదటి_zero,
	.కనుగొను_మొదటి_సమితి = ba_కనుగొను_మొదటి_సమితి
};
