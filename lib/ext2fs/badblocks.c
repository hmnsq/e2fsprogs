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
#만약 HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>
#include <time.h>
#만약 HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#만약 HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include "ext2_fs.h"
#include "ext2fsP.h"

/*
 * Helper function for making a badblocks list
 */
static errcode_t make_u32_list(정수 size, 정수 num, __u32 *list,
			       ext2_u32_list *ret)
{
	ext2_u32_list	bb;
	errcode_t	retval;

	retval = ext2fs_get_mem(sizeof(struct ext2_struct_u32_list), &bb);
	만약 (retval)
		반환 retval;
	memset(bb, 0, sizeof(struct ext2_struct_u32_list));
	bb->magic = EXT2_ET_MAGIC_BADBLOCKS_LIST;
	bb->size = size ? size : 10;
	bb->num = num;
	retval = ext2fs_get_array(bb->size, sizeof(blk_t), &bb->list);
	만약 (retval) {
		ext2fs_free_mem(&bb);
		반환 retval;
	}
	만약 (list)
		memcpy(bb->list, list, bb->size * sizeof(blk_t));
	아니면
		memset(bb->list, 0, bb->size * sizeof(blk_t));
	*ret = bb;
	반환 0;
}


/*
 * This procedure creates an empty u32 list.
 */
errcode_t ext2fs_u32_list_create(ext2_u32_list *ret, 정수 size)
{
	반환 make_u32_list(size, 0, 0, ret);
}

/*
 * This procedure creates an empty badblocks list.
 */
errcode_t ext2fs_badblocks_list_create(ext2_badblocks_list *ret, 정수 size)
{
	반환 make_u32_list(size, 0, 0, (ext2_badblocks_list *) ret);
}


/*
 * This procedure copies a badblocks list
 */
errcode_t ext2fs_u32_copy(ext2_u32_list src, ext2_u32_list *dest)
{
	errcode_t	retval;

	retval = make_u32_list(src->size, src->num, src->list, dest);
	만약 (retval)
		반환 retval;
	(*dest)->badblocks_flags = src->badblocks_flags;
	반환 0;
}

errcode_t ext2fs_badblocks_copy(ext2_badblocks_list src,
				ext2_badblocks_list *dest)
{
	반환 ext2fs_u32_copy((ext2_u32_list) src,
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
	정수		i, j;
	unsigned long	old_size;

	EXT2_CHECK_MAGIC(bb, EXT2_ET_MAGIC_BADBLOCKS_LIST);

	만약 (bb->num >= bb->size) {
		old_size = bb->size * sizeof(__u32);
		bb->size += 100;
		retval = ext2fs_resize_mem(old_size, bb->size * sizeof(__u32),
					   &bb->list);
		만약 (retval) {
			bb->size -= 100;
			반환 retval;
		}
	}

	/*
	 * Add special case code for appending to the end of the list
	 */
	i = bb->num-1;
	만약 ((bb->num != 0) && (bb->list[i] == blk))
		반환 0;
	만약 ((bb->num == 0) || (bb->list[i] < blk)) {
		bb->list[bb->num++] = blk;
		반환 0;
	}

	j = bb->num;
	위해 (i=0; i < bb->num; i++) {
		만약 (bb->list[i] == blk)
			반환 0;
		만약 (bb->list[i] > blk) {
			j = i;
			break;
		}
	}
	위해 (i=bb->num; i > j; i--)
		bb->list[i] = bb->list[i-1];
	bb->list[j] = blk;
	bb->num++;
	반환 0;
}

errcode_t ext2fs_badblocks_list_add(ext2_badblocks_list bb, blk_t blk)
{
	반환 ext2fs_u32_list_add((ext2_u32_list) bb, (__u32) blk);
}

/*
 * This procedure finds a particular block is on a badblocks
 * list.
 */
정수 ext2fs_u32_list_find(ext2_u32_list bb, __u32 blk)
{
	정수	low, high, mid;

	만약 (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		반환 -1;

	만약 (bb->num == 0)
		반환 -1;

	low = 0;
	high = bb->num-1;
	만약 (blk == bb->list[low])
		반환 low;
	만약 (blk == bb->list[high])
		반환 high;

	동안 (low < high) {
		mid = ((unsigned)low + (unsigned)high)/2;
		만약 (mid == low || mid == high)
			break;
		만약 (blk == bb->list[mid])
			반환 mid;
		만약 (blk < bb->list[mid])
			high = mid;
		아니면
			low = mid;
	}
	반환 -1;
}

/*
 * This procedure tests to see if a particular block is on a badblocks
 * list.
 */
정수 ext2fs_u32_list_test(ext2_u32_list bb, __u32 blk)
{
	만약 (ext2fs_u32_list_find(bb, blk) < 0)
		반환 0;
	아니면
		반환 1;
}

정수 ext2fs_badblocks_list_test(ext2_badblocks_list bb, blk_t blk)
{
	반환 ext2fs_u32_list_test((ext2_u32_list) bb, (__u32) blk);
}


/*
 * Remove a block from the badblock list
 */
정수 ext2fs_u32_list_del(ext2_u32_list bb, __u32 blk)
{
	정수	remloc, i;

	만약 (bb->num == 0)
		반환 -1;

	remloc = ext2fs_u32_list_find(bb, blk);
	만약 (remloc < 0)
		반환 -1;

	위해 (i = remloc ; i < bb->num-1; i++)
		bb->list[i] = bb->list[i+1];
	bb->num--;
	반환 0;
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
	만약 (retval)
		반환 retval;

	iter->magic = EXT2_ET_MAGIC_BADBLOCKS_ITERATE;
	iter->bb = bb;
	iter->ptr = 0;
	*ret = iter;
	반환 0;
}

errcode_t ext2fs_badblocks_list_iterate_begin(ext2_badblocks_list bb,
					      ext2_badblocks_iterate *ret)
{
	반환 ext2fs_u32_list_iterate_begin((ext2_u32_list) bb,
					      (ext2_u32_iterate *) ret);
}


정수 ext2fs_u32_list_iterate(ext2_u32_iterate iter, __u32 *blk)
{
	ext2_u32_list	bb;

	만약 (iter->magic != EXT2_ET_MAGIC_BADBLOCKS_ITERATE)
		반환 0;

	bb = iter->bb;

	만약 (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		반환 0;

	만약 (iter->ptr < bb->num) {
		*blk = bb->list[iter->ptr++];
		반환 1;
	}
	*blk = 0;
	반환 0;
}

정수 ext2fs_badblocks_list_iterate(ext2_badblocks_iterate iter, blk_t *blk)
{
	반환 ext2fs_u32_list_iterate((ext2_u32_iterate) iter,
				       (__u32 *) blk);
}


void ext2fs_u32_list_iterate_end(ext2_u32_iterate iter)
{
	만약 (!iter || (iter->magic != EXT2_ET_MAGIC_BADBLOCKS_ITERATE))
		반환;

	iter->bb = 0;
	ext2fs_free_mem(&iter);
}

void ext2fs_badblocks_list_iterate_end(ext2_badblocks_iterate iter)
{
	ext2fs_u32_list_iterate_end((ext2_u32_iterate) iter);
}


정수 ext2fs_u32_list_equal(ext2_u32_list bb1, ext2_u32_list bb2)
{
	EXT2_CHECK_MAGIC(bb1, EXT2_ET_MAGIC_BADBLOCKS_LIST);
	EXT2_CHECK_MAGIC(bb2, EXT2_ET_MAGIC_BADBLOCKS_LIST);

	만약 (bb1->num != bb2->num)
		반환 0;

	만약 (memcmp(bb1->list, bb2->list, bb1->num * sizeof(blk_t)) != 0)
		반환 0;
	반환 1;
}

정수 ext2fs_badblocks_equal(ext2_badblocks_list bb1, ext2_badblocks_list bb2)
{
	반환 ext2fs_u32_list_equal((ext2_u32_list) bb1,
				     (ext2_u32_list) bb2);
}

정수 ext2fs_u32_list_count(ext2_u32_list bb)
{
	반환 bb->num;
}
