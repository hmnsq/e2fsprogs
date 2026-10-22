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

#ضمن "config.h"
#ضمن <stdio.h>
#ضمن <string.h>
#ܐܢ HAVE_UNISTD_H
#ضمن <unistd.h>
#endif
#ضمن <fcntl.h>
#ضمن <time.h>
#ܐܢ HAVE_SYS_STAT_H
#ضمن <sys/stat.h>
#endif
#ܐܢ HAVE_SYS_TYPES_H
#ضمن <sys/types.h>
#endif

#ضمن "ext2_fs.h"
#ضمن "ext2fsP.h"

/*
 * Helper function for making a badblocks list
 */
ساكن errcode_t make_u32_list(صحيح size, صحيح num, __u32 *list,
			       ext2_u32_list *ret)
{
	ext2_u32_list	bb;
	errcode_t	retval;

	retval = ext2fs_get_mem(حجم(بنية ext2_struct_u32_list), &bb);
	ܐܢ (retval)
		ܦܢܐ retval;
	memset(bb, 0, حجم(بنية ext2_struct_u32_list));
	bb->magic = EXT2_ET_MAGIC_BADBLOCKS_LIST;
	bb->size = size ? size : 10;
	bb->num = num;
	retval = ext2fs_get_array(bb->size, حجم(blk_t), &bb->list);
	ܐܢ (retval) {
		ext2fs_free_mem(&bb);
		ܦܢܐ retval;
	}
	ܐܢ (list)
		memcpy(bb->list, list, bb->size * حجم(blk_t));
	ܐܚܪܢܐ
		memset(bb->list, 0, bb->size * حجم(blk_t));
	*ret = bb;
	ܦܢܐ 0;
}


/*
 * This procedure creates an empty u32 list.
 */
errcode_t ext2fs_u32_list_create(ext2_u32_list *ret, صحيح size)
{
	ܦܢܐ make_u32_list(size, 0, 0, ret);
}

/*
 * This procedure creates an empty badblocks list.
 */
errcode_t ext2fs_badblocks_list_create(ext2_badblocks_list *ret, صحيح size)
{
	ܦܢܐ make_u32_list(size, 0, 0, (ext2_badblocks_list *) ret);
}


/*
 * This procedure copies a badblocks list
 */
errcode_t ext2fs_u32_copy(ext2_u32_list src, ext2_u32_list *dest)
{
	errcode_t	retval;

	retval = make_u32_list(src->size, src->num, src->list, dest);
	ܐܢ (retval)
		ܦܢܐ retval;
	(*dest)->badblocks_flags = src->badblocks_flags;
	ܦܢܐ 0;
}

errcode_t ext2fs_badblocks_copy(ext2_badblocks_list src,
				ext2_badblocks_list *dest)
{
	ܦܢܐ ext2fs_u32_copy((ext2_u32_list) src,
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
	صحيح		i, j;
	unsigned long	old_size;

	EXT2_CHECK_MAGIC(bb, EXT2_ET_MAGIC_BADBLOCKS_LIST);

	ܐܢ (bb->num >= bb->size) {
		old_size = bb->size * حجم(__u32);
		bb->size += 100;
		retval = ext2fs_resize_mem(old_size, bb->size * حجم(__u32),
					   &bb->list);
		ܐܢ (retval) {
			bb->size -= 100;
			ܦܢܐ retval;
		}
	}

	/*
	 * Add special case code for appending to the end of the list
	 */
	i = bb->num-1;
	ܐܢ ((bb->num != 0) && (bb->list[i] == blk))
		ܦܢܐ 0;
	ܐܢ ((bb->num == 0) || (bb->list[i] < blk)) {
		bb->list[bb->num++] = blk;
		ܦܢܐ 0;
	}

	j = bb->num;
	لكل (i=0; i < bb->num; i++) {
		ܐܢ (bb->list[i] == blk)
			ܦܢܐ 0;
		ܐܢ (bb->list[i] > blk) {
			j = i;
			اكسر;
		}
	}
	لكل (i=bb->num; i > j; i--)
		bb->list[i] = bb->list[i-1];
	bb->list[j] = blk;
	bb->num++;
	ܦܢܐ 0;
}

errcode_t ext2fs_badblocks_list_add(ext2_badblocks_list bb, blk_t blk)
{
	ܦܢܐ ext2fs_u32_list_add((ext2_u32_list) bb, (__u32) blk);
}

/*
 * This procedure finds a particular block is on a badblocks
 * list.
 */
صحيح ext2fs_u32_list_find(ext2_u32_list bb, __u32 blk)
{
	صحيح	low, high, mid;

	ܐܢ (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		ܦܢܐ -1;

	ܐܢ (bb->num == 0)
		ܦܢܐ -1;

	low = 0;
	high = bb->num-1;
	ܐܢ (blk == bb->list[low])
		ܦܢܐ low;
	ܐܢ (blk == bb->list[high])
		ܦܢܐ high;

	ܟܡܐ (low < high) {
		mid = ((unsigned)low + (unsigned)high)/2;
		ܐܢ (mid == low || mid == high)
			اكسر;
		ܐܢ (blk == bb->list[mid])
			ܦܢܐ mid;
		ܐܢ (blk < bb->list[mid])
			high = mid;
		ܐܚܪܢܐ
			low = mid;
	}
	ܦܢܐ -1;
}

/*
 * This procedure tests to see if a particular block is on a badblocks
 * list.
 */
صحيح ext2fs_u32_list_test(ext2_u32_list bb, __u32 blk)
{
	ܐܢ (ext2fs_u32_list_find(bb, blk) < 0)
		ܦܢܐ 0;
	ܐܚܪܢܐ
		ܦܢܐ 1;
}

صحيح ext2fs_badblocks_list_test(ext2_badblocks_list bb, blk_t blk)
{
	ܦܢܐ ext2fs_u32_list_test((ext2_u32_list) bb, (__u32) blk);
}


/*
 * Remove a block from the badblock list
 */
صحيح ext2fs_u32_list_del(ext2_u32_list bb, __u32 blk)
{
	صحيح	remloc, i;

	ܐܢ (bb->num == 0)
		ܦܢܐ -1;

	remloc = ext2fs_u32_list_find(bb, blk);
	ܐܢ (remloc < 0)
		ܦܢܐ -1;

	لكل (i = remloc ; i < bb->num-1; i++)
		bb->list[i] = bb->list[i+1];
	bb->num--;
	ܦܢܐ 0;
}

فراغ ext2fs_badblocks_list_del(ext2_u32_list bb, __u32 blk)
{
	ext2fs_u32_list_del(bb, blk);
}

errcode_t ext2fs_u32_list_iterate_begin(ext2_u32_list bb,
					ext2_u32_iterate *ret)
{
	ext2_u32_iterate iter;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(bb, EXT2_ET_MAGIC_BADBLOCKS_LIST);

	retval = ext2fs_get_mem(حجم(بنية ext2_struct_u32_iterate), &iter);
	ܐܢ (retval)
		ܦܢܐ retval;

	iter->magic = EXT2_ET_MAGIC_BADBLOCKS_ITERATE;
	iter->bb = bb;
	iter->ptr = 0;
	*ret = iter;
	ܦܢܐ 0;
}

errcode_t ext2fs_badblocks_list_iterate_begin(ext2_badblocks_list bb,
					      ext2_badblocks_iterate *ret)
{
	ܦܢܐ ext2fs_u32_list_iterate_begin((ext2_u32_list) bb,
					      (ext2_u32_iterate *) ret);
}


صحيح ext2fs_u32_list_iterate(ext2_u32_iterate iter, __u32 *blk)
{
	ext2_u32_list	bb;

	ܐܢ (iter->magic != EXT2_ET_MAGIC_BADBLOCKS_ITERATE)
		ܦܢܐ 0;

	bb = iter->bb;

	ܐܢ (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		ܦܢܐ 0;

	ܐܢ (iter->ptr < bb->num) {
		*blk = bb->list[iter->ptr++];
		ܦܢܐ 1;
	}
	*blk = 0;
	ܦܢܐ 0;
}

صحيح ext2fs_badblocks_list_iterate(ext2_badblocks_iterate iter, blk_t *blk)
{
	ܦܢܐ ext2fs_u32_list_iterate((ext2_u32_iterate) iter,
				       (__u32 *) blk);
}


فراغ ext2fs_u32_list_iterate_end(ext2_u32_iterate iter)
{
	ܐܢ (!iter || (iter->magic != EXT2_ET_MAGIC_BADBLOCKS_ITERATE))
		ܦܢܐ;

	iter->bb = 0;
	ext2fs_free_mem(&iter);
}

فراغ ext2fs_badblocks_list_iterate_end(ext2_badblocks_iterate iter)
{
	ext2fs_u32_list_iterate_end((ext2_u32_iterate) iter);
}


صحيح ext2fs_u32_list_equal(ext2_u32_list bb1, ext2_u32_list bb2)
{
	EXT2_CHECK_MAGIC(bb1, EXT2_ET_MAGIC_BADBLOCKS_LIST);
	EXT2_CHECK_MAGIC(bb2, EXT2_ET_MAGIC_BADBLOCKS_LIST);

	ܐܢ (bb1->num != bb2->num)
		ܦܢܐ 0;

	ܐܢ (memcmp(bb1->list, bb2->list, bb1->num * حجم(blk_t)) != 0)
		ܦܢܐ 0;
	ܦܢܐ 1;
}

صحيح ext2fs_badblocks_equal(ext2_badblocks_list bb1, ext2_badblocks_list bb2)
{
	ܦܢܐ ext2fs_u32_list_equal((ext2_u32_list) bb1,
				     (ext2_u32_list) bb2);
}

صحيح ext2fs_u32_list_count(ext2_u32_list bb)
{
	ܦܢܐ bb->num;
}
