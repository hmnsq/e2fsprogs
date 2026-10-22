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

#include "config.h"
#include <stdio.h>
#include <string.h>
#ако HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>
#include <time.h>
#ако HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#ако HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include "ext2_fs.h"
#include "ext2fsP.h"

/*
 * Helper function for making a badblocks list
 */
static errcode_t make_u32_list(цео size, цео num, __u32 *list,
			       ext2_u32_list *ret)
{
	ext2_u32_list	bb;
	errcode_t	retval;

	retval = ext2fs_get_mem(sizeof(struct ext2_struct_u32_list), &bb);
	ако (retval)
		врати retval;
	memset(bb, 0, sizeof(struct ext2_struct_u32_list));
	bb->magic = EXT2_ET_MAGIC_BADBLOCKS_LIST;
	bb->size = size ? size : 10;
	bb->num = num;
	retval = ext2fs_get_array(bb->size, sizeof(blk_t), &bb->list);
	ако (retval) {
		ext2fs_free_mem(&bb);
		врати retval;
	}
	ако (list)
		memcpy(bb->list, list, bb->size * sizeof(blk_t));
	инако
		memset(bb->list, 0, bb->size * sizeof(blk_t));
	*ret = bb;
	врати 0;
}


/*
 * This procedure creates an empty u32 list.
 */
errcode_t ext2fs_u32_list_create(ext2_u32_list *ret, цео size)
{
	врати make_u32_list(size, 0, 0, ret);
}

/*
 * This procedure creates an empty badblocks list.
 */
errcode_t ext2fs_badblocks_list_create(ext2_badblocks_list *ret, цео size)
{
	врати make_u32_list(size, 0, 0, (ext2_badblocks_list *) ret);
}


/*
 * This procedure copies a badblocks list
 */
errcode_t ext2fs_u32_copy(ext2_u32_list src, ext2_u32_list *dest)
{
	errcode_t	retval;

	retval = make_u32_list(src->size, src->num, src->list, dest);
	ако (retval)
		врати retval;
	(*dest)->badblocks_flags = src->badblocks_flags;
	врати 0;
}

errcode_t ext2fs_badblocks_copy(ext2_badblocks_list src,
				ext2_badblocks_list *dest)
{
	врати ext2fs_u32_copy((ext2_u32_list) src,
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
	цео		i, j;
	unsigned long	old_size;

	EXT2_CHECK_MAGIC(bb, EXT2_ET_MAGIC_BADBLOCKS_LIST);

	ако (bb->num >= bb->size) {
		old_size = bb->size * sizeof(__u32);
		bb->size += 100;
		retval = ext2fs_resize_mem(old_size, bb->size * sizeof(__u32),
					   &bb->list);
		ако (retval) {
			bb->size -= 100;
			врати retval;
		}
	}

	/*
	 * Add special case code for appending to the end of the list
	 */
	i = bb->num-1;
	ако ((bb->num != 0) && (bb->list[i] == blk))
		врати 0;
	ако ((bb->num == 0) || (bb->list[i] < blk)) {
		bb->list[bb->num++] = blk;
		врати 0;
	}

	j = bb->num;
	за (i=0; i < bb->num; i++) {
		ако (bb->list[i] == blk)
			врати 0;
		ако (bb->list[i] > blk) {
			j = i;
			break;
		}
	}
	за (i=bb->num; i > j; i--)
		bb->list[i] = bb->list[i-1];
	bb->list[j] = blk;
	bb->num++;
	врати 0;
}

errcode_t ext2fs_badblocks_list_add(ext2_badblocks_list bb, blk_t blk)
{
	врати ext2fs_u32_list_add((ext2_u32_list) bb, (__u32) blk);
}

/*
 * This procedure finds a particular block is on a badblocks
 * list.
 */
цео ext2fs_u32_list_find(ext2_u32_list bb, __u32 blk)
{
	цео	low, high, mid;

	ако (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		врати -1;

	ако (bb->num == 0)
		врати -1;

	low = 0;
	high = bb->num-1;
	ако (blk == bb->list[low])
		врати low;
	ако (blk == bb->list[high])
		врати high;

	док (low < high) {
		mid = ((unsigned)low + (unsigned)high)/2;
		ако (mid == low || mid == high)
			break;
		ако (blk == bb->list[mid])
			врати mid;
		ако (blk < bb->list[mid])
			high = mid;
		инако
			low = mid;
	}
	врати -1;
}

/*
 * This procedure tests to see if a particular block is on a badblocks
 * list.
 */
цео ext2fs_u32_list_test(ext2_u32_list bb, __u32 blk)
{
	ако (ext2fs_u32_list_find(bb, blk) < 0)
		врати 0;
	инако
		врати 1;
}

цео ext2fs_badblocks_list_test(ext2_badblocks_list bb, blk_t blk)
{
	врати ext2fs_u32_list_test((ext2_u32_list) bb, (__u32) blk);
}


/*
 * Remove a block from the badblock list
 */
цео ext2fs_u32_list_del(ext2_u32_list bb, __u32 blk)
{
	цео	remloc, i;

	ако (bb->num == 0)
		врати -1;

	remloc = ext2fs_u32_list_find(bb, blk);
	ако (remloc < 0)
		врати -1;

	за (i = remloc ; i < bb->num-1; i++)
		bb->list[i] = bb->list[i+1];
	bb->num--;
	врати 0;
}

void ext2fs_badblocks_list_del(ext2_u32_list bb, __u32 blk)
{
	ext2fs_u32_list_del(bb, blk);
}

errcode_t ext2fs_u32_list_iterate_begin(ext2_u32_list bb,
					ext2_u32_iterate *ret)
{
	ext2_u32_iterate iter;
	errcode_t		retval;

	EXT2_CHECK_MAGIC(bb, EXT2_ET_MAGIC_BADBLOCKS_LIST);

	retval = ext2fs_get_mem(sizeof(struct ext2_struct_u32_iterate), &iter);
	ако (retval)
		врати retval;

	iter->magic = EXT2_ET_MAGIC_BADBLOCKS_ITERATE;
	iter->bb = bb;
	iter->ptr = 0;
	*ret = iter;
	врати 0;
}

errcode_t ext2fs_badblocks_list_iterate_begin(ext2_badblocks_list bb,
					      ext2_badblocks_iterate *ret)
{
	врати ext2fs_u32_list_iterate_begin((ext2_u32_list) bb,
					      (ext2_u32_iterate *) ret);
}


цео ext2fs_u32_list_iterate(ext2_u32_iterate iter, __u32 *blk)
{
	ext2_u32_list	bb;

	ако (iter->magic != EXT2_ET_MAGIC_BADBLOCKS_ITERATE)
		врати 0;

	bb = iter->bb;

	ако (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		врати 0;

	ако (iter->ptr < bb->num) {
		*blk = bb->list[iter->ptr++];
		врати 1;
	}
	*blk = 0;
	врати 0;
}

цео ext2fs_badblocks_list_iterate(ext2_badblocks_iterate iter, blk_t *blk)
{
	врати ext2fs_u32_list_iterate((ext2_u32_iterate) iter,
				       (__u32 *) blk);
}


void ext2fs_u32_list_iterate_end(ext2_u32_iterate iter)
{
	ако (!iter || (iter->magic != EXT2_ET_MAGIC_BADBLOCKS_ITERATE))
		врати;

	iter->bb = 0;
	ext2fs_free_mem(&iter);
}

void ext2fs_badblocks_list_iterate_end(ext2_badblocks_iterate iter)
{
	ext2fs_u32_list_iterate_end((ext2_u32_iterate) iter);
}


цео ext2fs_u32_list_equal(ext2_u32_list bb1, ext2_u32_list bb2)
{
	EXT2_CHECK_MAGIC(bb1, EXT2_ET_MAGIC_BADBLOCKS_LIST);
	EXT2_CHECK_MAGIC(bb2, EXT2_ET_MAGIC_BADBLOCKS_LIST);

	ако (bb1->num != bb2->num)
		врати 0;

	ако (memcmp(bb1->list, bb2->list, bb1->num * sizeof(blk_t)) != 0)
		врати 0;
	врати 1;
}

цео ext2fs_badblocks_equal(ext2_badblocks_list bb1, ext2_badblocks_list bb2)
{
	врати ext2fs_u32_list_equal((ext2_u32_list) bb1,
				     (ext2_u32_list) bb2);
}

цео ext2fs_u32_list_count(ext2_u32_list bb)
{
	врати bb->num;
}
