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
static errcode_t make_u32_list(цяло size, цяло num, __u32 *list,
			       ext2_u32_list *ret)
{
	ext2_u32_list	bb;
	errcode_t	retval;

	retval = ext2fs_get_mem(sizeof(struct ext2_struct_u32_list), &bb);
	ако (retval)
		върни retval;
	memset(bb, 0, sizeof(struct ext2_struct_u32_list));
	bb->magic = EXT2_ET_MAGIC_BADBLOCKS_LIST;
	bb->size = size ? size : 10;
	bb->num = num;
	retval = ext2fs_get_array(bb->size, sizeof(blk_t), &bb->list);
	ако (retval) {
		ext2fs_free_mem(&bb);
		върни retval;
	}
	ако (list)
		memcpy(bb->list, list, bb->size * sizeof(blk_t));
	иначе
		memset(bb->list, 0, bb->size * sizeof(blk_t));
	*ret = bb;
	върни 0;
}


/*
 * This procedure creates an empty u32 list.
 */
errcode_t ext2fs_u32_list_create(ext2_u32_list *ret, цяло size)
{
	върни make_u32_list(size, 0, 0, ret);
}

/*
 * This procedure creates an empty badblocks list.
 */
errcode_t ext2fs_badblocks_list_create(ext2_badblocks_list *ret, цяло size)
{
	върни make_u32_list(size, 0, 0, (ext2_badblocks_list *) ret);
}


/*
 * This procedure copies a badblocks list
 */
errcode_t ext2fs_u32_copy(ext2_u32_list src, ext2_u32_list *dest)
{
	errcode_t	retval;

	retval = make_u32_list(src->size, src->num, src->list, dest);
	ако (retval)
		върни retval;
	(*dest)->badblocks_flags = src->badblocks_flags;
	върни 0;
}

errcode_t ext2fs_badblocks_copy(ext2_badblocks_list src,
				ext2_badblocks_list *dest)
{
	върни ext2fs_u32_copy((ext2_u32_list) src,
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
	цяло		i, j;
	unsigned long	old_size;

	EXT2_CHECK_MAGIC(bb, EXT2_ET_MAGIC_BADBLOCKS_LIST);

	ако (bb->num >= bb->size) {
		old_size = bb->size * sizeof(__u32);
		bb->size += 100;
		retval = ext2fs_resize_mem(old_size, bb->size * sizeof(__u32),
					   &bb->list);
		ако (retval) {
			bb->size -= 100;
			върни retval;
		}
	}

	/*
	 * Add special case code for appending to the end of the list
	 */
	i = bb->num-1;
	ако ((bb->num != 0) && (bb->list[i] == blk))
		върни 0;
	ако ((bb->num == 0) || (bb->list[i] < blk)) {
		bb->list[bb->num++] = blk;
		върни 0;
	}

	j = bb->num;
	за (i=0; i < bb->num; i++) {
		ако (bb->list[i] == blk)
			върни 0;
		ако (bb->list[i] > blk) {
			j = i;
			break;
		}
	}
	за (i=bb->num; i > j; i--)
		bb->list[i] = bb->list[i-1];
	bb->list[j] = blk;
	bb->num++;
	върни 0;
}

errcode_t ext2fs_badblocks_list_add(ext2_badblocks_list bb, blk_t blk)
{
	върни ext2fs_u32_list_add((ext2_u32_list) bb, (__u32) blk);
}

/*
 * This procedure finds a particular block is on a badblocks
 * list.
 */
цяло ext2fs_u32_list_find(ext2_u32_list bb, __u32 blk)
{
	цяло	low, high, mid;

	ако (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		върни -1;

	ако (bb->num == 0)
		върни -1;

	low = 0;
	high = bb->num-1;
	ако (blk == bb->list[low])
		върни low;
	ако (blk == bb->list[high])
		върни high;

	докато (low < high) {
		mid = ((unsigned)low + (unsigned)high)/2;
		ако (mid == low || mid == high)
			break;
		ако (blk == bb->list[mid])
			върни mid;
		ако (blk < bb->list[mid])
			high = mid;
		иначе
			low = mid;
	}
	върни -1;
}

/*
 * This procedure tests to see if a particular block is on a badblocks
 * list.
 */
цяло ext2fs_u32_list_test(ext2_u32_list bb, __u32 blk)
{
	ако (ext2fs_u32_list_find(bb, blk) < 0)
		върни 0;
	иначе
		върни 1;
}

цяло ext2fs_badblocks_list_test(ext2_badblocks_list bb, blk_t blk)
{
	върни ext2fs_u32_list_test((ext2_u32_list) bb, (__u32) blk);
}


/*
 * Remove a block from the badblock list
 */
цяло ext2fs_u32_list_del(ext2_u32_list bb, __u32 blk)
{
	цяло	remloc, i;

	ако (bb->num == 0)
		върни -1;

	remloc = ext2fs_u32_list_find(bb, blk);
	ако (remloc < 0)
		върни -1;

	за (i = remloc ; i < bb->num-1; i++)
		bb->list[i] = bb->list[i+1];
	bb->num--;
	върни 0;
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
		върни retval;

	iter->magic = EXT2_ET_MAGIC_BADBLOCKS_ITERATE;
	iter->bb = bb;
	iter->ptr = 0;
	*ret = iter;
	върни 0;
}

errcode_t ext2fs_badblocks_list_iterate_begin(ext2_badblocks_list bb,
					      ext2_badblocks_iterate *ret)
{
	върни ext2fs_u32_list_iterate_begin((ext2_u32_list) bb,
					      (ext2_u32_iterate *) ret);
}


цяло ext2fs_u32_list_iterate(ext2_u32_iterate iter, __u32 *blk)
{
	ext2_u32_list	bb;

	ако (iter->magic != EXT2_ET_MAGIC_BADBLOCKS_ITERATE)
		върни 0;

	bb = iter->bb;

	ако (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		върни 0;

	ако (iter->ptr < bb->num) {
		*blk = bb->list[iter->ptr++];
		върни 1;
	}
	*blk = 0;
	върни 0;
}

цяло ext2fs_badblocks_list_iterate(ext2_badblocks_iterate iter, blk_t *blk)
{
	върни ext2fs_u32_list_iterate((ext2_u32_iterate) iter,
				       (__u32 *) blk);
}


void ext2fs_u32_list_iterate_end(ext2_u32_iterate iter)
{
	ако (!iter || (iter->magic != EXT2_ET_MAGIC_BADBLOCKS_ITERATE))
		върни;

	iter->bb = 0;
	ext2fs_free_mem(&iter);
}

void ext2fs_badblocks_list_iterate_end(ext2_badblocks_iterate iter)
{
	ext2fs_u32_list_iterate_end((ext2_u32_iterate) iter);
}


цяло ext2fs_u32_list_equal(ext2_u32_list bb1, ext2_u32_list bb2)
{
	EXT2_CHECK_MAGIC(bb1, EXT2_ET_MAGIC_BADBLOCKS_LIST);
	EXT2_CHECK_MAGIC(bb2, EXT2_ET_MAGIC_BADBLOCKS_LIST);

	ако (bb1->num != bb2->num)
		върни 0;

	ако (memcmp(bb1->list, bb2->list, bb1->num * sizeof(blk_t)) != 0)
		върни 0;
	върни 1;
}

цяло ext2fs_badblocks_equal(ext2_badblocks_list bb1, ext2_badblocks_list bb2)
{
	върни ext2fs_u32_list_equal((ext2_u32_list) bb1,
				     (ext2_u32_list) bb2);
}

цяло ext2fs_u32_list_count(ext2_u32_list bb)
{
	върни bb->num;
}
