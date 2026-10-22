/*
 * badblocks.c --- routines to manipulate the bad block structure
 *
 * Copyright (C) 1994, 1995, 1996 Theodore Ts'o.
 *
 * %Begin-Header%
 * This file may be redistributed under the terms of the GNU Library
 * General Public License, version 2.
 * %End-Header%
 */

#அந்தர்பாவ "config.h"
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <string.h>
#எனில் HAVE_UNISTD_H
#அந்தர்பாவ <unistd.h>
#endif
#அந்தர்பாவ <fcntl.h>
#அந்தர்பாவ <time.h>
#எனில் HAVE_SYS_STAT_H
#அந்தர்பாவ <sys/stat.h>
#endif
#எனில் HAVE_SYS_TYPES_H
#அந்தர்பாவ <sys/types.h>
#endif

#அந்தர்பாவ "ext2_fs.h"
#அந்தர்பாவ "ext2fsP.h"

/*
 * Helper function for making a badblocks list
 */
நிலையான errcode_t make_u32_list(முழு size, முழு இலக்கம், __u32 *list,
			       ext2_u32_list *ret)
{
	ext2_u32_list	bb;
	errcode_t	retval;

	retval = ext2fs_get_mem(அளவு(கட்டமைப்பு ext2_struct_u32_list), &bb);
	எனில் (retval)
		திரும்பு retval;
	memset(bb, 0, அளவு(கட்டமைப்பு ext2_struct_u32_list));
	bb->magic = EXT2_ET_MAGIC_BADBLOCKS_LIST;
	bb->size = size ? size : 10;
	bb->இலக்கம் = இலக்கம்;
	retval = ext2fs_get_array(bb->size, அளவு(blk_t), &bb->list);
	எனில் (retval) {
		ext2fs_free_mem(&bb);
		திரும்பு retval;
	}
	எனில் (list)
		memcpy(bb->list, list, bb->size * அளவு(blk_t));
	இல்லையெனில்
		memset(bb->list, 0, bb->size * அளவு(blk_t));
	*ret = bb;
	திரும்பு 0;
}


/*
 * This procedure creates an empty u32 list.
 */
errcode_t ext2fs_u32_list_create(ext2_u32_list *ret, முழு size)
{
	திரும்பு make_u32_list(size, 0, 0, ret);
}

/*
 * This procedure creates an empty badblocks list.
 */
errcode_t ext2fs_badblocks_list_create(ext2_badblocks_list *ret, முழு size)
{
	திரும்பு make_u32_list(size, 0, 0, (ext2_badblocks_list *) ret);
}


/*
 * This procedure copies a badblocks list
 */
errcode_t ext2fs_u32_copy(ext2_u32_list src, ext2_u32_list *dest)
{
	errcode_t	retval;

	retval = make_u32_list(src->size, src->இலக்கம், src->list, dest);
	எனில் (retval)
		திரும்பு retval;
	(*dest)->badblocks_flags = src->badblocks_flags;
	திரும்பு 0;
}

errcode_t ext2fs_badblocks_copy(ext2_badblocks_list src,
				ext2_badblocks_list *dest)
{
	திரும்பு ext2fs_u32_copy((ext2_u32_list) src,
			       (ext2_u32_list *) dest);
}

/*
 * This procedure frees a badblocks list.
 *
 * (note: moved to closefs.c)
 */


/*
 * This procedure adds an item to a tracking list (e.g. badblocks or casefold).
 */
errcode_t ext2fs_u32_list_add(ext2_u32_list bb, __u32 blk)
{
	errcode_t	retval;
	முழு		i, j;
	unsigned long	old_size;

	EXT2_CHECK_MAGIC(bb, EXT2_ET_MAGIC_BADBLOCKS_LIST);

	எனில் (bb->இலக்கம் >= bb->size) {
		old_size = bb->size * அளவு(__u32);
		bb->size += 100;
		retval = ext2fs_resize_mem(old_size, bb->size * அளவு(__u32),
					   &bb->list);
		எனில் (retval) {
			bb->size -= 100;
			திரும்பு retval;
		}
	}

	/*
	 * Add special case code for appending to the end of the list
	 */
	i = bb->இலக்கம்-1;
	எனில் ((bb->இலக்கம் != 0) && (bb->list[i] == blk))
		திரும்பு 0;
	எனில் ((bb->இலக்கம் == 0) || (bb->list[i] < blk)) {
		bb->list[bb->இலக்கம்++] = blk;
		திரும்பு 0;
	}

	j = bb->இலக்கம்;
	க்கு (i=0; i < bb->இலக்கம்; i++) {
		எனில் (bb->list[i] == blk)
			திரும்பு 0;
		எனில் (bb->list[i] > blk) {
			j = i;
			நிறுத்து;
		}
	}
	க்கு (i=bb->இலக்கம்; i > j; i--)
		bb->list[i] = bb->list[i-1];
	bb->list[j] = blk;
	bb->இலக்கம்++;
	திரும்பு 0;
}

errcode_t ext2fs_badblocks_list_add(ext2_badblocks_list bb, blk_t blk)
{
	திரும்பு ext2fs_u32_list_add((ext2_u32_list) bb, (__u32) blk);
}

/*
 * This procedure finds a particular block is on a badblocks
 * list.
 */
முழு ext2fs_u32_list_find(ext2_u32_list bb, __u32 blk)
{
	முழு	கீழ், உயர்வு, mid;

	எனில் (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		திரும்பு -1;

	எனில் (bb->இலக்கம் == 0)
		திரும்பு -1;

	கீழ் = 0;
	உயர்வு = bb->இலக்கம்-1;
	எனில் (blk == bb->list[கீழ்])
		திரும்பு கீழ்;
	எனில் (blk == bb->list[உயர்வு])
		திரும்பு உயர்வு;

	போது (கீழ் < உயர்வு) {
		mid = ((unsigned)கீழ் + (unsigned)உயர்வு)/2;
		எனில் (mid == கீழ் || mid == உயர்வு)
			நிறுத்து;
		எனில் (blk == bb->list[mid])
			திரும்பு mid;
		எனில் (blk < bb->list[mid])
			உயர்வு = mid;
		இல்லையெனில்
			கீழ் = mid;
	}
	திரும்பு -1;
}

/*
 * This procedure tests to see if a particular block is on a badblocks
 * list.
 */
முழு ext2fs_u32_list_test(ext2_u32_list bb, __u32 blk)
{
	எனில் (ext2fs_u32_list_find(bb, blk) < 0)
		திரும்பு 0;
	இல்லையெனில்
		திரும்பு 1;
}

முழு ext2fs_badblocks_list_test(ext2_badblocks_list bb, blk_t blk)
{
	திரும்பு ext2fs_u32_list_test((ext2_u32_list) bb, (__u32) blk);
}


/*
 * Remove a block from the badblock list
 */
முழு ext2fs_u32_list_del(ext2_u32_list bb, __u32 blk)
{
	முழு	remloc, i;

	எனில் (bb->இலக்கம் == 0)
		திரும்பு -1;

	remloc = ext2fs_u32_list_find(bb, blk);
	எனில் (remloc < 0)
		திரும்பு -1;

	க்கு (i = remloc ; i < bb->இலக்கம்-1; i++)
		bb->list[i] = bb->list[i+1];
	bb->இலக்கம்--;
	திரும்பு 0;
}

வெறுமை ext2fs_badblocks_list_del(ext2_u32_list bb, __u32 blk)
{
	ext2fs_u32_list_del(bb, blk);
}

errcode_t ext2fs_u32_list_iterate_begin(ext2_u32_list bb,
					ext2_u32_iterate *ret)
{
	ext2_u32_iterate iter;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(bb, EXT2_ET_MAGIC_BADBLOCKS_LIST);

	retval = ext2fs_get_mem(அளவு(கட்டமைப்பு ext2_struct_u32_iterate), &iter);
	எனில் (retval)
		திரும்பு retval;

	iter->magic = EXT2_ET_MAGIC_BADBLOCKS_ITERATE;
	iter->bb = bb;
	iter->ptr = 0;
	*ret = iter;
	திரும்பு 0;
}

errcode_t ext2fs_badblocks_list_iterate_begin(ext2_badblocks_list bb,
					      ext2_badblocks_iterate *ret)
{
	திரும்பு ext2fs_u32_list_iterate_begin((ext2_u32_list) bb,
					      (ext2_u32_iterate *) ret);
}


முழு ext2fs_u32_list_iterate(ext2_u32_iterate iter, __u32 *blk)
{
	ext2_u32_list	bb;

	எனில் (iter->magic != EXT2_ET_MAGIC_BADBLOCKS_ITERATE)
		திரும்பு 0;

	bb = iter->bb;

	எனில் (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		திரும்பு 0;

	எனில் (iter->ptr < bb->இலக்கம்) {
		*blk = bb->list[iter->ptr++];
		திரும்பு 1;
	}
	*blk = 0;
	திரும்பு 0;
}

முழு ext2fs_badblocks_list_iterate(ext2_badblocks_iterate iter, blk_t *blk)
{
	திரும்பு ext2fs_u32_list_iterate((ext2_u32_iterate) iter,
				       (__u32 *) blk);
}


வெறுமை ext2fs_u32_list_iterate_end(ext2_u32_iterate iter)
{
	எனில் (!iter || (iter->magic != EXT2_ET_MAGIC_BADBLOCKS_ITERATE))
		திரும்பு;

	iter->bb = 0;
	ext2fs_free_mem(&iter);
}

வெறுமை ext2fs_badblocks_list_iterate_end(ext2_badblocks_iterate iter)
{
	ext2fs_u32_list_iterate_end((ext2_u32_iterate) iter);
}


முழு ext2fs_u32_list_equal(ext2_u32_list bb1, ext2_u32_list bb2)
{
	EXT2_CHECK_MAGIC(bb1, EXT2_ET_MAGIC_BADBLOCKS_LIST);
	EXT2_CHECK_MAGIC(bb2, EXT2_ET_MAGIC_BADBLOCKS_LIST);

	எனில் (bb1->இலக்கம் != bb2->இலக்கம்)
		திரும்பு 0;

	எனில் (memcmp(bb1->list, bb2->list, bb1->இலக்கம் * அளவு(blk_t)) != 0)
		திரும்பு 0;
	திரும்பு 1;
}

முழு ext2fs_badblocks_equal(ext2_badblocks_list bb1, ext2_badblocks_list bb2)
{
	திரும்பு ext2fs_u32_list_equal((ext2_u32_list) bb1,
				     (ext2_u32_list) bb2);
}

முழு ext2fs_u32_list_மொத்தம்(ext2_u32_list bb)
{
	திரும்பு bb->இலக்கம்;
}
