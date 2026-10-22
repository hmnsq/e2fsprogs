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
#නම් HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <fcntl.h>
#include <time.h>
#නම් HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#නම් HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif

#include "ext2_fs.h"
#include "ext2fsP.h"

/*
 * Helper function for making a badblocks list
 */
static errcode_t make_u32_list(පූර්ණ_සංඛ්‍යා size, පූර්ණ_සංඛ්‍යා num, __u32 *list,
			       ext2_u32_list *ret)
{
	ext2_u32_list	bb;
	errcode_t	retval;

	retval = ext2fs_get_mem(parimaa_na(sa_nracanaa ext2_struct_u32_list), &bb);
	නම් (retval)
		ආපසු retval;
	memset(bb, 0, parimaa_na(sa_nracanaa ext2_struct_u32_list));
	bb->magic = EXT2_ET_MAGIC_BADBLOCKS_LIST;
	bb->size = size ? size : 10;
	bb->num = num;
	retval = ext2fs_get_array(bb->size, parimaa_na(blk_t), &bb->list);
	නම් (retval) {
		ext2fs_free_mem(&bb);
		ආපසු retval;
	}
	නම් (list)
		memcpy(bb->list, list, bb->size * parimaa_na(blk_t));
	නැතහොත්
		memset(bb->list, 0, bb->size * parimaa_na(blk_t));
	*ret = bb;
	ආපසු 0;
}


/*
 * This procedure creates an empty u32 list.
 */
errcode_t ext2fs_u32_list_create(ext2_u32_list *ret, පූර්ණ_සංඛ්‍යා size)
{
	ආපසු make_u32_list(size, 0, 0, ret);
}

/*
 * This procedure creates an empty badblocks list.
 */
errcode_t ext2fs_badblocks_list_create(ext2_badblocks_list *ret, පූර්ණ_සංඛ්‍යා size)
{
	ආපසු make_u32_list(size, 0, 0, (ext2_badblocks_list *) ret);
}


/*
 * This procedure copies a badblocks list
 */
errcode_t ext2fs_u32_copy(ext2_u32_list src, ext2_u32_list *dest)
{
	errcode_t	retval;

	retval = make_u32_list(src->size, src->num, src->list, dest);
	නම් (retval)
		ආපසු retval;
	(*dest)->badblocks_flags = src->badblocks_flags;
	ආපසු 0;
}

errcode_t ext2fs_badblocks_copy(ext2_badblocks_list src,
				ext2_badblocks_list *dest)
{
	ආපසු ext2fs_u32_copy((ext2_u32_list) src,
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
	පූර්ණ_සංඛ්‍යා		i, j;
	unsigned long	old_size;

	EXT2_CHECK_MAGIC(bb, EXT2_ET_MAGIC_BADBLOCKS_LIST);

	නම් (bb->num >= bb->size) {
		old_size = bb->size * parimaa_na(__u32);
		bb->size += 100;
		retval = ext2fs_resize_mem(old_size, bb->size * parimaa_na(__u32),
					   &bb->list);
		නම් (retval) {
			bb->size -= 100;
			ආපසු retval;
		}
	}

	/*
	 * Add special case code for appending to the end of the list
	 */
	i = bb->num-1;
	නම් ((bb->num != 0) && (bb->list[i] == blk))
		ආපසු 0;
	නම් ((bb->num == 0) || (bb->list[i] < blk)) {
		bb->list[bb->num++] = blk;
		ආපසු 0;
	}

	j = bb->num;
	සඳහා (i=0; i < bb->num; i++) {
		නම් (bb->list[i] == blk)
			ආපසු 0;
		නම් (bb->list[i] > blk) {
			j = i;
			bha_nga;
		}
	}
	සඳහා (i=bb->num; i > j; i--)
		bb->list[i] = bb->list[i-1];
	bb->list[j] = blk;
	bb->num++;
	ආපසු 0;
}

errcode_t ext2fs_badblocks_list_add(ext2_badblocks_list bb, blk_t blk)
{
	ආපසු ext2fs_u32_list_add((ext2_u32_list) bb, (__u32) blk);
}

/*
 * This procedure finds a particular block is on a badblocks
 * list.
 */
පූර්ණ_සංඛ්‍යා ext2fs_u32_list_find(ext2_u32_list bb, __u32 blk)
{
	පූර්ණ_සංඛ්‍යා	low, high, mid;

	නම් (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		ආපසු -1;

	නම් (bb->num == 0)
		ආපසු -1;

	low = 0;
	high = bb->num-1;
	නම් (blk == bb->list[low])
		ආපසු low;
	නම් (blk == bb->list[high])
		ආපසු high;

	අතරතුර (low < high) {
		mid = ((unsigned)low + (unsigned)high)/2;
		නම් (mid == low || mid == high)
			bha_nga;
		නම් (blk == bb->list[mid])
			ආපසු mid;
		නම් (blk < bb->list[mid])
			high = mid;
		නැතහොත්
			low = mid;
	}
	ආපසු -1;
}

/*
 * This procedure tests to see if a particular block is on a badblocks
 * list.
 */
පූර්ණ_සංඛ්‍යා ext2fs_u32_list_test(ext2_u32_list bb, __u32 blk)
{
	නම් (ext2fs_u32_list_find(bb, blk) < 0)
		ආපසු 0;
	නැතහොත්
		ආපසු 1;
}

පූර්ණ_සංඛ්‍යා ext2fs_badblocks_list_test(ext2_badblocks_list bb, blk_t blk)
{
	ආපසු ext2fs_u32_list_test((ext2_u32_list) bb, (__u32) blk);
}


/*
 * Remove a block from the badblock list
 */
පූර්ණ_සංඛ්‍යා ext2fs_u32_list_del(ext2_u32_list bb, __u32 blk)
{
	පූර්ණ_සංඛ්‍යා	remloc, i;

	නම් (bb->num == 0)
		ආපසු -1;

	remloc = ext2fs_u32_list_find(bb, blk);
	නම් (remloc < 0)
		ආපසු -1;

	සඳහා (i = remloc ; i < bb->num-1; i++)
		bb->list[i] = bb->list[i+1];
	bb->num--;
	ආපසු 0;
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

	retval = ext2fs_get_mem(parimaa_na(sa_nracanaa ext2_struct_u32_iterate), &iter);
	නම් (retval)
		ආපසු retval;

	iter->magic = EXT2_ET_MAGIC_BADBLOCKS_ITERATE;
	iter->bb = bb;
	iter->ptr = 0;
	*ret = iter;
	ආපසු 0;
}

errcode_t ext2fs_badblocks_list_iterate_begin(ext2_badblocks_list bb,
					      ext2_badblocks_iterate *ret)
{
	ආපසු ext2fs_u32_list_iterate_begin((ext2_u32_list) bb,
					      (ext2_u32_iterate *) ret);
}


පූර්ණ_සංඛ්‍යා ext2fs_u32_list_iterate(ext2_u32_iterate iter, __u32 *blk)
{
	ext2_u32_list	bb;

	නම් (iter->magic != EXT2_ET_MAGIC_BADBLOCKS_ITERATE)
		ආපසු 0;

	bb = iter->bb;

	නම් (bb->magic != EXT2_ET_MAGIC_BADBLOCKS_LIST)
		ආපසු 0;

	නම් (iter->ptr < bb->num) {
		*blk = bb->list[iter->ptr++];
		ආපසු 1;
	}
	*blk = 0;
	ආපසු 0;
}

පූර්ණ_සංඛ්‍යා ext2fs_badblocks_list_iterate(ext2_badblocks_iterate iter, blk_t *blk)
{
	ආපසු ext2fs_u32_list_iterate((ext2_u32_iterate) iter,
				       (__u32 *) blk);
}


void ext2fs_u32_list_iterate_end(ext2_u32_iterate iter)
{
	නම් (!iter || (iter->magic != EXT2_ET_MAGIC_BADBLOCKS_ITERATE))
		ආපසු;

	iter->bb = 0;
	ext2fs_free_mem(&iter);
}

void ext2fs_badblocks_list_iterate_end(ext2_badblocks_iterate iter)
{
	ext2fs_u32_list_iterate_end((ext2_u32_iterate) iter);
}


පූර්ණ_සංඛ්‍යා ext2fs_u32_list_equal(ext2_u32_list bb1, ext2_u32_list bb2)
{
	EXT2_CHECK_MAGIC(bb1, EXT2_ET_MAGIC_BADBLOCKS_LIST);
	EXT2_CHECK_MAGIC(bb2, EXT2_ET_MAGIC_BADBLOCKS_LIST);

	නම් (bb1->num != bb2->num)
		ආපසු 0;

	නම් (memcmp(bb1->list, bb2->list, bb1->num * parimaa_na(blk_t)) != 0)
		ආපසු 0;
	ආපසු 1;
}

පූර්ණ_සංඛ්‍යා ext2fs_badblocks_equal(ext2_badblocks_list bb1, ext2_badblocks_list bb2)
{
	ආපසු ext2fs_u32_list_equal((ext2_u32_list) bb1,
				     (ext2_u32_list) bb2);
}

පූර්ණ_සංඛ්‍යා ext2fs_u32_list_count(ext2_u32_list bb)
{
	ආපසු bb->num;
}
