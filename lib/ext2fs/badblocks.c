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
#если HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>
#include <time.h>
#если HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#если HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include "ext2_fs.h"
#include "ext2fsP.h"

/*
 * Helper function for making a badblocks list
 */
static errcode_t make_u32_list(целое size, целое num, __u32 *list,
			       ext2_u32_list *ret)
{
	ext2_u32_list	bb;
	errcode_t	retval;

	retval = ext2fs_get_mem(sizeof(struct ext2_struct_u32_list), &bb);
	если (retval)
		вернуть retval;
	memset(bb, 0, sizeof(struct ext2_struct_u32_list));
	bb->magic = EXT2_ET_MAGIC_BADBLOCKS_LIST;
	bb->size = size ? size : 10;
	bb->num = num;
	retval = ext2fs_get_array(bb->size, sizeof(blk_t), &bb->list);
	если (retval) {
		ext2fs_free_mem(&bb);
		вернуть retval;
	}
	если (list)
		memcpy(bb->list, list, bb->size * sizeof(blk_t));
	иначе
		memset(bb->list, 0, bb->size * sizeof(blk_t));
	*ret = bb;
	вернуть 0;
}


/*
 * This procedure creates an empty u32 list.
 */
errcode_t ext2fs_u32_list_create(ext2_u32_list *ret, целое size)
{
	вернуть make_u32_list(size, 0, 0, ret);
}

/*
 * This procedure creates an empty badblocks list.
 */
errcode_t ext2fs_badblocks_list_create(ext2_badblocks_list *ret, целое size)
{
	вернуть make_u32_list(size, 0, 0, (ext2_badblocks_list *) ret);
}


/*
 * This procedure copies a badblocks list
 */
errcode_t ext2fs_u32_copy(ext2_u32_list src, ext2_u32_list *dest)
{
	errcode_t	retval;

	retval = make_u32_list(src->size, src->num, src->list, dest);
	если (retval)
		вернуть retval;
	(*dest)->badblocks_flags = src->badblocks_flags;
	вернуть 0;
}

errcode_t ext2fs_badblocks_copy(ext2_badblocks_list src,
				ext2_badblocks_list *dest)
{
	вернуть ext2fs_u32_copy((ext2_u32_list) src,
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
	целое		i, j;
	unsigned long	old_size;

	EXT2_CHECK_MAGIC(bb, EXT2_ET_MAGIC_BADBLOCKS_LIST);

	если (bb->num >= bb->size) {
		old_size = bb->size * sizeof(__u32);
		bb->size += 100;
		retval = ext2fs_resize_mem(old_size, bb->size * sizeof(__u32),
					   &bb->list);
		если (retval) {
			bb->size -= 100;
			вернуть retval;
		}
	}

	/*
	 * Add special case code for appending to the end of the list
	 */
	i = bb->num-1;
	если ((bb->num != 0) && (bb->list[i] == blk))
		вернуть 0;
	если ((bb->num == 0) || (bb->list[i] < blk)) {
		bb->list[bb->num++] = blk;
		вернуть 0;
	}

	j = bb->num;
	для (i=0; i < bb->num; i++) {
		если (bb->list[i] == blk)
			вернуть 0;
		если (bb->list[i] > blk) {
			j = i;
			break;
		}
	}
	для (i=bb->num; i > j; i--)
		bb->list[i] = bb->list[i-1];
	bb->list[j] = blk;
	bb->num++;
	вернуть 0;
}

errcode_t ext2fs_badblocks_list_add(ext2_badblocks_list bb, blk_t blk)
{
	вернуть ext2fs_u32_list_add((ext2_u32_list) bb, (__u32) blk);
}

/*
 * This procedure finds a particular block is on a badblocks
 * list.
 */
целое ext2fs_u32_list_find(ext2_u32_list bb, __u32 blk)
{
	целое	low, high, mid;

	если (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		вернуть -1;

	если (bb->num == 0)
		вернуть -1;

	low = 0;
	high = bb->num-1;
	если (blk == bb->list[low])
		вернуть low;
	если (blk == bb->list[high])
		вернуть high;

	пока (low < high) {
		mid = ((unsigned)low + (unsigned)high)/2;
		если (mid == low || mid == high)
			break;
		если (blk == bb->list[mid])
			вернуть mid;
		если (blk < bb->list[mid])
			high = mid;
		иначе
			low = mid;
	}
	вернуть -1;
}

/*
 * This procedure tests to see if a particular block is on a badblocks
 * list.
 */
целое ext2fs_u32_list_test(ext2_u32_list bb, __u32 blk)
{
	если (ext2fs_u32_list_find(bb, blk) < 0)
		вернуть 0;
	иначе
		вернуть 1;
}

целое ext2fs_badblocks_list_test(ext2_badblocks_list bb, blk_t blk)
{
	вернуть ext2fs_u32_list_test((ext2_u32_list) bb, (__u32) blk);
}


/*
 * Remove a block from the badblock list
 */
целое ext2fs_u32_list_del(ext2_u32_list bb, __u32 blk)
{
	целое	remloc, i;

	если (bb->num == 0)
		вернуть -1;

	remloc = ext2fs_u32_list_find(bb, blk);
	если (remloc < 0)
		вернуть -1;

	для (i = remloc ; i < bb->num-1; i++)
		bb->list[i] = bb->list[i+1];
	bb->num--;
	вернуть 0;
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
	если (retval)
		вернуть retval;

	iter->magic = EXT2_ET_MAGIC_BADBLOCKS_ITERATE;
	iter->bb = bb;
	iter->ptr = 0;
	*ret = iter;
	вернуть 0;
}

errcode_t ext2fs_badblocks_list_iterate_begin(ext2_badblocks_list bb,
					      ext2_badblocks_iterate *ret)
{
	вернуть ext2fs_u32_list_iterate_begin((ext2_u32_list) bb,
					      (ext2_u32_iterate *) ret);
}


целое ext2fs_u32_list_iterate(ext2_u32_iterate iter, __u32 *blk)
{
	ext2_u32_list	bb;

	если (iter->magic != EXT2_ET_MAGIC_BADBLOCKS_ITERATE)
		вернуть 0;

	bb = iter->bb;

	если (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		вернуть 0;

	если (iter->ptr < bb->num) {
		*blk = bb->list[iter->ptr++];
		вернуть 1;
	}
	*blk = 0;
	вернуть 0;
}

целое ext2fs_badblocks_list_iterate(ext2_badblocks_iterate iter, blk_t *blk)
{
	вернуть ext2fs_u32_list_iterate((ext2_u32_iterate) iter,
				       (__u32 *) blk);
}


void ext2fs_u32_list_iterate_end(ext2_u32_iterate iter)
{
	если (!iter || (iter->magic != EXT2_ET_MAGIC_BADBLOCKS_ITERATE))
		вернуть;

	iter->bb = 0;
	ext2fs_free_mem(&iter);
}

void ext2fs_badblocks_list_iterate_end(ext2_badblocks_iterate iter)
{
	ext2fs_u32_list_iterate_end((ext2_u32_iterate) iter);
}


целое ext2fs_u32_list_equal(ext2_u32_list bb1, ext2_u32_list bb2)
{
	EXT2_CHECK_MAGIC(bb1, EXT2_ET_MAGIC_BADBLOCKS_LIST);
	EXT2_CHECK_MAGIC(bb2, EXT2_ET_MAGIC_BADBLOCKS_LIST);

	если (bb1->num != bb2->num)
		вернуть 0;

	если (memcmp(bb1->list, bb2->list, bb1->num * sizeof(blk_t)) != 0)
		вернуть 0;
	вернуть 1;
}

целое ext2fs_badblocks_equal(ext2_badblocks_list bb1, ext2_badblocks_list bb2)
{
	вернуть ext2fs_u32_list_equal((ext2_u32_list) bb1,
				     (ext2_u32_list) bb2);
}

целое ext2fs_u32_list_count(ext2_u32_list bb)
{
	вернуть bb->num;
}
