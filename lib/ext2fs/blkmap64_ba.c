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
#اگر_ختم
#شامل <fcntl.h>
#شامل <time.h>
#اگر HAVE_SYS_STAT_H
#شامل <sys/stat.h>
#اگر_ختم
#اگر HAVE_SYS_TYPES_H
#شامل <sys/types.h>
#اگر_ختم

#شامل "ext2_fs.h"
#شامل "ext2fsP.h"
#شامل "bmap64.h"

/*
 * Private data for bit array implementation of bitmap ops.
 * Currently, this is just a pointer to our big flat hunk of memory,
 * exactly equivalent to the old-skool char * bitmap member.
 */

ساخت ext2fs_ba_نجی_struct {
	حرف *bitarray;
};

قسمتعریف ساخت ext2fs_ba_نجی_struct *ext2fs_ba_نجی;

ساکن errcode_t ba_alloc_نجی_ڈیٹا (ext2fs_generic_bitmap_64 bitmap)
{
	ext2fs_ba_نجی bp;
	errcode_t	retval;
	size_t		سائز;

	/*
	 * Since we only have the one pointer, we could just shove our
	 * private data in the void *private field itself, but then
	 * we'd have to do a fair bit of rewriting if we ever added a
	 * field.  I'm agnostic.
	 */
	retval = ext2fs_get_mem(حجم (ext2fs_ba_نجی), &bp);
	اگر (retval)
		واپس retval;

	سائز = (size_t) (((bitmap->real_انتہا - bitmap->آغاز) / 8) + 1);

	retval = ext2fs_get_mem(سائز, &bp->bitarray);
	اگر (retval) {
		ext2fs_free_mem(&bp);
		bp = 0;
		واپس retval;
	}
	bitmap->نجی = (خالی *) bp;
	واپس 0;
}

ساکن errcode_t ba_new_bmap(ext2_filsys fs EXT2FS_ATTR((غیر_مستعمل)),
			     ext2fs_generic_bitmap_64 bitmap)
{
	ext2fs_ba_نجی bp;
	errcode_t	retval;
	size_t		سائز;

	retval = ba_alloc_نجی_ڈیٹا (bitmap);
	اگر (retval)
		واپس retval;

	bp = (ext2fs_ba_نجی) bitmap->نجی;
	سائز = (size_t) (((bitmap->real_انتہا - bitmap->آغاز) / 8) + 1);
	memset(bp->bitarray, 0, سائز);

	واپس 0;
}

ساکن خالی ba_free_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	ext2fs_ba_نجی bp = (ext2fs_ba_نجی) bitmap->نجی;

	اگر (!bp)
		واپس;

	اگر (bp->bitarray) {
		ext2fs_free_mem (&bp->bitarray);
		bp->bitarray = 0;
	}
	ext2fs_free_mem (&bp);
	bp = 0;
}

ساکن errcode_t ba_copy_bmap(ext2fs_generic_bitmap_64 src,
			      ext2fs_generic_bitmap_64 dest)
{
	ext2fs_ba_نجی src_bp = (ext2fs_ba_نجی) src->نجی;
	ext2fs_ba_نجی dest_bp;
	errcode_t retval;
	size_t سائز;

	retval = ba_alloc_نجی_ڈیٹا (dest);
	اگر (retval)
		واپس retval;

	dest_bp = (ext2fs_ba_نجی) dest->نجی;

	سائز = (size_t) (((src->real_انتہا - src->آغاز) / 8) + 1);
	حافظہ_نقل (dest_bp->bitarray, src_bp->bitarray, سائز);

	واپس 0;
}

ساکن errcode_t ba_resize_bmap(ext2fs_generic_bitmap_64 bmap,
				__u64 new_انتہا, __u64 new_real_انتہا)
{
	ext2fs_ba_نجی bp = (ext2fs_ba_نجی) bmap->نجی;
	errcode_t	retval;
	size_t		سائز, new_سائز;
	__u64		bitno;

	/*
	 * If we're expanding the bitmap, make sure all of the new
	 * parts of the bitmap are zero.
	 */
	اگر (new_انتہا > bmap->انتہا) {
		bitno = bmap->real_انتہا;
		اگر (bitno > new_انتہا)
			bitno = new_انتہا;
		برائے (; bitno > bmap->انتہا; bitno--)
			ext2fs_clear_bit64(bitno - bmap->آغاز, bp->bitarray);
	}
	اگر (new_real_انتہا == bmap->real_انتہا) {
		bmap->انتہا = new_انتہا;
		واپس 0;
	}

	سائز = ((bmap->real_انتہا - bmap->آغاز) / 8) + 1;
	new_سائز = ((new_real_انتہا - bmap->آغاز) / 8) + 1;

	اگر (سائز != new_سائز) {
		retval = ext2fs_resize_mem(سائز, new_سائز, &bp->bitarray);
		اگر (retval)
			واپس retval;
	}
	اگر (new_سائز > سائز)
		memset(bp->bitarray + سائز, 0, new_سائز - سائز);

	bmap->انتہا = new_انتہا;
	bmap->real_انتہا = new_real_انتہا;
	واپس 0;

}

ساکن صحیح ba_mark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ext2fs_ba_نجی bp = (ext2fs_ba_نجی) bitmap->نجی;
	blk64_t bitno = (blk64_t) arg;

	واپس ext2fs_سیٹ_bit64(bitno - bitmap->آغاز, bp->bitarray);
}

ساکن صحیح ba_unmark_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ext2fs_ba_نجی bp = (ext2fs_ba_نجی) bitmap->نجی;
	blk64_t bitno = (blk64_t) arg;

	واپس ext2fs_clear_bit64(bitno - bitmap->آغاز, bp->bitarray);
}

ساکن صحیح ba_test_bmap(ext2fs_generic_bitmap_64 bitmap, __u64 arg)
{
	ext2fs_ba_نجی bp = (ext2fs_ba_نجی) bitmap->نجی;
	blk64_t bitno = (blk64_t) arg;

	واپس ext2fs_test_bit64(bitno - bitmap->آغاز, bp->bitarray);
}

ساکن خالی ba_mark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				غیرعلامتی صحیح عدد)
{
	ext2fs_ba_نجی bp = (ext2fs_ba_نجی) bitmap->نجی;
	blk64_t bitno = (blk64_t) arg;
	غیرعلامتی صحیح i;

	برائے (i = 0; i < عدد; i++)
		ext2fs_fast_سیٹ_bit64(bitno + i - bitmap->آغاز, bp->bitarray);
}

ساکن خالی ba_unmark_bmap_extent(ext2fs_generic_bitmap_64 bitmap, __u64 arg,
				  غیرعلامتی صحیح عدد)
{
	ext2fs_ba_نجی bp = (ext2fs_ba_نجی) bitmap->نجی;
	blk64_t bitno = (blk64_t) arg;
	غیرعلامتی صحیح i;

	برائے (i = 0; i < عدد; i++)
		ext2fs_fast_clear_bit64(bitno + i - bitmap->آغاز, bp->bitarray);
}

ساکن صحیح ba_test_clear_bmap_extent(ext2fs_generic_bitmap_64 bitmap,
				     __u64 آغاز, غیرعلامتی صحیح len)
{
	ext2fs_ba_نجی bp = (ext2fs_ba_نجی) bitmap->نجی;
	__u64 آغاز_byte, len_byte = len >> 3;
	غیرعلامتی صحیح آغاز_ثنائی_ہندسہ, len_ثنائی_ہندسہ = len % 8;
	غیرعلامتی صحیح پہلا_ثنائی_ہندسہ = 0;
	غیرعلامتی صحیح آخری_ثنائی_ہندسہ  = 0;
	صحیح mark_گنتی = 0;
	صحیح mark_ثنائی_ہندسہ = 0;
	صحیح i;
	ثابت حرف *ADDR;

	ADDR = bp->bitarray;
	آغاز -= bitmap->آغاز;
	آغاز_byte = آغاز >> 3;
	آغاز_ثنائی_ہندسہ = آغاز % 8;

	اگر (آغاز_ثنائی_ہندسہ != 0) {
		/*
		 * The compared start block number or start inode number
		 * is not the first bit in a byte.
		 */
		mark_گنتی = 8 - آغاز_ثنائی_ہندسہ;
		اگر (len < 8 - آغاز_ثنائی_ہندسہ) {
			mark_گنتی = (صحیح)len;
			mark_ثنائی_ہندسہ = len + آغاز_ثنائی_ہندسہ - 1;
		} ورنہ
			mark_ثنائی_ہندسہ = 7;

		برائے (i = mark_گنتی; i > 0; i--, mark_ثنائی_ہندسہ--)
			پہلا_ثنائی_ہندسہ |= 1 << mark_ثنائی_ہندسہ;

		/*
		 * Compare blocks or inodes in the first byte.
		 * If there is any marked bit, this function returns 0.
		 */
		اگر (پہلا_ثنائی_ہندسہ & ADDR[آغاز_byte])
			واپس 0;
		ورنہ اگر (len <= 8 - آغاز_ثنائی_ہندسہ)
			واپس 1;

		آغاز_byte++;
		len_ثنائی_ہندسہ = (len - mark_گنتی) % 8;
		len_byte = (len - mark_گنتی) >> 3;
	}

	/*
	 * The compared start block number or start inode number is
	 * the first bit in a byte.
	 */
	اگر (len_ثنائی_ہندسہ != 0) {
		/*
		 * The compared end block number or end inode number is
		 * not the last bit in a byte.
		 */
		برائے (mark_ثنائی_ہندسہ = len_ثنائی_ہندسہ - 1; mark_ثنائی_ہندسہ >= 0; mark_ثنائی_ہندسہ--)
			آخری_ثنائی_ہندسہ |= 1 << mark_ثنائی_ہندسہ;

		/*
		 * Compare blocks or inodes in the last byte.
		 * If there is any marked bit, this function returns 0.
		 */
		اگر (آخری_ثنائی_ہندسہ & ADDR[آغاز_byte + len_byte])
			واپس 0;
		ورنہ اگر (len_byte == 0)
			واپس 1;
	}

	/* Check whether all bytes are 0 */
	واپس ext2fs_mem_is_zero(ADDR + آغاز_byte, len_byte);
}


ساکن errcode_t ba_سیٹ_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 آغاز, size_t عدد, خالی *اندر)
{
	ext2fs_ba_نجی bp = (ext2fs_ba_نجی) bitmap->نجی;

	حافظہ_نقل (bp->bitarray + (آغاز >> 3), اندر, (عدد + 7) >> 3);

	واپس 0;
}

ساکن errcode_t ba_get_bmap_range(ext2fs_generic_bitmap_64 bitmap,
				     __u64 آغاز, size_t عدد, خالی *برآمد)
{
	ext2fs_ba_نجی bp = (ext2fs_ba_نجی) bitmap->نجی;

	حافظہ_نقل (برآمد, bp->bitarray + (آغاز >> 3), (عدد + 7) >> 3);

	واپس 0;
}

ساکن خالی ba_clear_bmap(ext2fs_generic_bitmap_64 bitmap)
{
	ext2fs_ba_نجی bp = (ext2fs_ba_نجی) bitmap->نجی;

	memset(bp->bitarray, 0,
	       (size_t) (((bitmap->real_انتہا - bitmap->آغاز) / 8) + 1));
}

#اگر_تعریف_شدہ ENABLE_BMAP_STATS
ساکن خالی ba_print_stats(ext2fs_generic_bitmap_64 bitmap)
{
	fprintf(stderr, "%16llu Bytes used by bitarray\n", (غیرعلامتی طویل طویل)
		((bitmap->real_انتہا - bitmap->آغاز) >> 3) + 1 +
		حجم(ساخت ext2fs_ba_نجی_struct));
}
#ورنہ
ساکن خالی ba_print_stats(ext2fs_generic_bitmap_64 bitmap EXT2FS_ATTR((غیر_مستعمل)))
{
}
#اگر_ختم

/* Find the first zero bit between start and end, inclusive. */
ساکن errcode_t ba_ڈھونڈو_پہلا_zero(ext2fs_generic_bitmap_64 bitmap,
				    __u64 آغاز, __u64 انتہا, __u64 *برآمد)
{
	ext2fs_ba_نجی bp = (ext2fs_ba_نجی)bitmap->نجی;
	غیرعلامتی طویل bitpos = آغاز - bitmap->آغاز;
	غیرعلامتی طویل گنتی = انتہا - آغاز + 1;
	صحیح byte_ملا = 0; /* whether a != 0xff byte has been found */
	ثابت غیرعلامتی حرف *pos;
	غیرعلامتی طویل max_loop_گنتی, i;

	/* scan bits until we hit a byte boundary */
	جب ((bitpos & 0x7) != 0 && گنتی > 0) {
		اگر (!ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*برآمد = bitpos + bitmap->آغاز;
			واپس 0;
		}
		bitpos++;
		گنتی--;
	}

	اگر (!گنتی)
		واپس ENOENT;

	pos = ((غیرعلامتی حرف *)bp->bitarray) + (bitpos >> 3);
	/* scan bytes until 8-byte (64-bit) aligned */
	جب (گنتی >= 8 && (((uintptr_t)pos) & 0x07)) {
		اگر (*pos != 0xff) {
			byte_ملا = 1;
			توڑو;
		}
		pos++;
		گنتی -= 8;
		bitpos += 8;
	}

	اگر (!byte_ملا) {
		max_loop_گنتی = گنتی >> 6; /* 8-byte blocks */
		i = max_loop_گنتی;
		جب (i) {
			اگر (*((ثابت __u64 *)pos) != ((__u64)-1))
				توڑو;
			pos += 8;
			i--;
		}
		گنتی -= 64 * (max_loop_گنتی - i);
		bitpos += 64 * (max_loop_گنتی - i);

		max_loop_گنتی = گنتی >> 3;
		i = max_loop_گنتی;
		جب (i) {
			اگر (*pos != 0xff) {
				byte_ملا = 1;
				توڑو;
			}
			pos++;
			i--;
		}
		گنتی -= 8 * (max_loop_گنتی - i);
		bitpos += 8 * (max_loop_گنتی - i);
	}

	/* Here either count < 8 or byte_found == 1. */
	جب (گنتی-- > 0) {
		اگر (!ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*برآمد = bitpos + bitmap->آغاز;
			واپس 0;
		}
		bitpos++;
	}

	واپس ENOENT;
}

/* Find the first one bit between start and end, inclusive. */
ساکن errcode_t ba_ڈھونڈو_پہلا_سیٹ(ext2fs_generic_bitmap_64 bitmap,
				    __u64 آغاز, __u64 انتہا, __u64 *برآمد)
{
	ext2fs_ba_نجی bp = (ext2fs_ba_نجی)bitmap->نجی;
	غیرعلامتی طویل bitpos = آغاز - bitmap->آغاز;
	غیرعلامتی طویل گنتی = انتہا - آغاز + 1;
	صحیح byte_ملا = 0; /* whether a != 0xff byte has been found */
	ثابت غیرعلامتی حرف *pos;
	غیرعلامتی طویل max_loop_گنتی, i;

	/* scan bits until we hit a byte boundary */
	جب ((bitpos & 0x7) != 0 && گنتی > 0) {
		اگر (ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*برآمد = bitpos + bitmap->آغاز;
			واپس 0;
		}
		bitpos++;
		گنتی--;
	}

	اگر (!گنتی)
		واپس ENOENT;

	pos = ((غیرعلامتی حرف *)bp->bitarray) + (bitpos >> 3);
	/* scan bytes until 8-byte (64-bit) aligned */
	جب (گنتی >= 8 && (((uintptr_t)pos) & 0x07)) {
		اگر (*pos != 0) {
			byte_ملا = 1;
			توڑو;
		}
		pos++;
		گنتی -= 8;
		bitpos += 8;
	}

	اگر (!byte_ملا) {
		max_loop_گنتی = گنتی >> 6; /* 8-byte blocks */
		i = max_loop_گنتی;
		جب (i) {
			اگر (*((ثابت __u64 *)pos) != 0)
				توڑو;
			pos += 8;
			i--;
		}
		گنتی -= 64 * (max_loop_گنتی - i);
		bitpos += 64 * (max_loop_گنتی - i);

		max_loop_گنتی = گنتی >> 3;
		i = max_loop_گنتی;
		جب (i) {
			اگر (*pos != 0) {
				byte_ملا = 1;
				توڑو;
			}
			pos++;
			i--;
		}
		گنتی -= 8 * (max_loop_گنتی - i);
		bitpos += 8 * (max_loop_گنتی - i);
	}

	/* Here either count < 8 or byte_found == 1. */
	جب (گنتی-- > 0) {
		اگر (ext2fs_test_bit64(bitpos, bp->bitarray)) {
			*برآمد = bitpos + bitmap->آغاز;
			واپس 0;
		}
		bitpos++;
	}

	واپس ENOENT;
}

ساخت ext2_bitmap_ops ext2fs_blkmap64_bitarray = {
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
	.سیٹ_bmap_range = ba_سیٹ_bmap_range,
	.get_bmap_range = ba_get_bmap_range,
	.clear_bmap = ba_clear_bmap,
	.print_stats = ba_print_stats,
	.ڈھونڈو_پہلا_zero = ba_ڈھونڈو_پہلا_zero,
	.ڈھونڈو_پہلا_سیٹ = ba_ڈھونڈو_پہلا_سیٹ
};
