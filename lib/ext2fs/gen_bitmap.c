/*
 * gen_bitmap.c --- Generic (32-bit) bitmap routines
 *
 * Copyright (C) 2001 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
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

నిర్మాణం ext2fs_struct_generic_bitmap_32 {
	errcode_t	magic;
	ext2_filsys 	fs;
	__u32		ప్రారంభం, చివర;
	__u32		real_చివర;
	అక్షరం	*	description;
	అక్షరం	*	bitmap;
	errcode_t	పునాది_error_సంకేతం;
	__u32		reserved[7];
};

రకనిర్వచనం నిర్మాణం ext2fs_struct_generic_bitmap_32 *ext2fs_generic_bitmap_32;

#నిర్వచించు EXT2FS_IS_32_BITMAP(bmap) \
	(((bmap)->magic == EXT2_ET_MAGIC_GENERIC_BITMAP) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_ఖండం_BITMAP) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_INODE_BITMAP))

#నిర్వచించు EXT2FS_IS_64_BITMAP(bmap) \
	(((bmap)->magic == EXT2_ET_MAGIC_GENERIC_BITMAP64) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_ఖండం_BITMAP64) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_INODE_BITMAP64))

/*
 * Used by previously inlined function, so we have to export this and
 * not change the function signature
 */
శూన్య ext2fs_warn_bitmap2(ext2fs_generic_bitmap gen_bitmap,
			    పూర్ణాంకం సంకేతం_2, గుర్తులేని దీర్ఘ arg)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

#నిర్వచితం_కాకపోతే OMIT_COM_ERR
	ఒకవేళ (bitmap->description)
		com_err(0, bitmap->పునాది_error_సంకేతం+సంకేతం_2,
			"#%lu for %s", arg, bitmap->description);
	లేకపోతే
		com_err(0, bitmap->పునాది_error_సంకేతం + సంకేతం_2, "#%lu", arg);
#అయితే_ముగింపు
}

స్థిర errcode_t check_magic(ext2fs_generic_bitmap bitmap)
{
	ఒకవేళ (!bitmap || !((bitmap->magic == EXT2_ET_MAGIC_GENERIC_BITMAP) ||
			 (bitmap->magic == EXT2_ET_MAGIC_INODE_BITMAP) ||
			 (bitmap->magic == EXT2_ET_MAGIC_ఖండం_BITMAP)))
		తిరిగి EXT2_ET_MAGIC_GENERIC_BITMAP;
	తిరిగి 0;
}

errcode_t ext2fs_make_generic_bitmap(errcode_t magic, ext2_filsys fs,
				     __u32 ప్రారంభం, __u32 చివర, __u32 real_చివర,
				     స్థిరరాశి అక్షరం *descr, అక్షరం *init_map,
				     ext2fs_generic_bitmap *ret)
{
	ext2fs_generic_bitmap_32 bitmap;
	errcode_t		retval;
	size_t			కొలత;

	retval = ext2fs_get_mem(పరిమాణం(నిర్మాణం ext2fs_struct_generic_bitmap_32),
				&bitmap);
	ఒకవేళ (retval)
		తిరిగి retval;

	bitmap->magic = magic;
	bitmap->fs = fs;
	bitmap->ప్రారంభం = ప్రారంభం;
	bitmap->చివర = చివర;
	bitmap->real_చివర = real_చివర;
	ఎంపిక (magic) {
	స్థితి EXT2_ET_MAGIC_INODE_BITMAP:
		bitmap->పునాది_error_సంకేతం = EXT2_ET_BAD_INODE_MARK;
		ఆపు;
	స్థితి EXT2_ET_MAGIC_ఖండం_BITMAP:
		bitmap->పునాది_error_సంకేతం = EXT2_ET_BAD_ఖండం_MARK;
		ఆపు;
	అప్రమేయ:
		bitmap->పునాది_error_సంకేతం = EXT2_ET_BAD_GENERIC_MARK;
	}
	ఒకవేళ (descr) {
		retval = ext2fs_get_mem(అక్షరశ్రేణి_పొడవు(descr)+1, &bitmap->description);
		ఒకవేళ (retval) {
			ext2fs_free_mem(&bitmap);
			తిరిగి retval;
		}
		strcpy(bitmap->description, descr);
	} లేకపోతే
		bitmap->description = 0;

	కొలత = (size_t) (((bitmap->real_చివర - bitmap->ప్రారంభం) / 8) + 1);
	/* Round up to allow for the BT x86 instruction */
	కొలత = (కొలత + 7) & ~3;
	retval = ext2fs_get_mem(కొలత, &bitmap->bitmap);
	ఒకవేళ (retval) {
		ext2fs_free_mem(&bitmap->description);
		ext2fs_free_mem(&bitmap);
		తిరిగి retval;
	}

	ఒకవేళ (init_map)
		జ్ఞాపకం_నకలు(bitmap->bitmap, init_map, కొలత);
	లేకపోతే
		memset(bitmap->bitmap, 0, కొలత);
	*ret = (ext2fs_generic_bitmap) bitmap;
	తిరిగి 0;
}

errcode_t ext2fs_allocate_generic_bitmap(__u32 ప్రారంభం,
					 __u32 చివర,
					 __u32 real_చివర,
					 స్థిరరాశి అక్షరం *descr,
					 ext2fs_generic_bitmap *ret)
{
	తిరిగి ext2fs_make_generic_bitmap(EXT2_ET_MAGIC_GENERIC_BITMAP, 0,
					  ప్రారంభం, చివర, real_చివర, descr, 0, ret);
}

errcode_t ext2fs_copy_generic_bitmap(ext2fs_generic_bitmap gen_src,
				     ext2fs_generic_bitmap *dest)
{
	ext2fs_generic_bitmap_32 src = (ext2fs_generic_bitmap_32) gen_src;

	తిరిగి (ext2fs_make_generic_bitmap(src->magic, src->fs,
					   src->ప్రారంభం, src->చివర,
					   src->real_చివర,
					   src->description, src->bitmap,
					   dest));
}

శూన్య ext2fs_free_generic_bitmap(ext2fs_inode_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	ఒకవేళ (check_magic(gen_bitmap))
		తిరిగి;

	bitmap->magic = 0;
	ఒకవేళ (bitmap->description) {
		ext2fs_free_mem(&bitmap->description);
		bitmap->description = 0;
	}
	ఒకవేళ (bitmap->bitmap) {
		ext2fs_free_mem(&bitmap->bitmap);
		bitmap->bitmap = 0;
	}
	ext2fs_free_mem(&bitmap);
}

పూర్ణాంకం ext2fs_test_generic_bitmap(ext2fs_generic_bitmap bitmap,
					blk_t bitno)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	ఒకవేళ (!EXT2FS_IS_32_BITMAP(bitmap)) {
		ఒకవేళ (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			తిరిగి ext2fs_test_generic_bmap(bitmap, bitno);
		}
#నిర్వచితం_కాకపోతే OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"test_bitmap(%lu)", (గుర్తులేని దీర్ఘ) bitno);
#అయితే_ముగింపు
		తిరిగి 0;
	}

	ఒకవేళ ((bitno < bitmap32->ప్రారంభం) || (bitno > bitmap32->చివర)) {
		ext2fs_warn_bitmap2(bitmap, EXT2FS_TEST_ERROR, bitno);
		తిరిగి 0;
	}
	తిరిగి ext2fs_test_బిట్టు(bitno - bitmap32->ప్రారంభం, bitmap32->bitmap);
}

పూర్ణాంకం ext2fs_mark_generic_bitmap(ext2fs_generic_bitmap bitmap,
					 __u32 bitno)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	ఒకవేళ (!EXT2FS_IS_32_BITMAP(bitmap)) {
		ఒకవేళ (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			తిరిగి ext2fs_mark_generic_bmap(bitmap, bitno);
		}
#నిర్వచితం_కాకపోతే OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"mark_bitmap(%lu)", (గుర్తులేని దీర్ఘ) bitno);
#అయితే_ముగింపు
		తిరిగి 0;
	}

	ఒకవేళ ((bitno < bitmap32->ప్రారంభం) || (bitno > bitmap32->చివర)) {
		ext2fs_warn_bitmap2(bitmap, EXT2FS_MARK_ERROR, bitno);
		తిరిగి 0;
	}
	తిరిగి ext2fs_సమితి_బిట్టు(bitno - bitmap32->ప్రారంభం, bitmap32->bitmap);
}

పూర్ణాంకం ext2fs_unmark_generic_bitmap(ext2fs_generic_bitmap bitmap,
					   blk_t bitno)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	ఒకవేళ (!EXT2FS_IS_32_BITMAP(bitmap)) {
		ఒకవేళ (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			తిరిగి ext2fs_unmark_generic_bmap(bitmap, bitno);
		}
#నిర్వచితం_కాకపోతే OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"mark_bitmap(%lu)", (గుర్తులేని దీర్ఘ) bitno);
#అయితే_ముగింపు
		తిరిగి 0;
	}

	ఒకవేళ ((bitno < bitmap32->ప్రారంభం) || (bitno > bitmap32->చివర)) {
		ext2fs_warn_bitmap2(bitmap, EXT2FS_UNMARK_ERROR, bitno);
		తిరిగి 0;
	}
	తిరిగి ext2fs_clear_బిట్టు(bitno - bitmap32->ప్రారంభం, bitmap32->bitmap);
}

__u32 ext2fs_get_generic_bitmap_ప్రారంభం(ext2fs_generic_bitmap bitmap)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	ఒకవేళ (!EXT2FS_IS_32_BITMAP(bitmap)) {
		ఒకవేళ (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			తిరిగి ext2fs_get_generic_bmap_ప్రారంభం(bitmap);
		}
#నిర్వచితం_కాకపోతే OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"get_bitmap_start");
#అయితే_ముగింపు
		తిరిగి 0;
	}

	తిరిగి bitmap32->ప్రారంభం;
}

__u32 ext2fs_get_generic_bitmap_చివర(ext2fs_generic_bitmap bitmap)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	ఒకవేళ (!EXT2FS_IS_32_BITMAP(bitmap)) {
		ఒకవేళ (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			తిరిగి ext2fs_get_generic_bmap_చివర(bitmap);
		}
#నిర్వచితం_కాకపోతే OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"get_bitmap_end");
#అయితే_ముగింపు
		తిరిగి 0;
	}
	తిరిగి bitmap32->చివర;
}

శూన్య ext2fs_clear_generic_bitmap(ext2fs_generic_bitmap bitmap)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	ఒకవేళ (!EXT2FS_IS_32_BITMAP(bitmap)) {
		ఒకవేళ (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			ext2fs_clear_generic_bmap(bitmap);
			తిరిగి;
		}
#నిర్వచితం_కాకపోతే OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"clear_generic_bitmap");
#అయితే_ముగింపు
		తిరిగి;
	}

	memset(bitmap32->bitmap, 0,
	       (size_t) (((bitmap32->real_చివర - bitmap32->ప్రారంభం) / 8) + 1));
}

errcode_t ext2fs_fudge_generic_bitmap_చివర(ext2fs_inode_bitmap gen_bitmap,
					  errcode_t magic, errcode_t neq,
					  ext2_ino_t చివర, ext2_ino_t *oend)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	EXT2_CHECK_MAGIC(bitmap, magic);

	ఒకవేళ (చివర > bitmap->real_చివర)
		తిరిగి neq;
	ఒకవేళ (oend)
		*oend = bitmap->చివర;
	bitmap->చివర = చివర;
	తిరిగి 0;
}

errcode_t ext2fs_resize_generic_bitmap(errcode_t magic,
				       __u32 new_చివర, __u32 new_real_చివర,
				       ext2fs_generic_bitmap gen_bmap)
{
	ext2fs_generic_bitmap_32 bmap = (ext2fs_generic_bitmap_32) gen_bmap;
	errcode_t	retval;
	size_t		కొలత, new_కొలత;
	__u32		bitno;

	ఒకవేళ (!bmap || (bmap->magic != magic))
		తిరిగి magic;

	/*
	 * If we're expanding the bitmap, make sure all of the new
	 * parts of the bitmap are zero.
	 */
	ఒకవేళ (new_చివర > bmap->చివర) {
		bitno = bmap->real_చివర;
		ఒకవేళ (bitno > new_చివర)
			bitno = new_చివర;
		కొరకు (; bitno > bmap->చివర; bitno--)
			ext2fs_clear_బిట్టు(bitno - bmap->ప్రారంభం, bmap->bitmap);
	}
	ఒకవేళ (new_real_చివర == bmap->real_చివర) {
		bmap->చివర = new_చివర;
		తిరిగి 0;
	}

	కొలత = ((bmap->real_చివర - bmap->ప్రారంభం) / 8) + 1;
	new_కొలత = ((new_real_చివర - bmap->ప్రారంభం) / 8) + 1;

	ఒకవేళ (కొలత != new_కొలత) {
		retval = ext2fs_resize_mem(కొలత, new_కొలత, &bmap->bitmap);
		ఒకవేళ (retval)
			తిరిగి retval;
	}
	ఒకవేళ (new_కొలత > కొలత)
		memset(bmap->bitmap + కొలత, 0, new_కొలత - కొలత);

	bmap->చివర = new_చివర;
	bmap->real_చివర = new_real_చివర;
	తిరిగి 0;
}

errcode_t ext2fs_compare_generic_bitmap(errcode_t magic, errcode_t neq,
					ext2fs_generic_bitmap gen_bm1,
					ext2fs_generic_bitmap gen_bm2)
{
	ext2fs_generic_bitmap_32 bm1 = (ext2fs_generic_bitmap_32) gen_bm1;
	ext2fs_generic_bitmap_32 bm2 = (ext2fs_generic_bitmap_32) gen_bm2;
	blk_t	i;

	ఒకవేళ (!bm1 || bm1->magic != magic)
		తిరిగి magic;
	ఒకవేళ (!bm2 || bm2->magic != magic)
		తిరిగి magic;

	ఒకవేళ ((bm1->ప్రారంభం != bm2->ప్రారంభం) ||
	    (bm1->చివర != bm2->చివర) ||
	    (memcmp(bm1->bitmap, bm2->bitmap,
		    (size_t) (bm1->చివర - bm1->ప్రారంభం)/8)))
		తిరిగి neq;

	కొరకు (i = bm1->చివర - ((bm1->చివర - bm1->ప్రారంభం) % 8); i <= bm1->చివర; i++)
		ఒకవేళ (ext2fs_fast_test_ఖండం_bitmap(gen_bm1, i) !=
		    ext2fs_fast_test_ఖండం_bitmap(gen_bm2, i))
			తిరిగి neq;

	తిరిగి 0;
}

శూన్య ext2fs_సమితి_generic_bitmap_padding(ext2fs_generic_bitmap gen_map)
{
	ext2fs_generic_bitmap_32 map = (ext2fs_generic_bitmap_32) gen_map;
	__u32	i, j;

	/* Protect loop from wrap-around if map->real_end is maxed */
	కొరకు (i=map->చివర+1, j = i - map->ప్రారంభం;
	     i <= map->real_చివర && i > map->చివర;
	     i++, j++)
		ext2fs_సమితి_బిట్టు(j, map->bitmap);
}

errcode_t ext2fs_get_generic_bitmap_range(ext2fs_generic_bitmap gen_bmap,
					  errcode_t magic,
					  __u32 ప్రారంభం, __u32 అంకె,
					  శూన్య *బహిర్గతం)
{
	ext2fs_generic_bitmap_32 bmap = (ext2fs_generic_bitmap_32) gen_bmap;

	ఒకవేళ (!bmap || (bmap->magic != magic))
		తిరిగి magic;

	ఒకవేళ ((ప్రారంభం < bmap->ప్రారంభం) || (ప్రారంభం+అంకె-1 > bmap->real_చివర))
		తిరిగి EXT2_ET_INVALID_వాదం;

	జ్ఞాపకం_నకలు(బహిర్గతం, bmap->bitmap + ((ప్రారంభం - bmap->ప్రారంభం) >> 3), (అంకె+7) >> 3);
	తిరిగి 0;
}

errcode_t ext2fs_సమితి_generic_bitmap_range(ext2fs_generic_bitmap gen_bmap,
					  errcode_t magic,
					  __u32 ప్రారంభం, __u32 అంకె,
					  శూన్య *లోపల)
{
	ext2fs_generic_bitmap_32 bmap = (ext2fs_generic_bitmap_32) gen_bmap;

	ఒకవేళ (!bmap || (bmap->magic != magic))
		తిరిగి magic;

	ఒకవేళ ((ప్రారంభం < bmap->ప్రారంభం) || (ప్రారంభం+అంకె-1 > bmap->real_చివర))
		తిరిగి EXT2_ET_INVALID_వాదం;

	జ్ఞాపకం_నకలు(bmap->bitmap + ((ప్రారంభం - bmap->ప్రారంభం) >> 3), లోపల, (అంకె+7) >> 3);
	తిరిగి 0;
}

/*
 * Compare @mem to zero buffer by 256 bytes.
 * Return 1 if @mem is zeroed memory, otherwise return 0.
 */
పూర్ణాంకం ext2fs_mem_is_zero(స్థిరరాశి అక్షరం *mem, size_t len)
{
	స్థిర స్థిరరాశి అక్షరం zero_buf[256];

	అయితే (len >= పరిమాణం(zero_buf)) {
		ఒకవేళ (memcmp(mem, zero_buf, పరిమాణం(zero_buf)))
			తిరిగి 0;
		len -= పరిమాణం(zero_buf);
		mem += పరిమాణం(zero_buf);
	}
	/* Deal with leftover bytes. */
	ఒకవేళ (len)
		తిరిగి !memcmp(mem, zero_buf, len);
	తిరిగి 1;
}

/*
 * Return true if all of the bits in a specified range are clear
 */
స్థిర పూర్ణాంకం ext2fs_test_clear_generic_bitmap_range(ext2fs_generic_bitmap gen_bitmap,
						  గుర్తులేని పూర్ణాంకం ప్రారంభం,
						  గుర్తులేని పూర్ణాంకం len)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	size_t ప్రారంభం_byte, len_byte = len >> 3;
	గుర్తులేని పూర్ణాంకం ప్రారంభం_బిట్టు, len_బిట్టు = len % 8;
	పూర్ణాంకం మొదటి_బిట్టు = 0;
	పూర్ణాంకం చివరి_బిట్టు  = 0;
	పూర్ణాంకం mark_లెక్క = 0;
	పూర్ణాంకం mark_బిట్టు = 0;
	పూర్ణాంకం i;
	స్థిరరాశి అక్షరం *ADDR = bitmap->bitmap;

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

errcode_t ext2fs_కనుగొను_మొదటి_zero_generic_bitmap(ext2fs_generic_bitmap gen_bitmap,
						__u32 ప్రారంభం, __u32 చివర,
						__u32 *బహిర్గతం)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	blk_t b;

	ఒకవేళ (ప్రారంభం < bitmap->ప్రారంభం || చివర > bitmap->చివర || ప్రారంభం > చివర) {
		ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_TEST_ERROR, ప్రారంభం);
		తిరిగి EINVAL;
	}

	అయితే (ప్రారంభం <= చివర) {
		b = ext2fs_test_బిట్టు(ప్రారంభం - bitmap->ప్రారంభం, bitmap->bitmap);
		ఒకవేళ (!b) {
			*బహిర్గతం = ప్రారంభం;
			తిరిగి 0;
		}
		ప్రారంభం++;
	}

	తిరిగి ENOENT;
}

errcode_t ext2fs_కనుగొను_మొదటి_సమితి_generic_bitmap(ext2fs_generic_bitmap gen_bitmap,
					       __u32 ప్రారంభం, __u32 చివర,
					       __u32 *బహిర్గతం)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	blk_t b;

	ఒకవేళ (ప్రారంభం < bitmap->ప్రారంభం || చివర > bitmap->చివర || ప్రారంభం > చివర) {
		ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_TEST_ERROR, ప్రారంభం);
		తిరిగి EINVAL;
	}

	అయితే (ప్రారంభం <= చివర) {
		b = ext2fs_test_బిట్టు(ప్రారంభం - bitmap->ప్రారంభం, bitmap->bitmap);
		ఒకవేళ (b) {
			*బహిర్గతం = ప్రారంభం;
			తిరిగి 0;
		}
		ప్రారంభం++;
	}

	తిరిగి ENOENT;
}

పూర్ణాంకం ext2fs_test_ఖండం_bitmap_range(ext2fs_ఖండం_bitmap gen_bitmap,
				   blk_t ఖండం, పూర్ణాంకం అంకె)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	EXT2_CHECK_MAGIC(bitmap, EXT2_ET_MAGIC_ఖండం_BITMAP);
	ఒకవేళ ((ఖండం < bitmap->ప్రారంభం) || (ఖండం > bitmap->real_చివర) ||
	    (ఖండం+అంకె-1 > bitmap->real_చివర)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_ఖండం_TEST,
				   ఖండం, bitmap->description);
		తిరిగి 0;
	}
	తిరిగి ext2fs_test_clear_generic_bitmap_range((ext2fs_generic_bitmap)
						      bitmap, ఖండం, అంకె);
}

పూర్ణాంకం ext2fs_test_inode_bitmap_range(ext2fs_inode_bitmap gen_bitmap,
				   ext2_ino_t inode, పూర్ణాంకం అంకె)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	EXT2_CHECK_MAGIC(bitmap, EXT2_ET_MAGIC_INODE_BITMAP);
	ఒకవేళ ((inode < bitmap->ప్రారంభం) || (inode > bitmap->real_చివర) ||
	    (inode+అంకె-1 > bitmap->real_చివర)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_INODE_TEST,
				   inode, bitmap->description);
		తిరిగి 0;
	}
	తిరిగి ext2fs_test_clear_generic_bitmap_range((ext2fs_generic_bitmap)
						      bitmap, inode, అంకె);
}

శూన్య ext2fs_mark_ఖండం_bitmap_range(ext2fs_ఖండం_bitmap gen_bitmap,
				    blk_t ఖండం, పూర్ణాంకం అంకె)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	పూర్ణాంకం	i;

	ఒకవేళ ((ఖండం < bitmap->ప్రారంభం) || (ఖండం > bitmap->చివర) ||
	    (ఖండం+అంకె-1 > bitmap->చివర)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_ఖండం_MARK, ఖండం,
				   bitmap->description);
		తిరిగి;
	}
	కొరకు (i=0; i < అంకె; i++)
		ext2fs_fast_సమితి_బిట్టు(ఖండం + i - bitmap->ప్రారంభం, bitmap->bitmap);
}

శూన్య ext2fs_unmark_ఖండం_bitmap_range(ext2fs_ఖండం_bitmap gen_bitmap,
				      blk_t ఖండం, పూర్ణాంకం అంకె)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	పూర్ణాంకం	i;

	ఒకవేళ ((ఖండం < bitmap->ప్రారంభం) || (ఖండం > bitmap->చివర) ||
	    (ఖండం+అంకె-1 > bitmap->చివర)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_ఖండం_UNMARK, ఖండం,
				   bitmap->description);
		తిరిగి;
	}
	కొరకు (i=0; i < అంకె; i++)
		ext2fs_fast_clear_బిట్టు(ఖండం + i - bitmap->ప్రారంభం,
				      bitmap->bitmap);
}

