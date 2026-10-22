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

#شامل "config.h"
#شامل <stdio.h>
#شامل <string.h>
#اگر HAVE_UNISTD_H
#شامل <unistd.h>
#endif
#شامل <fcntl.h>
#شامل <time.h>
#اگر HAVE_SYS_STAT_H
#شامل <sys/stat.h>
#endif
#اگر HAVE_SYS_TYPES_H
#شامل <sys/types.h>
#endif

#شامل "ext2_fs.h"
#شامل "ext2fsP.h"

/*
 * Helper function for making a badblocks list
 */
ایستا errcode_t make_u32_list(صحیح size, صحیح رقم, __u32 *list,
			       ext2_u32_list *ret)
{
	ext2_u32_list	bb;
	errcode_t	retval;

	retval = ext2fs_get_mem(اندازه(ساختار ext2_struct_u32_list), &bb);
	اگر (retval)
		بازگشت retval;
	memset(bb, 0, اندازه(ساختار ext2_struct_u32_list));
	bb->magic = EXT2_ET_MAGIC_BADBLOCKS_LIST;
	bb->size = size ? size : 10;
	bb->رقم = رقم;
	retval = ext2fs_get_array(bb->size, اندازه(blk_t), &bb->list);
	اگر (retval) {
		ext2fs_free_mem(&bb);
		بازگشت retval;
	}
	اگر (list)
		memcpy(bb->list, list, bb->size * اندازه(blk_t));
	وگرنه
		memset(bb->list, 0, bb->size * اندازه(blk_t));
	*ret = bb;
	بازگشت 0;
}


/*
 * This procedure creates an empty u32 list.
 */
errcode_t ext2fs_u32_list_create(ext2_u32_list *ret, صحیح size)
{
	بازگشت make_u32_list(size, 0, 0, ret);
}

/*
 * This procedure creates an empty badblocks list.
 */
errcode_t ext2fs_badblocks_list_create(ext2_badblocks_list *ret, صحیح size)
{
	بازگشت make_u32_list(size, 0, 0, (ext2_badblocks_list *) ret);
}


/*
 * This procedure copies a badblocks list
 */
errcode_t ext2fs_u32_copy(ext2_u32_list src, ext2_u32_list *dest)
{
	errcode_t	retval;

	retval = make_u32_list(src->size, src->رقم, src->list, dest);
	اگر (retval)
		بازگشت retval;
	(*dest)->badblocks_flags = src->badblocks_flags;
	بازگشت 0;
}

errcode_t ext2fs_badblocks_copy(ext2_badblocks_list src,
				ext2_badblocks_list *dest)
{
	بازگشت ext2fs_u32_copy((ext2_u32_list) src,
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
	صحیح		i, j;
	unsigned long	old_size;

	EXT2_CHECK_MAGIC(bb, EXT2_ET_MAGIC_BADBLOCKS_LIST);

	اگر (bb->رقم >= bb->size) {
		old_size = bb->size * اندازه(__u32);
		bb->size += 100;
		retval = ext2fs_resize_mem(old_size, bb->size * اندازه(__u32),
					   &bb->list);
		اگر (retval) {
			bb->size -= 100;
			بازگشت retval;
		}
	}

	/*
	 * Add special case code for appending to the end of the list
	 */
	i = bb->رقم-1;
	اگر ((bb->رقم != 0) && (bb->list[i] == blk))
		بازگشت 0;
	اگر ((bb->رقم == 0) || (bb->list[i] < blk)) {
		bb->list[bb->رقم++] = blk;
		بازگشت 0;
	}

	j = bb->رقم;
	برای (i=0; i < bb->رقم; i++) {
		اگر (bb->list[i] == blk)
			بازگشت 0;
		اگر (bb->list[i] > blk) {
			j = i;
			بشکن;
		}
	}
	برای (i=bb->رقم; i > j; i--)
		bb->list[i] = bb->list[i-1];
	bb->list[j] = blk;
	bb->رقم++;
	بازگشت 0;
}

errcode_t ext2fs_badblocks_list_add(ext2_badblocks_list bb, blk_t blk)
{
	بازگشت ext2fs_u32_list_add((ext2_u32_list) bb, (__u32) blk);
}

/*
 * This procedure finds a particular block is on a badblocks
 * list.
 */
صحیح ext2fs_u32_list_find(ext2_u32_list bb, __u32 blk)
{
	صحیح	کم, بالا, mid;

	اگر (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		بازگشت -1;

	اگر (bb->رقم == 0)
		بازگشت -1;

	کم = 0;
	بالا = bb->رقم-1;
	اگر (blk == bb->list[کم])
		بازگشت کم;
	اگر (blk == bb->list[بالا])
		بازگشت بالا;

	وقتی (کم < بالا) {
		mid = ((unsigned)کم + (unsigned)بالا)/2;
		اگر (mid == کم || mid == بالا)
			بشکن;
		اگر (blk == bb->list[mid])
			بازگشت mid;
		اگر (blk < bb->list[mid])
			بالا = mid;
		وگرنه
			کم = mid;
	}
	بازگشت -1;
}

/*
 * This procedure tests to see if a particular block is on a badblocks
 * list.
 */
صحیح ext2fs_u32_list_test(ext2_u32_list bb, __u32 blk)
{
	اگر (ext2fs_u32_list_find(bb, blk) < 0)
		بازگشت 0;
	وگرنه
		بازگشت 1;
}

صحیح ext2fs_badblocks_list_test(ext2_badblocks_list bb, blk_t blk)
{
	بازگشت ext2fs_u32_list_test((ext2_u32_list) bb, (__u32) blk);
}


/*
 * Remove a block from the badblock list
 */
صحیح ext2fs_u32_list_del(ext2_u32_list bb, __u32 blk)
{
	صحیح	remloc, i;

	اگر (bb->رقم == 0)
		بازگشت -1;

	remloc = ext2fs_u32_list_find(bb, blk);
	اگر (remloc < 0)
		بازگشت -1;

	برای (i = remloc ; i < bb->رقم-1; i++)
		bb->list[i] = bb->list[i+1];
	bb->رقم--;
	بازگشت 0;
}

تهی ext2fs_badblocks_list_del(ext2_u32_list bb, __u32 blk)
{
	ext2fs_u32_list_del(bb, blk);
}

errcode_t ext2fs_u32_list_iterate_begin(ext2_u32_list bb,
					ext2_u32_iterate *ret)
{
	ext2_u32_iterate iter;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(bb, EXT2_ET_MAGIC_BADBLOCKS_LIST);

	retval = ext2fs_get_mem(اندازه(ساختار ext2_struct_u32_iterate), &iter);
	اگر (retval)
		بازگشت retval;

	iter->magic = EXT2_ET_MAGIC_BADBLOCKS_ITERATE;
	iter->bb = bb;
	iter->ptr = 0;
	*ret = iter;
	بازگشت 0;
}

errcode_t ext2fs_badblocks_list_iterate_begin(ext2_badblocks_list bb,
					      ext2_badblocks_iterate *ret)
{
	بازگشت ext2fs_u32_list_iterate_begin((ext2_u32_list) bb,
					      (ext2_u32_iterate *) ret);
}


صحیح ext2fs_u32_list_iterate(ext2_u32_iterate iter, __u32 *blk)
{
	ext2_u32_list	bb;

	اگر (iter->magic != EXT2_ET_MAGIC_BADBLOCKS_ITERATE)
		بازگشت 0;

	bb = iter->bb;

	اگر (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		بازگشت 0;

	اگر (iter->ptr < bb->رقم) {
		*blk = bb->list[iter->ptr++];
		بازگشت 1;
	}
	*blk = 0;
	بازگشت 0;
}

صحیح ext2fs_badblocks_list_iterate(ext2_badblocks_iterate iter, blk_t *blk)
{
	بازگشت ext2fs_u32_list_iterate((ext2_u32_iterate) iter,
				       (__u32 *) blk);
}


تهی ext2fs_u32_list_iterate_end(ext2_u32_iterate iter)
{
	اگر (!iter || (iter->magic != EXT2_ET_MAGIC_BADBLOCKS_ITERATE))
		بازگشت;

	iter->bb = 0;
	ext2fs_free_mem(&iter);
}

تهی ext2fs_badblocks_list_iterate_end(ext2_badblocks_iterate iter)
{
	ext2fs_u32_list_iterate_end((ext2_u32_iterate) iter);
}


صحیح ext2fs_u32_list_equal(ext2_u32_list bb1, ext2_u32_list bb2)
{
	EXT2_CHECK_MAGIC(bb1, EXT2_ET_MAGIC_BADBLOCKS_LIST);
	EXT2_CHECK_MAGIC(bb2, EXT2_ET_MAGIC_BADBLOCKS_LIST);

	اگر (bb1->رقم != bb2->رقم)
		بازگشت 0;

	اگر (memcmp(bb1->list, bb2->list, bb1->رقم * اندازه(blk_t)) != 0)
		بازگشت 0;
	بازگشت 1;
}

صحیح ext2fs_badblocks_equal(ext2_badblocks_list bb1, ext2_badblocks_list bb2)
{
	بازگشت ext2fs_u32_list_equal((ext2_u32_list) bb1,
				     (ext2_u32_list) bb2);
}

صحیح ext2fs_u32_list_شمار(ext2_u32_list bb)
{
	بازگشت bb->رقم;
}
