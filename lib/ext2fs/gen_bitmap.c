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

ഘടന ext2fs_struct_generic_bitmap_32 {
	errcode_t	magic;
	ext2_filsys 	fs;
	__u32		start, end;
	__u32		real_end;
	അക്ഷരം	*	description;
	അക്ഷരം	*	bitmap;
	errcode_t	base_error_code;
	__u32		reserved[7];
};

തരനിർവചനം ഘടന ext2fs_struct_generic_bitmap_32 *ext2fs_generic_bitmap_32;

#define EXT2FS_IS_32_BITMAP(bmap) \
	(((bmap)->magic == EXT2_ET_MAGIC_GENERIC_BITMAP) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_BLOCK_BITMAP) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_INODE_BITMAP))

#define EXT2FS_IS_64_BITMAP(bmap) \
	(((bmap)->magic == EXT2_ET_MAGIC_GENERIC_BITMAP64) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_BLOCK_BITMAP64) || \
	 ((bmap)->magic == EXT2_ET_MAGIC_INODE_BITMAP64))

/*
 * Used by previously inlined function, so we have to export this and
 * not change the function signature
 */
ശൂന്യം ext2fs_warn_bitmap2(ext2fs_generic_bitmap gen_bitmap,
			    പൂർണ്ണസംഖ്യ code, unsigned long arg)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

#ifndef OMIT_COM_ERR
	എങ്കിൽ (bitmap->description)
		com_err(0, bitmap->base_error_code+code,
			"#%lu for %s", arg, bitmap->description);
	അല്ലെങ്കിൽ
		com_err(0, bitmap->base_error_code + code, "#%lu", arg);
#endif
}

സ്ഥിതം errcode_t check_magic(ext2fs_generic_bitmap bitmap)
{
	എങ്കിൽ (!bitmap || !((bitmap->magic == EXT2_ET_MAGIC_GENERIC_BITMAP) ||
			 (bitmap->magic == EXT2_ET_MAGIC_INODE_BITMAP) ||
			 (bitmap->magic == EXT2_ET_MAGIC_BLOCK_BITMAP)))
		തിരികെ EXT2_ET_MAGIC_GENERIC_BITMAP;
	തിരികെ 0;
}

errcode_t ext2fs_make_generic_bitmap(errcode_t magic, ext2_filsys fs,
				     __u32 start, __u32 end, __u32 real_end,
				     സ്ഥിരം അക്ഷരം *descr, അക്ഷരം *init_map,
				     ext2fs_generic_bitmap *ret)
{
	ext2fs_generic_bitmap_32 bitmap;
	errcode_t		retval;
	size_t			size;

	retval = ext2fs_get_mem(വലുപ്പം(ഘടന ext2fs_struct_generic_bitmap_32),
				&bitmap);
	എങ്കിൽ (retval)
		തിരികെ retval;

	bitmap->magic = magic;
	bitmap->fs = fs;
	bitmap->start = start;
	bitmap->end = end;
	bitmap->real_end = real_end;
	തിരഞ്ഞെടുപ്പ് (magic) {
	അവസ്ഥ EXT2_ET_MAGIC_INODE_BITMAP:
		bitmap->base_error_code = EXT2_ET_BAD_INODE_MARK;
		നിർത്തുക;
	അവസ്ഥ EXT2_ET_MAGIC_BLOCK_BITMAP:
		bitmap->base_error_code = EXT2_ET_BAD_BLOCK_MARK;
		നിർത്തുക;
	സ്ഥിരസ്ഥിതി:
		bitmap->base_error_code = EXT2_ET_BAD_GENERIC_MARK;
	}
	എങ്കിൽ (descr) {
		retval = ext2fs_get_mem(strlen(descr)+1, &bitmap->description);
		എങ്കിൽ (retval) {
			ext2fs_free_mem(&bitmap);
			തിരികെ retval;
		}
		strcpy(bitmap->description, descr);
	} അല്ലെങ്കിൽ
		bitmap->description = 0;

	size = (size_t) (((bitmap->real_end - bitmap->start) / 8) + 1);
	/* Round up to allow for the BT x86 instruction */
	size = (size + 7) & ~3;
	retval = ext2fs_get_mem(size, &bitmap->bitmap);
	എങ്കിൽ (retval) {
		ext2fs_free_mem(&bitmap->description);
		ext2fs_free_mem(&bitmap);
		തിരികെ retval;
	}

	എങ്കിൽ (init_map)
		memcpy(bitmap->bitmap, init_map, size);
	അല്ലെങ്കിൽ
		memset(bitmap->bitmap, 0, size);
	*ret = (ext2fs_generic_bitmap) bitmap;
	തിരികെ 0;
}

errcode_t ext2fs_allocate_generic_bitmap(__u32 start,
					 __u32 end,
					 __u32 real_end,
					 സ്ഥിരം അക്ഷരം *descr,
					 ext2fs_generic_bitmap *ret)
{
	തിരികെ ext2fs_make_generic_bitmap(EXT2_ET_MAGIC_GENERIC_BITMAP, 0,
					  start, end, real_end, descr, 0, ret);
}

errcode_t ext2fs_copy_generic_bitmap(ext2fs_generic_bitmap gen_src,
				     ext2fs_generic_bitmap *dest)
{
	ext2fs_generic_bitmap_32 src = (ext2fs_generic_bitmap_32) gen_src;

	തിരികെ (ext2fs_make_generic_bitmap(src->magic, src->fs,
					   src->start, src->end,
					   src->real_end,
					   src->description, src->bitmap,
					   dest));
}

ശൂന്യം ext2fs_free_generic_bitmap(ext2fs_inode_bitmap gen_bitmap)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	എങ്കിൽ (check_magic(gen_bitmap))
		തിരികെ;

	bitmap->magic = 0;
	എങ്കിൽ (bitmap->description) {
		ext2fs_free_mem(&bitmap->description);
		bitmap->description = 0;
	}
	എങ്കിൽ (bitmap->bitmap) {
		ext2fs_free_mem(&bitmap->bitmap);
		bitmap->bitmap = 0;
	}
	ext2fs_free_mem(&bitmap);
}

പൂർണ്ണസംഖ്യ ext2fs_test_generic_bitmap(ext2fs_generic_bitmap bitmap,
					blk_t bitno)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	എങ്കിൽ (!EXT2FS_IS_32_BITMAP(bitmap)) {
		എങ്കിൽ (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			തിരികെ ext2fs_test_generic_bmap(bitmap, bitno);
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"test_bitmap(%lu)", (unsigned long) bitno);
#endif
		തിരികെ 0;
	}

	എങ്കിൽ ((bitno < bitmap32->start) || (bitno > bitmap32->end)) {
		ext2fs_warn_bitmap2(bitmap, EXT2FS_TEST_ERROR, bitno);
		തിരികെ 0;
	}
	തിരികെ ext2fs_test_bit(bitno - bitmap32->start, bitmap32->bitmap);
}

പൂർണ്ണസംഖ്യ ext2fs_mark_generic_bitmap(ext2fs_generic_bitmap bitmap,
					 __u32 bitno)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	എങ്കിൽ (!EXT2FS_IS_32_BITMAP(bitmap)) {
		എങ്കിൽ (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			തിരികെ ext2fs_mark_generic_bmap(bitmap, bitno);
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"mark_bitmap(%lu)", (unsigned long) bitno);
#endif
		തിരികെ 0;
	}

	എങ്കിൽ ((bitno < bitmap32->start) || (bitno > bitmap32->end)) {
		ext2fs_warn_bitmap2(bitmap, EXT2FS_MARK_ERROR, bitno);
		തിരികെ 0;
	}
	തിരികെ ext2fs_set_bit(bitno - bitmap32->start, bitmap32->bitmap);
}

പൂർണ്ണസംഖ്യ ext2fs_unmark_generic_bitmap(ext2fs_generic_bitmap bitmap,
					   blk_t bitno)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	എങ്കിൽ (!EXT2FS_IS_32_BITMAP(bitmap)) {
		എങ്കിൽ (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			തിരികെ ext2fs_unmark_generic_bmap(bitmap, bitno);
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"mark_bitmap(%lu)", (unsigned long) bitno);
#endif
		തിരികെ 0;
	}

	എങ്കിൽ ((bitno < bitmap32->start) || (bitno > bitmap32->end)) {
		ext2fs_warn_bitmap2(bitmap, EXT2FS_UNMARK_ERROR, bitno);
		തിരികെ 0;
	}
	തിരികെ ext2fs_clear_bit(bitno - bitmap32->start, bitmap32->bitmap);
}

__u32 ext2fs_get_generic_bitmap_start(ext2fs_generic_bitmap bitmap)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	എങ്കിൽ (!EXT2FS_IS_32_BITMAP(bitmap)) {
		എങ്കിൽ (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			തിരികെ ext2fs_get_generic_bmap_start(bitmap);
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"get_bitmap_start");
#endif
		തിരികെ 0;
	}

	തിരികെ bitmap32->start;
}

__u32 ext2fs_get_generic_bitmap_end(ext2fs_generic_bitmap bitmap)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	എങ്കിൽ (!EXT2FS_IS_32_BITMAP(bitmap)) {
		എങ്കിൽ (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			തിരികെ ext2fs_get_generic_bmap_end(bitmap);
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"get_bitmap_end");
#endif
		തിരികെ 0;
	}
	തിരികെ bitmap32->end;
}

ശൂന്യം ext2fs_clear_generic_bitmap(ext2fs_generic_bitmap bitmap)
{
	ext2fs_generic_bitmap_32 bitmap32 = (ext2fs_generic_bitmap_32) bitmap;

	എങ്കിൽ (!EXT2FS_IS_32_BITMAP(bitmap)) {
		എങ്കിൽ (EXT2FS_IS_64_BITMAP(bitmap)) {
			ext2fs_warn_bitmap32(bitmap, __func__);
			ext2fs_clear_generic_bmap(bitmap);
			തിരികെ;
		}
#ifndef OMIT_COM_ERR
		com_err(0, EXT2_ET_MAGIC_GENERIC_BITMAP,
			"clear_generic_bitmap");
#endif
		തിരികെ;
	}

	memset(bitmap32->bitmap, 0,
	       (size_t) (((bitmap32->real_end - bitmap32->start) / 8) + 1));
}

errcode_t ext2fs_fudge_generic_bitmap_end(ext2fs_inode_bitmap gen_bitmap,
					  errcode_t magic, errcode_t neq,
					  ext2_ino_t end, ext2_ino_t *oend)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	EXT2_CHECK_MAGIC(bitmap, magic);

	എങ്കിൽ (end > bitmap->real_end)
		തിരികെ neq;
	എങ്കിൽ (oend)
		*oend = bitmap->end;
	bitmap->end = end;
	തിരികെ 0;
}

errcode_t ext2fs_resize_generic_bitmap(errcode_t magic,
				       __u32 new_end, __u32 new_real_end,
				       ext2fs_generic_bitmap gen_bmap)
{
	ext2fs_generic_bitmap_32 bmap = (ext2fs_generic_bitmap_32) gen_bmap;
	errcode_t	retval;
	size_t		size, new_size;
	__u32		bitno;

	എങ്കിൽ (!bmap || (bmap->magic != magic))
		തിരികെ magic;

	/*
	 * If we're expanding the bitmap, make sure all of the new
	 * parts of the bitmap are zero.
	 */
	എങ്കിൽ (new_end > bmap->end) {
		bitno = bmap->real_end;
		എങ്കിൽ (bitno > new_end)
			bitno = new_end;
		വേണ്ടി (; bitno > bmap->end; bitno--)
			ext2fs_clear_bit(bitno - bmap->start, bmap->bitmap);
	}
	എങ്കിൽ (new_real_end == bmap->real_end) {
		bmap->end = new_end;
		തിരികെ 0;
	}

	size = ((bmap->real_end - bmap->start) / 8) + 1;
	new_size = ((new_real_end - bmap->start) / 8) + 1;

	എങ്കിൽ (size != new_size) {
		retval = ext2fs_resize_mem(size, new_size, &bmap->bitmap);
		എങ്കിൽ (retval)
			തിരികെ retval;
	}
	എങ്കിൽ (new_size > size)
		memset(bmap->bitmap + size, 0, new_size - size);

	bmap->end = new_end;
	bmap->real_end = new_real_end;
	തിരികെ 0;
}

errcode_t ext2fs_compare_generic_bitmap(errcode_t magic, errcode_t neq,
					ext2fs_generic_bitmap gen_bm1,
					ext2fs_generic_bitmap gen_bm2)
{
	ext2fs_generic_bitmap_32 bm1 = (ext2fs_generic_bitmap_32) gen_bm1;
	ext2fs_generic_bitmap_32 bm2 = (ext2fs_generic_bitmap_32) gen_bm2;
	blk_t	i;

	എങ്കിൽ (!bm1 || bm1->magic != magic)
		തിരികെ magic;
	എങ്കിൽ (!bm2 || bm2->magic != magic)
		തിരികെ magic;

	എങ്കിൽ ((bm1->start != bm2->start) ||
	    (bm1->end != bm2->end) ||
	    (memcmp(bm1->bitmap, bm2->bitmap,
		    (size_t) (bm1->end - bm1->start)/8)))
		തിരികെ neq;

	വേണ്ടി (i = bm1->end - ((bm1->end - bm1->start) % 8); i <= bm1->end; i++)
		എങ്കിൽ (ext2fs_fast_test_block_bitmap(gen_bm1, i) !=
		    ext2fs_fast_test_block_bitmap(gen_bm2, i))
			തിരികെ neq;

	തിരികെ 0;
}

ശൂന്യം ext2fs_set_generic_bitmap_padding(ext2fs_generic_bitmap gen_map)
{
	ext2fs_generic_bitmap_32 map = (ext2fs_generic_bitmap_32) gen_map;
	__u32	i, j;

	/* Protect loop from wrap-around if map->real_end is maxed */
	വേണ്ടി (i=map->end+1, j = i - map->start;
	     i <= map->real_end && i > map->end;
	     i++, j++)
		ext2fs_set_bit(j, map->bitmap);
}

errcode_t ext2fs_get_generic_bitmap_range(ext2fs_generic_bitmap gen_bmap,
					  errcode_t magic,
					  __u32 start, __u32 അക്കം,
					  ശൂന്യം *out)
{
	ext2fs_generic_bitmap_32 bmap = (ext2fs_generic_bitmap_32) gen_bmap;

	എങ്കിൽ (!bmap || (bmap->magic != magic))
		തിരികെ magic;

	എങ്കിൽ ((start < bmap->start) || (start+അക്കം-1 > bmap->real_end))
		തിരികെ EXT2_ET_INVALID_ARGUMENT;

	memcpy(out, bmap->bitmap + ((start - bmap->start) >> 3), (അക്കം+7) >> 3);
	തിരികെ 0;
}

errcode_t ext2fs_set_generic_bitmap_range(ext2fs_generic_bitmap gen_bmap,
					  errcode_t magic,
					  __u32 start, __u32 അക്കം,
					  ശൂന്യം *in)
{
	ext2fs_generic_bitmap_32 bmap = (ext2fs_generic_bitmap_32) gen_bmap;

	എങ്കിൽ (!bmap || (bmap->magic != magic))
		തിരികെ magic;

	എങ്കിൽ ((start < bmap->start) || (start+അക്കം-1 > bmap->real_end))
		തിരികെ EXT2_ET_INVALID_ARGUMENT;

	memcpy(bmap->bitmap + ((start - bmap->start) >> 3), in, (അക്കം+7) >> 3);
	തിരികെ 0;
}

/*
 * Compare @mem to zero buffer by 256 bytes.
 * Return 1 if @mem is zeroed memory, otherwise return 0.
 */
പൂർണ്ണസംഖ്യ ext2fs_mem_is_zero(സ്ഥിരം അക്ഷരം *mem, size_t len)
{
	സ്ഥിതം സ്ഥിരം അക്ഷരം zero_buf[256];

	ആകുമ്പോൾ (len >= വലുപ്പം(zero_buf)) {
		എങ്കിൽ (memcmp(mem, zero_buf, വലുപ്പം(zero_buf)))
			തിരികെ 0;
		len -= വലുപ്പം(zero_buf);
		mem += വലുപ്പം(zero_buf);
	}
	/* Deal with leftover bytes. */
	എങ്കിൽ (len)
		തിരികെ !memcmp(mem, zero_buf, len);
	തിരികെ 1;
}

/*
 * Return true if all of the bits in a specified range are clear
 */
സ്ഥിതം പൂർണ്ണസംഖ്യ ext2fs_test_clear_generic_bitmap_range(ext2fs_generic_bitmap gen_bitmap,
						  unsigned പൂർണ്ണസംഖ്യ start,
						  unsigned പൂർണ്ണസംഖ്യ len)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	size_t start_byte, len_byte = len >> 3;
	unsigned പൂർണ്ണസംഖ്യ start_bit, len_bit = len % 8;
	പൂർണ്ണസംഖ്യ first_bit = 0;
	പൂർണ്ണസംഖ്യ last_bit  = 0;
	പൂർണ്ണസംഖ്യ mark_എണ്ണം = 0;
	പൂർണ്ണസംഖ്യ mark_bit = 0;
	പൂർണ്ണസംഖ്യ i;
	സ്ഥിരം അക്ഷരം *ADDR = bitmap->bitmap;

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

errcode_t ext2fs_find_first_zero_generic_bitmap(ext2fs_generic_bitmap gen_bitmap,
						__u32 start, __u32 end,
						__u32 *out)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	blk_t b;

	എങ്കിൽ (start < bitmap->start || end > bitmap->end || start > end) {
		ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_TEST_ERROR, start);
		തിരികെ EINVAL;
	}

	ആകുമ്പോൾ (start <= end) {
		b = ext2fs_test_bit(start - bitmap->start, bitmap->bitmap);
		എങ്കിൽ (!b) {
			*out = start;
			തിരികെ 0;
		}
		start++;
	}

	തിരികെ ENOENT;
}

errcode_t ext2fs_find_first_set_generic_bitmap(ext2fs_generic_bitmap gen_bitmap,
					       __u32 start, __u32 end,
					       __u32 *out)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	blk_t b;

	എങ്കിൽ (start < bitmap->start || end > bitmap->end || start > end) {
		ext2fs_warn_bitmap2(gen_bitmap, EXT2FS_TEST_ERROR, start);
		തിരികെ EINVAL;
	}

	ആകുമ്പോൾ (start <= end) {
		b = ext2fs_test_bit(start - bitmap->start, bitmap->bitmap);
		എങ്കിൽ (b) {
			*out = start;
			തിരികെ 0;
		}
		start++;
	}

	തിരികെ ENOENT;
}

പൂർണ്ണസംഖ്യ ext2fs_test_block_bitmap_range(ext2fs_block_bitmap gen_bitmap,
				   blk_t block, പൂർണ്ണസംഖ്യ അക്കം)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	EXT2_CHECK_MAGIC(bitmap, EXT2_ET_MAGIC_BLOCK_BITMAP);
	എങ്കിൽ ((block < bitmap->start) || (block > bitmap->real_end) ||
	    (block+അക്കം-1 > bitmap->real_end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_TEST,
				   block, bitmap->description);
		തിരികെ 0;
	}
	തിരികെ ext2fs_test_clear_generic_bitmap_range((ext2fs_generic_bitmap)
						      bitmap, block, അക്കം);
}

പൂർണ്ണസംഖ്യ ext2fs_test_inode_bitmap_range(ext2fs_inode_bitmap gen_bitmap,
				   ext2_ino_t inode, പൂർണ്ണസംഖ്യ അക്കം)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;

	EXT2_CHECK_MAGIC(bitmap, EXT2_ET_MAGIC_INODE_BITMAP);
	എങ്കിൽ ((inode < bitmap->start) || (inode > bitmap->real_end) ||
	    (inode+അക്കം-1 > bitmap->real_end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_INODE_TEST,
				   inode, bitmap->description);
		തിരികെ 0;
	}
	തിരികെ ext2fs_test_clear_generic_bitmap_range((ext2fs_generic_bitmap)
						      bitmap, inode, അക്കം);
}

ശൂന്യം ext2fs_mark_block_bitmap_range(ext2fs_block_bitmap gen_bitmap,
				    blk_t block, പൂർണ്ണസംഖ്യ അക്കം)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	പൂർണ്ണസംഖ്യ	i;

	എങ്കിൽ ((block < bitmap->start) || (block > bitmap->end) ||
	    (block+അക്കം-1 > bitmap->end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_MARK, block,
				   bitmap->description);
		തിരികെ;
	}
	വേണ്ടി (i=0; i < അക്കം; i++)
		ext2fs_fast_set_bit(block + i - bitmap->start, bitmap->bitmap);
}

ശൂന്യം ext2fs_unmark_block_bitmap_range(ext2fs_block_bitmap gen_bitmap,
				      blk_t block, പൂർണ്ണസംഖ്യ അക്കം)
{
	ext2fs_generic_bitmap_32 bitmap = (ext2fs_generic_bitmap_32) gen_bitmap;
	പൂർണ്ണസംഖ്യ	i;

	എങ്കിൽ ((block < bitmap->start) || (block > bitmap->end) ||
	    (block+അക്കം-1 > bitmap->end)) {
		ext2fs_warn_bitmap(EXT2_ET_BAD_BLOCK_UNMARK, block,
				   bitmap->description);
		തിരികെ;
	}
	വേണ്ടി (i=0; i < അക്കം; i++)
		ext2fs_fast_clear_bit(block + i - bitmap->start,
				      bitmap->bitmap);
}

